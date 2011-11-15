/*
 * file: .../tbl-tools/pval/pval.c - decodes and prints a BER values
 *      given a type table and the type name of the value (module name
 *	optional).
 *
 * Mike Sample
 *
 * Copyright (C) 1993 Michael Sample
 *            and the University of British Columbia
 *
 * $Header: /baseline/SNACC/tbl-tools/pval/pval.c,v 1.2 2003/12/17 19:05:04 gronej Exp $
 * $Log: pval.c,v $
 * Revision 1.2  2003/12/17 19:05:04  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.1.2.1  2003/11/05 14:59:00  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.1.1.1  2000/08/21 20:35:47  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/02/15 19:15:09  rj
 * first check-in
 *
 */

#include "tbl-incl.h"
#include "sbuf.h"

void Usage PARAMS ((prg),
    char *prg)
{
    fprintf (stderr,"Usage: %s -T <tt file name> [-m <module name>] -n <type name>\n             <ber value file list> \n\n",prg);
    fprintf (stderr,"E.G.  %s -T x400.tt  -m P1 -n MPDU p1msg1.ber p1msg2.ber\n\n",prg);
    fprintf (stderr,"The BER values in the file list will be decoded and printed to stdout.\n");
    fprintf (stderr,"The module name is optional - if left out, the first occurence\nof the named type will be used.\n");
}

char*
NextParamStr PARAMS ((argv, currArg),
    char **argv _AND_
    int *currArg)
{
    char *retVal;

    if (argv[*currArg][2] != '\0')
    {
        /* no space between opt (ie -T) and parameter */
        retVal = &argv[*currArg][2];
        *currArg += 1;
    }
    else
    {
        retVal = argv[*currArg+1];
        *currArg += 2;
    }

    return retVal;
}

int
main PARAMS ((argc, argv),
    int argc _AND_
    char **argv)
{
    char *tblFileName;
    char *typeName;
    char *modName;
    int gotTbl;
    int gotType;
    TBL *tbl;
    int i;
    char *fileData;
    unsigned long int fsize;
    AVal *val;
    unsigned long int bytesDecoded;
    int currArg;
    char **fileArr;
    int numValFiles;
    SBuf sb;
    GenBuf gb;

    if (argc <= 1)
    {
        Usage (argv[0]);
        return 1;
    }

    fileArr = (char **) malloc ((argc -1) * sizeof (char *));

    /*
     * parse cmd line args
     */
    numValFiles = 0;
    gotType = gotTbl = FALSE;
    modName = NULL; /* optional param - present if non-null after cmd parse */
    for (currArg = 1; (currArg < argc); )
    {
        if ((argv[currArg][0] == '-') && (argv[currArg][1] != '\0'))
            switch (argv[currArg][1])
            {
                case 'T':
                    gotTbl = TRUE;
                    tblFileName = NextParamStr (argv, &currArg);
                break;

                case 'm':
                    modName = NextParamStr (argv, &currArg);
                break;

                case 'n':
                    gotType = TRUE;
                    typeName = NextParamStr (argv, &currArg);
                break;

                default:
                    fprintf (stderr,"%s: ERROR - unknown cmd line option \"%s\"\n\n", argv[0], argv[currArg]);
                    Usage (argv[0]);
                    return 1;
            }

        else /* assume it's the name of a BER file */
            fileArr[numValFiles++] = argv[currArg++];

    }

    /* check sanity of cmd line args */
    if (numValFiles == 0)
    {
        fprintf (stderr,"ERROR - no BER files were given.\n");
        Usage (argv[0]);
        return 1;
    }

    if (!gotType)
    {
        fprintf (stderr,"ERROR - you must specify a type name.\n");
        Usage (argv[0]);
        return 1;
    }

    if (!gotTbl)
    {
        fprintf (stderr,"ERROR - you must specify a type table file.\n");
        Usage (argv[0]);
        return 1;
    }


    /* start decoding and printing the given files */
    InitNibbleMem (1024,1024);

    tbl = LoadTblFile (tblFileName);
    if (tbl == NULL)
        return 1;

    for (i = 0; i < numValFiles; i++)
    {
        fileData = LoadFile (fileArr[i], &fsize);
        if (fileData == NULL)
            return 1;

        SBufInstallData (&sb, fileData, fsize);
        PutSBufInGenBuf (&sb, &gb);

        fprintf (stdout, "\n\n-- Contents of file \"%s\"--\n", fileArr[i]);
        fprintf (stdout, "-- module = %s, type = %s --\n\n", ((modName)? (modName):("???")), typeName);

        val = TblDecode (tbl, modName, typeName, &gb, &bytesDecoded);

        if (val == NULL)
            fprintf (stdout,"-- Decoding error occured somewhere -- \n");
        else
            TblPrintValue (tbl, modName, typeName, stdout, val);

        fprintf (stdout, "\n\n -- decoded %d bytes for the above value --\n\n", bytesDecoded, fileArr[i]);

        free (fileData); /* was malloc'd in LoadFile (holds BER data) */
    }

    return 0;
} /* main */
