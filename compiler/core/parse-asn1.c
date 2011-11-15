
/*  A Bison parser, made from core/parse-asn1.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	BSTRING_SYM	257
#define	HSTRING_SYM	258
#define	CSTRING_SYM	259
#define	UCASEFIRST_IDENT_SYM	260
#define	LCASEFIRST_IDENT_SYM	261
#define	NAMEDMACRO_SYM	262
#define	MACRODEFBODY_SYM	263
#define	BRACEBAL_SYM	264
#define	NUMBER_ERANGE	265
#define	NUMBER_SYM	266
#define	SNACC_ATTRIBUTES	267
#define	COLON_SYM	268
#define	ELLIPSIS_SYM	269
#define	DOT_SYM	270
#define	COMMA_SYM	271
#define	LEFTBRACE_SYM	272
#define	RIGHTBRACE_SYM	273
#define	LEFTPAREN_SYM	274
#define	RIGHTPAREN_SYM	275
#define	LEFTBRACKET_SYM	276
#define	RIGHTBRACKET_SYM	277
#define	LESSTHAN_SYM	278
#define	MINUS_SYM	279
#define	GETS_SYM	280
#define	BAR_SYM	281
#define	TAGS_SYM	282
#define	BOOLEAN_SYM	283
#define	INTEGER_SYM	284
#define	BIT_SYM	285
#define	STRING_SYM	286
#define	OCTET_SYM	287
#define	CONTAINING_SYM	288
#define	ENCODED_SYM	289
#define	NULL_SYM	290
#define	SEQUENCE_SYM	291
#define	OF_SYM	292
#define	SET_SYM	293
#define	IMPLICIT_SYM	294
#define	CHOICE_SYM	295
#define	ANY_SYM	296
#define	OBJECT_IDENTIFIER_SYM	297
#define	RELATIVE_OID_SYM	298
#define	OPTIONAL_SYM	299
#define	DEFAULT_SYM	300
#define	COMPONENTS_SYM	301
#define	UNIVERSAL_SYM	302
#define	APPLICATION_SYM	303
#define	PRIVATE_SYM	304
#define	TRUE_SYM	305
#define	FALSE_SYM	306
#define	BEGIN_SYM	307
#define	END_SYM	308
#define	DEFINITIONS_SYM	309
#define	EXPLICIT_SYM	310
#define	ENUMERATED_SYM	311
#define	EXPORTS_SYM	312
#define	IMPORTS_SYM	313
#define	REAL_SYM	314
#define	INCLUDES_SYM	315
#define	MIN_SYM	316
#define	MAX_SYM	317
#define	SIZE_SYM	318
#define	FROM_SYM	319
#define	WITH_SYM	320
#define	COMPONENT_SYM	321
#define	PRESENT_SYM	322
#define	ABSENT_SYM	323
#define	DEFINED_SYM	324
#define	BY_SYM	325
#define	PLUS_INFINITY_SYM	326
#define	MINUS_INFINITY_SYM	327
#define	SEMI_COLON_SYM	328
#define	IA5STRING_SYM	329
#define	PRINTABLESTRING_SYM	330
#define	NUMERICSTRING_SYM	331
#define	TELETEXSTRING_SYM	332
#define	T61STRING_SYM	333
#define	VIDEOTEXSTRING_SYM	334
#define	VISIBLESTRING_SYM	335
#define	ISO646STRING_SYM	336
#define	GRAPHICSTRING_SYM	337
#define	GENERALSTRING_SYM	338
#define	BMPSTRING_SYM	339
#define	UNIVERSALSTRING_SYM	340
#define	UTF8STRING_SYM	341
#define	GENERALIZEDTIME_SYM	342
#define	UTCTIME_SYM	343
#define	EXTERNAL_SYM	344
#define	OBJECTDESCRIPTOR_SYM	345
#define	OPERATION_SYM	346
#define	ARGUMENT_SYM	347
#define	RESULT_SYM	348
#define	ERRORS_SYM	349
#define	LINKED_SYM	350
#define	ERROR_SYM	351
#define	PARAMETER_SYM	352
#define	BIND_SYM	353
#define	BINDERROR_SYM	354
#define	UNBIND_SYM	355
#define	UNBINDERROR_SYM	356
#define	ASE_SYM	357
#define	OPERATIONS_SYM	358
#define	CONSUMERINVOKES_SYM	359
#define	SUPPLIERINVOKES_SYM	360
#define	AC_SYM	361
#define	ASES_SYM	362
#define	REMOTE_SYM	363
#define	INITIATOR_SYM	364
#define	RESPONDER_SYM	365
#define	ABSTRACTSYNTAXES_SYM	366
#define	CONSUMER_SYM	367
#define	EXTENSIONS_SYM	368
#define	CHOSEN_SYM	369
#define	EXTENSION_SYM	370
#define	CRITICAL_SYM	371
#define	FOR_SYM	372
#define	DELIVERY_SYM	373
#define	SUBMISSION_SYM	374
#define	TRANSFER_SYM	375
#define	EXTENSIONATTRIBUTE_SYM	376
#define	TOKEN_SYM	377
#define	TOKENDATA_SYM	378
#define	SECURITYCATEGORY_SYM	379
#define	OBJECT_SYM	380
#define	PORTS_SYM	381
#define	BOXC_SYM	382
#define	BOXS_SYM	383
#define	PORT_SYM	384
#define	ABSTRACTOPS_SYM	385
#define	REFINE_SYM	386
#define	AS_SYM	387
#define	RECURRING_SYM	388
#define	VISIBLE_SYM	389
#define	PAIRED_SYM	390
#define	ABSTRACTBIND_SYM	391
#define	ABSTRACTUNBIND_SYM	392
#define	TO_SYM	393
#define	ABSTRACTERROR_SYM	394
#define	ABSTRACTOPERATION_SYM	395
#define	ALGORITHM_SYM	396
#define	ENCRYPTED_SYM	397
#define	SIGNED_SYM	398
#define	SIGNATURE_SYM	399
#define	PROTECTED_SYM	400
#define	OBJECTTYPE_SYM	401
#define	SYNTAX_SYM	402
#define	ACCESS_SYM	403
#define	STATUS_SYM	404
#define	DESCRIPTION_SYM	405
#define	REFERENCE_SYM	406
#define	INDEX_SYM	407
#define	DEFVAL_SYM	408
#define	ATTRIB_ASN1_TYPE_ID	409
#define	ATTRIB_C_TYPE_ID	410
#define	ATTRIB_C_TYPE_NAME_SYM	411
#define	ATTRIB_C_FIELD_NAME_SYM	412
#define	ATTRIB_IS_PDU_SYM	413
#define	ATTRIB_IS_PTR_SYM	414
#define	ATTRIB_IS_PTR_TYPEDEF_SYM	415
#define	ATTRIB_IS_PTR_TYPE_REF_SYM	416
#define	ATTRIB_IS_PTR_IN_CHOICE_SYM	417
#define	ATTRIB_IS_PTR_FOR_OPT_SYM	418
#define	ATTRIB_OPT_TEST_ROUTINE_SYM	419
#define	ATTRIB_DEFAULT_FIELD_SYM	420
#define	ATTRIB_PRINT_ROUTINE_SYM	421
#define	ATTRIB_ENCODE_ROUTINE_SYM	422
#define	ATTRIB_DECODE_ROUTINE_SYM	423
#define	ATTRIB_FREE_ROUTINE_SYM	424
#define	ATTRIB_IS_ENC_DEC_SYM	425
#define	ATTRIB_GEN_TYPEDEF_SYM	426
#define	ATTRIB_GEN_PRINT_ROUTINE_SYM	427
#define	ATTRIB_GEN_ENCODE_ROUTINE_SYM	428
#define	ATTRIB_GEN_DECODE_ROUTINE_SYM	429
#define	ATTRIB_GEN_FREE_ROUTINE_SYM	430
#define	ATTRIB_CHOICE_ID_SYMBOL_SYM	431
#define	ATTRIB_CHOICE_ID_VALUE_SYM	432
#define	ATTRIB_CHOICE_ID_ENUM_NAME_SYM	433
#define	ATTRIB_CHOICE_ID_ENUM_FIELD_NAME_SYM	434
#define	ATTRIB_IS_BIG_INT_SYM	435
#define	ATTRIB_NAMESPACE_SYM	436
#define	C_CHOICE_SYM	437
#define	C_LIST_SYM	438
#define	C_ANY_SYM	439
#define	C_ANYDEFBY_SYM	440
#define	C_LIB_SYM	441
#define	C_STRUCT_SYM	442
#define	C_TYPEDEF_SYM	443
#define	C_TYPEREF_SYM	444
#define	C_NO_TYPE_SYM	445
#define	UNKNOWN_SYM	446
#define	BITSTRING_SYM	447
#define	OCTETSTRING_SYM	448
#define	SEQUENCE_OF_SYM	449
#define	SET_OF_SYM	450
#define	ANY_DEFINED_BY_SYM	451
#define	LOCAL_TYPE_REF_SYM	452
#define	IMPORT_TYPE_REF_SYM	453

#line 28 "core/parse-asn1.y"


#include "snacc.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
//#include <stdio.h>

#include "asn-incl.h"
#include "mem.h"
#include "asn1module.h"
#include "lib-types.h"
#include "snacc-util.h"
#include "exports.h"
#include "lex-stuff.h"


int OidArcNameToNum PROTO ((char *name));
int yylex();
static void yyerror(char* s);


extern FILE* errFileG;		// Defined in snacc.c

/*
 * smallErrG
 *    used for small errors that should prevent code generation but not
 *    prevent the later error checking passes
 */
int smallErrG = FALSE;

/*
 * firstTimeThroughG
 *    used incase the asn1.lex was compiled with flex in which
 *    case the lexical analyzer must be reset for every ASN.1 file
 *    parsed, except the first
 */
static int firstTimeThroughG = TRUE;

/*
 *  modulePtrG
 *    used to hold the parsed value.  The root of the parse tree.
 */
Module *modulePtrG;


/*
 * oidElmtValDefsG
 *    used to hold integer values that are defined as arc numbers
 *    the modules object identifiers.
 * eg. FOO-MODULE { joint-iso-ccitt dod (2) foo (2) 3 2 } DEFINITIONS ::=
 *     would put dod/2 and foo/2 in the oidElmtValDefsG list
 * Note: only some oid's (modules name/import list module names)
 *       are parsed by the yacc code.  The rest are parsed later
 *       due to ambiguities that arise without type info.
// REN -- 9/23/02
ValueDefList *oidElmtValDefsG = NULL;
 */


/*
 * ApplTag
 *   used to hold APPLICATION tags that have been defined in
 *   a module.  This permits checking for the the error of
 *   using the same APPLICATION tag in 1 module.  The
 *   ApplTags list (appTagsG) is emptied for each module.
 */
typedef struct ApplTag
{
    unsigned long lineNo;
    unsigned long tagCode;
    struct ApplTag *next;
} ApplTag;

ApplTag *applTagsG  = NULL;

/*
 * Protos for ApplTag related stuff. These are defined at the
 * end  of this file
 */
void PushApplTag PROTO ((unsigned long tagCode, unsigned long lineNo));
void FreeApplTags();



/*
 * the following are globals to simplify disparity between
 * productions and produced data structure
 */

/*
 * these are used in the ValueRange subtype production
 */
static int      valueRangeUpperEndInclusiveG;
static int      valueRangeLowerEndInclusiveG;

/*
 * used to set exports flag in Type/value defs
 * exportListG holds the explicitly exported elements.
 * see SetExports routine in export.c
 */
ExportElmt *exportListG = NULL;
int         exportsParsedG;


/*
 * globals for the APPLICATION-CONTEXT macro productions
 */
static ValueList *rosAcSymmetricAsesG;
static ValueList *rosAcResponderConsumerOfG;
static ValueList *rosAcInitiatorConsumerOfG;

/*
 * used with MTSAS Extension macro
 * set to NULL for the initial parse.
 */
static AsnBool *mtsasCriticalForSubmissionG = NULL;
static AsnBool *mtsasCriticalForTransferG = NULL;
static AsnBool *mtsasCriticalForDeliveryG = NULL;

/*
 * Asn PORT macro globals
 */
static TypeOrValueList *asnConsumerG;
static TypeOrValueList *asnSupplierG;


/*
 * parseErrCountG
 *   used to prevent too many cascade errors
 */
int parseErrCountG = 0;
#define MAX_ERR 50
#define PARSE_ERROR()\
    parseErrCountG++;\
    modulePtrG->status = MOD_ERROR;\
    if (parseErrCountG > MAX_ERR)\
    {\
        fprintf (errFileG, "Ackkkkk! too many errors - bye!\n");\
        YYABORT;\
    }



#line 182 "core/parse-asn1.y"
typedef union
{
    int              intVal;
    unsigned int     uintVal;
    char            *charPtr;
    Type            *typePtr;
    NamedType       *namedTypePtr;
    NamedTypeList   *namedTypeListPtr;
    Value           *valuePtr;
    NamedValue      *namedValuePtr;
    SubtypeValue    *subtypeValuePtr;
    Subtype         *subtypePtr;
    ModuleId        *moduleId;
    OID             *oidPtr;
    OidList         *oidListPtr;
    TypeDef         *typeDefPtr;
    TypeDefList     *typeDefListPtr;
    ValueDef        *valueDefPtr;
    ValueDefList    *valueDefListPtr;
    ExportElmt      *exportList;
    ImportModule    *importModulePtr;
    ImportModuleList *importModuleListPtr;
    ImportElmt      *importElmtPtr;
    ImportElmtList  *importElmtListPtr;
    Tag             *tagPtr;
    TagList         *tagListPtr;
    Constraint      *constraintPtr;
    ConstraintList  *constraintListPtr;
    InnerSubtype    *innerSubtypePtr;
	ValueRangeEndpoint *valueRangeEndpointPtr;
    ValueList       *valueListPtr;
    TypeOrValueList *typeOrValueListPtr;
    TypeOrValue     *typeOrValuePtr;
    AsnPort         *asnPortPtr;
    AsnPortList     *asnPortListPtr;
    SnaccDirectiveList *directiveList;
	SnaccDirective	*directivePtr;
	SnaccDirectiveEnum directiveEnum;
	enum BasicTypeChoiceId basicTypeChoiceIdEnum;
	CTypeId			cTypeIdEnum;
	AsnBool			boolVal;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>



#define	YYFINAL		834
#define	YYFLAG		-32768
#define	YYNTBASE	201

#define YYTRANSLATE(x) ((unsigned)(x) <= 453 ? yytranslate[x] : 416)

static const short yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,   200,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
    77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
   127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
   147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
   177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
   187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
   197,   198,   199
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     2,    13,    16,    19,    21,    23,    25,    28,
    30,    32,    36,    38,    42,    44,    48,    52,    54,    56,
    58,    60,    65,    69,    73,    75,    77,    79,    82,    84,
    89,    93,    95,    97,    99,   101,   104,   106,   108,   111,
   114,   116,   118,   119,   126,   130,   136,   138,   140,   147,
   152,   154,   156,   158,   160,   162,   164,   166,   168,   170,
   172,   174,   176,   178,   180,   182,   184,   186,   188,   190,
   192,   194,   196,   198,   200,   203,   205,   207,   209,   212,
   214,   216,   218,   223,   225,   229,   234,   239,   241,   243,
   246,   249,   254,   256,   259,   265,   267,   269,   271,   275,
   279,   282,   285,   287,   293,   295,   298,   301,   305,   307,
   309,   313,   315,   322,   328,   330,   333,   335,   337,   339,
   341,   344,   348,   352,   357,   361,   365,   369,   372,   376,
   382,   385,   387,   392,   394,   397,   400,   404,   406,   408,
   413,   415,   422,   424,   428,   432,   435,   439,   443,   448,
   450,   452,   454,   456,   458,   460,   462,   467,   472,   477,
   479,   481,   483,   485,   487,   489,   491,   493,   495,   497,
   499,   501,   503,   505,   507,   509,   511,   513,   516,   521,
   526,   530,   532,   536,   538,   540,   542,   544,   546,   548,
   550,   552,   554,   557,   562,   564,   567,   569,   572,   574,
   576,   578,   580,   583,   586,   590,   594,   596,   598,   600,
   604,   610,   612,   616,   619,   621,   624,   626,   628,   630,
   632,   634,   636,   642,   644,   646,   648,   650,   655,   657,
   659,   661,   663,   665,   667,   669,   670,   674,   676,   678,
   680,   682,   684,   686,   689,   693,   696,   698,   700,   702,
   704,   706,   708,   713,   718,   720,   722,   724,   726,   728,
   730,   732,   734,   735,   737,   739,   741,   744,   748,   752,
   756,   760,   764,   766,   768,   770,   772,   774,   776,   778,
   780,   782,   784,   786,   788,   790,   792,   794,   796,   798,
   800,   802,   804,   806,   808,   810,   812,   814,   816,   818,
   820,   822,   824,   826,   828,   830,   832,   834,   836,   838,
   840,   842,   844,   846,   848,   850,   852,   854,   856,   858,
   860,   862,   864,   866,   868,   870,   872,   874,   876,   878,
   880,   882,   884,   886,   888,   890,   892,   894,   896,   898,
   900,   902,   904,   906,   908,   910,   912,   914,   916,   918,
   920,   922,   924,   926,   928,   930,   932,   934,   936,   938,
   940,   942,   944,   946,   948,   950,   952,   954,   956,   958,
   960,   962,   964,   966,   968,   970,   972,   974,   976,   978,
   980,   982,   984,   986,   988,   991,   996,   999,  1001,  1004,
  1006,  1008,  1010,  1015,  1017,  1022,  1024,  1027,  1030,  1032,
  1037,  1040,  1042,  1045,  1047,  1050,  1052,  1057,  1060,  1062,
  1065,  1069,  1074,  1079,  1081,  1086,  1088,  1090,  1099,  1104,
  1112,  1114,  1120,  1122,  1125,  1132,  1134,  1141,  1143,  1148,
  1150,  1152,  1156,  1163,  1165,  1167,  1169,  1173,  1175,  1177,
  1179,  1183,  1185,  1187,  1192,  1194,  1197,  1199,  1203,  1205,
  1207,  1211,  1213,  1215,  1217,  1219,  1222,  1224,  1227,  1229,
  1232,  1234,  1237,  1240,  1245,  1247,  1249,  1253,  1256,  1258,
  1260,  1262,  1265,  1267,  1272,  1274,  1276,  1279,  1282,  1287,
  1292,  1297,  1299,  1303,  1306,  1308,  1311,  1313,  1317,  1321,
  1323,  1327,  1329,  1333,  1335,  1338,  1342,  1347,  1349,  1352,
  1356,  1361,  1363,  1366,  1369,  1373,  1376,  1379,  1382,  1385,
  1397,  1399,  1401,  1404,  1405,  1408,  1409,  1414,  1415,  1420
};

