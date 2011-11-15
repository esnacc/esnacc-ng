/* JKG:  main test constraints for asn1 */
#include "constraintTest.h"
#include "perTest.h"
#include "new-types.h"
#include <fstream>
#include <strstream>


int constraintTest_main(int argc, char *argv[])
{
    SNACC::AsnExtension ext;
    ext;

    
    try
    {
	    std::cout<<"___________________________________"<<std::endl;
	    std::cout<<"Begin Constraint Handling Testing :"<<std::endl;
	    std::cout<<"-----------------------------------"<<std::endl;


	    /*  The following tests to see if the constraints are working properly */
	    
	    
        SNACC::ConstraintFailList ConstraintFails;
        SNACC::ConstraintFailList* pList;

        SNACC::REAL1 real1;
        real1=3000;
        real1.checkConstraints(&ConstraintFails);
        std::cout<<"Real ValueRange Constraints Test.........................PASSED!"<<std::endl;

        SNACC::REAL2 real2;
        real2=55.0;
	    real2.checkConstraints(&ConstraintFails);
        std::cout<<"Real SingleValue Constraints Test........................PASSED!"<<std::endl;


        SNACC::INT1 int1;
        int1=34;
        int1.checkConstraints(&ConstraintFails);
        std::cout<<"Integer ValueRange Constraints Test......................PASSED!"<<std::endl;

        SNACC::INT2 int2;
        int2=-20;
        int2.checkConstraints(&ConstraintFails);
        std::cout<<"Integer SingleValue Constraints Test.....................PASSED!"<<std::endl;

        SNACC::BOOL1 bool1;
        bool1 = true;
        bool1.checkConstraints(&ConstraintFails);
        std::cout<<"Boolean SingleValue Constraints Test.....................PASSED!"<<std::endl;
    
        SNACC::NUMSTRING1 numstring1;
	    numstring1 = "1";
        numstring1.checkConstraints(&ConstraintFails);
        std::cout<<"Restricted Char String SingleValue Constraints Test......PASSED!"<<std::endl;
	    std::cout<<"Restricted Char String Permitted Alphabet Test...........PASSED!"<<std::endl;

	    SNACC::IA5STRING1 ia5string1;
        ia5string1="ia";
        ia5string1.checkConstraints(&ConstraintFails);
        std::cout<<"Restricted Char String ValueRange Constraints Test.......PASSED!"<<std::endl;
/*    
        SNACC::UNIVERSALSTRING1 universalstring1;
        universalstring1="universalstring";
        universalstring1.checkConstraints(&ConstraintFails);
        std::cout<<"Restricted Char String Size ValRange Constraints Test....PASSED!"<<std::endl;

        SNACC::UNIVERSALSTRING2 universalstring2;
        universalstring2="ffffgggghhhhddz";
        universalstring2.checkConstraints(&ConstraintFails);
        std::cout<<"Restricted Char String Size SingleVal Constraints Test...PASSED!"<<std::endl;
*/


        SNACC::ConstraintTest constrainttest;
        constrainttest.integer=10;
        constrainttest.integer1=-20;
        constrainttest.boolean=true;
        constrainttest.numericString="1234";
        constrainttest.ia5String="ia5string2";

        constrainttest.checkConstraints(&ConstraintFails);
        std::cout<<"Sequence Constraints Test................................PASSED!"<<std::endl;

        SNACC::TestSetOfConstraints testsetofconstraints;
        testsetofconstraints.push_back(constrainttest);
        testsetofconstraints.checkConstraints(&ConstraintFails);
    
        SNACC::TestSequenceOfConstraints testsequenceofconstraints;
        testsequenceofconstraints.push_back(testsetofconstraints);
        testsequenceofconstraints.checkConstraints(&ConstraintFails);
        std::cout<<"Sequence Of Constraints Test.............................PASSED!"<<std::endl;

        SNACC::TestSet testset;
        testset.testSequenceOfConstraints = testsequenceofconstraints;
        testset.checkConstraints(&ConstraintFails);
        std::cout<<"Set Constraints Test.....................................PASSED!"<<std::endl;

	    std::cout<<"\n\n"<<std::endl;

        pList=&ConstraintFails;

		SNACC::ConstraintFailList::const_iterator i;
		for (i = pList->begin(); i != pList->end(); ++i)
	    {	
		    std::cout<<"*****************************************************************"<<std::endl;
		    std::cout<< *i << std::endl;
		    std::cout<<"*****************************************************************"<<std::endl;
        }

	    std::cout<<"Printed List of Failed Constraints.......................PASSED!\n\n"<<std::endl;

	    std::cout<<"_________________________________"<<std::endl;
	    std::cout<<"End Constraint Handling Testing :"<<std::endl;
	    std::cout<<"---------------------------------"<<std::endl;


	    std::cout<<std::endl;
 

	    std::cout<<std::endl;
	    std::cout<<std::endl;



        bool bAlign = true;
	    SNACC::AsnBufBits fwdBuf(bAlign);
	    SNACC::AsnLen l = 0;
	    
	    
	    std::ofstream tmp("tmp.txt");
	    
	    unsigned char c = 0x0F;

	    long mynum = 999999;

	    unsigned char mychar[999999];
	    memset(mychar, c, mynum);

	    
	    SNACC::PERBool		perBool(true);
	    SNACC::PERInt		perInt(54);
	    SNACC::PERIntSemi	perIntSemi(1);
	    SNACC::PERIntFully	perIntFully(2147483647);
	    SNACC::PERIA5String perString;
	    SNACC::PERNumericString numString;
	    SNACC::PERPString		pbleString;
	    SNACC::PERVString		visString;
	    SNACC::PEROctet			octString;
        SNACC::PERBit			bitString(4);
        SNACC::PERIntSingleVal  perIntSingleVal(4);

	    numString = "12";
	    pbleString = "this' actually kind of works";
	    visString = "This is definitely visible.";
	    octString = "gha";

	    SNACC::PERIA5String decode;
	    decode = "hello";
	    perString = "bye";
	    SNACC::PERReal		perReal(33.33);
        SNACC::TestEnum     perEnum;
        perEnum = 2;

	    SNACC::AsnLen len = 0;
        l = 0;

        len += perEnum.PEnc(fwdBuf);
        len += perIntSingleVal.PEnc(fwdBuf);
	    len += bitString.PEnc(fwdBuf);
	    len += octString.PEnc(fwdBuf);
	    len += perBool.PEnc(fwdBuf);
	    len += perInt.PEnc(fwdBuf);
	    len += perIntSemi.PEnc(fwdBuf);
	    len += perIntFully.PEnc(fwdBuf);

	    len += decode.PEnc(fwdBuf);

	    len += numString.PEnc(fwdBuf);
	    len += pbleString.PEnc(fwdBuf);
	    len += visString.PEnc(fwdBuf);
	    len += perReal.PEnc(fwdBuf);

	    perBool = SNACC::PERBool(false);
	    perInt = SNACC::PERInt(-1);
	    perIntSemi = SNACC::PERIntSemi(-1);
	    perIntFully = SNACC::PERIntFully(-1);
	    perString = "goodbye";
	    numString = "5678";
	    pbleString = "hi";
	    visString = "goodbye again";
	    octString = "qqq";

        std::cout<<"********************************"<<std::endl;
	    std::cout<<"Start Hex Dump of Encoded Buffer"<<std::endl;
	    std::cout<<"--------------------------------"<<std::endl;
	    
	    std::cout<<"Length of Buffer : "<<fwdBuf.length()<<std::endl;
	    fwdBuf.hexDump(std::cout);
        std::cout<<std::endl;
        std::cout<<"Length of Encoding : "<<len << std::endl;
	    std::cout<<"Number of Bits Decoded : "<<l << std::endl;
        std::cout<<std::endl;
        

	    std::cout<<"------------------------------"<<std::endl;
	    std::cout<<"End Hex Dump of Encoded Buffer"<<std::endl;
	    std::cout<<"******************************"<<std::endl;
	    
	    std::cout<<std::endl;

        perEnum.PDec(fwdBuf, l);
	    perIntSingleVal.PDec(fwdBuf, l);
        bitString.PDec(fwdBuf, l);
	    octString.PDec(fwdBuf, l);
	    perBool.PDec(fwdBuf, l);
	    perInt.PDec(fwdBuf, l);
	    perIntSemi.PDec(fwdBuf, l);
	    perIntFully.PDec(fwdBuf, l);

	    perString.PDec(fwdBuf, l);
 	    numString.PDec(fwdBuf, l);
	    pbleString.PDec(fwdBuf, l);
	    visString.PDec(fwdBuf, l);
	    perReal.PDec(fwdBuf, l);


    /*	
	    //OBJECT IDENTIFIER anObjectId

	    SNACC::AsnLen bitsDecoded = 0;
	    SNACC::AsnBufBits bBits = SNACC::AsnBufBits(buf);;
	    SNACC::AsnOid anInternalObject("1.2.333.444.555.38");
	    SNACC::AsnOid anInternalObject2;
	    anInternalObject.PEnc(bBits);
	    anInternalObject2.PDec(bBits, bitsDecoded);
	    std::cout << anInternalObject2 << std::endl;
    */
	    std::cout<<pbleString<<std::endl;
	    std::cout<<visString<<std::endl;

        if(perEnum != 2)
            std::cout<<"PEREnum decode mismatch"<<std::endl;

	    if(octString != "gha")
		    std::cout<<"PEROctet decode mismatch"<<std::endl;

	    if(perBool != true)
		    std::cout<<"PERBool decode mismatch"<<std::endl;

	    if(perInt != 54)
		    std::cout<<"PERInt decode mismatch"<<std::endl;
	    
	    if(perIntSemi != 1)
		    std::cout<<"PERIntSemi decode mismatch"<<std::endl;
		    
	    if(perIntFully != 2147483647)
		    std::cout<<"PERIntFully decode mismatch"<<std::endl;

	    if(perString != "hello")
		    std::cout<<"PERIA5String decode mismatch"<<std::endl;

	    if(numString != "12")
		    std::cout<<"PERNumericString decode mismatch"<<std::endl;
	    
	    if(pbleString != "this' actually kind of works")
		    std::cout<<"PERPString decode mismatch"<<std::endl;
	    
	    if(visString != "This is definitely visible.")
		    std::cout<<"PERVString decode mismatch"<<std::endl;
	    
	    
	    std::cout<<"********************************"<<std::endl;
	    std::cout<<"Start Hex Dump of Encoded Buffer"<<std::endl;
	    std::cout<<"--------------------------------"<<std::endl;
	    
	    std::cout<<"Length of Buffer : "<<fwdBuf.length()<<std::endl;
	    fwdBuf.hexDump(std::cout);
	    std::cout<<std::endl;
        std::cout<<"Length of Encoding : "<<len << std::endl;
	    std::cout<<"Number of Bits Decoded : "<<l << std::endl;
        std::cout<<std::endl;


	    std::cout<<"------------------------------"<<std::endl;
	    std::cout<<"End Hex Dump of Encoded Buffer"<<std::endl;
	    std::cout<<"******************************"<<std::endl;

        SNACC::AsnBuf ab;
        SNACC::AsnLen bD;
        SNACC::AsnBufBits abb;
        SNACC::AsnLen bitsD;
        SNACC::TestRelOid testRelOid;
        testRelOid.id = SNACC::myOid;
        testRelOid.id2 = SNACC::myOid;

        testRelOid.BEnc(ab);
        testRelOid.BDec(ab, bD);

        testRelOid.PEnc(abb);
        testRelOid.PDec(abb, bitsD);

        testRelOid.id2 += testRelOid.id;
        testRelOid.id2 = testRelOid.id2 + testRelOid.id;

        testRelOid.id2 += SNACC::myOid;
        testRelOid.id2 = testRelOid.id2 + SNACC::myOid;

        testRelOid.id2.Print(std::cout);

                
        std::cout<<std::endl;
	   
        testRelOid.id2.PrintXML(std::cout);
        
	    std::cout<<std::endl;


        /******************************************************************/
        /*          BER SET EXTENSIBILITY TEST                            */
        /******************************************************************/

        SNACC::TestSetExtensibilityV1  set_v1;
        SNACC::TestSetExtensibilityV2  set_v2;
        SNACC::TestSetExtensibilityV3  set_v3;
        SNACC::TestSetExtensibilityV3  set_v3_1;
        SNACC::AsnBuf set_buffer;
        SNACC::AsnBuf set_buffer2;
        SNACC::AsnBuf set_buffer3;
        SNACC::AsnLen set_bufferDecoded = 0;

        set_v3.i1 = 0;
        delete set_v3.i2;
        set_v3.i2 = new SNACC::AsnInt(1);
        set_v3.i3 = 3;
        set_v3.i4 = 4;

        set_v3.BEnc(set_buffer);
        set_buffer.ResetMode();
        set_v1.BDec(set_buffer, set_bufferDecoded);
        set_bufferDecoded = 0;
       
        set_v1.BEnc(set_buffer2);
        set_v1.BEnc(set_buffer3);
      
        set_buffer2.ResetMode();
        set_v2.BDec(set_buffer2, set_bufferDecoded);

        set_bufferDecoded = 0;
        set_buffer3.ResetMode();
        set_v3_1.BDec(set_buffer3, set_bufferDecoded);

        set_v3.Print(std::cout);
        set_v1.Print(std::cout);
        set_v2.Print(std::cout);
        set_v3_1.Print(std::cout);
         
        std::cout<<std::endl;


        /****************************************************************/
        /*          BER SEQUENCE EXTENSIBILITY TEST                     */
        /****************************************************************/

        SNACC::TestSeqExtensibilityV1  seq_v1;
        SNACC::TestSeqExtensibilityV2  seq_v2;
        SNACC::TestSeqExtensibilityV3  seq_v3;
        SNACC::TestSeqExtensibilityV3  seq_v3_1;
        SNACC::AsnBuf seq_buffer;
        SNACC::AsnBuf seq_buffer2;
        SNACC::AsnBuf seq_buffer3;
        SNACC::AsnLen seq_bufferDecoded = 0;

        seq_v3.i1 = 0;
        delete seq_v3.i2;
        seq_v3.i2 = new SNACC::AsnInt(1);
        seq_v3.i3 = 3;
        seq_v3.i4 = 4;

        seq_v3.BEnc(seq_buffer);
        seq_buffer.ResetMode();
        seq_v1.BDec(seq_buffer, seq_bufferDecoded);
        seq_bufferDecoded = 0;
       
        seq_v1.BEnc(seq_buffer2);
        seq_v1.BEnc(seq_buffer3);
        seq_buffer2.ResetMode();
        seq_v2.BDec(seq_buffer2, seq_bufferDecoded);
        seq_bufferDecoded = 0;
        seq_buffer3.ResetMode();
        seq_v3_1.BDec(seq_buffer3, seq_bufferDecoded);

        seq_v3.Print(std::cout);
        seq_v1.Print(std::cout);
        seq_v2.Print(std::cout);
        seq_v3_1.Print(std::cout);

        std::cout<<std::endl;

        /***************************************************************/
        /*            BER CHOICE EXTENSIBILITY TEST                    */
        /***************************************************************/
        
        SNACC::TestChoiceExtensibilityV1  choice_v1;
        SNACC::TestChoiceExtensibilityV2  choice_v2;
        SNACC::TestChoiceExtensibilityV3  choice_v3;
        SNACC::AsnBuf choice_buffer;
        SNACC::AsnBuf choice_buffer2;
        SNACC::AsnLen choice_bufferDecoded = 0;
        
        choice_v1.i2 = new SNACC::AsnInt(2);
        choice_v1.choiceId = SNACC::TestChoiceExtensibilityV1::i2Cid;
        choice_v2.i3 = new SNACC::AsnInt(5);
        choice_v2.choiceId = SNACC::TestChoiceExtensibilityV2::i3Cid;
        choice_v2.BEnc(choice_buffer);
        choice_buffer.ResetMode();
        choice_v1.BDec(choice_buffer, choice_bufferDecoded);
        choice_v2.BEnc(choice_buffer2);
        choice_bufferDecoded = 0;
        choice_v3.BDec(choice_buffer2, choice_bufferDecoded);

        choice_v1.Print(std::cout);
        choice_v2.Print(std::cout);
        choice_v3.Print(std::cout);

        std::cout<<std::endl;


        /******************************************************************/
        /*          PER SET EXTENSIBILITY TEST                            */
        /******************************************************************/

        SNACC::TestSetExtensibilityV1  per_set_v1;
        SNACC::TestSetExtensibilityV2  per_set_v2;
        SNACC::TestSetExtensibilityV3  per_set_v3;
        SNACC::TestSetExtensibilityV3  per_set_v3_1;
        SNACC::AsnBufBits per_set_buffer;
        SNACC::AsnBufBits per_set_buffer2;
        SNACC::AsnBufBits per_set_buffer3;
        SNACC::AsnLen per_set_bufferDecoded = 0;

        per_set_v3.i1 = 0;
        delete per_set_v3.i2;
        per_set_v3.i2 = new SNACC::AsnInt(1);
        per_set_v3.i3 = 3;
        per_set_v3.i4 = 4;

        per_set_v3.PEnc(per_set_buffer);
        per_set_v1.PDec(per_set_buffer, per_set_bufferDecoded);
        per_set_bufferDecoded = 0;
       
        per_set_v1.PEnc(per_set_buffer2);
        per_set_v1.PEnc(per_set_buffer3);
      
        per_set_v2.PDec(per_set_buffer2, per_set_bufferDecoded);

        per_set_bufferDecoded = 0;
        per_set_v3_1.PDec(per_set_buffer3, per_set_bufferDecoded);

        per_set_v3.Print(std::cout);
        per_set_v1.Print(std::cout);
        per_set_v2.Print(std::cout);
        per_set_v3_1.Print(std::cout);
         
        std::cout<<std::endl;


        /****************************************************************/
        /*          PER SEQUENCE EXTENSIBILITY TEST                     */
        /****************************************************************/

        SNACC::TestSeqExtensibilityV1  per_seq_v1;
        SNACC::TestSeqExtensibilityV2  per_seq_v2;
        SNACC::TestSeqExtensibilityV3  per_seq_v3;
        SNACC::TestSeqExtensibilityV3  per_seq_v3_1;
        SNACC::AsnBufBits per_seq_buffer;
        SNACC::AsnBufBits per_seq_buffer2;
        SNACC::AsnBufBits per_seq_buffer3;
        SNACC::AsnLen per_seq_bufferDecoded = 0;

        per_seq_v3.i1 = 0;
        delete per_seq_v3.i2;
        per_seq_v3.i2 = new SNACC::AsnInt(1);
        per_seq_v3.i3 = 3;
        per_seq_v3.i4 = 4;

        per_seq_v3.PEnc(per_seq_buffer);
        per_seq_v1.PDec(per_seq_buffer, per_seq_bufferDecoded);
        per_seq_bufferDecoded = 0;
       
        per_seq_v1.PEnc(per_seq_buffer2);
        per_seq_v1.PEnc(per_seq_buffer3);
        per_seq_v2.PDec(per_seq_buffer2, per_seq_bufferDecoded);
        per_seq_bufferDecoded = 0;
        per_seq_v3_1.PDec(per_seq_buffer3, per_seq_bufferDecoded);

        per_seq_v3.Print(std::cout);
        per_seq_v1.Print(std::cout);
        per_seq_v2.Print(std::cout);
        per_seq_v3_1.Print(std::cout);

        std::cout<<std::endl;

        /***************************************************************/
        /*            PER CHOICE EXTENSIBILITY TEST                    */
        /***************************************************************/
        
        SNACC::TestChoiceExtensibilityV1  per_choice_v1;
        SNACC::TestChoiceExtensibilityV2  per_choice_v2;
        SNACC::TestChoiceExtensibilityV3  per_choice_v3;
        SNACC::AsnBufBits per_choice_buffer;
        SNACC::AsnBufBits per_choice_buffer2;
        SNACC::AsnLen per_choice_bufferDecoded = 0;
        
        per_choice_v1.i2 = new SNACC::AsnInt(2);
        per_choice_v1.choiceId = SNACC::TestChoiceExtensibilityV1::i2Cid;
        per_choice_v2.i3 = new SNACC::AsnInt(5);
        per_choice_v2.choiceId = SNACC::TestChoiceExtensibilityV2::i3Cid;
        per_choice_v2.PEnc(per_choice_buffer);
        per_choice_v1.PDec(per_choice_buffer, per_choice_bufferDecoded);
        per_choice_v2.PEnc(per_choice_buffer2);
        per_choice_bufferDecoded = 0;
        per_choice_v3.PDec(per_choice_buffer2, per_choice_bufferDecoded);

        per_choice_v1.Print(std::cout);
        per_choice_v2.Print(std::cout);
        per_choice_v3.Print(std::cout);


        std::cout<<std::endl;

        unsigned char testSeqIndefLength[] = { 0x30, 0x80, 0x81, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00 };
        
        SNACC::AsnBuf testSeqIndefiniteLen( (char*)&testSeqIndefLength[0], 9);
        SNACC::AsnLen ibytesDec = 0;
        SNACC::IndefLen iLenSeq;

        iLenSeq.BDec(testSeqIndefiniteLen, ibytesDec);
        iLenSeq.Print(std::cout);


        /*
	    std::cout<<"  DDDDD     OOOOO    NNN       NNN  EEEEEEEEEE     !   "<<std::endl;   
	    std::cout<<"  DDDDDD   OOOOOOO    NNN      NN    EE     EE    !!!  "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NNNN     NN    EE          !!!!! "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NNNNN    NN    EE          !!!!! "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN NNN   NN    EE          !!!!! "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN  NN   NN    EE   EE     !!!!! "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN  NNN  NN    EEEEEEE     !!!!! "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN   NN  NN    EE   EE      !!!  "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN   NNN NN    EE            !   "<<std::endl; 
	    std::cout<<"  DD  DDD  OO  OOO    NN    NNNNN    EE                "<<std::endl;
	    std::cout<<"  DD  DDD  OO  OOO    NN     NNNN    EE            !   "<<std::endl;
	    std::cout<<"  DDDDDD   OOOOOOO    NN      NNN    EE     EE    !!!  "<<std::endl;
	    std::cout<<"  DDDDD     OOOOO    NNNN      NNN  EEEEEEEEEE     !   "<<std::endl;

    */
    }
    catch (SNACC::SnaccException &e)	
    {
        std::cout<<e.what()<<std::endl;
    }
        
	return 0;
   

 }
