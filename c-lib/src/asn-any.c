/*
 * asn_any.c - BER encode, decode, print, free, type set up and installation
 *             routines for the ASN.1 ANY and ANY DEFINED BY types.
 *
 * MS 92
 * Copyright (C) 1992 Michael Sample and the University of British Columbia
 *
 * This library is free software; you can redistribute it and/or
 * modify it provided that this copyright/license information is retained
 * in original form.
 *
 * If you modify this file, you must clearly indicate your changes.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Header: /baseline/SNACC/c-lib/src/asn-any.c,v 1.11 2004/01/22 20:03:12 nicholar Exp $
 */

#include "asn-config.h"
#include "asn-len.h"
#include "asn-tag.h"
#include "asn-oid.h"
#include "asn-int.h"
#include "asn-any.h"
#include <memory.h>

AsnLen BEncUnknownAsnAny(GenBuf *b,void *value);
void BDecUnknownAsnAny(GenBuf *b,void *value, AsnLen *bytesDecoded, ENV_TYPE env);

/*
 * 2 hash tables. 1 for INTEGER to type mappings the other
 * for OBJECT IDENTIFER to type mappings.
 */
Table *anyOidHashTblG = NULL;
Table *anyIntHashTblG = NULL;

/*
 * given an ANY type value and a integer hash key, this defines
 * this any values type (gets ptr to hash tbl entry from int key).
 * The hash table entry contains ptrs to the encode/decode etc. routines.
 */
void
SetAnyTypeByInt PARAMS ((v, id),
    AsnAny *v _AND_
    AsnInt id)
{
    Hash hash;
    void *anyInfo;

    /* use int as hash string */
    hash = MakeHash ((char*)&id, sizeof (id));
    if (CheckForAndReturnValue (anyIntHashTblG, hash, &anyInfo))
        v->ai = (AnyInfo*) anyInfo;
    else
        v->ai = NULL; /* indicates failure */

	
} /* SetAnyTypeByInt */

/* -------- ========== DAD =========== -------------- */
/* I added the following two routines for dealing with
 * the decode/encode of asn any's 
 */
AsnLen BEncUnknownAsnAny(GenBuf *b,void *value)
{
    AsnOcts	*data;	/* where we will store the stuff */
	/* since the unknown type was just block copied into a oct's struct, we
	 * only need copy it out.... you will need to change this if
	 * you decide to stuff unknown any types for encoding in a
	 * different way.
	 */
	data = (AsnOcts *) value;
	BufPutSegRvs(b, data->octs, data->octetLen);
    return(data->octetLen);

}

void BDecUnknownAsnAny(GenBuf *b,void *value, AsnLen *bytesDecoded, ENV_TYPE env)
{
	int err = 0;

	/* we are going to try unknown items as blobs - we will
	 * peek the tag and length, then record the whole thing
	 * including the tag and length along with it's data
	 * into a buffer which we will have to allocate.
	 */

	/* need to record the current read location in the buffer so
	 * that we can copy out the tag+len+data later on.
	 */
#ifdef OLD_CODE
#ifdef USE_EXP_BUF
	loc = (**b).curr;	/* for XBUF */
#endif
#endif

	// Use the Memory Buffer Copy Any function
	err = GenBufCopyAny (b, value, bytesDecoded, env);
	if (err != 0)
		longjmp (env, -900);

#ifdef OLD_CODE
	
	tagId1 = BDecTag(b, &totalElmtsLen1, env);			/* item tag */
	elmtLen1 = BDecLen (b, &totalElmtsLen1, env);		/* len of item */
	if (elmtLen1 == INDEFINITE_LEN)
	{
		/* can't deal with indef len unknown types here (at least for now) */
		Asn1Error("BDecUnknownAsnAny: ERROR - indef length object found\n");
		longjmp(env, -1600);
	}
	
	/* and now decode the contents */
	data = (AsnOcts *) value;	/* allocated by the any routine */
	data->octetLen = elmtLen1 + totalElmtsLen1;	/* tag+len+data lengths */
	data->octs = Asn1Alloc(data->octetLen +1);
    CheckAsn1Alloc (data->octs, env);
	/* b was inc'd by our reading the tag - need to mem copy the data
	 * ourselves.
	 */
	memcpy(data->octs, loc, totalElmtsLen1);
	/* use normal buffer reading to append the rest */
    BufCopy(&data->octs[totalElmtsLen1] , b, elmtLen1);

    if (BufReadError(b))
    {
        Asn1Error("BDecUnknownAsnAny: ERROR - decoded past end of data\n");
        longjmp(env, -920);
    }

    /* add null terminator - this is not included in the str's len */
    data->octs[data->octetLen] = '\0';
    (*bytesDecoded) += data->octetLen;
	/* (*bytesDecoded) += totalElmtsLen1; just use the total blob length */
#endif
}
/* -------- ========== DAD =========== -------------- */

