                              SNMP V1 TEST DATA

This directory contains gzip compress tar files which contain SNMP V1 tests.

The following two files contain good and bad testcases.  SNACC should gracefully handle decoding these files.  More information about the testcases can be found at: 
http://www.ee.oulu.fi/research/ouspg/protos/testing/c06/snmpv1/index.html


snmpv1_trap_enc_tests.tgz
snmpv1_req_app_tests.tgz

To use any of these files extract them into a unique directory and pass the full path of the directory as the first parameter to "snacctest".



