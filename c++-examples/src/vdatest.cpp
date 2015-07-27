
#include <stdio.h>
#include <stdlib.h>
#include "asn-incl.h"
#include "vda_threads.h"
#include <strstream>
#include <fstream>

using namespace SNACC;

#include "vdatest_asn.h"
#include "vdatest_asn2.h"
short traverseDir(char *path);
void test_AsnOid();
void ANY_DEFINED_BY_test();
void ANY_DEFINED_BY_test_2();
int  append_test();
void Test_ASN_1_Strings();
void test_big_buffer();
void test_timing();
#define TEST_ENCODE_BUF 1
#define TEST_DECODE_BUF 2
void XML_test();
void AsnIntTest();
AsnInt AAA(22);     //test constructors
AsnInt AA2(0);      //test constructors
long vdaTestThreads(int lThreadCount);
void vdaTestThreadLocks();
long vdaTestPrintThreadLocks();
void test_IndefiniteLengthEncoding();
using namespace VDATestModule2Namespace;

//
//
int vdatest_main(int argc, char *argv[])
{
  
   /*if (argc > 1) 
   {
       traverseDir(argv[1]);

       exit(0);
   }*/
   test_IndefiniteLengthEncoding();

#ifdef BOUNDS_CHECKER_TEST_WITHOUT_THREADS
   vdaTestPrintThreadLocks();
   return(0);
   vdaTestThreadLocks();
   vdaTestThreads(55);  // AVOIDS problems with above; must be performed last.
#endif //ifdef BOUNDS_CHECKER_TEST_WITHOUT_THREADS

   try
   {
      ANY_DEFINED_BY_test();
      test_AsnOid();
      AsnIntTest();
      ANY_DEFINED_BY_test_2();
      XML_test();
      test_timing();
      test_big_buffer();
      // THE FOLLOWING TEST also tests SET OF ordering.
      // ALSO, it must be last since an error condition is trapped.
      append_test();
   }
   catch ( SnaccException &e )
   {
      std::cout << "ERROR: " << e.what() << "\n";
      std::cout << "Call stack:\n";
      e.getCallStack(std::cout);
      std::cout << "\n";
   }


   std::cout << "####### END OF vdatest TESTS #########\n";
   std::cout.flush();
  
   return 1;
}


// Excercise ANY code
//
void ANY_DEFINED_BY_test()
{
   FUNC("ANY_DEFINED_BY_test()");
  
   AsnOid testOid3_UNKNOWN("1.2.3.4.5.5.5.5.6");
  
   AsnBuf buf;
   TestDefinedByUsage A, A2, A3;

   PrintableString prtblStr, prtblStr2;
   PrintableString *pPrtblStr = NULL;
   
   prtblStr = "THIS IS A TEST";
   //pB->Set("THIS IS A TEST", strlen("THIS IS A TEST")+1);

   A.id = testOID2;
   A.anyDefBy.value  = prtblStr.Clone();
   A.i1 = 1;
   A.i2 = 2;
   A.i3 = 3;
   A.i4 = 4;
   
   // Encode TestDefinedByUsage into a buffer
   //
   if (! A.BEnc(buf))
      throw SNACC_EXCEPT("Encode of TestDefinedByUsage failed!");

   // Decode buffer into a different TestDefinedByUsage to demonstrate the
   // automatic decoding the ANY DEFINED BY.
   //
   unsigned long bytesDecoded=0;
   A2.BDec(buf, bytesDecoded);

   // Check the OID to determine if this is the ANY you are looking for.
   //
   if (A2.id == testOID2)
   {                     
      // You must cast the ANY to the type you expect.  It's up to the
      // application to do the proper casting.
      //
      pPrtblStr = (PrintableString *)A2.anyDefBy.value;

      std::cout << "ANY_DEFINED_BY_test: Good id, == testOID2." 
         << pPrtblStr->c_str() << "\n";
   }
   else
   {
      std::cout << "ANY_DEFINED_BY_test: ***** Bad id, EXPECTED testOID2.\n";
   }

   std::cout.flush();

   A.id = testOid3_UNKNOWN;
   //A.anyDefBy.value = new AsnAnyBuffer((char *)data, len);
   
   buf.ResetMode();
   if (! A.BEnc(buf))
      throw SNACC_EXCEPT("Encode of TestDefinedByUsage failed!");


   // There are three methods for assigning an AsnType to an ANY
   // 
   //
   // METHOD 1
   //
   // This demonstrates the preferred method of storing any valid AsnType
   // into an ANY.

   // Encode buffer into ANY.
   A.id = testOid3_UNKNOWN;

   AsnBuf Buf2;
   if (! prtblStr.BEnc(Buf2))
      throw SNACC_EXCEPT("Encode into ANY failed!");
   A.anyDefBy.BDec(Buf2, bytesDecoded);

   // clean up 
   delete A.anyDefBy.value;
   A.anyDefBy.value = NULL;

   // METHOD 2
   //
   // Now if this is an ANY DEFINED BY you don't have to encode the AsnType
   // into the ANY.  You copy the AsnType that corresponds to the oid in the 
   // (id).  NOTE:: This can cause an application to crash if hand-set by the 
   //               developer.  Be Careful!
   //
   A.id = testOid3_UNKNOWN;
   A.anyDefBy.value = prtblStr.Clone();
   
   // clean up 
   delete A.anyDefBy.value;
   A.anyDefBy.value = NULL;

   // METHOD 3
   //
   // Assign a buffer containing the ASN.1 to an ANY.
   //
   A.id = testOid3_UNKNOWN;
   if (pPrtblStr)
   {
       AsnBuf buf4;
       pPrtblStr->BEnc(buf4);
       A.anyDefBy.BDec(buf4, bytesDecoded);
       A3 = A;          // FOR next test...
   }        // END IF pPrtblStr
   
   // clean up
   delete A.anyDefBy.value;
   A.anyDefBy.value = NULL;

   // ##################################

            // "ai" is set ONLY if the OID is not in ANY DEFINED BY table OR
            //   if the definition is ANY (not ANY DEFINED BY).
   if (A3.anyDefBy.ai == NULL)     //THEN, we did not recognize the OID.A3.id == testOID2)
   {
      AsnBuf Buf2;
      A3.anyDefBy.BEnc(Buf2);
      if (A3.id == testOid3_UNKNOWN)// WE KNOW WHAT IT IS and have to check explicitely
      {
         PrintableString tmpPrtblStr;

         unsigned long bytesDecoded=0;
         tmpPrtblStr.BDec(Buf2, bytesDecoded);

         std::cout << "ANY_DEFINED_BY_test: Good id, == testOid3_UNKNOWN." 
            << tmpPrtblStr.c_str() << "\n";
      }
   }
   else
   {
      std::cout << "ANY_DEFINED_BY_test: Bad id, <> testOid3_UNKNOWN.\n";
   }
   std::cout.flush();
}


