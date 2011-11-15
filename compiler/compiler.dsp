# Microsoft Developer Studio Project File - Name="compiler" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=compiler - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "compiler.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "compiler.mak" CFG="compiler - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "compiler - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "compiler - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "compiler"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../SMPDist/bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /I "core" /I "back-ends" /I "../c-lib/inc" /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "COMPILER" /D "USE_EXP_BUF" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 /nologo /subsystem:console /pdb:none /map /machine:I386 /out:"../../SMPDist/bin/esnacc.exe"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../SMPDist/bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /I "core" /I "back-ends" /I "../c-lib/inc" /I ".." /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "COMPILER" /D "USE_GEN_BUF" /D "WIN32" /Fr /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/compiler.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /pdb:none /debug /machine:I386 /out:"../../SMPDist/bin/esnaccd.exe"

!ENDIF 

# Begin Target

# Name "compiler - Win32 Release"
# Name "compiler - Win32 Debug"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=core\asn1module.h
# End Source File
# Begin Source File

SOURCE=.\bison.simple
# End Source File
# Begin Source File

SOURCE=core\define.c
# End Source File
# Begin Source File

SOURCE=core\define.h
# End Source File
# Begin Source File

SOURCE=core\dependency.c
# End Source File
# Begin Source File

SOURCE="core\do-macros.c"
# End Source File
# Begin Source File

SOURCE="core\enc-rules.c"
# End Source File
# Begin Source File

SOURCE="core\enc-rules.h"
# End Source File
# Begin Source File

SOURCE="core\err-chk.c"
# End Source File
# Begin Source File

SOURCE=core\exports.c
# End Source File
# Begin Source File

SOURCE=core\exports.h
# End Source File
# Begin Source File

SOURCE="core\gen-tbls.c"
# End Source File
# Begin Source File

SOURCE=.\core\gfsi.c
# End Source File
# Begin Source File

SOURCE="core\lex-asn1.c"
# ADD CPP /D "YY_USE_PROTOS"
# End Source File
# Begin Source File

SOURCE=".\core\lex-asn1.l"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\core\lex-asn1.l"
InputName=lex-asn1

"core\$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -ocore/$(InputName).c core/$(InputName).l

# End Custom Build

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\core\lex-asn1.l"
InputName=lex-asn1

"core\$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex -ocore/$(InputName).c core/$(InputName).l

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="core\lex-stuff.h"
# End Source File
# Begin Source File

SOURCE="core\lib-types.c"
# End Source File
# Begin Source File

SOURCE="core\lib-types.h"
# End Source File
# Begin Source File

SOURCE="core\link-types.c"
# End Source File
# Begin Source File

SOURCE="core\link-values.c"
# End Source File
# Begin Source File

SOURCE=core\mem.c
# End Source File
# Begin Source File

SOURCE=core\mem.h
# End Source File
# Begin Source File

SOURCE=core\meta.c
# End Source File
# Begin Source File

SOURCE=core\meta.h
# End Source File
# Begin Source File

SOURCE=core\normalize.c
# End Source File
# Begin Source File

SOURCE=core\oid.c
# End Source File
# Begin Source File

SOURCE="core\parse-asn1.c"
# End Source File
# Begin Source File

SOURCE="core\parse-asn1.h"
# End Source File
# Begin Source File

SOURCE=".\core\parse-asn1.y"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\core\parse-asn1.y"
InputName=parse-asn1

BuildCmds= \
	bison -t -d -v -o .\core\$(InputName).c core/$(InputName).y

"core\$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"core\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\core\parse-asn1.y"
InputName=parse-asn1

BuildCmds= \
	bison -t -d -v -o .\core\$(InputName).c core/$(InputName).y

"core\$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"core\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=core\print.c
# End Source File
# Begin Source File

SOURCE=core\print.h
# End Source File
# Begin Source File

SOURCE=core\recursive.c
# End Source File
# Begin Source File

