# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Integer Types for eSNACC

from asn_base import AsnBase
from asn_base import BERConsts
from asn_base import Constraint


class CharacterValueConstraint(Constraint):
    def __init__(self, lowerBound=None, upperBound=None, charList=None):
        self.lower = lowerBound
        self.upper = upperBound
        self.chars = charList

    def withinConstraint(self, value):
        if isinstance(value, list) or isinstance(value, basestring):
            for x in value:
                if self.withinConstraint(x):
                    return False
        if self.lower is not None and value is not None and \
           value < self.lower:
            return False
        if self.upper is not None and value is not None and \
           value > self.upper:
            return False
        if self.chars is not None and value is not None and \
           value not in self.chars:
            return False
        return True


class OctetLengthConstraint(Constraint):
    def __init__(self, length=None):
        self.length = length

    def withinConstraint(self, value):
        if isinstance(value, list) or isinstance(value, basestring):
            if len(value) <= self.length:
                return True
        return False


class AsnOcts(AsnBase):
    BER_TAG = BERConsts.OCTETSTRING_TAG_CODE

    def __init__(self, value=None):
        self.constraints = None
        self.octs = None
        if value is not None:
            self.setData(value)

    def __getitem__(self, idx):
        if self.octs is None or idx > len(self.octs):
            raise IndexError("Octet value out of range")
        return self.octs[idx]

    def __contains__(self, a):
        if self.octs is not None:
            return a in self.octs
        return None

    def __len__(self):
        if self.octs is not None:
            return len(self.octs)
        return 0

    def __delitem__(self, idx):
        if self.octs is not None:
            del self.octs[idx]
        raise IndexError("Out of range")

    def __str__(self):
        s = "["
        if self.octs is not None:
            s += ' '.join(["%02X" % ord(x) for x in self.octs])
        s += "]"
        return s

    def typename(self):
        return "AsnOcts"

    def octs(self):
        return self.octs

    def setData(self, octets):
        if isinstance(octets, list):
            self.octs = octets
        elif isinstance(octets, basestring):
            self.octs = []
            for x in octets:
                self.octs.append(chr(ord(x)))

    def BEncContent(self, asnbuf):
        asnbuf.PutBufReverse(self.octs)
        return len(self.octs)

    def BDecContent(self, asnbuf, contentlen):
        self.octs = asnbuf.GetSeg(contentlen)
        return len(self.octs)


class AsnString(AsnOcts):
    def __init__(self, value=None):
        self.contains = None
        if value is None:
            value = ""
        self.setData(value)
