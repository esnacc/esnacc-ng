#include "cxxmultipleconstraints.h"
#include <string.h>
#include <math.h>
#include <ctype.h>

extern char *bVDAGlobalDLLExport;

/*finds all of the alphabets in the subtype constraints */
char* FindPermittedAlpha PARAMS((curr, cPermittedAlphabet, iAlphasize),
	AsnListNode* curr _AND_
	unsigned char* cPermittedAlphabet _AND_
	int *iAlphasize)
{
	Subtype* currS_type=NULL;
    Subtype* fakeList = NULL;

	while(curr)
	{
		currS_type=curr->data;
	
		if(currS_type->choiceId==SUBTYPE_SINGLE && 
				currS_type->a.single->choiceId==SUBTYPEVALUE_PERMITTEDALPHABET)
		{
			AsnListNode* currAlpha;

            if(currS_type->choiceId == SUBTYPE_OR)
			{
			    currAlpha=currS_type->a.single->a.permittedAlphabet->a.or->first;
	    	    PrintCxxPermittedAlphabetHandler(currAlpha, cPermittedAlphabet, iAlphasize);
   		    }	
            else if(currS_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(currS_type->a.single->a.permittedAlphabet, fakeList->a.or);
                currAlpha = fakeList->a.or->first;

	    	    PrintCxxPermittedAlphabetHandler(currAlpha, cPermittedAlphabet, iAlphasize);
                  
             }

		}
		curr=curr->next;
	}


	return (char*)cPermittedAlphabet;
}


/*Prints the declarations for constraint variables in the header file*/
int BasicTypeString_LISTS PARAMS((hdr, src, scList, iSCPresent, curr, currS_type),
	FILE* hdr _AND_
	FILE* src _AND_
	SizeConstraint *scList _AND_
	int iSCPresent _AND_
	AsnListNode* curr _AND_
	Subtype* currS_type)
{
	AsnListNode* pTmpnode;
    Subtype* fakeList = NULL;
	/*while there are nodes in the subtype list*/
	while(curr)
	{
		/*point to the current subtype*/
		currS_type=curr->data;
		
		if(currS_type->choiceId==SUBTYPE_SINGLE)
		{
			switch(currS_type->a.single->choiceId)
			{

			case SUBTYPEVALUE_VALUERANGE:
				{
					break;
				}/*case SUBTYPEVALUE_VALUERANGE*/

			case SUBTYPEVALUE_SINGLEVALUE:
				{
					break;
				}/*case SUBTYPEVALUE_SINGLEVALUE*/

			case SUBTYPEVALUE_SIZECONSTRAINT:
				{
				
                    if(currS_type->choiceId == SUBTYPE_OR)
			        {
					    pTmpnode=currS_type->a.single->a.sizeConstraint->a.or->first;
					    iSCPresent += SizeConstraint_LISTS(hdr, src, scList, iSCPresent, pTmpnode, currS_type);
    		        }	
                    else if(currS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(currS_type->a.single->a.sizeConstraint, fakeList->a.or);
                        pTmpnode = fakeList->a.or->first;

					    iSCPresent += SizeConstraint_LISTS(hdr, src, scList, iSCPresent, pTmpnode, currS_type);
                     }
    	
					break;
				}/*case SUBTYPEVALUE_SIZECONSTRAINT*/

			default:
				{
					break;
				}/*default*/

			}/*switch(curr......SINGLE)*/
		
		}/*if(curr......SINGLE)*/

		curr=curr->next;
	}/*while(curr)*/

	src=src; /*avoids warning for unused parameter*/
	return iSCPresent; 
}/*int BasicTypeString_LISTS*/

