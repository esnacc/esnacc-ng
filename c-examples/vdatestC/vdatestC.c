/* vdatestC.c : Defines the entry point for the console application. */
 

#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "asn-incl.h"
#include "exp-buf.h"
#include "vdatestC_asn.h"
#include "vdatest_asn2C.h"

void run_snmp_tests(char *path);
void test_AsnOid();
void ASN1init();
void ANY_test();
void ANY_DEFINED_BY_test();
void ANY_DEFINED_BY_test_2();
void InitAnyVdaTestModule2();
void test_timing();
#define TEST_ENCODE_BUF 1
#define TEST_DECODE_BUF 2
void outhex2(unsigned char *outChar, int Len);
int utf8compare2(unsigned char *expected, wchar_t *actual, int Len);
void InitAnyVdaTestModule();
void test_big_buffer();   /* fails - not supported */
void Test_ASN_1_Strings();
void test_bits();
int  append_test();
extern unsigned short stdIndentG = 4;

int main(int argc, char* argv[])
{ 
   ASN1init();
   ANY_test();
   if (argc > 1) 
   {
       run_snmp_tests(argv[1]);
       exit(0);
   }
   InitAnyVdaTestModule();
   InitAnyVdaTestModule2();
   ANY_DEFINED_BY_test();
   test_AsnOid();
   ANY_DEFINED_BY_test_2();
   test_timing();    
   test_big_buffer();
   test_bits();
   Test_ASN_1_Strings();
   append_test();

	return 0;
}

/*
*/
void test_AsnOid()
{
    AsnOid tmpoid;
	OID *pResult = NULL;
    AsnOid *poid = (AsnOid *) calloc(1, sizeof(AsnOid));
    long arrLength=19;
    unsigned long arcNumArr[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
                              14, 15, 16, 17, 18, 19};
    OID *poida = (OID *)calloc(1, sizeof(OID));
    OID *ptmpOid = poida;
    OID *ptmpOid2;
    int i;

    printf("test_AsnOid()\n");
	for (i=0; i < arrLength; i++)
	{
		ptmpOid->arcNum = arcNumArr[i];
		if (i < arrLength - 1)
        {
            ptmpOid->next = (OID *)calloc(1, sizeof(OID));
			ptmpOid = ptmpOid->next;
        }
	}
	
    poid->octs = (char *)calloc(1,100);
    tmpoid.octs = (char *)calloc(1,100);

    BuildEncodedOid(poida, poid);

    UnbuildEncodedOid(poid, &pResult);
    
	BuildEncodedOid(pResult, &tmpoid);

	if (AsnOidsEquiv(&tmpoid, poid))
		printf("AsnOids encode and decode SUCCESSFUL\n");
	else
		printf("AsnOids encode and decode UNSUCCESSFUL\n");

    ptmpOid = poida;
    ptmpOid2 = pResult;
	i=0;
	while (ptmpOid && ptmpOid2)
	{
		if (ptmpOid->arcNum != ptmpOid2->arcNum)
		{
			printf("AsnOid OID [%d] comparison failed\n",i);
			break;
		}
		ptmpOid = ptmpOid->next;
		ptmpOid2 = ptmpOid2->next;
	}
	if (ptmpOid || ptmpOid2)
		printf("AsnOid OID [%d] count difference\n",i);

    ptmpOid = poida;
    ptmpOid2 = pResult;
	while (ptmpOid && ptmpOid2)
	{
		OID *p1 = ptmpOid->next;
		OID *p2 = ptmpOid2->next;
		free(ptmpOid);
		ptmpOid = p1;
		free(ptmpOid2);
		ptmpOid2 = p2;
	}

	FreeAsnOid(poid);
    free(poid);
    free(tmpoid.octs);

}

void test_bits()
{
    AsnBits A, C, A2;
    struct ExpBuf *pB = NULL;
	struct GenBuf *gB;
    ENV_TYPE env;
    int val;
    unsigned int bytesDecoded=0;
    char data[10]={(char)0x00, (char)0x00, (char)0x00, (char)0xff, (char)0xff, 
        (char)0xff, (char)0xff, (char)0xff, (char)0x00, (char)0x00};
    char pchBuffer[1000];

    printf("test_bits()\n");

    pB = ExpBufAllocBufAndData();
    ExpBufInstallDataInBuf(pB, &pchBuffer[0], 1000);
	ExpBuftoGenBuf (pB, &gB);
    ExpBufResetInWriteRvsMode(pB);
    A.bits = data;
    A.bitLen = 22;
    SetAsnBit(&(A), 14);
    ExpBufResetInWriteRvsMode(pB);
    BEncAsnBits(gB, &A);  
    ExpBufResetInReadMode(&pB);
    if ((val = setjmp (env)) == 0)
       BDecAsnBits(gB, &C, &bytesDecoded, env);
    else
       printf("BDecAsnBits failed!\n");
    if (C.bitLen == 14+1)
        printf("test_bits:  Successful 0's removed!!!\n");
    else
        printf("test_bits:  ####ERROR, UNSuccessful 0's removed!!!####\n");

    A.bits = data;
    A.bitLen = 22;
    ExpBufResetInWriteRvsMode(gB->spare);
    BEncAsnBits(gB, &A);  
    ExpBufResetInReadMode(gB->bufInfo);
    if ((val = setjmp (env)) == 0)
       BDecAsnBits(gB, &C, &bytesDecoded, env);
    else
       printf("BDecAsnBits failed!\n");
    if (C.bitLen == 22 && GetAsnBit(&C, 0) == FALSE)  /* False if bit is 0 */
        printf("test_bits:  Successful ALL 0 BITS(No NamedBitListRules)!!!\n");
    else
        printf("test_bits:  ####ERROR, UNSuccessful ALL 0 BITS(No NamedBitListRules)!!!####\n");

    A.bits = data;
    A.bitLen = 22;
    ExpBufResetInWriteRvsMode(gB->spare);
    BEncAsnBits(gB, &A);  
    ExpBufResetInReadMode(gB->bufInfo);
    if ((val = setjmp (env)) == 0)
       BDecAsnBits(gB, &C, &bytesDecoded, env);
    else
       printf("BDecAsnBits failed!\n");
    if (C.bitLen == 0)
        printf("test_bits:  Successful ALL 0 BITS(NamedBitListRules)!!!\n");
    else
        printf("test_bits:  ####ERROR, UNSuccessful ALL 0 BITS(NamedBitListRules)!!!####\n");

    A.bits = data;
    A.bitLen = 24;
    SetAsnBit(&(A), 15);
    ExpBufResetInWriteRvsMode(gB->spare);
    BEncAsnBits(gB, &A);  
    ExpBufResetInReadMode(gB->bufInfo);
    if ((val = setjmp (env)) == 0)
       BDecAsnBits(gB, &C, &bytesDecoded, env);
    else
       printf("BDecAsnBits failed!\n");
    if (C.bitLen == 15+1)
        printf("test_bits:  Successful 0's removed2!!!\n");
    else
        printf("test_bits:  ####ERROR, UNSuccessful 0's removed2!!!####\n");

    A2.bits = data;
    A2.bitLen = 80;
    SetAsnBit(&(A2), 79);
    ExpBufResetInWriteRvsMode(gB->spare);
    BEncAsnBits(gB, &A2);  
    ExpBufResetInReadMode(gB->bufInfo);
    if ((val = setjmp (env)) == 0)
       BDecAsnBits(gB, &C, &bytesDecoded, env);
    else
       printf("BDecAsnBits failed!\n");
    if (C.bitLen == 79+1)
        printf("test_bits:  Successful top 1 set!!!\n");
    else
        printf("test_bits:  ####ERROR, UNSuccessful top 1 set!!!####\n");
}


