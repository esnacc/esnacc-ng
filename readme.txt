				eSNACC EKMS PER beta release 

Welcome to eSNACC (Enhanced SNACC) maintained by DigitalNet.  This distribution contains the latest release of eSNACC.  

Building eSNACC
---------------
The build process has changed considerably from the previous release.  Now all you have to do to build on a clean distribution is:

 * make

    or

 * make debug

From the top of the source tree. 

If you want to clean the source tree of all objects and libraries:

 * make clean

If you are going to reuse the same source tree on another platform make sure you
remove all platform specific files by doing:

 * make distclean


Note: release and debug versions use the same library names.  If you want to be a
      debug version you must make sure you are working with a clean source tree.  
      Same goes for building a release version after building a debug version.


FUTURE ENCHANCEMENTS
--------------------

	
 * Support for 1997 ASN.1 syntax
 * Redesign AsnBuf with new expanding buffer and steal functionality from
   CSM_Buffer (which will go away).
 * Change ANY tables so that each ASN.1 module is responsible for removing the 
   ANYs they install in the table.

PER FUTURE ENHANCEMENTS
-----------------------
 
 * PER 'C' code generation
 * Recognition and handling of extensibility	 
 

CHANGE LOG
----------
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



################################################################
WARNING: the rest of this README is out of date!


For the newer releases, please look at

  http://www.fokus.gmd.de/ovma/freeware/snacc/entry.html

Please send comments and bug reports to the new maintainer you will find
in the above web page.

Newer Snacc versions will continue to be made available at:

  ftp://ftp.fokus.gmd.de/pub/freeware/snacc/

Robert Joop, 1997-02-16



For more up-to-date information, please refer to doc/snacc.ps.

The bug-report address given below was for Snacc 1.1.




snacc ASN.1 Compiler Release  1.1 - (updated Jul 93)
-----------------------------------------------------

This package provides the C/yacc/lex source code for snacc, an ASN.1
to C or C++ compiler.  Snacc is short for "Sample Neufeld Asn.1 to C
Compiler" and ASN.1 stands for Abstract Syntax Notation One (CCITT
X.208/ ISO 8824).  See the COPYRIGHT file for copyright information.


What It Does
------------

Given an ASN.1 source file(s) snacc can produce:

   1. C routines for BER encoding, decoding, printing and freeing.
   2. C++ routines for BER encoding, decoding, and printing.
   3. A type table that can be used with C driver routines
      for BER encoding, decoding, printing and freeing.
   4. if you are really lucky, a core dump! :)

Snacc produces reasonably efficient C or C++ routines and data
structures to support BER encoding and decoding values of the given
ASN.1 data structures.  Printing routines for the decoded values are
also generated.  For C only, hierarchical freeing routines are
generated (but are not recommended for efficiency reasons).

When using the C or C++ (not table) options, snacc creates the
following C files for each ASN.1 source file (module):

        1. .h file for the C/C++ translation of the ASN.1 data
           structure and prototypes for the generated routines.

        2. .c/.C file for the C/C++ encode, decode, print and free
           routines.

When using the table option, snacc produces a type table file
(a BER encoding) that can later be loaded at runtime by the
table driven encoding and decoding routines.

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


Bugs are still quite likely even though this is the second release of
snacc.  if you find any or have other comments please email:
   snacc-bugs@cs.ubc.ca


To Install It
-------------

By default, the snacc makefile uses GNU bison and GNU flex (instead of
yacc and lex).  If you do not have these installed, see the
snacc/src/README and snacc/src/makefile.  Otherwise, type "make" in
this directory.  Hopefully this will put the snacc binary in the
snacc/bin directory and build the C runtime library, the table library
and the table tools. Alternatively, type "make all" to install these
and the C++ library.