SOURCE="core\snacc-util.c"
# End Source File
# Begin Source File

SOURCE="core\snacc-util.h"
# End Source File
# Begin Source File

SOURCE=core\snacc.c
# End Source File
# Begin Source File

SOURCE=..\asn1specs\tbl.asn1
# End Source File
# Begin Source File

SOURCE=core\tbl.c
# End Source File
# Begin Source File

SOURCE=core\tbl.h
# End Source File
# Begin Source File

SOURCE="core\val-parser.c"
# End Source File
# End Group
# Begin Group "Back-ends"

# PROP Default_Filter ""
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE="back-ends\c-gen\gen-any.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-code.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-dec.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-enc.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-free.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-print.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-type.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\gen-vals.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\kwd.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\rules.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\rules.h"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\type-info.c"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\type-info.h"
# End Source File
# Begin Source File

SOURCE="back-ends\c-gen\util.c"
# End Source File
# Begin Source File

SOURCE=".\back-ends\c-gen\util.h"
# End Source File
# End Group
# Begin Group "C++"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\back-ends\c++-gen\cxxconstraints.c"
# End Source File
# Begin Source File

SOURCE=".\back-ends\c++-gen\cxxconstraints.h"
# End Source File
# Begin Source File

SOURCE=".\back-ends\c++-gen\cxxmultipleconstraints.c"
# End Source File
# Begin Source File

SOURCE=".\back-ends\c++-gen\cxxmultipleconstraints.h"
# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\gen-any.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\gen-code.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\gen-vals.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\kwd.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\rules.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\rules.h"
# End Source File
# Begin Source File

SOURCE="back-ends\c++-gen\types.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/C++"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/C++"

!ENDIF 

# End Source File
# End Group
# Begin Group "IDL"

# PROP Default_Filter ""
# Begin Source File

SOURCE="back-ends\idl-gen\gen-any.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/IDL"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/IDL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\idl-gen\gen-code.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/IDL"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/IDL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\idl-gen\gen-vals.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/IDL"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/IDL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\idl-gen\rules.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/IDL"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/IDL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="back-ends\idl-gen\rules.h"
# End Source File
# Begin Source File

SOURCE="back-ends\idl-gen\types.c"

!IF  "$(CFG)" == "compiler - Win32 Release"

# PROP Intermediate_Dir "Release/IDL"

!ELSEIF  "$(CFG)" == "compiler - Win32 Debug"

# PROP Intermediate_Dir "Debug/IDL"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE="back-ends\cond.c"
# End Source File
# Begin Source File

SOURCE="back-ends\str-util.c"
# End Source File
# Begin Source File

SOURCE="back-ends\str-util.h"
# End Source File
# Begin Source File

SOURCE="back-ends\tag-util.c"
# End Source File
# Begin Source File

SOURCE="back-ends\tag-util.h"
# End Source File
# End Group
# Begin Group "Library Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\c-lib\inc\asn-any.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-bits.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-BMPString.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-bool.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-config.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-der.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-enum.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-IA5String.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-incl.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-int.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-len.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-list.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-null.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-NumericString.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-octs.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-oid.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-PrintableStr.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-real.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-relative-oid.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-tag.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-TeletexString.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-UniversalString.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-useful.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-UTF8String.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\asn-VisibleString.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\exp-buf.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\gen-buf.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\hash.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\mem.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\min-buf.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\nibble-alloc.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\print.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\sbuf.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\snaccCder.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\str-stk.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-dbg.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-dec.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-enc.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-free.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-gen-c-hdr.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-gen.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-incl.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-print.h"
# End Source File
# Begin Source File

SOURCE="..\c-lib\inc\tbl-util.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\config_win32.h
# End Source File
# Begin Source File

SOURCE=..\policy.h
# End Source File
# Begin Source File

SOURCE=..\snacc.h
# End Source File
# Begin Source File

SOURCE=..\version.h
# End Source File
# End Group
# End Target
# End Project
