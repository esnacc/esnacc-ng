// file: .../c++-lib/src/print.C
//
// MS 92
// Copyright (C) 1992 Michael Sample and the University of British Columbia
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// If you modify this file, you must clearly indicate your changes.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// $Header: /baseline/SNACC/c++-lib/src/print.cpp,v 1.7 2004/02/11 19:08:48 nicholar Exp $
//

#include "asn-incl.h"
#include "asn-iomanip.h"

void SNACC::Indent(std::ostream& os, unsigned short i)
{
	while (i-- > 0)
		os <<  '\t';
}

std::ostream& operator<<(std::ostream& os, const SNACC::AsnType& v)
{
    switch (SNACC::SNACC_getiosencodetype(os)) {
    default:
    case SNACC::SNACC_ASCII:
        v.Print(os);
        break;
    case SNACC::BER:
        {
            SNACC::AsnBuf b(os.rdbuf());
            b.ResetMode(std::ios_base::out);
            v.BEnc(b);
        }
        break;
    case SNACC::PER:
        throw SNACC::SnaccException(__FILE__, __LINE__,
                                    "operator<<",
                                    "No Proper PER Support at this time");
        break;
    }
    return os;
}

std::istream& operator>>(std::istream& is, SNACC::AsnType& v)
{
    switch (SNACC::SNACC_getiosencodetype(is)) {
    case SNACC::BER:
        {
            SNACC::AsnBuf b(is.rdbuf());
            SNACC::AsnLen l;
            v.BDec(b, l);
        }
        break;
    default:
        throw SNACC::SnaccException(__FILE__, __LINE__,
                                    "operator>>",
                                    "No IN support at this time");
        break;
    }
    return is;
}
