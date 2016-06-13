/*
 * c_examples/test_lib/test_lib.c
 *
 * uses SBufs for buffers
 *
 * MS 92
 *
 * $Header: /baseline/SNACC/c-examples/test-lib/test-lib.c,v 1.3 2003/12/17 19:05:03 gronej Exp $
 * $Log: test-lib.c,v $
 * Revision 1.3  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.2.2.1  2003/11/05 14:59:00  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.2  2002/10/24 16:24:53  mcphersc
 * Changed to GenBuf usage
 *
 * Revision 1.3.1.1  1997/08/20 23:14:51  povey
 *
 *
 * Revision 1.5  1995/07/24 20:50:34  rj
 * ``#error "..."'' instead of ``#error ...''.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.4  1995/02/18  16:17:44  rj
 * utilize either isinf(3) or finite(3), whatever happens to be present.
 *
 * Revision 1.3  1994/08/31  23:48:45  rj
 * more portable .h file inclusion.
 *
 * Revision 1.2  1994/08/31  08:59:39  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#include <stdio.h>
#ifdef WIN32
#include "windows.h"
#include "float.h"
#endif
#include "asn-incl.h"
#include "sbuf.h"

int TestAsnBuffers();
int TestAsnTag();
int TestAsnLen();
int TestAsnBool();
int TestAsnInt();
int TestAsnReal();
int TestAsnOcts();
int TestAsnBits();
int TestAsnOid();
int TestAsnList();

int bufSize = 256;

int
main()
{
    int isErr = FALSE;

    /* set up the PLUS and MINUS INFINITY globals */
    InitAsnInfinity();

    /* needed for OCTET STRING, BIT STRING and OBJECT IDENTIFIER decoding */
    InitNibbleMem (256, 256);

    if (!TestAsnBuffers())
    {
        fprintf (stdout, "Failed buffer tests, no point in proceeding ... bye!\n");
        return 1;
    }


    if (!TestAsnTag())
    {
        fprintf (stdout, "Failed Tag test.\n" );
        isErr = TRUE;
    }

    if (!TestAsnLen())
    {
        fprintf (stdout, "Failed Length test.\n" );
        isErr = TRUE;
    }

    if (!TestAsnBool())
    {
        fprintf (stdout, "Failed BOOLEAN test.\n" );
        isErr = TRUE;
    }


    if (!TestAsnInt())
    {
        fprintf (stdout, "Failed INTEGER test.\n" );
        isErr = TRUE;
    }

    if (!TestAsnOcts())
    {
        fprintf (stdout, "Failed OCTET STRING test.\n" );
        isErr = TRUE;
    }


    if (!TestAsnBits())
    {
        fprintf (stdout, "Failed BIT STRING test.\n" );
        isErr = TRUE;
    }


    if (!TestAsnOid())
    {
        fprintf (stdout, "Failed OBJECT IDENTIFIER test.\n" );
        isErr = TRUE;
    }


    if (!TestAsnReal())
    {
        fprintf (stdout, "Failed REAL test.\n" );
        isErr = TRUE;
    }



    if (isErr)
    {
        fprintf (stdout, "There are errors in the primitive type encoding/decoding\n" );
        fprintf (stdout, "library for this architecture.  Time for gdb...\n" );
    }
    else
    {
        fprintf (stdout, "The primitive type encoding/decoding library passed simple tests.\n");
        fprintf (stdout, "It should be safe to use...\n" );
    }

    return isErr;
}


/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnBuffers()
{
    int i,j;
    int noErr = TRUE;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
    SBufResetInWriteRvsMode (&b);
	SBuftoGenBuf (&b, &gb);
    /*
     * write whole range of byte (0..255)
     * remember, write works in reverse
     */
    for (i = 0; i < 256; i++)
        BufPutByteRvs (gb,i);

    if (BufWriteError (gb))
    {
        fprintf (stdout, "Error writing to buffer.\n" );
        noErr = FALSE;
    }

    /* read in values & verify */
    BufResetInReadMode (gb);
    for (i = 255; i >= 0; i--)
        if (BufGetByte (gb) != i)
        {
            fprintf (stdout, "Error verifying data written to buffer.\n" );
            noErr = FALSE;
        }

    if (BufReadError (gb))
    {
        fprintf (stdout, "Error reading from buffer.\n" );
        noErr = FALSE;
    }


    /* now make sure errors are detected */
    SBufResetInWriteRvsMode (gb->spare);

    for (i = 0; i < 257; i++) /*  write past end of buffer */
        BufPutByteRvs (gb,0);

    if (!BufWriteError (gb))
    {
        fprintf (stdout, "Buffers failed to report buffer write overflow.\n" );
        noErr = FALSE;
    }


    BufResetInReadMode (gb);
    for (i = 256; i >= 0; i--)  /*  read past end of buffer  */
        BufGetByte (gb);

    if (!BufReadError (gb))
    {
        fprintf (stdout, "Buffers failed to report buffer read overflow.\n" );
        noErr = FALSE;
    }

    return noErr;
}  /* TestAsnBuffers */



