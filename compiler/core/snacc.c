/*
 * compiler/core/snacc.c---Compiles ASN.1 src files into an internal type tree.
 *        Imported type/value references are resolved if possible.
 *        Produces C or C++ encoder/decoder/print/free code and .h for
 *        data struct and prototypes.
 *        Generated C can be either ANSI or old style via macros.
 *        Produces values for OBJECT IDENTIFIERs, INTEGERs and BOOLEANs
 *
 * Copyright (C) 1991, 1992 Michael Sample
 *            and the University of British Columbia
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program and the associated libraries are distributed in the hope
 * that they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License and GNU Library General
 * Public License for more details.
 *
 * $Header: /baseline/SNACC/compiler/core/snacc.c,v 1.54 2004/04/06 15:13:41 gronej Exp $
 *
 */

int gNO_NAMESPACE=0;
const char *gAlternateNamespaceString=0;

/*  DEFAULT no DLL Export of SNACC  built classes.
 */
char *bVDAGlobalDLLExport=(char *)0;

#ifdef WIN32
#pragma  warning( disable : 4706 4115)  /* IGNORE assign w/in conditional expression. */
#endif

#ifdef WIN32
#include <ctype.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <stdio.h>

#include "asn-incl.h"
	#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif


#include "version.h"
#include "mem.h"
#include "asn1module.h"
#include "exports.h"
#include "enc-rules.h"
#include "print.h"
#include "define.h"
#include "snacc-util.h"
#include "str-util.h"
#if META
#include "meta.h"
#endif

#include "c-gen/rules.h"		/* for c file generation */
#include "c-gen/type-info.h"

#include "c++-gen/rules.h"		/* for c++ file generation */

#if IDL
#include "idl-gen/rules.h"		/* for idl file generation */
#endif

typedef struct SRC_FILE
{
    char      *fileName;
	short	ImportFileFlag;
	struct SRC_FILE *next;
} SRC_FILE;



/* ******************* */
/* Function Prototypes */
/* ******************* */

/* Generic File System Interface (GFSI) */
typedef void * GFSI_HANDLE;
char *GFSI_GetFirstFile(GFSI_HANDLE *gfsi_handle, const char *path,
						const char *extension);
char *GFSI_GetNextFile( GFSI_HANDLE *gfsi_handle, const char *extension);
void GFSI_Close( GFSI_HANDLE *gfsi_handle );

void ErrChkModule PROTO ((Module *m));
void FillCxxTypeInfo PROTO ((CxxRules *r, ModuleList *m));
void FillIDLTypeInfo PROTO ((IDLRules *r, ModuleList *modList));
void GenTypeTbls PROTO ((ModuleList *mods, char *fileName, int tableFileVersion));
int  InitAsn1Parser PROTO ((Module *mod, char *fileName, FILE *fPtr));
int  LinkTypeRefs PROTO ((ModuleList *m));
int  LinkValueRefs PROTO ((ModuleList *m));
void MarkRecursiveTypes PROTO ((Module *m));
void NormalizeModule PROTO ((Module *m));
void NormalizeValue PROTO ((Module *m, ValueDef *vd, Value *v, int quiet));
int	 ParseValues PROTO ((ModuleList *mods, Module *m));
void PrintCCode PROTO ((FILE *src, FILE *hdr, ModuleList *mods, Module *m,
					   CRules *r, long int longJmpVal, int printTypes,
					   int printValues, int printEncoders, int printDecoders,
					   int printPrinters, int printFree));
void PrintCxxCode (FILE *src, FILE *hdr, if_META (MetaNameStyle genMeta COMMA
				   const Meta *meta COMMA MetaPDU *metapdus COMMA)
				   ModuleList *mods, Module *m, CxxRules *r, long longJmpVal,
				   int printTypes, int printValues, int printEncoders,
				   int printDecoders, int printPrinters, int printFree,
				   if_TCL (int printTcl COMMA) int novolatilefuncs);
void PrintIDLCode PROTO ((FILE *idl, ModuleList *mods, Module *m, IDLRules *r,
						 long int longJmpVal, int printValues));
void ProcessMacros PROTO ((Module *m));
void SortAllDependencies PROTO ((ModuleList *m));
int  yyparse();

/* Internal routines */
static short Add2SrcList(SRC_FILE **FileList, const char *InputFile,
						 short ImportFlag);
static int GenCCode PROTO ((ModuleList *allMods, long longJmpVal, int genTypes,
						   int genEncoders, int genDecoders, int genPrinters,
						   int genValues, int genFree));
static void GenCxxCode PROTO ((ModuleList *allMods, long longJmpVal,
							 int genTypes, int genEncoders, int genDecoders,
							 int genPrinters, int genValues, int genFree,
							 if_META (MetaNameStyle genMeta COMMA MetaPDU *meta_pdus COMMA)
							 if_TCL (int genTcl COMMA) int novolatilefuncs));
static void GenIDLCode PROTO ((ModuleList *allMods, long longJmpVal, int genTypes,
							 int genPrinters, int genValues, int genFree));
static int ModNamesUnique PROTO ((ModuleList *m));
static Module *ParseAsn1File PROTO ((char *fileName, short ImportFlag));
static short compareDupeFile(char *fullpath, SRC_FILE *FileList);
static char *sbasename(char *name);

#if META
static MetaPDU *parse_type_list PROTO (char *arg));
#endif /* META */



/* **************** */
/* Global Variables */
/* **************** */
extern int anyEnumValG;

extern int	smallErrG;		/* can continue processing but don't produce code - see more errs */
/*RWC;extern int	yydebug; / * set to 1 to enable debugging */

int	maxFileNameLenG = -1;	/* values > 2 are considered valid */
							/* this is used in back_ends/c_gen/str_util.c */

static const char versionG[] = VERSION;
static const char releasedateG[] = RELDATE;
static const char bugreportaddressG[] = BUGREPADDR;

FILE* errFileG = NULL;		/* Pointer to file for reporting errors */

int genPERCode = FALSE;

int isSyntax1997 = 1;		/* Deepak: 24/Mar/2003	Syntax Checking for 1990/1997,
								defaults to 1997 */
