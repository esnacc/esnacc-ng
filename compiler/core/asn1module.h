/*
 *    compiler/core/asn1module.h
 *
 *    "Asn1Module" ASN.1 module C type definitions and prototypes
 *
 *    This .h file was by snacc on Sun Feb  7 23:38:26 1993
 *
 *    UBC snacc written compiler by Mike Sample
 *- editing not recommended
 */


#ifndef _asn1module_h_
#define _asn1module_h_


typedef enum
    {
        PRESENT_CT = 0,
        ABSENT_CT = 1,
        EMPTY_CT = 2,
        OPTIONAL_CT = 3
    } ConstraintEnum; /* ENUMERATED { PRESENT_CT (0), ABSENT_CT (1), EMPTY_CT (2), OPTIONAL_CT (3) }  */

typedef enum
    {
        FULL_CT = 0,
        PARTIAL_CT = 1,
        SINGLE_CT = 2
    } InnerSubtypeEnum; /* ENUMERATED { FULL_CT (0), PARTIAL_CT (1), SINGLE_CT (2) }  */

typedef enum
    {
        SNMP_MANDATORY = 0,
        SNMP_OPTIONAL = 1,
        SNMP_OBSOLETE = 2,
        SNMP_DEPRECATED = 3
    } SnmpObjectTypeMacroTypeEnum1; /* ENUMERATED { SNMP_MANDATORY (0), SNMP_OPTIONAL (1), SNMP_OBSOLETE (2), SNMP_DEPRECATED (3) }  */

typedef enum
    {
        SNMP_READ_ONLY = 0,
        SNMP_READ_WRITE = 1,
        SNMP_WRITE_ONLY = 2,
        SNMP_NOT_ACCESSIBLE = 3
    } SnmpObjectTypeMacroTypeEnum; /* ENUMERATED { SNMP_READ_ONLY (0), SNMP_READ_WRITE (1), SNMP_WRITE_ONLY (2), SNMP_NOT_ACCESSIBLE (3) }  */

typedef enum
    {
        CONSUMER_PORT = 0,
        SUPPLIER_PORT = 1,
        SYMMETRIC_PORT = 2
    } AsnPortEnum; /* ENUMERATED { CONSUMER_PORT (0), SUPPLIER_PORT (1), SYMMETRIC_PORT (2) }  */

typedef enum
    {
        EXPORTS_ALL = 0,
        EXPORTS_NOTHING = 1,
        EXPORTS_SOME = 2
    } ModuleEnum2; /* ENUMERATED { EXPORTS_ALL (0), EXPORTS_NOTHING (1), EXPORTS_SOME (2) }  */

typedef enum
    {
        EXPLICIT_TAGS = 0,
        IMPLICIT_TAGS = 1
    } ModuleEnum1; /* ENUMERATED { EXPLICIT_TAGS (0), IMPLICIT_TAGS (1) }  */

typedef enum
    {
        MOD_OK = 0,
        MOD_NOT_LINKED = 1,
        MOD_ERROR = 2
    } ModuleEnum; /* ENUMERATED { MOD_OK (0), MOD_NOT_LINKED (1), MOD_ERROR (2) }  */


typedef AsnInt AsnRefineMacroType; /* INTEGER */

typedef enum
    {
        MIN_INT = 0,
        MAX_INT = 1
    } SpecialIntegerValue; /* ENUMERATED { MIN_INT (0), MAX_INT (1) }  */

typedef enum
    {
        MINUS_INFINITY_REAL = 0,
        PLUS_INFINITY_REAL = 1
    } SpecialRealValue; /* ENUMERATED { MINUS_INFINITY_REAL (0), PLUS_INFINITY_REAL (1) }  */

typedef enum
    {
        C_CHOICE = 0,
        C_LIST = 1,
        C_ANY = 2,
        C_ANYDEFINEDBY = 3,
        C_LIB = 4,
        C_STRUCT = 5,
        C_TYPEREF = 6,
        C_NO_TYPE = 7,
        C_TYPEDEF = 8,
		C_OBJECTCLASSFIELDTYPE = 9,			// Deepak: 05/Feb/2003
		C_OBJECTCLASS = 10,					// Deepak: 11/Mar/2003
		C_MACROTYPE = 11					// Deepak: 17/Apr/2003
    } CTypeId; /* ENUMERATED { C_CHOICE (0), C_LIST (1), C_ANY (2), C_ANYDEFINEDBY (3), C_LIB (4), C_STRUCT (5), C_TYPEREF (6), C_NO_TYPE (7), C_TYPEDEF (8), C_OBJECTCLASSFIELDTYPE (9), C_OBJECTCLASS (10), C_MACROTYPE (11) }  */


typedef struct OidOrInt /* CHOICE */
{
    enum OidOrIntChoiceId
    {
        OIDORINT_OID,
        OIDORINT_INTID
    } choiceId;
    union OidOrIntChoiceUnion
    {
    AsnOid *oid; /* OBJECT IDENTIFIER */
    AsnInt intId; /* INTEGER */
    } a;
} OidOrInt;


typedef AsnList OidList; /* SEQUENCE OF OBJECT IDENTIFIER */
typedef char *MyString; /* PrintableString */


typedef struct ModuleId /* SEQUENCE */
{
    MyString name; /* MyString */
    OID *oid; /* OBJECT IDENTIFIER OPTIONAL */
} ModuleId;


