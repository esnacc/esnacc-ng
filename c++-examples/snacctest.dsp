# Microsoft Developer Studio Project File - Name="snacctest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=snacctest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "snacctest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "snacctest.mak" CFG="snacctest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "snacctest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "snacctest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "snacctest"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "snacctest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\src" /I "..\c++-lib\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "snacctest - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\src" /I "..\c++-lib\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /Zm400 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/snacctestd.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "snacctest - Win32 Release"
# Name "snacctest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\asnutil.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bitstest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\gfsi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\inttest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=".\src\rfc1155-smi.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\rfc1157-snmp.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\snmp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\testbuf.cpp
# End Source File
# Begin Source File

SOURCE=.\src\testsetsorting.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vda_threads.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vdatest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\asnutil.h
# End Source File
# Begin Source File

SOURCE=".\src\rfc1155-smi.h"
# End Source File
# Begin Source File

SOURCE=".\src\rfc1157-snmp.h"
# End Source File
# Begin Source File

SOURCE=.\src\vda_threads.h
# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn.h
# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ASN Modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\src\rfc1155-smi.asn1"

!IF  "$(CFG)" == "snacctest - Win32 Release"

USERDEP__RFC11="..\..\SMPDist\bin\esnacc.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=".\src\rfc1155-smi.asn1"
InputName=rfc1155-smi

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "snacctest - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RFC11="..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=".\src\rfc1155-smi.asn1"
InputName=rfc1155-smi

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\src\rfc1157-snmp.asn1"

!IF  "$(CFG)" == "snacctest - Win32 Release"

USERDEP__RFC115="..\..\SMPDist\bin\esnacc.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=".\src\rfc1157-snmp.asn1"
InputName=rfc1157-snmp

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "snacctest - Win32 Debug"

USERDEP__RFC115="..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=".\src\rfc1157-snmp.asn1"
InputName=rfc1157-snmp

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn.asn1

!IF  "$(CFG)" == "snacctest - Win32 Release"

USERDEP__VDATE="..\..\SMPDist\bin\esnacc.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=.\src\vdatest_asn.asn1
InputName=vdatest_asn

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "snacctest - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VDATE="..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=.\src\vdatest_asn.asn1
InputName=vdatest_asn

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vdatest_asn2.asn1

!IF  "$(CFG)" == "snacctest - Win32 Release"

USERDEP__VDATES="..\..\SMPDist\bin\esnacc.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=.\src\vdatest_asn2.asn1
InputName=vdatest_asn2

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "snacctest - Win32 Debug"

USERDEP__VDATES="..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build - Performing eSNACC compile of $(InputPath)
InputDir=.\src
InputPath=.\src\vdatest_asn2.asn1
InputName=vdatest_asn2

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I. $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
