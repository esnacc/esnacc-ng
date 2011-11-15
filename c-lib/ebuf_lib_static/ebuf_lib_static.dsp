# Microsoft Developer Studio Project File - Name="c_asn1_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=c_asn1_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ebuf_lib_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ebuf_lib_static.mak" CFG="c_asn1_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "c_asn1_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "c_asn1_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ebuf_lib_static"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /I "..\.." /I "..\inc" /D "NDEBUG" /D "USE_EXP_BUF" /D "_WINDOWS" /D "WIN32" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../SMPDist/lib/casn1_static.lib"

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /I "..\.." /I "..\inc" /D "_DEBUG" /D "USE_GEN_BUF" /D "_WINDOWS" /D "WIN32" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../SMPDist/lib/casn1_static_d.lib"

!ENDIF 

# Begin Target

# Name "c_asn1_static - Win32 Release"
# Name "c_asn1_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\src\asn-any.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-bits.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-BMPString.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-bool.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-der.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-enum.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-IA5String.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-int.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-len.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-list.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-null.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-NumericString.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-octs.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-oid.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-PrintableStr.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-real.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-relative-oid.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-tag.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-TeletexString.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-UniversalString.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-useful.c"
# End Source File
# Begin Source File

SOURCE="..\src\asn-UTF8String.c"
# End Source File
# Begin Source File

SOURCE="..\src\exp-buf.c"
# End Source File
# Begin Source File

SOURCE=..\src\hash.c
# End Source File
# Begin Source File

SOURCE=..\src\mem.c
# End Source File
# Begin Source File

SOURCE="..\src\nibble-alloc.c"
# End Source File
# Begin Source File

SOURCE=..\src\print.c
# End Source File
# Begin Source File

SOURCE="..\src\str-stk.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\inc\asn-any.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-any.h"
InputName=asn-any

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-any.h"
InputName=asn-any

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-bits.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bits.h"
InputName=asn-bits

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bits.h"
InputName=asn-bits

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-BMPString.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-BMPString.h"
InputName=asn-BMPString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-BMPString.h"
InputName=asn-BMPString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-bool.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bool.h"
InputName=asn-bool

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bool.h"
InputName=asn-bool

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-config.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-config.h"
InputName=asn-config

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-config.h"
InputName=asn-config

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-der.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-der.h"
InputName=asn-der

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-der.h"
InputName=asn-der

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-enum.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-enum.h"
InputName=asn-enum

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-enum.h"
InputName=asn-enum

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-IA5String.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-IA5String.h"
InputName=asn-IA5String

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-IA5String.h"
InputName=asn-IA5String

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-incl.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-int.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-int.h"
InputName=asn-int

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-int.h"
InputName=asn-int

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-len.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-len.h"
InputName=asn-len

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-len.h"
InputName=asn-len

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-list.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-list.h"
InputName=asn-list

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-list.h"
InputName=asn-list

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-null.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-null.h"
InputName=asn-null

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-null.h"
InputName=asn-null

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-NumericString.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-NumericString.h"
InputName=asn-NumericString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-NumericString.h"
InputName=asn-NumericString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-octs.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-octs.h"
InputName=asn-octs

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-octs.h"
InputName=asn-octs

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-oid.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-oid.h"
InputName=asn-oid

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-oid.h"
InputName=asn-oid

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-PrintableStr.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-PrintableStr.h"
InputName=asn-PrintableStr

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-PrintableStr.h"
InputName=asn-PrintableStr

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-real.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-real.h"
InputName=asn-real

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-real.h"
InputName=asn-real

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-relative-oid.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-relative-oid.h"
InputName=asn-relative-oid

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-relative-oid.h"
InputName=asn-relative-oid

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-tag.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-tag.h"
InputName=asn-tag

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-tag.h"
InputName=asn-tag

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-TeletexString.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-TeletexString.h"
InputName=asn-TeletexString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-TeletexString.h"
InputName=asn-TeletexString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-UniversalString.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UniversalString.h"
InputName=asn-UniversalString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UniversalString.h"
InputName=asn-UniversalString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-useful.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-useful.h"
InputName=asn-useful

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-useful.h"
InputName=asn-useful

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-UTF8String.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UTF8String.h"
InputName=asn-UTF8String

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UTF8String.h"
InputName=asn-UTF8String

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-VisibleString.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\asn-VisibleString.h
InputName=asn-VisibleString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\asn-VisibleString.h
InputName=asn-VisibleString

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\config_win32.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\config_win32.h
InputName=config_win32

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\config_win32.h
InputName=config_win32

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\exp-buf.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\exp-buf.h"
InputName=exp-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\exp-buf.h"
InputName=exp-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\gen-buf.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\gen-buf.h"
InputName=gen-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\gen-buf.h"
InputName=gen-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\hash.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\hash.h
InputName=hash

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\hash.h
InputName=hash

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\mem.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\mem.h
InputName=mem

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\mem.h
InputName=mem

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\min-buf.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\min-buf.h"
InputName=min-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\min-buf.h"
InputName=min-buf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\nibble-alloc.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\nibble-alloc.h"
InputName=nibble-alloc

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\nibble-alloc.h"
InputName=nibble-alloc

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\policy.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\policy.h
InputName=policy

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\policy.h
InputName=policy

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\print.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\print.h
InputName=print

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\print.h
InputName=print

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\sbuf.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\sbuf.h
InputName=sbuf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\sbuf.h
InputName=sbuf

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\snacc.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\snacc.h
InputName=snacc

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\snacc.h
InputName=snacc

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\snaccCder.h

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\snaccCder.h
InputName=snaccCder

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\snaccCder.h
InputName=snaccCder

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\str-stk.h"

!IF  "$(CFG)" == "c_asn1_static - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\str-stk.h"
InputName=str-stk

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1_static - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\str-stk.h"
InputName=str-stk

"..\..\..\SMPDist\include\esnacc\c\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