static const short yyrhs[] = {    -1,
     0,   206,    55,   204,   203,   205,    26,    53,   208,   209,
    54,     0,    56,    28,     0,    40,    28,     0,   324,     0,
    15,     0,   324,     0,   322,   207,     0,   311,     0,   324,
     0,   182,   200,     5,     0,   324,     0,   210,   213,   219,
     0,   324,     0,    58,   211,    74,     0,    58,     1,    74,
     0,   324,     0,   212,     0,   324,     0,   218,     0,   212,
    17,   201,   218,     0,    59,   214,    74,     0,    59,     1,
    74,     0,   324,     0,   215,     0,   324,     0,   215,   216,
     0,   216,     0,   217,    65,   201,   206,     0,   217,    17,
   218,     0,   218,     0,   323,     0,   321,     0,   335,     0,
   219,   220,     0,   220,     0,   221,     0,   221,    74,     0,
     1,    74,     0,   224,     0,   301,     0,     0,     8,    26,
    53,   201,   222,     9,     0,     8,    26,   223,     0,     8,
    26,   322,    16,   223,     0,   323,     0,   335,     0,   323,
    26,   325,   201,   227,   325,     0,   322,    16,   201,   323,
     0,   225,     0,   323,     0,   334,     0,   228,     0,   226,
     0,   277,     0,   230,     0,   231,     0,   272,     0,   237,
     0,   239,     0,   240,     0,   242,     0,   253,     0,   255,
     0,   256,     0,   257,     0,   265,     0,   266,     0,   270,
     0,   273,     0,   235,     0,   236,     0,   271,     0,    33,
    32,     0,   276,     0,   275,     0,   274,     0,   321,   227,
     0,   227,     0,    29,     0,    30,     0,    30,    18,   232,
    19,     0,   233,     0,   232,    17,   233,     0,   321,    20,
   234,    21,     0,   321,    20,   303,    21,     0,    12,     0,
    11,     0,    25,    12,     0,    25,    11,     0,    57,    18,
   232,    19,     0,    60,     0,    31,    32,     0,    31,    32,
    18,   238,    19,     0,   232,     0,    36,     0,    90,     0,
    37,   201,    18,     0,   241,   243,    19,     0,   241,    19,
     0,   244,   325,     0,   245,     0,   252,    17,   325,   201,
   244,     0,   252,     0,    15,   246,     0,    17,    15,     0,
    17,   247,   251,     0,   324,     0,   248,     0,   247,    17,
   248,     0,   252,     0,    22,    22,   250,   249,    23,    23,
     0,   249,    17,   325,   201,   252,     0,   252,     0,   320,
    14,     0,   324,     0,    15,     0,   324,     0,   229,     0,
   229,    45,     0,   229,    46,   310,     0,    47,    38,   227,
     0,   321,    47,    38,   227,     0,    37,    38,   227,     0,
    39,   201,    18,     0,   254,   243,    19,     0,   254,    19,
     0,    39,    38,   227,     0,    41,   201,    18,   258,    19,
     0,   259,   325,     0,   229,     0,   229,    17,   325,   259,
     0,   260,     0,    15,   261,     0,    17,    15,     0,    17,
   262,   251,     0,   324,     0,   263,     0,   262,    17,   325,
   263,     0,   229,     0,    22,    22,   250,   264,    23,    23,
     0,   229,     0,   264,    17,   229,     0,   321,    24,   227,
     0,   267,   227,     0,   267,    40,   227,     0,   267,    56,
   227,     0,    22,   269,   268,    23,     0,   320,     0,   303,
     0,    48,     0,    49,     0,    50,     0,   324,     0,    42,
     0,    42,    70,    71,   321,     0,    33,    32,    34,   227,
     0,    31,    32,    34,   227,     0,    43,     0,    44,     0,
    88,     0,    89,     0,    91,     0,    77,     0,    76,     0,
    75,     0,    85,     0,    86,     0,    87,     0,    78,     0,
    79,     0,    80,     0,    81,     0,    82,     0,    83,     0,
    84,     0,   227,   278,     0,    39,   289,    38,   227,     0,
    37,   289,    38,   227,     0,    20,   279,    21,     0,   280,
     0,   279,    27,   280,     0,   282,     0,   283,     0,   284,
     0,   290,     0,   289,     0,   291,     0,   281,     0,    15,
     0,   302,     0,    61,   227,     0,   285,    16,    16,   286,
     0,   287,     0,   287,    24,     0,   288,     0,    24,   288,
     0,   302,     0,    62,     0,   302,     0,    63,     0,    64,
   278,     0,    65,   278,     0,    66,    67,   292,     0,    66,
    47,   293,     0,   278,     0,   294,     0,   295,     0,    18,
   296,    19,     0,    18,    15,    17,   296,    19,     0,   297,
     0,   296,    17,   297,     0,   321,   298,     0,   298,     0,
   299,   300,     0,   278,     0,   324,     0,    68,     0,    69,
     0,   324,     0,    45,     0,   321,   227,    26,   201,   302,
     0,   305,     0,   303,     0,   304,     0,   321,     0,   322,
    16,   201,   321,     0,   307,     0,   309,     0,   308,     0,
   234,     0,   318,     0,   317,     0,   319,     0,     0,    18,
   306,    10,     0,    51,     0,    52,     0,    72,     0,    73,
     0,    36,     0,   302,     0,   321,   302,     0,    18,   312,
    19,     0,   312,   313,     0,   313,     0,   314,     0,   315,
     0,   316,     0,   320,     0,   321,     0,   321,    20,   314,
    21,     0,   321,    20,   303,    21,     0,     3,     0,     4,
     0,     5,     0,    12,     0,    11,     0,     7,     0,     6,
     0,     6,     0,     0,   326,     0,   324,     0,   327,     0,
   326,   327,     0,   155,   200,   328,     0,   156,   200,   329,
     0,   330,   200,   331,     0,   332,   200,     5,     0,   333,
   200,    12,     0,   192,     0,    29,     0,    30,     0,   193,
     0,   194,     0,    36,     0,    43,     0,    60,     0,    57,
     0,    37,     0,   195,     0,    39,     0,   196,     0,    41,
     0,    42,     0,   197,     0,   198,     0,   199,     0,    77,
     0,    76,     0,    75,     0,    85,     0,    86,     0,    87,
     0,    78,     0,    44,     0,   183,     0,   184,     0,   185,
     0,   186,     0,   187,     0,   188,     0,   189,     0,   190,
     0,   191,     0,   159,     0,   160,     0,   161,     0,   162,
     0,   163,     0,   164,     0,   171,     0,   172,     0,   173,
     0,   174,     0,   175,     0,   176,     0,   181,     0,    51,
     0,    52,     0,   157,     0,   158,     0,   165,     0,   166,
     0,   167,     0,   168,     0,   169,     0,   170,     0,   177,
     0,   179,     0,   180,     0,   178,     0,   336,     0,   343,
     0,   345,     0,   349,     0,   351,     0,   356,     0,   371,
     0,   365,     0,   376,     0,   377,     0,   378,     0,   379,
     0,   380,     0,   385,     0,   389,     0,   398,     0,   400,
     0,   402,     0,   403,     0,   404,     0,   405,     0,   408,
     0,   407,     0,   406,     0,   409,     0,    92,     0,    97,
     0,    99,     0,   101,     0,   103,     0,   107,     0,   116,
     0,   114,     0,   122,     0,   123,     0,   124,     0,   125,
     0,   126,     0,   130,     0,   132,     0,   137,     0,   138,
     0,   141,     0,   140,     0,   142,     0,   143,     0,   144,
     0,   145,     0,   146,     0,   147,     0,    92,   337,     0,
   338,   339,   341,   342,     0,    93,   229,     0,   324,     0,
    94,   340,     0,   324,     0,   229,     0,   324,     0,    95,
    18,   368,    19,     0,   324,     0,    96,    18,   368,    19,
     0,   324,     0,    97,   344,     0,    98,   229,     0,   324,
     0,    99,   346,   347,   348,     0,    93,   229,     0,   324,
     0,    94,   229,     0,   324,     0,   100,   229,     0,   324,
     0,   101,   346,   347,   350,     0,   102,   229,     0,   324,
     0,   103,   352,     0,   103,   353,   354,     0,   104,    18,
   355,    19,     0,   105,    18,   355,    19,     0,   324,     0,
   106,    18,   355,    19,     0,   324,     0,   367,     0,   107,
   357,    99,   227,   101,   227,   358,   363,     0,   108,    18,
   367,    19,     0,   109,   104,    18,   302,    19,   359,   360,
     0,   324,     0,   104,    38,    18,   367,    19,     0,   324,
     0,   361,   362,     0,   110,   113,    38,    18,   367,    19,
     0,   324,     0,   111,   113,    38,    18,   367,    19,     0,
   324,     0,   112,    18,   364,    19,     0,   324,     0,   311,
     0,   364,    17,   311,     0,   114,   115,    65,    18,   366,
    19,     0,   367,     0,   324,     0,   302,     0,   367,    17,
   302,     0,   369,     0,   324,     0,   370,     0,   369,    17,
   370,     0,   227,     0,   302,     0,   116,   229,   372,   373,
     0,   116,     0,    46,   302,     0,   324,     0,   117,   118,
   374,     0,   324,     0,   375,     0,   374,    17,   375,     0,
   120,     0,   121,     0,   119,     0,   122,     0,   122,   227,
     0,   123,     0,   123,   227,     0,   124,     0,   124,   227,
     0,   125,     0,   125,   227,     0,   126,   381,     0,   127,
    18,   382,    19,     0,   324,     0,   383,     0,   382,    17,
   383,     0,   302,   384,     0,   128,     0,   129,     0,   324,
     0,   130,   386,     0,   130,     0,   131,    18,   369,    19,
     0,   387,     0,   388,     0,   387,   388,     0,   388,   387,
     0,   105,    18,   369,    19,     0,   106,    18,   369,    19,
     0,   132,   397,   133,   390,     0,   391,     0,   390,    17,
   391,     0,   392,   393,     0,   397,     0,   397,   134,     0,
   394,     0,   393,    17,   394,     0,   302,   384,   395,     0,
   135,     0,   136,    66,   396,     0,   397,     0,   396,    17,
   397,     0,   302,     0,   137,   399,     0,   137,   399,   227,
     0,   139,    18,   382,    19,     0,   324,     0,   138,   401,
     0,   138,   401,   227,     0,    65,    18,   382,    19,     0,
   324,     0,   141,   337,     0,   140,   344,     0,   142,    98,
   227,     0,   143,   227,     0,   144,   227,     0,   145,   227,
     0,   146,   227,     0,   147,   148,   227,   149,   410,   150,
   411,   412,   413,   414,   415,     0,   321,     0,   321,     0,
   151,   302,     0,     0,   152,   302,     0,     0,   153,    18,
   369,    19,     0,     0,   154,    18,   302,    19,     0,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   481,   483,   488,   525,   527,   528,   535,   540,   548,   557,
   559,   562,   564,   568,   570,   573,   584,   591,   594,   596,
   599,   607,   616,   618,   623,   626,   628,   631,   636,   643,
   654,   665,   678,   680,   681,   694,   696,   699,   701,   702,
   709,   724,   737,   739,   778,   794,   819,   821,   828,   849,
   864,   866,   876,   878,   879,   880,   883,   885,   886,   887,
   888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
   898,   899,   900,   901,   902,   906,   907,   908,   911,   918,
   927,   934,   940,   948,   954,   961,   969,   977,   982,   985,
   990,   995,  1006,  1014,  1020,  1027,  1033,  1040,  1047,  1052,
  1068,  1083,  1097,  1099,  1114,  1122,  1129,  1139,  1149,  1160,
  1162,  1169,  1176,  1182,  1199,  1207,  1209,  1216,  1218,  1221,
  1223,  1228,  1242,  1248,  1259,  1272,  1276,  1291,  1306,  1320,
  1337,  1350,  1356,  1368,  1371,  1377,  1387,  1391,  1402,  1404,
  1411,  1417,  1423,  1429,  1443,  1458,  1484,  1525,  1534,  1552,
  1558,  1566,  1568,  1569,  1570,  1574,  1579,  1587,  1595,  1603,
  1610,  1617,  1622,  1626,  1632,  1637,  1641,  1645,  1649,  1653,
  1657,  1661,  1665,  1669,  1673,  1677,  1681,  1687,  1697,  1710,
  1726,  1733,  1754,  1774,  1776,  1777,  1778,  1779,  1780,  1781,
  1784,  1790,  1798,  1807,  1818,  1825,  1833,  1840,  1848,  1850,
  1857,  1859,  1866,  1876,  1885,  1892,  1900,  1915,  1917,  1920,
  1929,  1939,  1945,  1952,  1958,  1962,  1971,  1973,  1976,  1978,
  1979,  1980,  1992,  2004,  2006,  2009,  2011,  2024,  2039,  2041,
  2042,  2043,  2049,  2057,  2065,  2073,  2074,  2090,  2097,  2106,
  2113,  2123,  2133,  2139,  2148,  2175,  2185,  2190,  2192,  2213,
  2217,  2225,  2230,  2251,  2269,  2273,  2277,  2281,  2289,  2297,
  2301,  2305,  2309,  2316,  2318,  2321,  2327,  2334,  2341,  2347,
  2353,  2359,  2367,  2369,  2370,  2371,  2372,  2373,  2374,  2375,
  2376,  2377,  2378,  2379,  2380,  2381,  2382,  2383,  2384,  2385,
  2386,  2387,  2388,  2389,  2390,  2391,  2392,  2393,  2396,  2398,
  2399,  2400,  2401,  2402,  2403,  2404,  2405,  2408,  2410,  2411,
  2412,  2413,  2414,  2415,  2416,  2417,  2418,  2419,  2420,  2421,
  2424,  2426,  2429,  2431,  2432,  2433,  2434,  2435,  2436,  2437,
  2438,  2439,  2440,  2443,  2453,  2455,  2456,  2457,  2458,  2459,
  2460,  2461,  2462,  2463,  2464,  2465,  2466,  2467,  2468,  2469,
  2470,  2471,  2472,  2473,  2474,  2475,  2476,  2477,  2478,  2481,
  2483,  2484,  2485,  2486,  2487,  2488,  2489,  2490,  2491,  2492,
  2493,  2494,  2495,  2496,  2497,  2498,  2499,  2500,  2501,  2502,
  2503,  2504,  2505,  2506,  2514,  2518,  2534,  2536,  2539,  2541,
  2545,  2547,  2551,  2556,  2561,  2566,  2577,  2591,  2593,  2601,
  2615,  2617,  2621,  2623,  2627,  2629,  2637,  2652,  2654,  2662,
  2671,  2683,  2691,  2696,  2700,  2705,  2709,  2718,  2742,  2750,
  2756,  2765,  2770,  2773,  2777,  2782,  2785,  2790,  2793,  2798,
  2802,  2808,  2820,  2834,  2836,  2839,  2845,  2852,  2854,  2857,
  2863,  2870,  2877,  2889,  2907,  2919,  2921,  2924,  2926,  2930,
  2932,  2935,  2941,  2946,  2959,  2969,  2984,  2993,  3007,  3017,
  3032,  3042,  3063,  3073,  3078,  3081,  3087,  3094,  3103,  3109,
  3114,  3126,  3137,  3145,  3150,  3156,  3162,  3168,  3176,  3183,
  3199,  3206,  3208,  3211,  3215,  3217,  3220,  3222,  3225,  3232,
  3234,  3238,  3240,  3243,  3256,  3266,  3278,  3283,  3292,  3303,
  3316,  3321,  3329,  3341,  3354,  3365,  3377,  3388,  3402,  3412,
  3438,  3460,  3481,  3483,  3486,  3488,  3491,  3496,  3499,  3504
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","BSTRING_SYM",
"HSTRING_SYM","CSTRING_SYM","UCASEFIRST_IDENT_SYM","LCASEFIRST_IDENT_SYM","NAMEDMACRO_SYM",
"MACRODEFBODY_SYM","BRACEBAL_SYM","NUMBER_ERANGE","NUMBER_SYM","SNACC_ATTRIBUTES",
"COLON_SYM","ELLIPSIS_SYM","DOT_SYM","COMMA_SYM","LEFTBRACE_SYM","RIGHTBRACE_SYM",
"LEFTPAREN_SYM","RIGHTPAREN_SYM","LEFTBRACKET_SYM","RIGHTBRACKET_SYM","LESSTHAN_SYM",
"MINUS_SYM","GETS_SYM","BAR_SYM","TAGS_SYM","BOOLEAN_SYM","INTEGER_SYM","BIT_SYM",
"STRING_SYM","OCTET_SYM","CONTAINING_SYM","ENCODED_SYM","NULL_SYM","SEQUENCE_SYM",
"OF_SYM","SET_SYM","IMPLICIT_SYM","CHOICE_SYM","ANY_SYM","OBJECT_IDENTIFIER_SYM",
"RELATIVE_OID_SYM","OPTIONAL_SYM","DEFAULT_SYM","COMPONENTS_SYM","UNIVERSAL_SYM",
"APPLICATION_SYM","PRIVATE_SYM","TRUE_SYM","FALSE_SYM","BEGIN_SYM","END_SYM",
"DEFINITIONS_SYM","EXPLICIT_SYM","ENUMERATED_SYM","EXPORTS_SYM","IMPORTS_SYM",
"REAL_SYM","INCLUDES_SYM","MIN_SYM","MAX_SYM","SIZE_SYM","FROM_SYM","WITH_SYM",
"COMPONENT_SYM","PRESENT_SYM","ABSENT_SYM","DEFINED_SYM","BY_SYM","PLUS_INFINITY_SYM",
"MINUS_INFINITY_SYM","SEMI_COLON_SYM","IA5STRING_SYM","PRINTABLESTRING_SYM",
"NUMERICSTRING_SYM","TELETEXSTRING_SYM","T61STRING_SYM","VIDEOTEXSTRING_SYM",
"VISIBLESTRING_SYM","ISO646STRING_SYM","GRAPHICSTRING_SYM","GENERALSTRING_SYM",
"BMPSTRING_SYM","UNIVERSALSTRING_SYM","UTF8STRING_SYM","GENERALIZEDTIME_SYM",
"UTCTIME_SYM","EXTERNAL_SYM","OBJECTDESCRIPTOR_SYM","OPERATION_SYM","ARGUMENT_SYM",
"RESULT_SYM","ERRORS_SYM","LINKED_SYM","ERROR_SYM","PARAMETER_SYM","BIND_SYM",
"BINDERROR_SYM","UNBIND_SYM","UNBINDERROR_SYM","ASE_SYM","OPERATIONS_SYM","CONSUMERINVOKES_SYM",
"SUPPLIERINVOKES_SYM","AC_SYM","ASES_SYM","REMOTE_SYM","INITIATOR_SYM","RESPONDER_SYM",
"ABSTRACTSYNTAXES_SYM","CONSUMER_SYM","EXTENSIONS_SYM","CHOSEN_SYM","EXTENSION_SYM",
"CRITICAL_SYM","FOR_SYM","DELIVERY_SYM","SUBMISSION_SYM","TRANSFER_SYM","EXTENSIONATTRIBUTE_SYM",
"TOKEN_SYM","TOKENDATA_SYM","SECURITYCATEGORY_SYM","OBJECT_SYM","PORTS_SYM",
"BOXC_SYM","BOXS_SYM","PORT_SYM","ABSTRACTOPS_SYM","REFINE_SYM","AS_SYM","RECURRING_SYM",
"VISIBLE_SYM","PAIRED_SYM","ABSTRACTBIND_SYM","ABSTRACTUNBIND_SYM","TO_SYM",
"ABSTRACTERROR_SYM","ABSTRACTOPERATION_SYM","ALGORITHM_SYM","ENCRYPTED_SYM",
"SIGNED_SYM","SIGNATURE_SYM","PROTECTED_SYM","OBJECTTYPE_SYM","SYNTAX_SYM","ACCESS_SYM",
"STATUS_SYM","DESCRIPTION_SYM","REFERENCE_SYM","INDEX_SYM","DEFVAL_SYM","ATTRIB_ASN1_TYPE_ID",
"ATTRIB_C_TYPE_ID","ATTRIB_C_TYPE_NAME_SYM","ATTRIB_C_FIELD_NAME_SYM","ATTRIB_IS_PDU_SYM",
"ATTRIB_IS_PTR_SYM","ATTRIB_IS_PTR_TYPEDEF_SYM","ATTRIB_IS_PTR_TYPE_REF_SYM",
"ATTRIB_IS_PTR_IN_CHOICE_SYM","ATTRIB_IS_PTR_FOR_OPT_SYM","ATTRIB_OPT_TEST_ROUTINE_SYM",
"ATTRIB_DEFAULT_FIELD_SYM","ATTRIB_PRINT_ROUTINE_SYM","ATTRIB_ENCODE_ROUTINE_SYM",
"ATTRIB_DECODE_ROUTINE_SYM","ATTRIB_FREE_ROUTINE_SYM","ATTRIB_IS_ENC_DEC_SYM",
"ATTRIB_GEN_TYPEDEF_SYM","ATTRIB_GEN_PRINT_ROUTINE_SYM","ATTRIB_GEN_ENCODE_ROUTINE_SYM",
"ATTRIB_GEN_DECODE_ROUTINE_SYM","ATTRIB_GEN_FREE_ROUTINE_SYM","ATTRIB_CHOICE_ID_SYMBOL_SYM",
"ATTRIB_CHOICE_ID_VALUE_SYM","ATTRIB_CHOICE_ID_ENUM_NAME_SYM","ATTRIB_CHOICE_ID_ENUM_FIELD_NAME_SYM",
"ATTRIB_IS_BIG_INT_SYM","ATTRIB_NAMESPACE_SYM","C_CHOICE_SYM","C_LIST_SYM","C_ANY_SYM",
"C_ANYDEFBY_SYM","C_LIB_SYM","C_STRUCT_SYM","C_TYPEDEF_SYM","C_TYPEREF_SYM",
"C_NO_TYPE_SYM","UNKNOWN_SYM","BITSTRING_SYM","OCTETSTRING_SYM","SEQUENCE_OF_SYM",
"SET_OF_SYM","ANY_DEFINED_BY_SYM","LOCAL_TYPE_REF_SYM","IMPORT_TYPE_REF_SYM",
"':'","LineNo","ModuleDefinition","@1","TagDefault","ExtensionDefault","ModuleIdentifier",
"AssignedIdentifier","OptionalSnaccNamespace","ModuleBody","Exports","SymbolsExported",
"ExportSymbolList","Imports","SymbolsImported","SymbolsFromModuleList","SymbolsFromModule",
"SymbolList","Symbol","AssignmentList","AssignmentOrError","Assignment","@2",
"MacroReference","TypeAssignment","ExternalTypeReference","DefinedType","Type",
"BuiltinType","NamedType","BooleanType","IntegerType","NamedNumberList","NamedNumber",
"SignedNumber","EnumeratedType","RealType","BitStringType","NamedBitList","NullType",
"ExternalType","SequenceOpening","SequenceType","ElementTypes","ElementTypeList",
"Extension","ExtensionAdditions","ExtensionAdditionList","ExtensionAddition",
"ExtList","VersionNumber","OptExtMarker","ElementType","SequenceOfType","SetOpening",
"SetType","SetOfType","ChoiceType","AlternativeTypes","AlternativeTypeList",
"ChoiceExtension","ExtAdditionAlternatives","ExtAddList","ExtAddAlternative",
"AltList","SelectionType","TaggedType","Tag","ClassNumber","Class","AnyType",
"OctetContainingType","BitContainingType","ObjectIdentifierType","RelativeOIDType",
"UsefulType","CharStrType","Subtype","SubtypeSpec","SubtypeValueSetList","SubtypeValueSet",
"ExtensibleSubtype","SingleValue","ContainedSubtype","ValueRange","LowerEndPoint",
"UpperEndPoint","LowerEndValue","UpperEndValue","SizeConstraint","PermittedAlphabet",
"InnerTypeConstraints","SingleTypeConstraint","MultipleTypeConstraints","FullSpecification",
"PartialSpecification","TypeConstraints","NamedConstraint","Constraint","ValueConstraint",
"PresenceConstraint","ValueAssignment","Value","DefinedValue","ExternalValueReference",
"BuiltinValue","@3","BooleanValue","SpecialRealValue","NullValue","NamedValue",
"ObjectIdentifierValue","ObjIdComponentList","ObjIdComponent","NumberForm","NameForm",
"NameAndNumberForm","BinaryString","HexString","CharString","number","identifier",
"modulereference","typereference","empty","SnaccDirectives","SnaccDirectiveList",
"SnaccDirective","SnaccDirectiveAsnTypeValue","SnaccDirectiveCTypeValue","SnaccDirectiveBoolType",
"SnaccDirectiveBoolValue","SnaccDirectiveStringType","SnaccDirectiveIntegerType",
"DefinedMacroType","DefinedMacroName","RosOperationMacroType","RosOperationMacroBody",
"RosOpArgument","RosOpResult","RosOpResultType","RosOpErrors","RosOpLinkedOps",
"RosErrorMacroType","RosErrParameter","RosBindMacroType","RosBindArgument","RosBindResult",
"RosBindError","RosUnbindMacroType","RosUnbindError","RosAseMacroType","RosAseSymmetricAse",
"RosAseConsumerInvokes","RosAseSupplierInvokes","RosAseOperationList","RosAcMacroType",
"RosAcNonRoElements","RosAcRoElements","RosAcSymmetricAses","RosAcAsymmetricAses",
"RosAcInitiatorConsumerOf","RosAcResponderConsumerOf","RosAcAbstractSyntaxes",
"OidList","MtsasExtensionsMacroType","PossiblyEmptyValueList","ValueList","PossiblyEmptyTypeOrValueList",
"TypeOrValueList","TypeOrValue","MtsasExtensionMacroType","MtsasExtDefaultVal",
"MtsasExtCritical","MtsasExtCriticalityList","MtsasExtCriticality","MtsasExtensionAttributeMacroType",
"MtsasTokenMacroType","MtsasTokenDataMacroType","MtsasSecurityCategoryMacroType",
"AsnObjectMacroType","AsnPorts","AsnPortList","AsnPort","AsnPortType","AsnPortMacroType",
"AsnOperations","AsnConsumer","AsnSupplier","AsnRefineMacroType","AsnComponentList",
"AsnComponent","AsnObjectSpec","AsnPortSpecList","AsnPortSpec","AsnPortStatus",
"AsnObjectList","AsnObject","AsnAbstractBindMacroType","AsnAbstractBindPorts",
"AsnAbstractUnbindMacroType","AsnAbstractUnbindPorts","AsnAbstractOperationMacroType",
"AsnAbstractErrorMacroType","AfAlgorithmMacroType","AfEncryptedMacroType","AfSignedMacroType",
"AfSignatureMacroType","AfProtectedMacroType","SnmpObjectTypeMacroType","SnmpAccess",
"SnmpStatus","SnmpDescrPart","SnmpReferPart","SnmpIndexPart","SnmpDefValPart", NULL
};
#endif

static const short yyr1[] = {     0,
   201,   203,   202,   204,   204,   204,   205,   205,   206,   207,
   207,   208,   208,   209,   209,   210,   210,   210,   211,   211,
   212,   212,   213,   213,   213,   214,   214,   215,   215,   216,
   217,   217,   218,   218,   218,   219,   219,   220,   220,   220,
   221,   221,   222,   221,   221,   221,   223,   223,   224,   225,
   226,   226,   227,   227,   227,   227,   228,   228,   228,   228,
   228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
   228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
   230,   231,   231,   232,   232,   233,   233,   234,   234,   234,
   234,   235,   236,   237,   237,   238,   239,   240,   241,   242,
   242,   243,   244,   244,   244,   245,   246,   246,   246,   247,
   247,   248,   248,   249,   249,   250,   250,   251,   251,   252,
   252,   252,   252,   252,   253,   254,   255,   255,   256,   257,
   258,   259,   259,   259,   260,   261,   261,   261,   262,   262,
   263,   263,   264,   264,   265,   266,   266,   266,   267,   268,
   268,   269,   269,   269,   269,   270,   270,   271,   272,   273,
   274,   275,   275,   275,   276,   276,   276,   276,   276,   276,
   276,   276,   276,   276,   276,   276,   276,   277,   277,   277,
   278,   279,   279,   280,   280,   280,   280,   280,   280,   280,
   281,   282,   283,   284,   285,   285,   286,   286,   287,   287,
   288,   288,   289,   290,   291,   291,   292,   293,   293,   294,
   295,   296,   296,   297,   297,   298,   299,   299,   300,   300,
   300,   300,   301,   302,   302,   303,   303,   304,   305,   305,
   305,   305,   305,   305,   305,   306,   305,   307,   307,   308,
   308,   309,   310,   310,   311,   312,   312,   313,   313,   313,
   314,   315,   316,   316,   317,   318,   319,   320,   320,   321,
   322,   323,   324,   325,   325,   326,   326,   327,   327,   327,
   327,   327,   328,   328,   328,   328,   328,   328,   328,   328,
   328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
   328,   328,   328,   328,   328,   328,   328,   328,   329,   329,
   329,   329,   329,   329,   329,   329,   329,   330,   330,   330,
   330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
   331,   331,   332,   332,   332,   332,   332,   332,   332,   332,
   332,   332,   332,   333,   334,   334,   334,   334,   334,   334,
   334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
   334,   334,   334,   334,   334,   334,   334,   334,   334,   335,
   335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
   335,   335,   335,   335,   335,   335,   335,   335,   335,   335,
   335,   335,   335,   335,   336,   337,   338,   338,   339,   339,
   340,   340,   341,   341,   342,   342,   343,   344,   344,   345,
   346,   346,   347,   347,   348,   348,   349,   350,   350,   351,
   351,   352,   353,   353,   354,   354,   355,   356,   357,   358,
   358,   359,   359,   360,   361,   361,   362,   362,   363,   363,
   364,   364,   365,   366,   366,   367,   367,   368,   368,   369,
   369,   370,   370,   371,   371,   372,   372,   373,   373,   374,
   374,   375,   375,   375,   376,   376,   377,   377,   378,   378,
   379,   379,   380,   381,   381,   382,   382,   383,   384,   384,
   384,   385,   385,   386,   386,   386,   386,   386,   387,   388,
   389,   390,   390,   391,   392,   392,   393,   393,   394,   395,
   395,   396,   396,   397,   398,   398,   399,   399,   400,   400,
   401,   401,   402,   403,   404,   405,   406,   407,   408,   409,
   410,   411,   412,   412,   413,   413,   414,   414,   415,   415
};

