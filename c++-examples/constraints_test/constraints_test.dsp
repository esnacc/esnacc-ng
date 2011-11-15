# Microsoft Developer Studio Project File - Name="constraints_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=constraints_test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "constraints_test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "constraints_test.mak" CFG="constraints_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "constraints_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "constraints_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "constraints_test"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "constraints_test - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\src" /I "..\..\c++-lib\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "constraints_test - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\src" /I "..\..\c++-lib\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "constraints_test - Win32 Release"
# Name "constraints_test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\constraints_test.cpp
# End Source File
# Begin Source File

SOURCE=.\constraintTest.cpp
# End Source File
# Begin Source File

SOURCE=.\constraintTest_main.cpp
# End Source File
# Begin Source File

SOURCE=.\constraintTest_mainRWC.cpp
# End Source File
# Begin Source File

SOURCE=".\new-types.cpp"
# End Source File
# Begin Source File

SOURCE=.\perTest.cpp
# End Source File
# Begin Source File

SOURCE=.\perTestSet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\constraintTest.h
# End Source File
# Begin Source File

SOURCE=".\new-types.h"
# End Source File
# Begin Source File

SOURCE=.\perTest.h
# End Source File
# Begin Source File

SOURCE=.\perTestSet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "ASN Modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\constraintTest.asn1

!IF  "$(CFG)" == "constraints_test - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=.\constraintTest.asn1
InputName=constraintTest

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "constraints_test - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__CONST="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputDir=.
InputPath=.\constraintTest.asn1
InputName=constraintTest

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\new-types.asn1"

!IF  "$(CFG)" == "constraints_test - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=".\new-types.asn1"
InputName=new-types

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "constraints_test - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__NEW_T="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputDir=.
InputPath=".\new-types.asn1"
InputName=new-types

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\perTest.asn1

!IF  "$(CFG)" == "constraints_test - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=.\perTest.asn1
InputName=perTest

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "constraints_test - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PERTE="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputDir=.
InputPath=.\perTest.asn1
InputName=perTest

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\perTestSet.asn1

!IF  "$(CFG)" == "constraints_test - Win32 Release"

# Begin Custom Build
InputDir=.
InputPath=.\perTestSet.asn1
InputName=perTestSet

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnacc.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "constraints_test - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__PERTES="..\..\..\SMPDist\bin\esnaccd.exe"	
# Begin Custom Build
InputDir=.
InputPath=.\perTestSet.asn1
InputName=perTestSet

BuildCmds= \
	cd $(InputDir) \
	..\..\..\SMPDist\bin\esnaccd.exe -C -b -I . $(InputName).asn1 \
	

"$(InputDir)\$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
