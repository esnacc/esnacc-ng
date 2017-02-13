/**
 * do-macros.h
 */

#include <string.h>
#include <ctype.h>		/* for islower() */
#include "asn-incl.h"
#include "asn1module.h"
#include "str-util.h"
#include "snacc-util.h"
#include "lib-types.h"

#ifndef SNACC_DO_MACROS_H__
#define SNACC_DO_MACROS_H__

/* Function Prototypes */
void AddAnyRefByOid PROTO ((AnyRefList **arl, char *enumIdName, AsnOid *oid));
void AddAnyRefByInt PROTO ((AnyRefList **arl, char *enumIdName, AsnInt intId));
void NormalizeValue PROTO ((Module *m, ValueDef *vd, Value *v, int quiet));
void ProcessMacrosInTypeDef PROTO ((Module *m, TypeDef *td));
void ProcessMacrosInValueDef PROTO ((Module *m, ValueDef *vd));
void ProcessMacrosInType PROTO ((Module *m, TypeDef *td, Type *t,
								ValueDef *v));
void ProcessMacrosInElmtTypes PROTO ((Module *m, TypeDef *td, NamedTypeList *e,
									 ValueDef *v));
void ProcessMacrosInBasicType PROTO ((Module  *m, TypeDef *td, Type *type,
									 BasicType *bt, ValueDef *v));
void DefineType PROTO ((Module *m, TypeDef *td, Type *t, char *name));
void ProcessRosOperationMacroType PROTO ((Module *m, TypeDef *td, Type *t,
										 BasicType *bt, RosOperationMacroType *op, ValueDef *v));
void ProcessRosErrorMacroType PROTO ((Module *m, TypeDef *td, Type *t,
									 BasicType *bt, RosErrorMacroType *err,
									 ValueDef *v));
void ProcessRosBindMacroType PROTO ((Module *m, TypeDef *td, Type *t,
									BasicType *bt, RosBindMacroType *bind,
									ValueDef *v));
void ProcessRosAseMacroType PROTO ((Module *m, TypeDef *td, Type *t,
								   BasicType *bt, RosAseMacroType *ase,
								   ValueDef *v));
void ProcessRosAcMacroType PROTO ((Module *m, TypeDef *td, Type *t,
								  BasicType *bt, RosAcMacroType *ac,
								  ValueDef *v));
void ProcessMtsasExtensionsMacroType PROTO ((Module *m, TypeDef *td, Type *t,
											BasicType *bt,
											MtsasExtensionsMacroType *exts,
											ValueDef *v));
void ProcessMtsasExtensionMacroType PROTO ((Module *m, TypeDef *td, Type *t,
										   BasicType *bt,
										   MtsasExtensionMacroType *ext,
										   ValueDef *v));
void ProcessMtsasExtensionAttributeMacroType PROTO ((Module *m, TypeDef *td,
													Type *t, BasicType *bt,
													MtsasExtensionAttributeMacroType *ext,
													ValueDef *v));
void ProcessMtsasTokenMacroType PROTO ((Module *m, TypeDef *td, Type *t,
									   BasicType *bt, MtsasTokenMacroType *tok,
									   ValueDef *v));
void ProcessMtsasTokenDataMacroType PROTO ((Module *m, TypeDef *td, Type *t,
										   BasicType *bt,
										   MtsasTokenDataMacroType *tok,
										   ValueDef *v));
void ProcessMtsasSecurityCategoryMacroType PROTO ((Module *m, TypeDef *td,
												  Type *t, BasicType *bt,
												  MtsasSecurityCategoryMacroType *sec,
												  ValueDef *v));
void ProcessAsnObjectMacroType PROTO ((Module *m, TypeDef *td, Type *t,
									  BasicType *bt, AsnObjectMacroType *obj,
									  ValueDef *v));
void ProcessAsnPortMacroType PROTO ((Module *m, TypeDef *td, Type *t,
									BasicType *bt, AsnPortMacroType *p,
									ValueDef *v));
void ProcessAsnAbstractBindMacroType PROTO ((Module *m, TypeDef *td, Type *t,
											BasicType *bt,
											AsnAbstractBindMacroType *bind,
											ValueDef *v));
void ProcessSnmpObjectTypeMacroType PROTO ((Module *m, TypeDef *td, Type *t,
										   BasicType *bt,
										   SnmpObjectTypeMacroType *bind,
										   ValueDef *v));

int GetROSEData(ValueDef *vd, char **argumentTypeName, char **resultTypeName,
                char **errorTypeName, Type **argumentType, Type **resultType,
                Type **errorType);
#endif