static const short yyr2[] = {     0,
     0,     0,    10,     2,     2,     1,     1,     1,     2,     1,
     1,     3,     1,     3,     1,     3,     3,     1,     1,     1,
     1,     4,     3,     3,     1,     1,     1,     2,     1,     4,
     3,     1,     1,     1,     1,     2,     1,     1,     2,     2,
     1,     1,     0,     6,     3,     5,     1,     1,     6,     4,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     2,     1,     1,     1,     2,     1,
     1,     1,     4,     1,     3,     4,     4,     1,     1,     2,
     2,     4,     1,     2,     5,     1,     1,     1,     3,     3,
     2,     2,     1,     5,     1,     2,     2,     3,     1,     1,
     3,     1,     6,     5,     1,     2,     1,     1,     1,     1,
     2,     3,     3,     4,     3,     3,     3,     2,     3,     5,
     2,     1,     4,     1,     2,     2,     3,     1,     1,     4,
     1,     6,     1,     3,     3,     2,     3,     3,     4,     1,
     1,     1,     1,     1,     1,     1,     4,     4,     4,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
     3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     2,     4,     1,     2,     1,     2,     1,     1,
     1,     1,     2,     2,     3,     3,     1,     1,     1,     3,
     5,     1,     3,     2,     1,     2,     1,     1,     1,     1,
     1,     1,     5,     1,     1,     1,     1,     4,     1,     1,
     1,     1,     1,     1,     1,     0,     3,     1,     1,     1,
     1,     1,     1,     2,     3,     2,     1,     1,     1,     1,
     1,     1,     4,     4,     1,     1,     1,     1,     1,     1,
     1,     1,     0,     1,     1,     1,     2,     3,     3,     3,
     3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     2,     4,     2,     1,     2,     1,
     1,     1,     4,     1,     4,     1,     2,     2,     1,     4,
     2,     1,     2,     1,     2,     1,     4,     2,     1,     2,
     3,     4,     4,     1,     4,     1,     1,     8,     4,     7,
     1,     5,     1,     2,     6,     1,     6,     1,     4,     1,
     1,     3,     6,     1,     1,     1,     3,     1,     1,     1,
     3,     1,     1,     4,     1,     2,     1,     3,     1,     1,
     3,     1,     1,     1,     1,     2,     1,     2,     1,     2,
     1,     2,     2,     4,     1,     1,     3,     2,     1,     1,
     1,     2,     1,     4,     1,     1,     2,     2,     4,     4,
     4,     1,     3,     2,     1,     2,     1,     3,     3,     1,
     3,     1,     3,     1,     2,     3,     4,     1,     2,     3,
     4,     1,     2,     2,     3,     2,     2,     2,     2,    11,
     1,     1,     2,     0,     2,     0,     4,     0,     4,     0
};

static const short yydefact[] = {     0,
   261,     0,   263,   263,     0,     9,    10,    11,     0,     0,
     2,     6,   260,   259,   258,     0,   247,   248,   249,   250,
   251,   252,     5,     4,   263,   245,   246,     0,     7,     0,
     8,     0,   226,     0,   227,     0,     0,   254,   253,     1,
   263,     0,     0,   263,    13,   228,     0,     0,     0,   263,
    18,    12,     0,   262,   360,   361,   362,   363,   364,   365,
   367,   366,   368,   369,   370,   371,   372,   373,   374,   375,
   376,   378,   377,   379,   380,   381,   382,   383,   384,     0,
    19,    21,    34,    33,    20,    35,     3,     0,     0,    25,
    17,    16,     1,     0,     0,    26,    29,     0,    32,    27,
     0,     0,     0,    37,    38,    41,    42,     0,     0,     0,
    24,    23,    28,     0,     1,    40,     0,    36,    39,   262,
   263,    81,    82,     0,     0,    97,     1,     1,     1,   156,
   160,   161,     0,    93,   167,   166,   165,   171,   172,   173,
   174,   175,   176,   177,   168,   169,   170,   162,   163,    98,
   164,   263,   263,   263,   263,   263,     0,     0,   445,   455,
   457,   459,   461,   263,   473,     0,   263,   263,   263,   263,
     0,     0,     0,     0,     0,     0,    51,    55,     0,    54,
    57,    58,    72,    73,    60,    61,    62,     0,    63,    64,
     0,    65,    66,    67,    68,    69,     0,    70,    74,    59,
    71,    78,    77,    76,    56,     0,     0,    52,    53,   335,
   336,   337,   338,   339,   340,   342,   341,   343,   344,   345,
   346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
   358,   357,   356,   359,   263,    22,    31,     0,     1,    45,
     0,    47,    48,   152,   153,   154,     0,   155,     0,    94,
    75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   388,   385,   263,     0,   399,   397,     0,   402,
   263,   263,     0,     0,   414,   410,   263,     0,     0,     0,
    80,   263,     0,   456,   458,   460,   462,     0,   465,   463,
     0,     0,     0,   472,   475,   476,   255,   256,   257,    89,
    88,   236,     0,   242,   238,   239,   240,   241,   232,   494,
   225,   224,   229,   231,   230,   234,   233,   235,     0,     0,
   498,   495,     0,   502,   499,   504,   503,     0,   506,   507,
   508,   509,     0,     0,     1,   178,   263,   101,     0,   120,
     0,   263,   103,   105,     0,   128,     0,     0,     0,   146,
     0,     1,     0,     0,   323,   324,   308,   309,   310,   311,
   312,   313,   325,   326,   327,   328,   329,   330,   314,   315,
   316,   317,   318,   319,   331,   334,   332,   333,   320,   265,
     1,   264,   266,     0,     0,     0,    30,    43,     0,     0,
   151,   150,     0,    84,     0,     0,     0,     0,   125,   203,
    99,     0,   129,   126,     0,     0,     0,     0,   387,   263,
   390,   263,   398,   401,     0,   404,   263,   263,     0,     0,
     0,   416,   411,     0,     0,     0,     0,   447,   263,    79,
     0,     0,     0,     0,   477,   478,     0,    91,    90,     0,
     0,   496,     0,   500,   505,     0,   191,     0,   200,     0,
     0,     0,   182,   190,   184,   185,   186,     0,   195,   188,
   187,   189,   192,     0,     0,   106,   109,     0,   121,     0,
   100,   102,   263,     0,   127,   147,   148,   145,     0,     0,
     0,     0,   267,     0,     0,     0,     0,    46,   149,     0,
    83,     0,    96,     0,   159,   158,   180,   179,   263,   132,
     0,   263,   134,   157,    92,   391,   392,   389,     0,   394,
   263,   403,     0,   406,   400,     0,   409,   407,   436,     0,
   417,     0,     0,     0,     0,   263,   446,     0,   449,   444,
   263,     0,   466,    97,   442,   443,   227,     0,     0,   440,
     0,     0,   237,   481,   482,     0,   485,     0,     0,     0,
   193,   204,     0,     0,   181,     0,     0,   196,   223,   107,
   263,   263,   110,   112,   123,   243,   122,   227,     1,     0,
    50,   274,   275,   278,   282,   284,   286,   287,   279,   298,
   281,   280,   293,   292,   291,   297,   294,   295,   296,   273,
   276,   277,   283,   285,   288,   289,   290,   268,   299,   300,
   301,   302,   303,   304,   305,   306,   307,   269,   263,   321,
   322,   270,   271,   272,    44,    85,     0,     0,    95,     0,
   135,   138,   263,   130,   131,   263,     0,   396,   386,   405,
   408,   412,     0,   413,     0,   419,     0,   435,     0,   434,
     0,   469,   470,   471,   468,     0,   464,     1,     0,   479,
   480,   474,     0,   263,   484,   487,   486,   497,   501,   511,
     0,   263,   206,   208,   209,   207,   205,   183,     0,   263,
   118,     0,   108,   119,   244,     0,   124,    49,    86,    87,
   136,   263,   141,   263,   139,     0,   439,     0,   438,   263,
   437,   415,   263,   433,   454,   452,   453,   448,   450,   467,
     0,   441,   483,     0,     0,     0,     0,   217,     0,   212,
   215,   263,   263,   218,     0,   202,   194,   197,   201,     0,
     0,   117,   111,   104,   263,   263,   137,   133,   393,     0,
     0,   421,   263,     0,   490,     0,   489,   488,   512,   514,
   263,   263,   210,   222,   219,   220,   216,   221,   214,   198,
     0,   115,   116,     0,     0,   395,     0,     0,   430,   418,
   451,     0,     0,   516,     0,   213,   263,     0,   143,     0,
   140,     0,     0,   491,   492,   513,     0,   518,   211,     1,
   113,     0,     0,     0,   431,     0,     0,   515,     0,   520,
     0,   144,   142,   263,     0,   429,   493,     0,     0,   510,
   114,     0,   423,   263,   432,     0,     0,     0,     0,   426,
   420,   263,   517,     0,     0,     0,     0,   428,   424,   519,
     0,     0,     0,   422,     0,     0,     0,     0,   425,     0,
   427,     0,     0,     0
};

static const short yydefgoto[] = {    42,
   832,    25,    11,    30,     2,     6,    44,    49,    50,    80,
    81,    89,    95,    96,    97,    98,    99,   103,   104,   105,
   487,   240,   106,   177,   178,   281,   180,   340,   181,   182,
   393,   394,   309,   183,   184,   185,   494,   186,   187,   188,
   189,   341,   342,   343,   466,   562,   563,   751,   720,   673,
   344,   190,   191,   192,   193,   194,   501,   502,   503,   621,
   684,   685,   770,   195,   196,   197,   390,   247,   198,   199,
   200,   201,   202,   203,   204,   205,   336,   452,   453,   454,
   455,   456,   457,   458,   717,   459,   718,   460,   461,   462,
   667,   663,   664,   665,   709,   710,   711,   712,   747,   107,
   519,   311,    33,   312,   437,   313,   314,   315,   567,     7,
    16,    17,    18,    19,    20,   316,   317,   318,    21,    35,
   207,   208,   380,   381,   382,   383,   598,   608,   384,   612,
   385,   386,   209,    86,   210,   264,   265,   412,   508,   511,
   629,   211,   268,   212,   271,   417,   515,   213,   518,   214,
   276,   277,   423,   520,   215,   279,   733,   804,   811,   812,
   819,   760,   786,   216,   639,   521,   688,   689,   540,   217,
   429,   530,   698,   699,   218,   219,   220,   221,   222,   290,
   532,   533,   645,   223,   294,   295,   296,   224,   544,   545,
   546,   655,   656,   737,   774,   547,   225,   322,   226,   325,
   227,   228,   229,   230,   231,   232,   233,   234,   661,   740,
   764,   778,   790,   800
};

static const short yypact[] = {    30,
-32768,   -16,    43,    13,   103,-32768,-32768,-32768,    42,    50,
-32768,-32768,-32768,-32768,-32768,    93,-32768,-32768,-32768,-32768,
-32768,    69,-32768,-32768,    67,-32768,-32768,   172,-32768,    72,
-32768,    90,-32768,   101,-32768,   130,    96,-32768,-32768,-32768,
   -30,   161,   -39,   157,-32768,-32768,   235,   462,   166,   170,
   180,-32768,   171,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   194,
   255,-32768,-32768,-32768,-32768,-32768,-32768,   992,   123,-32768,
-32768,-32768,-32768,   201,   203,  3046,-32768,    10,-32768,-32768,
   217,   256,    23,-32768,   219,-32768,-32768,  2863,   266,  3046,
-32768,-32768,-32768,  3046,-32768,-32768,  3102,-32768,-32768,   278,
   110,-32768,   279,   264,   269,-32768,   -12,     3,-32768,   234,
-32768,-32768,   285,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   212,   209,   213,   213,    22,   202,   196,  2863,  2863,
  2863,  2863,  2863,   185,   -71,   206,   175,   250,   209,   212,
   225,  2863,  2863,  2863,  2863,   176,-32768,-32768,    70,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1391,-32768,-32768,
  1533,-32768,-32768,-32768,-32768,-32768,  1660,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   298,   311,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,  3151,-32768,-32768,    30,-32768,-32768,
   312,-32768,-32768,-32768,-32768,-32768,   172,-32768,   161,    58,
   295,  2863,   310,   313,   297,  2863,   315,   299,   322,   270,
   161,  2863,-32768,-32768,   248,  2863,-32768,-32768,  2863,-32768,
   252,   252,   329,   330,-32768,-32768,   245,   331,   253,   288,
   310,   316,  2198,   310,   310,   310,   310,   338,-32768,-32768,
   339,   340,   342,-32768,   257,   259,-32768,-32768,-32768,-32768,
-32768,-32768,   164,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   228,   349,
-32768,  2863,   350,-32768,  2863,-32768,-32768,  2863,   310,   310,
   310,   310,  2863,   447,-32768,-32768,   352,-32768,   332,   154,
   355,  3151,-32768,   354,  1787,-32768,   357,  2863,  2863,   310,
  2863,-32768,   178,   181,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,  3151,-32768,   182,   183,   184,-32768,-32768,  3158,   362,
-32768,-32768,    62,-32768,   360,   161,  2863,  2863,   310,-32768,
-32768,  2863,   310,-32768,  2863,  2340,   161,    82,-32768,  2863,
-32768,   293,-32768,-32768,  2863,-32768,   289,   290,   206,   206,
   372,-32768,-32768,   206,  2863,   373,   206,-32768,   277,   310,
   206,  1249,  1249,  1249,-32768,-32768,   387,-32768,-32768,   206,
   206,   310,   206,   310,   310,    -7,-32768,  2863,-32768,   310,
    -9,    92,-32768,-32768,-32768,-32768,-32768,   382,   375,-32768,
-32768,-32768,    24,   206,  1929,-32768,-32768,  2863,-32768,   206,
-32768,-32768,  3151,   364,-32768,   310,   310,   310,   394,   822,
   476,  2863,-32768,   138,   398,   392,   396,-32768,-32768,   161,
-32768,   144,   390,   389,   310,   310,   310,   310,   395,   397,
   399,  3151,-32768,-32768,-32768,-32768,-32768,-32768,   393,-32768,
   317,-32768,  2863,-32768,-32768,  2863,-32768,-32768,-32768,   402,
   400,   403,   206,   145,    -4,   206,-32768,   305,-32768,-32768,
    79,   174,-32768,-32768,   310,-32768,   298,   409,   186,-32768,
   187,   197,-32768,   420,-32768,   206,   281,   204,   208,   161,
   310,-32768,   408,   310,-32768,   447,   422,-32768,-32768,-32768,
     1,   211,-32768,-32768,   310,-32768,-32768,   206,-32768,  2863,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   884,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   418,   423,-32768,  2482,
-32768,-32768,  3151,-32768,-32768,  1249,   427,-32768,-32768,-32768,
-32768,-32768,   206,-32768,   429,-32768,  2863,-32768,   430,   400,
    76,-32768,-32768,-32768,-32768,   206,-32768,-32768,  1249,-32768,
-32768,-32768,   206,    79,   438,-32768,-32768,-32768,-32768,-32768,
   296,    73,-32768,-32768,-32768,-32768,-32768,-32768,   129,   243,
-32768,  2609,-32768,-32768,-32768,  2071,   310,-32768,-32768,-32768,
-32768,    15,-32768,   215,-32768,  2340,-32768,   437,   440,  1249,
-32768,-32768,    -6,-32768,-32768,-32768,-32768,   443,-32768,-32768,
   274,-32768,-32768,   150,   206,   161,   449,-32768,   218,-32768,
-32768,   -13,   310,-32768,   314,-32768,-32768,-32768,-32768,  2736,
   457,-32768,-32768,-32768,   243,  3151,-32768,-32768,-32768,   454,
   370,-32768,   304,    76,-32768,   410,-32768,-32768,-32768,   324,
    37,    37,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   120,-32768,-32768,  2863,  2990,-32768,   459,   460,-32768,-32768,
-32768,   206,   206,   328,   224,-32768,  3151,   463,-32768,   121,
-32768,   206,    43,   465,-32768,-32768,   206,   335,-32768,-32768,
-32768,  2863,   468,   474,-32768,   229,   206,-32768,   477,   343,
  2736,-32768,-32768,   401,    43,-32768,-32768,  1249,   478,-32768,
-32768,   456,-32768,   391,-32768,   232,   206,   482,   405,-32768,
-32768,   404,-32768,   483,   206,   466,   411,-32768,-32768,-32768,
   233,   485,   469,-32768,   206,   488,   244,   206,-32768,   247,
-32768,   510,   514,-32768
};

static const short yypgoto[] = {   -82,
-32768,-32768,-32768,-32768,   283,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,   432,-32768,   -23,-32768,   413,-32768,
-32768,   136,-32768,-32768,-32768,   -89,-32768,  -141,-32768,-32768,
  -251,    40,    39,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,   341,  -143,-32768,-32768,-32768,  -138,-32768,  -190,  -145,
  -453,-32768,-32768,-32768,-32768,-32768,-32768,  -146,-32768,-32768,
-32768,  -214,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  -252,-32768,   -14,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,  -172,   160,-32768,-32768,
-32768,-32768,-32768,-32768,  -194,  -192,  -162,-32768,-32768,-32768,
   188,   -25,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -753,
-32768,   536,   525,-32768,-32768,-32768,-32768,-32768,  -241,   522,
     0,     6,    18,  -325,-32768,   173,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  -113,-32768,   388,-32768,-32768,-32768,-32768,
-32768,-32768,   406,-32768,   407,   300,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,  -405,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,  -419,  -133,  -425,   -78,-32768,
-32768,-32768,-32768,  -167,-32768,-32768,-32768,-32768,-32768,-32768,
  -170,   -73,   -80,-32768,-32768,   286,   282,-32768,-32768,   -72,
-32768,-32768,  -122,-32768,-32768,  -164,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768
};


#define	YYLAST		3332


