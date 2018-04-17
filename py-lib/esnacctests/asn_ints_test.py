#!/usr/bin/env python
# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ESNACC ASN.1 integer tests

import unittest
if __package__ is None:
    import sys
    from os import path
    sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))
    from esnacc import asn_ints
    from esnacc import asn_buffer
else:
    from ..esnacc import asn_ints
    from ..esnacc import asn_buffer


class TestAsnInts(unittest.TestCase):
    def test_operations_default(self):
        asninteger = asn_ints.AsnInt(1)

        self.assertTrue(asninteger == 1)
        self.assertTrue(asninteger > 0)
        self.assertTrue(asninteger < 2)

        asninteger += 1

        self.assertTrue(asninteger == 2)
        self.assertTrue((asninteger * 1) == 2)
        self.assertTrue((asninteger - 1) == 1)

    def test_ber_encoder_simple(self):
        class testVec(unittest.TestCase):
            def __init__(self, number, length, byteCodes):
                self.nm = number
                self.ln = length
                self.bc = byteCodes

            def run(self):
                asninteger = asn_ints.AsnInt(self.nm)
                asnbuf = asn_buffer.AsnBuf()

                self.assertTrue(asninteger.BEnc(asnbuf) == self.ln)
                byteCodes = asnbuf.Buffer()
                self.assertTrue(len(byteCodes) == len(self.bc))
                for x in range(len(byteCodes)):
                    self.assertTrue(self.bc[x] == byteCodes[x])
                return True

        tests = []
        tvn65535codes = [chr(0x2), chr(0x3), chr(0xff), chr(0x0), chr(0x1)]
        tests.append(testVec(-65535, 5, tvn65535codes))

        tvn10codes = [chr(0x2), chr(0x1), chr(0xf6)]
        tests.append(testVec(-10, 3, tvn10codes))

        tv0codes = [chr(0x2), chr(0x1), chr(0x0)]
        tests.append(testVec(0, 3, tv0codes))

        tv1codes = [chr(0x2), chr(0x1), chr(0x1)]
        tests.append(testVec(1, 3, tv1codes))

        tv2codes = [chr(0x2), chr(0x2), chr(0x3f), chr(0xc3)]
        tests.append(testVec(16323, 4, tv2codes))

        tv3codes = [chr(0x2), chr(02), chr(0x7f), chr(0xff)]
        tests.append(testVec(32767, 4, tv3codes))

        tv4codes = [chr(0x2), chr(0x3), chr(0x00), chr(0x80), chr(0x00)]
        tests.append(testVec(32768, 5, tv4codes))

        tv5codes = [chr(0x2), chr(0x3), chr(0x20), chr(0x00), chr(0x00)]
        tests.append(testVec(2097152, 5, tv5codes))

        tv6codes = [chr(0x2), chr(0x4), chr(0x1), chr(0x0), chr(0x0), chr(0x0)]
        tests.append(testVec(16777216, 6, tv6codes))

        for x in tests:
            self.assertTrue(x.run())

    def test_ber_decoder_simple(self):
        class testVec(unittest.TestCase):
            def __init__(self, number, length, byteCodes):
                self.nm = number
                self.ln = length
                self.bc = byteCodes

            def run(self):
                asnbuf = asn_buffer.AsnBuf(self.bc)
                asninteger = asn_ints.AsnInt()
                self.assertTrue(asninteger.BDec(asnbuf, 0) == self.ln)
                self.assertTrue(self.nm == int(asninteger))
                return True

        tests = []
        tvn65535codes = [chr(0x2), chr(0x3), chr(0xff), chr(0x0), chr(0x1)]
        tests.append(testVec(-65535, 5, tvn65535codes))

        tvn10codes = [chr(0x2), chr(0x1), chr(0xf6)]
        tests.append(testVec(-10, 3, tvn10codes))

        tv0codes = [chr(0x2), chr(0x1), chr(0x0)]
        tests.append(testVec(0, 3, tv0codes))

        tv1codes = [chr(0x2), chr(0x1), chr(0x1)]
        tests.append(testVec(1, 3, tv1codes))

        tv2codes = [chr(0x2), chr(0x2), chr(0x3f), chr(0xc3)]
        tests.append(testVec(16323, 4, tv2codes))

        tv3codes = [chr(0x2), chr(02), chr(0x7f), chr(0xff)]
        tests.append(testVec(32767, 4, tv3codes))

        tv4codes = [chr(0x2), chr(0x3), chr(0x00), chr(0x80), chr(0x00)]
        tests.append(testVec(32768, 5, tv4codes))

        tv5codes = [chr(0x2), chr(0x3), chr(0x20), chr(0x00), chr(0x00)]
        tests.append(testVec(2097152, 5, tv5codes))

        tv6codes = [chr(0x2), chr(0x4), chr(0x1), chr(0x0), chr(0x0), chr(0x0)]
        tests.append(testVec(16777216, 6, tv6codes))

        for x in tests:
            self.assertTrue(x.run())

    def test_constraints(self):
        class testVec(unittest.TestCase):
            def __init__(self, number):
                self.nm = number

            def run(self):
                cnstraint_good = asn_ints.IntegerConstraint(self.nm-1,
                                                            self.nm+1)
                asninteger = asn_ints.AsnInt(self.nm)
                asninteger.addConstraint(cnstraint_good)

                buf = asn_buffer.AsnBuf()
                self.assertTrue(asninteger.BEnc(buf) != 0)

                cnstraint_bad = asn_ints.IntegerConstraint(self.nm+1,
                                                           self.nm+2)
                asninteger.addConstraint(cnstraint_bad)
                with self.assertRaises(ValueError):
                    asninteger.BEnc(buf)
                return True

        tests = []
        tests.append(testVec(-65535))
        tests.append(testVec(-10))
        tests.append(testVec(0))
        tests.append(testVec(1))
        tests.append(testVec(16323))
        tests.append(testVec(32767))
        tests.append(testVec(32768))
        tests.append(testVec(2097152))
        tests.append(testVec(16777216))

        for x in tests:
            self.assertTrue(x.run())


class TestAsnEnums(unittest.TestCase):
    def test_enumeration_encoding(self):
        class testVec(unittest.TestCase):
            def __init__(self, number, length, byteCodes):
                self.nm = number
                self.ln = length
                self.bc = byteCodes

            def run(self):
                asnenum = asn_ints.AsnEnum(self.nm)
                asnbuf = asn_buffer.AsnBuf()

                self.assertTrue(asnenum.BEnc(asnbuf) == self.ln)
                byteCodes = asnbuf.Buffer()
                self.assertTrue(len(byteCodes) == len(self.bc))
                for x in range(len(byteCodes)):
                    self.assertTrue(self.bc[x] == byteCodes[x])
                return True

        tests = []
        tv0codes = [chr(0xa), chr(0x1), chr(0x0)]
        tests.append(testVec(0, 3, tv0codes))

        for x in tests:
            self.assertTrue(x.run())


if __name__ == '__main__':

    unittest.main()
