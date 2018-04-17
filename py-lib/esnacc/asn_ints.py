# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Integer Types for eSNACC

from asn_base import AsnBase
from asn_base import Constraint
from asn_base import BERConsts
from asn_buffer import AsnBuf
import math


class IntegerConstraint(Constraint):
    def __init__(self, lowerBound=None, upperBound=None):
        self.lower = lowerBound
        self.upper = upperBound

    def withinConstraint(self, value):
        if self.lower is not None and value is not None and value < self.lower:
            return False
        if self.upper is not None and value is not None and value > self.upper:
            return False
        return True


def isfundamental_int_type(value):
    if isinstance(value, int) or isinstance(value, float) or \
       isinstance(value, basestring):
        return True
    return False


class AsnInt(AsnBase):

    BER_TAG = BERConsts.INTEGER_TAG_CODE

    def __init__(self, value=None):
        self.constraints = None
        if value is None:
            self.intVal = 0
        elif isfundamental_int_type(value):
            self.intVal = int(value)
        elif isinstance(value, AsnInt):
            self.intVal = value.intVal
        else:
            raise ValueError("AsnInt - bad value")

    def __bool__(self):
        if self.intVal == 0:
            return False
        return True

    __nonzero__ = __bool__

    def __cmp__(self, obj):
        cmpValA = self.intVal
        if isinstance(obj, int):
            cmpValB = obj
        elif isinstance(obj, AsnInt):
            cmpValB = obj.intVal
        elif isinstance(obj, basestring) or isinstance(obj, float):
            cmpValB = int(obj)
        else:
            raise TypeError("Compare with int, or AsnInt")
        return cmp(cmpValA, cmpValB)

    def value(self, newval=None):
        if newval is not None:
            if isinstance(newval, int) or isinstance(newval, float) or \
               isinstance(newval, basestring):
                self.intVal = int(newval)
            elif isinstance(newval, AsnInt):
                self.intVal = newval.intVal
        return self.intVal

    def __int__(self):
        return self.value()

    def __float__(self):
        return float(self.intVal)

    def __add__(self, obj):
        if isinstance(obj, AsnInt):
            return self.intVal + obj.intVal
        return self.intVal + int(obj)

    def __iadd__(self, obj):
        if isinstance(obj, AsnInt):
            self.intVal += obj.intVal
        else:
            self.intVal += int(obj)
        return self

    def __sub__(self, obj):
        if isinstance(obj, AsnInt):
            return self.intVal - obj.intVal
        return self.intVal - int(obj)

    def __isub__(self, obj):
        if isinstance(obj, AsnInt):
            self.intVal -= obj.intVal
        else:
            self.intVal -= int(obj)
        return self

    def __mul__(self, obj):
        if isinstance(obj, AsnInt):
            return self.intVal * obj.intVal
        return self.intVal * obj

    def __imul__(self, obj):
        if isinstance(obj, AsnInt):
            self.intVal *= obj.intVal
        else:
            self.intVal *= obj
        return self

    def __str__(self):
        return str(self.value())

    def typename(self):
        return "AsnInt"

    def passesAllConstraints(self, constraints):
        if constraints is None:
            return True
        if isinstance(constraints, IntegerConstraint):
            return constraints.withinConstraint(self.intVal)
        elif isinstance(constraints, list):
            for x in constraints:
                if not self.passesAllConstraints(x):
                    return False
        return True

    def BEncContent(self, asnbuf):
        if not isinstance(asnbuf, AsnBuf):
            raise ValueError("Buffer must be esnacc.asn_buf.AsnBuf")

        octets = []
        value = int(self)
        while 1:
            octets.insert(0, value & 0xff)
            if value == 0 or value == -1:
                break
            value = value >> 8
        if value == 0 and octets[0] & 0x80:
            octets.insert(0, 0)
        while len(octets) > 1 and \
            (octets[0] == 0 and octets[1] & 0x80 == 0 or
             octets[0] == 0xff and octets[1] & 0x80 != 0):
            del octets[0]

        asnbuf.PutBufIntsReverse(octets)
        return len(octets)

    def BDecContent(self, asnbuf, intlen):
        buf = ord(asnbuf.Buffer()[0])
        result = 0
        if buf & 0x80:
            result = -1

        for x in range(intlen):
            result <<= 8
            result |= ord(asnbuf.GetByte())
        self.intVal = result
        return intlen


class EnumeratedConstraint(Constraint):
    def __init__(self, listOfEnumerations):
        self.enums = listOfEnumerations

    def withinConstraint(self, value):
        if value not in self.enums:
            return False
        return True


