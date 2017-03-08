/* JKG -- 7/31/03 --  */
/*  ../SNACC/compiler/back-ends/c++-gen/cxxconstraints.c  */
/*  This file contains implementations of functions for   */
/*  checking constraints and converting .asn1             */   
/*  primitives (with constraints) into valid C++ code     */
/*  Their respective prototypes are in:                   */
/*       'cxxconstraints.h' in the pwd                    */

#include "config.h"
#include "cxxconstraints.h"
#include <string.h>


/* Prints the default instantiation for a typedefined object */
void PrintTypeDefDefault PARAMS((hdr, src, td),
 FILE* hdr _AND_
 FILE* src _AND_
 TypeDef* td)
{
  src=src;
  fprintf (hdr, "typedef %s %s;\n\n", td->type->cxxTypeRefInfo->className, td->cxxTypeDefInfo->className);

}


void PrintCxxSetOfSizeConstraint(FILE* hdr, SubtypeValue* sizeConstraint,
								 Module* m, Type* type)
{
	long lBound = 0;
	long uBound = 0;
	int ubExists = 0;
	Value* pValue;

	switch (sizeConstraint->choiceId)
	{
	case SUBTYPEVALUE_SINGLEVALUE:
		pValue = GetValue(sizeConstraint->a.singleValue);
		if (pValue->basicValue->choiceId == BASICTYPE_INTEGER)
			lBound = pValue->basicValue->a.integer;
		else
		{
			PrintErrLoc(m->asn1SrcFileName, (long)type->lineNo);
			fprintf(errFileG, "ERROR - unsupported value in size constraint\n");
			return;
		}
		break;

	case SUBTYPEVALUE_VALUERANGE:
		// Get the lower end value
		pValue = GetValue(sizeConstraint->a.valueRange->lowerEndValue->endValue);
		if (pValue->basicValue->choiceId == BASICTYPE_INTEGER)
		{
			lBound = pValue->basicValue->a.integer;
			if (!sizeConstraint->a.valueRange->lowerEndValue->valueInclusive)
				++lBound;
		}
		else
		{
			PrintErrLoc(m->asn1SrcFileName, (long)type->lineNo);
			fprintf(errFileG, "ERROR - unsupported value in size constraint\n");
			return;
		}

		// Get the upper end value
		pValue = GetValue(sizeConstraint->a.valueRange->upperEndValue->endValue);
		if (pValue->basicValue->choiceId == BASICTYPE_INTEGER)
		{
			uBound = pValue->basicValue->a.integer;
			if (!sizeConstraint->a.valueRange->upperEndValue->valueInclusive)
				--uBound;
			ubExists = 1;
		}
		else if ((pValue->basicValue->choiceId == BASICVALUE_SPECIALINTEGER) &&
			(pValue->basicValue->a.specialInteger == MAX_INT))
		{
			uBound = 0;
			ubExists = 2;
		}
		else
		{
			PrintErrLoc(m->asn1SrcFileName, (long)type->lineNo);
			fprintf(errFileG, "ERROR - unsupported value in size constraint\n");
			return;
		}
		break;

	default:
		PrintErrLoc(m->asn1SrcFileName, (long)type->lineNo);
		fprintf(errFileG, "ERROR - unsupported size constraint\n");
		return;
	}

	// Check that the size constraint is valid
	if ((lBound < 0) || ((ubExists == 1) && (lBound > uBound)))
	{
		PrintErrLoc(m->asn1SrcFileName, (long)type->lineNo);
		fprintf(errFileG, "ERROR - invalid size constraint\n");
		return;
	}

	// Print the size constraints
	fprintf(hdr, "\tSizeConstraint* SizeConstraints() const {\n");
	fprintf(hdr, "\t\tstatic SizeConstraint s = { %ld, %ld, %d }; return &s; }\n",
		lBound, uBound, ubExists);
}



