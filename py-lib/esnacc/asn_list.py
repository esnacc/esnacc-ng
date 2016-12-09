# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Integer Types for eSNACC

from asn_base import AsnBase
from asn_base import BERConsts


class AsnSetOf(AsnBase):

    BER_TAG = BERConsts.SET_TAG_CODE
    BER_FORM = BERConsts.BER_CONSTRUCTED_FORM

    def __init__(self, elemts=None, expected=None):
        self.constraints = None
        self.elemts = []
        self.expected = expected
        if expected is None:
            raise ValueError("Cannot decode a bare typed list: specify a type")
        if elemts is not None:
            self.elemts = elemts

    def __getitem__(self, idx):
        if self.elemts is None or idx > len(self.elemts):
            raise IndexError("Octet value out of range")
        return self.elemts[idx]

    def __contains__(self, a):
        if self.elemts is not None:
            return a in self.elemts
        return None

    def __len__(self):
        if self.elemts is not None:
            return len(self.elemts)
        return 0

    def __delitem__(self, idx):
        if self.elemts is not None:
            del self.elemts[idx]
        raise IndexError("Out of range")

    def append(self, item):
        if isinstance(item, self.expected):
            self.elemts.append(item)
        else:
            raise TypeError("Invalid type, expected: %s" % (self.expected))

    def __str__(self):
        s = "%s" % self.typename()
        s += " {"
        s += ','.join(str(x) for x in self.elemts)
        s += "}"
        return s

    def typename(self):
        return "AsnSetOf"

    def BEncContent(self, asnbuf):
        enclength = 0
        for x in reversed(self.elemts):
            enclength += x.BEnc(asnbuf)

        return enclength

    def BDecContent(self, asnbuf, contentlen):
        while contentlen > 0:
            t = self.expected()
            contentlen -= t.BDec(asnbuf, contentlen)


class AsnSequenceOf(AsnSetOf):

    BER_TAG = BERConsts.SEQ_TAG_CODE

    def __init__(self, elemts=None, expected=None):
        AsnSetOf.__init__(self, elemts, expected)

    def typename(self):
        return "AsnSequenceOf"

    def BEncContent(self, asnbuf):
        self.elemts.sort()
        return AsnSetOf.BEncContent(self, asnbuf)

    def BDecContent(self, asnbuf, contentlen):
        ret = AsnSetOf.BDecContent(self, asnbuf, contentlen)
        # Generally, an ASN list is unsorted, but a SEQ implies sorted
        # - however -, if a remote end decides to give us an unsorted
        # list, we'll just do the sort here.
        self.elemts.sort()
        if ret is None:
            ret = 0
        return ret
