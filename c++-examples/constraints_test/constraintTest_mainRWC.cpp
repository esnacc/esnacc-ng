
/* RWC:  main test for PER Set/Sequence/Choice ASN.1 testing. 
 *    NOTE:  ERRORS TO BE FIXED::::
 *           1.) bAlign = 1 (true), causes integer decode to fail??? (per decode general).
 *           2.) Failure in Set/Sequence constructed logic when constraints used 
 *              (wrong class, e.g. AsnInt, needs to be custom class name)
 *           3.) 
 */
#include "constraintTest.h"
#include "perTestSet.h"
#include <fstream>
#include <strstream>
using namespace SNACC;
int SetSequence_AlignTest(bool bAlign);
int SetOfSequenceOf_AlignTest(bool bAlign);
int AnyDefinedBy_AlignTest(bool bAlign);

int constraintTest_mainRWC(int argc, char *argv[])
{
    try 
    {

	    std::cout << "******* START of PER Set/Sequence Alignment Test. ***\n" << std::endl;
        SetSequence_AlignTest(false);
        SetSequence_AlignTest(true);
	    std::cout << "******* END of PER Set/Sequence Alignment Test. ***\n" << std::endl;

	    std::cout << "******* START of PER SetOf/SequenceOf Alignment Test. ***\n" << std::endl;
        SetOfSequenceOf_AlignTest(false);
        SetOfSequenceOf_AlignTest(true);
	    std::cout << "******* END of PER SetOf/SequenceOf Alignment Test. ***\n" << std::endl;


	    std::cout << "******* START of PER AnyDefinedBy Alignment Test. ***\n" << std::endl;
        AnyDefinedBy_AlignTest(false);
        AnyDefinedBy_AlignTest(true);
	    std::cout << "******* END of PER AnyDefinedBy Alignment Test. ***\n" << std::endl;

    }       // END try
    catch (SnaccException &e)
    {
        std::cout << "**************\n" << e.what() << std::endl;
    }
	
	return 0;

 }