//
//
//  THIS Version tests the newly added ability of the SNACC compiler to handle
//  multiple Object-Type definitions in separate .asn files.
void ANY_DEFINED_BY_test_2()
{
   FUNC("ANY_DEFINED_BY_test()");
   
   AsnOid testOid3_UNKNOWN("1.2.3.4.5.5.5.5.6");

   AsnBuf buf;

   TestDefinedByUsage_2 A, A2, A3;

   
   PrintableString prtblStr;
   PrintableString *pPrtblStr = NULL;

   prtblStr = "THIS IS A TEST";

   A.id = testOID2_2;
   A.anyDefBy.value  = prtblStr.Clone();

   // Test encoding and decoding of type and contains an ANY DEFINED BY.
   //
   if (! A.BEnc(buf))
   {
      throw SNACC_EXCEPT("Encode of TestDefinedByUsage_2 failed!");
   }

   unsigned long bytesDecoded=0;
   A2.BDec(buf, bytesDecoded);


   // Test extraction of ANY DEFINED BY  
   //

   // First check the oid to make to determine the syntax that is
   // stored in the ANY. It's up to the application to perform this
   // check correctly.
   //
   if (A2.id == testOID2_2) 
   {                       
      // Is this is an ANY DEFINED by the syntax is already decoded into the
      // correct ASN.1 type.  It up to the application to perform the 
      // proper casting though.  Note: if a copy is desired call the Clone()
      // method and then cast.
      //
      pPrtblStr = (PrintableString *)A2.anyDefBy.value;
      std::cout << "ANY_DEFINED_BY_test: Good id, == testOID2_2." 
         << pPrtblStr->c_str() << "\n";
   }
   else
   {
      std::cout << "ANY_DEFINED_BY_test: Bad id, <> testOID2_2.\n";
   }
   std::cout.flush();


   // Use this method to assign a previously encoded value to the ANY DEFINED BY.
   // Note: buf contains an encoded value from above
   //
   A3.id = testOid3_UNKNOWN;
   buf.ResetMode();
   A3.anyDefBy.BDec(buf, bytesDecoded);

   // Use this method to enode any AsnType into an AsnAny (or AsnAnyDefinedBy)
   //
   if (! prtblStr.BEnc(buf))
      throw SNACC_EXCEPT("Encoding of PrintableString into AsnAny failed!");
   A.anyDefBy.BDec(buf, bytesDecoded);
}


//
//
void XML_test()
{
   FUNC("XML_test()");
   
   std::strstream os;
   TestSequence testSequence;
   AsnOcts TmpSNACCOcts;
   AsnBuf TmpBuf;
   AsnBuf buf;
   char Aarray[]= { (char )0x30,(char )0x2d,(char )0x02,(char )0x15,
       (char )0x00,(char )0x94,(char )0x76,(char )0x92,(char )0xae,
       (char )0x87,(char )0x71,(char )0x0e,(char )0x50,(char )0xf9,
       (char )0xd5,(char )0x35,(char )0x49,(char )0xda,(char )0xa2,
       (char )0x40 };

  
   /*TestAllAsnPrimativeTypes ::= SEQUENCE {
	  octs OCTET STRING,
	  boolTest BOOLEAN,
     oid OBJECT IDENTIFIER OPTIONAL,
     bitString [2] IMPLICIT BIT STRING,
	  integer INTEGER,
	  enumTest ENUMERATED {A(11), B(12), C(13) },
	  real REAL OPTIONAL,
	  null NULL
   }*/

   testSequence.testAllPrimatives.octsName.Set(Aarray, sizeof(Aarray));
   
   testSequence.testAllPrimatives.boolTestName = true;
   testSequence.testAllPrimatives.oidName = new AsnOid(testOID);
   char *pData="TES";//(char)0xa5, (char)0xa5, (char)0xa5};

   testSequence.testAllPrimatives.bitStringName.Set((const unsigned char*)pData, 24);

   testSequence.testAllPrimatives.integerName = 55;
   testSequence.testAllPrimatives.enumTestName = TestAllAsnPrimativeTypesEnum::aA;
   testSequence.testAllPrimatives.realName = new AsnReal(44.2);

   TmpSNACCOcts.Set("this is a test of SetOf", strlen("this is a test of SetOf")+1);
  
   AsnBuf Buf2;
   if (! TmpSNACCOcts.BEnc(Buf2))
      throw SNACC_EXCEPT("Encoding TmpSNACCOCts failed!");

   testSequence.testSetOfAny.insert(testSequence.testSetOfAny.end(),SNACC::AsnAny());
   unsigned long bytesDecoded=0;
   testSequence.testSetOfAny.back().BDec(Buf2, bytesDecoded);

   testSequence.directoryString.choiceId = DirectoryString::printableStringCid;
   testSequence.directoryString.printableString = new DirectoryString::PrintableString_;
   *testSequence.directoryString.printableString = "Printable String TEST";

//   testSequence.testSet = new VDATestSet;  // ALLOW all defaults...

   if (! testSequence.BEnc(buf) )
      throw SNACC_EXCEPT("Encoding of testSequence failed!");

   std::cout << "######################################\n";
   std::cout << "###   PrintXML test ##################\n";

   std::ofstream ofs("vdatest.xml");
   testSequence.PrintXML(ofs);
   
   std::cout << "###   END PrintXML test ##############\n";
   std::cout << "######################################\n";
   std::cout.flush();
}


