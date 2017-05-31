/* asn-rose.cxx: Remote Operations Service Element support
 *
 * Copyright (C) 2017, Aaron Conole
 *
 * Dual licensed under both the esnacc public license, and the
 * Lesser GNU Public License version 3 (lgpl v3).
 *
 * Portions of this file were contributed by estos GmbH, www.estos.com
 */

#include "asn-rose.h"
#include "snaccrose.h"

using namespace SNACC;

static uint64_t mhash64(const void *k, size_t l, uint64_t s)
{
    const uint64_t m = ESNACC_CONST_ULL(0xc6a4a7935bd1e995);
    const int r = 47;
    uint64_t h = s ^ (l * m);
    uint64_t dval = *(const uint64_t *)k;
    uint64_t *data;
    for (data = (uint64_t *)k;
         data != ((uint64_t *)k + (l / 8)); dval = *data++) {
        dval *= m;
        dval ^= dval >> r;
        dval *= m;

        h ^= dval;
        h *= m;
    }

    const unsigned char * data2 = (const unsigned char*)data;

    switch(l & 7) {
    case 7: h ^= uint64_t(data2[6]) << 48;
    case 6: h ^= uint64_t(data2[5]) << 40;
    case 5: h ^= uint64_t(data2[4]) << 32;
    case 4: h ^= uint64_t(data2[3]) << 24;
    case 3: h ^= uint64_t(data2[2]) << 16;
    case 2: h ^= uint64_t(data2[1]) << 8;
    case 1: h ^= uint64_t(data2[0]);
        h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

class InitialMonitoredStreamCB : public StreambufCallback
{
    OperationCoordinator &m_parent;

    static inline AsnLen DecodeMsg(std::streambuf *stream,
                                   SNACCEncodeDecodeRules r, ROSEMessage &rm)
    {
        AsnBuf buf(stream); AsnLen l;
        switch (r) {
        case BER: {
            rm.BDec(buf, l);
        }
        break;

        case PER:
        default:
            throw SnaccException(__FILE__, __LINE__, "DecodeMsg",
                                 "Unsupported encoding type");
            break;
        }
        return l;
    }

    StreambufCallback::Streambuf_CB_RESULT callback(Streambuf_CB_STATUS status,
                                                    std::streambuf *stream)
    {
        switch (status) {
        case StreambufCallback::CB_READ_OK: {
                try {
                    ROSEMessage rm;
                    DecodeMsg(stream, m_parent.StreambufEncodingType(stream),
                              rm);
                    m_parent.ReceiveMsg(rm);
                } catch (SnaccException &e) {
                    ROSEReject rejectMsg;
                    rejectMsg.invokedID.choiceId =
                        ROSERejectChoice::invokednullCid;
                    rejectMsg.invokedID.invokednull = new AsnNull;
                    ROSEMessage m;
                    m.reject = new ROSEReject(rejectMsg);
                    m_parent.SendMsg(stream, m);
                }
        }
        break;
        case StreambufCallback::CB_WRITE_OK:
        case StreambufCallback::CB_OPENED:
            return StreambufCallback::CB_NONE;

        case StreambufCallback::CB_CLOSED:
        default:
            return StreambufCallback::CB_DROP_STREAM;
        }
        return StreambufCallback::CB_NONE;
    }

public:
    InitialMonitoredStreamCB(OperationCoordinator &p) :
        m_parent(p) {}

};

OperationCoordinator::OperationCoordinator() :
    nameMap(), operationMap(), operationIdMap(),
    cb(new InitialMonitoredStreamCB(*this))
{

}

#define OPERATION_SEED 0x5bd1e995

void OperationCoordinator::SetInvokeCallback(const std::string &s, InvokeCB i,
                                             uint64_t o)
{
    uint64_t hash = o;
    if (!hash) hash = mhash64(s.c_str(), s.length(), OPERATION_SEED);
    nameMap[s] = i;
    operationMap[i] = s;
    operationIdMap[hash] = i;
}

void OperationCoordinator::MonitorStreambuf(std::streambuf *b,
                                            SNACCEncodeDecodeRules r)
{
    monbufs.push(b, *cb, false);
    bufEncodeMap[b] = r;
}

void OperationCoordinator::RemoveStreambuf(std::streambuf *b)
{
    monbufs.pop(b);
    bufEncodeMap.erase(b);
}

bool OperationCoordinator::ReceiveInvoke(const ROSEInvoke *i)
{
    InvokeCB operationId;
    bool found = false;

    switch (i->operationID.choiceId) {
    case ROSEInvokeChoice1::localCid:
        operationId = FindInvoke(*i->operationID.local, found);
        break;
    case ROSEInvokeChoice1::nameCid:
        operationId = FindInvoke(std::string((*i->operationID.name).getAsUTF8()),
                                 found);
        break;
    default:
        return false;
    }

    if (!found || !operationId) {
        return false;
    }

    AsnInvokeCtx ctx;

    /* For the first cut, just invoke the operation inline, and return a
     * result */
    long result = operationId(i, &ctx);
    if (result) {
        return false;
    }

    return true;
}

bool OperationCoordinator::ReceiveResult(const ROSEResult *r)
{
    return false;
}

bool OperationCoordinator::ReceiveReject(const ROSEReject *r)
{
    return false;
}

bool OperationCoordinator::ReceiveError(const ROSEError *e)
{
    return false;
}

bool OperationCoordinator::ReceiveMsg(const ROSEMessage &m)
{
    bool result = false;
    switch(m.choiceId) {
    case ROSEMessage::invokeCid:
        if (!m.invoke) {
            throw SnaccException(__FILE__, __LINE__, "ReceiveMsg",
                                 "Empty invoke requested");
        }
        result = ReceiveInvoke(m.invoke);
        break;
    case ROSEMessage::resultCid:
        if (!m.result) {
            throw SnaccException(__FILE__, __LINE__, "ReceiveMsg",
                                 "Empty result returned");
        }
        result = ReceiveResult(m.result);
        break;
    case ROSEMessage::rejectCid:
        if (!m.reject) {
            throw SnaccException(__FILE__, __LINE__, "ReceiveMsg",
                                 "Empty reject returned");
        }
        result = ReceiveReject(m.reject);
        break;
    case ROSEMessage::errorCid:
        if (!m.error) {
            throw SnaccException(__FILE__, __LINE__, "ReceiveMsg",
                                 "Empty reject returned");
        }
        result = ReceiveError(m.error);
        break;
    }
    return result;
}

bool OperationCoordinator::SendMsg(std::streambuf *b, ROSEMessage &r)
{
    AsnBuf buf(b); AsnLen l;
    switch (StreambufEncodingType(b)) {
    case BER:
        l = r.BEnc(buf);
        break;
    case PER:
    default:
        throw SnaccException(__FILE__, __LINE__, "SendMsg",
                             "Unsupported encoding type");
        break;
    }
    return l;
}

SNACCEncodeDecodeRules OperationCoordinator::StreambufEncodingType(std::streambuf *b)
{
    return bufEncodeMap[b];
}
