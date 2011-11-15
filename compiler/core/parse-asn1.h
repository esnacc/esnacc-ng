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


extern YYSTYPE yylval;