void test_big_buffer()
{
    char pchBuffer[200000];
    struct ExpBuf *pM = NULL;
	struct GenBuf GenBufpM;
    AsnOcts N;
    AsnLen bytesDecoded = 0;
    ENV_TYPE env;
    int val;

   printf("test_big_buffer()\n");

   pM = (struct ExpBuf *) calloc(1, sizeof(struct ExpBuf));
   memset(&N,'\0', sizeof(AsnOcts));
   memset(pchBuffer, 'A', 200000);
   N.octs = pchBuffer;
   N.octetLen = 200000;
/*   ENCODE_BUF(&N, pM); */
   ExpBufResetInWriteRvsMode(pM);
   PutExpBufInGenBuf(pM, &GenBufpM);
   BEncAsnOcts(&GenBufpM, &N);  

   printf("test_big_buffer: Octet String encoded length of 200000 byte buf= %d \n", 
       ExpBufDataSize(GenBufpM.spare));

/*   DECODE_BUF_NOFAIL(&N, pM, status); */
   if ((val = setjmp (env)) == 0)
       BDecAsnOcts(&GenBufpM, &N, &bytesDecoded, env);
   else
       printf("BDecAsnOcts failed.  Expected - C Snacc doesn't support big buffers.\n");

   ExpBufFreeData(GenBufpM.spare);
   free(pM);

}

/*
*/
void ANY_DEFINED_BY_test()
{
   AsnOid testOid3;

   struct ExpBuf *pBUF;
   struct GenBuf *gBUF;
   TestDefinedByUsage A, A2, A3;
   PrintableString *pB;
   PrintableString *pPS;
   AsnLen bytesDecoded = 0;
   ENV_TYPE env;
   int val;
   int result = 0;
   char *octs1 = ("1.2.3.4.5.5.5.5.6");
   char *test  = ("THIS IS A TEST");
   printf("ANY_DEFINED_BY_test()\n");

   testOid3.octs = octs1;
   testOid3.octetLen = strlen(octs1);
   pBUF = ExpBufAllocBufAndData();
   ExpBuftoGenBuf(pBUF, &gBUF);
   pB = (PrintableString *) calloc(1, sizeof(PrintableString));
   pB->octs = strdup(test);
   pB->octetLen = strlen(test);

   memset(&A,'\0', sizeof(A));
   memset(&A2,'\0', sizeof(A2));
   memset(&A3,'\0', sizeof(A3));
   A.id = testOID2;
   A.anyDefBy.value  = pB;
   A.i1 = 1;
   A.i2 = 2;
   A.i3 = 3;
   A.i4 = 4;
/*   ENCODE_BUF(&A, pBUF); */
   ExpBufResetInWriteRvsMode(gBUF->spare);
   result = DEncTestDefinedByUsage(gBUF, &A);  
   if (result == 0)
        printf("DEncTestDefinedByUsage FAILED!\n");
/*   DECODE_BUF(&A2, pBUF); */
   ExpBufResetInReadMode(gBUF->bufInfo);
   if ((val = setjmp (env)) == 0)
        DDecTestDefinedByUsage(gBUF, &A2, &bytesDecoded, env);
   else
        printf("DDecTestDefinedByUsage FAILED!\n");
   ExpBufFreeBufAndData(pBUF);

/* WE HAVE TO CHECK EXPLICITLY, but the decode is automatic. */
   result = memcmp(A2.id.octs, testOID2.octs, A2.id.octetLen);
   if (result == 0)
   {                       
      pPS = (PrintableString *)A2.anyDefBy.value;  /* WE MUST KNOW what the OID
                                                    is associated with. */
      printf("ANY_DEFINED_BY_test: Good id, == testOID2. %s\n", pPS->octs);
   }
   else
   {
      printf("ANY_DEFINED_BY_test: Bad id, <> testOID2.\n");
   }

   FreePrintableString(pB);
   free(pB);

}

/*  THIS Version tests the newly added ability of the SNACC compiler to handle
    multiple Object-Type definitions in separate .asn files. */
void ANY_DEFINED_BY_test_2()
{
   AsnOid testOid3;
   TestDefinedByUsage_2 A, A2, A3;
   struct ExpBuf *pBUF;
   struct GenBuf *gBUF;
   PrintableString *pB;
   PrintableString *pPS;
   AsnLen bytesDecoded = 0;
   ENV_TYPE env;
   int val;
   char *octs1 = ("1.2.3.4.5.5.5.5.6");
   char *test = ("THIS IS A TEST");
   int result = 0;
   
   printf("ANY_DEFINED_BY_test_2()\n");

   testOid3.octs = octs1;
   testOid3.octetLen = strlen(octs1);
   pBUF = ExpBufAllocBufAndData();
   ExpBuftoGenBuf(pBUF, &gBUF);
   pB = (PrintableString *) calloc(1, sizeof(PrintableString));
   pB->octs = strdup(test);
   pB->octetLen = strlen(test);
   memset(&A,'\0', sizeof(A));
   memset(&A2,'\0', sizeof(A2));
   memset(&A3,'\0', sizeof(A3));
   A.id = testOID2_2;
   A.anyDefBy.value  = pB;

/*   ENCODE_BUF(&A, pBUF); */
   
   ExpBufResetInWriteRvsMode(gBUF->spare);
//   result = DEncTestDefinedByUsage_2(&pBUF, &A);  
   result = DEncTestDefinedByUsage_2(gBUF, &A);  
   if (result == 0)
        printf("DEncTestDefinedByUsage_2 FAILED!\n");
/*   DECODE_BUF(&A2, pBUF); */
   ExpBufResetInReadMode(gBUF->bufInfo);
   if ((val = setjmp (env)) == 0)
        DDecTestDefinedByUsage_2(gBUF, &A2, &bytesDecoded, env);
   else
        printf("DDecTestDefinedByUsage_2 FAILED!\n");
   ExpBufFreeBufAndData(gBUF->spare);
   result = memcmp(A2.id.octs, testOID2_2.octs, A2.id.octetLen);
   if (result == 0)   /* WE HAVE TO CHECK EXPLICITELY, but the decode is  automatic. */
   {                        
      pPS = (PrintableString *)A2.anyDefBy.value;  /* WE MUST KNOW what the OID
                                                     is associated with. */
      printf("ANY_DEFINED_BY_test: Good id, == testOID2_2. %s\n", pPS->octs);
   }
   else
   {
      printf("ANY_DEFINED_BY_test: Bad id, <> testOID2_2.\n");
   }
   FreePrintableString(pB);
   free(pB);

}


