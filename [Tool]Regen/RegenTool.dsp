# Microsoft Developer Studio Project File - Name="RegenTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RegenTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RegenTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RegenTool.mak" CFG="RegenTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RegenTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RegenTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RegenTool - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib yhlibrary.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "RegenTool - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_REGENTOOL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib yhlibrary.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\CWorking/RegenTool.exe" /pdbtype:sept /libpath:"..\[Lib]YHLibrary"

!ENDIF 

# Begin Target

# Name "RegenTool - Win32 Release"
# Name "RegenTool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirectoryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\InMapMonsterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\line_node.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadList.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterListViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\RegenTool.cpp
# End Source File
# Begin Source File

SOURCE=.\RegenTool.rc
# End Source File
# Begin Source File

SOURCE=.\RegenToolDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RegenToolView.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectXY.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DefineStruct.h
# End Source File
# Begin Source File

SOURCE=.\DirectoryManager.h
# End Source File
# Begin Source File

SOURCE=.\InMapMonsterDlg.h
# End Source File
# Begin Source File

SOURCE=.\line_node.h
# End Source File
# Begin Source File

SOURCE=.\LoadList.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\MHFile.h
# End Source File
# Begin Source File

SOURCE=.\MonsterListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MonsterListViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\MyTracker.h
# End Source File
# Begin Source File

SOURCE=.\RegenTool.h
# End Source File
# Begin Source File

SOURCE=.\RegenToolDoc.h
# End Source File
# Begin Source File

SOURCE=.\RegenToolView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectXY.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Image\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\Delete_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\Delete_press.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\MonsterSelectArea_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\MonsterSelectArea_press.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\MonsterSelectNoArea_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\MonsterSelectNoArea_press.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\Move_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\Move_press.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RegenTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\RegenTool.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RegenToolDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Image\Select_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\Select_press.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\ZoomIn_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\ZoomIn_press.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\ZoomOut_on.bmp
# End Source File
# Begin Source File

SOURCE=.\Image\ZoomOut_press.bmp
# End Source File
# Begin Source File

SOURCE=".\Image\≥≠¡÷.bmp"
# End Source File
# End Group
# Begin Group "3DSource"

# PROP Default_Filter ""
# Begin Group "4DyuchiGRX_myself97_util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97_util\MouseLib.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97_util\MouseLib.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97_util\MToolMouseStatus.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97_util\MToolMouseStatus.h
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Engine.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineCamera.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineEffect.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineLight.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineLight.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineMap.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineObject.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineSky.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineSky.h
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\EngineSound.h
# End Source File
# Begin Source File

SOURCE=.\Engine\GraphicEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\GraphicEngine.h
# End Source File
# Begin Source File

SOURCE=.\Engine\SoundEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\SoundEngine.h
# End Source File
# End Group
# Begin Group "APPERANCE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppearanceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AppearanceManager.h
# End Source File
# End Group
# Begin Group "4DyuchiGXGFunc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\global.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\type.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\SS3DGFunc.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\MHMap.cpp
# End Source File
# Begin Source File

SOURCE=.\MHMap.h
# End Source File
# Begin Source File

SOURCE=.\Tile.cpp
# End Source File
# Begin Source File

SOURCE=.\Tile.h
# End Source File
# Begin Source File

SOURCE=.\TileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TileManager.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Input\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Input\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\Input\MouseEventReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Input\MouseEventReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Input\MouseEventThrower.cpp
# End Source File
# Begin Source File

SOURCE=.\Input\MouseEventThrower.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