class AsnEnum(AsnInt):
    BER_TAG = BERConsts.ENUM_TAG_CODE

    def __init__(self, value=None):
        self.constraints = None
        if value is not None:
            self.intVal = value

    def typename(self):
        return "AsnEnum"

    def passesAllConstraints(self, constraints):
        if constraints is None:
            return True
        if isinstance(constraints, IntegerConstraint) \
           or isinstance(constraints, EnumeratedConstraint):
            return constraints.withinConstraint(self.intVal)
        elif isinstance(constraints, list):
            for x in constraints:
                if not self.passesAllConstraints(x):
                    return False
        return True


class AsnReal(AsnBase):

    BER_TAG = BERConsts.REAL_TAG_CODE

    def __bool__(self):
        if self.floatVal == 0:
            return False
        return True

    def __int__(self):
        return int(self.value())

    def __float__(self):
        return self.value()

    def __add__(self, obj):
        if isinstance(obj, AsnReal):
            return self.floatVal + obj.floatVal
        return self.floatVal + float(obj)

    def __iadd__(self, obj):
        if isinstance(obj, AsnReal):
            self.floatVal += obj.floatVal
        else:
            self.floatVal += float(obj)
        return self

    def __sub__(self, obj):
        if isinstance(obj, AsnReal):
            return self.floatVal - obj.floatVal
        return self.floatVal - float(obj)

    def __isub__(self, obj):
        if isinstance(obj, AsnReal):
            self.floatVal -= obj.floatVal
        else:
            self.floatVal -= float(obj)
        return self

    def __mul__(self, obj):
        if isinstance(obj, AsnReal):
            return self.floatVal * obj.floatVal
        return self.floatVal * obj

    def __imul__(self, obj):
        if isinstance(obj, AsnReal):
            self.floatVal *= obj.floatVal
        else:
            self.floatVal *= obj
        return self

    def __str__(self):
        return str(self.value())

    __nonzero__ = __bool__

    def __cmp__(self, obj):
        cmpValA = self.floatVal
        if isinstance(obj, int):
            cmpValB = obj
        elif isinstance(obj, AsnReal):
            cmpValB = obj.floatVal
        elif isinstance(obj, basestring) or isinstance(obj, int):
            cmpValB = int(obj)
        else:
            raise TypeError("Compare with float, or AsnReal")
        return cmp(cmpValA, cmpValB)

    def typename(self):
        return "AsnReal"

    def value(self, newval=None):
        if newval is not None:
            if isinstance(newval, int) or isinstance(newval, float) or \
               isinstance(newval, basestring):
                self.floatVal = float(newval)
            elif isinstance(newval, AsnReal):
                self.floatVal = newval.floatVal
        return self.floatVal

    PLUS_INF = float('+inf')
    MINUS_INF = float('-inf')
    NOT_A_NUM = float('nan')

    def BEncContent(self, asnbuf):
        if not isinstance(asnbuf, AsnBuf):
            raise ValueError("Buffer must be esnacc.asn_buf.AsnBuf")

        if self.floatVal is 0.0:
            return 0

        if self.floatVal is AsnReal.PLUS_INF:
            asnbuf.PutBuf(chr(0x40))
            return 1
        elif self.floatVal is AsnReal.MINUS_INF:
            asnbuf.PutBuf(chr(0x41))
            return 1

        def intcnv(f):
            return chr(int(f))

        m, e = math.frexp(self.floatVal)
        # Time to normalize for base2 encoding

        encF = 0x80
        if m < 0:
            encF |= 0x40

        while m & 0x1 == 0:
            m >>= 1
            e += 1

        scaleF = 0
        while m & 0x1 == 0:
            m >>= 1
            scaleF += 1

        if scaleF > 3:
            raise ValueError('Scale Factor overflow')

        encE = intcnv(0 & 0xff)
        encF |= scaleF << 2
        if e in (0, -1):
            encE = intcnv(e & 0xff)
        else:
            while e not in (0, -1):
                encE = intcnv(e & 0xff) + encE
                e >>= 8
            if e == 0 and encE[0] != 0 and int(encE[0]) & 0x80:
                encE = intcnv(0) + encE
            if e == -1 and encE[0] != 0 and int(encE[0]) & 0x80:
                encE = intcnv(0xff) + encE
        ln = len(encE)
        if ln > 0xff:
            raise ValueError('Overflow of real value')
        if ln > 1:
            encF |= ln - 1
            if ln > 3:
                encE = intcnv(ln & 0xff) + encE

        encV = intcnv(0)
        while m:
            encV = intcnv(m & 0xff) + encV
            m >>= 8

        octs = intcnv(encF) + encE + encV
        asnbuf.PutBufReverse(octs)
        return len(octs)

    def BDecContent(self, asnbuf, length):
        raise NotImplementedError('Not ready yet')