int append_test()
{
  VDATestSet      *ptestSet;
  VDATestSet      *ptestSet2;
  VDATestSet      *ptestSetBER;
  char            testSeqBlobTbl[] = { 0x30, 0x06, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02 };
  /* SET OF Integers, 04 tag, 01 length, 01 then 02 data. */
  char            testSetBlobTbl[] = { 0x31, 0x06, 0x04, 0x01, 0x01, 0x04, 0x01, 0x02 };
  char            testSetBlobTblVaryLengths[] = 
                    { 0x31, 54, 
                      0x04, 0x01, 0x71, 
                      0x04, 0x02, 0x72, 0x72, 
                      0x04, 0x03, 0x72, 0x73, 0x74, 
                      0x04, 0x03, 0x74, 0x72, 0x73, 
                      0x04, 0x03, 0x74, 0x72, 0x79,
                      0x04, 0x03, 0x74, 0x72, 0x78,
                      0x04, 0x05, 0x74, 0x72, 0x73, 0x73, 0x73,
                      0x04, 0x03, 0x74, 0x72, 0x77,
                      0x04, 0x03, 0x74, 0x72, 0x76,
                      0x04, 0x03, 0x74, 0x72, 0x75,
                      0x04, 0x03, 0x74, 0x72, 0x74
                    };      /* THESE SHOULD all be ordered */
  char            testSetBlobBERTbl[] = { 0x31, 0x06, 0x04, 0x01, 0x02, 0x04, 0x01, 0x01 };
  char            testSetOfAnyBlobTbl[] = { 0x31, 0x08, 0x03, 0x02, 0x04, 0x0F, 0x03, 0x02, 0x05, 0x0E };
  char            testNullBitString[] = { 0x03, 0x01, 0x00 };
  char            testBit1BitString[] = { 0x03, 0x02, 0x02, 0x20}; /* 2 unused bits, data 0x20 (2nd bit set). */
  AsnOid          TestOid;
  int err = 1;
  struct ExpBuf testNullBitStringBlob;
  struct ExpBuf *ptestNullBitStringBlob2;
  struct GenBuf  ptestNullBitStringBlob2GenBuf;
  struct ExpBuf *ptestSeqBlob; 
  struct ExpBuf *ptestSeqBlob2;
  struct ExpBuf *ptestSetBlob;
  struct GenBuf *ptestSetBlobGenBuf;
  struct ExpBuf *ptestSetBlobBER;
  struct GenBuf *ptestSetBlobBERGenBuf;
  struct ExpBuf *ptestSetBlobVaryingLengths;
  struct GenBuf *GenBufptestSetBlobVaryingLengths;
  struct ExpBuf *ptestSetBlob2;
  struct GenBuf *ptestSetBlob2GenBuf;
  struct ExpBuf *ptestSetBlob4;
  struct GenBuf *ptestSetBlob4GenBuf;
  struct ExpBuf testSetOfAnyBlob;
  AsnBits    snaccNullBitString;
  VDATestBitString snaccNullBitString3;
  SequenceOfDefaults AA;
  struct ExpBuf  tmpBuf;
  struct ExpBuf  *ptmpBlob;
  struct GenBuf  ptmpBlobGenBuf;
  AsnBits     snaccBitStr;
  FILE *fp;
  int result;
  int size1 = 0;
  int size2 = 0;
  AsnLen bytesDecoded = 0;
  ENV_TYPE env;
  int val;
  char *octs1 = ("1.2.3.4.5.0.0.0");
  char *octs2 = ("1.2.3.4.5.6.8.8.8");
    /*Test_ASN_1_Strings(); */
   printf("append_test()\n");

  /* the following tables are DER encoded results for 
     VDATestSequence, VDATestSet, and VDATestSetOfAny.  Decode
     these tables and then re-encode them to determine if we
     come up with the same results.
    
     note: this doesn't test every scenario.  */
 
  memset(&snaccNullBitString,'\0', sizeof(AsnBits));
  memset(&snaccNullBitString3,'\0', sizeof(VDATestBitString));
  memset(&testNullBitStringBlob,'\0', sizeof(struct ExpBuf));
  memset(&AA,'\0', sizeof(SequenceOfDefaults));
  ExpBufInstallDataInBuf(&testNullBitStringBlob, &testNullBitString[0], 3);
  ptestNullBitStringBlob2 = ExpBufAllocBufAndData();
  ptmpBlob = ExpBufAllocBufAndData();
  ptestSeqBlob2 = ExpBufAllocBufAndData();
  ptestSeqBlob = ExpBufAllocBufAndData();
  ptestSetBlob = ExpBufAllocBufAndData();
  ptestSetBlob2 = ExpBufAllocBufAndData();
  ptestSetBlob4 = ExpBufAllocBufAndData();
  ptestSetBlobBER = ExpBufAllocBufAndData();
  ptestSet = (VDATestSet *) calloc(1, sizeof(VDATestSet));
  ptestSet2 = (VDATestSet *) calloc(1, sizeof(VDATestSet));
  ptestSetBER = (VDATestSet *) calloc(1, sizeof(VDATestSet));
 /* RWC;12/18/00; THIS SET of tests require that the Named Bit rules
     be applied.  Named Bit rules are set only for declared ASN SNACC
     compiler processed Bit Strings, not for "AsnBits" declared vars
     as in "snaccNullBitString" above.
   THIS 1st test should not produce 030100. */
  /* ENCODE_BUF_NO_ALLOC(&snaccNullBitString3, &testNullBitStringBlob2); */

  AA.defBitstring.bits = (char *) calloc(1,2);
  AA.defBitstring.bitLen = 9;

  snaccNullBitString.bits = (char *) calloc(1,2);
  snaccNullBitString.bitLen = 9;
  free(snaccNullBitString.bits);
  snaccNullBitString.bits = NULL;
  snaccNullBitString3.bits = (char *) calloc(1,2);
  snaccNullBitString3.bitLen = 8;

  /* ENCODE_BUF_NO_ALLOC(AA.defBitstring , &testNullBitStringBlob2); */
  ExpBufResetInWriteRvsMode(ptestNullBitStringBlob2);
  PutExpBufInGenBuf(ptestNullBitStringBlob2, &ptestNullBitStringBlob2GenBuf);
  DEncAsnBits(&ptestNullBitStringBlob2GenBuf, &AA.defBitstring);  
  size1 = ExpBufDataSize(ptestNullBitStringBlob2GenBuf.spare);
  size2 = ExpBufDataSize(&testNullBitStringBlob);
  if (size1 != size2)
     printf("NULL BIT STRING 1 FAILED SIZE COMPARE!\n");
  else
  {
      result = memcmp(ExpBufDataPtr(ptestNullBitStringBlob2),
             ExpBufDataPtr(&testNullBitStringBlob), size1);
      if (result == 0)
      {
         printf("NULL BIT STRING 1 TEST PASSED!\n");
      }
      else
      {
         printf("NULL BIT STRING TEST 1 FAILED!\n");
         printf("ENCODED VALUE IS:\n");
         printf("\nIT SHOULD BE:\n");
         printf("\n");
      }
  }

/*  ENCODE_BUF_NO_ALLOC(&snaccNullBitString3, &testNullBitStringBlob2); */
  ExpBufResetInWriteRvsMode(ptestNullBitStringBlob2);
  PutExpBufInGenBuf(ptestNullBitStringBlob2, &ptestNullBitStringBlob2GenBuf);
  DEncAsnBits(&ptestNullBitStringBlob2GenBuf, &snaccNullBitString3);  
  free(snaccNullBitString3.bits);
  snaccNullBitString3.bits = NULL;
  size1 = ExpBufDataSize(ptestNullBitStringBlob2GenBuf.spare);
  size2 = ExpBufDataSize(&testNullBitStringBlob);
  if (size1 != size2)
     printf("NULL BIT STRING 2 FAILED SIZE COMPARE!\n");
  else
  {

      result = memcmp(ExpBufDataPtr(ptestNullBitStringBlob2GenBuf.spare),
             ExpBufDataPtr(&testNullBitStringBlob), size1);
      if (result == 0)
      {
         printf("NULL BIT STRING 2 TEST PASSED!\n");
      }
      else
      {
         printf("NULL BIT STRING TEST 2 FAILED!\n");
         printf("ENCODED VALUE IS:\n");
         printf("\nIT SHOULD NOT BE:\n");
         printf("\n");
      }
  }

  free(AA.defBitstring.bits);
  AA.defBitstring.bits = NULL;
  /* Now let's test a bits string that has more than one octet that is null.
     The resulting encoding should look like the testNullBitString above. */
  memset(&AA,'\0', sizeof(SequenceOfDefaults));
  AA.defBitstring.bits = (char *) calloc(1,3);
  AA.defBitstring.bitLen = 18;
  SetAsnBit(&(AA.defBitstring), 18);
  /* testNullBitStringBlob2.Clear(); */
  ExpBufFreeBufAndData (ptestNullBitStringBlob2GenBuf.spare);
  ptestNullBitStringBlob2 = (struct ExpBuf *) calloc(1, sizeof(struct ExpBuf));
/*  ENCODE_BUF_NO_ALLOC(AA.defBitstring, &testNullBitStringBlob2); */
  ExpBufResetInWriteRvsMode(ptestNullBitStringBlob2);
  PutExpBufInGenBuf(ptestNullBitStringBlob2, &ptestNullBitStringBlob2GenBuf);
  DEncAsnBits(&ptestNullBitStringBlob2GenBuf, &AA.defBitstring);  
  free(AA.defBitstring.bits);
  AA.defBitstring.bits = NULL;
  size1 = ExpBufDataSize(ptestNullBitStringBlob2GenBuf.spare);
  size2 = ExpBufDataSize(&testNullBitStringBlob);
  if (size1 != size2)
     printf("NULL BIT STRING 3 FAILED SIZE COMPARE!\n");
  else
  {
      result = memcmp(ExpBufDataPtr(ptestNullBitStringBlob2GenBuf.spare),
             ExpBufDataPtr(&testNullBitStringBlob), size1);
      if (result == 0)
      {
         printf("Multiple null octets NULL BIT STRING test 3 passed!\n");
      }
      else
      {
         printf("Multiple null octets NULL BIT STRING TEST 3 FAILED!\n");
         printf("ENCODED VALUE IS:\n");
/*         testNullBitStringBlob2.ReportHexBuffer(cout); */
         printf("\nIT SHOULD BE:\n");
/*         testNullBitStringBlob.ReportHexBuffer(cout); */
         printf("\n");
      }
  }
  free(ptestNullBitStringBlob2);
  ptestNullBitStringBlob2 = NULL;

  /* Test BIT STRING with only the first bit set. */

  memset(&tmpBuf,'\0', sizeof(struct ExpBuf));
  ExpBufInstallDataInBuf(&tmpBuf, testBit1BitString, 4);
  snaccBitStr.bits = (char *) calloc(1,1);
  snaccBitStr.bitLen = 6;
  SetAsnBit(&(snaccBitStr), 2);

/*  ENCODE_BUF_NO_ALLOC(&snaccBitStr, &tmpBlob); */
  ExpBufResetInWriteRvsMode(ptmpBlob); 
  PutExpBufInGenBuf(ptmpBlob, &ptmpBlobGenBuf);
  DEncAsnBits(&ptmpBlobGenBuf, &snaccBitStr);  
  free(snaccBitStr.bits);
  snaccBitStr.bits = NULL;
  size1 = ExpBufDataSize(ptmpBlobGenBuf.spare);
  size2 = ExpBufDataSize(&tmpBuf);
  if (size1 != size2)
     printf("Bitstring with first bit set and uneven bit count FAILED SIZE COMPARE!\n");
  else
  {
      result = memcmp(ExpBufDataPtr(ptmpBlobGenBuf.spare),
             ExpBufDataPtr(&tmpBuf), size1);
      if (result == 0)
      {
         printf("Bitstring with first bit set and uneven bit count PASSED!\n");
      }
      else
      {
         printf("Bitstring with first bit set and uneven bit count FAILED!\n");
         printf("BitString Encoded as:\n");
/*         tmpBlob.ReportHexBuffer(cout); */
         printf("It should be:\n");
/*         tmpBuf.ReportHexBuffer(cout); */
         printf("\n");
      }
  }

  /* sequence of to integers */
  ExpBufInstallDataInBuf(ptestSeqBlob, &testSeqBlobTbl[0], 8);

  /* sequence of two octet strings */
  ExpBufInstallDataInBuf(ptestSetBlob, &testSetBlobTbl[0], 8);
  ExpBufInstallDataInBuf(ptestSetBlobBER, &testSetBlobBERTbl[0] , 8);

  ptestSetBlobVaryingLengths = ExpBufAllocBufAndData();
	ExpBuftoGenBuf(ptestSetBlobVaryingLengths, &GenBufptestSetBlobVaryingLengths);
  ExpBufInstallDataInBuf(GenBufptestSetBlobVaryingLengths->spare, &testSetBlobTblVaryLengths[0], 56);

  /* sequence of two ANYs.  In this case the any's are two BIT STRINGS */
  ExpBufInstallDataInBuf(&testSetOfAnyBlob, &testSetOfAnyBlobTbl[0] , 10);
  
  /* Decode, Encode, and Compare Test Sequence */
/* 
  BDecAsnBits(&ptestSeqBlob, testSeq.first, &bytesDecoded, env);
  ExpBufResetInWriteRvsMode(ptestSeqBlob2);
  DEncAsnBits(&ptestSeqBlob2, &testSeq);  
  size1 = ExpBufDataSize(ptestSeqBlob2);
  size2 = ExpBufDataSize(ptestSeqBlob);
  if (size1 != size2)
     printf("Sequence test count FAILED SIZE COMPARE!\n");
  else
  {
      result = memcmp(ExpBufDataPtr(ptestSeqBlob2),
             ExpBufDataPtr(ptestSeqBlob), size1);
      if (result == 0)
         printf("Passed Sequence test ...\n");
      else
         printf("Failed Sequence test ...\n");
  }
*/
  /* Decode, Encode, and Compare Test Set */
/*  DECODE_BUF( &testSet, &ptestSetBlob); */
	ExpBuftoGenBuf(ptestSetBlob, &ptestSetBlobGenBuf);
//    PutExpBufInGenBuf(ptestSetBlob, &ptestSetBlobGenBuf);
    if ((val = setjmp (env)) == 0)
        DDecVDATestSet(ptestSetBlobGenBuf, ptestSet, &bytesDecoded, env);
    else
        return 0;
/*  ENCODE_BUF_NO_ALLOC( &testSet, &ptestSetBlob2); */
    ExpBufResetInWriteRvsMode(ptestSetBlobGenBuf->spare);
    DEncVDATestSet(ptestSetBlobGenBuf, ptestSet);
    ExpBufInstallDataInBuf(ptestSetBlob2, ExpBufDataPtr(ptestSetBlobGenBuf->spare),
							ExpBufDataSize(ptestSetBlobGenBuf->spare));
	ExpBufResetInReadMode(ptestSetBlobGenBuf->bufInfo);
    size1 = ExpBufDataSize(ptestSetBlobGenBuf->spare);
    size2 = ExpBufDataSize(ptestSetBlob);
    if (size1 != size2)
       printf("Set test count FAILED SIZE COMPARE!\n");
    else
    {

      result = memcmp(ExpBufDataPtr(ptestSetBlobGenBuf->spare),
             testSetBlobTbl, size1);
      if (result == 0)
        printf("Passed SET test ... \n");
      else
        printf("Failed SET test ... \n");
    }

  /* This test should sort the various SET OF integers of varying lengths,
      REGARDLESS OF LENGTH (per X.690 specification section 11.6). */

  FreeVDATestSet(ptestSet);
  free(ptestSet);


  ptestSet = (VDATestSet *) calloc(1, sizeof(VDATestSet));

/*  DECODE_BUF( &testSet, &testSetBlobVaryingLengths); */
  if ((val = setjmp (env)) == 0)
      DDecVDATestSet(ptestSetBlobGenBuf, ptestSet, &bytesDecoded, env);
  else
      return 0;

/*  ENCODE_BUF_NO_ALLOC( &testSet, &testSetBlob2); */
  ExpBufResetInWriteRvsMode(ptestSetBlob2);
  ExpBuftoGenBuf(ptestSetBlob2, &ptestSetBlob2GenBuf);

  DEncVDATestSet(ptestSetBlob2GenBuf, ptestSet);
  free(ptestSet);
  ptestSet = NULL;

  bytesDecoded = 0;
/*  DECODE_BUF( &testSet2, &testSetBlob2); */
  /* Had to move data into a new buffer because encoding left data
     pointers unusable for decoding. */
  ExpBufInstallDataInBuf(ptestSetBlob4, ExpBufDataPtr(ptestSetBlob2GenBuf->spare),
      ExpBufDataSize(ptestSetBlob2GenBuf->spare));
  ExpBufResetInReadMode(&ptestSetBlob4);
  ExpBuftoGenBuf (ptestSetBlob4, &ptestSetBlob4GenBuf);

	if ((val = setjmp (env)) == 0) {
     DDecVDATestSet(ptestSetBlob4GenBuf, ptestSet2, &bytesDecoded, env);

	}
	 else
	     return 0;

  fp=fopen("vdatest.txt", "w");
  /* need a different print  - testSet2 is an AsnList */
  PrintVDATestSet(fp, ptestSet2, 1);      /* PRINT results, should be in proper order. */
  fclose(fp);
  free(ptestSet2);

/*
   Decode, Encode, and Compare Test SET OF ANY */
  /* RWC;10/07/00; */
/*  DECODE_BUF( &testSetOfAny, &testSetOfAnyBlob);
  ENCODE_BUF_NO_ALLOC( &testSetOfAny, &testSetOfAnyBlob2);
  if (testSetOfAnyBlob2.Compare(testSetOfAnyBlob) == 0)
     cout << "Passed SET OF ANY test ...\n";
  else 
     cout << "Failed SET OF ANY test ...\n";
*/
  /* Decode BER, Encode DER, and Compare Encoded DER
     to known DER encoding.
  */
  /*RWC;Causes crash on Linux;testSetBlob2.~CSM_Buffer(); */
/*  DECODE_BUF( &testSetBER, &testSetBlobBER); */
  bytesDecoded = 0;
  ExpBuftoGenBuf(ptestSetBlobBER, &ptestSetBlobBERGenBuf);
  if ((val = setjmp (env)) == 0)
      DDecVDATestSet(ptestSetBlobBERGenBuf, ptestSetBER, &bytesDecoded, env);
  else
     return 0;
/*  ENCODE_BUF_NO_ALLOC( &testSetBER, &testSetBlob2); */
  ExpBufResetInWriteRvsMode(ptestSetBlob2GenBuf->spare);
  DEncVDATestSet(ptestSetBlob2GenBuf, ptestSetBER);
  size1 = ExpBufDataSize(ptestSetBlob2GenBuf->spare);
  size2 = ExpBufDataSize(ptestSetBlob);
  free(ptestSetBER);
  if (size1 != size2)
     printf("DER test count FAILED SIZE COMPARE!\n");
  else
  {
    result = memcmp(ExpBufDataPtr(ptestSetBlob2GenBuf->spare),
           ExpBufDataPtr(ptestSetBlob), size1);
    if (result == 0)
      printf("Passed DER test ... \n");
    else
    {
      printf("Failed DER test ... \n");
      size2 = ExpBufDataSize(ptestSetBlobBER);
      if (size1 != size2)
         printf("BER decoded and re-encode as BER insteaded of DER count FAILED SIZE COMPARE!\n");
      else
      {
        result = memcmp(ExpBufDataPtr(ptestSetBlob2),
               ExpBufDataPtr(ptestSetBlobBER), size1);
        if (result == 0)
          printf("\tBER decoded and re-encode as BER insteaded of DER\n");
        else
        {
  /*      testSetBlob2.ReportHexBuffer( cout, testSetBlob2.Access(), testSetBlob2.Length()); */
          printf("Still failed DER test ... \n");
        }
      }
    }
  }
  printf ("At TestOid\n");
  TestOid.octs = octs1;
  TestOid.octetLen = strlen(octs1);
  printf("TestOid = %s\n", TestOid.octs);

  TestOid.octs = octs2;
  TestOid.octetLen = strlen(octs2);
  printf("TestOid = %s\n", TestOid.octs);


/*
  RWC; TEST Override for "<<" operator to avoid stdout prints...
  RWC;  Override is only used within SNACC generated code, not in snacc 
  RWC;  run-time includes.
  RWC;  Force failure
  printf("######### THE NEXT SET OF STATEMENTS SHOULD FAIL, YOU WILL ####\n");
  printf("#########     SEE THE ERROR FROM SNACC AT THE TOP OF THE   ####\n");
  printf("#########     STACK                                        ####\n");
  VDATestSequence aa;
  struct ExpBuf cBuf("aaa", 3);
  DECODE_BUF(&aa, &cBuf);
*/
  return 1;

} /* append_test */

