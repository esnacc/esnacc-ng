/*
 * asn_list.h
 *
 *  ---------
 *  | AsnList |
 *  |  last |-------------------------------------------|
 *  |  curr |--------------------------|                |
 *  |  first|--------|                 |                |
 *  ---------        |                 |                |
 *                   V                 V                V
 *                ---------        ---------        ---------
 *                |AsnListNode       |AsnListNode       |AsnListNode
 *                | next  |---...->|  next |--...-->| next  |-----|i.
 *         .i|----| prev  |<--...--|  prev |<--...--| prev  |
 *                | data  |        |  data |        | data  |
 *                ---------        ---------        ---------
 *
 * Originally by Murray Goldberg
 * Modified for ASN.1 use.
 * MS 92
 * Copyright (C) 1992 the University of British Columbia
 *
 * This library is free software; you can redistribute it and/or
 * modify it provided that this copyright/license information is retained
 * in original form.
 *
 * If you modify this file, you must clearly indicate your changes.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Header: /baseline/SNACC/c-lib/inc/asn-list.h,v 1.7 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-list.h,v $
 * Revision 1.7  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.6.2.1  2003/11/05 14:58:55  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.6  2003/02/20 21:07:59  leonberp
 * added #ifdef __cplusplus extern "C" to headers
 *
 * Revision 1.5  2002/10/21 17:13:24  mcphersc
 * fixed long int
 *
 * Revision 1.4  2002/10/18 13:10:32  mcphersc
 * took out long int to unsigned long
 *
 * Revision 1.3  2001/07/12 19:34:02  leonberp
 * Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
 *
 * Revision 1.2  2001/03/05 19:36:21  nicholar
 * no message
 *
 * Revision 1.1.1.1  2000/08/21 20:35:54  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/08/28 07:25:52  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:43  povey
 *
 * Revision 1.3  1995/07/24 21:01:14  rj
 * changed `_' to `-' in file names.
 *
 * Revision 1.2  1994/10/08  01:40:22  rj
 * it is unwise to #define unbalanced if()s! (fixed.)
 * three declarations added.
 *
 * Revision 1.1  1994/08/28  09:21:30  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */

#ifndef _asn_list_h_
#define _asn_list_h_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct AsnListNode
{
    struct AsnListNode	*prev;
    struct AsnListNode	*next;
    void		*data;		/* this must be the last field of this structure  */
} AsnListNode;

typedef struct AsnList
{
    AsnListNode		*first;
    AsnListNode		*last;
    AsnListNode		*curr;
    int			count;		/* number of elements in list               */
    int			dataSize;	/* space required in each node for the data */
} AsnList;

#define FOR_EACH_LIST_ELMT( elmt,  al)\
  if (!(al))\
    ;\
  else\
    for ((al)->curr = (al)->first; (al)->curr && (((elmt) = (al)->curr->data) != NULL); (al)->curr = (al)->curr->next)

#define FOR_EACH_LIST_ELMT_RVS( elmt,  al)\
  if (!(al))\
    ;\
  else\
    for ((al)->curr = (al)->last; (al)->curr && (((elmt) = (al)->curr->data) != NULL); (al)->curr = (al)->curr->prev)


#define FOR_REST_LIST_ELMT( elmt,  al)\
  if (!(al))\
    ;\
  else\
    for (; (al)->curr && (((elmt) = (al)->curr->data) != NULL); (al)->curr = (al)->curr->next)

#define FOR_REST_LIST_ELMT_RVS( elmt,  al)\
  if (!(al))\
    ;\
  else\
    for (; ((al)->curr && (((elmt) = (al)->curr->data) != NULL); (al)->curr = (al)->curr->prev)

/*
 * The following macros return the pointer stored in the
 * data part of the listNode.  The do not change the current
 * list pointer.
 */
#define CURR_LIST_ELMT( al)			((al)->curr->data)
#define NEXT_LIST_ELMT( al)			((al)->curr->next->data)
#define PREV_LIST_ELMT( al)			((al)->curr->prev->data)
#define LAST_LIST_ELMT( al)			((al)->last->data)
#define FIRST_LIST_ELMT( al)			((al)->first->data)
#define LIST_EMPTY( al)				((al)->count == 0)
#define LIST_COUNT( al)				((al)->count)

/*
 * list nodes are the parts of the list that contain ptrs/data
 * to/of the list elmts.
 */
#define CURR_LIST_NODE( al)			((al)->curr)
#define FIRST_LIST_NODE( al)			((al)->first)
#define LAST_LIST_NODE( al)			((al)->last)
#define PREV_LIST_NODE( al)			((al)->curr->prev)
#define NEXT_LIST_NODE( al)			((al)->curr->next)
#define SET_CURR_LIST_NODE( al, listNode)	((al)->curr = (listNode))

void	AsnListRemove PROTO ((AsnList *));
void	*AsnListAdd PROTO ((AsnList *));
void	*AsnListInsert PROTO ((AsnList *));
void	AsnListInit PROTO ((AsnList *list, int dataSize));
AsnList	*AsnListNew PROTO ((int));
void	*AsnListPrev PROTO ((AsnList *));
void	*AsnListNext PROTO ((AsnList *));
void	*AsnListLast PROTO ((AsnList *));
void	*AsnListFirst PROTO ((AsnList *));
void	*AsnListPrepend PROTO ((AsnList *));
void	*AsnListAppend PROTO ((AsnList *));
void	*AsnListCurr PROTO ((AsnList *));
AsnList *AsnListSort PROTO ((AsnList *list, int ((*cmp)(const void *, const void *))));
int	AsnListCount PROTO ((AsnList *));
AsnList	*AsnListConcat PROTO ((AsnList *, AsnList *));
long GetAsnListElmtIndex PROTO ((void *elmt,AsnList *list));
void	AsnListFree PROTO (( AsnList *));
void	*GetAsnListElmt PROTO ((AsnList *list, unsigned int index));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* conditional include */

