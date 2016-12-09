# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Base type for eSNACC

from asn_buffer import AsnBuf
from asn_buffer import BDecDefLen
from asn_buffer import BEncDefLen

class Constraint(object):
    def __init__(self):
        pass

    def withinConstraint(self, value):
        raise NotImplementedError


class BERConsts(object):
    BER_ANY_CLASS = -2
    BER_NULL_CLASS = -1
    BER_UNIVERSAL_CLASS = 0
    BER_APPLICATION_CLASS = 1 << 6
    BER_CONTEXTUAL_CLASS = 2 << 6
    BER_PRIVATE_CLASS = 3 << 6

    UNIV = BER_UNIVERSAL_CLASS
    APPL = BER_APPLICATION_CLASS
    CNTX = BER_CONTEXTUAL_CLASS
    PRIV = BER_PRIVATE_CLASS

    BER_ANY_FORM = -2
    BER_NULL_FORM = -1
    BER_PRIMITIVE_FORM = 0
    BER_CONSTRUCTED_FORM = 1 << 5

    PRIM = BER_PRIMITIVE_FORM
    CONS = BER_CONSTRUCTED_FORM

    @staticmethod
    def MakeTag(cls, frm, tag):
        fnl = cls & 0xff
        fnl |= (frm & 0xff)

        def intcnv(f):
            return chr(int(f))

        if tag < 31:
            fnl |= tag & 0xff
            fnl = chr(fnl)
        else:
            tg = intcnv(tag & 0x7f)
            tag >>= 7
            while tag:
                tg = intcnv(0x80|(tag & 0x7f)) + tg
                tag >>= 7
            fnl = intcnv(fnl|0x1f) + tg

        return int(fnl.encode('hex'), 16)

    NO_TAG_CODE = 0
    BOOLEAN_TAG_CODE = 1
    INTEGER_TAG_CODE = 2
    BITSTRING_TAG_CODE = 3
    OCTETSTRING_TAG_CODE = 4
    NULLTYPE_TAG_CODE = 5
    OID_TAG_CODE = 6
    OD_TAG_CODE = 7
    EXTERNAL_TAG_CODE = 8
    REAL_TAG_CODE = 9
    ENUM_TAG_CODE = 10
    UTF8STRING_TAG_CODE = 12
    RELATIVE_OID_TAG_CODE = 13
    SEQ_TAG_CODE = 16
    SET_TAG_CODE = 17
    NUMERICSTRING_TAG_CODE = 18
    PRINTABLESTRING_TAG_CODE = 19
    TELETEXSTRING_TAG_CODE = 20
    VIDEOTEXSTRING_TAG_CODE = 21
    IA5STRING_TAG_CODE = 22
    UTCTIME_TAG_CODE = 23
    GENERALIZEDTIME_TAG_CODE = 24
    GRAPHICSTRING_TAG_CODE = 25
    VISIBLESTRING_TAG_CODE = 26
    GENERALSTRING_TAG_CODE = 27
    UNIVERSALSTRING_TAG_CODE = 28
    BMPSTRING_TAG_CODE = 30


class AsnBase(object):

    BER_CLASS = BERConsts.BER_UNIVERSAL_CLASS
    BER_FORM = BERConsts.BER_PRIMITIVE_FORM

    def __init__(self):
        pass

    def typename(self):
        raise NotImplementedError

    def passesAllConstraints(self, constraints):
        return True

    def addConstraint(self, constraint):
        try:
            self.constraints.append(constraint)
        except AttributeError:
            self.constraints = []
            self.constraints.append(constraint)

    def BEncContent(self, asnbuf):
        raise NotImplementedError

    def BDecContent(self, asnbuf, contentlen):
        raise NotImplementedError

    def BEnc(self, asnbuf):
        try:
            if len(self.constraints) > 0 and \
               not self.passesAllConstraints(self.constraints):
                raise ValueError("Not all constraints passed")
        except AttributeError:
            pass
        except TypeError:
            pass

        newbuf = AsnBuf()
        asnlen = self.BEncContent(newbuf)
        asnlen += BEncDefLen(newbuf, asnlen)
        TAG_CODE = BERConsts.MakeTag(self.BER_CLASS & 0xff,
                                     self.BER_FORM & 0xff,
                                     self.BER_TAG & 0xff)
        asnlen += newbuf.PutBufReverse(TAG_CODE)
        asnbuf.PutBuf(newbuf.buf)
        return asnlen

    def BDecTag(self, asnbuf, asnlen):
        al = 1
        bufTag = ord(asnbuf.Buffer()[0])
        lastTag = bufTag

        if (lastTag & 0x1f) != 0x1f:
            return bufTag, al

        lastTag = ord(asnbuf.Buffer()[al])
        while lastTag & 0x80:
            lastTag = ord(asnbuf.Buffer()[al])
            bufTag <<= 8
            bufTag |= lastTag
            al += 1
        return bufTag, al

    def BDec(self, asnbuf, asnlen):
        bufTag, al = self.BDecTag(asnbuf, asnlen)

        PRIM_TAG = BERConsts.MakeTag(self.BER_CLASS & 0xff,
                                     BERConsts.BER_PRIMITIVE_FORM & 0xff,
                                     self.BER_TAG & 0xff)
        CONS_TAG = BERConsts.MakeTag(self.BER_CLASS & 0xff,
                                     BERConsts.BER_CONSTRUCTED_FORM & 0xff,
                                     self.BER_TAG & 0xff)

        if bufTag != PRIM_TAG and bufTag != CONS_TAG:
            raise IOError("Invalid bytes 0x%x expected [0x%x|0x%x]" %
                          (bufTag, PRIM_TAG, CONS_TAG))

        asnbuf.swallow(al)
        asnlen += al
        tag_total_len, totalBytesLength = BDecDefLen(asnbuf)
        asnlen += tag_total_len
        asnlen += totalBytesLength
        self.BDecContent(asnbuf, totalBytesLength)
        return asnlen

    def BEncPdu(self, asnbuf, asnlen):
        try:
            asnlen = self.BEnc(asnbuf)
        except Exception:
            return False
        return True, asnlen

    def BDecPdu(self, asnbuf, asnlen):
        try:
            asnlen = self.BDec(asnbuf, asnlen)
        except Exception:
            return False
        return True, asnlen