void Test_ASN_1_Strings()
{
/* RFC 2253 Test Data */
	unsigned char  RFC2253TestData[] = { 0x4c, 0x75, 0xC4, 0x8d, 0x69, 0xC4, 0x87, 0x00, 0x00 };
	const int RFC2253TestDataLen = 9;
/* RFC 2279 Test Data Section 4 (Examples) Example 1 */
	unsigned char  RFC2279TestData1[] = { 0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x91, 0x2E, 0x00 };
	unsigned char RFC2279TestResult1[] = {0x00, 0x41, 0x22, 0x62, 0x03, 0x91, 0x00, 0x2E };
/* RFC 2279 Test Data Section 4 (Examples) Example 2 */
    unsigned  char RFC2279TestData2[] = { 0xED ,0x95 ,0x9C ,0xEA, 0xB5, 0xAD, 0xEC, 0x96, 0xB4,0x00 };
	unsigned  char RFC2279TestResult2[] = { 0xD5, 0x5C, 0xAD, 0x6D, 0xC5, 0xB4 };
/* RFC 2279 Test Data Section 4 (Examples) Example 3 */
	unsigned  char RFC2279TestData3[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E };
	unsigned char RFC2279TestResult3[] = { 0x65, 0xE5, 0x67, 0x2C, 0x8A, 0x9E };
/* RFC 2279 Test Data Section 4 (Examples) Example 4 */
	unsigned  char UTF8TestData4[] = { 0x00,0x4c,0x80, 0x80,0x01, 0x0d, 0x00, 0x69, 0x01, 0x07, 0x00, 0x00 };
/*	unsigned  char RFC2279TestResult4[] = { 0x */
	const int RFC2279TestData1Len = 8;
	const int RFC2279TestData2Len = 10;
	const int RFC2279TestData3Len = 9;
	const int RFC2279TestData4Len = 12;
	const int RFC2279TestResult1Len = 8;
	const int RFC2279TestResult2Len = 6;
	const int RFC2279TestResult3Len = 6;
/* Universal String is 4 bytes long */
	char  universal_string[] = { 0x00, 0x00,0x00,0x4c,0x00, 0x00,0x00, 0x75,0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x01, 0x07, 0x00, 0x00,0x00,0x00 };
	char  bmp_string[] = {0x00, 'T', 0x00, 'E', 0x00, 'S', 0x00, 'T'};
	wchar_t bmpTest[] = {'T', 'E', 'S', 'T'};
	int universal_stringLen = 24;
    long status=0;
    UniversalString A;
	UTF8String C;
    PrintableString AP, AP2;
	BMPString bmp;
    wchar_t *pWchar=NULL;
	char *tempChar = NULL;
    char *blahstr = ("blah PrintableString");
   struct ExpBuf *pBUF;
   struct GenBuf gBUF;
   AsnLen bytesDecoded = 0;
   ENV_TYPE env;
   int val;
   int result = 0;
/*    printf("######### Test_ASN_1_Strings:   START OF TEST          ####\n");
    A.octs = (char *)calloc(1, sizeof(char));
    A.octs = (const char *)universal_string;
    A.octetLen = universal_stringLen;
    printf("Test_ASN_1_Strings: A(UniversalString)= %s\n",A.GetChar());
	printf("Converting from 4 byte wide characters to wchar_t %d bytes wide \n", sizeof(wchar_t));
	printf("UniversalString Results \n");
	tempChar = (char *)A.octs;
    for (i=0;  i < universal_stringLen+1/2; i++)
	{
		printf("0x %d",(int)tempChar[i]);
		printf(" ");
		if  ((tempChar[i+1] == 0x00) &&
			 (tempChar[i+2] == 0x00) &&
		 	 (tempChar[i+3] == 0x00) &&
			 (tempChar[i+4] == 0x00))
			i = universal_stringLen+10; 

	}
	printf("\n");
    printf("######### Test_ASN_1_Strings:   END OF TEST           ####\n\n"); */
    printf("######### Test_ASN_1_Strings:   START OF TEST           ####\n\n");
	printf("\n");
  /*   cout << "Test_ASN_1_Strings: pWchar of A=" << TmpChar << "\n"; */
    AP.octs = blahstr;
    AP.octetLen = strlen(blahstr);
    printf("Test_ASN_1_Strings: AP(PrintableString)= %s\n", AP.octs);
    A = AP;             /* TEST Multi-byte assignment to single-byte ASN.1 def. */
    printf("Test_ASN_1_Strings: A(UniversalString<-PrintableString)= %s\n",A.octs);
    printf("######### Test_ASN_1_Strings:   END OF TEST           ####\n\n");
    printf("######### Test_ASN_1_Strings:   START OF ENC/DEC TEST           ####");
	printf("\n");

   pBUF = ExpBufAllocBufAndData();
   ExpBufResetInWriteRvsMode(pBUF);
   PutExpBufInGenBuf (pBUF, &gBUF);
   result = BEncPrintableString(&gBUF, &AP);  
   if (result == 0)
        printf("BEncPrintableString FAILED!\n");
   ExpBufResetInReadMode(gBUF.bufInfo);
   if ((val = setjmp (env)) == 0)
        BDecPrintableString(&gBUF, &AP2, &bytesDecoded, env);
   else
        printf("BDecPrintableString FAILED!\n");
   ExpBufFreeBufAndData(gBUF.spare);
   result = memcmp(AP2.octs, AP.octs, AP2.octetLen);
   if (result == 0)   
   {                       
      printf("BEncPrintableString Success! \n");
   }
   else
   {
      printf("BEncPrintableString Failed! \n");
   }
    printf("######### Test_ASN_1_Strings:   END OF ENC/DEC TEST           ####\n\n");
	printf("######### Test_ASN_1_Strings:   START OF TESTS          ####\n");
	printf("Test 1 input data is: \n");
	outhex2(RFC2279TestData1, RFC2279TestData1Len);
	printf("\n");
/*    AP.Set((char *)RFC2279TestData1); */
    AP.octs = RFC2279TestData1;
    AP.octetLen = RFC2279TestData1Len;
    C = AP;             /* TEST Multi-byte assignment to single-byte ASN.1 def. */
    printf("Test 1 Expected Result:\n");
	outhex2(RFC2279TestResult1, RFC2279TestResult1Len);
/*	C.Set((char *)RFC2279TestData1, RFC2279TestData1Len); */
    C.octs = RFC2279TestData1;
    C.octetLen = RFC2279TestData1Len;
/*	pWchar = C; */ /* Transform the UTF-8 to Wide Character */
    status = CvtUTF8String2wchar(&C, &pWchar);
	printf("\nTest 1 Actual Test Result(Intel platforms will have bytes flipped):\n");
	if (pWchar != NULL)
	{
		outhex2((unsigned char *)pWchar, RFC2279TestResult1Len);
		status = utf8compare2(RFC2279TestResult1, pWchar, RFC2279TestData1Len);
		if (status != 0)
			printf("Actual Results do not compare with expected results\n");
		else
			printf("######### Results compare\n");
	}
	else
		printf("pWchar is NULL, which is in error\n");
	printf("\n");
	printf("Test 2 input data is: \n");
	outhex2(RFC2279TestData2, RFC2279TestData2Len);
	printf("\n");
/*    AP.Set((char *)RFC2279TestData2); */
    AP.octs = RFC2279TestData2;
    AP.octetLen = RFC2279TestData2Len;
    C = AP;             /* TEST Multi-byte assignment to single-byte ASN.1 def. */
	printf("Test 2 Expected Result:\n");
	outhex2(RFC2279TestResult2, RFC2279TestResult2Len);
/*	C.Set((char *)RFC2279TestData2, RFC2279TestData2Len); */
    C.octs = RFC2279TestData2;
    C.octetLen = RFC2279TestData2Len;
/*	pWchar = C; */  /* Transform the UTF-8 to Wide Character */
    status = CvtUTF8String2wchar(&C, &pWchar);
	printf("\nTest 2 Actual Test Result(Intel platforms will have bytes flipped):\n");
	if (pWchar != NULL)
	{
		outhex2((unsigned char *)pWchar, RFC2279TestResult2Len);
		status = utf8compare2(RFC2279TestResult2, pWchar, RFC2279TestResult2Len);
		if (status != 0)
			printf("Actual Results do not compare with expected results\n");
		else
			printf("######### Results compare\n");

	}
	else
		printf("pWchar is NULL, which is in error\n");
	printf("\n");

	printf("\n");
	printf("Test 3 input data is: \n");
	outhex2(RFC2279TestData3, RFC2279TestData3Len);
	printf("\n");
/*    AP.Set((char *)RFC2279TestData3); */
    AP.octs = RFC2279TestData3;
    AP.octetLen = RFC2279TestData3Len;
    C = AP;             /* TEST Multi-byte assignment to single-byte ASN.1 def. */
	printf("Test 3 Expected Result:\n");
	outhex2(RFC2279TestResult3, RFC2279TestResult3Len);
/*	C.Set((char *)RFC2279TestData3, RFC2279TestData3Len); */
    C.octs = (char *)calloc(1, strlen(RFC2279TestData3) + 1);
    C.octetLen = RFC2279TestData3Len;
    memcpy(C.octs, RFC2279TestData3, C.octetLen);
/*	pWchar = C; */ /* Transform the UTF-8 to Wide Character */
    status = CvtUTF8String2wchar(&C, &pWchar);
    free(C.octs);
    C.octs = NULL;
	printf("\nTest 3 Actual Test Result(Intel platforms will have bytes flipped):\n");
	if (pWchar != NULL)
	{
		outhex2((unsigned char *)pWchar, RFC2279TestResult3Len);
		status = utf8compare2(RFC2279TestResult3, pWchar, RFC2279TestResult3Len);
		if (status != 0)
			printf("Actual Results do not compare with expected results\n");
		else
			printf("######### Results compare\n");

	}
	else
		printf("pWchar is NULL, which is in error\n");
	printf("\n");
    printf("######### Test_ASN_1_Strings:   END OF TEST 3          ####\n\n");
	printf("\n");

	printf("######### Test_ASN_1_Strings:   START OF TESTS          ####\n");
	printf("Test 4 input data is: \n");
	outhex2((unsigned char *)bmp_string, 8);
	printf("\n");
/*	D.Set((char *)bmpTest, 8); */
    bmp.octs = (char *)calloc(1, 8 + 1);
    bmp.octetLen = 8;
    memcpy(bmp.octs, bmpTest, bmp.octetLen);
    status = CvtBMPString2wchar(&bmp, &pWchar);
    free(bmp.octs);
    bmp.octs = NULL;
	printf("Test 4 actual BMPString -> wchar_t is:\n");
	outhex2((unsigned char *)pWchar, 8);
	printf("\n");
    printf("######### Test_ASN_1_Strings:   END OF TEST 4          ####\n\n");
	printf("\n");

}