typedef struct AnyRef /* SEQUENCE */
{
    MyString anyIdName; /* MyString */
    struct OidOrInt *id; /* OidOrInt */
} AnyRef;

typedef enum
{
	ASN1_TypeID,				/* ASN.1 type value */
	C_TypeID,					/* CTypeId value */
	C_TypeName,					/* String value */
	C_FieldName,				/* String value */
	IsPDU,						/* Boolean value */
	IsPtr,						/* Boolean value */
	IsPtrForTypeDef,			/* Boolean value */
	IsPtrForTypeRef,			/* Boolean value */
	IsPtrInChoice,				/* Boolean value */
	IsPtrForOpt,				/* Boolean value */
	OptionalTestRoutineName,	/* String value */
	DefaultFieldName,			/* String value */
	PrintRoutineName,			/* String value */
	EncodeRoutineName,			/* String value */
	DecodeRoutineName,			/* String value */
	FreeRoutineName,			/* String value */
	IsEncDec,					/* Boolean value */
	GenTypeDef,					/* Boolean value */
	GenPrintRoutine,			/* Boolean value */
	GenEncodeRoutine,			/* Boolean value */
	GenDecodeRoutine,			/* Boolean value */
	GenFreeRoutine,				/* Boolean value */
	ChoiceIdSymbol,				/* String value */
	ChoiceIdValue,				/* Integer value */
	ChoiceIdEnumName,			/* String value */
	ChoiceIdEnumFieldName,		/* String value */
	IsBigInt					/* Boolean value */
} SnaccDirectiveEnum;

typedef AsnList SnaccDirectiveList; /* SEQUENCE OF SnaccDirective */
typedef AsnList AnyRefList; /* SEQUENCE OF AnyRef */
typedef MyString MacroDef; /* MyString */
typedef AsnList ImportModuleList; /* SEQUENCE OF ImportModule */
typedef AsnList ImportElmtList; /* SEQUENCE OF ImportElmt */
typedef AsnList TypeDefList; /* SEQUENCE OF TypeDef */
typedef AsnList TagList; /* SEQUENCE OF Tag */
typedef AsnList NamedTypeList; /* SEQUENCE OF NamedType */
typedef AsnList ValueList; /* SEQUENCE OF Value */
typedef AsnList TypeOrValueList; /* SEQUENCE OF TypeOrValue */
typedef AsnList AsnPortList; /* SEQUENCE OF AsnPort */
typedef AsnList SubtypeList; /* SEQUENCE OF Subtype */
typedef AsnList ConstraintList; /* SEQUENCE OF Constraint */
typedef AsnList ValueDefList; /* SEQUENCE OF ValueDef */

typedef struct ImportElmtChoice /* CHOICE */
{
    enum ImportElmtChoiceChoiceId
    {
        IMPORTELMTCHOICE_TYPE,
        IMPORTELMTCHOICE_VALUE
    } choiceId;
    union ImportElmtChoiceChoiceUnion
    {
    struct TypeDef *type; /* [0] IMPLICIT TypeDef */
    struct ValueDef *value; /* [1] IMPLICIT ValueDef */
    } a;
} ImportElmtChoice;


typedef AsnList ObjectAssignmentList;	/* Deepak: 26/Feb/2003 List of ObjectAssignment constructs */
typedef AsnList ObjectSetAssignmentList;/* Deepak: 26/Feb/2003 List of ObjectAssignment constructs */


typedef struct Module /* SEQUENCE */
{
    ModuleEnum				status; /* ModuleEnum */
    struct ModuleId 		*modId; /* ModuleId */
    ModuleEnum1 			tagDefault; /* ModuleEnum1 */
	MyString				namespaceToUse; /* MyString */
    ModuleEnum2 			exportStatus; /* ModuleEnum2 */
    ImportModuleList 		*imports; /* ImportModuleList */
    TypeDefList 			*typeDefs; /* TypeDefList */
    ValueDefList 			*valueDefs; /* ValueDefList */
	ObjectAssignmentList 	*objAssignments;	/* Deepak: 04/Mar/2003 */
	ObjectSetAssignmentList	*objSetAssignments; /* Deepak: 04/Mar/2003 */
    AsnBool 				hasAnys; /* BOOLEAN */
	AsnBool					ImportedFlag; /* BOOLEAN */
	AsnBool					ImportUsed; /* BOOLEAN */
    MyString 				asn1SrcFileName; /* MyString */
    MyString 				cHdrFileName; /* MyString */
    MyString 				cSrcFileName; /* MyString */
    MyString 				cxxHdrFileName; /* MyString */
    MyString 				cxxSrcFileName; /* MyString */
    AsnBool                 isExtensible; /*BOOLEAN*/
    
    MyString				cxxname; /* only used when META is defined */

#if IDL
    MyString				idlFileName; /* MyString */
    MyString				idlname;
#endif
} Module;



typedef struct ImportModule /* SEQUENCE */
{
    struct ModuleId *modId; /* ModuleId */
    ImportElmtList *importElmts; /* ImportElmtList */
    struct Module *moduleRef; /* Module */
    AsnInt lineNo; /* INTEGER */
} ImportModule;




typedef struct ImportElmt /* SEQUENCE */
{
    struct ImportElmtChoice *resolvedRef; /* ImportElmtChoice OPTIONAL */
    MyString name; /* MyString */
    AsnBool privateScope; /* BOOLEAN */
    AsnInt lineNo; /* INTEGER */
} ImportElmt;




