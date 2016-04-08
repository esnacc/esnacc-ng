// file: .../c++-lib/src/hash.C
//
// This was borrowed from Don Acton and Terry Coatta's Raven Code.
// It has been modified somewhat.
//    - Mike Sample 92
//
// This is a set or routines that implements an extensible hashing
// algorithm. At the moment it assumes that all the hash codes are unique
// (ie. there are no collisions). For the way hash codes are currently being
// supplied this is not a bad assumption.
// The extensible hashing routine used is based on a multiway tree with
// each node in the tree being a fixed array of (2^n) size. At a given
// level, i, in the tree with the first level being level 0, bits
// i*n through i*n through (i+1)*n-1 are used as the index into the table.
// Each entry in the table is either NULL (unused) or a pointer to an
// object of type entry. The entry contains all the information about a
// hash entry. The entry also contains a field indicating whether or not this
// is a leaf node. If an entry isn't a leaf node then it references a table at
// at the next level and not a value. With the current implementation
// a 32 hash value is used and table sizes are 256. The algorithm used
// here is the same as the one used in the Set class of the Raven
// class system.
//
// Copyright (C) 1992 the University of British Columbia
//
// This library is free software; you can redistribute it and/or
// modify it provided that this copyright/license information is retained
// in original form.
//
// If you modify this file, you must clearly indicate your changes.
//
// This source code is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// $Header: /baseline/SNACC/c++-lib/src/hash.cpp,v 1.9 2004/03/22 20:16:59 gronej Exp $
// $Log: hash.cpp,v $
// Revision 1.9  2004/03/22 20:16:59  gronej
// took out #include <string> since asn-incl already included them
//
// Revision 1.8  2004/03/22 18:30:17  leonberp
// Fixed Linux warnings.
//
// Revision 1.7  2002/05/29 19:26:06  leonberp
// removed PDU_MEMBERS_MACRO and added BDecPDU and BEncPDU to AsnType
//
// Revision 1.6  2001/10/15 17:14:26  leonberp
// mem leak fixes
//
// Revision 1.5  2001/07/19 16:02:52  sfl
// Yet more string.h updates.
//
// Revision 1.4  2001/07/12 19:33:43  leonberp
// Changed namespace to SNACC and added compiler options: -ns and -nons.  Also removed dead code.
//
// Revision 1.3  2001/06/28 15:29:46  rwc
// ADDED "SNACCASN" namespace definition to all SNACC data structures.
// This should not affect most applications since we do not have any name
// conflicts.
// ALSO, combined all ASN primitive data type includes into asn-incl.h.
//
// Revision 1.2  2001/06/18 17:47:45  rwc
// Updated to reflect newly added C++ Exception error handling, instead of "C" longjmp and setjmp calls.
// Changes made to both the compiler and the SNACC C++ run-time library.
//
// Revision 1.1.1.1  2000/08/21 20:36:09  leonberp
// First CVS Version of SNACC.
//
// Revision 1.7  1997/02/28 13:39:46  wan
// Modifications collected for new version 1.3: Bug fixes, tk4.2.
//
// Revision 1.6  1997/02/16 20:26:08  rj
// check-in of a few cosmetic changes
//
// Revision 1.5  1995/07/24  20:34:07  rj
// use memzero that is defined in .../snacc.h to use either memset or bzero.
//
// changed `_' to `-' in file names.
//
// Revision 1.4  1994/10/08  04:18:32  rj
// code for meta structures added (provides information about the generated code itself).
//
// code for Tcl interface added (makes use of the above mentioned meta code).
//
// virtual inline functions (the destructor, the Clone() function, BEnc(), BDec() and Print()) moved from inc/*.h to src/*.C because g++ turns every one of them into a static non-inline function in every file where the .h file gets included.
//
// made Print() const (and some other, mainly comparison functions).
//
// several `unsigned long int' turned into `size_t'.
//
// Revision 1.3  1994/08/31  23:43:05  rj
// FALSE/TRUE turned into false/true
//
// Revision 1.2  1994/08/28  10:01:21  rj
// comment leader fixed.
//
// Revision 1.1  1994/08/28  09:21:11  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.

/*#include <string>*/
#include "asn-incl.h"

_BEGIN_SNACC_NAMESPACE

