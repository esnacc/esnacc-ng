#include "asnutil.h"

using namespace SNACC;

typedef struct test_table
{
   char *input;
   unsigned char result[20];
   bool unsignedFlag;
} AsnIntTestTable;

AsnIntTestTable gIntTestTable[]=
    // input                  // result
{ 
   { 
     "1234",
     {0x02, 0x02, 0x04, 0xD2},
     true
   },
   { 
     "-3628",
     {0x02, 0x02, 0xf1, 0xD4},
     false
   },
   { 
     "2047483647",
     {0x02, 0x04, 0x7a,0x0a,0x1e,0xff},
     true
   },
   { 
     "-2047483648",
     {0x02, 0x05, 0x0, 0x85,0xf5,0xe1,0x00},
     true
   },
	{ 
     "0xffffffff",
     {0x02, 0x01, 0xff},
     false
   },
   {
     "0xffffffff",
     {0x02, 0x05, 0x00,0xff,0xff,0xff,0xff},
     true
   },
   { 
     "0x0000ffff",
     {0x02, 0x03,0x00,0xff,0xff},
     true
   },
   { 
     "0x0000ffff",
     {0x02, 0x03,0x00,0xff,0xff},
     true
   },
   { 
     "0x000fffff",
     {0x02,0x03,0x0f,0xff,0xff},
     false
   },
   { 
     "0x000001",
     {0x02, 0x01, 0x01},
     false
   },
   { 
     "0x0080",
     {0x02, 0x02, 0x00, 0x80},
     true
   },
   { 
     "0x0080",
     {0x02, 0x02, 0x00, 0x80},
     false
   },
   { 
     "0xffffff81",
     {0x02, 0x01, 0x81},
     false
   },
   { 
     "0xff81",
     {0x02,0x03,0x00,0xff,0x81},
     true
   },
   { 
     "5247483647",
     {0x02, 0x04, 0x38, 0xc6, 0x3e, 0xff},
     true
   },
   { 
     NULL, 
     NULL,
     false,
   }
};

#define gCT 11
int convTests[gCT] = { 0, -1, -5, -256, 256, 16, -10000, 10000, 655335, -655335,-1000000 };

void inttests(void)
{

   std::cout << "** Conversion tests **\n";
   for (int i=0; i < gCT; i++) {
       AsnInt n(convTests[i]);
       int    x = n;
       if (x == convTests[i]) {
           std::cout.width(10);
           std::cout << convTests[i] << " test SUCCESS!\n";
       } else {
           std::cout.width(10);
           std::cout << convTests[i] << " FAILED!\n";
           std::cout << "Input: " << convTests[i] << "\n";
           std::cout << "Result: " << x << "\n";
       }
   }
   std::cout << "** End Conversion tests **\n\n";

   for (int testIndex = 0; gIntTestTable[testIndex].input != NULL;
        testIndex++) {
       if (gIntTestTable[testIndex].unsignedFlag)
           std::cout << "AsnInt (unsigned) Test " << testIndex << ":"
                     << gIntTestTable[testIndex].input;
       else
           std::cout << "AsnInt ( signed ) Test " << testIndex << ":"
                     << gIntTestTable[testIndex].input;

       AsnInt asnInt (gIntTestTable[testIndex].input,
                      gIntTestTable[testIndex].unsignedFlag);
       AsnBuf expectedResult((const char *)gIntTestTable[testIndex].result,
                             DecTagLen(gIntTestTable[testIndex].result + 1) + 2);
       AsnBuf result;

       asnInt.BEnc(result);

       std::cout << "INTEGER encoding matches expected result? ";
       if (result == expectedResult) {
           std::cout << "YES!" << std::endl;
       } else {
           result.hexDump(std::cout);
           char msg[512] = {0};
           snprintf(msg, 512, "Failed INTEGER encode/decode test id: %d",
                    testIndex);
           throw SnaccException(__FILE__, __LINE__, __func__, strdup(msg));
       }
   }
}