static const short yytable[] = {     3,
   400,   319,    32,   243,   524,   392,   539,   541,   542,   408,
   110,   564,   334,   334,   522,   334,   472,   282,   179,   785,
     8,    12,   670,   101,    82,   252,   114,    36,    54,    13,
   102,   744,   238,   291,   292,     1,   725,   553,     4,  -199,
   256,   805,    31,    13,   254,   257,   259,  -199,   244,   245,
   246,   253,     9,    84,   745,   746,   334,   554,    45,   293,
     5,    51,   244,   245,   246,    85,   253,    90,    10,    23,
   284,   285,   286,   287,   115,   396,   -14,    24,   490,    13,
   491,    29,   329,   330,   331,   332,   236,   707,    28,   334,
   237,   397,   334,    84,   109,   335,   637,    37,   490,    13,
   505,    84,   731,    14,    15,   100,   640,   350,   109,    13,
    38,    26,   555,    14,    15,    84,   241,   635,   556,    84,
   409,    39,   242,   101,   413,   273,   274,   414,    54,    13,
   102,   297,   298,   299,     1,    13,   767,   782,   248,   300,
   301,   550,   768,   783,   493,    40,   302,   569,    41,     1,
    13,    43,   715,   303,   300,   301,   388,   244,   245,   246,
    47,   633,   399,   636,   304,    36,   403,    13,   303,   263,
   267,   270,   270,   275,   438,   439,   625,     1,    13,   305,
   306,   289,    14,    15,   321,   324,   267,   263,   610,   611,
   646,   716,   647,   430,   695,   696,   697,   552,   469,   470,
   307,   308,   649,   649,   650,   651,   642,   643,   297,   298,
   299,     1,    13,   649,    48,   652,   300,   301,   564,    87,
   646,   391,   658,   302,   646,   671,   659,   672,    88,   671,
   303,   726,   442,   -15,   742,   444,   743,     3,   445,    52,
   742,   304,   779,   446,    91,   795,    36,   796,   649,   633,
   813,   824,   464,    14,    15,   430,   305,   306,   476,   477,
   633,   478,   829,   633,   500,   831,   752,    92,   506,   479,
   548,    93,   549,   512,   111,   243,   112,   307,   308,    54,
    13,   117,   411,   678,   735,   736,   255,   258,   416,   416,
   116,   235,   119,  -261,   422,   250,   249,   686,   482,   428,
   251,   666,   261,   260,   262,   269,   266,   495,   496,   278,
   280,   288,   497,   320,   323,   498,   297,   298,   299,     1,
    13,   351,   328,   333,   300,   301,   352,   389,   398,   334,
   401,   302,   404,    36,   402,   525,   405,   801,   303,   406,
   407,   410,   535,   535,   535,   415,   419,   420,   424,   304,
   421,   425,   426,   310,   467,   431,   432,   433,   551,   434,
   440,   427,   292,   291,   305,   306,   441,   443,   465,   468,
   473,   630,   806,   471,   631,   475,   716,   480,   565,   492,
   481,   484,   485,   486,   489,   307,   308,   509,   513,   523,
   526,   516,   609,   528,   242,   821,   543,   557,   558,    54,
   755,   570,   613,   614,   615,   827,   490,   619,   830,   708,
   626,   620,   627,   623,   657,   758,   633,   624,    36,    36,
   632,   634,   641,    36,   648,   662,    36,   507,   721,   510,
    36,   538,   538,   538,   514,   517,   653,   669,   679,    36,
    36,   780,    36,   680,   690,   706,   529,   692,   694,   297,
   298,   299,     1,    13,   705,   729,   649,   300,   301,   734,
   708,   447,    53,    36,   302,   741,   618,    54,    13,    36,
   753,   303,   756,   757,   763,   762,   772,   773,   683,   777,
   677,   787,   304,   721,   571,   781,   676,   789,   708,   708,
   793,    36,   794,   808,   798,   807,   799,   305,   306,   815,
   809,   820,   825,   822,   802,   828,   826,   448,   449,   833,
   253,   450,   451,   834,   817,   118,   622,   816,   307,   308,
   387,   463,    36,   823,   488,    36,    22,   113,   628,   616,
   617,   347,   724,   723,   754,  -263,   535,    22,   727,   728,
   771,   668,   750,   638,   500,    36,   765,   693,   644,   766,
   749,    27,    34,    55,   483,    36,   730,   327,    56,   535,
    57,   272,    58,    46,    59,   701,   761,    36,    60,    83,
   702,   418,   700,   704,   326,    61,   435,    62,   248,   674,
   703,   436,   738,    63,    64,    65,    66,    67,     0,     0,
     0,    68,     0,    69,     0,     0,     0,   775,    70,    71,
   535,    72,    73,    74,    75,    76,    77,    78,    79,    83,
   108,     0,   769,   683,   527,     0,     0,    83,   531,   536,
   536,   536,   797,     0,   108,   538,     0,   310,   531,   206,
   531,    83,    36,     0,     0,    83,     0,     0,     0,     0,
   792,     0,     0,   687,     0,    36,     0,     0,   538,     0,
     0,   559,    36,     0,     0,     0,     0,   566,   599,   600,
   601,   602,   603,   604,   605,   606,   607,     0,    36,     0,
     0,   644,     0,     0,     0,     0,     0,     0,     0,   714,
   283,   206,   206,   206,   206,     0,     0,   722,     0,   538,
     0,     0,     0,   206,   206,   206,   206,   791,     0,   248,
     0,   674,     0,     0,    36,     0,   571,   687,   535,   345,
   732,     0,   345,     0,    36,     0,     0,     0,   206,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   748,
   714,     0,     0,   654,     0,     0,     0,     0,     0,     0,
     0,     0,   722,   463,     0,     0,     0,     0,     0,     0,
   759,     0,     0,     0,     0,   675,     0,     0,   714,   714,
     0,    36,    36,     0,     0,     0,     0,     0,     0,     0,
   395,    36,     0,   206,     0,     0,    36,   206,     0,     0,
     0,     0,   395,   283,     0,     0,    36,   283,     0,     0,
   283,     0,     0,     0,     0,     0,     0,   538,     0,     0,
     0,     0,     0,     0,   206,     0,    36,     0,     0,     0,
     0,   803,     0,   536,    36,     0,     0,     0,     0,     0,
   691,   810,     0,     0,    36,     0,     0,    36,     0,   818,
     0,     0,     0,   531,     0,     0,   536,     0,     0,     0,
   310,     0,     0,   206,     0,     0,   206,     0,     0,   206,
   572,   573,     0,     0,   206,     0,   719,   574,   575,     0,
   576,     0,   577,   578,   579,   580,   206,     0,     0,   206,
   206,     0,   206,     0,     0,     0,     0,   536,   581,     0,
     0,   582,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   654,     0,     0,     0,   583,   584,   585,   586,
     0,     0,   719,   334,     0,     0,   587,   588,   589,     0,
     0,     0,     0,     0,     0,     0,     0,   395,   206,   206,
     0,     0,     0,   206,     0,     0,   206,   283,   504,     0,
     0,   283,     0,     0,     0,     0,   283,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   206,     0,     0,   310,
   776,     0,     0,   537,   537,   537,     0,     0,     0,   784,
     0,     0,     0,     0,   788,     0,     0,     0,     0,   206,
     0,     0,     0,     0,   310,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   536,   345,     0,     0,   206,
     0,   568,    94,     0,   814,     0,     0,    54,    13,     0,
     0,     0,     0,   206,     0,     0,     0,     0,     0,     0,
     0,   395,     0,   590,   591,   592,   593,   594,   595,   596,
   597,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   283,     0,     0,   283,   353,   354,
   355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
   365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
   375,   376,   377,   378,   379,  -263,     0,     0,     0,     0,
     0,   660,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    55,     0,     0,     0,     0,    56,     0,
    57,   206,    58,     0,    59,     0,     0,     0,    60,     0,
     0,     0,     0,     0,     0,    61,     0,    62,     0,     0,
     0,     0,     0,    63,    64,    65,    66,    67,     0,     0,
     0,    68,     0,    69,     0,     0,     0,     0,    70,    71,
     0,    72,    73,    74,    75,    76,    77,    78,    79,     0,
     0,   283,     0,     0,     0,     0,     0,   537,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   713,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   345,     0,     0,     0,   345,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   283,     0,     0,
     0,   537,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    46,     0,     0,     0,     0,   739,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   297,   298,   299,   120,    13,     0,     0,     0,   300,
   301,     0,   713,   713,     0,     0,   302,     0,     0,     0,
   121,     0,     0,   303,     0,   283,   283,   122,   123,   124,
     0,   125,     0,     0,   534,   127,     0,   128,     0,   129,
   130,   131,   132,     0,     0,     0,     0,     0,     0,   305,
   306,     0,     0,   283,     0,   133,     0,     0,   134,     0,
     0,     0,   345,     0,     0,     0,     0,     0,     0,   537,
   307,   308,     0,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,     0,     0,     0,     0,   153,     0,   154,     0,   155,
     0,   156,     0,     0,     0,   157,     0,     0,     0,     0,
     0,     0,   158,     0,   159,     0,     0,     0,     0,     0,
   160,   161,   162,   163,   164,     0,     0,     0,   165,     0,
   166,     0,     0,     0,     0,   167,   168,     0,   169,   170,
   171,   172,   173,   174,   175,   176,   120,    13,     0,     0,
     0,     0,     0,     0,     0,   337,     0,     0,     0,   338,
     0,     0,   121,     0,     0,     0,     0,     0,     0,   122,
   123,   124,     0,   125,     0,     0,   126,   127,     0,   128,
     0,   129,   130,   131,   132,     0,     0,   339,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   133,     0,     0,
   134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   135,   136,   137,   138,   139,
   140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
   150,   151,   152,     0,     0,     0,     0,   153,     0,   154,
     0,   155,     0,   156,     0,     0,     0,   157,     0,     0,
     0,     0,     0,     0,   158,     0,   159,     0,     0,     0,
     0,     0,   160,   161,   162,   163,   164,     0,     0,     0,
   165,     0,   166,     0,     0,     0,     0,   167,   168,     0,
   169,   170,   171,   172,   173,   174,   175,   176,   120,    13,
     0,     0,     0,     0,     0,     0,     0,   337,     0,     0,
     0,   346,     0,     0,   121,     0,     0,     0,     0,     0,
     0,   122,   123,   124,     0,   125,     0,     0,   126,   127,
     0,   128,     0,   129,   130,   131,   132,     0,     0,   339,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   133,
     0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   135,   136,   137,
   138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
   148,   149,   150,   151,   152,     0,     0,     0,     0,   153,
     0,   154,     0,   155,     0,   156,     0,     0,     0,   157,
     0,     0,     0,     0,     0,     0,   158,     0,   159,     0,
     0,     0,     0,     0,   160,   161,   162,   163,   164,     0,
     0,     0,   165,     0,   166,   120,    13,     0,     0,   167,
   168,     0,   169,   170,   171,   172,   173,   174,   175,   176,
     0,   121,     0,     0,     0,     0,     0,     0,   122,   123,
   124,     0,   125,     0,     0,   126,   127,     0,   128,   348,
   129,   130,   131,   132,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   349,   133,     0,     0,   134,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,     0,     0,     0,     0,   153,     0,   154,     0,
   155,     0,   156,     0,     0,     0,   157,     0,     0,     0,
     0,     0,     0,   158,     0,   159,     0,     0,     0,     0,
     0,   160,   161,   162,   163,   164,     0,     0,     0,   165,
     0,   166,   120,    13,     0,     0,   167,   168,     0,   169,
   170,   171,   172,   173,   174,   175,   176,     0,   121,     0,
   351,     0,     0,     0,     0,   122,   123,   124,     0,   125,
     0,     0,   126,   127,     0,   128,     0,   129,   130,   131,
   132,     0,     0,   474,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   133,     0,     0,   134,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   135,   136,   137,   138,   139,   140,   141,   142,   143,
   144,   145,   146,   147,   148,   149,   150,   151,   152,     0,
     0,     0,     0,   153,     0,   154,     0,   155,     0,   156,
     0,     0,     0,   157,     0,     0,     0,     0,     0,     0,
   158,     0,   159,     0,     0,     0,     0,     0,   160,   161,
   162,   163,   164,     0,     0,     0,   165,     0,   166,     0,
     0,     0,     0,   167,   168,     0,   169,   170,   171,   172,
   173,   174,   175,   176,   120,    13,     0,     0,     0,     0,
     0,     0,     0,   560,     0,     0,     0,     0,     0,     0,
   561,     0,     0,     0,     0,     0,     0,   122,   123,   124,
     0,   125,     0,     0,   126,   127,     0,   128,     0,   129,
   130,   131,   132,     0,     0,   339,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   133,     0,     0,   134,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,     0,     0,     0,     0,   153,     0,   154,     0,   155,
     0,   156,     0,     0,     0,   157,     0,     0,     0,     0,
     0,     0,   158,     0,   159,     0,     0,     0,     0,     0,
   160,   161,   162,   163,   164,     0,     0,     0,   165,     0,
   166,     0,     0,     0,     0,   167,   168,     0,   169,   170,
   171,   172,   173,   174,   175,   176,   120,    13,     0,     0,
     0,     0,     0,     0,     0,   337,     0,     0,     0,     0,
     0,     0,   121,     0,     0,     0,     0,     0,     0,   122,
   123,   124,     0,   125,     0,     0,   126,   127,     0,   128,
     0,   129,   130,   131,   132,     0,     0,   339,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   133,     0,     0,
   134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   135,   136,   137,   138,   139,
   140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
   150,   151,   152,     0,     0,     0,     0,   153,     0,   154,
     0,   155,     0,   156,     0,     0,     0,   157,     0,     0,
     0,     0,     0,     0,   158,     0,   159,     0,     0,     0,
     0,     0,   160,   161,   162,   163,   164,     0,     0,     0,
   165,     0,   166,   120,    13,     0,     0,   167,   168,     0,
   169,   170,   171,   172,   173,   174,   175,   176,     0,   121,
     0,   351,     0,     0,     0,     0,   122,   123,   124,     0,
   125,     0,     0,   126,   127,     0,   128,     0,   129,   130,
   131,   132,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   133,     0,     0,   134,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   135,   136,   137,   138,   139,   140,   141,   142,
   143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     0,     0,     0,     0,   153,     0,   154,     0,   155,     0,
   156,     0,     0,     0,   157,     0,     0,     0,     0,     0,
     0,   158,     0,   159,     0,     0,     0,     0,     0,   160,
   161,   162,   163,   164,     0,     0,     0,   165,     0,   166,
     0,     0,     0,     0,   167,   168,     0,   169,   170,   171,
   172,   173,   174,   175,   176,   120,    13,     0,     0,     0,
     0,     0,     0,     0,   499,     0,     0,     0,     0,     0,
     0,   121,     0,     0,     0,     0,     0,     0,   122,   123,
   124,     0,   125,     0,     0,   126,   127,     0,   128,     0,
   129,   130,   131,   132,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   133,     0,     0,   134,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,     0,     0,     0,     0,   153,     0,   154,     0,
   155,     0,   156,     0,     0,     0,   157,     0,     0,     0,
     0,     0,     0,   158,     0,   159,     0,     0,     0,     0,
     0,   160,   161,   162,   163,   164,     0,     0,     0,   165,
     0,   166,     0,     0,     0,     0,   167,   168,     0,   169,
   170,   171,   172,   173,   174,   175,   176,   120,    13,     0,
     0,     0,     0,     0,     0,     0,   681,     0,     0,     0,
     0,     0,     0,   682,     0,     0,     0,     0,     0,     0,
   122,   123,   124,     0,   125,     0,     0,   126,   127,     0,
   128,     0,   129,   130,   131,   132,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
     0,   134,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
   139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
   149,   150,   151,   152,     0,     0,     0,     0,   153,     0,
   154,     0,   155,     0,   156,     0,     0,     0,   157,     0,
     0,     0,     0,     0,     0,   158,     0,   159,     0,     0,
     0,     0,     0,   160,   161,   162,   163,   164,     0,     0,
     0,   165,     0,   166,   120,    13,     0,     0,   167,   168,
     0,   169,   170,   171,   172,   173,   174,   175,   176,     0,
   561,     0,     0,     0,     0,     0,     0,   122,   123,   124,
     0,   125,     0,     0,   126,   127,     0,   128,     0,   129,
   130,   131,   132,     0,     0,   339,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   133,     0,     0,   134,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   135,   136,   137,   138,   139,   140,   141,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,     0,     0,     0,     0,   153,     0,   154,     0,   155,
     0,   156,     0,     0,     0,   157,     0,     0,     0,     0,
     0,     0,   158,     0,   159,     0,     0,     0,     0,     0,
   160,   161,   162,   163,   164,     0,     0,     0,   165,     0,
   166,   120,    13,     0,     0,   167,   168,     0,   169,   170,
   171,   172,   173,   174,   175,   176,     0,   121,     0,     0,
     0,     0,     0,     0,   122,   123,   124,     0,   125,     0,
     0,   126,   127,     0,   128,     0,   129,   130,   131,   132,
     0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   133,     0,     0,   134,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
   145,   146,   147,   148,   149,   150,   151,   152,     0,     0,
     0,     0,   153,     0,   154,     0,   155,     0,   156,     0,
     0,     0,   157,     0,     0,     0,     0,     0,     0,   158,
     0,   159,     0,     0,     0,     0,     0,   160,   161,   162,
   163,   164,     0,     0,     0,   165,     0,   166,   120,    13,
     0,     0,   167,   168,     0,   169,   170,   171,   172,   173,
   174,   175,   176,     0,   121,     0,     0,     0,     0,     0,
     0,   122,   123,   124,     0,   125,     0,     0,   126,   127,
     0,   128,     0,   129,   130,   131,   132,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   133,
     0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   135,   136,   137,
   138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
   148,   149,   150,   151,   152,     0,     0,     0,     0,   153,
     0,   154,     0,   155,     0,   156,     0,     0,     0,   157,
     0,     0,     0,     0,     0,     0,   158,     0,   159,     0,
     0,     0,     0,     0,   160,   161,   162,   163,   164,     0,
     0,     0,   165,     0,   166,   120,    13,     0,     0,   167,
   168,     0,   169,   170,   171,   172,   173,   174,   175,   176,
     0,   682,     0,     0,     0,     0,     0,     0,   122,   123,
   124,     0,   125,     0,     0,   126,   127,     0,   128,     0,
   129,   130,   131,   132,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   133,     0,     0,   134,
     0,    54,    13,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
   141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
   151,   152,     0,     0,     0,     0,   153,     0,   154,     0,
   155,     0,   156,     0,     0,     0,   157,     0,     0,     0,
     0,     0,     0,   158,     0,   159,     0,   120,     0,     0,
     0,   160,   161,   162,   163,   164,     0,     0,     0,   165,
     0,   166,     0,     0,     0,     0,   167,   168,     0,   169,
   170,   171,   172,   173,   174,   175,   176,    55,     0,     0,
     0,     0,    56,     0,    57,     0,    58,     0,    59,     0,
     0,     0,    60,     0,   239,     0,     0,     0,     0,    61,
     0,    62,     0,    54,     0,     0,     0,    63,    64,    65,
    66,    67,     0,     0,     0,    68,     0,    69,     0,     0,
     0,     0,    70,    71,     0,    72,    73,    74,    75,    76,
    77,    78,    79,    55,     0,     0,     0,     0,    56,     0,
    57,     0,    58,     0,    59,     0,     0,     0,    60,     0,
     0,     0,     0,     0,     0,    61,     0,    62,     0,     0,
     0,     0,     0,    63,    64,    65,    66,    67,     0,     0,
     0,    68,     0,    69,     0,     0,     0,     0,    70,    71,
     0,    72,    73,    74,    75,    76,    77,    78,    79,    55,
     0,     0,     0,     0,    56,     0,    57,     0,    58,     0,
    59,     0,     0,     0,    60,     0,     0,     0,     0,     0,
     0,    61,     0,    62,     0,     0,     0,     0,     0,    63,
    64,    65,    66,    67,     0,     0,     0,    68,     0,    69,
     0,     0,     0,     0,    70,    71,     0,    72,    73,    74,
    75,    76,    77,    78,    79,   353,   354,   355,   356,   357,
   358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
   368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
   378,   379
};

static const short yycheck[] = {     0,
   253,   166,    28,   117,   424,   247,   432,   433,   434,   261,
    93,   465,    20,    20,   420,    20,   342,   159,   108,   773,
     3,     4,    22,     1,    48,    38,    17,    28,     6,     7,
     8,    45,   115,   105,   106,     6,    22,    47,    55,    16,
    38,   795,    25,     7,   127,   128,   129,    24,    48,    49,
    50,    64,    40,    48,    68,    69,    20,    67,    41,   131,
    18,    44,    48,    49,    50,    48,    64,    50,    56,    28,
   160,   161,   162,   163,    65,    18,    54,    28,    17,     7,
    19,    15,   172,   173,   174,   175,   110,    15,    20,    20,
   114,    34,    20,    88,    89,    26,   101,    26,    17,     7,
    19,    96,   109,    11,    12,    88,   526,   197,   103,     7,
    21,    19,    21,    11,    12,   110,   117,   523,    27,   114,
   262,    21,   117,     1,   266,   104,   105,   269,     6,     7,
     8,     3,     4,     5,     6,     7,    17,    17,   121,    11,
    12,   149,    23,    23,   396,    16,    18,   473,    53,     6,
     7,   182,    24,    25,    11,    12,   239,    48,    49,    50,
   200,    17,   252,    19,    36,   166,   256,     7,    25,   152,
   153,   154,   155,   156,    11,    12,   502,     6,     7,    51,
    52,   164,    11,    12,   167,   168,   169,   170,    51,    52,
    17,    63,    19,   283,   119,   120,   121,   450,    45,    46,
    72,    73,    17,    17,    19,    19,   128,   129,     3,     4,
     5,     6,     7,    17,    58,    19,    11,    12,   672,    54,
    17,   247,    19,    18,    17,    15,    19,    17,    59,    15,
    25,    17,   322,    54,    17,   325,    19,   238,   328,     5,
    17,    36,    19,   333,    74,    17,   247,    19,    17,    17,
    19,    19,   335,    11,    12,   345,    51,    52,   348,   349,
    17,   351,    19,    17,   406,    19,   720,    74,   410,   352,
   441,    17,   443,   415,    74,   389,    74,    72,    73,     6,
     7,    26,   265,   609,   135,   136,   127,   128,   271,   272,
    74,    26,    74,    16,   277,    32,    18,   623,   381,   282,
    32,   554,    18,    70,    93,    93,    98,   397,   398,   108,
   115,   127,   402,   139,    65,   405,     3,     4,     5,     6,
     7,    24,    98,   148,    11,    12,    16,    16,    34,    20,
    18,    18,    18,   334,    38,   425,    38,   791,    25,    18,
    71,    94,   432,   433,   434,    94,    18,    18,    18,    36,
   106,    99,    65,   166,   337,    18,    18,    18,   448,    18,
   133,    46,   106,   105,    51,    52,    18,    18,    17,    38,
    17,   513,   798,    19,   516,    19,    63,   200,   468,    20,
   200,   200,   200,   200,    23,    72,    73,    95,   100,    18,
    18,   102,   482,   117,   389,   815,    10,    16,    24,     6,
   726,    38,     5,    12,     9,   825,    17,    19,   828,   662,
    18,    17,    96,    17,   134,   112,    17,    19,   419,   420,
    19,    19,   118,   424,    16,    18,   427,   410,   670,   412,
   431,   432,   433,   434,   417,   418,    17,    16,    21,   440,
   441,   767,   443,    21,    18,   150,   429,    19,    19,     3,
     4,     5,     6,     7,    17,    19,    17,    11,    12,    17,
   713,    15,     1,   464,    18,    17,   492,     6,     7,   470,
    14,    25,    19,   104,   151,    66,    18,    18,   620,   152,
   570,    17,    36,   725,   479,    23,   569,   153,   741,   742,
    23,   492,    19,    38,    18,    18,   154,    51,    52,    18,
   110,    19,    18,    38,   104,    18,    38,    61,    62,     0,
    64,    65,    66,     0,   111,   103,   499,   113,    72,    73,
   238,   334,   523,   113,   389,   526,     5,    96,   511,   490,
   492,   191,   676,   672,   725,    74,   626,    16,   684,   686,
   755,   556,   715,   526,   686,   546,   741,   637,   531,   742,
   713,    16,    28,    92,   382,   556,   690,   170,    97,   649,
    99,   155,   101,    42,   103,   648,   734,   568,   107,    48,
   649,   272,   646,   654,   169,   114,   295,   116,   561,   562,
   653,   296,   705,   122,   123,   124,   125,   126,    -1,    -1,
    -1,   130,    -1,   132,    -1,    -1,    -1,   762,   137,   138,
   690,   140,   141,   142,   143,   144,   145,   146,   147,    88,
    89,    -1,   754,   755,   427,    -1,    -1,    96,   431,   432,
   433,   434,   787,    -1,   103,   626,    -1,   440,   441,   108,
   443,   110,   633,    -1,    -1,   114,    -1,    -1,    -1,    -1,
   782,    -1,    -1,   626,    -1,   646,    -1,    -1,   649,    -1,
    -1,   464,   653,    -1,    -1,    -1,    -1,   470,   183,   184,
   185,   186,   187,   188,   189,   190,   191,    -1,   669,    -1,
    -1,   654,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   662,
   159,   160,   161,   162,   163,    -1,    -1,   670,    -1,   690,
    -1,    -1,    -1,   172,   173,   174,   175,   780,    -1,   682,
    -1,   684,    -1,    -1,   705,    -1,   701,   690,   798,   188,
   693,    -1,   191,    -1,   715,    -1,    -1,    -1,   197,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   712,
   713,    -1,    -1,   546,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   725,   556,    -1,    -1,    -1,    -1,    -1,    -1,
   733,    -1,    -1,    -1,    -1,   568,    -1,    -1,   741,   742,
    -1,   762,   763,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   249,   772,    -1,   252,    -1,    -1,   777,   256,    -1,    -1,
    -1,    -1,   261,   262,    -1,    -1,   787,   266,    -1,    -1,
   269,    -1,    -1,    -1,    -1,    -1,    -1,   798,    -1,    -1,
    -1,    -1,    -1,    -1,   283,    -1,   807,    -1,    -1,    -1,
    -1,   794,    -1,   626,   815,    -1,    -1,    -1,    -1,    -1,
   633,   804,    -1,    -1,   825,    -1,    -1,   828,    -1,   812,
    -1,    -1,    -1,   646,    -1,    -1,   649,    -1,    -1,    -1,
   653,    -1,    -1,   322,    -1,    -1,   325,    -1,    -1,   328,
    29,    30,    -1,    -1,   333,    -1,   669,    36,    37,    -1,
    39,    -1,    41,    42,    43,    44,   345,    -1,    -1,   348,
   349,    -1,   351,    -1,    -1,    -1,    -1,   690,    57,    -1,
    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   705,    -1,    -1,    -1,    75,    76,    77,    78,
    -1,    -1,   715,    20,    -1,    -1,    85,    86,    87,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   396,   397,   398,
    -1,    -1,    -1,   402,    -1,    -1,   405,   406,   407,    -1,
    -1,   410,    -1,    -1,    -1,    -1,   415,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   425,    -1,    -1,   762,
   763,    -1,    -1,   432,   433,   434,    -1,    -1,    -1,   772,
    -1,    -1,    -1,    -1,   777,    -1,    -1,    -1,    -1,   448,
    -1,    -1,    -1,    -1,   787,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,   798,   465,    -1,    -1,   468,
    -1,   470,     1,    -1,   807,    -1,    -1,     6,     7,    -1,
    -1,    -1,    -1,   482,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   490,    -1,   192,   193,   194,   195,   196,   197,   198,
   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   513,    -1,    -1,   516,   155,   156,
   157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
   167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
   177,   178,   179,   180,   181,    74,    -1,    -1,    -1,    -1,
    -1,   550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    97,    -1,
    99,   570,   101,    -1,   103,    -1,    -1,    -1,   107,    -1,
    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,
    -1,    -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,
    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,   137,   138,
    -1,   140,   141,   142,   143,   144,   145,   146,   147,    -1,
    -1,   620,    -1,    -1,    -1,    -1,    -1,   626,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   637,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   649,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   662,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,   672,    -1,    -1,    -1,   676,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,   686,    -1,    -1,
    -1,   690,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   701,    -1,    -1,    -1,    -1,   706,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,   720,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
    12,    -1,   741,   742,    -1,    -1,    18,    -1,    -1,    -1,
    22,    -1,    -1,    25,    -1,   754,   755,    29,    30,    31,
    -1,    33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,
    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
    52,    -1,    -1,   782,    -1,    57,    -1,    -1,    60,    -1,
    -1,    -1,   791,    -1,    -1,    -1,    -1,    -1,    -1,   798,
    72,    73,    -1,    75,    76,    77,    78,    79,    80,    81,
    82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,
    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,    -1,
   132,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
   142,   143,   144,   145,   146,   147,     6,     7,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    19,
    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,
    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,
    -1,    41,    42,    43,    44,    -1,    -1,    47,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
    90,    91,    92,    -1,    -1,    -1,    -1,    97,    -1,    99,
    -1,   101,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,
    -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,    -1,
   130,    -1,   132,    -1,    -1,    -1,    -1,   137,   138,    -1,
   140,   141,   142,   143,   144,   145,   146,   147,     6,     7,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
    -1,    29,    30,    31,    -1,    33,    -1,    -1,    36,    37,
    -1,    39,    -1,    41,    42,    43,    44,    -1,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    97,
    -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,   107,
    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,
    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,    -1,
    -1,    -1,   130,    -1,   132,     6,     7,    -1,    -1,   137,
   138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
    31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,    40,
    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    56,    57,    -1,    -1,    60,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
   101,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,
    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,
    -1,   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,
    -1,   132,     6,     7,    -1,    -1,   137,   138,    -1,   140,
   141,   142,   143,   144,   145,   146,   147,    -1,    22,    -1,
    24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
    -1,    -1,    36,    37,    -1,    39,    -1,    41,    42,    43,
    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    75,    76,    77,    78,    79,    80,    81,    82,    83,
    84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,
    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,   123,
   124,   125,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,
    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,
   144,   145,   146,   147,     6,     7,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
    -1,    33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,
    42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
    82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,
    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,    -1,
   132,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,
   142,   143,   144,   145,   146,   147,     6,     7,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,
    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,
    -1,    41,    42,    43,    44,    -1,    -1,    47,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,
    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,
    80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
    90,    91,    92,    -1,    -1,    -1,    -1,    97,    -1,    99,
    -1,   101,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,
    -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,    -1,
   130,    -1,   132,     6,     7,    -1,    -1,   137,   138,    -1,
   140,   141,   142,   143,   144,   145,   146,   147,    -1,    22,
    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,
    33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,    42,
    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
   123,   124,   125,   126,    -1,    -1,    -1,   130,    -1,   132,
    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
   143,   144,   145,   146,   147,     6,     7,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
    31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,    -1,
    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
   101,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,
    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,
    -1,   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,
    -1,   132,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
   141,   142,   143,   144,   145,   146,   147,     6,     7,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    30,    31,    -1,    33,    -1,    -1,    36,    37,    -1,
    39,    -1,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    78,
    79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
    89,    90,    91,    92,    -1,    -1,    -1,    -1,    97,    -1,
    99,    -1,   101,    -1,   103,    -1,    -1,    -1,   107,    -1,
    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,
    -1,    -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,
    -1,   130,    -1,   132,     6,     7,    -1,    -1,   137,   138,
    -1,   140,   141,   142,   143,   144,   145,   146,   147,    -1,
    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
    -1,    33,    -1,    -1,    36,    37,    -1,    39,    -1,    41,
    42,    43,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,    81,
    82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,
    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
    -1,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,    -1,
   132,     6,     7,    -1,    -1,   137,   138,    -1,   140,   141,
   142,   143,   144,   145,   146,   147,    -1,    22,    -1,    -1,
    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,
    -1,    36,    37,    -1,    39,    -1,    41,    42,    43,    44,
    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
    85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
    -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,   114,
    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
   125,   126,    -1,    -1,    -1,   130,    -1,   132,     6,     7,
    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
   145,   146,   147,    -1,    22,    -1,    -1,    -1,    -1,    -1,
    -1,    29,    30,    31,    -1,    33,    -1,    -1,    36,    37,
    -1,    39,    -1,    41,    42,    43,    44,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
    78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,    97,
    -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,   107,
    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,
    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,    -1,
    -1,    -1,   130,    -1,   132,     6,     7,    -1,    -1,   137,
   138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
    31,    -1,    33,    -1,    -1,    36,    37,    -1,    39,    -1,
    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,
    -1,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    75,    76,    77,    78,    79,    80,
    81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
    91,    92,    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,
   101,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,
    -1,    -1,    -1,   114,    -1,   116,    -1,     6,    -1,    -1,
    -1,   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,
    -1,   132,    -1,    -1,    -1,    -1,   137,   138,    -1,   140,
   141,   142,   143,   144,   145,   146,   147,    92,    -1,    -1,
    -1,    -1,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
    -1,    -1,   107,    -1,    53,    -1,    -1,    -1,    -1,   114,
    -1,   116,    -1,     6,    -1,    -1,    -1,   122,   123,   124,
   125,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,    -1,
    -1,    -1,   137,   138,    -1,   140,   141,   142,   143,   144,
   145,   146,   147,    92,    -1,    -1,    -1,    -1,    97,    -1,
    99,    -1,   101,    -1,   103,    -1,    -1,    -1,   107,    -1,
    -1,    -1,    -1,    -1,    -1,   114,    -1,   116,    -1,    -1,
    -1,    -1,    -1,   122,   123,   124,   125,   126,    -1,    -1,
    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,   137,   138,
    -1,   140,   141,   142,   143,   144,   145,   146,   147,    92,
    -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
   123,   124,   125,   126,    -1,    -1,    -1,   130,    -1,   132,
    -1,    -1,    -1,    -1,   137,   138,    -1,   140,   141,   142,
   143,   144,   145,   146,   147,   155,   156,   157,   158,   159,
   160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
   170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
   180,   181
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifdef _MSC_VER
#pragma warning(disable: 4701)
#endif

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif

#line 201 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;	/*  the variable used to return		*/
					/*  semantic values from the action	*/
					/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = (short)yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 481 "core/parse-asn1.y"
{ yyval.intVal = myLineNoG; ;
    break;}
case 2:
#line 486 "core/parse-asn1.y"
{ modulePtrG->tagDefault = yyvsp[0].intVal; ;
    break;}
case 3:
#line 493 "core/parse-asn1.y"
{
         modulePtrG->modId = yyvsp[-9].moduleId;
		 modulePtrG->namespaceToUse = yyvsp[-2].charPtr;

         /*
          * Set exported flags in type/value defs as appropriate
          */
         SetExports (modulePtrG, exportListG, exportsParsedG);

         /* clean up */

         /* Free Application tag list */
         FreeApplTags();

         /*
          * Add values defined in any parsed object identifiers.
          * Only the Module name and some macro oids have been parsed,
          * the rest are just "{...}" strings at this point
          * (they will be parsed in later)
// REN -- 9/23/02
         modulePtrG->valueDefs =
             AsnListConcat (modulePtrG->valueDefs, oidElmtValDefsG);
          */

         /*
          * free list head only
// REN -- 9/23/02
         Free (oidElmtValDefsG);
          */
     ;
    break;}
case 4:
#line 526 "core/parse-asn1.y"
{ yyval.intVal = EXPLICIT_TAGS; ;
    break;}
case 5:
#line 527 "core/parse-asn1.y"
{ yyval.intVal = IMPLICIT_TAGS; ;
    break;}
case 6:
#line 529 "core/parse-asn1.y"
{
        /* default is EXPLICIT TAGS */
        yyval.intVal = EXPLICIT_TAGS;
    ;
    break;}
case 7:
#line 537 "core/parse-asn1.y"
{
		yyval.boolVal = TRUE;
	;
    break;}
case 8:
#line 541 "core/parse-asn1.y"
{
		yyval.boolVal = FALSE;
	;
    break;}
case 9:
#line 550 "core/parse-asn1.y"
{
        yyval.moduleId = MT (ModuleId);
        yyval.moduleId->name = yyvsp[-1].charPtr;
        yyval.moduleId->oid = yyvsp[0].oidPtr;
    ;
    break;}
case 11:
#line 559 "core/parse-asn1.y"
{ yyval.oidPtr = NULL; ;
    break;}
case 12:
#line 563 "core/parse-asn1.y"
{ yyval.charPtr = yyvsp[0].charPtr; ;
    break;}
case 13:
#line 564 "core/parse-asn1.y"
{ yyval.charPtr = NULL; ;
    break;}
case 16:
#line 575 "core/parse-asn1.y"
{
        /*
         *  allows differentiation between "EXPORTS;"
         *         (in which no exports allowed)
         *  and when the EXPORTS symbol does not appear
         *         (then all are exported)
         */
        exportsParsedG = TRUE;
    ;
    break;}
case 17:
#line 585 "core/parse-asn1.y"
{
        PARSE_ERROR();
        exportsParsedG = FALSE;
        exportListG = NULL;
        yyerrok;
    ;
    break;}
case 18:
#line 591 "core/parse-asn1.y"
{ exportsParsedG = FALSE; ;
    break;}
case 19:
#line 595 "core/parse-asn1.y"
{ exportListG = yyvsp[0].exportList; ;
    break;}
case 20:
#line 596 "core/parse-asn1.y"
{ exportListG = NULL; ;
    break;}
case 21:
#line 601 "core/parse-asn1.y"
{
        yyval.exportList = MT (ExportElmt);
        yyval.exportList->name = yyvsp[0].charPtr;
        yyval.exportList->lineNo = myLineNoG;
        yyval.exportList->next = NULL;
    ;
    break;}
case 22:
#line 608 "core/parse-asn1.y"
{
        yyval.exportList = MT (ExportElmt);
        yyval.exportList->name = yyvsp[0].charPtr;
        yyval.exportList->next = yyvsp[-3].exportList;
        yyval.exportList->lineNo = yyvsp[-1].intVal;
    ;
    break;}
case 24:
#line 619 "core/parse-asn1.y"
{
       PARSE_ERROR();
       yyerrok;
    ;
    break;}
case 26:
#line 627 "core/parse-asn1.y"
{ modulePtrG->imports = yyvsp[0].importModuleListPtr; ;
    break;}
case 28:
#line 633 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].importModulePtr,yyvsp[-1].importModuleListPtr);
    ;
    break;}
