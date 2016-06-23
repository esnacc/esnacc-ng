#include "asnutil.h"
#include <sstream>

#ifdef WIN32
    #include <crtdbg.h>
#endif

void newBufTest();
int automaticTests();

using namespace SNACC;

void doubleDecodeTest(void)
{
    char buf[] = {0x04,0x01,0x31,0x04,0x01,0x32};
    AsnBuf asnBuf(buf, 6);
    AsnLen len;
    AsnOcts octs;
    try {
        std::cout << "*** doubleDecodeTest ***\n";
        if (octs.BDecPdu(asnBuf, len)) {
            if (octs.c_ustr()[0] == '1') {
                std::cout << "First OCTET STRING value: "
                          << octs.c_ustr()[0] << std::endl;

                if (octs.BDecPdu(asnBuf, len)) {
                    if (octs.c_ustr()[0] == '2') {
                        std::cout << "Second OCTET STRING value: " 
                                  << octs.c_ustr()[0] << std::endl;
                    } else {
                        std::cout
                            << "Unexpected result for second OCTET STRING: "
                            << octs.c_ustr()[0] << std::endl;
                    }
                } else {
                    std::cout << "Decode of second OCTET STRING failed!\n";
                }
            } else {
                std::cout
                    << "ERROR: Unexpected result for first OCTET STRING: "
                    << octs.c_ustr()[0] << std::endl;
            }
        } else {
            std::cout << "Decode of double OCTET STRING encoded FAILED!\n";
        }

        std::cout << "*** doubleDecodeTest ***\n";

  } catch(SnaccException &e) {
        std::cout << "Double decode test failed:\n";
        std::cout << "ERROR STRING: ";
        std::cout << e.what() << "\n";
        std::cout.flush();
        std::cout << "*** doubleDecodeTest ***\n";
    }
}

void octsTest(void)
{
    unsigned char buf[] = {0x04,0x14,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
                           0x38,0x39,0x30,0x41,0x42,0x43,0x44,0x45,0x46,
                           0x47,0x48,0x49,0x50};
    AsnBuf asnBuf((char *)buf, 22);
    size_t i;
    AsnLen len = 22;

    AsnOcts octs;
    try {
        std::cout << "*** start of AsnOcts tests ***\n";
        if (octs.BDecPdu(asnBuf, len)) {
            if (memcmp(octs.c_ustr(), &buf[2], octs.Len()) == 0) {
                for (i = 0; i < 20; i++) {
                    std::cout << "OCTET STRING value: " << octs.c_ustr()[i] << std::endl;
                }
            } else {
                for (i = 0; i < 20; i++) {
                    std::cout
                        << "ERROR: Unexpected result for OCTET STRING: "
                        << octs.c_ustr()[i] << std::endl;
                }
            }
        } else {
            std::cout << "Decode of OCTET STRING encoded FAILED!\n";
        }

        std::cout << "*** End of AsnOcts tests ***\n";
    } catch(SnaccException &e) {
        std::cout << "Octs test failed:\n";
        std::cout << "ERROR STRING: ";
        std::cout << e.what() << "\n";
        std::cout.flush();
        std::cout << "*** End of AsnOcts tests ***\n";
    }
}

void fillTest(void)
{
    unsigned char buf[] = {
        0x24, 0x80, 
            0x24, 0x80, 
                0x04, 0x01, 0xaa, 
                0x04, 0x01, 0xbb,
            0x00, 0x00, 
            0x04, 0x01, 0xcc,
            0x24, 0x80,
                0x04, 0x01, 0xdd,
                0x04, 0x01, 0xee, 
                0x24, 0x80, 
                    0x24, 0x80,
                        0x24, 0x80,
                            0x24, 0x80,
                                0x04, 0x01, 0xff,
                            0x00, 0x00,
                        0x00, 0x00,
                        0x04, 0x01, 0x11,
                    0x00, 0x00,
                0x00, 0x00,
                0x04, 0x01, 0x22,
            0x00, 0x00,
            0x04, 0x01, 0x33,
        0x00, 0x00
    };

    AsnBuf asnBuf((char *)buf, sizeof(buf));
    AsnLen len = sizeof(buf);

    AsnOcts octs;
    try {
        std::cout << "*** start of Recursive Fill tests ***\n";
        octs.BDec(asnBuf, len);

        std::cout << std::endl;

        octs.PrintXML(std::cout);

        std::cout << std::endl; 
        std::cout << len;
        std::cout << "*** End of Recursive Fill tests ***\n";

    } catch(SnaccException &e) {
        std::cout << "Octs test failed:\n";
        std::cout << "ERROR STRING: ";
        std::cout << e.what() << "\n";
        std::cout.flush();
        std::cout << "*** End of Recursive Fill tests ***\n";
    }
}

int main(int argc, char *argv[])
{
#ifdef WIN32
    long memAllocNum = 0;

    // Set the debug flags for memory leak checking
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

    int debugFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    debugFlag = _CrtSetDbgFlag(debugFlag | _CRTDBG_LEAK_CHECK_DF );

    if (memAllocNum > 0) {
        _CrtSetBreakAlloc(memAllocNum);
    }
#endif // WIN32

   char *snmpPath=NULL;

   try {
       newBufTest();

       if (argc > 1) {
           snmpPath = argv[1];
       }

       octsTest();
       doubleDecodeTest();
       inttests();
       bittests();

       if (snmpPath != NULL) {
           run_snmp_tests(snmpPath);
       }

       fillTest();
       if (automaticTests()) {
           std::cout << "Automatic test failure." << std::endl;
           return 1;
       }
   } catch (SnaccException &e) {
       std::cout << "\n**** Caught SnaccException ****\n";
       std::cout << "What: " << e.what() << std::endl;
       std::cout << "Call Stack:\n";
       e.getCallStack(std::cout);
   }
   return 0;
}