/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnTag()
{
    AsnTag aTag1;
    AsnTag aTag2;
    int i, j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;
    BER_CLASS class;
    BER_FORM form;
    BER_UNIV_CODE code;


    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);

    /* encode a true value and verify */
    class = UNIV;
    form = PRIM;
    code = INTEGER_TAG_CODE;
    aTag1 = MAKE_TAG_ID (class, form, code);

    for (i = 0; i < 2; i++)
    {
        SBufResetInWriteRvsMode (gb->spare);
        len1 = BEncTag1 (gb, class, form, code);

        if (BufWriteError (gb))
        {
            noErr = FALSE;
            fprintf (stdout, "Error encoding a Tag.\n" );
        }

        BufResetInReadMode (gb);

        aTag2 = 0;

        /* make sure no decode errors and that it decodes to same tag */
        len2 = 0;
        if ((val = setjmp (env)) == 0)
        {
            aTag2 = BDecTag (gb, &len2, env);
        }
        else
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding a Tag - error number %d\n", val);
        }
        if (noErr && ((aTag2 != aTag1) || (len1 != len2)))
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoded Tag does not match encoded Tag.\n" );
        }
        /* set a new test tag value */
        class = CNTX;
        form = CONS;
        code = 29;
        aTag1 = MAKE_TAG_ID (class, form, code);
    }
    return noErr;
}  /* TestAsnTag */


/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnLen()
{
    AsnLen aLen1;
    AsnLen aLen2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);

    /* encode a true value and verify */
    aLen1 = 99999;
    for (i = 0; i < 2; i++)
    {
        SBufResetInWriteRvsMode (gb->spare);
        len1 = BEncDefLen (gb, aLen1);

        if (BufWriteError (gb))
        {
            noErr = FALSE;
            fprintf (stdout, "Error encoding Length.\n" );
        }

        BufResetInReadMode (gb);

        aLen2 = 0;

        /* make sure no decode errors and that it decodes to true */
        len2 = 0;
        if ((val = setjmp (env)) == 0)
        {
            aLen2 = BDecLen (gb, &len2, env);
        }
        else
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding Length - error number %d\n", val);
        }


        if (noErr && ((aLen2 != aLen1) || (len1 != len2)))
        {
            noErr = FALSE;
            fprintf (stdout, "Error - decoded lenght does not match encoded length\n");
        }
        aLen1 = 2;
    }


    /* test indef len */
    SBufResetInWriteRvsMode (gb->spare);
    len1 = BEncIndefLen (gb);

    if (BufWriteError (gb))
    {
        noErr = FALSE;
        fprintf (stdout, "Error encoding indefinite Length.\n" );
    }

    BufResetInReadMode (gb);

    aLen2 = 0;

    /* make sure no decode errors */
    len2 = 0;
    if ((val = setjmp (env)) == 0)
    {
        aLen2 = BDecLen (gb, &len2, env);
    }
    else
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding Length - error number %d\n", val);
    }


    if (noErr && ((aLen2 != INDEFINITE_LEN) || (len1 != len2)))
    {
        noErr = FALSE;
        fprintf (stdout, "Error - decoded length does not match encoded length\n");
    }

    /* test EOC */
    SBufResetInWriteRvsMode (gb->spare);
    len1 = BEncEoc (gb);

    if (BufWriteError (gb))
    {
        noErr = FALSE;
        fprintf (stdout, "Error encoding indefinite Length.\n" );
    }

    BufResetInReadMode (gb);

    aLen2 = 0;

    /* make sure no decode errors */
    len2 = 0;
    if ((val = setjmp (env)) == 0)
    {
        BDecEoc (gb, &len2, env);
    }
    else
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding Length - error number %d\n", val);
    }


    if (noErr && (len1 != len2))
    {
        noErr = FALSE;
        fprintf (stdout, "Error - decoded EOC length error.\n");
    }

    return noErr;
}  /* TestAsnLen */