case 29:
#line 637 "core/parse-asn1.y"
{
        yyval.importModuleListPtr = NEWLIST();
        APPEND (yyvsp[0].importModulePtr, yyval.importModuleListPtr);
    ;
    break;}
case 30:
#line 645 "core/parse-asn1.y"
{
        yyval.importModulePtr = MT (ImportModule);
        yyval.importModulePtr->modId   = yyvsp[0].moduleId;
        yyval.importModulePtr->lineNo = yyvsp[-1].intVal;
        yyval.importModulePtr->importElmts = yyvsp[-3].importElmtListPtr;
    ;
    break;}
case 31:
#line 656 "core/parse-asn1.y"
{
        ImportElmt *ie;

        ie = MT (ImportElmt);
        ie->name = yyvsp[0].charPtr;
        ie->lineNo = myLineNoG;
        APPEND (ie, yyvsp[-2].importElmtListPtr);
        yyval.importElmtListPtr = yyvsp[-2].importElmtListPtr;
    ;
    break;}
case 32:
#line 666 "core/parse-asn1.y"
{
        ImportElmt *ie;

        /* called for the first element only, so create list head */
        yyval.importElmtListPtr = NEWLIST();
        ie = MT (ImportElmt);
        ie->name = yyvsp[0].charPtr;
        ie->lineNo = myLineNoG;
        APPEND (ie, yyval.importElmtListPtr);
    ;
    break;}
case 35:
#line 682 "core/parse-asn1.y"
{
        /*
         * hack to make DefinedMacroNames "freeable"
         * like idents and typeref
         */
        yyval.charPtr = Malloc (strlen (yyvsp[0].charPtr)+1);
        strcpy (yyval.charPtr, yyvsp[0].charPtr);
    ;
    break;}
case 40:
#line 703 "core/parse-asn1.y"
{
        PARSE_ERROR();
        yyerrok;
    ;
    break;}
case 41:
#line 711 "core/parse-asn1.y"
{
        /*
         * a macro may produce a null type
         */
        if (yyvsp[0].typeDefPtr != NULL)
        {
            /*
             * add to head of  type def list
             */
            APPEND (yyvsp[0].typeDefPtr, modulePtrG->typeDefs);
        }

    ;
    break;}
case 42:
#line 725 "core/parse-asn1.y"
{
        /*
         * a macro may produce a null value
         */
        if (yyvsp[0].valueDefPtr != NULL)
        {
            /*
             * add to head of value def list
             */
            APPEND (yyvsp[0].valueDefPtr, modulePtrG->valueDefs);
        }
    ;
    break;}
case 43:
#line 737 "core/parse-asn1.y"
{ LexBeginMacroDefContext(); ;
    break;}
case 44:
#line 739 "core/parse-asn1.y"
{
        TypeDef *tmpTypeDef;

        /*
         *  LEXICAL TIE IN!!
         * create macro type to eliminate import resolution
         * errors msgs from other modules importing the macro.
         * (hopefully) Only the import list will link with
         * these type defs.
         * keeps macro def around incase of future processing needs
         *
         * NOTE: MACRODEFBODY_SYM returns the macro def body with
         * with "BEGIN" at the begininning and "END" at the end
         */

        /*
         * put lexical analyzer back in normal state
         */
        /*  BEGIN (INITIAL);  */
        LexBeginInitialContext();

        tmpTypeDef = MT (TypeDef);
        SetupType (&tmpTypeDef->type, BASICTYPE_MACRODEF, yyvsp[-2].intVal);
        tmpTypeDef->definedName = yyvsp[-5].charPtr;

        /*
         * keeps the macro def body
         * (all text between & including the BEGIN and END)
         * as a simple string - incase you want to fart around with
         * it.
         */
        tmpTypeDef->type->basicType->a.macroDef = yyvsp[0].charPtr;

        /*
         * put in type list
         */
        APPEND (tmpTypeDef, modulePtrG->typeDefs);

    ;
    break;}
case 45:
#line 779 "core/parse-asn1.y"
{
        TypeDef *tmpTypeDef;

        tmpTypeDef = MT (TypeDef);
        SetupType (&tmpTypeDef->type, BASICTYPE_MACRODEF, myLineNoG);
        tmpTypeDef->definedName = yyvsp[-2].charPtr;

        tmpTypeDef->type->basicType->a.macroDef = yyvsp[0].charPtr;

        /*
         * put in type list
         */
        APPEND (tmpTypeDef, modulePtrG->typeDefs);

    ;
    break;}
case 46:
#line 795 "core/parse-asn1.y"
{
        TypeDef *tmpTypeDef;

        tmpTypeDef = MT (TypeDef);
        SetupType (&tmpTypeDef->type, BASICTYPE_MACRODEF, myLineNoG);
        tmpTypeDef->definedName = yyvsp[-4].charPtr;

        tmpTypeDef->type->basicType->a.macroDef =
               (MyString) Malloc (strlen (yyvsp[-2].charPtr) + strlen (yyvsp[0].charPtr) + 2);

        strcpy (tmpTypeDef->type->basicType->a.macroDef, yyvsp[-2].charPtr);
        strcat (tmpTypeDef->type->basicType->a.macroDef, ".");
        strcat (tmpTypeDef->type->basicType->a.macroDef, yyvsp[0].charPtr);

       /*
         * put in type list
         */
        APPEND (tmpTypeDef, modulePtrG->typeDefs);

        Free (yyvsp[-2].charPtr);
        Free (yyvsp[0].charPtr);
    ;
    break;}
case 49:
#line 830 "core/parse-asn1.y"
{
        /*
         * a macro type may produce a null type
         */
        if (yyvsp[-1].typePtr != NULL)
        {
            yyval.typeDefPtr = MT (TypeDef);
            yyval.typeDefPtr->type =  yyvsp[-1].typePtr;
            yyval.typeDefPtr->type->lineNo = yyvsp[-2].intVal;
            yyval.typeDefPtr->type->attrList = yyvsp[0].directiveList;
            yyval.typeDefPtr->definedName = yyvsp[-5].charPtr;
            yyval.typeDefPtr->attrList = yyvsp[-3].directiveList;
        }
        else
            yyval.typeDefPtr = NULL;
    ;
    break;}
case 50:
#line 851 "core/parse-asn1.y"
{
        /* allocate a Type with basic type of ImportTypeRef */
        SetupType (&yyval.typePtr, BASICTYPE_IMPORTTYPEREF, yyvsp[-1].intVal);
        yyval.typePtr->basicType->a.importTypeRef = MT (TypeRef);
        yyval.typePtr->basicType->a.importTypeRef->typeName = yyvsp[0].charPtr;
        yyval.typePtr->basicType->a.importTypeRef->moduleName = yyvsp[-3].charPtr;

        /* add entry to this module's import list */
        AddPrivateImportElmt (modulePtrG, yyvsp[0].charPtr, yyvsp[-3].charPtr, yyvsp[-1].intVal);
    ;
    break;}
case 51:
#line 865 "core/parse-asn1.y"
{ yyval.typePtr = yyvsp[0].typePtr; ;
    break;}
case 52:
#line 867 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_LOCALTYPEREF, myLineNoG);
        yyval.typePtr->basicType->a.localTypeRef = MT (TypeRef);
        yyval.typePtr->basicType->a.localTypeRef->typeName = yyvsp[0].charPtr;
    ;
    break;}
case 75:
#line 903 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_OCTETSTRING, myLineNoG);
  ;
    break;}
case 79:
#line 913 "core/parse-asn1.y"
{
        yyval.namedTypePtr = MT (NamedType);
        yyval.namedTypePtr->type = yyvsp[0].typePtr;
        yyval.namedTypePtr->fieldName = yyvsp[-1].charPtr;
    ;
    break;}
case 80:
#line 919 "core/parse-asn1.y"
{
		printf("Line %ld: Warning - 2002 ASN.1 syntax no longer supports un-named types!\n", myLineNoG);
		printf("	--Suggest using an identifier or may cause errors\n");
        yyval.namedTypePtr = MT (NamedType);
        yyval.namedTypePtr->type = yyvsp[0].typePtr;
    ;
    break;}
case 81:
#line 929 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_BOOLEAN, myLineNoG);
    ;
    break;}
case 82:
#line 936 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_INTEGER, myLineNoG);
        yyval.typePtr->basicType->a.integer = NEWLIST();  /* empty list */
    ;
    break;}
case 83:
#line 941 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_INTEGER, myLineNoG);
        yyval.typePtr->basicType->a.integer = yyvsp[-1].valueDefListPtr;
    ;
    break;}
case 84:
#line 950 "core/parse-asn1.y"
{
        yyval.valueDefListPtr = NEWLIST();
        APPEND (yyvsp[0].valueDefPtr, yyval.valueDefListPtr);
    ;
    break;}
case 85:
#line 955 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].valueDefPtr,yyvsp[-2].valueDefListPtr);
        yyval.valueDefListPtr = yyvsp[-2].valueDefListPtr;
    ;
    break;}
case 86:
#line 963 "core/parse-asn1.y"
{
        yyval.valueDefPtr = MT (ValueDef);
        yyval.valueDefPtr->definedName = yyvsp[-3].charPtr;
        SetupValue (&yyval.valueDefPtr->value, BASICVALUE_INTEGER, myLineNoG);
        yyval.valueDefPtr->value->basicValue->a.integer = yyvsp[-1].intVal;
    ;
    break;}
case 87:
#line 970 "core/parse-asn1.y"
{
        yyval.valueDefPtr = MT (ValueDef);
        yyval.valueDefPtr->definedName = yyvsp[-3].charPtr;
        yyval.valueDefPtr->value = yyvsp[-1].valuePtr;
    ;
    break;}
case 88:
#line 979 "core/parse-asn1.y"
{
		yyval.intVal=yyvsp[0].uintVal;
    ;
    break;}
case 89:
#line 983 "core/parse-asn1.y"
{
    ;
    break;}
case 90:
#line 986 "core/parse-asn1.y"
{
		yyvsp[0].uintVal=0-yyvsp[0].uintVal;
		yyval.intVal=yyvsp[0].uintVal;
    ;
    break;}
case 91:
#line 991 "core/parse-asn1.y"
{
    ;
    break;}
case 92:
#line 997 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_ENUMERATED, myLineNoG);
        yyval.typePtr->basicType->a.enumerated = yyvsp[-1].valueDefListPtr;
    ;
    break;}
case 93:
#line 1008 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_REAL, myLineNoG);
    ;
    break;}
case 94:
#line 1016 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_BITSTRING, myLineNoG);
        yyval.typePtr->basicType->a.bitString = NEWLIST(); /* empty list */
    ;
    break;}
case 95:
#line 1021 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_BITSTRING, myLineNoG);
        yyval.typePtr->basicType->a.bitString = yyvsp[-1].valueDefListPtr;
    ;
    break;}
case 97:
#line 1035 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_NULL, myLineNoG);
    ;
    break;}
case 98:
#line 1042 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_EXTERNAL, myLineNoG);
    ;
    break;}
case 99:
#line 1049 "core/parse-asn1.y"
{ yyval.intVal = yyvsp[-1].intVal; ;
    break;}
case 100:
#line 1054 "core/parse-asn1.y"
{
        NamedType *n;

        SetupType (&yyval.typePtr, BASICTYPE_SEQUENCE, yyvsp[-2].intVal);

        if (AsnListCount ((AsnList*)yyvsp[-1].namedTypeListPtr) != 0)
        {
            n = (NamedType*) FIRST_LIST_ELMT ((AsnList*)yyvsp[-1].namedTypeListPtr);
            n->type->lineNo = yyvsp[-2].intVal;
        }

        yyval.typePtr->basicType->a.sequence = yyvsp[-1].namedTypeListPtr;

    ;
    break;}
case 101:
#line 1069 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_SEQUENCE, yyvsp[-1].intVal);

        /* set up empty list for SEQ with no elmts */
        yyval.typePtr->basicType->a.sequence = AsnListNew (sizeof (void*));
    ;
    break;}
case 102:
#line 1085 "core/parse-asn1.y"
{
      NamedType *lastElmt;
      if (yyvsp[0].directiveList != NULL)
        {
            lastElmt = (NamedType*)LAST_LIST_ELMT (yyvsp[-1].namedTypeListPtr);
            lastElmt->type->attrList = yyvsp[0].directiveList;
        }

        yyval.namedTypeListPtr = yyvsp[-1].namedTypeListPtr;
    ;
    break;}
case 104:
#line 1100 "core/parse-asn1.y"
{
        NamedType *firstElmt;

        if (yyvsp[-2].directiveList != NULL)
        {
            firstElmt = (NamedType*)FIRST_LIST_ELMT (yyvsp[0].namedTypeListPtr);
            firstElmt->type->attrList = yyvsp[-2].directiveList;
        }

       PREPEND (yyvsp[-4].namedTypePtr, yyvsp[0].namedTypeListPtr);
       firstElmt = (NamedType*)FIRST_LIST_ELMT (yyvsp[0].namedTypeListPtr);
       firstElmt->type->lineNo = yyvsp[-1].intVal;
       yyval.namedTypeListPtr = yyvsp[0].namedTypeListPtr;
    ;
    break;}
case 105:
#line 1115 "core/parse-asn1.y"
{
	   yyval.namedTypeListPtr = NEWLIST();
       PREPEND (yyvsp[0].namedTypePtr,yyval.namedTypeListPtr);
    ;
    break;}
case 106:
#line 1124 "core/parse-asn1.y"
{   
		yyval.namedTypeListPtr = yyvsp[0].namedTypeListPtr;
    ;
    break;}
case 107:
#line 1131 "core/parse-asn1.y"
{
		NamedType *nt;
		yyval.namedTypeListPtr = NEWLIST();
		nt = MT(NamedType);
		SetupType (&nt->type, BASICTYPE_EXTENSION, myLineNoG);
		nt->type->extensionAddition = TRUE;
		APPEND(nt, yyval.namedTypeListPtr);
	;
    break;}
case 108:
#line 1140 "core/parse-asn1.y"
{
		NamedType *nt;
		yyval.namedTypeListPtr = yyvsp[-1].namedTypeListPtr;
		nt = MT(NamedType);
		SetupType (&nt->type, BASICTYPE_EXTENSION, myLineNoG);
		nt->type->extensionAddition = TRUE;
		APPEND(nt, yyval.namedTypeListPtr);

	;
    break;}
case 109:
#line 1150 "core/parse-asn1.y"
{
		NamedType *nt;
		yyval.namedTypeListPtr = NEWLIST();
		nt = MT(NamedType);
		SetupType (&nt->type, BASICTYPE_EXTENSION, myLineNoG);
		nt->type->extensionAddition = TRUE; /* PL: wasn't set */
		APPEND(nt, yyval.namedTypeListPtr);
	;
    break;}
case 111:
#line 1163 "core/parse-asn1.y"
{
		CONCAT(yyvsp[-2].namedTypeListPtr, yyvsp[0].namedTypeListPtr);
		yyval.namedTypeListPtr = yyvsp[-2].namedTypeListPtr;
	;
    break;}
case 112:
#line 1171 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = NEWLIST();
		yyvsp[0].namedTypePtr->type->extensionAddition = TRUE;
		APPEND(yyvsp[0].namedTypePtr, yyval.namedTypeListPtr);
	;
    break;}
case 113:
#line 1177 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = yyvsp[-2].namedTypeListPtr;
	;
    break;}
case 114:
#line 1184 "core/parse-asn1.y"
{
		NamedType *lastElmt;
		yyvsp[0].namedTypePtr->type->extensionAddition = TRUE;

        if (yyvsp[-2].directiveList != NULL)
        {
            lastElmt = (NamedType*)LAST_LIST_ELMT (yyvsp[-4].namedTypeListPtr);
            lastElmt->type->attrList = yyvsp[-2].directiveList;
        }

       APPEND(yyvsp[0].namedTypePtr, yyvsp[-4].namedTypeListPtr);
       lastElmt = (NamedType*)LAST_LIST_ELMT (yyvsp[-4].namedTypeListPtr);
       lastElmt->type->lineNo = yyvsp[-1].intVal;
       yyval.namedTypeListPtr = yyvsp[-4].namedTypeListPtr;
    ;
    break;}
case 115:
#line 1200 "core/parse-asn1.y"
{
	   yyval.namedTypeListPtr = NEWLIST();
	   yyvsp[0].namedTypePtr->type->extensionAddition = TRUE;
       APPEND(yyvsp[0].namedTypePtr,yyval.namedTypeListPtr);
    ;
    break;}
