# Microsoft Developer Studio Project File - Name="4DyuchiGXMapEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=4DyuchiGXMapEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXMapEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXMapEditor.mak" CFG="4DyuchiGXMapEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4DyuchiGXMapEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "4DyuchiGXMapEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/4DyuchiGXMapEditor", JXBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "4DyuchiGXMapEditor - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "4DyuchiGXMapEditor - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /out:"../../4DyuchiGX/4DyuchiGXTools/4DyuchiGXMapEditor.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "4DyuchiGXMapEditor - Win32 Release"
# Name "4DyuchiGXMapEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4DyuchiGXMapEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\4DyuchiGXMapEditor.rc
# End Source File
# Begin Source File

SOURCE=.\4DyuchiGXMapEditorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\4DyuchiGXMapEditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomPane.cpp
# End Source File
# Begin Source File

SOURCE=.\brush.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDirectionalLightProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogHFieldProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTileBrushProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTilePalette.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarHField.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBarTool.cpp
# End Source File
# Begin Source File

SOURCE=.\helper.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemCount.cpp
# End Source File
# Begin Source File

SOURCE=.\LightPosDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LightProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadFailedTextureDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGX_UTIL\MouseLib.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGX_UTIL\MToolMouseStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\RightView.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TileBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\TileView.cpp
# End Source File
# Begin Source File

SOURCE=.\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\TriggerProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\Voidlist.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\4DyuchiGXMapEditor.h
# End Source File
# Begin Source File

SOURCE=.\4DyuchiGXMapEditorDoc.h
# End Source File
# Begin Source File

SOURCE=.\4DyuchiGXMapEditorView.h
# End Source File
# Begin Source File

SOURCE=.\BottomPane.h
# End Source File
# Begin Source File

SOURCE=.\brush.h
# End Source File
# Begin Source File

SOURCE=.\ColorPalette.h
# End Source File
# Begin Source File

SOURCE=.\DialogConsole.h
# End Source File
# Begin Source File

SOURCE=.\DialogDirectionalLightProperty.h
# End Source File
# Begin Source File

SOURCE=.\DialogHFieldProperty.h
# End Source File
# Begin Source File

SOURCE=.\DialogTileBrushProperty.h
# End Source File
# Begin Source File

SOURCE=.\DialogTilePalette.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarHField.h
# End Source File
# Begin Source File

SOURCE=.\DlgBarTool.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\FileStorage_typedef.h
# End Source File
# Begin Source File

SOURCE=.\FuncTable.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\global.h
# End Source File
# Begin Source File

SOURCE=.\GXSCHEDULE_PROC_MSG.h
# End Source File
# Begin Source File

SOURCE=.\helper.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IAudio.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IExecutive.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IFileStorage.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IGeometry.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\IRenderer.h
# End Source File
# Begin Source File

SOURCE=.\ItemCount.h
# End Source File
# Begin Source File

SOURCE=.\LightPosDlg.h
# End Source File
# Begin Source File

SOURCE=.\LightProperty.h
# End Source File
# Begin Source File

SOURCE=.\ListBoxContainer.h
# End Source File
# Begin Source File

SOURCE=.\LoadFailedTextureDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\math.inl
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGX_UTIL\MouseLib.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGX_UTIL\MToolMouseStatus.h
# End Source File
# Begin Source File

SOURCE=.\MyListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MySplitter.h
# End Source File
# Begin Source File

SOURCE=.\NpcInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObjectProperty.h
# End Source File
# Begin Source File

SOURCE=.\parser.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RightView.h
# End Source File
# Begin Source File

SOURCE=.\SectionInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TileBrush.h
# End Source File
# Begin Source File

SOURCE=.\TileTableDesc.h
# End Source File
# Begin Source File

SOURCE=.\TileView.h
# End Source File
# Begin Source File

SOURCE=.\Tool.h
# End Source File
# Begin Source File

SOURCE=.\TriggerProperty.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# Begin Source File

SOURCE=.\Voidlist.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\4DyuchiGXMapEditor.ico
# End Source File
# Begin Source File

SOURCE=.\res\4DyuchiGXMapEditorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_bott.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_lefp.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_left.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mode.ico
# End Source File
# Begin Source File

SOURCE=.\res\pick.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pick_color.bmp
# End Source File
# Begin Source File

SOURCE=.\sdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "TileSet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DialogEditEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogEditEntry.h
# End Source File
# Begin Source File

SOURCE=.\DialogEditTileSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogEditTileSet.h
# End Source File
# Begin Source File

SOURCE=.\DialogTileSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTileSet.h
# End Source File
# Begin Source File

SOURCE=.\TileSet.cpp
# End Source File
# Begin Source File

SOURCE=.\TileSet.h
# End Source File
# Begin Source File

SOURCE=.\TileSetApplyer.cpp
# End Source File
# Begin Source File

SOURCE=.\TileSetApplyer.h
# End Source File
# Begin Source File

SOURCE=.\TileSetEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\TileSetEntry.h
# End Source File
# Begin Source File

SOURCE=.\TileSetPiece.cpp
# End Source File
# Begin Source File

SOURCE=.\TileSetPiece.h
# End Source File
# Begin Source File

SOURCE=.\TileSetView.cpp
# End Source File
# Begin Source File

SOURCE=.\TileSetView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