/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnBool()
{
    AsnBool aBool1;
    AsnBool aBool2;
    int j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
    SBufResetInWriteRvsMode (&b);
	SBuftoGenBuf(&b, &gb);

    /* encode a true value and verify */
    aBool1 = TRUE;
    len1 = BEncAsnBoolContent (gb, &aBool1);

    if (BufWriteError (gb))
    {
        noErr = FALSE;
        fprintf (stdout, "Error encoding TRUE BOOLEAN value.\n" );
    }

    BufResetInReadMode (gb);

    aBool2 = FALSE; /* set to opposite of expected value */

    /* make sure no decode errors and that it decodes to true */
    len2 = 0;
    if ((val = setjmp (env)) == 0)
    {
        BDecAsnBoolContent (gb, tag, len1, &aBool2, &len2, env);
    }
    else
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding a BOOLEAN - error number %d\n", val);
    }


    if (noErr && ((aBool2 != aBool1) || (len1 != len2)))
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding TRUE BOOLEAN value.\n" );
    }

    /* now encode a false value and verify */
    SBufResetInWriteRvsMode (gb->spare);
    aBool1 = FALSE;

    len1 = BEncAsnBoolContent (gb, &aBool1);
    if (BufWriteError (gb))
    {
        noErr = FALSE;
        fprintf (stdout, "Error encoding FALSE BOOLEAN value.\n" );
    }

    BufResetInReadMode (gb);

    aBool2 = TRUE; /* set to opposite of expected value */

    /* make sure no decode errors and that it decodes to true */
    len2 = 0;
    if ((val = setjmp (env)) == 0)
    {
        BDecAsnBoolContent (gb, tag, len1, &aBool2, &len2, env);
    }
    else
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding a BOOLEAN - error number %d\n", val);
    }


    if (noErr && ((aBool2 != aBool1) || (len1 != len2)))
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding TRUE BOOLEAN value.\n" );
    }

    /* make sure no decode errors and that it decodes to false */

    return noErr;
}  /* TestAsnBool */



/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnInt()
{
    AsnInt a1;
    AsnInt a2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;
    int sign;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);
    /*
     * Encode a range of integers: negative & positive in
     * the 1 to sizeof (AsnInt) range
     */
    sign = 1;
    for (j = 0; j < 2; j++)
    {
        for (i = 0; i < sizeof (AsnInt); i++)
        {
            SBufResetInWriteRvsMode (gb->spare);

            a1 = sign * (17 << (i * 8)); /* 17 is a random choice :) */
            len1 = BEncAsnIntContent (gb, &a1);
            if (BufWriteError (gb))
            {
                noErr = FALSE;
                fprintf (stdout, "Error encoding INTEGER value %d.\n", a1 );
            }

            BufResetInReadMode (gb);

            /* make sure no decode errors and that it decodes to true */
            len2 = 0;
            if ((val = setjmp (env)) == 0)
            {
                BDecAsnIntContent (gb, tag, len1, &a2, &len2, env);
            }
            else
            {
                noErr = FALSE;
                fprintf (stdout, "Error decoding a INTEGER - error number %d\n", val);
            }

            if (noErr && ((a2 != a1) || (len1 != len2)))
            {
                noErr = FALSE;
                fprintf (stdout, "Error decoding INTEGER value %d.\n", a1 );
            }
        }
        sign = -1;
    }

    return noErr;

}  /* TestAsnInt */