int append_test()
{
   FUNC("append_test()");

   VDATestSequence testSeq;
   VDATestSet      testSet;
   VDATestSet      testSet2;
   VDATestSet      testSetBER;
   VDATestSetOfAny testSetOfAny;
   char            testSeqBlobTbl[] = { 0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02 };
   // SET OF Integers, 04 tag, 01 length, 01 then 02 data.
   char            testSetBlobTbl[] = { 0x31, 0x06, 0x02, 0x01, 0x01, 0x04, 0x01, 0x02 };
   char            testSetBlobTblVaryLengths[] = 
                    { 0x31, 54, 
                      0x04, 0x01, 0x71, 
                      0x04, 0x02, 0x72, 0x72, 
                      0x04, 0x03, 0x72, 0x73, 0x74, 
                      0x04, 0x03, 0x74, 0x72, 0x73, 
                      0x04, 0x05, 0x74, 0x72, 0x73, 0x73, 0x73,
                      0x04, 0x03, 0x74, 0x72, 0x79,
                      0x04, 0x03, 0x74, 0x72, 0x78,
                      0x04, 0x03, 0x74, 0x72, 0x77,
                      0x04, 0x03, 0x74, 0x72, 0x76,
                      0x04, 0x03, 0x74, 0x72, 0x75,
                      0x04, 0x03, 0x74, 0x72, 0x74
                    };      // THESE SHOULD all be ordered
   char            testSetBlobBERTbl[] = { 0x31, 0x06, 0x02, 0x01, 0x02, 0x04, 0x01, 0x01 };
   //RWC;? 0x03?;char            testSetOfAnyBlobTbl[] = { 0x31, 0x08, 0x03, 0x02, 0x04, 0x0F, 0x03, 0x02, 0x05, 0x0E };
   char            testSetOfAnyBlobTbl[] = { 0x31, 0x08, 0x04, 0x02, 0x04, 0x0F, 0x04, 0x02, 0x05, 0x0E };
   char            testNullBitString[] = { 0x03, 0x01, 0x00 };
   char            testBit1BitString[] = { 0x03, 0x02, 0x02, 0x20}; // 2 unused bits, data 0x20 (2nd bit set).
   AsnOid          TestOid;



   // the following tables are DER encoded results for 
   // VDATestSequence, VDATestSet, and VDATestSetOfAny.  Decode
   // these tables and then re-encode them to determine if we
   // come up with the same results.
   //
   // note: this doesn't test every scenario.  
   //
   AsnBuf buf;
   AsnBuf testNullBitStringBlob(&testNullBitString[0], 3);
   AsnBuf testNullBitStringBlob2;
   AsnBits    snaccNullBitString(8); // 8 bits with nothing set
   VDATestBitString snaccNullBitString3(8);

   // RWC;12/18/00; THIS SET of tests require that the Named Bit rules
   //   be applied.  Named Bit rules are set only for declared ASN SNACC
   //   compiler processed Bit Strings, not for "AsnBits" declared vars
   //   as in "snaccNullBitString" above.
   // THIS 1st test should not produce 030100.
   //ENCODE_BUF_NO_ALLOC(&snaccNullBitString3, &testNullBitStringBlob2);
   SequenceOfDefaults AA;
   AA.defBitstring = new ClassList(8);
   if (! AA.defBitstring->BEnc(buf) )
      throw SNACC_EXCEPT("Encoding of DEFAULT BIT STRING failed!");

   if (buf == testNullBitStringBlob)
   {
      std::cout << "NULL BIT STRING 1 TEST PASSED!\n";
      std::cout.flush();
   }
   else
   {
      std::cout << "NULL BIT STRING TEST 1 FAILED!\n";
      std::cout << "ENCODED VALUE IS:\n";
      buf.hexDump(std::cout);
      std::cout << "\nIT SHOULD BE:\n";
      testNullBitStringBlob.hexDump(std::cout);
      std::cout << "\n";
      std::cout.flush();
   }

   if (! snaccNullBitString3.BEnc(buf) )
      throw SNACC_EXCEPT("Encoding of NULL BIT STRING failed!");

   if (buf == testNullBitStringBlob)
   {
      std::cout << "NULL BIT STRING 2 TEST PASSED!\n";
      std::cout.flush();
   }
   else
   {
      std::cout << "NULL BIT STRING TEST 2 FAILED!\n";
      std::cout << "ENCODED VALUE IS:\n";
      buf.hexDump(std::cout);
      std::cout << "\nIT SHOULD NOT BE:\n";
      testNullBitStringBlob.hexDump(std::cout);
      std::cout << "\n";
      std::cout.flush();
   }

   // Now let's test a bits string that has more than one octet that is null.
   // The resulting encoding should look like the testNullBitString above.
   AsnBuf bufAA;
   AA.defBitstring->Set(17);

   if (! AA.defBitstring->BEnc(bufAA) )
      throw SNACC_EXCEPT("Encoding of NULL BIT STRING failed");

   if (bufAA == testNullBitStringBlob)
   {
      std::cout << "Multiple null octets NULL BIT STRING test 3 passed!\n";
      std::cout.flush();
   }
   else
   {
      std::cout << "Multiple null octets NULL BIT STRING TEST 3 FAILED!\n";
      std::cout << "ENCODED VALUE IS:\n";
      bufAA.hexDump(std::cout);
      std::cout << "\nIT SHOULD BE:\n";
      testNullBitStringBlob.hexDump(std::cout);
      std::cout << "\n";
      std::cout.flush();
   }

   // Test BIT STRING with only the first bit set.
   //
   AsnBuf  tmpBuf(testBit1BitString, 4);
   AsnBuf  tmpBlob;
   AsnBits     snaccBitStr;

   snaccBitStr.UseNamedBitListRules(false);
   snaccBitStr.Set(6);
   snaccBitStr.SetBit(2);
   
   if (snaccBitStr.IsEmpty())
      std::cout << "ERROR: Bitstring is empty!\n";
   
   AsnBuf bufAA2;
   if (! snaccBitStr.BEnc(bufAA2) )
      throw SNACC_EXCEPT("Encoding of BIT with only 1st bit set failed!");
   
   if (bufAA2 == tmpBuf)
   {
      std::cout << "Bitstring with first bit set and uneven bit count PASSED!\n";
      std::cout.flush();
   }
   else
   {
      std::cout << "Bitstring with first bit set and uneven bit count FAILED!\n";
      std::cout << "BitString Encoded as:\n";
      bufAA2.hexDump(std::cout);
      std::cout << "It should be:\n";
      tmpBuf.hexDump(std::cout);
      std::cout << "\n";
      std::cout.flush();
   }

   memset(&snaccBitStr, 0, sizeof(snaccBitStr));
   
   if (! snaccBitStr.IsEmpty())
      std::cout << "ERROR: Bitstring expected to by empty!\n";

   // sequence of to integers
   //
   AsnBuf testSeqBlob( &testSeqBlobTbl[0], 8); 
   AsnBuf testSeqBlob2;

   // sequence of two octet strings
   //
   AsnBuf testSetBlob( &testSetBlobTbl[0], 8);
   AsnBuf testSetBlobBER( &testSetBlobBERTbl[0] , 8);
   AsnBuf testSetBlobVaryingLengths( &testSetBlobTblVaryLengths[0], 56);
   AsnBuf testSetBlob2;
   AsnBuf testSetBlobDER2;

   // sequence of two ANYs.  In this case the any's are two BIT STRINGS
   //
   AsnBuf testSetOfAnyBlob( &testSetOfAnyBlobTbl[0] , 10);
   AsnBuf testSetOfAnyBlob2;

   // Decode, Encode, and Compare Test Sequence
   //

   unsigned long bytesDecoded=0;
   testSeq.BDec(testSeqBlob, bytesDecoded);

   if (! testSeq.BEnc(testSeqBlob2) )
      throw SNACC_EXCEPT("Re-encode of testSeq failed!");

   if (testSeqBlob2 == testSeqBlob)
      std::cout << "Passed Sequence test ...\n";
   else
      std::cout << "Failed Sequence test ...\n";

   // Decode, Encode, and Compare Test Set
   //
   testSet.BDec(testSetBlob, bytesDecoded);
   testSetBlob.ResetMode();

   if (! testSet.BEnc(testSetBlob2) )
      throw SNACC_EXCEPT("Re-encode of testSet failed!");

   testSetBlob2.hexDump(std::cout);
   std::cout<<std::endl;
   testSetBlob.hexDump(std::cout);
   std::cout<<std::endl;

   if ( testSetBlob2 == testSetBlob)
      std::cout << "Passed SET test ... \n";
   else
      std::cout << "Failed SET test ... \n";

   // This test should sort the various SET OF integers of varying lengths,
   //  REGARDLESS OF LENGTH (per X.690 specification section 11.6).
   //
   
   testSet.BDec(testSetOfAnyBlob, bytesDecoded);


   AsnBuf testSetBlob3;
   /**RWC;TBD;DOES NOT RESET BUFFER FOR WRITING??????@#!##$#$;
   testSetBlob2.ResetMode(std::ios_base::out);
   testSetBlob2.ResetMode(); **RWC;TBD;WHY????*/
   if ( ! testSet.BEnc(testSetBlob3) )
      throw SNACC_EXCEPT("Re-encode of testSet failed!");

   testSetBlob3.ResetMode();  //RWC;TBD;PIERCE!!!! ERROR HERE, WITHOUT THIS, the 
                              //   buffer returns 3 extra pre-pended bytes, BAD!!!
   char *ptr=testSetBlob3.GetSeg(testSetBlob3.length());
   delete ptr;
   testSetBlob3.ResetMode();
   testSet2.BDec(testSetBlob3, bytesDecoded);

   testSet2.Print(std::cout);
   std::cout.flush();

   // Decode, Encode, and Compare Test SET OF ANY
   //
   //RWC;10/07/00;
   testSetOfAnyBlob.ResetMode();
   testSetOfAny.BDec(testSetOfAnyBlob, bytesDecoded);

   if (!  testSetOfAny.BEnc(testSetOfAnyBlob2) )
      throw SNACC_EXCEPT("Re-encode of testSetOfAny failed!");

   if (testSetOfAnyBlob2 == testSetOfAnyBlob)
      std::cout << "Passed SET OF ANY test ...\n";
   else 
      std::cout << "Failed SET OF ANY test ...\n";

   // Decode BER, Encode DER, and Compare Encoded DER
   // to known DER encoding.
   //
   testSetBER.BDec(testSetBlobBER, bytesDecoded);

   if (! testSetBER.BEnc(testSetBlobDER2) )
      throw SNACC_EXCEPT("Error re-encoding testSetBER");

   testSetBlobBER.ResetMode();

   testSetBlobDER2.hexDump(std::cout);
   testSetBlobDER2.ResetMode();
   std::cout<<std::endl;

   testSetBlobBER.hexDump(std::cout);
   testSetBlobBER.ResetMode();
   std::cout<<std::endl;

   if (testSetBlobDER2 == testSetBlobBER)
      std::cout << "Passed DER test ...\n";
   else
   {
      std::cout << "Failed DER test ...\n";
     if (testSetBlobDER2 == testSetBlobBER)
        std::cout << "\tERROR: BER decoded and re-encode as BER insteaded of DER\n";
     else
        testSetBlobDER2.hexDump( std::cout );
   }

   TestOid.Set("1.2.3.4.5.0.0.0");
   std::cout << "TestOid = " << TestOid << "\n";
   TestOid.Set("1.2.3.4.5.6.8.8.8");
   std::cout << "TestOid = " << TestOid << "\n";



   //RWC; TEST Override for "<<" operator to avoid stdout prints...
   //RWC;  Override is only used within SNACC generated code, not in snacc 
   //RWC;  run-time includes.
   //RWC;  Force failure
   std::cout << "######### THE NEXT SET OF STATEMENTS SHOULD FAIL, YOU WILL ####\n";
   std::cout << "#########     SEE THE ERROR FROM SNACC AT THE TOP OF THE   ####\n";
   std::cout << "#########     STACK                                        ####\n";
   VDATestSequence aa;
   AsnBuf cBuf("aaa", 3);
   aa.BDec(cBuf, bytesDecoded); // expected to fail

   return 1;
} /* append_test */



