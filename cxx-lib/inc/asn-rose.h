/* asn-rose.h: Remote Operations Service Element support
 *
 * Copyright (C) 2017, Aaron Conole
 *
 * Dual licensed under both the esnacc public license, and the
 * Lesser GNU Public License version 3 (lgpl v3).
 *
 * Portions of this file were contributed by estos GmbH, www.estos.com
 */

#include <stdint.h>
#include <string>
#include <map>
#include <memory>
#include <streambuf>
#include <stdexcept>

#include "asn-incl.h"
#include "asn-buf.h"
#include "asn-iomanip.h"

#ifndef ESNACC_ASN_ROSE_H__
#define ESNACC_ASN_ROSE_H__

#define ROSE_TRANSPORT_ERROR(code) (code & 0x000F)
#define ROSE_RE(code)              (code & 0x00F0)
#define ROSE_REJECT(code)          (code & 0x0F00)
#define ROSE_ERROR(code)           (code & 0xF000)

/* fwd decl. for this - implementation detail.  */
class InitialMonitoredStreamCB;

namespace SNACC {

class AsnType;
class ROSEMessage;
class ROSEInvoke;
class ROSEResult;
class ROSEError;
class ROSEReject;
class InvokeProblem;
class ROSEAuthRequest;
class ROSEAuthResult;

static const uint32_t ROSE_TRANSPORT_FAILED   = 0x1;
static const uint32_t ROSE_TRANSPORT_SHUTDOWN = 0x2;
static const uint32_t ROSE_TRANSPORT_TIMEOUT  = 0x3;

static const uint32_t ROSE_RE_INVALID_ANSWER = 0x10;
static const uint32_t ROSE_RE_DECODE_ERROR   = 0x20;

static const uint32_t ROSE_REJECT_UNKNOWN_OPERATION = 0x100;
static const uint32_t ROSE_REJECT_MISTYPED_ARGUMENT = 0x200;
static const uint32_t ROSE_REJECT_FUNCTIONS_MISSING = 0x300;
static const uint32_t ROSE_REJECT_INVALID_CODE      = 0x400;
static const uint32_t ROSE_REJECT_INVALID_SESSION   = 0x500;
static const uint32_t ROSE_REJECT_START_SSL_REQUIRED        = 0x600;
static const uint32_t ROSE_REJECT_AUTHENTICATION_INCOMPLETE = 0x700;
static const uint32_t ROSE_REJECT_AUTHENTICATION_FAILED     = 0x800;

static const uint32_t ROSE_GENERIC_ERROR = 0x1000;

static const uint64_t ROSE_TIMEOUT_INF = ~0ULL;

enum SnaccInvokeResult {
    returnResult,
    returnError,
    returnReject
};

struct AsnInvokeCtx
{
    AsnInvokeCtx() : rejectResult(0), invokeAuthReq(0),
                     invokeAuthRes(0)
    {
    }

    ~AsnInvokeCtx();

    long rejectResult;
    ROSEAuthRequest *invokeAuthReq;
    ROSEAuthResult  *invokeAuthRes;
};

typedef long (*InvokeCB)(const ROSEInvoke *, AsnInvokeCtx *);

class OperationCoordinator
{
public:
    typedef std::map<std::string, InvokeCB> ByNameMap;
    typedef std::map<InvokeCB, std::string> ByOperationMap;
    typedef std::map<uint64_t, InvokeCB>    ByOpIdMap;

    OperationCoordinator();
    ~OperationCoordinator() { }

    void SetInvokeCallback(const std::string &s, InvokeCB i, uint64_t o = 0);

    InvokeCB FindInvoke(const std::string &s, bool &b) const
    {
        ByNameMap::const_iterator cit = nameMap.find(s);
        b = false;
        if (cit != nameMap.end()) {
            b = true;
            return cit->second;
        }
        return (InvokeCB)0;
    }

    InvokeCB FindInvoke(const uint64_t i, bool &b) const
    {
        ByOpIdMap::const_iterator cit = operationIdMap.find(i);
        b = false;
        if (cit != operationIdMap.end()) {
            b = true;
            return cit->second;
        }
        return (InvokeCB)0;
    }

    InvokeCB operator[](const std::string &s) const
    {
        InvokeCB i;
        bool b;

        i = FindInvoke(s, b);
        if (!b || !i) {
            throw std::range_error(std::string() +
                                   "Invalid operation requested from directory");
        }
        return i;
    }

    void MonitorStreambuf(std::streambuf *b, SNACCEncodeDecodeRules r);
    void RemoveStreambuf(std::streambuf *b);
    SNACCEncodeDecodeRules StreambufEncodingType(std::streambuf *b);

    void monitor()
    {
        monbufs();
    }

    bool ReceiveMsg(const ROSEMessage &m);
    bool SendMsg(std::streambuf *b, ROSEMessage &r);

protected:

    bool ReceiveInvoke(const ROSEInvoke *i);
    bool ReceiveResult(const ROSEResult *r);
    bool ReceiveReject(const ROSEReject *r);
    bool ReceiveError(const ROSEError *e);

    ByNameMap      nameMap;
    ByOperationMap operationMap;
    ByOpIdMap      operationIdMap;
    InitialMonitoredStreamCB *cb;
    StreambufMonitor monbufs;
    std::map<std::streambuf *, SNACCEncodeDecodeRules> bufEncodeMap;
};

}
#endif
