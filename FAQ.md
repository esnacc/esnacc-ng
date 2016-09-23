Frequently Asked Questions
==========================

Enhanced SNACC <http://esnacc-ng.org>

## Contents

- [General](#general)
- [Licensing](#licensing)


## General

### Q: What is eSNACC?

A: eSNACC is the Enhanced Sample Neufeld ASN.1 C Compiler.  It is a libre
   project with the goal of producing an ASN.1 to C/C++ (and more) code
   compiler.  ASN.1 is a standardized language maintained by the International
   Telecommunications Union.  The goal of ASN.1 is to produce a language which
   describes various types of data, and defines the framework by which that
   data is serialized and deserialized.


### Q: What versions of ASN.1 does eSNACC interoperate with?

A: eSNACC can support most of X.608/X.609 for Basic Encoding Rules.  Some
   notable exceptions exist (there are a few compliance issues with string
   formats and internationalization).


### Q: Which languages does eSNACC target?

A: Currently, eSNACC targets C and C++ languages.  There are some outstanding
   efforts to provide back-ends for python, java, C#, and go.

   Note that not every feature may be available in every language.  For example,
   some encoding rules may only be implemented in one language.  In these cases,
   the documentation should explain the differences.


### Q: Which encoding rules are support?  Do you support X enconding?

A: There is a good amount of support for Basic Encoding Rules.  Some
   Distinguished Encoding Rules and Packet Encoding Rules support exists, but
   it is very early stages.  Work is currently underway to enhance this and
   provide DER, PER, XER, and GSER support both in C and C++ languages.

   Other encodings will be added.  The best way to get your favorite encoding
   is to write a new set of support libraries and add the code generation. Then
   submit it as a patch (hopefully with tests for `make check`).


## Licensing

### Q: Why do you have such a strange set of licenses?

A: A bit of history.  The SNACC project began back when free licenses were not
   fully vetted.  The original code was licensed as both LGPL and GPL, but this
   was not workable (it would create a piece of code that was always GPL'd).

   It was relicensed for 1.1 as GPLv2 for the compiler, and "Free Software" for
   the library.  Although it is a bit fuzzy, the understanding is that this
   means you are free to run this work, modify this work, redistribute this
   work (including your modifications), and study the code this work (where
   this work means the parts of code which make up 'the library').

   In 2002/2003 timeframe, DigitalNet (a now defunct branch of Getronics) took
   the SNACC code and enhanced it to include better support for C++, and newer
   ASN.1 syntax.  These enhancements were added under the eSNACC Public License
   under contract to the US government.  They codify the 'Free Software' 
   license referenced above (although, obviously do not re-license the 
   software).


### Q: So am I free to use eSNACC in my application?

A: Yes.  If you incorporate the eSNACC compiler, you will be subject to the
   terms of the GNU GPL v2.  If you only use the compiler to generate code and
   then use the resulting code and support libraries, your new work can be 
   licensed however you like.


### Q: Have you vetted this by a lawyer?

A: A little bit, yes.
