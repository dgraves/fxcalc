# Microsoft Developer Studio Project File - Name="fxcalc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=fxcalc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fxcalc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fxcalc.mak" CFG="fxcalc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fxcalc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "fxcalc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fxcalc - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O1 /I "../include" /I "../doubledouble" /I "../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "HAVE_RTTI" /D "HAVE_DOUBLEDOUBLE" /D "DD_INLINE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 fox.lib comctl32.lib wsock32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /entry:"mainCRTStartup" /subsystem:windows /machine:I386 /libpath:"../../lib"

!ELSEIF  "$(CFG)" == "fxcalc - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "../include" /I "../doubledouble" /I "../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "HAVE_RTTI" /D "HAVE_DOUBLEDOUBLE" /D "DD_INLINE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 foxd.lib comctl32.lib wsock32.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib /nologo /entry:"mainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../lib"

!ENDIF 

# Begin Target

# Name "fxcalc - Win32 Release"
# Name "fxcalc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\calc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CALCDigitBox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CALCLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CALCList.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CALCStatBox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\CALCWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\src\icons.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\CALCdefs.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCDigitBox.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCHelp.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCLabel.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCList.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCStatBox.h
# End Source File
# Begin Source File

SOURCE=..\include\CALCWindow.h
# End Source File
# Begin Source File

SOURCE=..\include\icons.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\fxcalc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\fxcalc.res
# End Source File
# End Target
# End Project