typedef struct TypeDef /* SEQUENCE */
{
    AsnBool exported; /* BOOLEAN */
    AsnBool recursive; /* BOOLEAN */
    AsnBool isPdu; /* BOOLEAN */
	AsnBool bHasTableConstraint; /* BOOLEAN */
    AsnInt localRefCount; /* INTEGER */
    AsnInt importRefCount; /* INTEGER */
    AsnInt tmpRefCount; /* INTEGER */
    AsnBool visited; /* BOOLEAN */
    MyString definedName; /* MyString */
    struct Type *type; /* Type */
    struct CTDI *cTypeDefInfo; /* CTDI */
    struct CxxTDI *cxxTypeDefInfo; /* CxxTDI */
    struct IDLTDI *idlTypeDefInfo; /* IDLTDI */
    SnaccDirectiveList *attrList; /* SnaccDirectiveList */
    TypeDefList *refList; /* TypeDefList */
    AnyRefList *anyRefs; /* AnyRefList */
} TypeDef;




typedef struct Tag /* SEQUENCE */
{
    AsnInt tclass; /* INTEGER */
    AsnInt form; /* INTEGER */
    AsnInt code; /* INTEGER */
    AsnBool explicit; /* BOOLEAN */
    struct Value *valueRef; /* Value */
} Tag;


typedef AsnList MyStringList;

typedef AsnList WithSyntaxList;		// Deepak: 22/Feb/2003	List of WithSyntax constructs
typedef struct ObjectClassDef		// Deepak: 22/Feb/2003
{
	NamedTypeList* classdef;
	WithSyntaxList* withsyntax;
}ObjectClassDef;


typedef struct ObjectSetAssignment			// Deepak: 26/Feb/2003 
{
	AsnInt				lineNo; /* INTEGER */
	MyString			objectSetName;
	MyString			objectClassName;
	ObjectClassDef		*objectClassDefLink;	// Deepak: 26/Mar/2003
	TypeOrValueList		*objectNameList;		/* List of MyString */
}ObjectSetAssignment;


typedef struct TableConstraint 
{
	ObjectSetAssignment		*objSetAssignment;	// Deepak: 26/Mar/2003
	//MyString				objSetName;
	MyStringList			*atNotations;
}TableConstraint;


typedef struct Type /* SEQUENCE */
{
    AsnBool optional; /* BOOLEAN */
    AsnBool implicit; /* BOOLEAN */
    TagList *tags; /* TagList */
    struct NamedValue *defaultVal; /* [0] IMPLICIT NamedValue OPTIONAL */
    struct Subtype *subtypes; /* [1] Subtype OPTIONAL */
    struct BasicType *basicType; /* [2] BasicType */
    AsnInt lineNo; /* INTEGER */
    struct CTRI *cTypeRefInfo; /* CTRI */
    struct CxxTRI *cxxTypeRefInfo; /* CxxTRI */
    struct IDLTRI *idlTypeRefInfo; /* IDLTRI */
    SnaccDirectiveList *attrList; /* SnaccDirectiveList */
	struct TableConstraint *tableConstraint;			// Deepak: 10/Mar/2003
	MyString typeName;	// Deepak: 26/Mar/2003	
    AsnBool extensionAddition; /* BOOLEAN */
} Type;


typedef ValueDefList NamedNumberList; /* ValueDefList */


typedef struct TypeOrValue /* CHOICE */
{
    enum TypeOrValueChoiceId
    {
        TYPEORVALUE_TYPE,
        TYPEORVALUE_VALUE
    } choiceId;
    union TypeOrValueChoiceUnion
    {
    struct Type *type; /* [0] IMPLICIT Type */
    struct Value *value; /* [1] IMPLICIT Value */
    } a;
} TypeOrValue;


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct WithSyntax		// Deepak: 22/Feb/2003
{
	MyString			definedName;
	MyString			typeName;
	AsnBool				bOptional;
	AsnInt				lineNo; /* INTEGER */
	WithSyntaxList		*parent;
	WithSyntaxList		*subToken;		// Deepak: 27/Mar/2003
}WithSyntax;

typedef AsnList ObjectAssignmentFieldList;	// Deepak: 25/Feb/2003 List of ObjectAssignmentField constructs 
typedef struct ObjectAssignmentField
{
	MyString		objectFieldName;
	MyString		objectFieldNameWS;
	AsnBool			bOptional;			// Deepak: 22/Mar/2003
	AsnBool			bPresent;			// Deepak: 22/Mar/2003
	AsnBool			bUnknownType;		// Deepak: 22/Mar/2003
	TypeOrValue*	typeOrValue;
	AsnInt			lineNo; /* INTEGER */
}ObjectAssignmentField;

typedef struct ObjectAssignment			// Deepak: 26/Feb/2003 
{
	MyString					objectName;
	MyString					objectClassName;
	AsnBool						bWithSyntaxPresent;
	AsnInt						lineNo; /* INTEGER */
	ObjectAssignmentFieldList	*objectAssignmentField;
}ObjectAssignment;


// Deepak: 23/Apr/2003
// Parameterized Types Handling
typedef struct ParameterList
{
	int i;
}ParameterList;