/* Function generates the checkconstraints function for */
/*    a setOf or seqOf with size constraints set        */
void PrintCxxSetOfSizeValRangeConstraints PARAMS((hdr, src, td),
 FILE* hdr _AND_
 FILE* src _AND_
 TypeDef* td)
{
    Subtype* s_type;
    long lboundLower = 0;
    long lboundUpper = 0;
    int ubExists = 2;

    hdr=hdr;
    src=src;
    td=td;

    s_type = (Subtype*)td->type->subtypes->a.single->a.sizeConstraint->a.or->last->data;

    if(s_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->choiceId == BASICTYPE_INTEGER)
    {
        if(s_type->a.single->a.valueRange->lowerEndValue->valueInclusive)
        {
        
            lboundLower = s_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer;
        }
        else
        {
            lboundLower = s_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer+1;
        }

        if(s_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId == BASICTYPE_INTEGER)
        {
            if(s_type->a.single->a.valueRange->upperEndValue->valueInclusive)
            {
	            lboundUpper = s_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer;	
                ubExists = 1;
            }
            else
            {
	            lboundUpper = s_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer-1;
                ubExists = 1;
            }
        }
    }

    if(lboundUpper > 65536)
    {
        ubExists = 2;
    }

    if(ubExists == 2)
    {
        lboundUpper = 0;
    }

   if(lboundLower >= 0 && ((lboundUpper >= 0 && (lboundUpper >= lboundLower)) || ubExists == 2))
    {
        fprintf (hdr, "     SizeConstraint* SizeConstraints()const{\n");
        fprintf (hdr, "         static SizeConstraint s = {%ld, %ld, %d}; SizeConstraint * ps = &s; return ps;}\n", lboundLower, lboundUpper, ubExists);

        //fprintf (hdr, "     SizeConstraint* SizeConstraints(){\n");
        //fprintf (hdr, "         static SizeConstraint s = {%d, %d, %d}; SizeConstraint * ps = &s; return ps;}\n", lboundLower, lboundUpper, ubExists);

        /*
        fprintf (hdr, "     AsnLen PEnc(AsnBufBits &b)const;\n");
        fprintf (src, "AsnLen %s::PEnc(AsnBufBits &b)const\n", td->definedName);
        fprintf (src, "{\n");
        fprintf (src, "         AsnLen len = 0;\n");
        fprintf (src, "         return len;\n}\n"); 

        fprintf (hdr, "     void PDec(AsnBufBits &b, AsnLen &bitsDecoded);\n");
        fprintf (src, "void %s::PDec(AsnBufBits &b, AsnLen &bitsDecoded)\n", td->definedName);
        fprintf (src, "{\n");
        fprintf (src, "}\n");
*/
        /*
        fprintf(hdr, "   virtual int checkConstraints(ConstraintFailList* pConstraintFails)const;\n");
        fprintf(src, "int %s::checkConstraints(ConstraintFailList* pConstraintFails)const\n{\n", td->cxxTypeDefInfo->className);
    			    
        fprintf (src, "  std::string * pTmpError;\n");
        if(ubExists == 1)
        {
            fprintf (src, "  char* ptr = checkSOfValRange(%d, %d);\n\n", lboundLower, lboundUpper);
        }
        else
        {
            fprintf (src, "  char* ptr = checkSOfValRange(%d, 65535/\*TBD-fix check to check for unconstrainted UB*//*);\n\n", lboundLower);*/
/*        }

        fprintf (src, "  if(ptr)\n   {\n");
        fprintf (src, "     if(pConstraintFails!=NULL)\n     {\n");
        fprintf (src, "       pTmpError=pConstraintFails->Append();\n");
        fprintf (src, "       *pTmpError += ptr;\n");
        fprintf (src, "       *pTmpError += \"In function call:  %s::checkConstraints(...)\\n\";\n", td->cxxTypeDefInfo->className);
        fprintf (src, "     }\n");
        fprintf (src, "   }\n   else\n   {\n");
    
        fprintf (src, "     return checkListConstraints(pConstraintFails);\n");
        fprintf (src, "   }\n\n");
        fprintf (src, "   return checkListConstraints(pConstraintFails);\n");
        fprintf(src, "\n}\n");
    }
    else
    {
      
        fprintf(hdr, "      int checkConstraints(ConstraintFailList* pConstraintFails)const;\n");
        fprintf(src, "int %s::checkConstraints(ConstraintFailList* pConstraintFails)const\n{\n", td->cxxTypeDefInfo->className);
        fprintf(src, "   return checkListConstraints(pConstraintFails);\n");
    
        fprintf(src, "}\n\n");
        */
    }
   
}

