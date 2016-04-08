Enhanced Sample Neufield ASN C Compiler
=======================================

Build Status
------------
[![Travis Build Status](https://travis-ci.org/azsnacc/esnacc-ng.png)](https://travis-ci.org/azsnacc/esnacc-ng)

[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/azsnacc/esnacc-ng?branch=master&svg=true)](https://ci.appveyor.com/api/projects/status/github/azsnacc/esnacc-ng?branch=master)

What is eSNACC
--------------
This package originally provided the C/yacc/lex source code for snacc,
an ASN.1 to C or C++ compiler.  Snacc is short for "Sample Neufeld Asn.1
to C Compiler" and ASN.1 stands for Abstract Syntax Notation One (CCITT
X.208/ ISO 8824).  See the COPYRIGHT file for copyright information.

eSNACC is an enhanced version of the Sample Neufield ASN C Compiler. It 
supports BER, DER, and PER rulesets for C and C++ code generation. There is
experimental support for TCL.

eSNACC takes ASN.1 code files, parses them, and outputs appropriate header
and source files for inclusion in a larger project.

eSNACC is licensed under two different liceses. The compiler itself is a
GPLv2+ licensed software; the version maintained here will continue to be
GPLv2, unless a compelling reason to switch presents itself. The runtime is
a much less restrictive license - it is considered as Public Domain code. So
if you wish to contribute, be sure that you understand any runtime changes
you contribute MUST be public domain for inclusion into this distribution. Any
compiler changes MUST be GPL, at least v2.

Building eSNACC
---------------
First, run the './boot.sh' script, to generate the appropriate configure
scripts.

Next, run './configure' passing in the appropriate options.

Finally, run 'make' to build.

'make install' will put the binaries and library data in the appropriate
places.

Note: release and debug versions use the same library names.  If you want to be a
      debug version you must make sure you are working with a clean source tree.  
      Same goes for building a release version after building a debug version.


Running eSNACC
--------------

Given an ASN.1 source file(s) esnacc can produce:

   1. C routines for BER encoding, decoding, printing and freeing.
   2. C++ routines for BER encoding, decoding, and printing.
   3. A type table that can be used with C driver routines
      for BER encoding, decoding, printing and freeing.
   4. if you are really lucky, a core dump! :)

esnacc tries to produce reasonably efficient C or C++ routines and data
structures to support BER encoding and decoding values of the given
ASN.1 data structures.  Printing routines for the decoded values are
also generated.  For C only, hierarchical freeing routines are
generated (but are not recommended for efficiency reasons).

When using the C or C++ (not table) options, esnacc creates the
following C files for each ASN.1 source file (module):

        1. .h file for the C/C++ translation of the ASN.1 data
           structure and prototypes for the generated routines.

        2. .c/.cpp file for the C/C++ encode, decode, print and free
           routines.

When using the table option, snacc produces a type table file
(a BER encoding) that can later be loaded at runtime by the
table driven encoding and decoding routines.


FUTURE ENCHANCEMENTS
--------------------

* Support for 1997 ASN.1 syntax
* Change ANY tables so that each ASN.1 module is responsible for removing the 
  ANYs they install in the table.

PER FUTURE ENHANCEMENTS
-----------------------

 * PER 'C' code generation
 * Recognition and handling of extensibility	 

CHANGE LOG
----------
New in 1.8
    * After years of dust accumulating, eSNACC is re-maintained.

New in 1.7
    *  Enchancements to C++ runtime:
    *	Support of constraints checking for BER/PER
    *	Added Asn-Relative-Oid's
    *	Updated Asn-Oid to be inherited from Asn-Relative-Oid
    *	Added Extensibility to the set/sequence/choice syntax (BER encoding/decoding only, PER is not yet supported)
    *	asn::list has been changed to std::list (many changes)
    *	non-optional set/sequence/choice elements are no longer generated as pointers
    *	PER encode/decode capability for both aligned and unaligned variants (see PER beta notes below)
  *  Enchancements to C runtime:
    *	Added useful types
    *	Added Asn-Relative-Oid's
  *  Enhancements to compiler
    *	eSNACC no longer supports un-named types (2002 syntax update)  
    *	added -b compiler option to turn on/off PER encoding/decoding function generation in set/sequence/choice (Note -- calling PEnc/PDec will still work, but will not produce correct encoding unless -b is used!!)