typedef struct Parameter
{
	int i;
}Parameter;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/* Both the CTDI and CxxTDI arrays in the CRules and CxxRules structures,
respectively, must also be initialized for any new types added here.
The CRules and CxxRules structures reside in the rules.c files in the C and
C++ compiler back-ends.
The new type must appear in the same index in the array as its enum value
here. */
typedef struct BasicType /* CHOICE */
{
    enum BasicTypeChoiceId
    {                           /* Index to CTDI & CxxTDI array */
		BASICTYPE_UNKNOWN,      /* 0 */
		BASICTYPE_BOOLEAN,      /* 1 */
		BASICTYPE_INTEGER,      /* 2 */
		BASICTYPE_BITSTRING,    /* 3 */
		BASICTYPE_OCTETSTRING,  /* 4 */
		BASICTYPE_NULL,         /* 5 */
		BASICTYPE_OID,          /* 6 */
		BASICTYPE_REAL,         /* 7 */
		BASICTYPE_ENUMERATED,   /* 8 */ 
		BASICTYPE_SEQUENCE,     /* 9 */
		BASICTYPE_SEQUENCEOF,   /* 10 */
		BASICTYPE_SET,          /* 11 */
		BASICTYPE_SETOF,        /* 12 */
		BASICTYPE_CHOICE,       /* 13 */
		BASICTYPE_SELECTION,    /* 14 */
		BASICTYPE_COMPONENTSOF, /* 15 */
		BASICTYPE_ANY,          /* 16 */
		BASICTYPE_ANYDEFINEDBY, /* 17 */
		BASICTYPE_LOCALTYPEREF, /* 18 */
		BASICTYPE_IMPORTTYPEREF,/* 19 */
		BASICTYPE_MACROTYPE,    /* 20 */
		BASICTYPE_MACRODEF,     /* 21 */
		BASICTYPE_NUMERIC_STR,  /* 22 */
		BASICTYPE_PRINTABLE_STR,/* 23 */
		BASICTYPE_UNIVERSAL_STR,/* 24 */
		BASICTYPE_IA5_STR,      /* 25 */
		BASICTYPE_BMP_STR,      /* 26 */
		BASICTYPE_UTF8_STR,     /* 27 */
		BASICTYPE_UTCTIME,		/* 28 */
		BASICTYPE_GENERALIZEDTIME,	/* 29 */
		BASICTYPE_GRAPHIC_STR,		/* 30 */
		BASICTYPE_VISIBLE_STR,		/* 31 aka ISO646String */ 
		BASICTYPE_GENERAL_STR,		/* 32 */
		BASICTYPE_OBJECTDESCRIPTOR,	/* 33 */
		BASICTYPE_VIDEOTEX_STR,		/* 34 */
		BASICTYPE_T61_STR,			/* 35 */
		BASICTYPE_EXTERNAL,			/* 36 */
		BASICTYPE_OCTETCONTAINING,	/* 37 */
		BASICTYPE_BITCONTAINING,	/* 38 */
		BASICTYPE_RELATIVE_OID,  /* 39 JKG: added 11/Jan/2004 */
        BASICTYPE_EXTENSION, /*40 JKG:  added 21/Jan/2004 */
        BASICTYPE_SEQUENCET,		/* 41 Deepak: read Note above */
		BASICTYPE_OBJECTCLASS,		/* 42 Deepak: added on 11/Dec/2002 */
		BASICTYPE_OBJECTCLASSFIELDTYPE, /* 43 Deepak: added on 04/Feb/2003 */
        
        
    } choiceId;
    union BasicTypeChoiceUnion
    {
    AsnNull unknown; /* [0] IMPLICIT NULL */
    AsnNull boolean; /* [1] IMPLICIT NULL */
    NamedNumberList *integer; /* [2] IMPLICIT NamedNumberList */
    NamedNumberList *bitString; /* [3] IMPLICIT NamedNumberList */
    AsnNull octetString; /* [4] IMPLICIT NULL */
    AsnNull null; /* [5] IMPLICIT NULL */
    AsnNull oid; /* [6] IMPLICIT NULL */
    AsnNull real; /* [7] IMPLICIT NULL */
    NamedNumberList *enumerated; /* [8] IMPLICIT NamedNumberList */
    NamedTypeList *sequence; /* [9] IMPLICIT NamedTypeList */
    struct Type *sequenceOf; /* [10] IMPLICIT Type */
    NamedTypeList *set; /* [11] IMPLICIT NamedTypeList */
    struct Type *setOf; /* [12] IMPLICIT Type */
    NamedTypeList *choice; /* [13] IMPLICIT NamedTypeList */
    struct SelectionType *selection; /* [14] IMPLICIT SelectionType */
    struct Type *componentsOf; /* [15] IMPLICIT Type */
    AsnNull any; /* [16] IMPLICIT NULL */
    struct AnyDefinedByType *anyDefinedBy; /* [17] IMPLICIT AnyDefinedByType */
    struct TypeRef *localTypeRef; /* [19] IMPLICIT TypeRef */
    struct TypeRef *importTypeRef; /* [20] IMPLICIT TypeRef */
    struct MacroType *macroType; /* [21] MacroType */
    MacroDef macroDef; /* [22] IMPLICIT MacroDef */
    struct Type *stringContaining; /* OCTET STRING CONTAINING Type */
    AsnNull relativeOID;  /*JKG  added 11/Jan/2004 */
    void   *voidPtr; /* catch all */
	NamedTypeList *sequencet;		// Deepak: 30/Nov/2002
//	NamedTypeList *objectclass;		// Deepak: 11/Dec/2002
	ObjectClassDef *objectclass;		// Deepak: 22/Feb/2003
    } a;
} BasicType;