case 117:
#line 1210 "core/parse-asn1.y"
{
		yyval.intVal = -1;
	;
    break;}
case 121:
#line 1224 "core/parse-asn1.y"
{
        yyval.namedTypePtr = yyvsp[-1].namedTypePtr;
        yyval.namedTypePtr->type->optional = TRUE;
    ;
    break;}
case 122:
#line 1229 "core/parse-asn1.y"
{
        /*
         * this rules uses NamedValue instead of Value
         * for the stupid choice value syntax (fieldname value)
         * it should be like a set/seq value (ie with
         * enclosing { }
         */
        yyval.namedTypePtr = yyvsp[-2].namedTypePtr;
        yyval.namedTypePtr->type->defaultVal = yyvsp[0].namedValuePtr;
        /*
         * could link value to the elmt type here (done in link_types.c)
         */
    ;
    break;}
case 123:
#line 1243 "core/parse-asn1.y"
{
        yyval.namedTypePtr = MT (NamedType);
        SetupType (&yyval.namedTypePtr->type, BASICTYPE_COMPONENTSOF, myLineNoG);
        yyval.namedTypePtr->type->basicType->a.componentsOf = yyvsp[0].typePtr;
    ;
    break;}
case 124:
#line 1249 "core/parse-asn1.y"
{
        yyval.namedTypePtr = MT (NamedType);
        SetupType (&yyval.namedTypePtr->type, BASICTYPE_COMPONENTSOF, myLineNoG);
        yyval.namedTypePtr->fieldName = yyvsp[-3].charPtr;
        yyval.namedTypePtr->type->basicType->a.componentsOf = yyvsp[0].typePtr;
    ;
    break;}
case 125:
#line 1261 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_SEQUENCEOF, myLineNoG);

        /* grab line number from first elmt */
        if (yyvsp[0].typePtr != NULL)
            yyval.typePtr->lineNo = yyvsp[0].typePtr->lineNo - 1;

        yyval.typePtr->basicType->a.sequenceOf = yyvsp[0].typePtr;
    ;
    break;}
case 126:
#line 1273 "core/parse-asn1.y"
{ yyval.intVal = yyvsp[-1].intVal; ;
    break;}
case 127:
#line 1278 "core/parse-asn1.y"
{
        NamedType *n;

        SetupType (&yyval.typePtr, BASICTYPE_SET, yyvsp[-2].intVal);

        /* reset first elmt's line number */
        if (AsnListCount ((AsnList*)yyvsp[-1].namedTypeListPtr) != 0)
        {
            n = (NamedType*)FIRST_LIST_ELMT ((AsnList*)yyvsp[-1].namedTypeListPtr);
            n->type->lineNo = yyvsp[-2].intVal;
        }
        yyval.typePtr->basicType->a.set = yyvsp[-1].namedTypeListPtr;
    ;
    break;}
case 128:
#line 1292 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_SET, yyvsp[-1].intVal);

        /* set up empty elmt list for SET */
        yyval.typePtr->basicType->a.set = AsnListNew (sizeof (void*));
    ;
    break;}
case 129:
#line 1308 "core/parse-asn1.y"
{
       /* does not allow SET == SET OF ANY Abrev */
        SetupType (&yyval.typePtr, BASICTYPE_SETOF, myLineNoG);

        if (yyvsp[0].typePtr != NULL)
            yyval.typePtr->lineNo = yyvsp[0].typePtr->lineNo;

        yyval.typePtr->basicType->a.setOf = yyvsp[0].typePtr;
    ;
    break;}
case 130:
#line 1322 "core/parse-asn1.y"
{
        NamedType *n;

        SetupType (&yyval.typePtr, BASICTYPE_CHOICE, yyvsp[-3].intVal);

        yyval.typePtr->basicType->a.choice = yyvsp[-1].namedTypeListPtr;

        if (AsnListCount (yyvsp[-1].namedTypeListPtr) != 0)
        {
            n = (NamedType*)FIRST_LIST_ELMT (yyvsp[-1].namedTypeListPtr);
            n->type->lineNo = yyvsp[-3].intVal;
        }
    ;
    break;}
case 131:
#line 1339 "core/parse-asn1.y"
{
        NamedType *lastElmt;
        if (yyvsp[0].directiveList != NULL)
        {
            lastElmt = (NamedType*)LAST_LIST_ELMT (yyvsp[-1].namedTypeListPtr);
            lastElmt->type->attrList = yyvsp[0].directiveList;
        }
        yyval.namedTypeListPtr = yyvsp[-1].namedTypeListPtr;
    ;
    break;}
case 132:
#line 1352 "core/parse-asn1.y"
{
        yyval.namedTypeListPtr = NEWLIST();
        PREPEND (yyvsp[0].namedTypePtr, yyval.namedTypeListPtr);
    ;
    break;}
case 133:
#line 1357 "core/parse-asn1.y"
{
        NamedType *firstElmt;

        if (yyvsp[-1].directiveList != NULL)
        {
            firstElmt = (NamedType*)FIRST_LIST_ELMT (yyvsp[0].namedTypeListPtr);
            firstElmt->type->attrList = yyvsp[-1].directiveList;
        }
        PREPEND (yyvsp[-3].namedTypePtr,yyvsp[0].namedTypeListPtr);
        yyval.namedTypeListPtr = yyvsp[0].namedTypeListPtr;
    ;
    break;}
case 135:
#line 1373 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = yyvsp[0].namedTypeListPtr;
	;
    break;}
case 136:
#line 1379 "core/parse-asn1.y"
{
		NamedType *nt;
		yyval.namedTypeListPtr = NEWLIST();
		nt = MT(NamedType);
		SetupType (&nt->type, BASICTYPE_EXTENSION, myLineNoG);
		nt->type->extensionAddition = TRUE;
		APPEND(nt, yyval.namedTypeListPtr);
	;
    break;}
case 137:
#line 1388 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = yyvsp[-1].namedTypeListPtr;
	;
    break;}
case 138:
#line 1392 "core/parse-asn1.y"
{
		NamedType *nt;
		yyval.namedTypeListPtr = NEWLIST();
		nt = MT(NamedType);
		SetupType (&nt->type, BASICTYPE_EXTENSION, myLineNoG);
		nt->type->extensionAddition = TRUE;
		APPEND(nt, yyval.namedTypeListPtr);
	;
    break;}
case 140:
#line 1405 "core/parse-asn1.y"
{
		CONCAT(yyvsp[-3].namedTypeListPtr, yyvsp[0].namedTypeListPtr);
		yyval.namedTypeListPtr = yyvsp[-3].namedTypeListPtr;
	;
    break;}
case 141:
#line 1413 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = NEWLIST();
		PREPEND (yyvsp[0].namedTypePtr, yyval.namedTypeListPtr);
	;
    break;}
case 142:
#line 1418 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = yyvsp[-2].namedTypeListPtr;
	;
    break;}
case 143:
#line 1425 "core/parse-asn1.y"
{
		yyval.namedTypeListPtr = NEWLIST();
        PREPEND (yyvsp[0].namedTypePtr, yyval.namedTypeListPtr);
	;
    break;}
case 144:
#line 1430 "core/parse-asn1.y"
{
		NamedType *firstElmt;

        if (yyvsp[-2].namedTypeListPtr != NULL)
        {
            firstElmt = (NamedType*)FIRST_LIST_ELMT (yyvsp[-2].namedTypeListPtr);
            firstElmt->type->attrList = yyvsp[-2].namedTypeListPtr;
        }
        PREPEND (yyvsp[0].namedTypePtr,yyvsp[-2].namedTypeListPtr);
        yyval.namedTypeListPtr = yyvsp[-2].namedTypeListPtr;
	;
    break;}
case 145:
#line 1445 "core/parse-asn1.y"
{
        /*
         * the selection type should be replaced after
         * link with actual type
         */
        SetupType (&yyval.typePtr, BASICTYPE_SELECTION, myLineNoG);

        yyval.typePtr->basicType->a.selection = MT (SelectionType);
        yyval.typePtr->basicType->a.selection->typeRef = yyvsp[0].typePtr;
        yyval.typePtr->basicType->a.selection->fieldName = yyvsp[-2].charPtr;
    ;
    break;}
case 146:
#line 1460 "core/parse-asn1.y"
{
        Tag *tag;

        /* remove next tag if any  && IMPLICIT_TAGS */
 	if ((modulePtrG->tagDefault == IMPLICIT_TAGS) &&
            (yyvsp[0].typePtr->tags != NULL) && !LIST_EMPTY (yyvsp[0].typePtr->tags))
        {
            tag = (Tag*)FIRST_LIST_ELMT (yyvsp[0].typePtr->tags); /* set curr to first */
	    AsnListFirst (yyvsp[0].typePtr->tags); /* set curr to first elmt */
            AsnListRemove (yyvsp[0].typePtr->tags);      /* remove first elmt */

            /*
             * set implicit if implicitly tagged built in type (ie not ref)
             * (this simplifies the module ASN.1 printer (print.c))
             */
            if (tag->tclass == UNIV)
                 yyvsp[0].typePtr->implicit = TRUE;

            Free (tag);
        }

        PREPEND (yyvsp[-1].tagPtr, yyvsp[0].typePtr->tags);
        yyval.typePtr = yyvsp[0].typePtr;
    ;
    break;}
case 147:
#line 1485 "core/parse-asn1.y"
{
        Tag *tag;

        /* remove next tag if any */
 	if ((yyvsp[0].typePtr->tags != NULL) && !LIST_EMPTY (yyvsp[0].typePtr->tags))
        {
            tag = (Tag*)FIRST_LIST_ELMT (yyvsp[0].typePtr->tags); /* set curr to first */
	    AsnListFirst (yyvsp[0].typePtr->tags); /* set curr to first elmt */
            AsnListRemove (yyvsp[0].typePtr->tags);      /* remove first elmt */

            if (tag->tclass == UNIV)
                 yyvsp[0].typePtr->implicit = TRUE;

            Free (tag);
        }

        /*
         * must check after linking that implicitly tagged
         * local/import type refs are not untagged choice/any etc
         */
        else if ((yyvsp[0].typePtr->basicType->choiceId == BASICTYPE_IMPORTTYPEREF) ||
                 (yyvsp[0].typePtr->basicType->choiceId == BASICTYPE_LOCALTYPEREF) ||
                 (yyvsp[0].typePtr->basicType->choiceId == BASICTYPE_SELECTION))
            yyvsp[0].typePtr->implicit = TRUE;

        /*
         *  all other implicitly tagable types should have tags
         *  to remove - if this else clause fires then it is
         *  probably a CHOICE or ANY type
         */
        else
        {
            PrintErrLoc (modulePtrG->asn1SrcFileName, yyvsp[0].typePtr->lineNo);
            fprintf (errFileG, "ERROR - attempt to implicitly reference untagged type\n");
            smallErrG = 1;
        }

        PREPEND (yyvsp[-2].tagPtr, yyvsp[0].typePtr->tags);
        yyval.typePtr = yyvsp[0].typePtr;
    ;
    break;}
case 148:
#line 1526 "core/parse-asn1.y"
{
        /* insert tag at head of list */
        yyvsp[-2].tagPtr->explicit = TRUE;
        PREPEND (yyvsp[-2].tagPtr, yyvsp[0].typePtr->tags);
        yyval.typePtr = yyvsp[0].typePtr;
    ;
    break;}
case 149:
#line 1536 "core/parse-asn1.y"
{
        yyval.tagPtr = yyvsp[-1].tagPtr;
        yyval.tagPtr->tclass = yyvsp[-2].intVal;
        yyval.tagPtr->explicit = FALSE; /* default to false */

        /*
         *  keep track of APPLICATION Tags per module
         *  should only be used once
         */
        if ((yyvsp[-2].intVal == APPL) && (yyval.tagPtr->valueRef == NULL))
        {
            PushApplTag (yyval.tagPtr->code, myLineNoG);
        }
    ;
    break;}
case 150:
#line 1554 "core/parse-asn1.y"
{
        yyval.tagPtr = MT (Tag);
        yyval.tagPtr->code = yyvsp[0].intVal;
    ;
    break;}
case 151:
#line 1559 "core/parse-asn1.y"
{
        yyval.tagPtr = MT (Tag);
        yyval.tagPtr->code = NO_TAG_CODE;
        yyval.tagPtr->valueRef = yyvsp[0].valuePtr;
    ;
    break;}
case 152:
#line 1567 "core/parse-asn1.y"
{ yyval.intVal = UNIV; ;
    break;}
case 153:
#line 1568 "core/parse-asn1.y"
{ yyval.intVal = APPL; ;
    break;}
case 154:
#line 1569 "core/parse-asn1.y"
{ yyval.intVal = PRIV; ;
    break;}
case 155:
#line 1570 "core/parse-asn1.y"
{ yyval.intVal = CNTX; ;
    break;}
case 156:
#line 1576 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_ANY, myLineNoG);
    ;
    break;}
case 157:
#line 1580 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_ANYDEFINEDBY, myLineNoG);
        yyval.typePtr->basicType->a.anyDefinedBy = MT (AnyDefinedByType);
        yyval.typePtr->basicType->a.anyDefinedBy->fieldName = yyvsp[0].charPtr;
    ;
    break;}
case 158:
#line 1589 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_OCTETCONTAINING, myLineNoG);
        yyval.typePtr->basicType->a.stringContaining = yyvsp[0].typePtr;
    ;
    break;}
case 159:
#line 1597 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_BITCONTAINING, myLineNoG);
        yyval.typePtr->basicType->a.stringContaining = yyvsp[0].typePtr;
    ;
    break;}
case 160:
#line 1605 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_OID, myLineNoG);
    ;
    break;}
case 161:
#line 1612 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_RELATIVE_OID, myLineNoG);
	;
    break;}
case 162:
#line 1619 "core/parse-asn1.y"
{
      SetupType (&yyval.typePtr, BASICTYPE_GENERALIZEDTIME, myLineNoG);
   ;
    break;}
case 163:
#line 1623 "core/parse-asn1.y"
{
      SetupType (&yyval.typePtr, BASICTYPE_UTCTIME, myLineNoG);
   ;
    break;}
case 164:
#line 1627 "core/parse-asn1.y"
{
      SetupType (&yyval.typePtr, BASICTYPE_OBJECTDESCRIPTOR, myLineNoG);
   ;
    break;}
case 165:
#line 1634 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_NUMERIC_STR, myLineNoG);
	;
    break;}
case 166:
#line 1638 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_PRINTABLE_STR, myLineNoG);
	;
    break;}
case 167:
#line 1642 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_IA5_STR, myLineNoG);
	;
    break;}
case 168:
#line 1646 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_BMP_STR, myLineNoG);
	;
    break;}
case 169:
#line 1650 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_UNIVERSAL_STR, myLineNoG);
	;
    break;}
case 170:
#line 1654 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_UTF8_STR, myLineNoG);
	;
    break;}
case 171:
#line 1658 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_T61_STR, myLineNoG);
	;
    break;}
case 172:
#line 1662 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_T61_STR, myLineNoG);
	;
    break;}
case 173:
#line 1666 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_VIDEOTEX_STR, myLineNoG);
	;
    break;}
case 174:
#line 1670 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_VISIBLE_STR, myLineNoG);
	;
    break;}
case 175:
#line 1674 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_VISIBLE_STR, myLineNoG);
	;
    break;}
case 176:
#line 1678 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_GRAPHIC_STR, myLineNoG);
	;
    break;}
case 177:
#line 1682 "core/parse-asn1.y"
{
		SetupType (&yyval.typePtr, BASICTYPE_GENERAL_STR, myLineNoG);
	;
    break;}
case 178:
#line 1689 "core/parse-asn1.y"
{
        /*
         * append new subtype list to existing one (s) if any
         * with AND relation
         */
        AppendSubtype (&yyvsp[-1].typePtr->subtypes, yyvsp[0].subtypePtr, SUBTYPE_AND);
        yyval.typePtr = yyvsp[-1].typePtr;
    ;
    break;}
case 179:
#line 1698 "core/parse-asn1.y"
{
        Subtype *s;

        SetupType (&yyval.typePtr, BASICTYPE_SETOF, myLineNoG);
        yyval.typePtr->basicType->a.setOf = yyvsp[0].typePtr;

        /* add size constraint */
        s = MT (Subtype);
        s->choiceId = SUBTYPE_SINGLE;
        s->a.single = yyvsp[-2].subtypeValuePtr;
        AppendSubtype (&yyval.typePtr->subtypes, s, SUBTYPE_AND);
    ;
    break;}
case 180:
#line 1711 "core/parse-asn1.y"
{
        Subtype *s;

        SetupType (&yyval.typePtr, BASICTYPE_SEQUENCEOF, myLineNoG);
        yyval.typePtr->basicType->a.sequenceOf = yyvsp[0].typePtr;

        /* add size constraint */
        s = MT (Subtype);
        s->choiceId = SUBTYPE_SINGLE;
        s->a.single = yyvsp[-2].subtypeValuePtr;
        AppendSubtype (&yyval.typePtr->subtypes, s, SUBTYPE_AND);
    ;
    break;}
case 181:
#line 1728 "core/parse-asn1.y"
{
        yyval.subtypePtr = yyvsp[-1].subtypePtr;
    ;
    break;}
case 182:
#line 1735 "core/parse-asn1.y"
{
		yyval.subtypePtr = MT (Subtype);
		yyval.subtypePtr->choiceId = SUBTYPE_SINGLE;
		yyval.subtypePtr->a.single = yyvsp[0].subtypeValuePtr;

/*		Subtype *s;

		OR relation between all elmts of in	ValueSetList *

		$$ = MT (Subtype);
		$$->choiceId = SUBTYPE_OR;
		$$->a.or = NEWLIST();

		s = MT (Subtype);
		s->choiceId = SUBTYPE_SINGLE;
		s->a.single = $1;
		APPEND (s, $$->a.or);
		*/
	;
    break;}
case 183:
#line 1755 "core/parse-asn1.y"
{
		Subtype* s = MT (Subtype);
		s->choiceId = SUBTYPE_SINGLE;
		s->a.single = yyvsp[0].subtypeValuePtr;

		if (yyvsp[-2].subtypePtr->choiceId == SUBTYPE_OR)
			yyval.subtypePtr = yyvsp[-2].subtypePtr;
		else
		{
			yyval.subtypePtr = MT (Subtype);
			yyval.subtypePtr->choiceId = SUBTYPE_OR;
			APPEND(yyvsp[-2].subtypePtr, yyval.subtypePtr->a.or);
		}

		APPEND(s, yyval.subtypePtr->a.or);
	;
    break;}
case 191:
#line 1786 "core/parse-asn1.y"
{
		yyval.subtypeValuePtr = MT (SubtypeValue);	
	;
    break;}
case 192:
#line 1792 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_SINGLEVALUE;
        yyval.subtypeValuePtr->a.singleValue = yyvsp[0].valuePtr;
    ;
    break;}
case 193:
#line 1800 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_CONTAINED;
        yyval.subtypeValuePtr->a.contained = yyvsp[0].typePtr;
    ;
    break;}
case 194:
#line 1809 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_VALUERANGE;
        yyval.subtypeValuePtr->a.valueRange = MT (ValueRangeSubtype);
        yyval.subtypeValuePtr->a.valueRange->lowerEndValue  = yyvsp[-3].valueRangeEndpointPtr;
        yyval.subtypeValuePtr->a.valueRange->upperEndValue = yyvsp[0].valueRangeEndpointPtr;
    ;
    break;}
case 195:
#line 1820 "core/parse-asn1.y"
{
       yyval.valueRangeEndpointPtr = MT (ValueRangeEndpoint);
	   yyval.valueRangeEndpointPtr->valueInclusive = TRUE;
	   yyval.valueRangeEndpointPtr->endValue = yyvsp[0].valuePtr;
    ;
    break;}
case 196:
#line 1826 "core/parse-asn1.y"
{
       yyval.valueRangeEndpointPtr = MT (ValueRangeEndpoint);
	   yyval.valueRangeEndpointPtr->valueInclusive = FALSE;
	   yyval.valueRangeEndpointPtr->endValue = yyvsp[-1].valuePtr;
    ;
    break;}
case 197:
#line 1835 "core/parse-asn1.y"
{
       yyval.valueRangeEndpointPtr = MT (ValueRangeEndpoint);
	   yyval.valueRangeEndpointPtr->valueInclusive = TRUE;
	   yyval.valueRangeEndpointPtr->endValue = yyvsp[0].valuePtr;
    ;
    break;}
case 198:
#line 1841 "core/parse-asn1.y"
{
       yyval.valueRangeEndpointPtr = MT (ValueRangeEndpoint);
	   yyval.valueRangeEndpointPtr->valueInclusive = FALSE;
	   yyval.valueRangeEndpointPtr->endValue = yyvsp[0].valuePtr;
    ;
    break;}
case 199:
#line 1849 "core/parse-asn1.y"
{ yyval.valuePtr = yyvsp[0].valuePtr; ;
    break;}
case 200:
#line 1851 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_SPECIALINTEGER, myLineNoG);
        yyval.valuePtr->basicValue->a.specialInteger =  MIN_INT;
    ;
    break;}
case 201:
#line 1858 "core/parse-asn1.y"
{ yyval.valuePtr = yyvsp[0].valuePtr; ;
    break;}
case 202:
#line 1860 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_SPECIALINTEGER, myLineNoG);
        yyval.valuePtr->basicValue->a.specialInteger =  MAX_INT;
    ;
    break;}
case 203:
#line 1868 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_SIZECONSTRAINT;
        yyval.subtypeValuePtr->a.sizeConstraint = yyvsp[0].subtypePtr;
    ;
    break;}
case 204:
#line 1878 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_PERMITTEDALPHABET;
        yyval.subtypeValuePtr->a.permittedAlphabet = yyvsp[0].subtypePtr;
    ;
    break;}
case 205:
#line 1887 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_INNERSUBTYPE;
        yyval.subtypeValuePtr->a.innerSubtype = yyvsp[0].innerSubtypePtr;
    ;
    break;}
case 206:
#line 1893 "core/parse-asn1.y"
{
        yyval.subtypeValuePtr = MT (SubtypeValue);
        yyval.subtypeValuePtr->choiceId = SUBTYPEVALUE_INNERSUBTYPE;
        yyval.subtypeValuePtr->a.innerSubtype = yyvsp[0].innerSubtypePtr;
    ;
    break;}
case 207:
#line 1902 "core/parse-asn1.y"
{
        Constraint *constraint;

        /* this constrains the elmt of setof or seq of */
        yyval.innerSubtypePtr = MT (InnerSubtype);
        yyval.innerSubtypePtr->constraintType = SINGLE_CT;
        yyval.innerSubtypePtr->constraints = NEWLIST();
        constraint = MT (Constraint);
        APPEND (constraint, yyval.innerSubtypePtr->constraints);
        constraint->valueConstraints = yyvsp[0].subtypePtr;
    ;
    break;}
case 210:
#line 1922 "core/parse-asn1.y"
{
        yyval.innerSubtypePtr = MT (InnerSubtype);
        yyval.innerSubtypePtr->constraintType = FULL_CT;
        yyval.innerSubtypePtr->constraints = yyvsp[-1].constraintListPtr;
    ;
    break;}
case 211:
#line 1931 "core/parse-asn1.y"
{
        yyval.innerSubtypePtr = MT (InnerSubtype);
        yyval.innerSubtypePtr->constraintType = PARTIAL_CT;
        yyval.innerSubtypePtr->constraints = yyvsp[-1].constraintListPtr;
    ;
    break;}
case 212:
#line 1941 "core/parse-asn1.y"
{
        yyval.constraintListPtr = NEWLIST();
        APPEND (yyvsp[0].constraintPtr, yyval.constraintListPtr);
    ;
    break;}
case 213:
#line 1946 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].constraintPtr, yyvsp[-2].constraintListPtr);
        yyval.constraintListPtr = yyvsp[-2].constraintListPtr;
    ;
    break;}
case 214:
#line 1954 "core/parse-asn1.y"
{
        yyval.constraintPtr = yyvsp[0].constraintPtr;
        yyval.constraintPtr->fieldRef = yyvsp[-1].charPtr;
    ;
    break;}
case 216:
#line 1964 "core/parse-asn1.y"
{
        yyval.constraintPtr = MT (Constraint);
        yyval.constraintPtr->presenceConstraint = yyvsp[0].intVal;
        yyval.constraintPtr->valueConstraints = yyvsp[-1].subtypePtr;
    ;
    break;}