void outhex(unsigned char *outChar, int Len)
{
	int i = 0;
	std::cout.flush(); // Flush the buffer
   std::cout.setf(std::ios::hex);
	 for (i=0;  i < Len; i++)
	{
		printf ("0x%x ", outChar[i]);
		std::cout << " ";
	}
	printf ("\n");

}

int utf8compare (unsigned char *expected, wchar_t *actual, int Len)
{
long value = 1;
int stat = 0;
#define TWO
#define WCHAR_SIZE sizeof (wchar_t)
#ifdef WIN32
	unsigned short *buf;
#else
	unsigned long *buf;
#endif

	/* Check which byte the value 1 is in (Little Endian stuff)*/
	if (*(char *)&value == 1)
	{
#ifdef WIN32

			/* We have to flip the wchar_t for comparison */

			int		j;
			unsigned short   tempnum;
			buf = (unsigned short *)actual;

			for ( j = 0; j < Len/2; j++ )
			{
				tempnum = (unsigned short)((buf[j] << 8) | (buf[j] >> 8));
				buf[j] = tempnum;
			}
		
#else
			int		j;
			unsigned long   tempnum;
			buf = (unsigned long *)actual;

			 for ( j = 0; j < Len/4; j++ )
			 {
				   tempnum = (buf[j] << 16) | (buf[j] >> 16);
					buf[j] = ((tempnum & 0xff00ff00L) >> 8) | ((tempnum & 0x00ff00ffL) << 8);
			 }
#endif
	}
	else
#ifdef WIN32
		buf = (unsigned short *)actual;
#else
		buf = (unsigned long *)actual;
#endif

	// Compare the strings
	stat = strncmp((const char *)expected, (const char *)actual, Len);
	return stat;
}
void Test_ASN_1_Strings()
{
// RFC 2253 Test Data
	//unsigned char  RFC2253TestData[] = { 0x4c, 0x75, 0xC4, 0x8d, 0x69, 0xC4, 0x87, 0x00, 0x00 };
	//const int RFC2253TestDataLen = 9;

// RFC 2279 Test Data Section 4 (Examples) Example 1
	unsigned char  RFC2279TestData1[] = { 0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x91, 0x2E, 0x00 };
	unsigned char RFC2279TestResult1[] = {0x00, 0x41, 0x22, 0x62, 0x03, 0x91, 0x00, 0x2E };

// RFC 2279 Test Data Section 4 (Examples) Example 2
   unsigned  char RFC2279TestData2[] = { 0xED ,0x95 ,0x9C ,0xEA, 0xB5, 0xAD, 0xEC, 0x96, 0xB4,0x00 };
	unsigned  char RFC2279TestResult2[] = { 0xD5, 0x5C, 0xAD, 0x6D, 0xC5, 0xB4 };

// RFC 2279 Test Data Section 4 (Examples) Example 3 
	unsigned  char RFC2279TestData3[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E };
	unsigned char RFC2279TestResult3[] = { 0x65, 0xE5, 0x67, 0x2C, 0x8A, 0x9E };

// RFC 2279 Test Data Section 4 (Examples) Example 4
	//unsigned  char UTF8TestData4[] = { 0x00,0x4c,0x80, 0x80,0x01, 0x0d, 0x00, 0x69, 0x01, 0x07, 0x00, 0x00 };

//	unsigned  char RFC2279TestResult4[] = { 0x
	const int RFC2279TestData1Len = 8;
	const int RFC2279TestData2Len = 10;
	//const int RFC2279TestData3Len = 9;
	//const int RFC2279TestData4Len = 12;
	const int RFC2279TestResult1Len = 8;
	const int RFC2279TestResult2Len = 6;
	const int RFC2279TestResult3Len = 6;

// Universal String is 4 bytes long
   /*char  universal_string[] = 
   { 0x00, 0x00,0x00,0x4c,0x00, 0x00,0x00, 0x75,0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x01, 0x07, 0x00, 0x00,0x00,0x00 }; */
   wchar_t wc_universal_string[] = { 0x0000004c, 0x00000075, 0x0000010d, 0x00000069, 0x00000107, 0x00000000 };
   char  bmp_string[] = {0x00, 'T', 0x00, 'E', 0x00, 'S', 0x00, 'T'};
   std::wstring bmpTest;
   int universal_stringLen = 24;
   long status=0;
   UniversalString unvStr;
   PrintableString prtblStr;
   PrintableString prtblStr2;

   UTF8String C;
   PrintableString AP;
	BMPString D;
   wchar_t *pWchar=NULL;
	char *tempChar = NULL;
   int i;
   std::string utf8Form;
   
   std::cout << "######### Test_ASN_1_Strings:   START OF TEST          ####\n";
   

   unvStr = &wc_universal_string[0];
   
   // Excercise UniversalString code
   //
   std::cout << "UniversalString:\n";
   unvStr.getAsUTF8(utf8Form);
   std::cout << "UTF8 Form:\n" << utf8Form.c_str() << "\n";

   std::cout << "UniversalString Results \n";
   std::cout.setf(std::ios::hex);
   tempChar = (char *)pWchar;

   for (i=0;  i < universal_stringLen+1/2; i++)
	{
      std::cout << "0x"<< (int)tempChar[i];
      std::cout << " ";
		if  ((tempChar[i+1] == 0x00) &&
			 (tempChar[i+2] == 0x00) &&
		 	 (tempChar[i+3] == 0x00) &&
			 (tempChar[i+4] == 0x00))
			i = universal_stringLen+10; // Exit

	}

   std::cout << "\n";
   std::cout.setf(std::ios::dec);
   std::cout.flush();
   std::cout << "######### Test_ASN_1_Strings:   END OF TEST           ####\n\n";
   std::cout << "######### Test_ASN_1_Strings:   START OF TEST           ####\n\n";
   std::cout << "\n";

  //   cout << "Test_ASN_1_Strings: pWchar of A=" << TmpChar << "\n";
   
   // Exercise PrintableString
   //

   prtblStr = "blah PrintableString";

   std::cout << "PrinableString:\nnarrow form: " << prtblStr.c_str() << "\n";

   std::cout << "######### Test_ASN_1_Strings:   START OF TESTS          ####\n";
   std::cout << "Test 1 input data is: \n";
   std::cout.setf(std::ios::hex);

	outhex (RFC2279TestData1, RFC2279TestData1Len);

   std::cout << "\n";
   std::cout.setf(std::ios::dec);
   std::cout.flush();

   prtblStr2 = (const char *)&RFC2279TestData1[0];

   //utf8Str = prtblStr2;
   C = AP;             // TEST Multi-byte assignment to single-byte ASN.1 def.
   
   std::cout << "Test 1 Expected Result:\n";

   outhex(RFC2279TestResult1, RFC2279TestResult1Len);
	
   C.set((char *)RFC2279TestData1);//, RFC2279TestData1Len);
   std::wstring wideString;
   wideString = C; // WORKS; Transform the UTF-8 to Wide Character
   char *ptr2=C.getAsUTF8();

   std::cout << "\nTest 1 Actual Test Result(Intel platforms will have bytes flipped):\n";
	if (ptr2 && wideString.length())
	{
		outhex((unsigned char *)ptr2, RFC2279TestResult1Len);
		status = utf8compare (RFC2279TestResult1, (wchar_t*)wideString.c_str(), RFC2279TestData1Len);
		if (status != 0)
         std::cout << "Actual Results do not compare with expectd results\n";
		else
         std::cout << "######### Results compare\n";
	}
	else
      std::cout << "ptr2 is NULL, which is in error\n";
   
   std::cout << "\n";
   std::cout << "Test 2 input data is: \n";
   std::cout.setf(std::ios::hex);

   outhex (RFC2279TestData2, RFC2279TestData2Len);

   std::cout << "\n";
   std::cout.setf(std::ios::dec);
   std::cout.flush();
   AP = (char *)RFC2279TestData2;
   
   C = AP;             // TEST Multi-byte assignment to single-byte ASN.1 def.
	
   std::cout << "Test 2 Expected Result:\n";
	
   outhex (RFC2279TestResult2, RFC2279TestResult2Len);
	
   C = (char *)RFC2279TestData2;
   wideString = C; // Transform the UTF-8 to Wide Character
   ptr2 = C.getAsUTF8();
	
   std::cout << "\nTest 2 Actual Test Result(Intel platforms will have bytes flipped):\n";
	
   if (ptr2 != NULL && wideString.length() != 0)
	{
		outhex ((unsigned char *)ptr2, RFC2279TestResult2Len);
		status = utf8compare (RFC2279TestResult2, (wchar_t*)wideString.c_str(), RFC2279TestResult2Len);
		if (status != 0)
         std::cout << "Actual Results do not compare with expectd results\n";
		else
         std::cout << "######### Results compare\n";

	}
	else
      std::cout << "wideString is NULL, which is in error\n";
   
   std::cout << "\n";

   std::cout << "\n";
   std::cout << "Test 3 input data is: \n";
   std::cout.setf(std::ios::hex);

   outhex (RFC2279TestData2, RFC2279TestData2Len);

   std::cout << "\n";
   std::cout.setf(std::ios::dec);
   std::cout.flush();

   AP = (char *)RFC2279TestData3;
   
   C = AP;             // TEST Multi-byte assignment to single-byte ASN.1 def.
	
   std::cout << "Test 3 Expected Result:\n";
	
   outhex (RFC2279TestResult3, RFC2279TestResult3Len);
	
   //C.Set((char *)RFC2279TestData3, RFC2279TestData3Len);
   C = (char *)RFC2279TestData3;
   wideString = C; // Transform the UTF-8 to Wide Character
   ptr2 = C.getAsUTF8();
   
   std::cout << "\nTest 3 Actual Test Result(Intel platforms will have bytes flipped):\n";
	
   if (ptr2 && wideString.length() != 0)
	{
		outhex ((unsigned char *)ptr2, RFC2279TestResult3Len);
	
      status = utf8compare (RFC2279TestResult3, (wchar_t*)wideString.c_str(), RFC2279TestResult3Len);
		
      if (status != 0)
         std::cout << "Actual Results do not compare with expectd results\n";
		else
         std::cout << "######### Results compare\n";
	}
	else
      std::cout << "wideString.c_str() is NULL, which is in error\n";
   
   std::cout << "\n";
   
   std::cout << "######### Test_ASN_1_Strings:   END OF TEST 3          ####\n\n";
   std::cout << "\n";

   std::cout << "######### Test_ASN_1_Strings:   START OF TESTS          ####\n";
   std::cout << "Test 4 input data is: \n";
	
   outhex ((unsigned char *)bmp_string, 8);
	
   std::cout << "\n";
   bmpTest += 'T';
   bmpTest += 'E';
   bmpTest += 'S';
   bmpTest += 'T';
   //D.Set((char *)bmpTest, 8);
   D = bmpTest;
	
   std::wstring tmpwString;
   tmpwString = D;
   ptr2 = D.getAsUTF8();

   std::cout << "Test 4 actual BMPString -> wstring is:\n";
   outhex ((unsigned char *)ptr2, 8);
   std::cout << "\n";
   std::cout << "######### Test_ASN_1_Strings:   END OF TEST 4          ####\n\n";
   std::cout << "\n";
   std::cout.flush();
}


