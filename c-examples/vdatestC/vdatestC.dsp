# Microsoft Developer Studio Project File - Name="vdatestC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=vdatestC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "vdatestC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "vdatestC.mak" CFG="vdatestC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "vdatestC - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "vdatestC - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SNACC/c-examples/vdatestC"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vdatestC - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /I "..\..\c-lib\inc" /I "..\.." /I "..\..\c-lib\boot" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_EXP_BUF" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "vdatestC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "..\\" /I "..\..\c-lib\inc" /I "..\.." /I "..\..\c-lib\boot" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "USE_GEN_BUF" /FR /FD /GZ /c
# SUBTRACT CPP /u /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:none /debug /machine:I386

!ENDIF 

# Begin Target

# Name "vdatestC - Win32 Release"
# Name "vdatestC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\gfsi.c
# End Source File
# Begin Source File

SOURCE=".\rfc1155-smi.c"
# End Source File
# Begin Source File

SOURCE=".\rfc1157-snmp.c"
# End Source File
# Begin Source File

SOURCE=.\snmp.c
# End Source File
# Begin Source File

SOURCE=.\vdatest_asn2C.c
# End Source File
# Begin Source File

SOURCE=.\vdatestC.c
# End Source File
# Begin Source File

SOURCE=.\vdatestC_asn.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\rfc1155-smi.h"
# End Source File
# Begin Source File

SOURCE=".\rfc1157-snmp.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\vdatest_asn2C.h
# End Source File
# Begin Source File

SOURCE=.\vdatestC_asn.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ASN Modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\rfc1155-smi.asn1"

!IF  "$(CFG)" == "vdatestC - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\rfc1155-smi.asn1"
InputName=rfc1155-smi

BuildCmds= \
	..\..\..\SMPDist\bin\esnacc.exe  -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vdatestC - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RFC11="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputPath=".\rfc1155-smi.asn1"
InputName=rfc1155-smi

BuildCmds= \
	..\..\..\SMPDist\bin\esnaccd.exe   -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\rfc1157-snmp.asn1"

!IF  "$(CFG)" == "vdatestC - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\rfc1157-snmp.asn1"
InputName=rfc1157-snmp

BuildCmds= \
	..\..\..\SMPDist\bin\esnacc.exe  -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vdatestC - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RFC115="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputPath=".\rfc1157-snmp.asn1"
InputName=rfc1157-snmp

BuildCmds= \
	..\..\..\SMPDist\bin\esnaccd.exe   -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vdatest_asn2C.asn1

!IF  "$(CFG)" == "vdatestC - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\vdatest_asn2C.asn1
InputName=vdatest_asn2C

BuildCmds= \
	..\..\..\SMPDist\bin\esnacc.exe  -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vdatestC - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VDATE="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputPath=.\vdatest_asn2C.asn1
InputName=vdatest_asn2C

BuildCmds= \
	..\..\..\SMPDist\bin\esnaccd.exe   -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\vdatestC_asn.asn1

!IF  "$(CFG)" == "vdatestC - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\vdatestC_asn.asn1
InputName=vdatestC_asn

BuildCmds= \
	..\..\..\SMPDist\bin\esnacc.exe  -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "vdatestC - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VDATES="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputPath=.\vdatestC_asn.asn1
InputName=vdatestC_asn

BuildCmds= \
	..\..\..\SMPDist\bin\esnaccd.exe   -E DER -c -I . $(InputPath)

"$(InputName).c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
