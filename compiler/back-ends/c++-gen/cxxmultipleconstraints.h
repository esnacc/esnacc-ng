#ifndef _cxxmultipleconstraints_h_
#define _cxxmultipleconstraints_h_


#include "asn-incl.h"
#include "asn1module.h"
#include "snacc.h"
#include "limits.h"
#include "rules.h"
#include "snacc-util.h"
#include "str-util.h"

typedef struct SizeConstraint{
	long lowerBound;
	long upperBound;
	int  upperBoundExists;
}SizeConstraint;

typedef SizeConstraint ValueRange;

typedef struct PermittedAlphabet{
	unsigned char* ucApha; 
}PermittedAlphabet;

char* PrintCxxPermittedAlphabetHandler PROTO(( AsnListNode* currAlpha, unsigned char* cPermittedAlphabet, int *iAlphasize));

int SizeConstraint_LISTS PROTO((FILE* hdr, FILE* src, SizeConstraint* scList, int iSCPresent, AsnListNode* curr, Subtype* currS_type));

int PrintCxxMultiConstraintOrHandler PROTO((FILE* hdr, FILE* src, char* definedName, NamedType* e, int i));

int ValueRange_LISTS PROTO((FILE* hdr, FILE* src, ValueRange* vrList, int iVRPresent, AsnListNode* curr, Subtype* currS_type));

int BasicTypeString_LISTS PROTO((FILE* hdr, FILE* src, SizeConstraint* scList, int iSCPresent, AsnListNode* curr, Subtype* currS_type));

char* FindPermittedAlpha PROTO((AsnListNode* curr, unsigned char* cPermittedAlphabet, int *iAlphasize));

#endif