case 217:
#line 1972 "core/parse-asn1.y"
{ yyval.subtypePtr = yyvsp[0].subtypePtr; ;
    break;}
case 218:
#line 1973 "core/parse-asn1.y"
{ yyval.subtypePtr = NULL; ;
    break;}
case 219:
#line 1977 "core/parse-asn1.y"
{ yyval.intVal = PRESENT_CT; ;
    break;}
case 220:
#line 1978 "core/parse-asn1.y"
{ yyval.intVal = ABSENT_CT; ;
    break;}
case 221:
#line 1979 "core/parse-asn1.y"
{ yyval.intVal = EMPTY_CT; ;
    break;}
case 222:
#line 1980 "core/parse-asn1.y"
{ yyval.intVal = OPTIONAL_CT; ;
    break;}
case 223:
#line 1994 "core/parse-asn1.y"
{
        yyval.valueDefPtr = MT (ValueDef);
        yyval.valueDefPtr->definedName = yyvsp[-4].charPtr;
        yyval.valueDefPtr->value = yyvsp[0].valuePtr;
        yyval.valueDefPtr->value->lineNo = yyvsp[-1].intVal;
        yyval.valueDefPtr->value->type = yyvsp[-3].typePtr;
    ;
    break;}
case 226:
#line 2010 "core/parse-asn1.y"
{ yyval.valuePtr =  yyvsp[0].valuePtr; ;
    break;}
case 227:
#line 2012 "core/parse-asn1.y"
{
        /*
         * for parse, may be set to BASICVALUE_IMPORTEDTYPEREF
         * by linker
         */
        SetupValue (&yyval.valuePtr, BASICVALUE_LOCALVALUEREF, myLineNoG);
        yyval.valuePtr->basicValue->a.localValueRef = MT (ValueRef);
        yyval.valuePtr->basicValue->a.localValueRef->valueName = yyvsp[0].charPtr;
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
   ;
    break;}
case 228:
#line 2026 "core/parse-asn1.y"
{
        /* Alloc value with basicValue of importValueRef */
        SetupValue (&yyval.valuePtr, BASICVALUE_IMPORTVALUEREF, yyvsp[-1].intVal);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.importValueRef = MT (ValueRef);
        yyval.valuePtr->basicValue->a.importValueRef->valueName = yyvsp[0].charPtr;
        yyval.valuePtr->basicValue->a.importValueRef->moduleName = yyvsp[-3].charPtr;

        /* add entry to this module's import list */
        AddPrivateImportElmt (modulePtrG, yyvsp[0].charPtr, yyvsp[-3].charPtr, yyvsp[-1].intVal);
    ;
    break;}
case 232:
#line 2044 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_INTEGER, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.integer = yyvsp[0].intVal;
    ;
    break;}
case 233:
#line 2050 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_ASCIIHEX, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.asciiHex = MT (AsnOcts);
        yyval.valuePtr->basicValue->a.asciiHex->octs = yyvsp[0].charPtr;
        yyval.valuePtr->basicValue->a.asciiHex->octetLen = strlen (yyvsp[0].charPtr);
    ;
    break;}
case 234:
#line 2058 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_ASCIIBITSTRING, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.asciiBitString = MT (AsnOcts);
        yyval.valuePtr->basicValue->a.asciiBitString->octs = yyvsp[0].charPtr;
        yyval.valuePtr->basicValue->a.asciiBitString->octetLen = strlen (yyvsp[0].charPtr);
    ;
    break;}
case 235:
#line 2066 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_ASCIITEXT, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.asciiText = MT (AsnOcts);
        yyval.valuePtr->basicValue->a.asciiText->octs = yyvsp[0].charPtr;
        yyval.valuePtr->basicValue->a.asciiText->octetLen = strlen (yyvsp[0].charPtr);
    ;
    break;}
case 236:
#line 2073 "core/parse-asn1.y"
{ LexBeginBraceBalContext(); ;
    break;}
case 237:
#line 2074 "core/parse-asn1.y"
{
        /*
         *  LEXICAL TIE IN!!
         * string returned by BRACEBAL_SYM has
         * the $1 '{' prepended and includes everything
         * upto and including '}' that balances $1
         */
        LexBeginInitialContext();
        SetupValue (&yyval.valuePtr, BASICVALUE_VALUENOTATION, myLineNoG);
        yyval.valuePtr->basicValue->a.valueNotation = MT (AsnOcts);
        yyval.valuePtr->basicValue->a.valueNotation->octs = yyvsp[0].charPtr;
        yyval.valuePtr->basicValue->a.valueNotation->octetLen = strlen (yyvsp[0].charPtr);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
    ;
    break;}
case 238:
#line 2092 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_BOOLEAN, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.boolean =  TRUE;
    ;
    break;}
case 239:
#line 2098 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_BOOLEAN, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.boolean = FALSE;
    ;
    break;}
case 240:
#line 2108 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_SPECIALREAL, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.specialReal =  PLUS_INFINITY_REAL;
    ;
    break;}
case 241:
#line 2114 "core/parse-asn1.y"
{
        SetupValue (&yyval.valuePtr, BASICVALUE_SPECIALREAL, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
        yyval.valuePtr->basicValue->a.specialReal = MINUS_INFINITY_REAL;
    ;
    break;}
case 242:
#line 2125 "core/parse-asn1.y"
{
        /* create a NULL value  */
        SetupValue (&yyval.valuePtr, BASICVALUE_NULL, myLineNoG);
        yyval.valuePtr->valueType = BASICTYPE_UNKNOWN;
    ;
    break;}
case 243:
#line 2135 "core/parse-asn1.y"
{
        yyval.namedValuePtr = MT (NamedValue);
        yyval.namedValuePtr->value = yyvsp[0].valuePtr;
    ;
    break;}
case 244:
#line 2140 "core/parse-asn1.y"
{
        yyval.namedValuePtr = MT (NamedValue);
        yyval.namedValuePtr->value = yyvsp[0].valuePtr;
        yyval.namedValuePtr->fieldName = yyvsp[-1].charPtr;
    ;
    break;}
case 245:
#line 2150 "core/parse-asn1.y"
{
        /*
         * example OID setup
         *
         * for { ccitt foo (1) bar bell (bunt) 2 }
         *
         * ccitt
         *   - arcnum is set to number from oid table (oid.c)
         * foo (1)
         *   - sets up a new value def foo defined as 1
         *   - makes oid valueref a value ref to foo (doesn't link it tho)
         * bar
         *   - makes oid valueref a value ref to bar (doesn't link it tho)
         * bell (bunt)
         *   - sets up a new value def bell defined as a val ref to bunt
         *   - makes oid valueref a value ref to bell (doesn't link it tho)
         * 2
         *  - arcnum is set to 2
         */

        yyval.oidPtr = yyvsp[-1].oidPtr;
    ;
    break;}
case 246:
#line 2177 "core/parse-asn1.y"
{
        OID *o;
        /* append component */
        for (o = yyvsp[-1].oidPtr; o->next != NULL; o = o->next)
	    ;
        o->next = yyvsp[0].oidPtr;
        yyval.oidPtr = yyvsp[-1].oidPtr;
    ;
    break;}
case 249:
#line 2193 "core/parse-asn1.y"
{
        Value *newVal;
        /*
         * if the arcName is a defined arc name like
         * ccitt or iso etc, fill in the arc number.
         * otherwise make a value ref to that named value
         */
        yyval.oidPtr = MT (OID);

        yyval.oidPtr->arcNum = OidArcNameToNum (yyvsp[0].charPtr);
        if (yyval.oidPtr->arcNum == NULL_OID_ARCNUM)
        {
            /* set up value ref to named value */
            SetupValue (&newVal, BASICVALUE_LOCALVALUEREF, myLineNoG);
            newVal->basicValue->a.localValueRef = MT (ValueRef);
            newVal->valueType = BASICTYPE_INTEGER;
            newVal->basicValue->a.localValueRef->valueName = yyvsp[0].charPtr;
            yyval.oidPtr->valueRef = newVal;
        }
    ;
    break;}
case 251:
#line 2219 "core/parse-asn1.y"
{
        yyval.oidPtr = MT (OID);
        yyval.oidPtr->arcNum = yyvsp[0].intVal;
    ;
    break;}
case 253:
#line 2232 "core/parse-asn1.y"
{
/*		Value *newVal;	// REN -- 9/23/02 */

		yyval.oidPtr = yyvsp[-1].oidPtr;

		/* shared refs to named numbers name
// REN -- 9/23/02 --
		SetupValue (&newVal, BASICVALUE_INTEGER, myLineNoG);
		newVal->basicValue->a.integer = $$->arcNum;
		newVal->valueType = BASICTYPE_INTEGER;
		AddNewValueDef (oidElmtValDefsG, $1, newVal);
		SetupValue (&newVal, BASICVALUE_LOCALVALUEREF, myLineNoG);
		newVal->basicValue->a.localValueRef = MT (ValueRef);
		newVal->basicValue->a.localValueRef->valueName = $1;

		$$->valueRef = newVal;
*/
		Free(yyvsp[-3].charPtr);		// REN -- 9/23/02 
	;
    break;}
case 254:
#line 2252 "core/parse-asn1.y"
{
/*		Value *newVal;	// REN -- 9/23/02 */

		/* shared refs to named numbers name */
		yyval.oidPtr = MT (OID);
		yyval.oidPtr->arcNum = NULL_OID_ARCNUM;

/*		AddNewValueDef (oidElmtValDefsG, $1, $3);	// REN -- 9/23/02 */

		yyval.oidPtr->valueRef = yyvsp[-1].valuePtr;

		Free(yyvsp[-3].charPtr);		// REN -- 9/23/02 
	;
    break;}
case 258:
#line 2283 "core/parse-asn1.y"
{
	if (yyvsp[0].uintVal>0x7FFFFFFF) {
	    yyerror("Warning: number out of range");
	    yyval.intVal = 0x7FFFFFFF;
	}
	;
    break;}
case 259:
#line 2290 "core/parse-asn1.y"
{
	yyerror ("Warning: number out of range");
	yyval.intVal = 0x7FFFFFFF;
	/* modulePtrG->status = MOD_ERROR; */
	;
    break;}
case 265:
#line 2318 "core/parse-asn1.y"
{ yyval.directiveList = NULL; ;
    break;}
case 266:
#line 2323 "core/parse-asn1.y"
{
        yyval.directiveList = NEWLIST();
        APPEND (yyvsp[0].directivePtr,yyval.directiveList);
	;
    break;}
case 267:
#line 2328 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].directivePtr,yyvsp[-1].directiveList);
        yyval.directiveList = yyvsp[-1].directiveList;
    ;
    break;}
case 268:
#line 2336 "core/parse-asn1.y"
{
		yyval.directivePtr = MT (SnaccDirective);
		yyval.directivePtr->type = ASN1_TypeID;
		yyval.directivePtr->value.asnTypeVal = yyvsp[0].basicTypeChoiceIdEnum;
	;
    break;}
case 269:
#line 2342 "core/parse-asn1.y"
{
		yyval.directivePtr = MT (SnaccDirective);
		yyval.directivePtr->type = C_TypeID;
		yyval.directivePtr->value.cTypeVal = yyvsp[0].cTypeIdEnum;
	;
    break;}
case 270:
#line 2348 "core/parse-asn1.y"
{
		yyval.directivePtr = MT (SnaccDirective);
		yyval.directivePtr->type = yyvsp[-2].directiveEnum;
		yyval.directivePtr->value.boolVal = yyvsp[0].boolVal;
	;
    break;}
case 271:
#line 2354 "core/parse-asn1.y"
{
		yyval.directivePtr = MT (SnaccDirective);
		yyval.directivePtr->type = yyvsp[-2].directiveEnum;
		yyval.directivePtr->value.stringVal = yyvsp[0].charPtr;
	;
    break;}
case 272:
#line 2360 "core/parse-asn1.y"
{
		yyval.directivePtr = MT (SnaccDirective);
		yyval.directivePtr->type = yyvsp[-2].directiveEnum;
		yyval.directivePtr->value.integerVal = yyvsp[0].uintVal;
	;
    break;}
case 273:
#line 2368 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_UNKNOWN; ;
    break;}
case 274:
#line 2369 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_BOOLEAN; ;
    break;}
case 275:
#line 2370 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_INTEGER; ;
    break;}
case 276:
#line 2371 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_BITSTRING; ;
    break;}
case 277:
#line 2372 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_OCTETSTRING; ;
    break;}
case 278:
#line 2373 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_NULL; ;
    break;}
case 279:
#line 2374 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_OID; ;
    break;}
case 280:
#line 2375 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_REAL; ;
    break;}
case 281:
#line 2376 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_ENUMERATED; ;
    break;}
case 282:
#line 2377 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_SEQUENCE; ;
    break;}
case 283:
#line 2378 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_SEQUENCEOF; ;
    break;}
case 284:
#line 2379 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_SET; ;
    break;}
case 285:
#line 2380 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_SETOF; ;
    break;}
case 286:
#line 2381 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_CHOICE; ;
    break;}
case 287:
#line 2382 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_ANY; ;
    break;}
case 288:
#line 2383 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_ANYDEFINEDBY; ;
    break;}
case 289:
#line 2384 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_LOCALTYPEREF; ;
    break;}
case 290:
#line 2385 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_IMPORTTYPEREF; ;
    break;}
case 291:
#line 2386 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_NUMERIC_STR; ;
    break;}
case 292:
#line 2387 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_PRINTABLE_STR; ;
    break;}
case 293:
#line 2388 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_IA5_STR; ;
    break;}
case 294:
#line 2389 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_BMP_STR; ;
    break;}
case 295:
#line 2390 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_UNIVERSAL_STR; ;
    break;}
case 296:
#line 2391 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_UTF8_STR; ;
    break;}
case 297:
#line 2392 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_T61_STR; ;
    break;}
case 298:
#line 2393 "core/parse-asn1.y"
{ yyval.basicTypeChoiceIdEnum = BASICTYPE_RELATIVE_OID; ;
    break;}
case 299:
#line 2397 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_CHOICE; ;
    break;}
case 300:
#line 2398 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_LIST; ;
    break;}
case 301:
#line 2399 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_ANY; ;
    break;}
case 302:
#line 2400 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_ANYDEFINEDBY; ;
    break;}
case 303:
#line 2401 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_LIB; ;
    break;}
case 304:
#line 2402 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_STRUCT; ;
    break;}
case 305:
#line 2403 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_TYPEDEF; ;
    break;}
case 306:
#line 2404 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_TYPEREF; ;
    break;}
case 307:
#line 2405 "core/parse-asn1.y"
{ yyval.cTypeIdEnum = C_NO_TYPE; ;
    break;}
case 308:
#line 2409 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPDU; ;
    break;}
case 309:
#line 2410 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPtr; ;
    break;}
case 310:
#line 2411 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPtrForTypeDef; ;
    break;}
case 311:
#line 2412 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPtrForTypeRef; ;
    break;}
case 312:
#line 2413 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPtrInChoice; ;
    break;}
case 313:
#line 2414 "core/parse-asn1.y"
{ yyval.directiveEnum = IsPtrForOpt; ;
    break;}
case 314:
#line 2415 "core/parse-asn1.y"
{ yyval.directiveEnum = IsEncDec; ;
    break;}
case 315:
#line 2416 "core/parse-asn1.y"
{ yyval.directiveEnum = GenTypeDef; ;
    break;}
case 316:
#line 2417 "core/parse-asn1.y"
{ yyval.directiveEnum = GenPrintRoutine; ;
    break;}
case 317:
#line 2418 "core/parse-asn1.y"
{ yyval.directiveEnum = GenEncodeRoutine; ;
    break;}
case 318:
#line 2419 "core/parse-asn1.y"
{ yyval.directiveEnum = GenDecodeRoutine; ;
    break;}
case 319:
#line 2420 "core/parse-asn1.y"
{ yyval.directiveEnum = GenFreeRoutine; ;
    break;}
case 320:
#line 2421 "core/parse-asn1.y"
{ yyval.directiveEnum = IsBigInt; ;
    break;}
case 321:
#line 2425 "core/parse-asn1.y"
{ yyval.boolVal = TRUE; ;
    break;}
case 322:
#line 2426 "core/parse-asn1.y"
{ yyval.boolVal = FALSE; ;
    break;}
case 323:
#line 2430 "core/parse-asn1.y"
{ yyval.directiveEnum = C_TypeName; ;
    break;}
case 324:
#line 2431 "core/parse-asn1.y"
{ yyval.directiveEnum = C_FieldName; ;
    break;}
case 325:
#line 2432 "core/parse-asn1.y"
{ yyval.directiveEnum = OptionalTestRoutineName; ;
    break;}
case 326:
#line 2433 "core/parse-asn1.y"
{ yyval.directiveEnum = DefaultFieldName; ;
    break;}
case 327:
#line 2434 "core/parse-asn1.y"
{ yyval.directiveEnum = PrintRoutineName; ;
    break;}
case 328:
#line 2435 "core/parse-asn1.y"
{ yyval.directiveEnum = EncodeRoutineName; ;
    break;}
case 329:
#line 2436 "core/parse-asn1.y"
{ yyval.directiveEnum = DecodeRoutineName; ;
    break;}
case 330:
#line 2437 "core/parse-asn1.y"
{ yyval.directiveEnum = FreeRoutineName; ;
    break;}
case 331:
#line 2438 "core/parse-asn1.y"
{ yyval.directiveEnum = ChoiceIdSymbol; ;
    break;}
case 332:
#line 2439 "core/parse-asn1.y"
{ yyval.directiveEnum = ChoiceIdEnumName; ;
    break;}
case 333:
#line 2440 "core/parse-asn1.y"
{ yyval.directiveEnum = ChoiceIdEnumFieldName; ;
    break;}
case 334:
#line 2444 "core/parse-asn1.y"
{ yyval.directiveEnum = ChoiceIdValue; ;
    break;}
case 360:
#line 2482 "core/parse-asn1.y"
{ yyval.charPtr = "OPERATION"; ;
    break;}
case 361:
#line 2483 "core/parse-asn1.y"
{ yyval.charPtr = "ERROR"; ;
    break;}
case 362:
#line 2484 "core/parse-asn1.y"
{ yyval.charPtr = "BIND"; ;
    break;}
case 363:
#line 2485 "core/parse-asn1.y"
{ yyval.charPtr = "UNBIND"; ;
    break;}
case 364:
#line 2486 "core/parse-asn1.y"
{ yyval.charPtr = "APPLICATION-SERVICE-ELEMENT"; ;
    break;}
case 365:
#line 2487 "core/parse-asn1.y"
{ yyval.charPtr = "APPLICATION-CONTEXT"; ;
    break;}
case 366:
#line 2488 "core/parse-asn1.y"
{ yyval.charPtr = "EXTENSION"; ;
    break;}
case 367:
#line 2489 "core/parse-asn1.y"
{ yyval.charPtr = "EXTENSIONS"; ;
    break;}
case 368:
#line 2490 "core/parse-asn1.y"
{ yyval.charPtr = "EXTENSION-ATTRIBUTE"; ;
    break;}
case 369:
#line 2491 "core/parse-asn1.y"
{ yyval.charPtr = "TOKEN"; ;
    break;}
case 370:
#line 2492 "core/parse-asn1.y"
{ yyval.charPtr = "TOKEN-DATA"; ;
    break;}
case 371:
#line 2493 "core/parse-asn1.y"
{ yyval.charPtr = "SECURITY-CATEGORY"; ;
    break;}
case 372:
#line 2494 "core/parse-asn1.y"
{ yyval.charPtr = "OBJECT"; ;
    break;}
case 373:
#line 2495 "core/parse-asn1.y"
{ yyval.charPtr = "PORT"; ;
    break;}
case 374:
#line 2496 "core/parse-asn1.y"
{ yyval.charPtr = "REFINE"; ;
    break;}
case 375:
#line 2497 "core/parse-asn1.y"
{ yyval.charPtr = "ABSTRACT-BIND"; ;
    break;}
case 376:
#line 2498 "core/parse-asn1.y"
{ yyval.charPtr = "ABSTRACT-UNBIND"; ;
    break;}
case 377:
#line 2499 "core/parse-asn1.y"
{ yyval.charPtr = "ABSTRACT-OPERATION"; ;
    break;}
case 378:
#line 2500 "core/parse-asn1.y"
{ yyval.charPtr = "ABSTRACT-ERROR"; ;
    break;}
case 379:
#line 2501 "core/parse-asn1.y"
{ yyval.charPtr = "ALGORITHM"; ;
    break;}
case 380:
#line 2502 "core/parse-asn1.y"
{ yyval.charPtr = "ENCRYPTED"; ;
    break;}
case 381:
#line 2503 "core/parse-asn1.y"
{ yyval.charPtr = "SIGNED"; ;
    break;}
case 382:
#line 2504 "core/parse-asn1.y"
{ yyval.charPtr = "SIGNATURE"; ;
    break;}
case 383:
#line 2505 "core/parse-asn1.y"
{ yyval.charPtr = "PROTECTED"; ;
    break;}
case 384:
#line 2506 "core/parse-asn1.y"
{ yyval.charPtr = "OBJECT-TYPE"; ;
    break;}
case 385:
#line 2515 "core/parse-asn1.y"
{ yyval.typePtr = yyvsp[0].typePtr; ;
    break;}
case 386:
#line 2520 "core/parse-asn1.y"
{
        RosOperationMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSOPERATION, myLineNoG);
        r = yyval.typePtr->basicType->a.macroType->a.rosOperation  =
            MT (RosOperationMacroType);
        r->arguments = yyvsp[-3].namedTypePtr;
        r->result    = yyvsp[-2].namedTypePtr;
        r->errors    = yyvsp[-1].typeOrValueListPtr;
        r->linkedOps = yyvsp[0].typeOrValueListPtr;
    ;
    break;}
case 387:
#line 2535 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 388:
#line 2536 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 389:
#line 2540 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 390:
#line 2541 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 392:
#line 2547 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 393:
#line 2553 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = yyvsp[-1].typeOrValueListPtr;
    ;
    break;}
case 394:
#line 2556 "core/parse-asn1.y"
{ yyval.typeOrValueListPtr = NULL; ;
    break;}
case 395:
#line 2563 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = yyvsp[-1].typeOrValueListPtr;
    ;
    break;}
case 396:
#line 2566 "core/parse-asn1.y"
{ yyval.typeOrValueListPtr = NULL; ;
    break;}
case 397:
#line 2579 "core/parse-asn1.y"
{
        RosErrorMacroType *r;
        /*
         * defines error macro type
         */
        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSERROR, myLineNoG);
        r = yyval.typePtr->basicType->a.macroType->a.rosError = MT (RosErrorMacroType);
        r->parameter = yyvsp[0].namedTypePtr;
    ;
    break;}
case 398:
#line 2592 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 399:
#line 2593 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 400:
#line 2603 "core/parse-asn1.y"
{
        RosBindMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSBIND, myLineNoG);

        r = yyval.typePtr->basicType->a.macroType->a.rosBind = MT (RosBindMacroType);
        r->argument  = yyvsp[-2].namedTypePtr;
        r->result = yyvsp[-1].namedTypePtr;
        r->error  = yyvsp[0].namedTypePtr;
    ;
    break;}
case 401:
#line 2616 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 402:
#line 2617 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 403:
#line 2622 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 404:
#line 2623 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 405:
#line 2628 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 406:
#line 2629 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 407:
#line 2639 "core/parse-asn1.y"
{
        RosBindMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSUNBIND, myLineNoG);

        r = yyval.typePtr->basicType->a.macroType->a.rosUnbind = MT (RosBindMacroType);
        r->argument = yyvsp[-2].namedTypePtr;
        r->result = yyvsp[-1].namedTypePtr;
        r->error  = yyvsp[0].namedTypePtr;
    ;
    break;}
case 408:
#line 2653 "core/parse-asn1.y"
{ yyval.namedTypePtr = yyvsp[0].namedTypePtr; ;
    break;}
case 409:
#line 2654 "core/parse-asn1.y"
{ yyval.namedTypePtr = NULL; ;
    break;}
case 410:
#line 2664 "core/parse-asn1.y"
{
        RosAseMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSASE, myLineNoG);
        r = yyval.typePtr->basicType->a.macroType->a.rosAse  = MT (RosAseMacroType);
        r->operations = yyvsp[0].valueListPtr;
    ;
    break;}
