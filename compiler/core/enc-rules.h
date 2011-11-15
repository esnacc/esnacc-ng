/*
 * compiler/core/enc-rules.h
 *
 * Copyright (C) 1997 Dean Povey and the Distributed Systems Technology Centre
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Header: /baseline/SNACC/compiler/core/enc-rules.h,v 1.2 2001/07/12 19:34:28 leonberp Exp $
 * $Log: enc-rules.h,v $
 * Revision 1.2  2001/07/12 19:34:28  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.1.1.1  2000/08/21 20:35:59  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.1  1997/08/28 07:37:15  povey
 * Initial revision
 *
 * Revision 1.3.1.1  1997/08/20 23:14:39  povey
 *
 */
#ifndef _enc_rules_h
#define _enc_rules_h

#if defined (__cplusplus)
extern "C" {
#endif

#include "asn-incl.h"

#if !defined(lint)
  static const char rcs_ENC_RULES_H[] = "@(#)$RCSfile: enc-rules.h,v $ $Revision: 1.2 $";
#endif

  /* Type of encoding rule being used */
  typedef enum {BER, DER, NOP} EncRulesType; 

  int SetEncRules PROTO((EncRulesType encoding));
  EncRulesType *GetEncRules();
  void AddEncRules PROTO((EncRulesType encoding));
  char* GetEncRulePrefix PROTO (());
  EncRulesType GetEncEncRulesType PROTO (());

#if defined (__cplusplus)
}
#endif

/***************************************************************/
#endif /* _enc_rules_h */

