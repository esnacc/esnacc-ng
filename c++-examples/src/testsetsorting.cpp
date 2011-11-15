#include "asnutil.h"
#include "vdatest_asn.h"

using namespace SNACC;

void multiCardSetTest();

typedef struct set_test_table
{
   AsnType *asnType;
   char *input;
   char *derSorted;
} SetTestTable;

TestSetSorting1 testSetSorting1_AsnType;
VDATestSetOfAny testSetOfAny_AsnType;

SetTestTable gSetSortingTable[]=
{ 
     {
       // SET 
	    //   INTEGER
	    //   OBJECT IDENTIFER
	    //   OCTET STRING
	    //   BIT STRING
        &testSetSorting1_AsnType,
       "311F05000202112206034C02010403112233030200FF1301313000310405000500", // hex input
       "311F02021122030200FF0403112233050006034C02013000310405000500130131"  // hex result
	 },
    {
       &testSetOfAny_AsnType,
       "31050500020111", // set of NULLs input
       "31050201110500"  // result
    },
    {
       &testSetOfAny_AsnType,
       "310B02021122130132030200FF", //  input
       "310B02021122030200FF130132"  // result
    },
    {
       &testSetOfAny_AsnType,
       "318030800202112204011100000000",
       "310B3080020211220401110000"
    },
    {
        &testSetOfAny_AsnType,
       "318005000000",
       "31020500"
    },
	 {
      NULL,
      NULL,
      NULL
	 }
};

void TestSetSorting(void)
{
   int i = 0;
   while (gSetSortingTable[i].input != NULL)
   {
      AsnLen bytesDecoded=0, bytesEncoded;
      //TestSetSorting1 testSetSorting1;
      int ch;

      std::string input, result;
      
      hex2str(gSetSortingTable[i].input, input);
      hex2str(gSetSortingTable[i].derSorted, result);
     
      std::stringstream ssInput(input);
      std::stringstream ssResult(result);
      
      AsnBuf inputBuf(ssInput);
      AsnBuf expectedResultBuf(ssResult);
      AsnBuf outputBuf;

      inputBuf.ResetMode();
      expectedResultBuf.ResetMode();

      gSetSortingTable[i].asnType->BDec(inputBuf, bytesDecoded);
      bytesEncoded = gSetSortingTable[i].asnType->BEnc(outputBuf);
      if (bytesEncoded != bytesDecoded)
      {
         std::cout << "Encoded / Decode byte length mismatch!!!\n";
      }
      
      if (outputBuf == expectedResultBuf)
      {
         std::cout << "SET sorting SUCCESS!\n";
      }
      else
      {

         std::cout << "SET sorting FAILURE!\n";
         std::string str;
         std::cout << "Expected result: ";
         expectedResultBuf.hexDump(std::cout);
         std::cout << std::endl;
         std::cout << "Actual result  : ";
         outputBuf.ResetMode();
         outputBuf.hexDump(std::cout);

         std::cout << std::endl;
      }
      i++;
   }
   multiCardSetTest();
}

void checkBuf(AsnBuf &b)
{
   Deck::const_iterator i;
   
   for (i = b.deck().begin(); i != b.deck().end(); i++)
   {
      Card *tmpCard = *i;
   }
}

void multiCardSetTest()
{
   OctetStringSetOf octsSet;
   unsigned char test[10213];
   fillTestBuffer(test,10213);

   octsSet.insert(octsSet.end(), SNACC::AsnOcts())->Set((char*)test, 10213);

   std::cout << "**** LARGE SET OF OCTET STRING TEST ****\n";

   AsnBuf tmpBuf;
   AsnLen bytesEncoded = octsSet.BEnc(tmpBuf);
   checkBuf(tmpBuf);
   AsnLen bytesDecoded = 0;
   std::stringstream ss;
   ss << tmpBuf;
   

   OctetStringSetOf octsSet2;
   tmpBuf.ResetMode();
   octsSet2.BDec(tmpBuf, bytesDecoded);

   if (bytesEncoded != bytesDecoded)
   {
      std::cout << "Bytes encoded/decoded do not match\n";
   }

   if (checkTestBuffer(octsSet2.front().c_ustr(), octsSet2.front().Len()))
   {
      std::cout << "SUCCESS!\n";
   }
   else
   {
      std::cout << "FAILURE!\n";
   }

   std::cout << "**** END OF LARGE SET OF OCTET STRING TEST ****\n";
}
