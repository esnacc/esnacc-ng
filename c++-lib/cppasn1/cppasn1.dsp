# Microsoft Developer Studio Project File - Name="cppasn1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cppasn1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cppasn1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cppasn1.mak" CFG="cppasn1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cppasn1 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cppasn1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "cppasn1"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CPPASN1_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\inc" /I "..\cppasn1" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CPPASN1_EXPORTS" /D "SNACCDLL_EXPORTS" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386
# Begin Custom Build - Copying $(InputPath) to system32 directory...
InputPath=\rerelease\SMPDist\lib\cppasn1.dll
InputName=cppasn1
SOURCE="$(InputPath)"

"$(WINDIR)\system32\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" %WINDIR%\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CPPASN1_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /I "..\..\\" /I "..\inc" /I "..\cppasn1" /D "_DEBUG" /D "_USRDLL" /D "SNACCDLL_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /pdb:none /map /debug /machine:I386 /out:"../../../SMPDist/lib/cppasn1_d.dll"
# Begin Custom Build - Copying $(InputPath) to system32 directory...
InputPath=\rerelease\SMPDist\lib\cppasn1_d.dll
InputName=cppasn1_d
SOURCE="$(InputPath)"

"$(WINDIR)\system32\$(InputName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" %WINDIR%\system32

# End Custom Build

!ENDIF 

# Begin Target

# Name "cppasn1 - Win32 Release"
# Name "cppasn1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\src\asn-any.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-bits.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-bool.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-buf.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-bufbits.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-enum.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-extension.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-fileseg.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-int.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-len.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-null.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-octs.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-oid.cpp"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# ADD CPP /O2
# SUBTRACT CPP /Z<none>

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\src\asn-PERGeneral.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-real.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-RelativeOid.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-rvsbuf.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-stringtype.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-tag.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-type.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-usefultypes.cpp"
# End Source File
# Begin Source File

SOURCE=..\src\hash.cpp
# End Source File
# Begin Source File

SOURCE=..\src\meta.cpp
# End Source File
# Begin Source File

SOURCE=..\src\print.cpp
# End Source File
# Begin Source File

SOURCE=..\src\snaccexcept.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vda_threads2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\inc\asn-buf.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\asn-buf.h"
InputName=asn-buf

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\asn-buf.h"
InputName=asn-buf

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-config.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\asn-config.h"
InputName=asn-config

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\asn-config.h"
InputName=asn-config

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-incl.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\asn-incl.h"
InputName=asn-incl

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-listset.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\asn-listset.h"
InputName=asn-listset

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\asn-listset.h"
InputName=asn-listset

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\asn-usefultypes.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\asn-usefultypes.h"
InputName=asn-usefultypes

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\asn-usefultypes.h"
InputName=asn-usefultypes

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\init.h

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath=..\inc\init.h
InputName=init

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath=..\inc\init.h
InputName=init

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\meta.h

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath=..\inc\meta.h
InputName=meta

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath=..\inc\meta.h
InputName=meta

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\inc\snaccexcept.h

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath=..\inc\snaccexcept.h
InputName=snaccexcept

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath=..\inc\snaccexcept.h
InputName=snaccexcept

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\inc\tcl-if.h"

!IF  "$(CFG)" == "cppasn1 - Win32 Release"

# Begin Custom Build
InputPath="..\inc\tcl-if.h"
InputName=tcl-if

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ELSEIF  "$(CFG)" == "cppasn1 - Win32 Debug"

# Begin Custom Build
InputPath="..\inc\tcl-if.h"
InputName=tcl-if

"..\..\..\SMPDist\include\esnacc\c++\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) "..\..\..\SMPDist\include\esnacc\c++"

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
