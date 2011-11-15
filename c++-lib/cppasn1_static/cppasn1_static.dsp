# Microsoft Developer Studio Project File - Name="cppasn1_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cppasn1_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cppasn1_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cppasn1_static.mak" CFG="cppasn1_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cppasn1_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cppasn1_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "cppasn1_static"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cppasn1_static - Win32 Release"

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
# ADD CPP /nologo /MD /W4 /GX /I "..\inc" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_WINDOWS" /D "SNACCDLL_NONE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../SMPDist/lib/cppasn1_static.lib"

!ELSEIF  "$(CFG)" == "cppasn1_static - Win32 Debug"

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
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /I "..\inc" /I "..\..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SNACCDLL_NONE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../SMPDist/lib/cppasn1_static_d.lib"

!ENDIF 

# Begin Target

# Name "cppasn1_static - Win32 Release"
# Name "cppasn1_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
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

SOURCE="..\src\asn-bufbits.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-enum.cpp"
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
# End Source File
# Begin Source File

SOURCE="..\src\asn-PERGeneral.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-real.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\asn-rvsbuf.cpp"
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

SOURCE=..\src\config.h
# End Source File
# Begin Source File

SOURCE=..\src\hash.cpp
# End Source File
# Begin Source File

SOURCE=..\src\meta.cpp
# End Source File
# Begin Source File

SOURCE=..\src\policy.h
# End Source File
# Begin Source File

SOURCE=..\src\print.cpp
# End Source File
# Begin Source File

SOURCE=..\src\snacc.h
# End Source File
# Begin Source File

SOURCE=..\src\snaccexcept.cpp
# End Source File
# Begin Source File

SOURCE=..\src\vda_threads2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\inc\asn-buf.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-config.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-enum.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-incl.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-len.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-list.h"
# End Source File
# Begin Source File

SOURCE="..\inc\asn-usefultypes.h"
# End Source File
# Begin Source File

SOURCE=..\inc\hash.h
# End Source File
# Begin Source File

SOURCE=..\inc\init.h
# End Source File
# Begin Source File

SOURCE=..\inc\meta.h
# End Source File
# Begin Source File

SOURCE=..\inc\print.h
# End Source File
# Begin Source File

SOURCE=..\inc\sm_vdasnacc.h
# End Source File
# Begin Source File

SOURCE=..\inc\snaccexcept.h
# End Source File
# Begin Source File

SOURCE="..\inc\str-stk.h"
# End Source File
# Begin Source File

SOURCE="..\inc\tcl-if.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