/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnOcts()
{
    AsnOcts a1;
    AsnOcts a2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);
    a1.octs = "Hello Gumby";
    a1.octetLen = strlen (a1.octs);

    /*
     * octet string decoder needs to know tag form
     * (snacc always encodes octet strings as primitives)
     */
    tag = MAKE_TAG_ID (UNIV, PRIM, OCTETSTRING_TAG_CODE);

    for (j = 0; j < 2; j++)
    {
        SBufResetInWriteRvsMode (gb->spare);

        len1 = BEncAsnOctsContent (gb, &a1);
        if (BufWriteError (gb))
        {
            noErr = FALSE;
            fprintf (stdout, "Error encoding OCTET STRING value \"%s\".\n", a1.octs );
        }
        BufResetInReadMode (gb);

        /* make sure no decode errors and that it decodes to true */
        len2 = 0;
        if ((val = setjmp (env)) == 0)
        {
            BDecAsnOctsContent (gb, tag, len1, &a2, &len2, env);
        }
        else
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding an OCTET STRING - error number %d\n", val);
        }

        if (noErr && (!AsnOctsEquiv (&a2,&a1) || (len1 != len2)))
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding OCTET STRING value %s.\n", a1.octs );
        }
        a1.octs = ""; /* test empty string */
        a1.octetLen = strlen (a1.octs);
    }

    ResetNibbleMem();
    return noErr;

}  /* TestAsnOcts */



/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnBits()
{
    AsnBits a1;
    AsnBits a2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;
    short bitsToSet[35];

    /*
     * init bitsToSet - old compilers don't support automatic init
     * of aggregate types.
     */
    bitsToSet[0] = 0;
    bitsToSet[1] = 1;
    bitsToSet[2] = 0;
    bitsToSet[3] = 0;
    bitsToSet[4] = 1;
    bitsToSet[5] = 1;
    bitsToSet[6] = 0;
    bitsToSet[7] = 1;
    bitsToSet[8] = 0;
    bitsToSet[9] = 1;
    bitsToSet[10] = 0;
    bitsToSet[11] = 0;
    bitsToSet[12] = 1;
    bitsToSet[13] = 1;
    bitsToSet[14] = 0;
    bitsToSet[15] = 1;
    bitsToSet[16] = 0;
    bitsToSet[17] = 1;
    bitsToSet[18] = 0;
    bitsToSet[19] = 0;
    bitsToSet[20] = 1;
    bitsToSet[21] = 1;
    bitsToSet[22] = 0;
    bitsToSet[23] = 1;
    bitsToSet[24] = 0;
    bitsToSet[25] = 1;
    bitsToSet[26] = 0;
    bitsToSet[27] = 1;
    bitsToSet[28] = 1;
    bitsToSet[29] = 0;
    bitsToSet[30] = 1;
    bitsToSet[31] = 1;
    bitsToSet[32] = 0;
    bitsToSet[33] = 1;
    bitsToSet[34] = 0;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);
    /* initialize bit string */
    a1.bits = Asn1Alloc (5);
    a1.bitLen = 35;
    for (i = 0; i < 35; i++)
    {
        if (bitsToSet[i])
            SetAsnBit (&a1, i);
        else
            ClrAsnBit (&a1, i);
    }

    /*
     * bit string decoder needs to know tag form
     * (snacc always encodes bit strings as primitives)
     */
    tag = MAKE_TAG_ID (UNIV, PRIM, BITSTRING_TAG_CODE);

    SBufResetInWriteRvsMode (gb->spare);

    len1 = BEncAsnBitsContent (gb, &a1);
    if (BufWriteError (gb))
    {
        noErr = FALSE;
        fprintf (stdout, "Error encoding BIT STRING value ");
        PrintAsnBits (stdout, &a1, 0);
        fprintf (stdout, "\n");
    }
    BufResetInReadMode (gb);

    /* make sure no decode errors and that it decodes to true */
    len2 = 0;
    if ((val = setjmp (env)) == 0)
    {
        BDecAsnBitsContent (gb, tag, len1, &a2, &len2, env);
    }
    else
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding an BIT STRING - error number %d\n", val);
    }

    if (noErr && (!AsnBitsEquiv (&a2,&a1) || (len1 != len2)))
    {
        noErr = FALSE;
        fprintf (stdout, "Error decoding BIT STRING value ");
        PrintAsnBits (stdout, &a1, 0);
        fprintf (stdout, "\n");
    }
    ResetNibbleMem();
    return noErr;

}  /* TestAsnBits */


