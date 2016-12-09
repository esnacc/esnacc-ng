# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 'Useful' Types for eSNACC
# Normally, this should be auto-generated.  For now, we'll handcode it
# (because insanity)

from asn_base import BERConsts
from asn_octs import AsnOcts
from asn_octs import CharacterValueConstraint


NumericStrConstraint = \
        CharacterValueConstraint(charList=['0', '1', '2', '3', '4', '5',
                                           '6', '7', '8', '9', ' '])


class NumericString(AsnOcts):

    BER_TAG = BERConsts.NUMERICSTRING_TAG_CODE

    def __init__(self, value=None):
        self.constraints = [NumericStrConstraint]
        self.octs = value
        if self.octs is not None:
            self.setData(value)

class IA5String(AsnOcts):
    BER_TAG = BERConsts.IA5STRING_TAG_CODE

    def __init__(self, value=None):
        AsnOcts.__init__(self, value)
