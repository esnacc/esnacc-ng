#include "asnutil.h"

using namespace SNACC;

typedef struct test_table
{
   char *input;
   char result[20];
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
     true
   },
   { 
     "2047483647", 
     {0x02, 0x04, 0x7a,0x0a,0x1e,0xff},
     true
   },
   { 
     "-2047483648", 
     {0x02, 0x04, 0x85,0xf5,0xe1,0x00},
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
     "0xff81",      
     {0x02, 0x01, 0x81},
     false
   },
   { 
     "0xff81",      
     {0x02,0x03,0x00,0xff,0x81},
     true
   },
   { 
     "5247483647", //bad
     {0x02, 0x04, 0x7f,0xff,0xff,0xff},
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
   FUNC("intTests()");
   try
   {
      std::cout << "** Conversion tests **\n";
      for (int i=0; i < gCT; i++)
      {
         AsnInt n(convTests[i]);
         int    x = n;
         if (x == convTests[i])
         {
            std::cout.width(10);
            std::cout << convTests[i] << " test SUCCESS!\n";
         }
         else
         {
            std::cout.width(10);
            std::cout << convTests[i] << " FAILED!\n";
            std::cout << "Input: " << convTests[i] << "\n";
            std::cout << "Result: " << x << "\n";
         }
      }
      std::cout << "** End Conversion tests **\n\n";

      for (int testIndex = 0; gIntTestTable[testIndex].input != NULL; testIndex++)
      {
         if (gIntTestTable[testIndex].unsignedFlag)
            std::cout << "AsnInt (unsigned) Test " << testIndex << ":";
         else
            std::cout << "AsnInt ( signed ) Test " << testIndex << ":";

         AsnInt asnInt (gIntTestTable[testIndex].input,
            gIntTestTable[testIndex].unsignedFlag);
         AsnBuf expectedResult(gIntTestTable[testIndex].result,
            DecTagLen((unsigned char *)gIntTestTable[testIndex].result + 1) + 2);
         AsnBuf result;
      
         try
         {
            asnInt.BEnc(result);
         
            std::cout << "INTEGER encoding matches expected result? ";
            if (result == expectedResult)
            {
               std::cout << "YES!" << std::endl;
            }
            else
            {
               std::cout << "NO!" << std::endl;
               std::cout << "Input: " << gIntTestTable[testIndex].input << std::endl;
               std::cout << "Expected Result: ";
               std::cout << std::ios_base::hex << expectedResult;
               std::cout << std::endl;
               std::cout << "Actual Result: ";
               std::cout << std::ios_base::hex << result;
               std::cout << std::endl;
            }
         }
         catch (SnaccException &se)
         {
            std::cout << "AsnInt encode/decode test failed:" << std::endl;
            std::cout << "Error: " << se.what() << std::endl;
            std::cout << "Stack: \n";
            se.getCallStack(std::cout);
         }
      }
   }
   catch(SnaccException &e)
   {
     std::cout << "Int test failed:\n";
     std::cout << "ERROR STRING: ";
     std::cout << e.what() << "\n";
     std::cout.flush();
     std::cout << "*** Int Test ***\n";
   }
}