/*
 * Same as SetAnyTypeByInt except that the hash key is an OBJECT IDENTIFER.
 */
void SetAnyTypeByOid PARAMS ((v, id),
    AsnAny *v _AND_
    AsnOid *id)
{
    Hash hash;
    void *anyInfo;

    /* use encoded oid as hash string */
    hash = MakeHash (id->octs, id->octetLen);
    if (CheckForAndReturnValue (anyOidHashTblG, hash, &anyInfo))
        v->ai = (AnyInfo*) anyInfo;
    else
        v->ai = NULL; /* indicates failure */

	/* DAD - 1/21/98
	 * Modification to deal with Unknown ANY's - we wish to
	 * leave them in an octet chunk
	 */
	if(v->ai == NULL)	/* no table entry */
	{
		v->ai = (AnyInfo*) Asn1Alloc (sizeof(AnyInfo));

/*		v->ai = (AnyInfo*) malloc(sizeof(AnyInfo)); */ /* fix2 */
		/* CheckAsn1Alloc((v->ai), env); fix */
	 /*   v->ai->anyId = (int) hash;	 need id numbers - use hash for now*/
	 
/* mod for CM v1.2, we will place an unknown any constant into the
 * ID field so that our higher level code can detect an unknown any based
 * upon this id rather than assuming that it must scan a list of known
 * oids...
 */
 		v->ai->anyId = (int) kUnknownAnyObjectID;	 
	    v->ai->oid.octs = id->octs;
	    v->ai->oid.octetLen = id->octetLen;
	    v->ai->size = sizeof(AsnOcts); /* store in octs struct, but diff coders */
	    v->ai->Encode = (EncodeFcn)BEncUnknownAsnAny;
	    v->ai->Decode = (DecodeFcn)BDecUnknownAsnAny;
	    v->ai->Free = (FreeFcn)FreeAsnOcts;

#ifdef _USE_PRINTING_
	    v->ai->Print = (PrintFcn)PrintAsnOcts;
#else
		v->ai->Print = (PrintFcn)NULL;
#endif	/* _USE_PRINTING_ */


/* Additional mod - for CM version 1.2 we will no longer place the Unknown
 * any handler into the table, intead catching the failed lookup each
 * time for a particular instance.
 */
#ifdef _OLDER_LIB_

	    /* after this is called with an oid we didn't have, from now on
	     * this generic handler will be found for all further calls with
	     * the given oid.
	     */
		if (anyOidHashTblG == NULL)
			anyOidHashTblG = InitHash();

		if(anyOidHashTblG != NULL)	/* make sure we didn't fail */
	    	Insert(anyOidHashTblG, v->ai, hash);
	    	
#endif /* _OLDER_LIB_ */	    	
	}
	
} /* SetAnyTypeByOid */

/*
 *RWC;ADDED this new "C" function to allow generic ANY's to be encoded/decoded.
 */
void SetAnyTypeUnknown PARAMS ((v),
    AsnAny *v)
{
	if(v->ai == NULL)	/* no table entry */
	{
		v->ai = (AnyInfo*) Asn1Alloc (sizeof(AnyInfo));

 		v->ai->anyId = (int) kUnknownAnyObjectID;	 
	    /*RWC;v->ai->oid.octs = id->octs;
	    v->ai->oid.octetLen = id->octetLen;*RWC;*/
	    v->ai->size = sizeof(AsnOcts); /* store in octs struct, but diff coders */
	    v->ai->Encode = (EncodeFcn)BEncUnknownAsnAny;
	    v->ai->Decode = (DecodeFcn)BDecUnknownAsnAny;
	    v->ai->Free = (FreeFcn)FreeAsnOcts;

#ifdef _USE_PRINTING_
	    v->ai->Print = (PrintFcn)PrintAsnOcts;
#else
		v->ai->Print = (PrintFcn)NULL;
#endif	/* _USE_PRINTING_ */

	}
	
} /* SetAnyTypeByUnknown */


/*
 * Creates an entry in the hash table that contains the
 * type's size, encode, decode, free, and print routines and anyId.
 * The given intId is used as the hash key so future calls to
 * SetAnyTypeByInt with that intId as the id will reference this entry.
 * The anyId is stored in the hash tbl entry as well so the user can
 * figure out the type with a simple integer comparison.
 *
 * This routine is usually called from the AnyInit routine that
 * the compiler generates from MACRO info.  Call this routine
 * once for each possible ANY type to set up the hash table.
 * Future calls to SetAnyTypeByInt/Oid will reference this table.
 */
