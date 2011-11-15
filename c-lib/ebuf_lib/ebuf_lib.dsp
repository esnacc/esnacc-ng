# Microsoft Developer Studio Project File - Name="c_asn1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=c_asn1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ebuf_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ebuf_lib.mak" CFG="c_asn1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "c_asn1 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "c_asn1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ebuf_lib"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../SMPDist/lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIB_EBUF_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /I "..\.." /I "..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "USE_EXP_BUF" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /pdb:none /machine:I386 /def:".\snaccc.def" /out:"../../../SMPDist/lib/casn1.dll"
# Begin Custom Build - Copying DLL to system32 directory...
InputPath=\devel.d\develR2.4Beta_3.d\SMPDist\lib\casn1.dll
InputName=casn1
SOURCE="$(InputPath)"

"$(WINDIR)\system32\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" %WINDIR%\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../SMPDist/lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIB_EBUF_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /I "..\.." /I "..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "USE_GEN_BUF" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/ebuf_lib.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /pdb:none /debug /machine:I386 /def:".\snaccc.def" /out:"../../../SMPDist/lib/casn1_d.dll"
# Begin Custom Build - Copying DLL to system32 directory...
InputPath=\devel.d\develR2.4Beta_3.d\SMPDist\lib\casn1_d.dll
InputName=casn1_d
SOURCE="$(InputPath)"

"$(WINDIR)\system32\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" %WINDIR%\system32

# End Custom Build

!ENDIF 

# Begin Target

# Name "c_asn1 - Win32 Release"
# Name "c_asn1 - Win32 Debug"
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

SOURCE="..\src\asn-VisibleString.c"
# End Source File
# Begin Source File

SOURCE=..\src\ASN1init.c
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

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-any.h"
InputName=asn-any

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-any.h"
InputName=asn-any

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-bits.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bits.h"
InputName=asn-bits

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bits.h"
InputName=asn-bits

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-BMPString.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-BMPString.h"
InputName=asn-BMPString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-BMPString.h"
InputName=asn-BMPString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-bool.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bool.h"
InputName=asn-bool

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-bool.h"
InputName=asn-bool

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-config.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-config.h"
InputName=asn-config

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-config.h"
InputName=asn-config

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-der.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-der.h"
InputName=asn-der

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-der.h"
InputName=asn-der

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-enum.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-enum.h"
InputName=asn-enum

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-enum.h"
InputName=asn-enum

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-IA5String.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-IA5String.h"
InputName=asn-IA5String

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-IA5String.h"
InputName=asn-IA5String

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-incl.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-int.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-int.h"
InputName=asn-int

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-int.h"
InputName=asn-int

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-len.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-len.h"
InputName=asn-len

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-len.h"
InputName=asn-len

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-list.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-list.h"
InputName=asn-list

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-list.h"
InputName=asn-list

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-null.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-null.h"
InputName=asn-null

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-null.h"
InputName=asn-null

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-NumericString.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-NumericString.h"
InputName=asn-NumericString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-NumericString.h"
InputName=asn-NumericString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-octs.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-octs.h"
InputName=asn-octs

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-octs.h"
InputName=asn-octs

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-oid.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-oid.h"
InputName=asn-oid

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-oid.h"
InputName=asn-oid

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-PrintableStr.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-PrintableStr.h"
InputName=asn-PrintableStr

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-PrintableStr.h"
InputName=asn-PrintableStr

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-real.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-real.h"
InputName=asn-real

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-real.h"
InputName=asn-real

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-relative-oid.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-relative-oid.h"
InputName=asn-relative-oid

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-relative-oid.h"
InputName=asn-relative-oid

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-tag.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-tag.h"
InputName=asn-tag

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-tag.h"
InputName=asn-tag

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-TeletexString.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-TeletexString.h"
InputName=asn-TeletexString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-TeletexString.h"
InputName=asn-TeletexString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-UniversalString.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UniversalString.h"
InputName=asn-UniversalString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UniversalString.h"
InputName=asn-UniversalString

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-useful.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-useful.h"
InputName=asn-useful

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-useful.h"
InputName=asn-useful

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-UTF8String.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UTF8String.h"
InputName=asn-UTF8String

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\asn-UTF8String.h"
InputName=asn-UTF8String

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\config_win32.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\config_win32.h
InputName=config_win32

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\config_win32.h
InputName=config_win32

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\exp-buf.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\exp-buf.h"
InputName=exp-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\exp-buf.h"
InputName=exp-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\gen-buf.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\gen-buf.h"
InputName=gen-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\gen-buf.h"
InputName=gen-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\hash.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\hash.h
InputName=hash

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\hash.h
InputName=hash

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\mem.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\mem.h
InputName=mem

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\mem.h
InputName=mem

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\min-buf.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\min-buf.h"
InputName=min-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\min-buf.h"
InputName=min-buf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\nibble-alloc.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\nibble-alloc.h"
InputName=nibble-alloc

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\nibble-alloc.h"
InputName=nibble-alloc

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\policy.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\policy.h
InputName=policy

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\policy.h
InputName=policy

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\print.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\print.h
InputName=print

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\print.h
InputName=print

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\sbuf.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\sbuf.h
InputName=sbuf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\sbuf.h
InputName=sbuf

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\snacc.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\snacc.h
InputName=snacc

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\..\snacc.h
InputName=snacc

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\snaccCder.h

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\snaccCder.h
InputName=snaccCder

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath=..\inc\snaccCder.h
InputName=snaccCder

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\str-stk.h"

!IF  "$(CFG)" == "c_asn1 - Win32 Release"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\str-stk.h"
InputName=str-stk

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ELSEIF  "$(CFG)" == "c_asn1 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath) to SMPDist directory...
InputPath="..\inc\str-stk.h"
InputName=str-stk

"../../../SMPDist/include/esnacc/c/$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "../../../SMPDist/include/esnacc/c"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\snaccc.def
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