/*Finds all of the valueranges and single values in the subtype constraints*/
int ValueRange_LISTS PARAMS((hdr, src, vrList, iVRPresent, curr, currS_type),
	FILE* hdr _AND_
	FILE* src _AND_
	ValueRange* vrList _AND_
	int iVRPresent _AND_
	AsnListNode* curr _AND_
	Subtype* currS_type)
{
	int found = 0;
	vrList = vrList;

	//int j = 0;
	hdr = hdr;

	/*while there are still subtyes found in the list*/
	while(curr)
	{
		/*point to the current subtype*/
        currS_type=curr->data;
		
		if(currS_type->choiceId==SUBTYPE_SINGLE)
		{
			switch(currS_type->a.single->choiceId)
			{
			case SUBTYPEVALUE_VALUERANGE:
				{
					if(iVRPresent < 15  &&  currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->choiceId==BASICVALUE_INTEGER)
					{
						found += 1;
						if(currS_type->a.single->a.valueRange->lowerEndValue->valueInclusive)
						{
							vrList[iVRPresent].lowerBound = currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer;
						}
						else
						{
							vrList[iVRPresent].lowerBound = currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer + 1;
						}

						if(currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId==BASICVALUE_INTEGER)
						{
							if(currS_type->a.single->a.valueRange->upperEndValue->valueInclusive)
							{
								vrList[iVRPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer;	
							
							}
							else
							{
								vrList[iVRPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer + 1;	
							}

							vrList[iVRPresent].upperBoundExists = 1;
						}
						else
						{
							vrList[iVRPresent].upperBound = 0;
							vrList[iVRPresent].upperBoundExists = 0;
						}

						iVRPresent++;
					}
						
					
					break;
				}/*case SUBTYPE_VALUERANGE*/
			case SUBTYPEVALUE_SINGLEVALUE:
				{
					if(iVRPresent < 15 && currS_type->a.single->a.singleValue->basicValue->choiceId == BASICVALUE_INTEGER)
					{
						found += 1;
						vrList[iVRPresent].lowerBound = currS_type->a.single->a.singleValue->basicValue->a.integer;

						vrList[iVRPresent].upperBound = 0;
						vrList[iVRPresent].upperBoundExists = 2;

						iVRPresent ++;
					}
					break;
				}/*case SUBTYPE_SINGLEVALUE*/

			case SUBTYPEVALUE_CONTAINED:
			default:
				{
					break;
				}/*default*/
			
			}/*switch(curr......SINGLE)*/

		}/*if(curr.......SINGLE)*/

		curr=curr->next;
	
	}/*while(curr)*/

	src=src;/*avoids warning on unused parameter*/ 
	
	return found;
}/*int BasicTypeInt_LISTS*/

/*assembles all permitted alphabets into one larger inclusive alphabet*/
char* PrintCxxPermittedAlphabetHandler PARAMS((currAlpha, cPermittedAlphabet, iAlphaSize),
	AsnListNode* currAlpha _AND_
	unsigned char* cPermittedAlphabet _AND_
	int *iAlphasize)
{
	Subtype* currS_type;
	int found = 0;
	int x = 0;

	/************************************/
	/*        To avoid warning          */

		    currAlpha=currAlpha;

	/*    Reference all Parameters      */
	/************************************/

	/*while there are alphabets found*/
	while(currAlpha)
	{
		currS_type=currAlpha->data;
		if(currS_type->choiceId==SUBTYPE_SINGLE &&
			currS_type->a.single->choiceId==SUBTYPEVALUE_SINGLEVALUE)
		{
			/*concatenate the alphabets together*/
			char* temp = currS_type->a.single->a.singleValue->basicValue->a.asciiText->octs;
			
			int tempSize = strlen(temp);
	
			while(tempSize)
			{
				found = 0;

				for(x = 0; x < *iAlphasize; x++)
				{
					if(cPermittedAlphabet[x] == temp[tempSize - 1])
						found = 1;
				}

				if(found == 0)
				{
					cPermittedAlphabet[*iAlphasize] = temp[tempSize - 1];
					*iAlphasize += 1;
				}

				tempSize--;
			}				

		}/*if(curr.......SINGLEVALUE)*/

		if(currS_type->choiceId==SUBTYPE_SINGLE &&
			currS_type->a.single->choiceId==SUBTYPEVALUE_VALUERANGE)
		{
			char* temp;
			unsigned char lower;
			unsigned char upper;
			unsigned char c_alpha;

			/*concatenate the alphabets together*/
			temp=currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.asciiText->octs;
			lower=temp[0];
			
			temp=currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.asciiText->octs;
			upper=temp[0];

			for( ; lower<=upper; lower++)
			{
				found = 0;	
				c_alpha = lower;

				for(x = 0; x < *iAlphasize; x++)
				{
					if(cPermittedAlphabet[x] == c_alpha)
						found = 1;
				}

				if(found == 0)
				{
					cPermittedAlphabet[*iAlphasize] = c_alpha;
					*iAlphasize += 1;
				}
			}


		}/*if(curr.......VALUERANGE)*/

		currAlpha=currAlpha->next;
	}/*while(currAlpha)*/

	
	return (char*)cPermittedAlphabet;
}/*char* PrintCxxPermittedAlphabetHandler*/

/*Handles Size Constraints*/
int SizeConstraint_LISTS PARAMS((hdr, src, scList, iSCPresent, curr, currS_type),
	FILE* hdr _AND_
	FILE* src _AND_
	SizeConstraint* scList _AND_
	int iSCPresent _AND_
	AsnListNode* curr _AND_
	Subtype* currS_type)
{
	int found = 0;
	scList = scList;

	//int j = 0;
	hdr = hdr;

	/*while there are still subtyes found in the list*/
	while(curr)
	{
		/*point to the current subtype*/
		currS_type=curr->data;
		
		if(currS_type->choiceId==SUBTYPE_SINGLE)
		{
			switch(currS_type->a.single->choiceId)
			{
			case SUBTYPEVALUE_VALUERANGE:
				{
					found += 1;
					if((currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId==BASICVALUE_INTEGER && 
                        currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer <= 65536) ||
                        currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId==BASICVALUE_LOCALVALUEREF) 
						
					{
						if(iSCPresent < 15  &&  currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->choiceId==BASICVALUE_INTEGER)
						{
							if(currS_type->a.single->a.valueRange->lowerEndValue->valueInclusive)
							{
								scList[iSCPresent].lowerBound = currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer;
										 
							}
							else
							{
								scList[iSCPresent].lowerBound = currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.integer + 1;
							}
						}
						else if(currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->choiceId == BASICVALUE_LOCALVALUEREF)
					    {
                            scList[iSCPresent].lowerBound = currS_type->a.single->a.valueRange->lowerEndValue->endValue->basicValue->a.localValueRef->link->value->basicValue->a.integer;
                        }

                        if(currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId==BASICVALUE_INTEGER)
                        {
                            if(currS_type->a.single->a.valueRange->upperEndValue->valueInclusive)
						    {
							    scList[iSCPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer;	
							    
						    }
						    else
						    {
							    scList[iSCPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.integer + 1;	
						    }
                        }
                        else if(currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->choiceId == BASICVALUE_LOCALVALUEREF)
                        {
                            if(currS_type->a.single->a.valueRange->upperEndValue->valueInclusive)
						    {
							    scList[iSCPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.localValueRef->link->value->basicValue->a.integer;
							    
						    }
						    else
						    {
							    scList[iSCPresent].upperBound = currS_type->a.single->a.valueRange->upperEndValue->endValue->basicValue->a.localValueRef->link->value->basicValue->a.integer + 1;
						    }
                        }
						scList[iSCPresent].upperBoundExists = 1;
						
						iSCPresent++;
					
					}
					else
					{
						scList[iSCPresent].upperBoundExists = 2;
						iSCPresent++;
					}
					
					break;
				}/*case SUBTYPE_VALUERANGE*/
			case SUBTYPEVALUE_SINGLEVALUE:
				{
					found += 1;
					if(iSCPresent < 15 && currS_type->a.single->a.singleValue->basicValue->choiceId == BASICVALUE_INTEGER)
					{
						scList[iSCPresent].lowerBound = currS_type->a.single->a.singleValue->basicValue->a.integer;

						scList[iSCPresent].upperBound = 0;
						scList[iSCPresent].upperBoundExists = 0;
						iSCPresent++;
					}
					else if(currS_type->a.single->a.singleValue->basicValue->choiceId == BASICVALUE_LOCALVALUEREF)
					{
                        scList[iSCPresent].lowerBound = currS_type->a.single->a.singleValue->basicValue->a.localValueRef->link->value->basicValue->a.integer;
                        scList[iSCPresent].upperBound = 0;
						scList[iSCPresent].upperBoundExists = 0;
						scList[iSCPresent].upperBoundExists = 0;
						iSCPresent++;
					}
					break;
				}/*case SUBTYPE_SINGLEVALUE*/

			case SUBTYPEVALUE_CONTAINED:
			default:
				{
					break;
				}/*default*/
			
			}/*switch(curr......SINGLE)*/

		}/*if(curr.......SINGLE)*/

		curr=curr->next;
	
	}/*while(curr)*/

	src=src;/*avoids warning on unused parameter*/ 
	
	return found;
}/*int BasicTypeInt_LISTS*/

/*Handles constraints found in the subtypes of primitives*/
int PrintCxxMultiConstraintOrHandler PARAMS((hdr, src, definedName, e, i),
	FILE* hdr _AND_
	FILE* src _AND_
	char* definedName _AND_
    NamedType* e _AND_
	int i)
{
	int returnvalue=0;
    char *pszGlobalExport="";
	char *className;
    char *tmpClassName;
	
	className = strdup(e->type->cxxTypeRefInfo->fieldName);
	className[0] = (char)toupper(className[0]);
    
	if(!strcmp(className, e->type->cxxTypeRefInfo->className))
    {
        tmpClassName = strdup(className);
        free(className);
        className = malloc(strlen(tmpClassName) + 3);
        strcpy(className, tmpClassName);
        strcat(className, "_\0");
        free(tmpClassName);
    }
    else if(i == 1 || i == 3)
    {
        if(!strcmp(className, definedName) )
        {
            tmpClassName = strdup(className);
            free(className);
            className = malloc(strlen(tmpClassName) + 3);
            strcpy(className, tmpClassName);
            strcat(className, "_\0");
            free(tmpClassName);
        }
    }
	
    className = Asn1TypeName2CTypeName(className);
    e->type->cxxTypeRefInfo->fieldName = Asn1TypeName2CTypeName(e->type->cxxTypeRefInfo->fieldName);
    e->type->cxxTypeRefInfo->className = Asn1TypeName2CTypeName(e->type->cxxTypeRefInfo->className);
    
    if (bVDAGlobalDLLExport)
        pszGlobalExport = bVDAGlobalDLLExport;


	switch(e->type->basicType->choiceId)
	{
	
	case BASICTYPE_INTEGER:
		{
			Subtype* s_type;
			Subtype* currS_type=NULL;
			Subtype* andS_type=NULL;
            Subtype* fakeList=NULL;
			AsnListNode* curr;
			AsnListNode* and_curr;
			int iVRPresent = 0;
			int count = 0;
			
			ValueRange vrList[15];		
			
			s_type = e->type->subtypes; 
			
			if(s_type->choiceId==SUBTYPE_AND)
			{
				and_curr=s_type->a.and->first;
				while(and_curr)
				{	
					andS_type=and_curr->data;
                    if(andS_type->choiceId == SUBTYPE_OR)
			        {
				        curr=andS_type->a.or->first;
				        iVRPresent += ValueRange_LISTS(hdr, src, vrList, iVRPresent, curr, currS_type);
			        }	
                    else if(andS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(andS_type, fakeList->a.or);
                        curr = fakeList->a.or->first;

                        iVRPresent += ValueRange_LISTS(hdr, src, vrList, iVRPresent, curr, currS_type);
                    }
					and_curr=and_curr->next;
				}
			}
			else if(s_type->choiceId == SUBTYPE_OR)
			{
				curr=s_type->a.or->first;
				iVRPresent += ValueRange_LISTS(hdr, src, vrList, iVRPresent, curr, currS_type);
			}	
            else if(s_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(s_type, fakeList->a.or);
                curr = fakeList->a.or->first;

                iVRPresent += ValueRange_LISTS(hdr, src, vrList, iVRPresent, curr, currS_type);
            }

			

            /*print the generic header for the generated class*/
			fprintf (hdr, "class %s %s: public AsnInt  {\n", pszGlobalExport, className);
			fprintf (hdr, "    public:\n");
			fprintf (hdr, "        %s(AsnIntType val=0):AsnInt(val){ }\n", className);
			fprintf (hdr, "        %s(const char *str, bool unsignedFlag = true):AsnInt(str, unsignedFlag){ }\n", className);
			fprintf (hdr, "        %s(const AsnOcts &o, bool unsignedFlag = true):AsnInt(o, unsignedFlag){ }\n", className);
			fprintf (hdr, "        %s(const char *str, const size_t len, bool unsignedFlag = true):AsnInt(str, len, unsignedFlag){ }\n", className);
			fprintf (hdr, "        %s(const %s &that):AsnInt(that){ }\n\n", className, className);
			fprintf(hdr, "		%s & operator =(const %s &o);\n", className, className);


			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s & \n", className);


			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s::operator =(const %s &that)\n", className, className);
			fprintf(src, "{\n");
			fprintf(src, "	m_len = that.m_len;\n");
			fprintf(src, "	delete[] m_bytes;\n");
			fprintf(src, "	m_bytes = new unsigned char[m_len];\n");
			fprintf(src, "	memcpy(m_bytes, that.m_bytes, m_len);\n");			
			fprintf(src, "	return *this;\n}\n\n");

			fprintf (hdr, "		const ValueRange* ValueRanges(int &sizeVRList) const;\n\n");
			fprintf (src, "const ValueRange*  \n");
			
			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s::ValueRanges(int &sizeVRList) const\n", className);
			fprintf (src, "{\n");
	
			if(iVRPresent > 0)
			{
				fprintf(src, "		static const ValueRange %s_ValueRangeList[] = \n			{",
					e->type->cxxTypeRefInfo->fieldName);
				
				for(count = 0; count < iVRPresent; count++)
				{
					fprintf(src, "{ %ld, %ld, %d }", 
						vrList[count].lowerBound,
						vrList[count].upperBound,
						vrList[count].upperBoundExists);
					if(count + 1 < iVRPresent)
					{
						fprintf(src, ",\n		");
					}
				}

				fprintf(src, "};\n\n");
			    fprintf(src, "		sizeVRList = %d;\n", iVRPresent);
				fprintf(src, "		return &%s_ValueRangeList[0];\n", e->type->cxxTypeRefInfo->fieldName);				
			}
			else
			{
				fprintf(src, "		sizeVRList = 0;\n");
                fprintf(src, "		return NULL;\n");
			}
		
			fprintf (src, "}\n\n");
/*
			fprintf(hdr, "		  AsnLen PEnc (AsnBufBits &_b, bool bAlign = false){return PEncConstraints(_b, bAlign);}\n");
	
			fprintf(hdr, "	     void PDec (AsnBufBits &_b, AsnLen &bitsDecoded, bool bAlign = false){PDecConstraints(_b, bitsDecoded, bAlign);}\n");
*/
		
		fprintf (hdr, "  };\n");
    
        if(i != 3 && i != 0)
        {
		    /* Last we declare an instance of the internal class we */
		    /*   just created                                       */
		    if(i==0)
		    {		
			    fprintf (hdr, "typedef ");
		    }/*if(i==0)*/
 
		    fprintf (hdr, "  %s		", className);

		    if (e->type->cxxTypeRefInfo->isPtr)
		    {
			    fprintf (hdr, "*");
		    }/*if (e->type->cxxTypeRefInfo->isPtr)*/

		    fprintf (hdr, "%s;\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (hdr, "\n\n");
        }

        free(e->type->cxxTypeRefInfo->className);
        e->type->cxxTypeRefInfo->className = strdup(className);
		returnvalue=1;
			break;
		}/*case BASICTYPE_INTEGER*/
    
	
	case BASICTYPE_NUMERIC_STR:
    case BASICTYPE_PRINTABLE_STR:
    case BASICTYPE_IA5_STR:
    case BASICTYPE_VISIBLE_STR:
		{
			int bAlphafound=0;

			unsigned char cPermittedAlphabet[500];
			char cAlpha[1000];
			int iAlphasize = 0;
			int iSort1 = 0;
			int iSort2 = 0;
			unsigned char cSort = 0x00;
			int j = 0;
			Subtype* s_type;
			Subtype* currS_type=NULL;
			Subtype* andS_type=NULL;
            Subtype* fakeList = NULL;
			AsnListNode* curr;
			AsnListNode* and_curr;
			char* cTmp;
			int iSCPresent = 0;
			int count = 0;
			int iSCLowerBound = 0;
			int iSCUpperBound = 0;
			SizeConstraint scList[15];
		
       
	        /*print the generic header for the generated class*/
	        fprintf (hdr, "class %s %s: public %s  {\n", pszGlobalExport, className, e->type->cxxTypeRefInfo->className);
	        s_type = e->type->subtypes;
            iSort2 = iSort2;

		    if(s_type->choiceId==SUBTYPE_AND)
		    {
			    and_curr=s_type->a.and->first;
			    while(and_curr)
			    {	
				    andS_type=and_curr->data;
				    if(andS_type->choiceId == SUBTYPE_OR)
			        {
				        curr=andS_type->a.or->first;
				        cTmp=FindPermittedAlpha(curr, cPermittedAlphabet, &iAlphasize);
			        }	
                    else if(andS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(andS_type, fakeList->a.or);
                        curr = fakeList->a.or->first;

                        cTmp=FindPermittedAlpha(curr, cPermittedAlphabet, &iAlphasize);
                    }
				    
				    and_curr=and_curr->next;
			    }
		    }
		    else if(s_type->choiceId == SUBTYPE_OR)
		    {
			    curr=s_type->a.or->first;
			    cTmp=FindPermittedAlpha(curr, cPermittedAlphabet, &iAlphasize);
		    }	
            else if(s_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(s_type, fakeList->a.or);
                curr = fakeList->a.or->first;

                cTmp=FindPermittedAlpha(curr, cPermittedAlphabet, &iAlphasize);
            }

			    
			    
		    if(iAlphasize > 0)
		    {
			    bAlphafound=1;
			    
	    
			    for(iSort1 = 0; iSort1 < iAlphasize; iSort1++)
			    {
				    for(iSort2 = (iSort1 +1); iSort2 < iAlphasize; iSort2++)
				    {
					    if(cPermittedAlphabet[iSort1] > cPermittedAlphabet[iSort2])
					    {	
						    cSort = cPermittedAlphabet[iSort2];
						    cPermittedAlphabet[iSort2] = cPermittedAlphabet[iSort1];
						    cPermittedAlphabet[iSort1] = cSort;
					    }
				    }
			    }

			    for(iSort1 = 0; iSort1 < iAlphasize; iSort1++)
			    {
				    if(iSort1 == 0)
					    j += sprintf(cAlpha+j, "0x%x", cPermittedAlphabet[iSort1]);
				    else
					    j += sprintf(cAlpha+j, ",0x%x", cPermittedAlphabet[iSort1]);
			    }
		    }
		
			s_type = e->type->subtypes; 

			if(s_type->choiceId==SUBTYPE_AND)
			{
                
				and_curr=s_type->a.and->first;
				while(and_curr)
				{	
					andS_type=and_curr->data;
                    if(andS_type->choiceId == SUBTYPE_OR)
			        {
				        curr=andS_type->a.or->first;
				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		        }	
                    else if(andS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(andS_type, fakeList->a.or);
                        curr = fakeList->a.or->first;

				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
                    }
					and_curr=and_curr->next;
				}
			}
			else if(s_type->choiceId == SUBTYPE_OR)
			{
				curr=s_type->a.or->first;
				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		}	
            else if(s_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(s_type, fakeList->a.or);
                curr = fakeList->a.or->first;

				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
            }
	


			fprintf (hdr, "     public:\n");
			fprintf (hdr, "        %s():%s(){ }\n\n", className, e->type->cxxTypeRefInfo->className);
    		fprintf (hdr, "		const SizeConstraint* SizeConstraints(int &sizeList)const;\n\n");
			fprintf (src, "const SizeConstraint*  \n");
			
			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s::SizeConstraints(int &sizeList)const\n", className);
			fprintf (src, "{\n");

			if(iSCPresent > 0 )
			{
				count = 0;
				if(scList[count].upperBoundExists == 1 &&
					scList[count].upperBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].upperBound;
				}

				if(scList[count].upperBoundExists == 0 &&
					scList[count].lowerBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].lowerBound;
				}

				if(scList[count].lowerBound < iSCLowerBound)
				{
					iSCLowerBound = scList[count].lowerBound;
				}

				while(count < iSCPresent)
				{
					if(scList[count].upperBoundExists == 2)
					{
						iSCPresent = 0;
					}
										
					count++;
				}  
			}

			if(iSCPresent > 0 )
			{
				fprintf(src, "	static const SizeConstraint %s_SizeConstraintList[] = \n			{",	e->type->cxxTypeRefInfo->fieldName);
								
				for(count = 0; count < iSCPresent; count++)
				{
					fprintf(src, "{ %ld, %ld, %d }", 
						scList[count].lowerBound,
						scList[count].upperBound,
						scList[count].upperBoundExists);
					if(count + 1 < iSCPresent)
					{
						fprintf(src, ",\n			");
					}
				}

				fprintf(src, "};\n\n");
				fprintf(src, "	sizeList = %d;\n", iSCPresent);
                fprintf(src, "	return &%s_SizeConstraintList[0];\n", e->type->cxxTypeRefInfo->fieldName);
			}
			else
			{
			    fprintf(src, "	sizeList = 0;\n");
                fprintf(src, "	return NULL;\n");
			}
		    fprintf (src, "}\n\n\n");

            if(bAlphafound == 1)
			{
                fprintf (hdr, "		const char* PermittedAlphabet(int &sizePermittedAlpha)const;\n\n");
			    fprintf (src, "const char*  \n");
			    
			    if(i == 1 || i == 3)
			    {
				    fprintf(src, "%s::", definedName);
			    }

			    fprintf(src, "%s::PermittedAlphabet(int &sizePermittedAlpha)const\n", className);
			    fprintf (src, "{\n");

			
				fprintf(src, "\n");
				fprintf(src, "	static const char alpha[] = {%s};\n", cAlpha);
				fprintf(src, "	sizePermittedAlpha = %d;\n", iAlphasize);
                fprintf(src, "	return alpha;\n");
                fprintf (src, "}\n\n\n");
			}
            

			fprintf (hdr, "		%s& operator=(const char* str)\n", className);
			fprintf (hdr, "			  { %s::operator=(str); return *this;}\n",e->type->cxxTypeRefInfo->className);
			fprintf (hdr, "		%s& operator=(const std::string& str)\n",className );
			fprintf (hdr, "			  { %s::operator=(str); return *this;}\n",e->type->cxxTypeRefInfo->className);
    		fprintf (hdr, "  };\n\n");
    
        if(i!= 3 && i != 0)
        {

		    /* Last we declare an instance of the internal class we */
		    /*   just created                                       */
		    if(i==0)
		    {		
			    fprintf (hdr, "typedef ");
		    }/*if(i==0)*/
 
		    fprintf (hdr, "  %s		", className);

		    if (e->type->cxxTypeRefInfo->isPtr)
		    {
			    fprintf (hdr, "*");
		    }/*if (e->type->cxxTypeRefInfo->isPtr)*/

		    fprintf (hdr, "%s;\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (hdr, "\n\n");
        }
        free(e->type->cxxTypeRefInfo->className);
        e->type->cxxTypeRefInfo->className = strdup(className);
 		returnvalue=1;

			break;
		}/*	case BASICTYPE_NUMERIC_STR:   *\
		  * case BASICTYPE_PRINTABLE_STR: *
		  *	case BASICTYPE_BMP_STR:       *
		  *	case BASICTYPE_VISIBLE_STR:   *
		  *	case BASICTYPE_UNIVERSAL_STR: *
		  *	case BASICTYPE_IA5_STR:       * 
		 \*                               */                 
	
    case BASICTYPE_BITSTRING:
		{
			Subtype* s_type;
			Subtype* currS_type=NULL;
			Subtype* andS_type=NULL;
            Subtype* fakeList = NULL;
			AsnListNode* curr;
			AsnListNode* and_curr;
			int iSCPresent = 0;
			int count = 0;
			int iSCLowerBound = 0;
			int iSCUpperBound = 0;
			SizeConstraint scList[15];
		

	        /*print the generic header for the generated class*/
	        fprintf (hdr, "class %s %s: public %s  {\n", pszGlobalExport, className, e->type->cxxTypeRefInfo->className);
	        s_type = e->type->subtypes;

			if(s_type->choiceId==SUBTYPE_AND)
			{
				and_curr=s_type->a.and->first;
				while(and_curr)
				{	
					andS_type=and_curr->data;
                    if(andS_type->choiceId == SUBTYPE_OR)
			        {
				        curr=andS_type->a.or->first;
				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		        }	
                    else if(andS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(andS_type, fakeList->a.or);
                        curr = fakeList->a.or->first;

				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
                    }
					and_curr=and_curr->next;
				}
			}
			else if(s_type->choiceId == SUBTYPE_OR)
			{
				curr=s_type->a.or->first;
				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		}	
            else if(s_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(s_type, fakeList->a.or);
                curr = fakeList->a.or->first;

				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
            }
	
			fprintf(hdr, "     public:\n");
			fprintf(hdr, "		%s (const char *stringForm=NULL):AsnBits(stringForm){ }\n", className);
			fprintf(hdr, "		%s (size_t numBits):AsnBits(numBits) { }\n", className);
			fprintf(hdr, "		%s (const unsigned char *bitOcts, size_t numBits):AsnBits(bitOcts, numBits){ }\n", className);
			fprintf(hdr, "		%s (const %s &_b):AsnBits(_b){ }\n", className, className);
     
			fprintf (hdr, "		const SizeConstraint* SizeConstraints(int &sizeList)const;\n\n");
			fprintf (src, "const SizeConstraint*  \n");
			
			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s::SizeConstraints(int &sizeList)const\n", className);
			fprintf (src, "{\n");

			if(iSCPresent > 0 )
			{
				count = 0;
				if(scList[count].upperBoundExists == 1 &&
					scList[count].upperBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].upperBound;
				}

				if(scList[count].upperBoundExists == 0 &&
					scList[count].lowerBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].lowerBound;
				}

				if(scList[count].lowerBound < iSCLowerBound)
				{
					iSCLowerBound = scList[count].lowerBound;
				}

				while(count < iSCPresent)
				{
					if(scList[count].upperBoundExists == 2)
					{
						iSCPresent = 0;
					}
										
					count++;
				}  
			}
			
			if(iSCPresent > 0 )
			{
				fprintf(src, "	static const SizeConstraint %s_SizeConstraintList[] = \n			{",
					e->type->cxxTypeRefInfo->fieldName);
								
				for(count = 0; count < iSCPresent; count++)
				{
					fprintf(src, "{ %ld, %ld, %d }", 
						scList[count].lowerBound,
						scList[count].upperBound,
						scList[count].upperBoundExists);
					if(count + 1 < iSCPresent)
					{
						fprintf(src, ",\n			");
					}
				}

				fprintf(src, "};\n\n");
				fprintf(src, "	sizeList = %d;\n", iSCPresent);
                fprintf(src, "	return &%s_SizeConstraintList[0];\n", e->type->cxxTypeRefInfo->fieldName);
			}
			else
			{
				fprintf(src, "	sizeList = 0;\n");
                fprintf(src, "	return NULL;\n");
			}
		
    		fprintf (src, "}\n\n\n");
			fprintf (hdr, "		%s & operator=(const char *stringForm){ SetEqual(stringForm); return *this; }\n", className);
    		fprintf (hdr, "  };\n\n");
    

        if (i != 3 && i != 0)
        {
		    /* Last we declare an instance of the internal class we */
		    /*   just created                                       */
		    if(i==0)
		    {		
			    fprintf (hdr, "typedef ");
		    }/*if(i==0)*/
 
		    fprintf (hdr, "  %s		", className);

		    if (e->type->cxxTypeRefInfo->isPtr)
		    {
			    fprintf (hdr, "*");
		    }/*if (e->type->cxxTypeRefInfo->isPtr)*/

		    fprintf (hdr, "%s;\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (hdr, "\n\n");
        }

        free(e->type->cxxTypeRefInfo->className);
        e->type->cxxTypeRefInfo->className = strdup(className);
		returnvalue=1;

		break;
	}
	 /* case BASICTYPE_BITSTRING:	 */

	case BASICTYPE_OCTETSTRING:
    {
			Subtype* s_type;
			Subtype* currS_type=NULL;
			Subtype* andS_type=NULL;
            Subtype* fakeList = NULL;
			AsnListNode* curr;
			AsnListNode* and_curr;
			int iSCPresent = 0;
			int count = 0;
			int iSCLowerBound = 0;
			int iSCUpperBound = 0;
			SizeConstraint scList[15];
		

	        /*print the generic header for the generated class*/

	        fprintf (hdr, "class %s %s: public %s  {\n", pszGlobalExport, className, e->type->cxxTypeRefInfo->className);

		    s_type = e->type->subtypes;

			if(s_type->choiceId==SUBTYPE_AND)
			{
				and_curr=s_type->a.and->first;
				while(and_curr)
				{	
					andS_type=and_curr->data;
                    if(andS_type->choiceId == SUBTYPE_OR)
			        {
				        curr=andS_type->a.or->first;
				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		        }	
                    else if(andS_type->choiceId == SUBTYPE_SINGLE)
                    {
                        fakeList = MT(Subtype);
                        fakeList->choiceId = SUBTYPE_OR;
			            APPEND(andS_type, fakeList->a.or);
                        curr = fakeList->a.or->first;

				        iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
                    }
					and_curr=and_curr->next;
				}
			}
			else if(s_type->choiceId == SUBTYPE_OR)
			{
				curr=s_type->a.or->first;
				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
    		}	
            else if(s_type->choiceId == SUBTYPE_SINGLE)
            {
                fakeList = MT(Subtype);
                fakeList->choiceId = SUBTYPE_OR;
			    APPEND(s_type, fakeList->a.or);
                curr = fakeList->a.or->first;

				iSCPresent = BasicTypeString_LISTS(hdr, src, scList, iSCPresent, curr, currS_type);
            }
	
			fprintf (hdr, "		public:\n");

			fprintf (hdr, "			%s ():AsnOcts() { }\n", className);
			fprintf (hdr, "			%s (const char *str):AsnOcts(str) { }\n", className);
			fprintf (hdr, "			%s (const char *str, const size_t len):AsnOcts(str, len) { }\n", className);
			fprintf (hdr, "			%s (const %s &o):AsnOcts(o){ }\n", className, className);

			fprintf (hdr, "			const SizeConstraint* SizeConstraints(int &sizeList)const;\n\n");
			fprintf (src, "const SizeConstraint*  \n");
			
			if(i == 1 || i == 3)
			{
				fprintf(src, "%s::", definedName);
			}

			fprintf(src, "%s::SizeConstraints(int &sizeList)const\n", className);
			fprintf (src, "{\n");

			if(iSCPresent > 0 )
			{
				count = 0;
				if(scList[count].upperBoundExists == 1 &&
					scList[count].upperBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].upperBound;
				}

				if(scList[count].upperBoundExists == 0 &&
					scList[count].lowerBound > iSCUpperBound)
				{
					iSCUpperBound = scList[count].lowerBound;
				}

				if(scList[count].lowerBound < iSCLowerBound)
				{
					iSCLowerBound = scList[count].lowerBound;
				}

				while(count < iSCPresent)
				{
					if(scList[count].upperBoundExists == 2)
					{
						iSCPresent = 0;
					}
										
					count++;
				}  
			}
			
			if(iSCPresent > 0 )
			{
				fprintf(src, "	static const SizeConstraint %s_SizeConstraintList[] = \n			{",	e->type->cxxTypeRefInfo->fieldName);
								
				for(count = 0; count < iSCPresent; count++)
				{
					fprintf(src, "{ %ld, %ld, %d }", 
						scList[count].lowerBound,
						scList[count].upperBound,
						scList[count].upperBoundExists);
					if(count + 1 < iSCPresent)
					{
						fprintf(src, ",\n			");
					}
				}

				fprintf(src, "};\n\n");
				fprintf(src, "	sizeList = %d;\n", iSCPresent);
                fprintf(src, "	return &%s_SizeConstraintList[0];\n", e->type->cxxTypeRefInfo->fieldName);
			}
			else
			{
				fprintf(src, "	sizeList = 0;\n");
                fprintf(src, "	return NULL;\n");
			}
		
			fprintf (src, "}\n\n\n");

			fprintf (hdr, "			%s		&operator = (const AsnOcts &o)\n", className);
			fprintf	(hdr, "			    { SetEqual(o);  return *this;}\n");
			fprintf (hdr, "			%s		&operator = (const char *str)\n", className);
			fprintf	(hdr, "				{ SetEqual(str); return *this; }\n");
    		fprintf (hdr, "  };\n\n");
    

        if(i != 3 && i != 0)
        {
		    /* Last we declare an instance of the internal class we */
		    /*   just created                                       */
		    if(i==0)
		    {		
			    fprintf (hdr, "typedef ");
		    }/*if(i==0)*/
 
		    fprintf (hdr, "  %s		", className);

		    if (e->type->cxxTypeRefInfo->isPtr)
		    {
			    fprintf (hdr, "*");
		    }/*if (e->type->cxxTypeRefInfo->isPtr)*/

		    fprintf (hdr, "%s;\n", e->type->cxxTypeRefInfo->fieldName);
		    fprintf (hdr, "\n\n");
        }

        free(e->type->cxxTypeRefInfo->className);
        e->type->cxxTypeRefInfo->className = strdup(className);
		returnvalue=1;

		break;
	}/*	case BASICTYPE_OCETSTRING:   */
	 
	default:
	{
        returnvalue = 0;
		break;
	}/*default*/
	
	}/*switch(e->type->basicType->choiceId)*/

    free(className);
   	return returnvalue;
	
}/*int PrintCxxMultiConstraintOrHandler*/