void outhex2(unsigned char *outChar, int Len)
{
	int i = 0;
    for (i=0;  i < Len; i++)
	{
		printf ("0x%x ", outChar[i]);
		printf(" ");
	}
	printf ("\n");

}

int utf8compare2(unsigned char *expected, wchar_t *actual, int Len)
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

	/* Compare the strings */
	stat = strncmp((const char *)expected, (const char *)actual, Len);
	return stat;
}

/* Timing */
void test_timingDoTestPrint(long BYTE_COUNT, long iCount, long lTestType);
void test_timing()
{
   long iCount=1000;

   test_timingDoTestPrint(1000, iCount, TEST_ENCODE_BUF);
   test_timingDoTestPrint(1000, iCount, TEST_ENCODE_BUF);
   test_timingDoTestPrint(1000, iCount, TEST_DECODE_BUF);
   test_timingDoTestPrint(1000, iCount, TEST_DECODE_BUF);
#ifdef TESTS_ALREADY_DONE
#endif

   /*RELEASE Version
   test_timing: Start.
   test_timing:  OLD ENCODE_BUF iCount=1000, Time=10.160000, single=0.010160 Size=90000bytes.
   test_timing: Start.
   test_timing:  NEW ENCODE_BUF iCount=1000, Time=2.640000, single=0.002640 Size=90000bytes.
   DEBUG Version
   test_timing: Start.
   test_timing:  OLD ENCODE_BUF iCount=1000, Time=14.060000, single=0.014060 Size=90000bytes.
   test_timing: Start.
   test_timing:  NEW ENCODE_BUF iCount=1000, Time=6.860000, single=0.006860 Size=90000bytes.
   test_timing: Start.
   test_timing:  OLD DECODE_BUF iCount=1000, Time=7.910000, single=0.007910 Size=90000bytes.
   test_timing: Start.
   test_timing:  NEW DECODE_BUF iCount=1000, Time=1.210000, single=0.001210 Size=90000bytes. */

}

