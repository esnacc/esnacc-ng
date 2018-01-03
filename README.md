Enhanced Sample Neufeld ASN C Compiler
=======================================

Build Status
------------
[![Travis Build Status](https://travis-ci.org/esnacc/esnacc-ng.png)](https://travis-ci.org/esnacc/esnacc-ng) | [![Build status](https://ci.appveyor.com/api/projects/status/f63n5ka5sqv93530/branch/master?svg=true)](https://ci.appveyor.com/project/orgcandman/esnacc-ng/branch/master)

What is eSNACC
--------------
This package originally provided the C/yacc/lex source code for snacc,
an ASN.1 to C or C++ compiler.  Snacc is short for "Sample Neufeld Asn.1
to C Compiler" and ASN.1 stands for Abstract Syntax Notation One (CCITT
X.208/ ISO 8824).  See the COPYRIGHT file for copyright information.

eSNACC is an enhanced version of the Sample Neufeld ASN C Compiler. It 
supports BER, DER, and PER rulesets for C and C++ code generation. There is
experimental support for TCL, and Python.

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
The following represents a very quick start guide to building the esnacc
codebase. For more detailed instructions, see [INSTALL.md]

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
   3. Python routines for BER encoding, decoding, and printing.
   4. A type table that can be used with C driver routines
      for BER encoding, decoding, printing and freeing.
   5. if you are really lucky, a core dump! :)

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

When using the python option, esnacc produces a .py file that can be 
used for BER encoding and decoding (sometimes).

FUTURE ENCHANCEMENTS
--------------------

* Support for 1997 ASN.1 syntax
* Change ANY tables so that each ASN.1 module is responsible for removing the 
  ANYs they install in the table.

PER FUTURE ENHANCEMENTS
-----------------------

 * PER 'C' code generation
 * Recognition and handling of extensibility


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