int isTableConstraintAllowed = 1;	/* Deepak: 25/Mar/2003 */
int isInWithSyntax = 0;				/* Deepak: 26/Mar/2003 */


#ifdef WIN_SNACC					/* Deepak: 14/Feb/2003 */
	#define main Win_Snacc_Main
#endif



void Usage PARAMS ((prgName, fp),
    char	*prgName _AND_
    FILE	*fp)
{
    fprintf (fp, "\nUsage: %s ", prgName);
    fprintf (fp, "[-h] [-P] [-t] [-v] [-e] [-d] [-p] [-f] [-y] [-M] [-B] \n");
#if IDL
    fprintf (fp, "            [-c | -C | -T <table output file> | -idl ]\n");
#else
    fprintf (fp, "            [-c | -C | -T <table output file>]\n");
#endif
	fprintf (fp, "            [-I <include directory>]\n");
    fprintf (fp, "            [-mm] [-mf <max file name length>]\n");
    fprintf (fp, "            [-l <neg number>]\n");
    fprintf (fp, "            [-VDAexport=DEFINE_NAME] to designate export of SNACC generated classes\n");
    fprintf (fp, "            [-E BER|DER select encoding rules to generate (C only)]\n");
	fprintf (fp, "            [-a <start number>] select starting number for ANYs\n");

#if META
    fprintf (fp, "            [-meta <type list>] [-mA | -mC]\n");
#if TCL
    fprintf (fp, "            [-tcl <type list>]\n");
#endif
#endif
    fprintf (fp, "            <ASN.1 file list>\n\n");
    fprintf (fp, "  -c   generate C encoders and decoders (default)\n");
    fprintf (fp, "  -C   generate C++ encoders and decoders\n");
    fprintf (fp, "  -T <filename> write a type table file for the ASN.1 modules to file filename\n");
    fprintf (fp, "  -O <filename> writes the type table file in the original (<1.3b2) format\n");
    fprintf (fp, "  -B   turns off generation of constrained PER classes\n");
#if IDL
    fprintf (fp, "  -idl generate CORBA IDL\n");
#endif
    fprintf (fp, "  -h   prints this msg\n");
    fprintf (fp, "  -P   print the parsed ASN.1 modules to stdout from their parse trees\n");
    fprintf (fp, "       (helpful debugging)\n");
    fprintf (fp, "  -t   generate type definitions\n");
    fprintf (fp, "  -v   generate value definitions (limited)\n");
    fprintf (fp, "  -e   generate encode routines\n");
    fprintf (fp, "  -d   generate decode routines\n");
    fprintf (fp, "  -p   generate print routines\n");
    fprintf (fp, "  -f   generate hierarchical free routines (C only)\n");
    fprintf (fp, "    Note: if none of -t -v -e -d -p -f are given, all are generated.\n");
	fprintf (fp, "    These do not affect type tables.\n");
	fprintf (fp, "  -y   enable bison debugging\n");
	fprintf (fp, "  -M   uses the 1990 Syntax, default is the 1997 Syntax\n");

    fprintf (fp, "  -mm  mangle output file name into module name (by default, the output file\n");
    fprintf (fp, "       inherits the input file's name, with only the suffix replaced)\n");
    fprintf (fp, "  -mf <num> num is maximum file name length for the generated source files\n");

    fprintf (fp, "  -l <neg num> where to start error longjmp values decending from (obscure).\n");
    fprintf (fp, "  -L <error log file> print syntax errors to the specified error log file\n");
	fprintf (fp, "                      (default is stderr)\n");

#if META
    fprintf (fp, "  -meta <type list> generate meta code that describes the generated types. Implies -C.\n");
    fprintf (fp, "  -mA  metacode: use names as defined in the ASN.1 files.\n");
    fprintf (fp, "  -mC  metacode: use names as used in the generated C++ files.\n");
#if TCL
    fprintf (fp, "  -tcl <type list> generate code for a Tcl interpreter. Implies -meta.\n");
#endif
    fprintf (fp, "    <type list> has the following syntax: <module>.<type>[,<module>.<type>[...]]\n");
    fprintf (fp, "    the types listed are the top level PDUs.\n");
#endif

    fprintf (fp, "\nUse `-' as the ASN.1 source file name to parse stdin.\n\n");


    fprintf (fp, "This ASN.1 compiler produces C or C++ BER encoders and decoders or type tables.\n");


    fprintf (fp, "\nVersion %s\n", versionG);
    fprintf (fp, "Release Date:  %s\n", releasedateG);
    fprintf (fp, "Please see %s for new versions and where to send bug reports and comments.\n\n", bugreportaddressG);

    fprintf (fp, "Copyright (C) 1993 Michael Sample and UBC\n");
    fprintf (fp, "Copyright (C) 1994, 1995 by Robert Joop and GMD FOKUS\n\n");

    fprintf (fp, "This program is free software; you can redistribute it and/or modify\n");
    fprintf (fp, "it under the terms of the GNU General Public License as published by\n");
    fprintf (fp, "the Free Software Foundation; either version 2 of the License, or\n");
    fprintf (fp, "(at your option) any later version.\n\n");

    fprintf (fp, "This program is distributed in the hope that it will be useful,\n");
    fprintf (fp, "but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    fprintf (fp, "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    fprintf (fp, "GNU General Public License for more details.\n\n");

    fprintf (fp, "You should have received a copy of the GNU General Public License\n");

}


/****************/
/* main routine */
/****************/
int main PARAMS ((argc, argv),
    int			argc _AND_
    char		**argv)
{
    int			semErr;
    SRC_FILE	*srcList = NULL, *tmpLst = NULL;
	char		*parseFile;
	char		*file1, *file2;
    int			numSrcFiles;
    ModuleList		*allMods = NULL, *ImpMods = NULL;
    Module		*currMod;
	Module		**tmpModHndl;
    int			currArg;
    int			printModuleFlag = FALSE;  /* default: Don't print */
    int			genTypeTbls = 0; /* default: Don't gen tbls */
    char		*tblFileName=NULL;
	int         encRulesSet = FALSE;
    int			genTypeCode = FALSE;
    int			genEncodeCode = FALSE;
    int			genDecodeCode = FALSE;
    int			genPrintCode = FALSE;
    int			genValueCode = FALSE;
    int			genFreeCode = FALSE;
#if META
    MetaNameStyle	genMetaCode = META_off;
    MetaPDU		*meta_pdus = NULL;
#if TCL
    int			genTclCode = FALSE;
#endif
#endif
    int			genCCode = FALSE;        /* defaults to C if neither specified */
    int			genCxxCode = FALSE;
    int			genIDLCode = FALSE;
    long		longJmpVal = -100;
    int			novolatilefuncs = FALSE;
	char*		dirName;					/* REN -- 6/2/03 -- added */
	char*		errFileName;				/* REN -- 7/7/03 -- added */
	int			incDirSpecified = FALSE;	/* REN -- 7/7/03 -- added */

	if (argc <= 1)
	{
		Usage (argv[0], stderr);
		return 1;
	}

    srcList = (SRC_FILE*) Malloc ((argc -1) * sizeof (SRC_FILE));


    /*
     * parse cmd line args
     */
    numSrcFiles = 0;
    for (currArg = 1; (currArg < argc); )
    {
        if ((argv[currArg][0] == '-') && (argv[currArg][1] != '\0'))
		{
			switch (argv[currArg][1])
			{
      		case 'h':
				Usage (argv[0], stdout);
				return 0;
				break;
			
			case 'M':	// Deepak: 24/Mar/2003
				isSyntax1997 = 0;
				isTableConstraintAllowed = 0;
				currArg++;
				break;

			case 'a': /* AnyID start value */
				if (argv[currArg][2] != '\0')  /* no space after -a */
                {
                    anyEnumValG = atoi (&argv[currArg][2]);
                    currArg++;
                }
                else
                {
                    anyEnumValG = atoi (argv[currArg+1]);
                    currArg += 2;
                }
				break;

			case 'I':
				incDirSpecified = TRUE;
				if (argv[currArg][2] != '\0')
					dirName = &argv[currArg][2];
				else
					dirName = argv[++currArg];

				// Add or append the files
				if ((Add2SrcList(&srcList, dirName, TRUE)) == -1)
				{
					fprintf (stderr, "%s: ERROR---Unknown ASN Import Directory -I",
						dirName);
					Usage (argv[0], stderr);
					return 1;
				}
				currArg++;
				break;

			case 'P':
				printModuleFlag = TRUE;
				currArg++;
				break;

			case 'v':
				genValueCode = TRUE;
				currArg++;
				break;
#if IDL
			case 'i':
				if (!strcmp (argv[currArg]+1, "idl"))
				{
					genIDLCode = TRUE;
					currArg++;
				}
				else
					goto error;
				break;
#endif

			case 't':
				if (!strcmp (argv[currArg]+1, "tcl"))
				{
#if TCL
					meta_pdus = parse_type_list (argv[++currArg]);
					genTclCode = TRUE;
					if (!genMetaCode)
						genMetaCode = META_backend_names;
					genCxxCode = TRUE;
#else
					goto error;
#endif
				}
				else
					genTypeCode = TRUE;
				currArg++;
				break;

			case 'e':
				genEncodeCode = TRUE;
				currArg++;
				break;

			case 'd':
				genDecodeCode = TRUE;
				currArg++;
				break;

			case 'p':
				genPrintCode = TRUE;
				currArg++;
				break;

			case 'f':
				genFreeCode = TRUE;
				currArg++;
				break;

			case 'C': /* produce C++ code */
				genCxxCode = TRUE;
				currArg++;
				break;
            
            case 'b': /* produce C++ code */
				genPERCode = TRUE;
				currArg++;
				break;

			case 'n':
				if (!strcmp (argv[currArg], "nons"))
				{
					currArg++;
					gNO_NAMESPACE=1;
				}
				else if(!strncmp(argv[currArg], "-ns", 3))
				{
					gAlternateNamespaceString = &argv[currArg][4];
					currArg+=2;
				}
				else if (!strcmp (argv[currArg]+1, "novolat"))
				{
					novolatilefuncs = TRUE;
					currArg++;
				}
				else
					goto error;
				break;

			case 'c':
				genCCode = TRUE;
				currArg++;
				break;

			case 'l':
				if (argv[currArg][2] != '\0')  /* no space after -l */
				{
					longJmpVal = atoi (&argv[currArg][2]);
					currArg++;
				}
				else
				{
					longJmpVal = atoi (argv[currArg+1]);
					currArg += 2;
				}
				break;

			case 'T':
			case 'O':
				genTypeTbls = argv[currArg][1]=='T'?2:1;
				if (argv[currArg][2] != '\0')  /* no space after -T */
				{
					tblFileName = &argv[currArg][2];
					currArg++;
				}
				else
				{
					tblFileName = argv[currArg+1];
					currArg += 2;
				}
                break;
			
			case 'E':
				if (currArg + 1 == argc)
				{
					fprintf (errFileG, "%s: ERROR---encoding rule missing after -E\n",
						argv[0]);
					Usage(argv[0], stdout);
					return 1;
				}
				/* Select encoding rules */
				if (strcmp(argv[currArg+1], "BER") == 0)
				{
					AddEncRules(BER);
					encRulesSet = TRUE;
					currArg+=2;
				}
				else if (strcmp(argv[currArg+1], "DER") == 0)
				{
					AddEncRules(DER);
					encRulesSet = TRUE;
					currArg+=2;
				}
				else
				{
					fprintf (errFileG, "%s: ERROR---no such encoding rule \"%s\". Try BER or DER\n",
						argv[0], argv[currArg+1]);
					Usage(argv[0], stdout);
					return 1;
				}
				break;

			case 'V':
				if (!strncmp (argv[currArg]+1, "VDAexport",
					strlen("VDAexport")))
				{
					if (strlen(argv[currArg]+1) > strlen("VDAexport"))
						bVDAGlobalDLLExport = strdup(argv[currArg]+1+
						strlen("VDAexport")+1);    //TRUE
					else        // Default a definition for SFL.
						bVDAGlobalDLLExport = "VDASNACCDLL_API";
					currArg++;
					break;
				}
				else
					currArg++;  // IGNORE the "-V" option.
				break;

			case 'L':
				if (errFileG != NULL)
				{
					fprintf (stderr, "ERROR---Multiple occurrences of error log file option -L");
					Usage (argv[0], stderr);
					return 1;
				}
				if (argv[currArg][2] != '\0')
					errFileName = &argv[currArg][2];
				else
					errFileName = argv[++currArg];

				/* Open the error log file */
				errFileG = fopen(errFileName, "wt");
				if (errFileG == NULL)
				{
					fprintf (stderr, "ERROR---Unable to open error log file: \'%s\'\n",
						errFileName);
					return 1;
				}
				currArg++;
				break;

			case 'y':
				/*RWC;yydebug = 1;*/
				currArg++;
				break;

			case 'm':
				if (argv[currArg][2] == 'f')
				{
					if (argv[currArg][3] != '\0')  /* no space after -mf */
					{
						maxFileNameLenG = atoi (&argv[currArg][3]);
						currArg++;
					}
					else
					{
						maxFileNameLenG = atoi (argv[currArg+1]);
						currArg += 2;
					}
					break;
				}
#if META
				else if (!strcmp (argv[currArg]+1, "meta"))
				{
					meta_pdus = parse_type_list (argv[++currArg]);
					if (!genMetaCode)
						genMetaCode = META_backend_names;
					genCxxCode = TRUE;
					currArg++;
					break;
				}
				else if (!strcmp (argv[currArg]+1, "mA"))
				{
					genMetaCode = META_asn1_names;
					genCxxCode = TRUE;
					currArg++;
					break;
				}
				else if (!strcmp (argv[currArg]+1, "mC"))
				{
					genMetaCode = META_backend_names;
					genCxxCode = TRUE;
					currArg++;
					break;
				}
#endif
				else if (argv[currArg][2] == 'm')
				{
					keepbaseG = FALSE;
					currArg++;
					break;
				}


error:
			default:
				fprintf (stderr, "%s: ERROR---unknown cmd line option `%s'\n\n", argv[0], argv[currArg]);
				Usage (argv[0], stderr);
				return 1;
			}
		}
		else /* asn1srcFileName */
		{
			numSrcFiles++;
			tmpLst = srcList;
			file1 = sbasename(argv[currArg]);
			Add2SrcList(&srcList, argv[currArg], FALSE);
			currArg++;
			while (tmpLst != NULL)
			{
				if (tmpLst->fileName != NULL)
				{
					file2 = sbasename(tmpLst->fileName);
					if (strstr (file1, file2))
					{
						free(tmpLst->fileName);
						tmpLst->fileName = NULL;
					}
				}
				tmpLst = tmpLst->next;
			}
		}
	} /* end of for loop */

	if (numSrcFiles == 0)
	{
		fprintf (stderr, "%s: ERROR---no ASN.1 source files were specified\n",
			argv[0]);
		Usage (argv[0], stderr);
		return 1;
	}


    /*
	 * set default options
	 */
    if (!(genTypeCode || genValueCode || genEncodeCode || genDecodeCode ||
          genFreeCode || genPrintCode))
    {
        genTypeCode = TRUE;
        genValueCode = TRUE;
        genEncodeCode = TRUE;
        genDecodeCode = TRUE;
        genFreeCode = TRUE;
        genPrintCode = TRUE;
    }
    else if (genCCode + genCxxCode + genTypeTbls + genIDLCode > 1)
    {
        fprintf (stderr, "%s: ERROR---Choose only one of the -c -C or -T options\n",
			argv[0]);
        Usage (argv[0], stderr);
        return 1;
    }

    if (!genCCode && !genCxxCode && !genTypeTbls && !genIDLCode)
        genCCode = TRUE;  /* default to C if neither specified */

	/* Set the encoding rules to BER if not set */
	if (!encRulesSet)
		AddEncRules(BER);

	/* Set the error log file to stderr if not specified */
	if (errFileG == NULL)
		errFileG = stderr;

	/*
	 * STEP 1---parse each ASN.1 src file
	 */
	allMods = (ModuleList *)AsnListNew (sizeof (void*));
	ImpMods = (ModuleList *)AsnListNew (sizeof (void*));
	
	while (srcList != NULL)
	{
		// Only do if not NULL 
		if (srcList->fileName)
		{
			parseFile = strdup (srcList->fileName);
			currMod = ParseAsn1File (parseFile, srcList->ImportFileFlag);
		
			if (currMod == NULL)
				return 1;

			/*
			 * insert this module at the head of the list
			 * of already parsed (if any) modules
			 */
			tmpModHndl = (Module **)AsnListAppend (allMods);
			*tmpModHndl = currMod;
		}
		srcList = srcList->next;
	}  /* end per src file for loop */

    /*
	 * Check that the module names/oids are unique
	 */
	if (!ModNamesUnique (allMods))
	{
		fprintf (errFileG, "\nConflicting module names, cannot proceed.\n");
		return 1;
	}


	/*
	 * STEP 2
	 * Now that all files have been parsed,
	 * link local and locatable import type refs
	 */
	if (LinkTypeRefs (allMods) < 0)
	{
		fprintf (errFileG, "\nType linking errors---cannot proceed\n");
		return 2;
	}


    /*
	 * STEP 3
	 * Parse constructed values now that types are all parsed
	 * and have been linked.  Need type info to be able to
	 * parse values easily (elimitate ambiguity).
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		if (ParseValues (allMods, currMod) != 0)
			fprintf (errFileG, "WARNING: Value parsing error (s), attempting to continue\n");
    }


    /*
     * STEP 4
     * Value parsing may have defined some new values
     * so can link local and locatable import value refs now.
     */
    if (LinkValueRefs (allMods) < 0)
    {
        fprintf (errFileG, "\nValue linking errors---cannot proceed\n");
        return 4;
    }


    /*
     * STEP 5
     * process macros
     *   - adding type/value defs as nec
     *   - mark type defs with ANY DEFINED BY id if nec
     *     so they are put in the id to ANY type hash tbl.
     */
    semErr = 0;
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{	// For Macors, New TypeDefs are added here, if required
		ProcessMacros (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}
	if (semErr)
		return 5;


    /*
	 * STEP 6
	 * convert silly type constructs into
	 * a normal format, leaving behind pure type/value info
	 * eg: expand COMPONENTS OF refs, SELECTION types.
	 * boil down values into simplest rep. (eg OID -> ENC_OID)
	 */
	semErr = 0;
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{	// New TypeDefs are added here, if required
		NormalizeModule (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}
    if (semErr)
		return 6;


    /*
	 * STEP 7
	 * Mark recusive types.  Currently the recursive information is
	 * not used elsewhere.
	 */
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		MarkRecursiveTypes (currMod);
	}


	/*
	 * STEP 8
	 * Check for errors in the ASN.1 modules.
	 * Check all modules and exit if errors were found
	 */
	semErr = 0;
	FOR_EACH_LIST_ELMT (currMod, allMods)
	{
		ErrChkModule (currMod);
		if (currMod->status == MOD_ERROR)
			semErr = 1;
	}
	if (semErr)
		return 8;


    /*
	 * exit if any sundry errors occurred at any point.
	 * smallErrG is set upon finding small errors that prevent code
	 * production but should not affect the other processing/error
	 * checking steps.  This allows full display of errors.
	 */
	if (smallErrG)
	{
		/*
		 * for debugging show "parsed" version of ASN.1 module if
		 * the print flag is set.
		 * Dumps each module to stdout. Printed from Module data struct
		 * print here before exiting otherwise print after sorting
		 */
		if (printModuleFlag)
		{
			FOR_EACH_LIST_ELMT (currMod, allMods)
			{
				printf ("\n\n");
				PrintModule (stdout, currMod);
			}
		}
		
		return 8;
	}


	/*
	 * STEP 9
	 * Make C/C++ typenames/routine names for enc/decode.
	 * Type/Value renaming will occur if name conflicts
	 * arise between modules.
	 *
	 * NOTE: this is done before sorting the types because
	 *       the type sorting routine may use the 'isPtr'
	 *       information to help order knots of recursive types.
	 */
	if (genCCode)
		FillCTypeInfo (&cRulesG, allMods);

	else if (genCxxCode)
		FillCxxTypeInfo (&cxxRulesG, allMods);

#if IDL
	else if (genIDLCode)
		FillIDLTypeInfo (&idlRulesG, allMods);
#endif


	/*
	 * STEP 10
	 * Sort each typedef list such that independent types are
	 * before the types that depend on them
	 *
	 *  modules remain in same order as given on command line
	 *  (cmd line file order should be
	 *      least dependent module-> most dependent module
	 *      so that include file order in generated src is correct)
	 */
	SortAllDependencies (allMods);

	/*
	 * for debugging show "parsed" version of ASN.1 module.
	 * dumps each module to stdout. Printed from Module data struct
	 * Shows the results of normalization and sorting.
	 */
	if (printModuleFlag)
	{
		FOR_EACH_LIST_ELMT (currMod, allMods)
		{
			printf ("\n\n");
			PrintModule (stdout, currMod);
		}
	}


	/*
	 * Step 12
	 * Final Step: Code/Type Table generation
	 */
	if (genCCode)
		GenCCode (allMods, longJmpVal, genTypeCode, genValueCode,
			genEncodeCode, genDecodeCode, genPrintCode, genFreeCode);

    else if (genCxxCode)
        GenCxxCode (allMods, longJmpVal, genTypeCode, genValueCode,
			genEncodeCode, genDecodeCode, genPrintCode, genFreeCode,
			if_META (genMetaCode COMMA meta_pdus COMMA)
			if_TCL (genTclCode COMMA) novolatilefuncs);

	else if (genTypeTbls)
		GenTypeTbls (allMods, tblFileName, genTypeTbls);

#if IDL
	else if (genIDLCode)
		GenIDLCode (allMods, longJmpVal, genTypeCode, genValueCode,
			genPrintCode, genFreeCode);
#endif

    free(allMods);

	return 0;
} /* end main */



#if META
MetaPDU *parse_type_list PARAMS ((arg),
    char *arg)
{
    MetaPDU *meta_pdus = NULL;
    char *module;
    for (module = strtok (arg, ".:"); module; module = strtok (NULL, ".:"))
    {
		MetaPDU *pdu = MT (MetaPDU);
		char *type = strtok (NULL, " /,;");
		if (!type)
		{
			fprintf (errFileG, "usage: {-meta|-tcl} module.type[,module.type[...]]\n");
			return (1);
		}
		pdu->module = module;
		pdu->type = type;
		pdu->used = FALSE;
		
		pdu->next = meta_pdus;
		meta_pdus = pdu;
    }
    return meta_pdus;
}
#endif /* META */


/*
 * Calls the yacc/lex parser given a the ASN.1 src file's filename.
 * Returns a Module *for the given ASN.1 module. If the filename is
 * "-" stdin is used.
 */
Module *
ParseAsn1File PARAMS ((fileName, ImportFlag),
    char *fileName _AND_ 
	short ImportFlag)
{
    FILE *fPtr;
    Module *retVal;
    int parseResult;

    /*
	 *  Open input file for lexical analyzer/parser
	 *  Use stdin if the filename is "-"
	 */
	if (strcmp (fileName, "-") == 0)
	{
		if (keepbaseG)
		{
			fprintf (errFileG, "ERROR---asn1 src file `%s' cannot be processed without output filename mangling\n", fileName);
			return NULL;
		}
		else
			fPtr = stdin;
	}
	else
	{
		fPtr = fopen (fileName, "r");
		if (fPtr == NULL)
		{
			perror ("fopen: ");
			fprintf (errFileG, "ERROR---asn1 src file `%s' cannot be opened for reading\n",
				fileName);
			return NULL;
		}
	}

    retVal = (Module *)Malloc (sizeof (Module));

    /*
     * Init Parser by giving it a ptr to the Module data struct
     * to initialize/use, and the file name associtated with
     * the given FILE *, fPtr (for error reporting).
     * fPtr should be an opened FILE *to an ASN.1 source FILE
     */
    InitAsn1Parser (retVal, fileName, fPtr);


    /*
     * parse the current asn1 src file into the
     * Module data struct
     */
    parseResult = yyparse();

    if (parseResult != 0 || retVal->status == MOD_ERROR)
    {
        /* parser will print exact err msg */
        fprintf (errFileG, "\nParsing errors---cannot proceed\n");
        return NULL;
    }

	if (fPtr != stdin)
		fclose (fPtr);

	retVal->ImportedFlag = (unsigned char)ImportFlag;

    return retVal;

}  /* ParseAsn1File */


/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .c
 * for the enc/dec/print/free routine code.
 */
int
GenCCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree),
    ModuleList *allMods _AND_
    long longJmpVal _AND_
    int genTypes _AND_
    int genValues _AND_
    int genEncoders _AND_
    int genDecoders _AND_
    int genPrinters _AND_
    int genFree)
{
    Module *currMod;
    char *modBaseFileName;
    FILE *cHdrFilePtr = NULL;
    FILE *cSrcFilePtr = NULL;
    DefinedObj *fNames;
    int fNameConflict = FALSE;

    /*
     * Make names for each module's encoder/decoder src and hdr files
     * so import references can be made via include files.
     * If file names conflict, print error msg & exit.
     */
    fNames = NewObjList();
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
        modBaseFileName = MakeBaseFileName (keepbaseG
		? currMod->asn1SrcFileName
		: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
			currMod->cHdrFileName = MakeCHdrFileName (modBaseFileName);
			currMod->cSrcFileName = MakeCSrcFileName (modBaseFileName);
        if (ObjIsDefined (fNames, currMod->cHdrFileName, StrObjCmp) ||
            ObjIsDefined (fNames, currMod->cSrcFileName, StrObjCmp))
        {
            fprintf (errFileG, "Ack! ERROR---file name conflict for generated source files with names `%s' and `%s'.\n\n", currMod->cHdrFileName, currMod->cSrcFileName);
            fprintf (errFileG, "This usually means the max file name length is truncating the file names.\n");
            fprintf (errFileG, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
            fprintf (errFileG, "This error can also be caused by 2 modules with the same names but different OBJECT IDENTIFIERs.");
            fprintf (errFileG, "  Try renaming the modules to correct this.\n");
            fNameConflict = TRUE;
        }
        else
        {
            DefineObj (&fNames, currMod->cHdrFileName);
            DefineObj (&fNames, currMod->cSrcFileName);
        }
        Free (modBaseFileName);
    }
    if (fNameConflict)
        return (1);

    FreeDefinedObjs (&fNames);
    /*
     * make c files
     */
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
		if (currMod->ImportedFlag == FALSE)
		{
			cHdrFilePtr = fopen (currMod->cHdrFileName, "w");
			cSrcFilePtr = fopen (currMod->cSrcFileName, "w");
		}
		if ((currMod->ImportedFlag == FALSE) && 
			((cSrcFilePtr == NULL) || (cHdrFilePtr == NULL)))
			perror ("fopen");

		else
		{
			if (currMod->ImportedFlag == FALSE)
			{
				 PrintCCode (cSrcFilePtr, cHdrFilePtr, allMods, currMod, 
						&cRulesG, longJmpVal, genTypes,  genValues, 
						genEncoders, genDecoders, genPrinters, genFree);
			}
		}
        if (cSrcFilePtr)
        {
            fclose (cSrcFilePtr);
            cSrcFilePtr = NULL;
        }

        if (cHdrFilePtr)
        {
            fclose (cHdrFilePtr);
            cHdrFilePtr = NULL;
        }

    }
	return 0;
}  /* GenCCode */