New in EKMS PER Beta (Packed Encoding Rules)
    *	Aligned and unaligned PER variants (C++ only)
    *	Limited constraint checking and PER encoding for PER visible constraints
    * Supported
	*  char Stringtypes
        *  Integer
	*  Octet String
	*  Bit String
	*  Sequence-of / Set-of (limited)
    *Currently Unsupported 
	*  wide char stringtypes
	*  extensibility in constraints

New in 1.6Beta
    *	Updated "C" library to automatically handle ANY load/unloads as buffers.
    *	Added interpretation of ASN.1 integer constants as values in tag references for "C" and C++.
    *	Added "--snacc namespace: " pre-processor feature for unique C++ ASn.1 
    	  module namespace references.
    *	Updated SNACC document (in the ./SNACC/doc directory) to present DigitalNet
    	  updates/enhancements.
    *	Updated c++-examples and c-examples to demontrate recent features.

New in 1.5
    *	Updated "C" library to automatically handle ANY load/unloads as buffers.
    *	Added interpretation of ASN.1 integer constants as values in tag references for "C" and C++.
    *	Added "--snacc namespace: " pre-processor feature for unique C++ ASn.1 module namespace references.
    *	Updated SNACC document (in the ./SNACC/doc directory) to present DigitalNet updates/enhancements.
    *	Updated c++-examples and c-examples to demontrate recent features.


New in 1.4
 * rewrote makefiles to make build process easier and faster.
 * Enchancements to C++ runtime:
   * AsnInt changed to be so that it no longer inherits AsnOcts
   * AsnBits enchanced to construct BitStrings from binary strings.
   * Added AsnSetOf and AsnSeqOf templates.
   * Added Exception handling (see snaccexcept.h)
   * Moved BDecPdu to AsnType.  So every type as access to it now.  This
     was done to help reduce the number of symbols & methods the compiler
     generates.
   * Added useful types
 * Enchancements to C runtime:
   * Added useful types
 * Enhancements to compiler
   * Removed -u switch because useful types are now in the runtime library.
   * Added useful types as basic types.

New in 1.3b4:
 * Generic table decoding routines with callback mechanism in tbl library
 * New tool "berdecode", decoding any BER data, can also use table file
 * New Tcl/Tk tool "asnwish", reads table files, access to grammar, en-/decoding
 * Fix for tag values > 2^14
 * Fix for table-encoding tag values dividable by 128
 * Small fix in AsnBits::GetBit to return TRUE (i.e., 1) instead of #bit in byte

New in 1.3b3:
 * Added isPdu flag to tables
 * Added number range checks during parsing

New in 1.3b2:
 * Small fix of C value string generation wrt char values >127
 * Added (limited) size constraints, bitstring and enumeration names to tables

New in 1.3b1:
 * Made snacc.h more C++-readable (credits to Steve Walker).
 * Improved dependency generation for stupid makedepends.
 * Corrected PeekTag to peek into buffer only as far as necessary.
 * Added installable error handler.
 * Fixed small glitch in idl-code generator (credits to Markku Savela).
 * Finally fixed cut-and-paste error in SEQUENCE OF parsing.

New in 1.3a<n>:
 * Continued porting C, C++, tbl to 64 bit, any endianness: Hash typedef,
   PeekTag algorithm used by tables.
 * Fixed cut-and-paste error in SEQUENCE OF parsing.
 * Added missing initialisation of exponent in asn-real.[Cc].
 * Fixed tbl-free.c and some other table-related bugs.
 * Added casts and stuff for picky C++ compilers.
 * Incorporated all valid bug fixes from snacc-bugs.
 * Initial move of tcl stuff to tcl7.6/tk4.2/tree4.1.


snacc ASN.1 Compiler Release  1.1 - (updated Jul 93)
-----------------------------------------------------




See the latex or PostScript version of the documentation in the
snacc/doc directory for indepth information on this tool.  For a quick
introduction, look at the examples in snacc/c_examples,
snacc/c++_examples and snacc/tbl_example.


Features of ASN.1 Supported
---------------------------

   - parses ASN.1 '90 (subtype notation etc.)
   - macro definitions do not generate syntax errors but are
     are not processed. (retained as string)
   - value notation is parsed and for OBJECT IDENTIFIERs, INTEGERs
      and BOOLEANS (any other value in { }'s is currently kept as a string)
   - handles multiple ASN.1 module compiling/linking (IMPORTS/EXPORTS)
   - some X.400 and SNMP macros are parsed
   - supports ";" separted type or value definitions in the ASN.1 source.
     This is useful when dealing with some macros that introduce parsing
     problems.  Does not require the ";"'s though.
   - ANY DEFINED BY types are handled using the SNMP OBJECT-TYPE
     macro to define the identifier to type mapping.
