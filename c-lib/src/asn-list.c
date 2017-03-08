/*
 * asn_list.c  - borrowed from Murray Goldberg
 *
 * the following routines implement the list data structure
 *
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
 * $Header: /baseline/SNACC/c-lib/src/asn-list.c,v 1.7 2003/12/17 19:05:03 gronej Exp $
 * $Log: asn-list.c,v $
 * Revision 1.7  2003/12/17 19:05:03  gronej
 * SNACC baseline merged with PER v1_7 tag
 *
 * Revision 1.6.2.1  2003/11/05 14:58:53  gronej
 * working PER code merged with esnacc_1_6
 *
 * Revision 1.6  2002/10/21 17:13:16  mcphersc
 * fixed long int
 *
 * Revision 1.5  2002/09/25 12:10:44  mcphersc
 * fixed warnings
 *
 * Revision 1.4  2002/09/03 19:33:19  vracarl
 * got rid of c++ comments
 *
 * Revision 1.3  2002/07/25 11:01:45  sfl
 * Added ifdef WIN32 around pragma to get rid of warnings
 *
 * Revision 1.1.1.1  2000/08/21 20:35:52  leonberp
 * First CVS Version of SNACC.
 *
 * Revision 1.4  1997/08/28 07:25:58  povey
 * Changes to support DER encoding/decoding
 *
 * Revision 1.3.1.1  1997/08/20 23:14:44  povey
 *
 * Revision 1.2  1995/07/27 08:59:36  rj
 * merged GetAsnListElmt(), a function used only by the type table code.
 *
 * changed `_' to `-' in file names.
 *
 * Revision 1.1  1994/08/28  09:45:55  rj
 * first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
 *
 */
#include "config.h"
#include "asn-config.h"
#include "asn-list.h"
#ifdef WIN32
#pragma  warning( disable : 4706 )  /* IGNORE assign w/in conditional expression. */
#endif

/* remove the entire list and all its nodes (not the actual list data elmts) */
/* this is set up for the snace compiler */
void
AsnListFree PARAMS ((list),
    AsnList *list)
{
    AsnListNode *node, *next;

    node = list->first;
    while (node)
    {
	next = node->next;
	Asn1Free (node);
	node = next;
    }

    Asn1Free (list);
}  /* AsnListFree */


/*
 * this routine removes the current node from the list. After removal the
 * current pointer will point to the next node in line, or NULL if the
 * removed item was at the tail of the list.
 */
void
AsnListRemove PARAMS ((list),
    AsnList *list)
{
    AsnListNode *node;

    if (list->curr)
    {
	if (list->curr->next)
	    list->curr->next->prev = list->curr->prev;
	else
	    list->last = list->curr->prev;

	if (list->curr->prev)
	    list->curr->prev->next = list->curr->next;
	else
	    list->first = list->curr->next;

	node       = list->curr;

	list->curr = list->curr->next;
	list->count--;

	Asn1Free (node);
    }
}

/*
 * this creates a new node after the current node and returns the
 * address of the memory allocated for data. The current pointer is changed
 * to point to the newly added node in the list. If the current pointer is
 * initially off the list then this operation fails.
 */
void*
AsnListAdd PARAMS ((list),
    AsnList *list)
{
    AsnListNode *newNode;
    void      *dataAddr;

    if (list->curr)
    {
	newNode  = (AsnListNode *) Asn1Alloc (sizeof (AsnListNode) + list->dataSize);
	dataAddr = (void *) &(newNode->data);

	newNode->next = list->curr->next;
	newNode->prev = list->curr;
	if (list->curr->next)
	    list->curr->next->prev = newNode;
	else
	    list->last = newNode;
	list->curr->next = newNode;

	list->curr = newNode;
	list->count++;
    }

    else
	dataAddr = NULL;

    return dataAddr;
}

/*
 * this creates a new node before the current node and returns the
 * address of the memory allocated for data. The current pointer is changed
 * to point to the newly added node in the list. If the current pointer is
 * initially off the list then this operation fails.
 */