typedef struct
{
	SnaccDirectiveEnum type;
	union {
		enum BasicTypeChoiceId asnTypeVal;
		CTypeId cTypeVal;
		AsnBool boolVal;
		unsigned int integerVal;
		char* stringVal;
	} value;
} SnaccDirective;


typedef struct MacroType /* CHOICE */
{
    enum MacroTypeChoiceId
    {
        MACROTYPE_ROSOPERATION,
        MACROTYPE_ROSERROR,
        MACROTYPE_ROSBIND,
        MACROTYPE_ROSUNBIND,
        MACROTYPE_ROSASE,
        MACROTYPE_ROSAC,
        MACROTYPE_MTSASEXTENSION,
        MACROTYPE_MTSASEXTENSIONS,
        MACROTYPE_MTSASEXTENSIONATTRIBUTE,
        MACROTYPE_MTSASTOKEN,
        MACROTYPE_MTSASTOKENDATA,
        MACROTYPE_MTSASSECURITYCATEGORY,
        MACROTYPE_ASNOBJECT,
        MACROTYPE_ASNPORT,
        MACROTYPE_ASNREFINE,
        MACROTYPE_ASNABSTRACTBIND,
        MACROTYPE_ASNABSTRACTUNBIND,
        MACROTYPE_ASNABSTRACTOPERATION,
        MACROTYPE_ASNABSTRACTERROR,
        MACROTYPE_AFALGORITHM,
        MACROTYPE_AFENCRYPTED,
        MACROTYPE_AFPROTECTED,
        MACROTYPE_AFSIGNATURE,
        MACROTYPE_AFSIGNED,
        MACROTYPE_SNMPOBJECTTYPE
    } choiceId;
    union MacroTypeChoiceUnion
    {
    struct RosOperationMacroType *rosOperation; /* [0] IMPLICIT RosOperationMacroType */
    struct RosErrorMacroType *rosError; /* [1] IMPLICIT RosErrorMacroType */
    struct RosBindMacroType *rosBind; /* [2] IMPLICIT RosBindMacroType */
    struct RosBindMacroType *rosUnbind; /* [3] IMPLICIT RosBindMacroType */
    struct RosAseMacroType *rosAse; /* [4] IMPLICIT RosAseMacroType */
    struct RosAcMacroType *rosAc; /* [5] IMPLICIT RosAcMacroType */
    struct MtsasExtensionMacroType *mtsasExtension; /* [6] IMPLICIT MtsasExtensionMacroType */
    struct MtsasExtensionsMacroType *mtsasExtensions; /* [7] IMPLICIT MtsasExtensionsMacroType */
    struct MtsasExtensionAttributeMacroType *mtsasExtensionAttribute; /* [8] IMPLICIT MtsasExtensionAttributeMacroType */
    struct MtsasTokenMacroType *mtsasToken; /* [9] IMPLICIT MtsasTokenMacroType */
    struct MtsasTokenDataMacroType *mtsasTokenData; /* [10] IMPLICIT MtsasTokenDataMacroType */
    struct MtsasSecurityCategoryMacroType *mtsasSecurityCategory; /* [11] IMPLICIT MtsasSecurityCategoryMacroType */
    struct AsnObjectMacroType *asnObject; /* [12] IMPLICIT AsnObjectMacroType */
    struct AsnPortMacroType *asnPort; /* [13] IMPLICIT AsnPortMacroType */
    AsnRefineMacroType asnRefine; /* [14] IMPLICIT AsnRefineMacroType */
    struct AsnAbstractBindMacroType *asnAbstractBind; /* [15] IMPLICIT AsnAbstractBindMacroType */
    struct AsnAbstractBindMacroType *asnAbstractUnbind; /* [16] IMPLICIT AsnAbstractBindMacroType */
    struct RosOperationMacroType *asnAbstractOperation; /* [17] IMPLICIT RosOperationMacroType */
    struct RosErrorMacroType *asnAbstractError; /* [18] IMPLICIT RosErrorMacroType */
    struct Type *afAlgorithm; /* [19] IMPLICIT Type */
    struct Type *afEncrypted; /* [20] IMPLICIT Type */
    struct Type *afProtected; /* [21] IMPLICIT Type */
    struct Type *afSignature; /* [22] IMPLICIT Type */
    struct Type *afSigned; /* [23] IMPLICIT Type */
    struct SnmpObjectTypeMacroType *snmpObjectType; /* [24] IMPLICIT SnmpObjectTypeMacroType */
    } a;
} MacroType;




typedef struct AnyDefinedByType /* SEQUENCE */
{
    MyString fieldName; /* MyString */
    struct NamedType *link; /* NamedType OPTIONAL */
} AnyDefinedByType;


typedef struct SelectionType /* SEQUENCE */
{
    MyString fieldName; /* MyString */
    struct Type *typeRef; /* Type */
    struct NamedType *link; /* NamedType OPTIONAL */
} SelectionType;




typedef struct NamedType /* SEQUENCE */
{
    MyString fieldName; /* MyString */
    struct Type *type; /* Type */
} NamedType;




typedef struct TypeRef /* SEQUENCE */
{
    MyString typeName; /* MyString */
    MyString moduleName; /* MyString */
    struct Module *module; /* Module */
    struct TypeDef *link; /* TypeDef */
	struct NamedType *namedTypeLink; /* NamedType */		// Deepak: added on 05/Feb/2003
} TypeRef;