double test_timingDoTest(long BYTE_COUNT, long iCount, long lTestType);
void test_timingDoTestPrint(long lByteCount, long iCount, long lTestType)
{
   double dSingle;
   double dResult;
   char *lpszTypeFlag;

   dResult = test_timingDoTest(lByteCount, iCount, lTestType);
   dSingle = dResult / iCount;
   if (lTestType == TEST_ENCODE_BUF)
      lpszTypeFlag = "ENCODE_BUF";
   else if (lTestType == TEST_DECODE_BUF)
      lpszTypeFlag = "DECODE_BUF";
   printf("test_timing:  %s iCount=%d, Time=%f, single=%f Size=%ldbytes.\n", 
      lpszTypeFlag, iCount, dResult, dSingle, lByteCount);
}

#ifndef WIN32		/* artificially fix symbols. */
#define _timeb timeb
#define _ftime ftime
#endif
double test_timingDoTest(long BYTE_COUNT, long iCount, long lTestType)
{
   double dTime, dTime2, dSingle;
   struct _timeb timebuffer;
   PrintableString AA;
   PrintableString *pAA;  /* CANNOT use "AA" due to SNACC limitation where it does
                       not free previous memory on subsequent decodes!!!#@$% */
   struct ExpBuf *pBB=NULL;
   struct GenBuf *genpBB = NULL;
   struct ExpBuf *pCANNEDTmpEncodedBuf=NULL;
   struct GenBuf *pCANNEDTmpEncodedGenBuf;
   struct ExpBuf *pCANNEDEncodedBuf=NULL;
   struct GenBuf CANNEDEncodedGenBuf;

   int i;
   int result;
   ENV_TYPE env;
   int val;
   unsigned int bytesDecoded=0;
   char *pChar=(char *)calloc(1, BYTE_COUNT);

   printf("test_big_buffer()");
   printf("test_timing: Start.\n");
   memset((void *)pChar, 'R', BYTE_COUNT);
   pBB = ExpBufAllocBufAndData();
   pCANNEDTmpEncodedBuf = ExpBufAllocBufAndData();
   pCANNEDEncodedBuf = ExpBufAllocBufAndData();
   ExpBuftoGenBuf(pCANNEDTmpEncodedBuf, &pCANNEDTmpEncodedGenBuf);
   AA.octs = pChar;    
   AA.octetLen = BYTE_COUNT;
/*   ENCODE_BUF22(&AA, pCANNEDTmpEncodedBuf); */ /* FOR DECODE_BUF ops. */
   ExpBufResetInWriteRvsMode(pCANNEDTmpEncodedGenBuf->spare);
   result = BEncPrintableString(pCANNEDTmpEncodedGenBuf, &AA);  
   if (result == 0)
        printf("BEncPrintableString FAILED!\n");
/*   pCANNEDEncodedBuf = new CSM_Buffer(*pCANNEDTmpEncodedBuf); */
/*   delete pCANNEDTmpEncodedBuf; */ /* need to copy to avoid oversized buffer 
                                  from ENCODE_BUF ops. */
  ExpBufInstallDataInBuf(pCANNEDEncodedBuf, ExpBufDataPtr(pCANNEDTmpEncodedGenBuf->spare),
      ExpBufDataSize(pCANNEDTmpEncodedGenBuf->spare));
/*   ExpBufFreeBufAndData(pCANNEDTmpEncodedBuf); */

       
   _ftime( &timebuffer);
   dTime = timebuffer.time;
   dTime += (.001) * timebuffer.millitm;



   for (i=0; i < iCount; i++)
   {
     pBB = NULL;

     if (lTestType == TEST_ENCODE_BUF)
     {
        pBB = ExpBufAllocBufAndData();
		ExpBuftoGenBuf (pBB, &genpBB);
        ExpBufResetInWriteRvsMode(genpBB->spare);
        result = BEncPrintableString(genpBB, &AA);  
        ExpBufFreeBufAndData(genpBB->spare);
		free (genpBB);
     }
     else if (lTestType == TEST_DECODE_BUF)
     {
        pAA = (PrintableString *) calloc(1, sizeof(PrintableString));
        ExpBufResetInReadMode(&pCANNEDEncodedBuf);
		PutExpBufInGenBuf(pCANNEDEncodedBuf, &CANNEDEncodedGenBuf);
        if ((val = setjmp (env)) == 0)
            BDecPrintableString(&CANNEDEncodedGenBuf, pAA, &bytesDecoded, env);
        else
            printf("BDecPrintableString FAILED!\n");
        FreePrintableString(pAA);
        free(pAA);
     }
   }

   _ftime( &timebuffer);
   dTime2 = timebuffer.time;
   dTime2 += (.001) * timebuffer.millitm;
   dSingle = dTime2 - dTime;
   free(pChar);

   
  return(dSingle);
    
}