/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C++ code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .C
 * for the enc/dec/print/free routine code.
 */
void
GenCxxCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree, if_META (genMeta COMMA meta_pdus COMMA) if_TCL (genTcl COMMA) novolatilefuncs),
    ModuleList *allMods _AND_
    long longJmpVal _AND_
    int genTypes _AND_
    int genValues _AND_
    int genEncoders _AND_
    int genDecoders _AND_
    int genPrinters _AND_
    int genFree _AND_
    if_META (MetaNameStyle genMeta _AND_)
    if_META (MetaPDU *meta_pdus _AND_)
    if_TCL (int genTcl _AND_)
    int novolatilefuncs)
{
    Module		*currMod;
	AsnListNode	*saveMods;
    char		*modBaseFileName;
    FILE		*hdrFilePtr;
    FILE		*srcFilePtr;
    DefinedObj		*fNames;
    int			fNameConflict = FALSE;

#if META
    static const char	metabasefn[] = "modules";
    Meta		meta;
#if TCL
    const MetaPDU	*pdu;
#endif
#endif

    /*
     * Make names for each module's encoder/decoder src and hdr files
     * so import references can be made via include files
     * check for truncation --> name conflicts & exit if nec
     */
    fNames = NewObjList();
#if META
    if (genMeta)
	DefineObj (&fNames, meta.srcfn = MakeCxxSrcFileName (metabasefn));
#endif
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
        modBaseFileName = MakeBaseFileName (keepbaseG
		? currMod->asn1SrcFileName
		: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
        currMod->cxxHdrFileName = MakeCxxHdrFileName (modBaseFileName);
        currMod->cxxSrcFileName = MakeCxxSrcFileName (modBaseFileName);

#if META
	{
	    char *in, *out;

	    out = currMod->cxxname = (char *)malloc (strlen (in = currMod->modId->name)+1);
	    do
		*out++ = *in == '-' ? '_' : *in;
	    while (*in++);
	}
#endif

        if (ObjIsDefined (fNames, currMod->cxxHdrFileName, StrObjCmp) || ObjIsDefined (fNames, currMod->cxxSrcFileName, StrObjCmp))
        {
            fprintf (errFileG, "Ack! ERROR---file name conflict for generated source files with names `%s' and `%s'.\n\n", currMod->cxxHdrFileName, currMod->cxxSrcFileName);
            fprintf (errFileG, "This usually means the max file name length is truncating the file names.\n");
            fprintf (errFileG, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
            fprintf (errFileG, "This error can also be caused by 2 modules have the same names but different OBJECT IDENTIFIERs.");
            fprintf (errFileG, "  Try renaming the modules to correct this.\n");
            fNameConflict = TRUE;
        }
        else
        {
            DefineObj (&fNames, currMod->cxxHdrFileName);
            DefineObj (&fNames, currMod->cxxSrcFileName);
        }
        Free (modBaseFileName);
    
    if (fNameConflict)
        return;

    FreeDefinedObjs (&fNames);

    /*
     * make C++ files
     */
#if META
    if (genMeta)
    {
		time_t now = time (NULL);

		if (!(meta.srcfp = fopen (meta.srcfn, "w")))
		{
		  perror ("fopen");
		  exit (1);
		}
		fprintf (meta.srcfp, "// NOTE: this is a machine generated file--editing not recommended\n");
		fprintf (meta.srcfp, "//\n");
		fprintf (meta.srcfp, "// modules.C - reference to all modules and their types\n");
		fprintf (meta.srcfp, "//\n");
		fprintf (meta.srcfp, "// This file was generated by snacc on %s", ctime (&now));
    }
#endif
	}
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
		if (currMod->ImportedFlag == FALSE)
		{
        /*
         * create and fill .h file for module's data structs
         */
        hdrFilePtr = fopen (currMod->cxxHdrFileName, "wt");
        srcFilePtr = fopen (currMod->cxxSrcFileName, "wt");

        if ((hdrFilePtr == NULL) || (srcFilePtr == NULL))
        {
            perror ("fopen");
        }
        else
        {
			saveMods = allMods->curr;
            PrintCxxCode (srcFilePtr, hdrFilePtr,
			if_META (genMeta COMMA &meta COMMA meta_pdus COMMA)
			allMods, currMod, &cxxRulesG, longJmpVal,
			genTypes, genValues, genEncoders, genDecoders, genPrinters, genFree,
			if_TCL (genTcl COMMA) novolatilefuncs);
			allMods->curr = saveMods;
            fclose (hdrFilePtr);
            fclose (srcFilePtr);
        }
#if META
    if (genMeta)
    {
	fprintf (meta.srcfp, "\n");
	fprintf (meta.srcfp, "#ifndef META\n");
	fprintf (meta.srcfp, "#define META	1\n");
	fprintf (meta.srcfp, "#endif\n");
	if (meta_pdus)
	{
	    for (pdu=meta_pdus; pdu; pdu=pdu->next)
		if (!pdu->used)
		    fprintf (errFileG, "warning: PDU %s.%s couldn't be found\n", pdu->module, pdu->type);
	}
#if TCL
	fprintf (meta.srcfp, "#ifndef TCL\n");
	fprintf (meta.srcfp, "#define TCL	META\n");
	fprintf (meta.srcfp, "#endif\n");
	}
#endif
	fprintf (meta.srcfp, "\n");

	fprintf (meta.srcfp, "#include \"asn-incl.h\"\n");
	FOR_EACH_LIST_ELMT (currMod, allMods)
	    fprintf (meta.srcfp, "#include \"%s\"\n", currMod->cxxHdrFileName);
	fprintf (meta.srcfp, "\n");

	fprintf (meta.srcfp, "#if META\n\n");

	fprintf (meta.srcfp, "const AsnModuleDesc *asnModuleDescs[] =\n");
	fprintf (meta.srcfp, "{\n");
	FOR_EACH_LIST_ELMT (currMod, allMods)
	    fprintf (meta.srcfp, "  &%sModuleDesc,\n", currMod->cxxname);
	fprintf (meta.srcfp, "  NULL\n");
	fprintf (meta.srcfp, "};\n\n");

	if (genTcl)
	{
	  fprintf (meta.srcfp, "#if TCL\n\n");

	  fprintf (meta.srcfp, "// hack to avoid the neccessity to list -ltk -ltcl both before and after -lasn1tcl:\n");
	  fprintf (meta.srcfp, "static int (*dummy)(Tcl_Interp *) = Tcl_AppInit;\n\n");

	  fprintf (meta.srcfp, "#endif // TCL\n\n");
	}

	fprintf (meta.srcfp, "#endif // META\n");

	fclose (meta.srcfp);
    
#endif
	}
	}
}  /* GenCxxCode */