typedef struct RosOperationMacroType /* SEQUENCE */
{
    struct NamedType *arguments; /* NamedType */
    struct NamedType *result; /* NamedType */
    TypeOrValueList *errors; /* [0] IMPLICIT TypeOrValueList OPTIONAL */
    TypeOrValueList *linkedOps; /* [1] IMPLICIT TypeOrValueList OPTIONAL */
} RosOperationMacroType;




typedef struct RosErrorMacroType /* SEQUENCE */
{
    struct NamedType *parameter; /* NamedType */
} RosErrorMacroType;




typedef struct RosBindMacroType /* SEQUENCE */
{
    struct NamedType *argument; /* NamedType */
    struct NamedType *result; /* NamedType */
    struct NamedType *error; /* NamedType */
} RosBindMacroType;




typedef struct RosAseMacroType /* SEQUENCE */
{
    ValueList *operations; /* ValueList */
    ValueList *consumerInvokes; /* ValueList */
    ValueList *supplierInvokes; /* ValueList */
} RosAseMacroType;




typedef struct RosAcMacroType /* SEQUENCE */
{
    ValueList *nonRoElements; /* ValueList */
    struct Type *bindMacroType; /* Type */
    struct Type *unbindMacroType; /* Type */
    struct Value *remoteOperations; /* Value */
    ValueList *operationsOf; /* ValueList */
    ValueList *initiatorConsumerOf; /* ValueList */
    ValueList *responderConsumerOf; /* ValueList */
    OidList *abstractSyntaxes; /* OidList */
} RosAcMacroType;




typedef struct MtsasExtensionMacroType /* SEQUENCE */
{
    struct NamedType *elmtType; /* [0] IMPLICIT NamedType OPTIONAL */
    struct Value *defaultValue; /* [1] IMPLICIT Value OPTIONAL */
    AsnBool *criticalForSubmission; /* [2] IMPLICIT BOOLEAN OPTIONAL */
    AsnBool *criticalForTransfer; /* [3] IMPLICIT BOOLEAN OPTIONAL */
    AsnBool *criticalForDelivery; /* [4] IMPLICIT BOOLEAN OPTIONAL */
} MtsasExtensionMacroType;




typedef struct MtsasExtensionsMacroType /* SEQUENCE */
{
    ValueList *extensions; /* ValueList */
} MtsasExtensionsMacroType;




typedef struct MtsasExtensionAttributeMacroType /* SEQUENCE */
{
    struct Type *type; /* Type OPTIONAL */
} MtsasExtensionAttributeMacroType;




typedef struct MtsasTokenMacroType /* SEQUENCE */
{
    struct Type *type; /* Type OPTIONAL */
} MtsasTokenMacroType;




typedef struct MtsasTokenDataMacroType /* SEQUENCE */
{
    struct Type *type; /* Type OPTIONAL */
} MtsasTokenDataMacroType;




typedef struct MtsasSecurityCategoryMacroType /* SEQUENCE */
{
    struct Type *type; /* Type OPTIONAL */
} MtsasSecurityCategoryMacroType;




typedef struct AsnObjectMacroType /* SEQUENCE */
{
    AsnPortList *ports; /* AsnPortList OPTIONAL */
} AsnObjectMacroType;




typedef struct AsnPort /* SEQUENCE */
{
    struct Value *portValue; /* Value */
    AsnPortEnum portType; /* AsnPortEnum */
} AsnPort;




typedef struct AsnPortMacroType /* SEQUENCE */
{
    TypeOrValueList *abstractOps; /* [0] IMPLICIT TypeOrValueList OPTIONAL */
    TypeOrValueList *consumerInvokes; /* [1] IMPLICIT TypeOrValueList OPTIONAL */
    TypeOrValueList *supplierInvokes; /* [2] IMPLICIT TypeOrValueList OPTIONAL */
} AsnPortMacroType;




typedef struct AsnAbstractBindMacroType /* SEQUENCE */
{
    AsnPortList *ports; /* [0] IMPLICIT AsnPortList OPTIONAL */
    struct Type *type; /* [1] IMPLICIT Type OPTIONAL */
} AsnAbstractBindMacroType;




typedef struct SnmpObjectTypeMacroType /* SEQUENCE */
{
    struct Type *syntax; /* Type */
    SnmpObjectTypeMacroTypeEnum access; /* SnmpObjectTypeMacroTypeEnum */
    SnmpObjectTypeMacroTypeEnum1 status; /* SnmpObjectTypeMacroTypeEnum1 */
    struct Value *description; /* [0] IMPLICIT Value OPTIONAL */
    struct Value *reference; /* [1] IMPLICIT Value OPTIONAL */
    TypeOrValueList *index; /* [2] IMPLICIT TypeOrValueList OPTIONAL */
    struct Value *defVal; /* [3] IMPLICIT Value OPTIONAL */
} SnmpObjectTypeMacroType;




typedef struct Subtype
{
    enum SubtypeChoiceId
    {
        SUBTYPE_SINGLE,
        SUBTYPE_AND,
        SUBTYPE_OR,
        SUBTYPE_NOT
    } choiceId;
    union SubtypeChoiceUnion
    {
    struct SubtypeValue *single; /* [0] SubtypeValue */
    SubtypeList *and; /* [1] IMPLICIT SubtypeList */
    SubtypeList *or; /* [2] IMPLICIT SubtypeList */
    struct Subtype *not; /* [3] Subtype */
    } a;
} Subtype;


