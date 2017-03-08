/*
 * compiler/core/enc-rules.c
 *
 * utilities for dealing with different encoding rules 
 *
 * AUTHOR: Dean Povey 
 * DATE:   97/08/27 
 *
 * Copyright (C) 1997 Dean Povey and the Distributed Systems Technology Centre 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/enc-rules.c,v 1.2 2003/07/07 14:50:13 nicholar Exp $
 * $Log: enc-rules.c,v $
 * Revision 1.2  2003/07/07 14:50:13  nicholar
 * Eliminated headers and cleaned up include references
 *
 * Revision 1.1.1.1  2000/08/21 20:35:59  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/08/28 07:37:15  povey
 * Initial revision
 *
 *
 */
#include "config.h"
#if !defined(lint)
static const char rcsid[] = "@(#)$RCSfile: enc-rules.c,v $ $Revision: 1.2 $";
#endif


/*
 * Local file includes
 */
#include "enc-rules.h"
#include "asn1module.h"
#include "lib-types.h"


/* Static variables. */
static EncRulesType rulesG;
static char* prefixG;
static EncRulesType rulesListG[3] = {NOP, NOP, NOP};

/* Set the encoding rule to be used */
int SetEncRules PARAMS ((encoding), EncRulesType encoding)
{
  switch(encoding) {
  case BER:
    /* Basic Encoding Rules */
    rulesG = BER;
    prefixG = "B";

    /* Set the correct encodings */
    SET_BER_LIBTYPE();

    return 1; 
  case DER:
    /* Distinguished Encoding Rules */
    rulesG = DER;
    prefixG = "D";
    
    /* Set the encodings for each lib type */
    SET_DER_LIBTYPE();
    return 1;
  default:
    /* No such rule */
    return 0;
  }
}

/* Add an encoding to the list of encoders to generate */
void AddEncRules PARAMS((encoding), EncRulesType encoding)
{
  if (rulesListG[0] == NOP) {
    rulesListG[0] = encoding;
  } else if (rulesListG[0] != encoding && rulesListG[1] == NOP) {
    rulesListG[1] = encoding;
  } else {
    /* Encoding must be already set */
    return;
  }

  return;
    
}

/* Return a list of the encoders to generate */
EncRulesType *GetEncRules()
{
  return rulesListG;
}

char* GetEncRulePrefix()
{
  return prefixG;
}

EncRulesType GetEncRulesType()
{
  return rulesG;
}





/***************************************************************/
/* end of FILE_C */