#if IDL
/*
 * Given the list of parsed, linked, normalized, error-checked and sorted
 * modules, and some code generation flags, generates C++ code and
 * writes it to files derived from each modules name.  Each module
 * gets 2 source files, one .h for data struct and prototypes, the other .C
 * for the enc/dec/print/free routine code.
 */
void
GenIDLCode PARAMS ((allMods, longJmpVal, genTypes, genValues, genPrinters, genFree),
    ModuleList *allMods _AND_
    long longJmpVal _AND_
    int genTypes _AND_
    int genValues _AND_
    int genPrinters _AND_
    int genFree)
{
    Module		*currMod;
    char		*modBaseFileName;
    FILE		*idlFilePtr;
    DefinedObj		*fNames;
    int			fNameConflict = FALSE;

    /*
     * Make names for each module's encoder/decoder src and hdr files
     * so import references can be made via include files
     * check for truncation --> name conflicts & exit if nec
     */
    fNames = NewObjList();
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
        modBaseFileName = MakeBaseFileName (keepbaseG
		? currMod->asn1SrcFileName
		: currMod->modId->name); /* shorten module name if necessary (SYSV etc) */
        currMod->idlFileName = MakeIDLFileName (modBaseFileName);
	{
	    char *in, *out;

	    out = currMod->idlname = (char *)malloc (strlen (in = currMod->modId->name)+1);
	    do
		*out++ = (char)(*in == '-' ? '_' : *in);
	    while (*in++);
	}

        if (ObjIsDefined (fNames, currMod->idlFileName, StrObjCmp))
        {
            fprintf (errFileG, "Ack! ERROR---file name conflict for generated source file with name `%s'.\n\n", currMod->idlFileName);
            fprintf (errFileG, "This usually means the max file name length is truncating the file names.\n");
            fprintf (errFileG, "Try re-naming the modules with shorter names or increasing the argument to -mf option (if you are using it).\n");
            fprintf (errFileG, "This error can also be caused by 2 modules have the same names but different OBJECT IDENTIFIERs.");
            fprintf (errFileG, "  Try renaming the modules to correct this.\n");
            fNameConflict = TRUE;
        }
        else
        {
            DefineObj (&fNames, currMod->idlFileName);
        }
        Free (modBaseFileName);
    }
    if (fNameConflict)
        return;

    FreeDefinedObjs (&fNames);

    /*
     * make C++ files
     */
    FOR_EACH_LIST_ELMT (currMod, allMods)
    {
        /*
         * create and fill .h file for module's data structs
         */
        idlFilePtr = fopen (currMod->idlFileName, "w");
        if (idlFilePtr == NULL)
            perror ("fopen");
        else
        {
            PrintIDLCode (idlFilePtr, allMods, currMod, &idlRulesG, longJmpVal, genValues);

            fclose (idlFilePtr);
        }
    }
    genFree = genFree;
    genPrinters = genPrinters;
    genTypes = genTypes;   /* AVOIDS warnings. */
}  /* GenIDLCode */
#endif /* IDL */