int SetSequence_AlignTest(bool bAlign)
{
    try
    {
        PerSetChoiceTest A_PerSetChoiceTest;
        PerSetChoiceTest A_PerSetChoiceTest2;
        PerSequenceChoiceTest A_PerSequenceChoiceTest;
        PerSequenceChoiceTest A_PerSequenceChoiceTest2;
	    AsnBufBits b(bAlign);
	    AsnBufBits tmpBufBits(bAlign);
	    AsnBufBits bSet(bAlign);
        AsnLen bytesDecoded=0;


	    std::cout << "___________________________________" << std::endl;
	    std::cout << "Begin Set/Sequence Handling Testing: bAlign = " << bAlign << std::endl;
	    std::cout << "-----------------------------------" << std::endl;

        //******************** PER SET TEST **************************
        A_PerSetChoiceTest.a = 22;
        A_PerSetChoiceTest.bDef.choiceId = PerSetChoiceTestChoice::cCid;
        A_PerSetChoiceTest.bDef.c = new AsnInt(44);
        {
            // SETUP ANY values; for testing we load the same value into both ANYs.
            AsnInt IntValue(77);
            IntValue.PEnc(tmpBufBits);
            long ltmpAnyBitLength=tmpBufBits.length();
            long ltmpAnyByteLength=ltmpAnyBitLength/8;
            if (ltmpAnyByteLength*8 < ltmpAnyBitLength)
                ltmpAnyByteLength++; // ALLOW 0 padded result, octet aligned.
            unsigned char *ptmpBufBits = tmpBufBits.GetBits(ltmpAnyBitLength);
            A_PerSetChoiceTest.perAnyTest.anyBuf = new AsnBuf((char *)ptmpBufBits, ltmpAnyByteLength);
                                        // NO OID for this test.
            A_PerSetChoiceTest.perAnyTest2.anyBuf = new AsnBuf((char *)ptmpBufBits, ltmpAnyByteLength);
                                        // NO OID for this test.
            delete[] ptmpBufBits;
        }

        A_PerSetChoiceTest.PEnc(b);
        std::cout << "*** After encode (no OPTIONAL)." << std::endl;
        std::cout << "INPUT:   " << A_PerSetChoiceTest << std::endl;
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;
    
        // TEST that we properly reset buffer.
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;

        A_PerSetChoiceTest2.PDec(b, bytesDecoded);
        std::cout << "OUTPUT:  " << A_PerSetChoiceTest2 << std::endl;

	    std::cout<<std::endl;

        // TEST that we properly reset buffer.
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;

        //******************** PER SET TEST, with OPTIONAL added **************************
        A_PerSetChoiceTest.e = new PerSetChoiceTestChoice1;
        A_PerSetChoiceTest.e->choiceId = PerSetChoiceTestChoice1::fCid;
        A_PerSetChoiceTest.e->f = new PerSetChoiceTestChoice1Choice;
        A_PerSetChoiceTest.e->f->choiceId = PerSetChoiceTestChoice1Choice::gCid;
        A_PerSetChoiceTest.e->f->g = new AsnInt(66);

        A_PerSetChoiceTest.PEnc(bSet);
        std::cout << "*** After encode (with OPTIONAL)." << std::endl;
        std::cout << "INPUT:   " << A_PerSetChoiceTest << std::endl;
	    std::cout << "Length of Buffer : " << bSet.length() << std::endl;
	    bSet.hexDump(std::cout);
	    std::cout << std::endl;
    
        A_PerSetChoiceTest2.PDec(bSet, bytesDecoded);
        std::cout << "OUTPUT Set, with OPTIONAL:  " << A_PerSetChoiceTest2 << "\n\n" << std::endl;


        //******************** PER SEQUENCE TEST **************************
	    AsnBufBits bSeq(bAlign);
	    AsnBufBits bSeq2(bAlign);
        A_PerSequenceChoiceTest.bDef.choiceId = PerSequenceChoiceTestChoice::cCid;
        A_PerSequenceChoiceTest.bDef.c = new AsnInt;
        *A_PerSequenceChoiceTest.bDef.c = 44;
        A_PerSequenceChoiceTest.eInteger = 66;

        A_PerSequenceChoiceTest.PEnc(bSeq);
        std::cout << "*** SEQUENCE: After encode (no OPTIONAL)." << std::endl;
        std::cout << "INPUT:   " << A_PerSequenceChoiceTest << std::endl;
	    std::cout << "Length of Buffer : " << bSeq.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;
            A_PerSequenceChoiceTest2.PDec(bSeq, bytesDecoded);
        std::cout << "OUTPUT:  " << A_PerSequenceChoiceTest2 << std::endl;
	    std::cout<<std::endl;

        A_PerSequenceChoiceTest.a = new AsnInt(22);     // LOAD OPTIONAL item.
        if (A_PerSequenceChoiceTest.fTaggedInteger)     // LOAD DEFAULT item.
            delete A_PerSequenceChoiceTest.fTaggedInteger;

        A_PerSequenceChoiceTest.fTaggedInteger = new PerSequenceChoiceTest::FTaggedInteger(59);

        //A_PerSequenceChoiceTest.fTaggedInteger = new AsnInt(59);

        A_PerSequenceChoiceTest.PEnc(bSeq2);
        std::cout << "*** SEQUENCE: After encode (with OPTIONAL)." << std::endl;
        std::cout << "INPUT:   " << A_PerSequenceChoiceTest << std::endl;
	    std::cout << "Length of Buffer : " << bSeq2.length() << std::endl;
	    bSeq2.hexDump(std::cout);
	    std::cout << std::endl;
            A_PerSequenceChoiceTest2.PDec(bSeq2, bytesDecoded);
        std::cout << "OUTPUT:  " << A_PerSequenceChoiceTest2 << std::endl;
	    std::cout<<std::endl;

	    std::cout<<std::endl;
    }
    catch (SNACC::SnaccException &e)	
    {
        std::cout<<e.what()<<std::endl;
    }
    return 0;

}       // END int SetSequence_AlignTest(bool )