case 411:
#line 2672 "core/parse-asn1.y"
{
        RosAseMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSASE, myLineNoG);
        r = yyval.typePtr->basicType->a.macroType->a.rosAse  = MT (RosAseMacroType);
        r->consumerInvokes = yyvsp[-1].valueListPtr;
        r->supplierInvokes = yyvsp[0].valueListPtr;
    ;
    break;}
case 412:
#line 2685 "core/parse-asn1.y"
{
        yyval.valueListPtr = yyvsp[-1].valueListPtr;
    ;
    break;}
case 413:
#line 2693 "core/parse-asn1.y"
{
        yyval.valueListPtr = yyvsp[-1].valueListPtr;
    ;
    break;}
case 414:
#line 2696 "core/parse-asn1.y"
{ yyval.valueListPtr = NULL; ;
    break;}
case 415:
#line 2702 "core/parse-asn1.y"
{
        yyval.valueListPtr = yyvsp[-1].valueListPtr;
    ;
    break;}
case 416:
#line 2705 "core/parse-asn1.y"
{ yyval.valueListPtr = NULL; ;
    break;}
case 418:
#line 2725 "core/parse-asn1.y"
{
        RosAcMacroType *r;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ROSAC, myLineNoG);
        r = yyval.typePtr->basicType->a.macroType->a.rosAc = MT (RosAcMacroType);
        r->nonRoElements = yyvsp[-6].valueListPtr;
        r->bindMacroType = yyvsp[-4].typePtr;
        r->unbindMacroType = yyvsp[-2].typePtr;
        r->remoteOperations = yyvsp[-1].valuePtr;
        r->operationsOf = rosAcSymmetricAsesG;
        r->initiatorConsumerOf = rosAcInitiatorConsumerOfG;
        r->responderConsumerOf = rosAcResponderConsumerOfG;
        r->abstractSyntaxes = yyvsp[0].oidListPtr;
    ;
    break;}
case 419:
#line 2744 "core/parse-asn1.y"
{
        yyval.valueListPtr = yyvsp[-1].valueListPtr;
    ;
    break;}
case 420:
#line 2753 "core/parse-asn1.y"
{
        yyval.valuePtr = yyvsp[-3].valuePtr;
    ;
    break;}
case 421:
#line 2757 "core/parse-asn1.y"
{
        yyval.valuePtr = NULL;
        rosAcSymmetricAsesG = NULL;
        rosAcInitiatorConsumerOfG = NULL;
        rosAcResponderConsumerOfG = NULL;
    ;
    break;}
case 422:
#line 2767 "core/parse-asn1.y"
{
        rosAcSymmetricAsesG = yyvsp[-1].valueListPtr;
    ;
    break;}
case 423:
#line 2770 "core/parse-asn1.y"
{ rosAcSymmetricAsesG = NULL; ;
    break;}
case 425:
#line 2779 "core/parse-asn1.y"
{
        rosAcInitiatorConsumerOfG = yyvsp[-1].valueListPtr;
    ;
    break;}
case 426:
#line 2782 "core/parse-asn1.y"
{ rosAcInitiatorConsumerOfG = NULL; ;
    break;}
case 427:
#line 2787 "core/parse-asn1.y"
{
        rosAcResponderConsumerOfG = yyvsp[-1].valueListPtr;
    ;
    break;}
case 428:
#line 2790 "core/parse-asn1.y"
{ rosAcResponderConsumerOfG = NULL; ;
    break;}
case 429:
#line 2795 "core/parse-asn1.y"
{
        yyval.oidListPtr = yyvsp[-1].oidListPtr;
    ;
    break;}
case 430:
#line 2798 "core/parse-asn1.y"
{ yyval.oidListPtr = NULL; ;
    break;}
case 431:
#line 2804 "core/parse-asn1.y"
{
        yyval.oidListPtr = NEWLIST();
        APPEND (yyvsp[0].oidPtr,yyval.oidListPtr);
    ;
    break;}
case 432:
#line 2809 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].oidPtr, yyvsp[-2].oidListPtr);
        yyval.oidListPtr = yyvsp[-2].oidListPtr;
    ;
    break;}
case 433:
#line 2823 "core/parse-asn1.y"
{
          MtsasExtensionsMacroType *m;

          SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASEXTENSIONS, myLineNoG);
          m = yyval.typePtr->basicType->a.macroType->a.mtsasExtensions =
              MT (MtsasExtensionsMacroType);
          m->extensions = yyvsp[-1].valueListPtr;
      ;
    break;}
case 435:
#line 2836 "core/parse-asn1.y"
{ yyval.valueListPtr = NULL; ;
    break;}
case 436:
#line 2841 "core/parse-asn1.y"
{
         yyval.valueListPtr = NEWLIST();
         APPEND (yyvsp[0].valuePtr, yyval.valueListPtr);
     ;
    break;}
case 437:
#line 2846 "core/parse-asn1.y"
{
         APPEND (yyvsp[0].valuePtr,yyvsp[-2].valueListPtr);
         yyval.valueListPtr = yyvsp[-2].valueListPtr;
     ;
    break;}
case 439:
#line 2854 "core/parse-asn1.y"
{ yyval.typeOrValueListPtr = NULL; ;
    break;}
case 440:
#line 2859 "core/parse-asn1.y"
{
         yyval.typeOrValueListPtr = NEWLIST();
         APPEND (yyvsp[0].typeOrValuePtr, yyval.typeOrValueListPtr);
     ;
    break;}
case 441:
#line 2864 "core/parse-asn1.y"
{
         APPEND (yyvsp[0].typeOrValuePtr,yyvsp[-2].typeOrValueListPtr);
         yyval.typeOrValueListPtr = yyvsp[-2].typeOrValueListPtr;
     ;
    break;}
case 442:
#line 2872 "core/parse-asn1.y"
{
         yyval.typeOrValuePtr = MT (TypeOrValue);
         yyval.typeOrValuePtr->choiceId = TYPEORVALUE_TYPE;
         yyval.typeOrValuePtr->a.type = yyvsp[0].typePtr;
     ;
    break;}
case 443:
#line 2878 "core/parse-asn1.y"
{
         yyval.typeOrValuePtr = MT (TypeOrValue);
         yyval.typeOrValuePtr->choiceId = TYPEORVALUE_VALUE;
         yyval.typeOrValuePtr->a.value = yyvsp[0].valuePtr;
     ;
    break;}
case 444:
#line 2891 "core/parse-asn1.y"
{
        MtsasExtensionMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASEXTENSION, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasExtension =
            MT (MtsasExtensionMacroType);
        m->elmtType = yyvsp[-2].namedTypePtr;
        m->defaultValue = yyvsp[-1].valuePtr;
        m->criticalForSubmission = mtsasCriticalForSubmissionG;
        m->criticalForTransfer = mtsasCriticalForTransferG;
        m->criticalForDelivery = mtsasCriticalForDeliveryG;

        mtsasCriticalForSubmissionG = NULL;  /* set up for next parse */
        mtsasCriticalForTransferG = NULL;
        mtsasCriticalForDeliveryG = NULL;
    ;
    break;}
case 445:
#line 2908 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASEXTENSION, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.mtsasExtension =
            MT (MtsasExtensionMacroType);
        /*
         * all fields are NULL in the MtsasExtensionsMacroType
         * for this production
         */
    ;
    break;}
case 446:
#line 2920 "core/parse-asn1.y"
{ yyval.valuePtr = yyvsp[0].valuePtr; ;
    break;}
case 447:
#line 2921 "core/parse-asn1.y"
{ yyval.valuePtr = NULL; ;
    break;}
case 452:
#line 2937 "core/parse-asn1.y"
{
        mtsasCriticalForSubmissionG = MT (AsnBool);
        *mtsasCriticalForSubmissionG = TRUE;
    ;
    break;}
case 453:
#line 2942 "core/parse-asn1.y"
{
        mtsasCriticalForTransferG = MT (AsnBool);
        *mtsasCriticalForTransferG = TRUE;
    ;
    break;}
case 454:
#line 2947 "core/parse-asn1.y"
{
        mtsasCriticalForDeliveryG = MT (AsnBool);
        *mtsasCriticalForDeliveryG = TRUE;
    ;
    break;}
case 455:
#line 2961 "core/parse-asn1.y"
{
        MtsasExtensionAttributeMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASEXTENSIONATTRIBUTE, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasExtensionAttribute =
            MT (MtsasExtensionAttributeMacroType);
        m->type = NULL;
    ;
    break;}
case 456:
#line 2970 "core/parse-asn1.y"
{
        MtsasExtensionAttributeMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASEXTENSIONATTRIBUTE, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasExtensionAttribute =
            MT (MtsasExtensionAttributeMacroType);
        m->type = yyvsp[0].typePtr;
    ;
    break;}
case 457:
#line 2986 "core/parse-asn1.y"
{
        MtsasTokenMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASTOKEN, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasToken = MT (MtsasTokenMacroType);
        m->type = NULL;
    ;
    break;}
case 458:
#line 2994 "core/parse-asn1.y"
{
        MtsasTokenMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASTOKEN, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasToken = MT (MtsasTokenMacroType);
        m->type = yyvsp[0].typePtr;
    ;
    break;}
case 459:
#line 3009 "core/parse-asn1.y"
{
        MtsasTokenDataMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASTOKENDATA, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasTokenData =
            MT (MtsasTokenDataMacroType);
        m->type = NULL;
    ;
    break;}
case 460:
#line 3018 "core/parse-asn1.y"
{
        MtsasTokenDataMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASTOKENDATA, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasTokenData =
            MT (MtsasTokenDataMacroType);
        m->type = yyvsp[0].typePtr;
    ;
    break;}
case 461:
#line 3034 "core/parse-asn1.y"
{
        MtsasSecurityCategoryMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASSECURITYCATEGORY, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasSecurityCategory =
            MT (MtsasSecurityCategoryMacroType);
        m->type = NULL;
    ;
    break;}
case 462:
#line 3043 "core/parse-asn1.y"
{
        MtsasSecurityCategoryMacroType *m;

        SetupMacroType (&yyval.typePtr, MACROTYPE_MTSASSECURITYCATEGORY, myLineNoG);
        m = yyval.typePtr->basicType->a.macroType->a.mtsasSecurityCategory =
            MT (MtsasSecurityCategoryMacroType);
        m->type = yyvsp[0].typePtr;
    ;
    break;}
case 463:
#line 3065 "core/parse-asn1.y"
{
        AsnObjectMacroType *a;
        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNOBJECT, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnObject = MT (AsnObjectMacroType);
        a->ports = yyvsp[0].asnPortListPtr;
    ;
    break;}
case 464:
#line 3075 "core/parse-asn1.y"
{
        yyval.asnPortListPtr = yyvsp[-1].asnPortListPtr;
    ;
    break;}
case 465:
#line 3078 "core/parse-asn1.y"
{ yyval.asnPortListPtr = NULL; ;
    break;}
case 466:
#line 3083 "core/parse-asn1.y"
{
        yyval.asnPortListPtr = NEWLIST();
        APPEND (yyvsp[0].asnPortPtr, yyval.asnPortListPtr);
    ;
    break;}
case 467:
#line 3088 "core/parse-asn1.y"
{
        APPEND (yyvsp[0].asnPortPtr, yyvsp[-2].asnPortListPtr);
        yyval.asnPortListPtr = yyvsp[-2].asnPortListPtr;
    ;
    break;}
case 468:
#line 3096 "core/parse-asn1.y"
{
        yyval.asnPortPtr = MT (AsnPort);
        yyval.asnPortPtr->portValue = yyvsp[-1].valuePtr;
        yyval.asnPortPtr->portType = yyvsp[0].intVal;
    ;
    break;}
case 469:
#line 3105 "core/parse-asn1.y"
{
        /* [C] consumer */
        yyval.intVal = CONSUMER_PORT;
    ;
    break;}
case 470:
#line 3110 "core/parse-asn1.y"
{
        /* [S] supplier */
        yyval.intVal = SUPPLIER_PORT;
    ;
    break;}
case 471:
#line 3115 "core/parse-asn1.y"
{
       /* symmetric */
        yyval.intVal = SYMMETRIC_PORT;
    ;
    break;}
case 472:
#line 3128 "core/parse-asn1.y"
{
        AsnPortMacroType *a;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNPORT, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnPort = MT (AsnPortMacroType);
        a->abstractOps = yyvsp[0].typeOrValueListPtr;
        a->consumerInvokes = asnConsumerG;
        a->supplierInvokes = asnSupplierG;
    ;
    break;}
case 473:
#line 3138 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNPORT, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.asnPort = MT (AsnPortMacroType);
    ;
    break;}
case 474:
#line 3147 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = yyvsp[-1].typeOrValueListPtr;
    ;
    break;}
case 475:
#line 3151 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = NULL;
        asnConsumerG = yyvsp[0].typeOrValueListPtr;
        asnSupplierG = NULL;
    ;
    break;}
case 476:
#line 3157 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = NULL;
        asnConsumerG = yyvsp[0].typeOrValueListPtr;
        asnSupplierG = NULL;
    ;
    break;}
case 477:
#line 3163 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = NULL;
        asnConsumerG = yyvsp[-1].typeOrValueListPtr;
        asnSupplierG = NULL;
    ;
    break;}
case 478:
#line 3169 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = NULL;
        asnConsumerG = yyvsp[-1].typeOrValueListPtr;
        asnSupplierG = NULL;
    ;
    break;}
case 479:
#line 3178 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = yyvsp[-1].typeOrValueListPtr;
    ;
    break;}
case 480:
#line 3185 "core/parse-asn1.y"
{
        yyval.typeOrValueListPtr = yyvsp[-1].typeOrValueListPtr;
    ;
    break;}
case 481:
#line 3201 "core/parse-asn1.y"
{
        SetupType (&yyval.typePtr, BASICTYPE_UNKNOWN, myLineNoG);
    ;
    break;}
case 489:
#line 3227 "core/parse-asn1.y"
{
       yyval.intVal = 0; /* just to quiet yacc warning */
    ;
    break;}
case 494:
#line 3245 "core/parse-asn1.y"
{
        yyval.intVal = 0; /* just to quiet yacc warning */
    ;
    break;}
case 495:
#line 3258 "core/parse-asn1.y"
{
        AsnAbstractBindMacroType *a;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNABSTRACTBIND, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnAbstractBind =
            MT (AsnAbstractBindMacroType);
        a->ports = yyvsp[0].asnPortListPtr;
    ;
    break;}
case 496:
#line 3267 "core/parse-asn1.y"
{
        AsnAbstractBindMacroType *a;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNABSTRACTBIND, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnAbstractBind =
            MT (AsnAbstractBindMacroType);
        a->ports = yyvsp[-1].asnPortListPtr;
        a->type = yyvsp[0].typePtr;
    ;
    break;}
case 497:
#line 3280 "core/parse-asn1.y"
{
        yyval.asnPortListPtr = yyvsp[-1].asnPortListPtr;
    ;
    break;}
case 498:
#line 3283 "core/parse-asn1.y"
{ yyval.asnPortListPtr = NULL; ;
    break;}
case 499:
#line 3294 "core/parse-asn1.y"
{
        AsnAbstractBindMacroType *a;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNABSTRACTUNBIND, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnAbstractUnbind =
            MT (AsnAbstractBindMacroType);

        a->ports = yyvsp[0].asnPortListPtr;
    ;
    break;}
case 500:
#line 3304 "core/parse-asn1.y"
{
        AsnAbstractBindMacroType *a;

        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNABSTRACTUNBIND, myLineNoG);
        a = yyval.typePtr->basicType->a.macroType->a.asnAbstractUnbind =
            MT (AsnAbstractBindMacroType);

        a->ports = yyvsp[-1].asnPortListPtr;
        a->type = yyvsp[0].typePtr;
    ;
    break;}
case 501:
#line 3318 "core/parse-asn1.y"
{
        yyval.asnPortListPtr = yyvsp[-1].asnPortListPtr;
    ;
    break;}
case 502:
#line 3321 "core/parse-asn1.y"
{ yyval.asnPortListPtr = NULL; ;
    break;}
case 503:
#line 3331 "core/parse-asn1.y"
{
       yyval.typePtr = yyvsp[0].typePtr;
       yyvsp[0].typePtr->basicType->a.macroType->choiceId = MACROTYPE_ASNABSTRACTOPERATION;
    ;
    break;}
case 504:
#line 3343 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_ASNABSTRACTERROR, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.asnAbstractError = MT (RosErrorMacroType);
        yyval.typePtr->basicType->a.macroType->a.asnAbstractError->parameter = yyvsp[0].namedTypePtr;
    ;
    break;}
case 505:
#line 3356 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_AFALGORITHM, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.afAlgorithm = yyvsp[0].typePtr;
    ;
    break;}
case 506:
#line 3367 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_AFENCRYPTED, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.afEncrypted = yyvsp[0].typePtr;
    ;
    break;}
case 507:
#line 3379 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_AFSIGNED, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.afSigned = yyvsp[0].typePtr;
    ;
    break;}
case 508:
#line 3390 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_AFSIGNATURE, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.afSignature = yyvsp[0].typePtr;
    ;
    break;}
case 509:
#line 3404 "core/parse-asn1.y"
{
        SetupMacroType (&yyval.typePtr, MACROTYPE_AFPROTECTED, myLineNoG);
        yyval.typePtr->basicType->a.macroType->a.afProtected = yyvsp[0].typePtr;
    ;
    break;}
case 510:
#line 3421 "core/parse-asn1.y"
{
        SnmpObjectTypeMacroType *s;

        SetupMacroType (&yyval.typePtr, MACROTYPE_SNMPOBJECTTYPE, myLineNoG);
        s = yyval.typePtr->basicType->a.macroType->a.snmpObjectType =
            MT (SnmpObjectTypeMacroType);

        s->syntax = yyvsp[-8].typePtr;
        s->access = yyvsp[-6].intVal;
        s->status = yyvsp[-4].intVal;
        s->description = yyvsp[-3].valuePtr;
        s->reference = yyvsp[-2].valuePtr;
        s->index = yyvsp[-1].typeOrValueListPtr;
        s->defVal = yyvsp[0].valuePtr;
    ;
    break;}
case 511:
#line 3440 "core/parse-asn1.y"
{
        if (strcmp (yyvsp[0].charPtr, "read-only") == 0)
            yyval.intVal = SNMP_READ_ONLY;
        else if (strcmp (yyvsp[0].charPtr, "read-write") == 0)
            yyval.intVal = SNMP_READ_WRITE;
        else if (strcmp (yyvsp[0].charPtr, "write-only") == 0)
            yyval.intVal = SNMP_WRITE_ONLY;
        else if (strcmp (yyvsp[0].charPtr, "not-accessible") == 0)
            yyval.intVal = SNMP_NOT_ACCESSIBLE;
        else
        {
            yyerror ("ACCESS field of SNMP OBJECT-TYPE MACRO can only be one of \"read-write\", \"write-only\" or \"not-accessible\"");
            yyval.intVal = -1;
            modulePtrG->status = MOD_ERROR;
        }
        Free (yyvsp[0].charPtr);
   ;
    break;}
case 512:
#line 3462 "core/parse-asn1.y"
{
        if (strcmp (yyvsp[0].charPtr, "mandatory") == 0)
            yyval.intVal = SNMP_MANDATORY;
        else if (strcmp (yyvsp[0].charPtr, "optional") == 0)
            yyval.intVal = SNMP_OPTIONAL;
        else if (strcmp (yyvsp[0].charPtr, "obsolete") == 0)
            yyval.intVal = SNMP_OBSOLETE;
        else if (strcmp (yyvsp[0].charPtr, "deprecated") == 0)
            yyval.intVal = SNMP_DEPRECATED;
        else
        {
            yyerror ("STATUS field of SNMP OBJECT-TYPE MACRO can only be one of \"optional\", \"obsolete\" or \"deprecated\"");
            yyval.intVal = -1;
            modulePtrG->status = MOD_ERROR;
        }
        Free (yyvsp[0].charPtr);
   ;
    break;}
case 513:
#line 3482 "core/parse-asn1.y"
{ yyval.valuePtr = yyvsp[0].valuePtr; ;
    break;}
case 514:
#line 3483 "core/parse-asn1.y"
{ yyval.valuePtr = NULL; ;
    break;}
case 515:
#line 3487 "core/parse-asn1.y"
{ yyval.valuePtr = yyvsp[0].valuePtr; ;
    break;}
case 516:
#line 3488 "core/parse-asn1.y"
{ yyval.valuePtr = NULL; ;
    break;}
case 517:
#line 3493 "core/parse-asn1.y"
{
       yyval.typeOrValueListPtr  = yyvsp[-1].typeOrValueListPtr;
   ;
    break;}
case 518:
#line 3496 "core/parse-asn1.y"
{ yyval.typeOrValueListPtr = NULL; ;
    break;}
case 519:
#line 3501 "core/parse-asn1.y"
{
       yyval.valuePtr  = yyvsp[-1].valuePtr;
   ;
    break;}
case 520:
#line 3504 "core/parse-asn1.y"
{ yyval.valuePtr = NULL; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 527 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 3507 "core/parse-asn1.y"


void yyerror (char* s)
{
	fprintf (errFileG,
		"%s(%ld) : %s at symbol \"%s\"\n\n",
		modulePtrG->asn1SrcFileName, myLineNoG, s, yytext);
}


/*
 * given a Module*, the file name associated witht the open
 * FILE *fPtr, InitAsn1Parser sets up the yacc/lex parser
 * to parse an ASN.1 module read from fPtr and write the
 * parse results into the given Module *mod.
 */
int
InitAsn1Parser PARAMS ((mod, fileName, fPtr),
    Module	*mod _AND_
    char	*fileName _AND_
    FILE	*fPtr)
{
    yyin = fPtr;

    /*
     * reset lexical analyzer input file ptr
     * (only do this on succesive calls ow yyrestart seg faults
     */
#ifdef FLEX_IN_USE
    if (!firstTimeThroughG)
        yyrestart (fPtr);

    firstTimeThroughG = FALSE;
#endif


    /*
     * init modulePtr
     */
    memzero (mod, sizeof (Module));
    modulePtrG = mod;
    mod->asn1SrcFileName = fileName;
    mod->status = MOD_NOT_LINKED;
    mod->hasAnys = FALSE;

    /* init lists to empty */
    mod->typeDefs = AsnListNew (sizeof (void*));
    mod->valueDefs = AsnListNew (sizeof (void*));

    /*
     * init export list stuff
     */
    exportListG = NULL;
    exportsParsedG = FALSE;

    /*
     * reset line number to 1
     */
    myLineNoG = 1;

    /*
     * reset error count
     */
    parseErrCountG = 0;

   /*
    * set up list to hold values defined in parsed oids
// REN -- 9/23/02
    oidElmtValDefsG = AsnListNew (sizeof (void *));
    */

    smallErrG = 0;

    return 0;

}  /* InitAsn1Parser */


/*
 * puts the applicatin tag code, tagCode, and line number it was
 * parsed at into the applTagsG list.  If the APPLICATION tag code
 * is already in the applTagsG list then an error is printed.
 * and the smallErrG flag set to prevent code production.
 */
void
PushApplTag PARAMS ((tagCode, lineNo),
    unsigned long tagCode _AND_
    unsigned long lineNo)
{
    ApplTag *l;
    ApplTag *new;
    int wasDefined = 0;

    /* make sure not already in list */
    for (l = applTagsG; l != NULL; l = l->next)
    {
        if (l->tagCode == tagCode)
        {
            PrintErrLoc (modulePtrG->asn1SrcFileName, lineNo);
            fprintf (errFileG, "ERROR - APPLICATION tags can be used only once per ASN.1 module.  The tag \"[APPLICATION %ld]\" was previously used on line %ld.\n", tagCode, l->lineNo);
            wasDefined = 1;
            smallErrG = 1;
        }
    }
    if (!wasDefined)
    {
        new = MT (ApplTag);
        new->lineNo = lineNo;
        new->tagCode = tagCode;
        new->next = applTagsG;
        applTagsG = new;
    }
}  /* PushApplTag */


/*
 * Empties the applTagsG list.  Usually done between modules.
 */
void
FreeApplTags()
{
    ApplTag *l;
    ApplTag *lTmp;

    for (l = applTagsG; l != NULL; )
    {
        lTmp = l->next;
        Free (l);
        l = lTmp;
    }
    applTagsG = NULL;
}  /* FreeApplTags */