void PrintCxxSetOfSizeSingleValConstraints PARAMS((hdr, src, td),
 FILE* hdr _AND_
 FILE* src _AND_
 TypeDef* td)
{
    Subtype* s_type;
    long lbound = 0;

    hdr=hdr;
    src=src;
    td=td;

    s_type = (Subtype*)td->type->subtypes->a.single->a.sizeConstraint->a.or->last->data;

    if(s_type->a.single->a.singleValue->basicValue->choiceId == BASICTYPE_INTEGER)
    {
        lbound = s_type->a.single->a.singleValue->basicValue->a.integer;
    }

    if(lbound >= 0)
    {
        fprintf (hdr, "     SizeConstraint* SizeConstraints()const{\n");
        fprintf (hdr, "         SizeConstraint s = {%ld, 0, 0}; SizeConstraint * ps = &s; return ps;}\n", lbound);

        fprintf (hdr, "     SizeConstraint* SizeConstraints(){\n");
        fprintf (hdr, "         SizeConstraint s = {%ld, 0, 0}; SizeConstraint * ps = &s; return ps;}\n", lbound);

      /*

        fprintf (hdr, "     AsnLen PEnc(AsnBufBits &b)const;\n");
        fprintf (src, "AsnLen %s::PEnc(AsnBufBits &b)const\n", td->definedName);
        fprintf (src, "{\n");
        fprintf (src, "     FUNC(\"%s::PEnc\");\n", td->definedName);
        fprintf (src, "     AsnLen len = 0;\n");
        fprintf (src, "     typename AsnList<T>::ListElmt *currElmt;\n");
        fprintf (src, "     long lbound = %d;\n\n", lbound);
        fprintf (src, "     if(Count() == lbound)\n");
        fprintf (src, "     {\n");
        fprintf (src, "         for (currElmt = first; currElmt != NULL; currElmt = currElmt->next)\n");
        fprintf (src, "             len += currElmt->elmt->PEnc (b);\n");
        fprintf (src, "     }\n");
        fprintf (src, "     else\n");
        fprintf (src, "     {\n");
        fprintf (src, "         throw (\"%s count does not match singlevalue constraint bound\");\n", td->definedName);
        fprintf (src, "     }\n");
        fprintf (src, "     return len;\n}\n"); 

        fprintf (hdr, "     void PDec(AsnBufBits &b, AsnLen &bitsDecoded);\n");
        fprintf (src, "void %s::PDec(AsnBufBits &b, AsnLen &bitsDecoded)\n", td->definedName);
        fprintf (src, "{\n");
        fprintf (src, "     T *listElmt;\n");
        fprintf (src, "     long lbound = %d;\n", lbound);
        fprintf (src, "     while(lbound)\n");
        fprintf (src, "     {\n");
        fprintf (src, "         listElmt = Append();\n");
        fprintf (src, "         listElmt->PDec(b, bitsDecoded);\n");
        fprintf (src, "         lbound--;\n");
        fprintf (src, "     }\n");
        fprintf (src, "}\n");
*/
        /*
        fprintf(hdr, "   virtual int checkConstraints(ConstraintFailList* pConstraintFails)const;\n");
        fprintf(src, "int %s::checkConstraints(ConstraintFailList*pConstraintFails)const\n{\n", td->cxxTypeDefInfo->className);
    				

        fprintf (src, "  std::string * pTmpError;\n");
        fprintf (src, "  char* ptr = checkSOfSingleVal(%d);\n\n", lbound);
        fprintf (src, "  if(ptr)\n   {\n");
        fprintf (src, "     if(pConstraintFails!=NULL)\n     {\n");
        fprintf (src, "       pTmpError=pConstraintFails->Append();\n");
        fprintf (src, "       *pTmpError += ptr;\n");
        fprintf (src, "       *pTmpError += \"In function call:  %s::checkConstraints(...)\\n\";\n", td->cxxTypeDefInfo->className);
        fprintf (src, "     }\n");
        fprintf (src, "   }\n   else\n   {\n");
        
        fprintf (src, "     return checkListConstraints(pConstraintFails);\n");
        fprintf (src, "   }\n\n");
        fprintf (src, "   return checkListConstraints(pConstraintFails);\n");
        fprintf(src, "\n}\n");
        */
    }
    else
    {
        /*
        fprintf(hdr, "      int checkConstraints(ConstraintFailList* pConstraintFails)const;\n");
        fprintf(src, "int %s::checkConstraints(ConstraintFailList* pConstraintFails)const\n{\n", td->cxxTypeDefInfo->className);
        fprintf(src, "   return checkListConstraints(pConstraintFails);\n");
    
        fprintf(src, "}\n\n");
        */
    }

}