void    test_big_buffer()
{
    char pchBuffer[200000];
    AsnBuf *pM=new AsnBuf;
    AsnOcts N;
    unsigned long bytesDecoded=0;


   memset(pchBuffer, 'A', 200000);
   N.Set(pchBuffer, 200000);
   N.BEnc(*pM);

   std::cout << "test_big_buffer: Octet STring encoded length of 200000 byte buf=" 
       << pM->length() << "\n";

   N.BDec(*pM, bytesDecoded);

   delete pM;

}


//


#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
void test_timingDoTestPrint(long BYTE_COUNT, long iCount, int new_flag, long lTestType);
void test_timing()
{

#ifdef TESTS_ALREADY_DONE
   test_timingDoTestPrint(90000, iCount, 0, TEST_ENCODE_BUF);
   test_timingDoTestPrint(90000, iCount, 1, TEST_ENCODE_BUF);
   test_timingDoTestPrint(90000, iCount, 0, TEST_DECODE_BUF);
   test_timingDoTestPrint(90000, iCount, 1, TEST_DECODE_BUF);
   test_timingDoTestPrint(90000, iCount, 0, TEST_ENCODE_BUF);
   test_timingDoTestPrint(90000, iCount, 0, TEST_DECODE_BUF);
   test_timingDoTestPrint(90000, iCount, 2, TEST_ENCODE_BUF);
   test_timingDoTestPrint(90000, iCount, 2, TEST_DECODE_BUF);
#endif

   //RELEASE Version
   //test_timing: Start.
   //test_timing:  OLD ENCODE_BUF iCount=1000, Time=10.160000, single=0.010160 Size=90000bytes.
   //test_timing: Start.
   //test_timing:  NEW ENCODE_BUF iCount=1000, Time=2.640000, single=0.002640 Size=90000bytes.
   //DEBUG Version
   //test_timing: Start.
   //test_timing:  OLD ENCODE_BUF iCount=1000, Time=14.060000, single=0.014060 Size=90000bytes.
   //test_timing: Start.
   //test_timing:  NEW ENCODE_BUF iCount=1000, Time=6.860000, single=0.006860 Size=90000bytes.
   //test_timing: Start.
   //test_timing:  OLD DECODE_BUF iCount=1000, Time=7.910000, single=0.007910 Size=90000bytes.
   //test_timing: Start.
   //test_timing:  NEW DECODE_BUF iCount=1000, Time=1.210000, single=0.001210 Size=90000bytes.

}

double test_timingDoTest(long BYTE_COUNT, long iCount, int new_flag, long lTestType);
void test_timingDoTestPrint(long lByteCount, long iCount, int new_flag, long lTestType)
{
   double dSingle;
   double dResult;
   char *lpszNewFlag;
   char *lpszTypeFlag="TEST";
   dResult = test_timingDoTest(lByteCount, iCount, new_flag,lTestType);
   dSingle = dResult / iCount;
   if (new_flag)
      lpszNewFlag = "NEW";
   else
      lpszNewFlag = "OLD";
   if (lTestType == TEST_ENCODE_BUF)
      lpszTypeFlag = "ENCODE_BUF";
   else if (lTestType == TEST_DECODE_BUF)
      lpszTypeFlag = "DECODE_BUF";
   printf("test_timing:  %s %s iCount=%d, Time=%f, single=%f Size=%ldbytes.\n", 
      lpszNewFlag, lpszTypeFlag, iCount, dResult, dSingle, lByteCount);
}