typedef struct
{
    AsnBool valueInclusive; /* BOOLEAN */
    struct Value *endValue; /* Value */
} ValueRangeEndpoint;


typedef struct
{
    ValueRangeEndpoint *lowerEndValue;
    ValueRangeEndpoint *upperEndValue;
} ValueRangeSubtype;


typedef struct SubtypeValue
{
    enum SubtypeValueChoiceId
	{
		SUBTYPEVALUE_SINGLEVALUE,
		SUBTYPEVALUE_CONTAINED,
		SUBTYPEVALUE_VALUERANGE,
		SUBTYPEVALUE_PERMITTEDALPHABET,
		SUBTYPEVALUE_SIZECONSTRAINT,
		SUBTYPEVALUE_INNERSUBTYPE
	} choiceId;
	union SubtypeValueChoiceUnion
	{
		struct Value	*singleValue;
		struct Type		*contained;
		ValueRangeSubtype *valueRange;
		Subtype			*permittedAlphabet;
		Subtype			*sizeConstraint;
		struct InnerSubtype *innerSubtype;
	} a;
} SubtypeValue;




typedef struct InnerSubtype /* SEQUENCE */
{
    InnerSubtypeEnum constraintType; /* InnerSubtypeEnum */
    ConstraintList *constraints; /* ConstraintList */
} InnerSubtype;




typedef struct Constraint /* SEQUENCE */
{
    MyString fieldRef; /* MyString */
    ConstraintEnum presenceConstraint; /* ConstraintEnum */
    struct Subtype *valueConstraints; /* Subtype */
} Constraint;




typedef struct ValueDef /* SEQUENCE */
{
    AsnBool exported; /* BOOLEAN */
    MyString definedName; /* MyString */
    struct Value *value; /* Value */
} ValueDef;




typedef struct Value /* SEQUENCE */
{
    struct Type *type; /* Type OPTIONAL */
    AsnInt valueType; /* INTEGER */
    struct BasicValue *basicValue; /* BasicValue */
    AsnInt lineNo; /* INTEGER */
} Value;




typedef struct BasicValue /* CHOICE */
{
    enum BasicValueChoiceId
    {
        BASICVALUE_UNKNOWN,
        BASICVALUE_EMPTY,
        BASICVALUE_INTEGER,
        BASICVALUE_SPECIALINTEGER,
        BASICVALUE_LONGINTEGER,
        BASICVALUE_BOOLEAN,
        BASICVALUE_REAL,
        BASICVALUE_SPECIALREAL,
        BASICVALUE_ASCIITEXT,
        BASICVALUE_ASCIIHEX,
        BASICVALUE_ASCIIBITSTRING,
        BASICVALUE_OID,
        BASICVALUE_LINKEDOID,
        BASICVALUE_BERVALUE,
        BASICVALUE_PERVALUE,
        BASICVALUE_NAMEDVALUE,
        BASICVALUE_NULL,
        BASICVALUE_LOCALVALUEREF,
        BASICVALUE_IMPORTVALUEREF,
        BASICVALUE_VALUENOTATION,
		BASICVALUE_OBJECTASSIGNMENT,		// Deepak: 24/Mar/2003 ????? perhaps will not be needed
        BASICVALUE_RELATIVE_OID
    } choiceId;
    union BasicValueChoiceUnion
    {
    AsnNull unknown; /* [0] IMPLICIT NULL */
    AsnNull empty; /* [1] IMPLICIT NULL */
    AsnInt integer; /* [2] IMPLICIT INTEGER */
    SpecialIntegerValue specialInteger; /* [3] IMPLICIT SpecialIntegerValue */
    AsnInt longInteger; /* [4] IMPLICIT INTEGER */
    AsnBool boolean; /* [5] IMPLICIT BOOLEAN */
    AsnReal real; /* [6] IMPLICIT REAL */
    SpecialRealValue specialReal; /* [7] IMPLICIT SpecialRealValue */
    AsnOcts *asciiText; /* [8] IMPLICIT OCTET STRING */
    AsnOcts *asciiHex; /* [9] IMPLICIT OCTET STRING */
    AsnOcts *asciiBitString; /* [10] IMPLICIT OCTET STRING */
    AsnOid *oid; /* [11] IMPLICIT OBJECT IDENTIFIER */
    OID *linkedOid; /* [12] IMPLICIT OBJECT IDENTIFIER */
    AsnOcts *berValue; /* [13] IMPLICIT OCTET STRING */
    AsnOcts *perValue; /* [14] IMPLICIT OCTET STRING */
    struct NamedValue *namedValue; /* [15] IMPLICIT NamedValue */
    AsnNull null; /* [16] IMPLICIT NULL */
    struct ValueRef *localValueRef; /* [17] IMPLICIT ValueRef */
    struct ValueRef *importValueRef; /* [18] IMPLICIT ValueRef */
    AsnOcts *valueNotation; /* [19] IMPLICIT OCTET STRING */
	ObjectAssignment *objAssignment;	// Deepak: 24/Mar/2003
    AsnRelativeOid *relativeOid;
    } a;
} BasicValue;




typedef struct ValueRef /* SEQUENCE */
{
    MyString valueName; /* MyString */
    MyString moduleName; /* MyString */
    struct ValueDef *link; /* ValueDef */
    struct Module *module; /* Module */
} ValueRef;




