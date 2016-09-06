#include "asnutil.h"

using namespace SNACC;

typedef struct test_table
{
   char *input;
   unsigned char result[20];
   bool nblFlag;
} AsnBitTestTable;

AsnBitTestTable gBitTestTable[]=
    // input                  // result
{ 
   { 
     "'00000101'B", 
     {0x03, 0x02, 0x00, 0x05},
     false
   },
   {
     "'00000101'B", 
     {0x03, 0x02, 0x00, 0x05},
     true
   },
   { 
     "'1'B", 
     {0x03, 0x02,0x07,0x80},
     false
   },
   { 
     "'1'B", 
     {0x03, 0x02,0x07,0x80},
     true
   },
   { 
     "'1111000000000000'B",
     { 0x03, 0x03, 0x00, 0xF0, 0x00},
     false
   },
   { 
     "'1111000000000000'B",
     { 0x03, 0x02, 0x04, 0xF0},
     true
   },
   { 
     "'0000000000001111'B",
     { 0x03, 0x03, 0x00, 0x00, 0x0F},
     false
   },
   {
     "'00001'B",
     { 0x03, 0x02, 0x03, 0x08 },
     false
   },
   {
     "'00010'B",
     { 0x03, 0x02, 0x03, 0x10 },
     false
   },
   {
     "'00010'B",
     { 0x03, 0x02, 0x04, 0x10 },
     true
   },
   {
     "'00'B",
     { 0x03, 0x01, 0x00 },
     true
   },
   {
     "'0'B",
     { 0x03, 0x02, 0x07, 0x00 },
     false
   },
   {
     "''B",
     { 0x03, 0x01, 0x00 },
     true
   },
   {
      NULL,
      0,
      false
   }
};
 
void bittests(void)
{

   std::cout << "*** Start of AsnBits tests ***\n";

   for (int index=0; gBitTestTable[index].input != NULL; index++)
   {
      AsnBits asnBits(gBitTestTable[index].input);
      asnBits.UseNamedBitListRules(gBitTestTable[index].nblFlag);
      AsnBuf expectedResult((const char *)&gBitTestTable[index].result[0],
         DecTagLen(gBitTestTable[index].result+1) + 2);
      AsnBuf result;
      AsnLen bytesEncoded;

      std::cout << "AsnBits test " << index << " matches expected result? ";

      try
      {
         asnBits.BEnc(result);
         if (!(result == expectedResult))
         {
            std::string str;
            int i;
            int ch;
            std::cout << "NO!\n";

            std::cout << "FAILED!\n";
            std::cout << "Expected Result: ";

            expectedResult.hexDump(std::cout);

            std::cout << "\n";
   
            std::cout << "\nActual Result: ";

            result.hexDump(std::cout);

            std::cout << "\n";
         }
         else
         {
            std::cout << "YES!\n";
         }
         
      }
      catch (SnaccException &e)
      {
         std::cout << "Encode FAILED: \n";
         std::cout << "Error: " << e.what() << std::endl;
         std::cout << "Stack:\n";
         e.getCallStack(std::cout);
      }
   }
   std::cout << "*** End of AsnBits tests ***\n";
}
