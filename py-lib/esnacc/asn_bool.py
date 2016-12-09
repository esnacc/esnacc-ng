# Copyright (C) 2016, Aaron Conole <aconole@bytheb.org>
#
# Licensed under the terms of the GNU General Public License
# agreement (version 2 or greater).  Alternately, may be licensed
# under the terms of the ESNACC Public License.
#
# ASN.1 Boolean for eSNACC

from asn_base import BERConsts
from asn_ints import AsnInt

def convert_to_bool(value):
    if value is None:
        return False

    if isinstance(value, int) or isinstance(value, float):
        if value == 0:
            return False
        return True

    if isinstance(value, basestring):
        if value.lower() != "true":
            return False
        return True

    return False

class AsnBool(AsnInt):

    BER_TAG = BERConsts.BOOLEAN_TAG_CODE

    def __init__(self, value=None):
        self.value(value)

    def value(self, newval=None):
        if convert_to_bool(newval):
            self.intVal = 0xff
        else:
            self.intVal = 0
        return convert_to_bool(newval)

    def typename(self):
        return "AsnBool"

    def __int__(self):
        if convert_to_bool(self.value()):
            return 0xff
        return 0

    def __str__(self):
        if convert_to_bool(self.value()):
            return 'True'
        return 'False'