//
//
int SetOfSequenceOf_AlignTest(bool bAlign)
{
    try
    {
        PerSetOfTest perSetOfTest;
        PerSetOfTest perSetOfTest2;
        PerSequenceOfTest perSequenceOfTest;
        PerSequenceOfTest perSequenceOfTest2;
	    AsnBufBits b(bAlign);
	    AsnBufBits bSeq(bAlign);
        unsigned long bitsDecoded=0;
        int ii;

	    std::cout << "___________________________________" << std::endl;
	    std::cout << "Begin SetOf/SequenceOf Handling Testing: bAlign = " << bAlign << std::endl;
	    std::cout << "-----------------------------------" << std::endl;

        //****** SETUP SetOf test
        for (ii=0; ii < 22; ii++)
        {
           *perSetOfTest.insert(perSetOfTest.end(), AsnInt()) = ii + 100;
        }   // END FOR 22 SET INTEGER elements.

        std::cout << "*** Input Data:  " << perSetOfTest << std::endl;
        perSetOfTest.PEnc(b);
        std::cout << "*** SetOf: After encode." << std::endl;
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;

        perSetOfTest2.PDec(b, bitsDecoded);
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
        std::cout << "Number of Bits Decoded : "<< bitsDecoded << std::endl;
        std::cout << "*** Output Data:  " << perSetOfTest2 << std::endl;
	    std::cout << std::endl;

        //****** SETUP SequenceOf test
        for (ii=0; ii < 11; ii++)
        {
           *perSequenceOfTest.insert(perSequenceOfTest.end(), AsnInt()) = ii + 200;
        }   // END FOR 22 SET INTEGER elements.

        std::cout << "*** Input Data:  " << perSequenceOfTest << std::endl;
        perSequenceOfTest.PEnc(bSeq);
        std::cout << "*** SequenceOf: After encode." << std::endl;
	    std::cout << "Length of Buffer : " << bSeq.length() << std::endl;
	    bSeq.hexDump(std::cout);
	    std::cout << std::endl;

        perSequenceOfTest2.PDec(bSeq, bitsDecoded);
	    std::cout << "Length of Buffer : " << bSeq.length() << std::endl;
        std::cout << "*** Output Data:  " << perSequenceOfTest2 << std::endl;
	    std::cout << std::endl;

	    std::cout << std::endl;
    }
    catch (SNACC::SnaccException &e)	
    {
        std::cout<<e.what()<<std::endl;
    }

    return(0);
}       // END SetOfSequenceOf_AlignTest(...)


//
//
int AnyDefinedBy_AlignTest(bool bAlign)
{
    try
    {
        PerTestDefinedByUsage defByUsage, defByUsage2;
        PrintableString *pPrintString;
	    AsnBufBits b(bAlign);
	    AsnBufBits bWithAny(bAlign);
        AsnLen bitsDecoded=0;

	    std::cout << "___________________________________" << std::endl;
	    std::cout << "Begin AnyDefinedBy Handling Testing: bAlign = " << bAlign << std::endl;
	    std::cout << "-----------------------------------" << std::endl;

        defByUsage.id = perTestOID2;
        pPrintString = new PrintableString;
        *pPrintString = "This is a test PrintableString";
        //printString.PEnc(bPS, bAlign);
        defByUsage.anyDefBy.value = pPrintString;  //GIVE up memory here.
                    // THIS case encodes as an AsnType, not a Built-In-Type.

        std::cout << "*** Input Data:  " << defByUsage << std::endl;
        defByUsage.PEnc(b);
        std::cout << "*** AnyDefinedBy: After encode." << std::endl;
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
	    b.hexDump(std::cout);
	    std::cout << std::endl;

        defByUsage2.PDec(b, bitsDecoded);
	    std::cout << "Length of Buffer : " << b.length() << std::endl;
        std::cout << "*** Output Data:  " << defByUsage2 << std::endl;
	    std::cout << std::endl;

        defByUsage2.PEnc(bWithAny);
                    // THIS case encodes as a Built-In-Type, since it was decoded.
        std::cout << "*** AnyDefinedBy: After decode/re-encode." << std::endl;
	    std::cout << "Length of Buffer : " << bWithAny.length() << std::endl;
	    bWithAny.hexDump(std::cout);
	    std::cout << std::endl;

	    std::cout << std::endl;
    }
    catch (SNACC::SnaccException &e)	
    {
        std::cout<<e.what()<<std::endl;
    }

    return(0);
}       // END AnyDefinedBy_AlignTest(...)


// EOF constraintTest_mainRWC.cpp