/*
 * returns 1 if the module names and oid's are unique.
 * otherwise returns 0
 */
int ModNamesUnique PARAMS ((mods),
    ModuleList *mods)
{
    DefinedObj *names;
    DefinedObj *oids;
    Module *m;
    int retVal = 1;

    names = NewObjList();
    oids = NewObjList();

    FOR_EACH_LIST_ELMT (m, mods)
    {
		m->ImportUsed = FALSE;
        if (((m->modId->oid != NULL) &&
            ObjIsDefined (oids, m->modId->oid, OidObjCmp)))
        {
            /* oops, 2 modules have the same oid */
            PrintErrLoc (m->asn1SrcFileName, (long)1);
            fprintf (errFileG, "ERROR---2 modules have the OBJECT IDENTIFIER `");
            PrintOid (stdout, m->modId->oid);
            fprintf (errFileG, "'.\n");
            retVal = 0;
        }
        /* name is only signficant if oid is empty */
        else if ((m->modId->oid == NULL) &&
             (ObjIsDefined (names, m->modId->name, StrObjCmp)))
        {
            /* oops, 2 modules have the same name */
            PrintErrLoc (m->asn1SrcFileName, (long)1);
            fprintf (errFileG, "ERROR---2 modules have the name `%s'\n", m->modId->name);
            retVal = 0;
        }
        else
        {
			// Add in the Imported File 
            DefineObj (&names, m->modId->name);
            if (m->modId->oid != NULL)
                DefineObj (&oids, m->modId->oid);
        }
    }
    FreeDefinedObjs (&names);
    FreeDefinedObjs (&oids);
    return retVal;
} /* ModNamesUnique */

