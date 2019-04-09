# Microsoft Developer Studio Project File - Name="4DyuchiGXExecutive" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=4DyuchiGXExecutive - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXExecutive.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXExecutive.mak" CFG="4DyuchiGXExecutive - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4DyuchiGXExecutive - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGXExecutive - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/4DyuchiGXExecutive", SNCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "4DyuchiGXExecutive - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXEXECUTIVE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXEXECUTIVE_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x65000000" /dll /pdb:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXExecutive.pdb" /map:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXExecutive.map" /debug /machine:I386 /out:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXExecutive.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "4DyuchiGXExecutive - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXEXECUTIVE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXEXECUTIVE_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x65000000" /dll /pdb:"../../4DyuchiDLL/4DyuchiGX_Debug/4DyuchiGXExecutive.pdb" /map:"../../4DyuchiDLL/4DyuchiGX_Debug/4DyuchiGXExecutive.map" /debug /debugtype:both /machine:I386 /out:"../../4DyuchiDLL/4DyuchiGX_debug/4DyuchiGXExecutive.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "4DyuchiGXExecutive - Win32 Release"
# Name "4DyuchiGXExecutive - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4DyuchiGXEXECUTIVE.def
# End Source File
# Begin Source File

SOURCE=.\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\factory.cpp
# End Source File
# Begin Source File

SOURCE=.\GXDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\factory.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\FileStorage_typedef.h
# End Source File
# Begin Source File

SOURCE=.\global_variable.h
# End Source File
# Begin Source File

SOURCE=.\GXDefault.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "map"

# PROP Default_Filter ""
# Begin Group "myself97"

# PROP Default_Filter ""
# Begin Group "충돌처리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\AAEllipsoidCollisionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\AAEllipsoidCollisionTest.h
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\CollisionTestMovingEllipsoidMeetTriangle.cpp
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\CollisionTestMovingEllipsoidMeetTriangle.h
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\CollisionTestMovingSphereMeetTriangle.cpp
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CollisionTest\CollisionTestMovingSphereMeetTriangle.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\DummyCollisionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\DummyCollisionTest.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\SphereCollisionTest.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\SphereCollisionTest.h
# End Source File
# End Group
# Begin Group "StaticTree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\MStaticObjectTree.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\MStaticObjectTree.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CheckClock.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CheckClock.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\Logger.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\logger.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\MClipper.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\MClipper.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\Myself97Common.cpp

!IF  "$(CFG)" == "4DyuchiGXExecutive - Win32 Release"

!ELSEIF  "$(CFG)" == "4DyuchiGXExecutive - Win32 Debug"

# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\Myself97Common.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\TriangleLib.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\TriangleLib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# End Group
# Begin Group "executive"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\executive.cpp
# End Source File
# Begin Source File

SOURCE=.\executive.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\global.h
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_COMMON\GXSCHEDULE_PROC_MSG.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\math.inl
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\mesh_flag.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# End Group
# Begin Group "COM Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IExecutive.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IExecutive_GUID.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IFileStorage.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IGeometry.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IGeometry_GUID.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IRenderer.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IRenderer_GUID.h
# End Source File
# End Group
# Begin Group "object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GXDecal.cpp
# End Source File
# Begin Source File

SOURCE=.\GXDecal.h
# End Source File
# Begin Source File

SOURCE=.\GXEventTriggerObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GXEventTriggerObject.h
# End Source File
# Begin Source File

SOURCE=.\GXHFieldMapObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GXHFieldMapObject.h
# End Source File
# Begin Source File

SOURCE=.\GXLight.cpp
# End Source File
# Begin Source File

SOURCE=.\GXLight.h
# End Source File
# Begin Source File

SOURCE=.\GXMapObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GXMapObject.h
# End Source File
# Begin Source File

SOURCE=.\GXObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GXObject.h
# End Source File
# Begin Source File

SOURCE=.\GXStaticMapObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GXStaticMapObject.h
# End Source File
# End Group
# Begin Group "ShadeManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ShadeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeManager.h
# End Source File
# End Group
# End Target
# End Project