#ifndef WIN32		//artificially fix symbols.
#define _timeb timeb
#define _ftime ftime
#endif
double test_timingDoTest(long BYTE_COUNT, long iCount, int new_flag, long lTestType)
{
   double dTime, dTime2, dSingle;
   struct _timeb timebuffer;
   UTF8String AA;
   UTF8String *pAA;  // CANNOT use "AA" due to SNACC limitation where it does
                     //  not free previous memory on subsequent decodes!!!#@$%
   AsnBuf *pBB=NULL;
   AsnBuf *pCANNEDTmpEncodedBuf=NULL;
   AsnBuf *pCANNEDEncodedBuf=NULL;
   int i;
   char *pChar=(char *)calloc(1, BYTE_COUNT);
   unsigned long bytesDecoded=0;

   printf("test_timing: Start.\n");
   memset((void *)pChar, 'R', BYTE_COUNT);
   //AA.Set("Test 1", strlen("Test 1"));
   AA = (const char *)pChar;
   free((void *)pChar);
   AA.BEnc(*pCANNEDTmpEncodedBuf); // FOR DECODE_BUF ops.
   pCANNEDEncodedBuf = new AsnBuf(*pCANNEDTmpEncodedBuf);
   delete pCANNEDTmpEncodedBuf; // need to copy to avoid oversized buffer 
                                //  from ENCODE_BUF ops.

       
   _ftime( &timebuffer);
   dTime = timebuffer.time;
   dTime += (.001) * timebuffer.millitm;



   for (i=0; i < iCount; i++)
   {
      pBB = NULL;

      if (new_flag==2)      // newest, from Pierce (RWC).
      {
         if (lTestType == TEST_ENCODE_BUF)
         {
            AA.BEnc(*pBB);
            delete pBB;
         }
         else if (lTestType == TEST_DECODE_BUF)
         {
            pAA = new UTF8String;
            pAA->BDec(*pCANNEDEncodedBuf, bytesDecoded);
            delete pAA;
         }
      }
      else if (new_flag)
      {
         if (lTestType == TEST_ENCODE_BUF)
         {
            AA.BEnc(*pBB);
            delete pBB;
         }
         else if (lTestType == TEST_DECODE_BUF)
         {
            pAA = new UTF8String;
            pAA->BDec(*pCANNEDEncodedBuf, bytesDecoded);
            delete pAA;
         }
      }
      else
      {
         if (lTestType == TEST_ENCODE_BUF)
         {
            AA.BEnc(*pBB);
            delete pBB;
         }
         else if (lTestType == TEST_DECODE_BUF)
         {
            pAA = new UTF8String;
            AA.BDec(*pCANNEDEncodedBuf, bytesDecoded);
            delete pAA;
         }
      }
   }

   _ftime( &timebuffer);
   dTime2 = timebuffer.time;
   dTime2 += (.001) * timebuffer.millitm;
   dSingle = dTime2 - dTime;


   
  return(dSingle);
    
}


// This function demonstrates/tests the new AsnInt BigInteger logic.
void AsnIntTest()
{
    AsnInt A,B,H;
    int /*AsnIntType*/ CInt;
    AsnBuf b;
    AsnLen bytesDecoded=0;
    char TmpData[7]={0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x00 };
    bool bFlag;
    int i;

    //######### convert string into Hex ##########################
    H = AsnInt("0x20FF");
    if (H == 767)
        std::cout << "\nAsnIntTest:  SUCCESSFUL string to hex conversion, " << H 
             << ".\n";
    else
        std::cout << "AsnIntTest:  UNSUCCESSFUL string to hex conversion, " << H 
             << ".\n";
    H = AsnInt("0xF0");
    if (H == 15)
        std::cout << "AsnIntTest:  SUCCESSFUL string to hex conversion, " << H 
             << ".\n";
    else
        std::cout << "AsnIntTest:  UNSUCCESSFUL string to hex conversion, " << H 
             << ".\n";

    //######### check small integer.##############################
    A = 20;
    A.BEnc(b);

    B.BDec(b, bytesDecoded);
    if (bytesDecoded)
    {
        if (B == 20)
            std::cout << "AsnIntTest:  SUCCESSFUL integer encode/decode, " << B 
                << ".\n";
        else
            std::cout << "AsnIntTest:  UNSUCCESSFUL integer encode/decode, " << B 
                << ".\n";
    }


    //######### check positive multi-byte integer.###########################
    //
    A.Set((const unsigned char *)TmpData, 7);
    A.BEnc(b);

    bytesDecoded = 0;
    B.BDec(b, bytesDecoded);
    if (bytesDecoded == 9 && B.length() == 7/*for Data*/)
    {
        bFlag = true;          // Start out assuming all data is good.
        const unsigned char *pBuf=B.c_str();
        for (int i=0; i < 7; i++)
            if (pBuf[i] != (unsigned char)TmpData[i])
                bFlag = false;
        if (bFlag)
        {
            std::cout << "AsnIntTest:  SUCCESSFUL 7 byte integer encode/decode, "
                << ".\n";
            char buf[200];
            sprintf(buf, "%xx%xx%xx%xx%xx%xx%xx.\n", pBuf[0], pBuf[2], 
                pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6]);
            std::cout << buf;
        }
        else
            std::cout << "AsnIntTest:  UNSUCCESSFUL 7 byte integer encode/decode, "
                << ".\n";
    }
    else
       std::cout << "AsnIntTest:  UNSUCCESSFUL length 7 byte integer encode/decode, bytesDecoded="
                << bytesDecoded << ".\n";

    //######### check assignment of multi-byte integer to a small int variable.
    //
      try
      {
       CInt = B;        // Attempt to assign 7 byte integer to 4 byte variable.
       std::cout << "AsnIntTest:  UNSUCCESSFUL ERROR on large integer assign to too small long."
                << ".\n";
      }
      catch (...)
      {
           std::cout << "AsnIntTest:  SUCCESSFUL ERROR (catch) on large integer assign to too small long."
                << ".\n";
      }

    //######### check negative small integer.##############################
    //
    bytesDecoded = 0;
    A = -20;
    A.BEnc(b);

    B.BDec(b, bytesDecoded);
    if (bytesDecoded)
    {
        if (B == -20)
            std::cout << "AsnIntTest:  SUCCESSFUL negative integer encode/decode, " << B 
                << ".\n";
        else
            std::cout << "AsnIntTest:  UNSUCCESSFUL negative integer encode/decode, " << B 
                << ".\n";
    }

    //######### check sign-extended negative large integer, with known size.##
    //
    char TmpData2[128];
    for (i=0; i < 128; i++)
        TmpData2[i] = (char)0xa0;   // negative MSB.
    TmpData2[0] = (char)0xff;
    TmpData2[1] = (char)0xff;
    TmpData2[2] = (char)0xff;
    TmpData2[3] = (char)0xff;
    bytesDecoded = 0;
    A.Set((unsigned char *)TmpData2, 128);
    A.BEnc(b);      // EXPECT 4 less bytes encoded due to sign extension.

    bytesDecoded = 0;
    B.BDec(b, bytesDecoded);
    if (B.length() == 124/*for Data*/)
    {
        bFlag = true;          // Start out assuming all data is good.
        const unsigned char *pBuf=B.c_str();
        for (i=0; i < 124; i++)
            if (pBuf[i] != (unsigned char)TmpData2[i])
                bFlag = false;
        if (bFlag)
        {
            std::cout << "AsnIntTest:  SUCCESSFUL 124 byte negative integer encode/decode, "
                << ".\n";
            char buf[200];
            sprintf(buf, "%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx.\n", pBuf[0], pBuf[2], 
                pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6]);
            std::cout << buf;
            unsigned char *pBuf3 = NULL;        // RESET for next operation.
            size_t length;
            B.getPadded(pBuf3, length, size_t(128));
            bFlag = true;          // Start out assuming all data is good.
            for (int i=0; i < 4; i++)
                if (pBuf3[i] != (char)0xff)
                    bFlag = false;
            if (!bFlag || length != 128)
                std::cout << "AsnIntTest:  UNSUCCESSFUL 128 byte negative integer GetSignedBitExtendedData(...).\n";
            else
                std::cout << "AsnIntTest:  SUCCESSFUL 128 byte negative integer GetSignedBitExtendedData(...).\n";
            if (pBuf3)
               free(pBuf3);
        }
        else
            std::cout << "AsnIntTest:  UNSUCCESSFUL 124 byte negative integer encode/decode, "
                << ".\n";
    }
    else
       std::cout << "AsnIntTest:  UNSUCCESSFUL length 124 byte integer encode/decode, bytesDecoded="
                << bytesDecoded << ".\n";

   
    //######### check sign-extended positive large integer, with known size.##
    //
    for (i=0; i < 128; i++)
        TmpData2[i] = (char)0x0a;   //NON-negative MSB.
    TmpData2[0] = (char)0x00;
    TmpData2[1] = (char)0x00;
    TmpData2[2] = (char)0x00;
    TmpData2[3] = (char)0x00;
    bytesDecoded = 0;
    A.Set((const unsigned char *)TmpData2, 128);
    A.BEnc(b);      // EXPECT 4 less bytes encoded due to sign extension.

    bytesDecoded = 0;
    B.BDec(b, bytesDecoded);
    if (B.length() == 124/*for Data*/)
    {
        bFlag = true;          // Start out assuming all data is good.
        {
        const unsigned char *pBuf= B.c_str();
        for (i=0; i < 124; i++)
            if (pBuf[i] != (unsigned char)TmpData2[i])
                bFlag = false;
        }
        if (bFlag)
        {
            unsigned char *pBuf;
            std::cout << "AsnIntTest:  SUCCESSFUL 124 byte integer encode/decode, "
                << ".\n";
            char buf[200];
            sprintf(buf, "%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx%2.2xx.\n", pBuf[0], pBuf[2], 
                pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6]);
            std::cout << buf;
            pBuf = NULL;        // RESET for next operation.
            size_t length;
            B.getPadded(pBuf, length, size_t(128));
            bFlag = true;          // Start out assuming all data is good.
            for (int i=0; i < 4; i++)
                if (pBuf[i] != (char)0x00)
                    bFlag = false;
            if (!bFlag || length != 128)
                std::cout << "AsnIntTest:  UNSUCCESSFUL 128 byte positive integer GetSignedBitExtendedData(...).\n";
        }
        else
            std::cout << "AsnIntTest:  UNSUCCESSFUL 124 byte positive integer encode/decode, "
                << ".\n";
    }
    else
       std::cout << "AsnIntTest:  UNSUCCESSFUL length 124 byte positive integer encode/decode, bytesDecoded="
                << bytesDecoded << ".\n";

   std::cout.flush();
}


