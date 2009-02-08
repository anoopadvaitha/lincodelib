# Microsoft Developer Studio Project File - Name="UtilsDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=UtilsDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UtilsDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UtilsDemo.mak" CFG="UtilsDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UtilsDemo - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "UtilsDemo - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UtilsDemo - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../3rdparty/gdiplus/include" /I "../3rdparty/zip" /D "WIN32" /D "NDEBUG" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../3rdparty/gdiplus/lib"

!ELSEIF  "$(CFG)" == "UtilsDemo - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_windows" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../3rdparty/gdiplus/include" /I "../3rdparty/zip" /I "../3rdparty/encode" /I "../3rdparty/winxtest/help" /D "WIN32" /D "_DEBUG" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../3rdparty/gdiplus/lib"

!ENDIF 

# Begin Target

# Name "UtilsDemo - Win32 Release"
# Name "UtilsDemo - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\test.png
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\test\testencode.cpp
# End Source File
# Begin Source File

SOURCE=.\test\testimage.cpp
# End Source File
# Begin Source File

SOURCE=.\test\testinifile.cpp
# End Source File
# Begin Source File

SOURCE=.\test\TestMain.h
# End Source File
# Begin Source File

SOURCE=.\test\TestPropObj.cpp
# End Source File
# Begin Source File

SOURCE=.\test\teststream.h
# End Source File
# Begin Source File

SOURCE=.\test\teststringlist.cpp
# End Source File
# Begin Source File

SOURCE=.\test\testzip.cpp
# End Source File
# End Group
# Begin Group "3rdparty"

# PROP Default_Filter ""
# Begin Group "zip"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\3rdparty\zip\unzip.cpp
# End Source File
# Begin Source File

SOURCE=..\3rdparty\zip\unzip.h
# End Source File
# Begin Source File

SOURCE=..\3rdparty\zip\zip.cpp
# End Source File
# Begin Source File

SOURCE=..\3rdparty\zip\zip.h
# End Source File
# End Group
# End Group
# Begin Group "wdlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\CheckUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\ComUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\DebugUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\encodeutils.h
# End Source File
# Begin Source File

SOURCE=..\src\ErrorUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\EventUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\FileUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\gdiplusutils.h
# End Source File
# Begin Source File

SOURCE=..\src\GDIUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\IniFile.h
# End Source File
# Begin Source File

SOURCE=..\src\Initcpp.h
# End Source File
# Begin Source File

SOURCE=..\src\internalwnd.h
# End Source File
# Begin Source File

SOURCE=..\src\OleUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\propobj.h
# End Source File
# Begin Source File

SOURCE=..\src\streams.h
# End Source File
# Begin Source File

SOURCE=..\src\StringList.h
# End Source File
# Begin Source File

SOURCE=..\src\StrUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\threadeventinvoker.h
# End Source File
# Begin Source File

SOURCE=..\src\Threads.h
# End Source File
# Begin Source File

SOURCE=..\src\wdlib.h
# End Source File
# Begin Source File

SOURCE=..\src\wdTypes.h
# End Source File
# Begin Source File

SOURCE=..\src\WndUtils.h
# End Source File
# Begin Source File

SOURCE=..\src\zipper.h
# End Source File
# End Group
# Begin Group "demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UtilsDemo.cpp
# End Source File
# End Group
# End Target
# End Project
