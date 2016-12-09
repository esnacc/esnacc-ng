# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Buffer library for eSNACC


def ints2octs(listOfInts):
    return [chr(int(x)) for x in listOfInts]


def BEncDefLen(asnbuf, length):
    if length < 128:
        asnbuf.PutBufReverse(chr(length & 0xff))
        return 1
    elif length < 256:
        asnbuf.PutBufReverse(chr(length & 0xff))
        asnbuf.PutBufReverse(chr(0x81))
        return 2
    elif length < 65536:
        asnbuf.PutBufReverse(chr(length & 0xff))
        asnbuf.PutBufReverse(chr((length & 0xff00) >> 8))
        asnbuf.PutBufReverse(chr(0x82))
        return 3
    elif length < 16777126:
        asnbuf.PutBufReverse(chr(length & 0xff))
        asnbuf.PutBufReverse(chr((length & 0xff00) >> 8))
        asnbuf.PutBufReverse(chr((length & 0xff0000) >> 16))
        asnbuf.PutBufReverse(chr(0x83))
        return 4
    else:
        asnbuf.PutBufReverse(chr(length & 0xff))
        asnbuf.PutBufReverse(chr((length & 0xff00) >> 8))
        asnbuf.PutBufReverse(chr((length & 0xff0000) >> 16))
        asnbuf.PutBufReverse(chr((length & 0xff000000) >> 24))
        asnbuf.PutBufReverse(chr(0x84))
        return 5


def BDecDefLen(asnbuf):
    lenByte = ord(asnbuf.GetByte())
    if lenByte < 128:
        return 1, lenByte
    elif lenByte == 0x80:
        raise IOError("INDEFINITE length not supported")

    bytesTotal = lenByte & 0x7f
    lenByte = 0
    for b in range(bytesTotal):
        lenByte = lenByte << 8
        lenByte += ord(asnbuf.GetByte())
    return bytesTotal, lenByte


class AsnBuf(object):

    def __init__(self, bufbytes=None):
        if bufbytes is None:
            self.buf = []
        else:
            if isinstance(bufbytes, basestring):
                self.buf = list(bufbytes)
            elif isinstance(bufbytes, file):
                self.buf = list(bufbytes.read())
            else:
                self.buf = bufbytes

    def __len__(self):
        return len(self.buf)

    def PutBufReverse(self, listOf):
        length = 0
        if isinstance(listOf, list):
            for x in reversed(listOf):
                length += self.PutBufReverse(x)
        else:
            self.buf.insert(0, listOf)
            length = 1
        return length

    def PutBufIntsReverse(self, listOfInts):
        return self.PutBufReverse(ints2octs(listOfInts))

    def PutBuf(self, listOf):
        length = 0
        if isinstance(listOf, list):
            for x in listOf:
                length += self.PutBuf(x)
        else:
            self.buf.append(listOf)
            length = 1
        return length

    def PutBufInts(self, listOfInts):
        return self.PutBuf(ints2octs(listOfInts))

    def Buffer(self):
        return self.buf

    def swallow(self, num):
        if len(self.buf) < num:
            raise ValueError("Too many bytes to swallow")

        for x in range(num):
            del self.buf[0]

    def GetByte(self):
        ret = self.buf[0]
        self.swallow(1)
        return ret

    def GetSeg(self, length):
        ret = []

        if len(self.buf) < length:
            raise ValueError("Too many bytes specified")

        for x in range(length):
            ret.append(self.buf[0])
            del self.buf[0]

        return ret