void
InstallAnyByInt PARAMS ((anyId, intId, size, Encode, Decode, Free, Print),
    int anyId _AND_
    AsnInt intId _AND_
    unsigned int size _AND_
    EncodeFcn Encode _AND_
    DecodeFcn Decode _AND_
    FreeFcn Free _AND_
    PrintFcn Print)
{
    AnyInfo *a;
    Hash h;

    a = (AnyInfo*) malloc (sizeof (AnyInfo));
    a->anyId = anyId;
    a->oid.octs = NULL;
    a->oid.octetLen = 0;
    a->intId = intId;
    a->size = size;
    a->Encode = Encode;
    a->Decode = Decode;
    a->Free = Free;
    a->Print = Print;

    if (anyIntHashTblG == NULL)
        anyIntHashTblG = InitHash();

    h = MakeHash ((char*)&intId, sizeof (intId));
	if(anyIntHashTblG != NULL)	/* make sure we didn't fail */
    	Insert(anyIntHashTblG, a, h);

}  /* InstallAnyByOid */


/*
 * Same as InstallAnyByInt except the oid is used as the hash key
 */
void
InstallAnyByOid PARAMS ((anyId, oid, size, Encode, Decode, Free, Print),
    int anyId _AND_
    AsnOid *oid _AND_
    unsigned int size _AND_
    EncodeFcn Encode _AND_
    DecodeFcn Decode _AND_
    FreeFcn Free _AND_
    PrintFcn Print)
{
    AnyInfo *a;
    Hash h;

    a = (AnyInfo*) malloc (sizeof (AnyInfo));
    a->anyId = anyId;
    a->oid.octs = oid->octs;
    a->oid.octetLen = oid->octetLen;
    a->size = size;
    a->Encode = Encode;
    a->Decode = Decode;
    a->Free = Free;
    a->Print = Print;

    h = MakeHash (oid->octs, oid->octetLen);

    if (anyOidHashTblG == NULL)
        anyOidHashTblG = InitHash();

	if(anyOidHashTblG != NULL)	/* make sure we didn't fail */
   		Insert(anyOidHashTblG, a, h);

}  /* InstallAnyByOid */


/*
 * Calls the free routine in this type's any info.
 * If the routine ptr is NULL, nothing is done
 * (This is the case for INTEGERs, BOOLEANs and other simple
 * values)
 */
void
FreeAsnAny PARAMS ((v),
    AsnAny *v)
{
    if ((v->ai != NULL) && (v->ai->Free != NULL)) {
        v->ai->Free (v->value);
    } else {
        Asn1Free(v->value);
    }

    if (v->ai && v->ai->anyId == kUnknownAnyObjectID) {
        Asn1Free(v->ai);
    }
} /* FreeAsnAny */


/*
 * Calls the Encode routine pointed to in the given type's
 * Any Info.  If the routine ptr is NULL nothing is encoded
 * (This should set some type of error).
 * Note: this calls the BEncFoo not BEncFooContent routine form
 * since the tags are needed too.
 */
AsnLen
BEncAsnAny PARAMS ((b, v),
    GenBuf *b _AND_
    AsnAny *v)
{
    if ((v->ai != NULL) && (v->ai->Encode != NULL))
        return v->ai->Encode (b, v->value);
    else
        return 0;
} /* BEncAsnAny */


/*
 * Calls the Decode routine pointed to in the given type's
 * Any Info.  If the routine ptr is NULL any error is flagged.
 * Note: this calls the BDecFoo not BDecFooContent routine form
 * since the tags are needed too.
 */
void BDecAsnAny PARAMS ((b, result, bytesDecoded, env),
    GenBuf *b _AND_
    AsnAny  *result _AND_
    AsnLen *bytesDecoded _AND_
    ENV_TYPE env)
{
    if ((result->ai != NULL) && (result->ai->Decode != NULL))
    {
        result->value = (void*) Asn1Alloc (result->ai->size);
        CheckAsn1Alloc (result->value, env);
        result->ai->Decode (b, result->value, bytesDecoded, env);
    }
    else
    {
        Asn1Error ("ERROR - ANY Decode routine is NULL\n");
        longjmp (env, -44);
    }
}

/*
 * Calls the print routine pointed to from the given type's
 * Any Info.  Prints an error if the type does not have
 * any 'AnyInfo' or if the AnyInfo has a NULL Print routine ptr.
 */
void PrintAsnAny PARAMS ((f, v, indent),
    FILE *f _AND_
    AsnAny *v _AND_
	unsigned int indent)
{
    if ((v->ai != NULL) && (v->ai->Print != NULL))
        v->ai->Print (f, v->value, indent);
    else
        fprintf (f," -- ERROR: malformed ANY value --");
}