/*
 * FileList is the linked list of files
 * InputFile is used only for snacc command line files
 * ImportFlag is set to true signifies a Imported file if
 * false then the file is a command line file
 */
short Add2SrcList(SRC_FILE **FileList, const char *InputFile, short ImportFlag)
{
#ifdef _WIN32		//RWC;
	static const char* gAsnExt = "asn*";
#else		//RWC;
	static const char* gAsnExt = "asn1";  //RWC;
#endif		//RWC;

    GFSI_HANDLE gfsi_handle = NULL;
	char *fileName = NULL;
	char *fullPath;
	SRC_FILE *tmpList;
	SRC_FILE *prev;
	short err = -1;

	/* Find the last file in the list */
	prev = *FileList;
	while ((prev != NULL) && (prev->next != NULL))
		prev = prev->next;

	if (ImportFlag)
	{
		/* Loop through the files in the specified directory and
		   process each file with the proper extension.
		*/
		fileName = GFSI_GetFirstFile(&gfsi_handle, InputFile, gAsnExt);
		while (fileName != NULL)
		{
            /* Alloc 2 extra bytes below - 1 for null byte, and 1 for the /
               which needs to be inserted in the middle */
            fullPath = (char *)calloc(strlen(InputFile)+strlen(fileName)+2,
                                      sizeof char);
            
			/* Build the full path to the file */
            
			sprintf(fullPath, "%s/%s", InputFile, fileName);

			/* Check if this file is already present */
			err = compareDupeFile(fullPath, *FileList);
			if (err < 0)
			{
				fprintf (stderr, "Duplicate include import file reference %s, using first reference \n",
					fullPath);
			}
			else if (err == 0)
			{
				/* Add this file to the list */
				tmpList = (SRC_FILE*)Malloc(sizeof(SRC_FILE));
				if (prev == NULL)
					*FileList = tmpList;
				else
					prev->next = tmpList;
				prev = tmpList;
				tmpList->fileName = strdup(fullPath);
				tmpList->ImportFileFlag = ImportFlag;
			}
			/* else skip this file */

			/* Get the next file */
			fileName = GFSI_GetNextFile(&gfsi_handle, gAsnExt);

			err = 0;
            free(fullPath);
            fullPath = 0;
		}
		
		GFSI_Close(&gfsi_handle);
        free(fileName);
	}
	else	/* File name specified on command line */
	{
		/* Add to head of the list */
		tmpList = (SRC_FILE*)Malloc(sizeof(SRC_FILE));
		tmpList->ImportFileFlag = ImportFlag;
		tmpList->fileName = strdup(InputFile);
		tmpList->next = *FileList;
		*FileList = tmpList;

		err = 0;
	}

	return err;
}


char *sbasename (char *name)
{
	char *base;
	
#ifdef WIN32
	/* Skip over the disk name in MSDOS pathnames. */
	if (isalpha (name[0]) && name[1] == ':') 
		name += 2;
#endif
	
	for (base = name; *name; name++)
	{
		if ((name[0] == '/') || (name[0] == '\\'))
		{
			base = name + 1;
		}
		
	}
	return (char *) base;
}


short compareDupeFile(char *fullpath, SRC_FILE *FileList)
{
	SRC_FILE *tmplist = FileList;
	char* file1 = sbasename(fullpath);
	char *file2;

	while (tmplist != NULL)
	{
		if (tmplist->fileName != NULL)
		{
			file2 = sbasename (tmplist->fileName);
			if (strstr (file1, file2) != NULL)
			{
				if (tmplist->ImportFileFlag)
					return -1;
				else
					return 1;
			}
		}
		tmplist = tmplist->next;
	}
	return 0;
}