//
//
#define OIDString "1.2.3.4.5.6.7.8.9.10"
#define OIDString2 "1.2.840.10040.4.1"
void test_AsnOid()
{
    AsnOid tmpoid(OIDString);
    AsnOid tmpoid2(OIDString2);
    char ptmpoid2Data[]={(char )0x2a, (char )0x86, (char )0x48, (char )0xce, 
                        (char )0x38, (char )0x04, (char )0x01};
    AsnOid *poid = new AsnOid;
    long arrLength=19, arcNumArrOUTLen;
    unsigned long arcNumArr[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
                              14, 15, 16, 17, 18, 19, (unsigned long)-1 };
    unsigned long *arcNumArrOUT=(unsigned long *)calloc(100, sizeof(unsigned long));

    *poid = tmpoid;     // TEST "=" operator.

    char *ptr = poid->GetChar();
    if (strcmp(OIDString, ptr) != 0)
        std::cout << "test_AsnOid: UNSUCCESSFUL AsnOid(\"1.2.3.4.5.6.7.8.9.10\") = "
             << ptr << "\n";
    else
        std::cout << "test_AsnOid: SUCCESSFUL AsnOid(\"1.2.3.4.5.6.7.8.9.10\") = "
             << ptr << "\n";
    delete poid;
    free(ptr);


    poid = new AsnOid;
    poid->Set(arcNumArr, arrLength);
	arcNumArrOUTLen = poid->NumArcs();
    poid->GetOidArray(arcNumArrOUT);
    delete poid;
    if (arcNumArrOUTLen != 19)
       std::cout << "test_AsnOid:: UNSUCCESSFUL long array extraction, extracted length=" 
                    << arcNumArrOUTLen << "\n";
    for (int i=0; i < arcNumArrOUTLen; i++)
        if (arcNumArrOUT[i] != arcNumArr[i])
            std::cout << "test_AsnOid:: UNSUCCESSFUL long array extraction." << 
                    arcNumArrOUT[i] << "," << arcNumArr[i] << "\n";

    if (tmpoid == OIDString)
       std::cout << "test_AsnOid:: SUCCESSFUL OID String Comparison." << "\n";
    if (tmpoid != "1.2.333.333.333")
       std::cout << "test_AsnOid:: SUCCESSFUL NOT OID String Comparison." << "\n";

    if (arcNumArrOUT)
        free(arcNumArrOUT);

    AsnOid TmpOid3;
    TmpOid3.Set(ptmpoid2Data, 7);
    if (TmpOid3 != tmpoid2)
       std::cout << "test_AsnOid:: UNSUCCESSFUL id_dsa OID String Comparison (bad news, bob)." << "\n";
}


//#include "vdatest_asn.h"
TestSequence testSequence;   //RWC;GLOBAL in order to share thread start logic.
                             //RWC;  (NOT GOOD DESIGN PRACTICE, but good for test).
//
//
void vdaTestPrintThreads_Perform(void *f_INPUT)
{
    struct ThreadNumberDef *f=(struct ThreadNumberDef *)f_INPUT;
    f->lThreadStatus = VDATHREAD_STATUS_STARTED;
    int VDATEST_COUNT=20;

    std::cout << "\n########################## vdaTestPrintThreads_Perform" << f->lThreadNumber << "\n";
    // PERFORM thread test operation(s).
    for (int i=0; i < VDATEST_COUNT; i++)
    {
      testSequence.Print(std::cout);  
    }

    //printf("ThreadNumber Test = %d DONE!\n", f->lThreadNumber);
    std::cout << "\n########################## DONE ThreadNumber Test = " <<  f->lThreadNumber << "\n";
    std::cout.flush();
    f->lThreadStatus = VDATHREAD_STATUS_DONE;
}


