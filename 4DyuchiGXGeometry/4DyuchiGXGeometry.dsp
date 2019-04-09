# Microsoft Developer Studio Project File - Name="4DyuchiGXGeometry" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=4DyuchiGXGeometry - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXGeometry.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGXGeometry.mak" CFG="4DyuchiGXGeometry - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4DyuchiGXGeometry - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGXGeometry - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/4DyuchiGXGeometry", NKAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "4DyuchiGXGeometry - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXGEOMETRY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Op /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXGEOMETRY_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x63000000" /dll /pdb:"d:\4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXGeometry.pdb" /map:"d:\4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXGeometry.map" /debug /machine:I386 /out:"D:/4dyuchiDLL/4DyuchiGX_release/4DyuchiGXGeometry.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "4DyuchiGXGeometry - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXGEOMETRY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGXGEOMETRY_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x63000000" /dll /pdb:"D:\4dyuchiDLL/4DyuchiGX_Debug/4DyuchiGXGeometry.pdb" /map:"D:\4dyuchiDLL/4DyuchiGX_Debug/4DyuchiGXGeometry.map" /debug /machine:I386 /out:"D:/4dyuchiDLL/4DyuchiGX_Debug/4DyuchiGXGeometry.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "4DyuchiGXGeometry - Win32 Release"
# Name "4DyuchiGXGeometry - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4DyuchiGXGEOMETRY.def
# End Source File
# Begin Source File

SOURCE=.\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\factory.cpp
# End Source File
# Begin Source File

SOURCE=.\geometry_global.cpp
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

SOURCE=.\geometry_global.h
# End Source File
# Begin Source File

SOURCE=.\global_variable.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "BaseObjects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bone_object.cpp
# End Source File
# Begin Source File

SOURCE=.\bone_object.h
# End Source File
# Begin Source File

SOURCE=.\camera_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\camera_obj.h
# End Source File
# Begin Source File

SOURCE=.\face_group.cpp
# End Source File
# Begin Source File

SOURCE=.\face_group.h
# End Source File
# Begin Source File

SOURCE=.\geom_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\geom_obj.h
# End Source File
# Begin Source File

SOURCE=.\light_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\light_obj.h
# End Source File
# Begin Source File

SOURCE=.\LightTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\LightTexture.h
# End Source File
# Begin Source File

SOURCE=.\material.cpp
# End Source File
# Begin Source File

SOURCE=.\material.h
# End Source File
# Begin Source File

SOURCE=.\MaterialList.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialList.h
# End Source File
# Begin Source File

SOURCE=.\object.cpp
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\Physique.cpp
# End Source File
# Begin Source File

SOURCE=.\Physique.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bintree.cpp
# End Source File
# Begin Source File

SOURCE=.\bintree.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\global.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\math.inl
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\mesh_flag.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\motion_flag.h
# End Source File
# Begin Source File

SOURCE=.\RenderType.h
# End Source File
# Begin Source File

SOURCE=.\SimpleMemoryPool.cpp
# End Source File
# Begin Source File

SOURCE=.\SImpleMemoryPool.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# End Group
# Begin Group "GeometryManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\CoGeometry.h
# End Source File
# Begin Source File

SOURCE=.\ModelRefManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelRefManager.h
# End Source File
# End Group
# Begin Group "COM Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DyuchiGRX_common\FileStorage_typedef.h
# End Source File
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

SOURCE=..\4DyuchiGRX_common\IFileStorage_GUID.h
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
# Begin Group "SkyMesh"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SkyMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyMesh.h
# End Source File
# End Group
# Begin Group "base component"

# PROP Default_Filter ""
# Begin Group "StaticModel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoStaticModel.cpp
# End Source File
# Begin Source File

SOURCE=.\CoStaticModel.h
# End Source File
# End Group
# Begin Group "HeightField"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HeightField.cpp
# End Source File
# Begin Source File

SOURCE=.\HeightField.h
# End Source File
# Begin Source File

SOURCE=.\HFieldObject.cpp
# End Source File
# Begin Source File

SOURCE=.\HFieldObject.h
# End Source File
# Begin Source File

SOURCE=.\HFieldTools.cpp
# End Source File
# Begin Source File

SOURCE=.\HFieldTools.h
# End Source File
# Begin Source File

SOURCE=.\MeshQuadTree.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshQuadTree.h
# End Source File
# Begin Source File

SOURCE=.\TileObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TileObject.h
# End Source File
# End Group
# Begin Group "motion"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MOTION.CPP
# End Source File
# Begin Source File

SOURCE=.\MOTION.H
# End Source File
# Begin Source File

SOURCE=.\motion_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\motion_obj.h
# End Source File
# End Group
# Begin Group "model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\model.cpp
# End Source File
# Begin Source File

SOURCE=.\model.h
# End Source File
# End Group
# Begin Group "MaterialLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MaterialLib.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialLib.h
# End Source File
# End Group
# Begin Group "CollisionStaticModel"

# PROP Default_Filter ""
# Begin Group "Ãæµ¹"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CheckClock.cpp
# End Source File
# Begin Source File

SOURCE=..\4DYUCHIGRX_MYSELF97\CheckClock.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\CollisionTestMovingEllipsoidMeetTriangle.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\CollisionTestMovingEllipsoidMeetTriangle.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\CollisionTestMovingSphereMeetTriangle.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\CollisionTest\CollisionTestMovingSphereMeetTriangle.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\Myself97Common.cpp
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_myself97\Myself97Common.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CollisionMeshObjectTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CollisionMeshObjectTree.h
# End Source File
# Begin Source File

SOURCE=.\CollisionStaticModel.cpp
# End Source File
# Begin Source File

SOURCE=.\CollisionStaticModel.h
# End Source File
# Begin Source File

SOURCE=.\colmeshobj.cpp
# End Source File
# Begin Source File

SOURCE=.\colmeshobj.h
# End Source File
# Begin Source File

SOURCE=.\SearchGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchGrid.h
# End Source File
# Begin Source File

SOURCE=.\SearchGridTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchGridTree.h
# End Source File
# End Group
# End Group
# End Target
# End Project