/*
 *
 * From sdbm, an ndbm work-alike hashed database library
 * Author: oz@nexus.yorku.ca
 * Status: public domain.
 *
 * polynomial conversion ignoring overflows
 * [this seems to work remarkably well, in fact better
 * then the ndbm hash function. Replace at your own risk]
 * use: 65599   nice.
 *      65587   even better.
 *
 * [In one experiment, this function hashed 84165 symbols (English words
 * plus symbol table values) with no collisions. -bjb]
 *
 */

Hash
MakeHash (const char *str, size_t len)
{
    register Hash n = 0;

#define HASHC   n = *str++ + 65587 * n

    if (len > 0)
    {
        int loop;
        loop = (len + 8 - 1) >> 3;
        switch (len & (8 - 1))
	{
          case 0:            /* very strange! - switch labels in do loop */
            do
	    {
                HASHC;
              case 7: HASHC;
              case 6: HASHC;
              case 5: HASHC;
              case 4: HASHC;
              case 3: HASHC;
              case 2: HASHC;
              case 1: HASHC;
	    } while (--loop);
	}
    }
    return n;
}


/* Creates and clears a new hash slot */
static HashSlot *
NewHashSlot()
{
  HashSlot *foo;

  foo =  new HashSlot;
  if (foo == NULL)
      return NULL;
  memset (foo, 0, sizeof (HashSlot));
  return foo;
}

/* Create a new cleared hash table */
static Table *
NewTable()
{
  Table *new_table;

//  new_table = new Table;
// whose bug is it that gcc won't compile the above line?
  new_table = (Table *) new Table;
  if (new_table == NULL)
      return NULL;
  memset (new_table, 0, sizeof (Table));
  return new_table;
}

/* This routine is used to initialize the hash tables. When it is called
 * it returns a value which is used to identify which hash table
 * a particular request is to operate on.
 */
Table *
InitHash()
{
  Table *table;
  table = NewTable();
  if (table == NULL)
      return 0;
  else
      return table;
}

/* When a hash collision occurs at a leaf slot this routine is called to
 * split the entry and add a new level to the tree at this point.
 */
static int
SplitAndInsert (HashSlot *entry, void *element, Hash hash_value)
{

  if (((entry->table = NewTable()) == NULL) ||
      !Insert (entry->table, entry->value, entry->hash >> INDEXSHIFT) ||
      !Insert (entry->table, element, hash_value >> INDEXSHIFT))
    return false;

  entry->leaf = false;
  entry->value = NULL; // this is now contained in entry->table->value, so
  // avoid a double release
  return true;
}

/* This routine takes a hash table identifier, an element (value) and the
 * coresponding hash value for that element and enters it into the table
 * assuming it isn't already there.
 */
int
Insert (Table *table, void *element, Hash hash_value)
{
  HashSlot *entry;

  entry = (HashSlot *) (*table)[hash_value & INDEXMASK];

  if (entry == NULL) {
    /* Need to add this element here */
    entry = NewHashSlot();
    if (entry == NULL)
        return false;
    entry->leaf = true;
    entry->value = element;
    entry->hash = hash_value;
    (*table)[hash_value & INDEXMASK] = entry;
    return true;
  }

  if (hash_value == entry->hash)
      return false;

  if (entry->leaf)
      return SplitAndInsert (entry, element, hash_value);

  return Insert (entry->table, element, hash_value >> INDEXSHIFT);
}


/* This routine looks to see if a particular hash value is already stored in
 * the table. It returns true if it is and false otherwise.
 */
int
CheckFor (Table *table, Hash hash)
{
  HashSlot *entry;

  entry = (HashSlot *) table[hash & INDEXMASK];

  if (entry == NULL)
      return false;
  if (entry->leaf)
      return entry->hash == hash;
  return CheckFor (entry->table, hash >> INDEXSHIFT);
}

/* In addition to checking for a hash value in the tree this function also
 * returns the coresponding element value into the space pointed to by
 * the value parameter. If the hash value isn't found false is returned
 * the the space pointed to by value is not changed.
 */
int
CheckForAndReturnValue (Table *table, Hash hash, void **value)
{
  HashSlot *entry;
  if (table)
  {
      entry = (HashSlot *) (*table)[hash & INDEXMASK];

      if (entry == NULL)
          return false;

      if (entry->leaf)
      {
          if (entry->hash == hash)
          {
              *value = entry->value;
              return true;
          }
          else
              return false;
      }
      return CheckForAndReturnValue (entry->table, hash >> INDEXSHIFT, value);
  }
  else
     return false;
}

_END_SNACC_NAMESPACE
