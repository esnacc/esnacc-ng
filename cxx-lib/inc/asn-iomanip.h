#ifndef __ASN_IOMANIP_H__
#define __ASN_IOMANIP_H__

#include "asn-config.h"
#include <iostream>

_BEGIN_SNACC_NAMESPACE

inline int
getSNACCEncoderIOSType()
{
    static int iDelimIdx = std::ios_base::xalloc();
    return iDelimIdx;
}

static inline std::ios_base&
SNACC_setiosencodetype(std::ios_base &s, SNACCEncodeDecodeRules t)
{
    s.iword(getSNACCEncoderIOSType()) = (int)t;
    return s;
}

static inline SNACCEncodeDecodeRules
SNACC_getiosencodetype(std::ios_base &s)
{
    return (SNACCEncodeDecodeRules)s.iword(getSNACCEncoderIOSType());
}

static inline std::ios_base &
EncodeBER(std::ios_base &s)
{
    return SNACC_setiosencodetype(s, BER);
}

static inline std::ios_base &
EncodeNORMAL(std::ios_base &s)
{
    return SNACC_setiosencodetype(s, SNACC_ASCII);
}

static inline std::ios_base &
EncodePER(std::ios_base &s)
{
    return SNACC_setiosencodetype(s, PER);
}

_END_SNACC_NAMESPACE

// Overload of operator<< to stream out an AsnType
SNACCDLL_API std::ostream& operator<<(std::ostream& os,
                                      const SNACC::AsnType& a);

// Overload of operator>> to stream into an AsnType
SNACCDLL_API std::istream& operator>>(std::istream& is, SNACC::AsnType& a);

#endif