/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnOid()
{
    AsnOid a1;
    AsnOid a2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);
    /*  mib-2 oid  { iso 3 6 1 2 1 }*/
     a1.octetLen = 5;
     a1.octs = "\53\6\1\2\1";


    for (j = 0; j < 2; j++)
    {
        SBufResetInWriteRvsMode (&b);

        len1 = BEncAsnOidContent (gb, &a1);
        if (BufWriteError (gb))
        {
            noErr = FALSE;
            fprintf (stdout, "Error encoding OCTET STRING value \"%s\".\n", a1.octs );
        }
        BufResetInReadMode (gb);

        /* make sure no decode errors and that it decodes to true */
        len2 = 0;
        if ((val = setjmp (env)) == 0)
        {
            BDecAsnOidContent (gb, tag, len1, &a2, &len2, env);
        }
        else
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding an OCTET STRING - error number %d\n", val);
        }

        if (noErr && (!AsnOidsEquiv (&a2,&a1) || (len1 != len2)))
        {
            noErr = FALSE;
            fprintf (stdout, "Error decoding OCTET STRING value %s.\n", a1.octs );
        }
        /*  system  { mib-2 1 }*/
        a1.octs = "\53\6\1\2\1\1";
        a1.octetLen = 6;
    }
    ResetNibbleMem();
    return noErr;

}  /* TestAsnOid */

/*
 * returns TRUE if passes encode/decode tests
 */
int
TestAsnReal()
{
    AsnReal a1[5];
    AsnReal a2;
    int i,j;
    AsnLen len1;
    AsnLen len2;
    AsnTag tag;
    int noErr = TRUE;
    int elmtErr = FALSE;
    ENV_TYPE env;
    SBuf  b;
	GenBuf *gb;
    char bufData[256];
    long int val;
    int sign;
    AsnReal inf;
    unsigned char *c;


    /*
     * if you do not have the ieee_functions in your math lib,
     * this will not link.  Comment it out and cross you fingers.
     * (or check/set the +/-infinity values for you architecture)
     */
#if HAVE_ISINFx
    if (!isinf (PLUS_INFINITY) || !isinf (MINUS_INFINITY))
#else
#if HAVE_FINITEx
    if (finite (PLUS_INFINITY) || finite (MINUS_INFINITY))
#else
	#define YUCK //warning "oops: you've got neither isinf(3) nor finite(3)?!"
#endif
#endif
    {
//        fprintf (stdout, "WARNING: PLUS_INFINITY and MINUS_INFINITY in asn_real.c are not\n");
//        fprintf (stdout, "correct for this architecture.  Modify the InitAsnInfinity() Routine.\n");
    }

    /*
     * init test value array.
     * some old compilers don't support automatic init of aggregate types
     * like:
     * AsnReal a1[] = { 0.0, 0.8, -22.484848, PLUS_INFINITY, MINUS_INFINITY};
     */
    a1[0] = 0.0;
    a1[1] = 0.8;
    a1[2] = -22.484848;
    a1[3] = PLUS_INFINITY;
    a1[4] = MINUS_INFINITY;

    /* initialize buffer */
    SBufInit (&b, bufData, 256);
	SBuftoGenBuf (&b, &gb);

    /*
     * Encode a range of integers: negative & positive in
     * the 1 to sizeof (AsnInt) range
     */
    for (i = 0; i < 5; i++)
    {
        elmtErr = FALSE;
        SBufResetInWriteRvsMode (gb->spare);

        len1 = BEncAsnRealContent (gb, &a1[i]);
        if (BufWriteError (gb))
        {
            elmtErr = TRUE;
            fprintf (stdout, "Error encoding REAL value ");
            PrintAsnReal (stdout,&a1[i],0);
            fprintf (stdout, ".\n");
        }

        BufResetInReadMode (gb);

        /* make sure no decode errors and that it decodes to true */
        len2 = 0;
        if ((val = setjmp (env)) == 0)
        {
            BDecAsnRealContent (gb, tag, len1, &a2, &len2, env);
        }
        else
        {
            elmtErr = TRUE;
            fprintf (stdout, "Error decoding a REAL - error number %d\n", val);
        }

        /* testing reals for equality is sketchy */
        if (!elmtErr && ((a2 != a1[i]) || (len1 != len2)))
        {

            elmtErr = TRUE;
            fprintf (stdout, "Error decoding REAL value ");
            PrintAsnReal (stdout, &a1[i], 0);
            fprintf (stdout, ".\n");

            if (len1 == len2)  /* therefore a2 != a1[i] */
            {
                fprintf (stdout, "The value decoded was ");
                PrintAsnReal (stdout, &a2, 0);
                fprintf (stdout, ".\n");
            }
            else
                fprintf (stdout, "The encoded and decoded length disagree.\n");
        }
        if (elmtErr)
            noErr = FALSE;
    }


    return noErr;

}  /* TestAsnReal */