void ANY_test()
{
   //AsnOid testOid3;
   TestANYUsage A4, A5;
   //TestDefinedByUsage_2 A, A2, A3;
   struct ExpBuf *pBUF, *pBUF2=NULL;
   struct GenBuf *gBUF, *gBUF2=NULL;
   PrintableString *pB;
   PrintableString *pPS;
   AsnLen bytesDecoded = 0;
   ENV_TYPE env;
   int val;
   //char *octs1 = ("1.2.3.4.5.5.5.5.6");
   char *test = ("THIS IS A TEST");
   int result = 0;
   AsnLen iAnyLen;
   
   printf("ANY_test()\n");


//AsnLen BEncAsnAny(GenBuf *b, AsnAny *v);
//void BDecAsnAny(GenBuf *b, AsnAny *result, AsnLen *bytesDecoded, ENV_TYPE env);

   // FIRST, load PrintableString for ANY.
   pBUF = ExpBufAllocBufAndData();
   ExpBuftoGenBuf(pBUF, &gBUF);
   pB = (PrintableString *) calloc(1, sizeof(PrintableString));
   pB->octs = strdup(test);
   pB->octetLen = strlen(test);
   ExpBufResetInWriteRvsMode(gBUF->spare);
   result = BEncPrintableString(gBUF, pB);
   FreePrintableString(pB);
   free(pB);
   ExpBufResetInReadMode(gBUF->bufInfo);
   if (result)
   {
       // NEXT, load encoded PrintableString (in gBUF) into ANY of A4
       memset(&A4,'\0', sizeof(A4));
       SetAnyTypeUnknown((&A4.aBlob));  // SETUP for this buffer of ANY...
       iAnyLen = 0;
       BDecAsnAny(gBUF, &A4.aBlob, &iAnyLen, env);
       if (iAnyLen)
       {
          pBUF2 = ExpBufAllocBufAndData();
          ExpBuftoGenBuf(pBUF2, &gBUF2);
          printf("ANY_test: SUCCESSFUL ANY load.\n");
          ExpBufResetInWriteRvsMode(gBUF2->spare);
          result = DEncTestANYUsage(gBUF2, &A4);
          if (result)
             printf("ANY_test: SUCCESSFUL TestANYUsage encode, with ANY.\n");
          else
             printf("ANY_test: ***** UNSUCCESSFUL TestANYUsage encode, with ANY.\n");
          ExpBufResetInReadMode(gBUF2->bufInfo);
       }    // END IF ANY loaded.
       else
           printf("ANY_test: ***** UNSUCCESSFUL ANY load.\n");

   }        // END IF encode or PrintableString
   else
       printf("ANY_test: ***** UNSUCCESSFUL PrintableString encode.\n");



   // NOW that we have an encoded TestANYUsage, attempt to decode and 
   //  reference data.
   if (gBUF2)
   {
        iAnyLen = 0;
        if ((val = setjmp (env)) == 0)
            DDecTestANYUsage(gBUF2, &A5, &iAnyLen, env);
        else
        {
            printf("***** DDecTestANYUsage FAILED!\n");
            return;
        }
        SetAnyTypeUnknown((&A5.aBlob));  // SETUP for this buffer of ANY...
        ExpBufResetInWriteRvsMode(gBUF->spare);
        iAnyLen = BEncAsnAny(gBUF, &A5.aBlob);
        if (iAnyLen)
        {
            pPS = (PrintableString *) calloc(1, sizeof(PrintableString));
            iAnyLen = 0;
            ExpBufResetInReadMode(gBUF->bufInfo);
            BDecPrintableString(gBUF, pPS, &iAnyLen, env);
            if (iAnyLen)  // SUCCEEDED in deocding ANY, previously encoded 
                          //  as PrintableString.
               printf("ANY_test: SUCCESSFUL ANY PrintableString DECODE; |%s|.\n", pPS->octs);
            else
               printf("ANY_test: ***** UNSUCCESSFUL ANY PrintableString DECODE.\n");
            ExpBufFreeBufAndData(gBUF->spare);
            FreePrintableString(pPS);
            free(pPS);
        }   // IF iAnyLen on decode...
        else
           printf("ANY_test: ***** UNSUCCESSFUL ANY unload.\n");
        ExpBufFreeBufAndData(gBUF2->spare);
   }        // END IF gBUF2

}       // END ANY_test()