void*
AsnListInsert PARAMS ((list),
    AsnList *list)
{
    AsnListNode *newNode;
    void      *dataAddr;

    if (list->curr)
    {
	newNode  = (AsnListNode *) Asn1Alloc (sizeof (AsnListNode) + list->dataSize);
	dataAddr = (void *) &(newNode->data);

	newNode->next = list->curr;
	newNode->prev = list->curr->prev;
	if (list->curr->prev)
	    list->curr->prev->next = newNode;
	else
	    list->first  = newNode;
	list->curr->prev = newNode;

	list->curr = newNode;
	list->count++;
    }

    else
	dataAddr = NULL;

    return dataAddr;
}


void
AsnListInit PARAMS ((list, dataSize),
    AsnList *list _AND_
    int dataSize)
{
    list->first = list->last = list->curr = NULL;
    list->count = 0;
    list->dataSize = dataSize;

}  /* AsnListInit */


AsnList*
AsnListNew PARAMS ((dataSize),
    int dataSize)
{
    AsnList *list;

    list = (AsnList *) Asn1Alloc (sizeof (AsnList));
    list->first = list->last = list->curr = NULL;
    list->count = 0;
    list->dataSize = dataSize;

    return list;
}

/*
 * backs up the current pointer by one and returns the data address of the new
 * current node. If the current pointer is off the list, the new current node
 * will be the last node of the list (unless the list is empty).
 */
void*
AsnListPrev PARAMS ((list),
    AsnList *list)
{
    void *retVal;

    if (list->curr == NULL)
	list->curr = list->last;
    else
	list->curr = list->curr->prev;

    if (list->curr == NULL)
	retVal = NULL;
    else
	retVal = (void *) &(list->curr->data);

    return retVal;
}

/*
 * advances the current pointer by one and returns the data address of the new
 * current node. If the current pointer is off the list, the new current node
 * will be the first node of the list (unless the list is empty).
 */
void*
AsnListNext PARAMS ((list),
    AsnList *list)
{
    void *retVal;

    if (list->curr == NULL)
	list->curr = list->first;
    else
	list->curr = list->curr->next;

    if (list->curr == NULL)
	retVal = NULL;
    else
	retVal = (void *) &(list->curr->data);

    return retVal;
}

/*
 * returns the data address of the last node (if there is one) and sets the
 * current pointer to this node.
 */
void*
AsnListLast PARAMS ((list),
    AsnList *list)
{
    void *retVal;

    list->curr = list->last;

    if (list->curr == NULL)
	retVal = NULL;
    else
	retVal = (void *) &(list->curr->data);

    return retVal;
}

/*
 * returns the data address of the first node (if there is one) and sets the
 * current pointer to this node.
 */
void*
AsnListFirst PARAMS ((list),
    AsnList *list)
{
    void *retVal;

    list->curr = list->first;

    if (list->curr == NULL)
	retVal = NULL;
    else
	retVal = (void *) &(list->curr->data);

    return retVal;
}

/*
 * this creates a new node at the beginning of the list and returns the
 * address of the memory allocated for data. The current pointer is changed
 * to point to the newly added node in the list.
 */
void*
AsnListPrepend PARAMS ((list),
    AsnList *list)
{
    AsnListNode *newNode;
    void      *dataAddr;

    newNode  = (AsnListNode *) Asn1Alloc (sizeof (AsnListNode) + list->dataSize);
    dataAddr = (void *) &(newNode->data);

    newNode->prev = NULL;

    if (list->first == NULL)
    {
	newNode->next = NULL;
	list->first   = list->last = newNode;
    }
    else
    {
	newNode->next     = list->first;
	list->first->prev = newNode;
	list->first       = newNode;
    }

    list->curr = newNode;
    list->count++;

    return dataAddr;
}

/*
 * this creates a new node at the end of the list and returns the
 * address of the memory allocated for data. The current pointer is changed
 * to point to the newly added node in the list.
 */
void*
AsnListAppend PARAMS ((list),
    AsnList *list)
{
    AsnListNode *newNode;
    void      *dataAddr;

    newNode  = (AsnListNode *) Asn1Alloc (sizeof (AsnListNode) + list->dataSize);
    dataAddr = (void *) &(newNode->data);

    newNode->next = NULL;

    if (list->last == NULL)
    {
        newNode->prev = NULL;
        list->first   = list->last = newNode;
    }
    else
    {
        newNode->prev     = list->last;
        list->last->next  = newNode;
        list->last        = newNode;
    }

    list->curr = newNode;
    list->count++;

    return dataAddr;
}

