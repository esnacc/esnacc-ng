/*
 * compiler/back-ends/py-gen/rules.h
 *
 * Copyright (C) 2016 Aaron Conole
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef PY_RulesInclude
#define PY_RulesInclude
/* see asn1module.h for CxxTDI (C++ type def info) */

typedef struct PyRules
{
    int  maxDigitsToAppend;
    char *choiceIdFieldName;   /* name of choiceId field */
    char *choiceIdEnumName;  /* name (tag) for choiceId enum def name */
    char *choiceUnionFieldName; /* what the name of the choice's union is */
    char *choiceUnionName;  /* name (tag) for choice union def name */
    int   capitalizeNamedElmts;
    char *encodeBaseName;
    char *decodeBaseName;
    char *encodeContentBaseName;
    char *decodeContentBaseName;
    char *encodePduBaseName;
    char *decodePduBaseName;
    CxxTDI typeConvTbl[BASICTYPE_EXTENSION + 1];
}  PyRules;

extern PyRules pyRulesG;

void FillPyTypeInfo PROTO ((PyRules *r, ModuleList *m));

#endif