long vdaTestPrintThreadLocks()
{
   long status=0;
   unsigned long bytesDecoded=0;
   //struct ThreadNumberDef *plThread=NULL;
   AsnOcts TmpSNACCOcts;
   AsnBuf TmpBuf;
   char Aarray[]= { (char )0x30,(char )0x2d,(char )0x02,(char )0x15,
       (char )0x00,(char )0x94,(char )0x76,(char )0x92,(char )0xae,
       (char )0x87,(char )0x71,(char )0x0e,(char )0x50,(char )0xf9,
       (char )0xd5,(char )0x35,(char )0x49,(char )0xda,(char )0xa2,
       (char )0x40 };
   /*TestAllAsnPrimativeTypes ::= SEQUENCE {
	  octs OCTET STRING,
	  boolTest BOOLEAN,
     oid OBJECT IDENTIFIER OPTIONAL,
     bitString [2] IMPLICIT BIT STRING,
	  integer INTEGER,
	  enumTest ENUMERATED {A(11), B(12), C(13) },
	  real REAL OPTIONAL,
	  null NULL
   }*/

//   testSequence.testAllPrimatives = new TestAllAsnPrimativeTypes;
   //testSequence.testAllPrimatives->octsName.
   //   Set("This is an OCTET STRING test", strlen("This is an OCTET STRING test")+1);
   testSequence.testAllPrimatives.octsName.Set(Aarray, sizeof(Aarray));
   
   testSequence.testAllPrimatives.boolTestName = true;
   testSequence.testAllPrimatives.oidName = new AsnOid;
   *testSequence.testAllPrimatives.oidName = testOID;
   char *pData="TES";//(char)0xa5, (char)0xa5, (char)0xa5;
   testSequence.testAllPrimatives.bitStringName.Set((const unsigned char*)pData, 24);

   testSequence.testAllPrimatives.integerName = 55;
   testSequence.testAllPrimatives.enumTestName = TestAllAsnPrimativeTypesEnum::aA;
   testSequence.testAllPrimatives.realName = new AsnReal;
   *testSequence.testAllPrimatives.realName = 44.2;
   //testSequence.nullName

   TmpSNACCOcts.Set("this is a test of SetOf", strlen("this is a test of SetOf")+1);
   AsnBuf Buf3;
   TmpSNACCOcts.BEnc(Buf3);

   testSequence.testSetOfAny.insert(testSequence.testSetOfAny.end(), SNACC::AsnAny());
   testSequence.testSetOfAny.back().BDec(Buf3, bytesDecoded);

   if (testSequence.directoryString.choiceId == DirectoryString::teletexStringCid)
      delete testSequence.directoryString.teletexString;
            // FOR SOME reason, the default constructor of a DirectoryString
            //  Creates a TeletexString.
   testSequence.directoryString.choiceId = DirectoryString::printableStringCid;
   testSequence.directoryString.printableString = new DirectoryString::PrintableString_;
   *testSequence.directoryString.printableString = "Printable String TEST!";
   testSequence.BEnc(TmpBuf); // Be sure it encodes...


   return(status);
}

//
//
void test_IndefiniteLengthEncoding()
{
   FUNC("test_IndefiniteLengthEncoding");

   ContentInfo A, A2;
   unsigned long bytesDecoded=0;
   /* create IndefiniteLength encoding of ContentInfo. */
   AlgorithmIdentifier SNACCAlgId, SNACCAlgId2;
   AsnBuf *pBufAlgId=NULL;
   char ptrParams[]={(char)0x30, (char)0x12, (char)0x13, (char)0x10, 'T', 'h', 'i', 's', ' ', 'i', 's', 
      ' ', 'a', ' ', 't', 'e', 's', 't', '.', '\n' };
   AsnBuf BufParams(ptrParams, sizeof(ptrParams));

#ifndef SKIP_IndefiniteLength_ENCODING
   char ptr[]={(char)0x30, (char)0x80, (char)0x13, (char)0x10, 'T', 'h', 'i', 's', ' ', 'i', 's', 
              ' ', 'a', ' ', 't', 'e', 's', 't', '.', '\n', 
              (char)0x00, (char)0x00 /* Terminate IndefiniteLength */};
#else       // RWC;TEST.
   char ptr[]={(char)0x30, (char)0x12, (char)0x13, (char)0x10, 'T', 'h', 'i', 's', ' ', 'i', 's', 
      ' ', 'a', ' ', 't', 'e', 's', 't', '.', '\n' };
#endif
   AsnBuf bufAlgId;
   AsnBuf tmpBuf;
   AsnBuf DBuf(ptr, sizeof(ptr));   
                                 // THIS is the Any, with encoding tag 
                                 //  and indefinite length encoding length.
   

   //
   // FIRST, test outer envelope indefinite length sequencd
   SNACCAlgId.algorithm.PutChar("1.2.3.4.5");   // IGNORE option element.
   SNACCAlgId.parameters = new AsnAny;

   SNACCAlgId.parameters->BDec(BufParams, bytesDecoded);

   if (! SNACCAlgId.BEnc(bufAlgId) )
      throw SNACC_EXCEPT("Error encoding SNACCAlgId");

   // Attempt out envelope IndefiniteLength decode.
   //
   
   // Artificially make indefinite length.
   //
   unsigned long iAsnBufLength = bufAlgId.length();
   char *ptr2 = bufAlgId.GetSeg(iAsnBufLength);//10000);

   for (unsigned int i=0; i < iAsnBufLength; i++)
   {
      if ((ptr2[i] == (char)0x13) && (ptr2[i+1] == (char)0x10))
      {             // LOOK for text
         char ptr4[10000];
         ptr2[i+1] = (char)0x80; // REPLACE length.
         ptr2[i-1] += 2;      // Add 2 to sequence length count.
         ptr2[1] += 2;        // Add 2 to outer length count.
         memcpy(ptr4, ptr2, iAsnBufLength);
         ptr4[iAsnBufLength] = '\0';
         ptr4[iAsnBufLength+1] = '\0';
         pBufAlgId = new AsnBuf(ptr4, iAsnBufLength+2);     // IndefninteLength termination of EXPLICIT tag.
         break;
         free(ptr2);
      }  // END if found proper location to override.
   }

   if (pBufAlgId)
   {
       SNACCAlgId.BDec(*pBufAlgId, bytesDecoded ); // WILL throw exception if error
       bufAlgId = *pBufAlgId;
       delete pBufAlgId;
       pBufAlgId = NULL;
   }        // END IF pBufAlgId


   //
   // SECOND, test inner envelope indefinite length sequencd
   A.contentType.PutChar("1.2.3.4.5");
   A.content.BDec(DBuf, bytesDecoded);

   if (! A.BEnc(tmpBuf) )
      throw SNACC_EXCEPT("Encoding failed!");
 
   // RWC; if indefinite length encoding test, override EXPLICIT
   // RWC;  tag of ContentInfo with indefinite length flag as well.
   //
   AsnBuf *ptmpBuf = NULL;
   if (ptr[1] == (char)0x80)                                
   {
      iAsnBufLength = tmpBuf.length();
      char *ptr2=tmpBuf.GetSeg(iAsnBufLength);
      for (unsigned int i=1; i < iAsnBufLength; i++)
      {
         if ((ptr2[i] == (char)0x30) && (ptr2[i+1] == (char)0x80))
         {
            ptr2[i-1] = (char)0x80; // MAKE outer envelope CI indefinite as well...
            ptr2[1] += 2;           // Add 2 bytes to outer envelope count.
             char ptr4[10000];
             memcpy(ptr4, ptr2, iAsnBufLength);
             ptr4[iAsnBufLength] = '\0';
             ptr4[iAsnBufLength+1] = '\0';
             ptmpBuf = new AsnBuf(ptr4, iAsnBufLength+2);     // IndefninteLength termination of EXPLICIT tag.
            break;
         }  // END if found proper location to override.
      }
      if (ptr2)
          delete ptr2;
   }     // END if 0x80 check

   char *ptr3 = NULL;
   if (ptmpBuf)
   {
       A2.BDec(*ptmpBuf, bytesDecoded); // WILL throw exception if error.
       delete ptmpBuf;
       ptmpBuf = new AsnBuf;
       A2.content.BEnc(*ptmpBuf);     // GET just the content.
       iAsnBufLength = ptmpBuf->length();
       ptr3 = ptmpBuf->GetSeg(iAsnBufLength);
       delete ptmpBuf;
   }        // END IF ptmpBuf

   if (ptr3 && iAsnBufLength != sizeof(ptr) ||
       memcmp(ptr3, ptr, sizeof(ptr)) != 0)
      std::cout << "test_IndefiniteLengthEncoding:: ERROR in IndefiniteLength decoding.\n"
           << "  Decoded Length=" << iAsnBufLength;
   delete ptr3;


}


// EOF vdatest.cpp