void*
AsnListCurr PARAMS ((list),
    AsnList *list)
{
    void *retVal;

    if (list->curr)
	retVal = (void *) &(list->curr->data);
    else
	retVal = NULL;

    return retVal;
}

/* Sort the list in ascending order of content
   XXX: In-place sort in the future */
AsnList* 
AsnListSort PARAMS ((list, cmp),
    AsnList *list _AND_
    int ((*cmp)(const void *, const void *)))
{
  void **sortArray;
  void *elmt;
  int i;
  AsnList* ret;
  void** tmpVar;

  /* Make sure list is not null */
  if (!(list)) {
    return NULL;
  }

  /* Create array of elements so we can qsort the list */
  sortArray = (void **) Asn1Alloc(list->count * sizeof(void *));

  /* Copy the elements from the list into the sort array */
  for ((list)->curr = (list)->first, i=0; 
       (list)->curr && ((elmt) = (void *)(list)->curr->data) && 
	 i < list->count; 
       (list)->curr = (list)->curr->next, i++) {
    sortArray[i] = elmt;
  }

  /* Sort the array */
  qsort(sortArray, list->count, sizeof(void *), cmp);

  /* Make a new list from the sorted Array */
  ret = AsnListNew(list->dataSize);
  for (i=0; i<list->count; i++) {
    tmpVar = AsnListAppend(ret);
    (*tmpVar) = sortArray[i];
  
  }

  Asn1Free(sortArray);
  
  /* Return the sorted list */
  return ret;

}

int
AsnListCount PARAMS ((list),
    AsnList *list)
{
    return list->count;
}


AsnList*
AsnListConcat PARAMS ((l1,l2),
    AsnList *l1 _AND_
    AsnList *l2)
{
    if (l2->count == 0)
        return l1;

    if (l1->count == 0)
    {
        l1->count = l2->count;
        l1->last = l2->last;
        l1->first = l2->first;
        l1->curr = l1->first;
    }
    else
    {
        l1->count += l2->count;
        l1->last->next = l2->first;
        l2->first->prev = l1->last;
        l1->last = l2->last;
    }

    return l1;
}


/*
 * Returns the index (starting a 0 for the first elmt)
 * of the given elmt in the given list
 * returns -1 if the elmt is not in the list
 * Assumes that the list node contains a single pointer
 */
long 
GetAsnListElmtIndex PARAMS ((elmt, list),
    void *elmt _AND_
    AsnList *list)
{
    void *tmp;
    void *tmpElmt;
    long  index;

    index = 0;
    tmp = (void*) CURR_LIST_NODE (list);
    FOR_EACH_LIST_ELMT (tmpElmt, list)
    {
        if (tmpElmt == elmt)
        {
            SET_CURR_LIST_NODE (list, tmp);
            return index;
        }
        else
            index++;
    }

    SET_CURR_LIST_NODE (list, tmp);
    return -1;

}  /* GetAsnListElmtIndex */


#if TTBL
/*
 * Returns the element with the given index.
 * indexes start a 0 for the first elmt.
 * returns NULL if the index is too large.
 * Assumes that the list node contains a single pointer.
 */
void*
GetAsnListElmt PARAMS ((list, index),
    AsnList *list _AND_
    unsigned int index)
{
    void *tmp;
    void *tmpElmt;
    unsigned long currIndex;

    if (index > (unsigned long)LIST_COUNT (list))
        return NULL;

    currIndex = 0;
    tmp = (void*) CURR_LIST_NODE (list);
    FOR_EACH_LIST_ELMT (tmpElmt, list)
    {
        if (currIndex == index)
        {
            SET_CURR_LIST_NODE (list, tmp);
            return tmpElmt;
        }
        currIndex++;
    }
    SET_CURR_LIST_NODE (list, tmp);
    return NULL;

}  /* GetAsnListElmt */
#endif /* TTBL */