typedef struct NamedValue /* SEQUENCE */
{
    MyString fieldName; /* MyString */
    struct Value *value; /* Value */
} NamedValue;




typedef AsnList ModuleList; /* SEQUENCE OF Module */




typedef struct Modules /* [APPLICATION 0] IMPLICIT SEQUENCE */
{
    AsnInt creationTime; /* INTEGER */
    ModuleList *modules; /* ModuleList */
} Modules;




typedef AsnList NamedValueList; /* SEQUENCE OF NamedValue */


typedef struct CTDI /* SEQUENCE */
{
    enum BasicTypeChoiceId asn1TypeId; /* INTEGER */
    CTypeId cTypeId; /* CTypeId */
    MyString cTypeName; /* MyString */
    AsnBool isPdu; /* BOOLEAN */
    AsnBool isEncDec; /* BOOLEAN */
    AsnBool isPtrForTypeDef; /* BOOLEAN */
    AsnBool isPtrForTypeRef; /* BOOLEAN */
    AsnBool isPtrInChoice; /* BOOLEAN */
    AsnBool isPtrForOpt; /* BOOLEAN */
    MyString optTestRoutineName; /* MyString */
    MyString defaultFieldName; /* MyString */
    MyString printRoutineName; /* MyString */
    MyString encodeRoutineName; /* MyString */
    MyString decodeRoutineName; /* MyString */
    MyString freeRoutineName; /* MyString */
    AsnBool genPrintRoutine; /* BOOLEAN */
    AsnBool genEncodeRoutine; /* BOOLEAN */
    AsnBool genDecodeRoutine; /* BOOLEAN */
    AsnBool genFreeRoutine; /* BOOLEAN */
    AsnBool genTypeDef; /* BOOLEAN */
} CTDI;




typedef struct CNamedElmt /* SEQUENCE */
{
    MyString name; /* MyString */
    AsnInt value; /* INTEGER */
} CNamedElmt;




typedef struct CxxTDI /* SEQUENCE */
{
    enum BasicTypeChoiceId asn1TypeId; /* INTEGER */
    MyString className; /* MyString */
    AsnBool isPdu; /* BOOLEAN */
    AsnBool isEnc; /* BOOLEAN */
    AsnBool isPtrForTypeDef; /* BOOLEAN */
    AsnBool isPtrForOpt; /* BOOLEAN */
    AsnBool isPtrInChoice; /* BOOLEAN */
    AsnBool isPtrInSetAndSeq; /* BOOLEAN */
    AsnBool isPtrInList; /* BOOLEAN */
    MyString optTestRoutineName; /* MyString */
    MyString defaultFieldName; /* MyString */
} CxxTDI;

typedef struct IDLTDI /* SEQUENCE */
{
    enum BasicTypeChoiceId asn1TypeId; /* INTEGER */
    MyString typeName; /* MyString */
    AsnBool isPdu; /* BOOLEAN */
    AsnBool isEnc; /* BOOLEAN */
    AsnBool isPtrForTypeDef; /* BOOLEAN */
        AsnBool isPtrForOpt; /* BOOLEAN */
    AsnBool isPtrInChoice; /* BOOLEAN */
    AsnBool isPtrInSetAndSeq; /* BOOLEAN */
    AsnBool isPtrInList; /* BOOLEAN */
    MyString optTestRoutineName; /* MyString */
    MyString defaultFieldName; /* MyString */
} IDLTDI;




typedef AsnList CNamedElmts; /* SEQUENCE OF CNamedElmt */




typedef struct CxxTRI /* SEQUENCE */
{
    AsnBool isEnc; /* BOOLEAN */
    MyString className; /* MyString */
    MyString fieldName; /* MyString */
    AsnBool isPtr; /* BOOLEAN */
    CNamedElmts *namedElmts; /* CNamedElmts */
    MyString choiceIdSymbol; /* MyString */
    AsnInt choiceIdValue; /* INTEGER */
    MyString optTestRoutineName; /* MyString */
} CxxTRI;

typedef struct IDLTRI /* SEQUENCE */
{
    AsnBool isEnc; /* BOOLEAN */
    MyString typeName; /* MyString */
    MyString fieldName; /* MyString */
    AsnBool isPtr; /* BOOLEAN */
    CNamedElmts *namedElmts; /* CNamedElmts */
    MyString choiceIdSymbol; /* MyString */
    AsnInt choiceIdValue; /* INTEGER */
    MyString optTestRoutineName; /* MyString */
} IDLTRI;




typedef struct CTRI /* SEQUENCE */
{
    CTypeId cTypeId; /* CTypeId */
    MyString cFieldName; /* MyString */
    MyString cTypeName; /* MyString */
    AsnBool isPtr; /* BOOLEAN */
    CNamedElmts *cNamedElmts; /* CNamedElmts OPTIONAL */
    AsnInt choiceIdValue; /* INTEGER */
    MyString choiceIdSymbol; /* MyString */
    MyString choiceIdEnumName; /* MyString */
    MyString choiceIdEnumFieldName; /* MyString */
    MyString optTestRoutineName; /* MyString */
    MyString printRoutineName; /* MyString */
    MyString encodeRoutineName; /* MyString */
    MyString decodeRoutineName; /* MyString */
    MyString freeRoutineName; /* MyString */
    AsnBool isEncDec; /* BOOLEAN */
} CTRI;



#endif /* conditional include of asn1module.h */