Compiling problems may occurr on different flavours of UNIX but they
should not be too difficult fix.  Likely sources of problems include
non-standard include files and lex, yacc and cc idiosyncrasies.
If your yacc croaks with a message about too many productions use
bison.  Same thing with lex -> use flex.   Bison and flex are both
freely available from numerous ftp sites. (even cs.ubc.ca in /mirror2/gnu)

The makefiles take advantage of the file inclusion feature. Since this
has already been supported by UNIX System III make (somewhen around
1980), I consider it to be pretty portable. If your make is crippled,
either use a newer one (e.g. GNU make), or as a last resort, remove the
include statements and call make with `-f makehead makefile maketail'.
  The dependencies have been moved into a separate file that is not
under cvs control. Otherwise, the makefiles would inflate the
repository unnecessarily. GNU make automatically makes the dependencies
if the file does not exist, but other makes simply give up. In that
case, an initial (empty) file has to be generated. The top level
makefile does this for you.
  The behaviour of makedepend has been changed between X11R5 and R6.
The new version keeps the source files' dirname and replaces the suffix
only, the old version removed the dirname.  The makefiles expect the
new behaviour. If you've still got the R5 makedepend, the compiler's
dependencies will be useless.  (If you only install the code and don't
make any changes, this won't hurt you.)

The snacc compiler and the generated code will compile under ANSI and
non-ANSI C compilers.  See the README and the makefile in the
snacc/src directory for more compiling information.  Snacc has been
successfully installed on Sun SPARCs, HP700s, IBM RS 6000s, MIPS,
i486/Linux, Alpha OSF/1 and many other machines.

#-------------------------------------------------------------------------------
# $Header: /baseline/SNACC/readme.txt,v 1.6 2004/04/22 15:25:02 gronej Exp $
# $Log: readme.txt,v $
# Revision 1.6  2004/04/22 15:25:02  gronej
# release  modifications
#
# Revision 1.5  2003/12/17 19:05:02  gronej
# SNACC baseline merged with PER v1_7 tag
#
# Revision 1.4.2.3  2003/12/16 15:50:27  gronej
# EKMS PER release
#
# Revision 1.4.2.1  2003/11/06 14:57:33  colestor
# new for v1_7 baseline
#
# Revision 1.3  2003/08/06 13:05:33  colestor
# New release notes, changed version to 1.6Beta.
#
# Revision 1.2  2003/08/05 17:18:21  colestor
# Updates for interim 1.5Beta release.
#
# Revision 1.1  2002/07/19 15:03:30  leonberp
# added new readme.txt removed old readmes
#
# Revision 1.1.1.1  2000/08/21 20:35:45  leonberp
# First CVS Version of SNACC.
#
# Revision 1.13  1997/09/16 14:57:59  wan
# Proper description of last-minute changes
#
# Revision 1.12  1997/09/05 07:51:04  wan
# Updated
#
# Revision 1.11  1997/08/28 09:48:08  wan
# Added description of changes
#
# Revision 1.10  1997/06/19 09:32:22  wan
# Added 1.3b3 change description
#
# Revision 1.9  1997/05/07 15:20:57  wan
# Added (limited) size constraints, bitstring and enumeration names to tables
#
# Revision 1.8  1997/04/07 13:14:16  wan
# Final README for 1.3b1
#
# Revision 1.7  1997/03/20 15:50:24  wan
# Push to 1.3b1.
#
# Revision 1.6  1997/02/16 22:26:40  rj
# url corrected
#
# Revision 1.5  1997/02/16 20:23:14  rj
# final release of snacc 1.2rj
#
# Revision 1.4  1995/09/07 20:54:03  rj
# check-in for release 1.2rj.8
#
# Revision 1.3  1995/02/22  09:54:05  rj
# added Alpha OSF/1 to the list of supported platforms.
#
# Revision 1.2  1994/09/01  01:40:54  rj
# some word about makefile inclusion and dependencies added.
#
# Revision 1.1  1994/08/28  11:02:04  rj
# first check-in
