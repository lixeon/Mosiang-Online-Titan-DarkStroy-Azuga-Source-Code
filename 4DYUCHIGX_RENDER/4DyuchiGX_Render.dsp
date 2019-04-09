# Microsoft Developer Studio Project File - Name="4DyuchiGX_Render" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=4DyuchiGX_Render - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGX_Render.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "4DyuchiGX_Render.mak" CFG="4DyuchiGX_Render - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "4DyuchiGX_Render - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "4DyuchiGX_Render - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4DyuchiGX/4DyuchiGX_Render", DGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "4DyuchiGX_Render - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGX_RENDER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGX_RENDER_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib d3d8.lib /nologo /base:"0x64000000" /dll /incremental:yes /pdb:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXRenderer.pdb" /map:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXRenderer.map" /debug /machine:I386 /nodefaultlib:"LIBMMT.LIB" /out:"../../4DyuchiDLL/4DyuchiGX_Release/4DyuchiGXRenderer.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "4DyuchiGX_Render - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGX_RENDER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DYUCHIGX_RENDER_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib d3d8.lib /nologo /base:"0x64000000" /dll /pdb:"../../4DyuchiDLL/4DyuchiGX_Debug/4DyuchiGXRenderer.pdb" /map:"../../4DyuchiDLL/4DyuchiGX_Debug/4DyuchiGXRenderer.map" /debug /machine:I386 /nodefaultlib:"LIBMMT.LIB" /out:"../../4dyuchiDLL/4DyuchiGX_debug/4DyuchiGXRenderer.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "4DyuchiGX_Render - Win32 Release"
# Name "4DyuchiGX_Render - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\4DyuchiGX_Render.def
# End Source File
# Begin Source File

SOURCE=.\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\factory.cpp
# End Source File
# Begin Source File

SOURCE=.\hashsearch.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Render Object"

# PROP Default_Filter ""
# Begin Group "LightMap Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LMMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\LMMeshObject.h
# End Source File
# End Group
# Begin Group "Font"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\FontCache.cpp
# End Source File
# Begin Source File

SOURCE=.\FontCache.h
# End Source File
# Begin Source File

SOURCE=.\FontHash.cpp
# End Source File
# Begin Source File

SOURCE=.\FontHash.h
# End Source File
# Begin Source File

SOURCE=.\FontObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FontObject.h
# End Source File
# End Group
# Begin Group "VertexLight Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PLMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PLMeshObject.h
# End Source File
# Begin Source File

SOURCE=.\VLMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\VLMeshObject.h
# End Source File
# Begin Source File

SOURCE=.\VLSMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\VLSMeshObject.h
# End Source File
# End Group
# Begin Group "Base Element"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base_facegroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Base_facegroup.h
# End Source File
# Begin Source File

SOURCE=.\BaseFontObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseFontObject.h
# End Source File
# Begin Source File

SOURCE=.\BaseMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseMeshObject.h
# End Source File
# Begin Source File

SOURCE=.\CTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\CTexture.h
# End Source File
# Begin Source File

SOURCE=.\d3dtexture.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dtexture.h
# End Source File
# Begin Source File

SOURCE=.\IndexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\IResource.h
# End Source File
# Begin Source File

SOURCE=.\LMFaceGRoup.cpp
# End Source File
# Begin Source File

SOURCE=.\LMFaceGroup.h
# End Source File
# Begin Source File

SOURCE=.\RenderTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderTexture.h
# End Source File
# Begin Source File

SOURCE=.\VertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\VLFaceGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\VLFaceGroup.h
# End Source File
# End Group
# Begin Group "HeightField"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HFieldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HFieldManager.h
# End Source File
# Begin Source File

SOURCE=.\HFieldObject.cpp
# End Source File
# Begin Source File

SOURCE=.\HFieldObject.h
# End Source File
# End Group
# Begin Group "sprite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3DSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\3DSprite.h
# End Source File
# End Group
# Begin Group "BillBoard"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BiiBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\BillBoard.h
# End Source File
# End Group
# Begin Group "ImmMeshObject"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImmMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ImmMeshObject.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\circle.cpp
# End Source File
# Begin Source File

SOURCE=.\circle.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\d3d_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\d3d_helper.h
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGXGFunc\global.h
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\render_type.h
# End Source File
# Begin Source File

SOURCE=.\renderer_typedef.h
# End Source File
# Begin Source File

SOURCE=.\TIF.CPP
# End Source File
# Begin Source File

SOURCE=.\TIF.H
# End Source File
# Begin Source File

SOURCE=..\4DyuchiGRX_common\typedef.h
# End Source File
# Begin Source File

SOURCE=.\VariableBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\VariableBuffer.h
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
# Begin Group "device"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoD3DDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\CoD3DDevice.h
# End Source File
# Begin Source File

SOURCE=.\D3DResourceBufferHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DResourceBufferHeap.h
# End Source File
# Begin Source File

SOURCE=.\D3DResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DResourceManager.h
# End Source File
# Begin Source File

SOURCE=.\IBCache.cpp
# End Source File
# Begin Source File

SOURCE=.\IBCache.h
# End Source File
# Begin Source File

SOURCE=.\IBHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\IBHeap.h
# End Source File
# Begin Source File

SOURCE=.\IBManager.h
# End Source File
# Begin Source File

SOURCE=.\IResourcePool.h
# End Source File
# Begin Source File

SOURCE=.\MaterialManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialManager.h
# End Source File
# Begin Source File

SOURCE=.\RenderTexturePool.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderTexturePool.h
# End Source File
# Begin Source File

SOURCE=.\StaticD3DResourcePool.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticD3DResourcePool.h
# End Source File
# Begin Source File

SOURCE=.\StaticIBPool.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticIBPool.h
# End Source File
# Begin Source File

SOURCE=.\StaticVBPool.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticVBPool.h
# End Source File
# Begin Source File

SOURCE=.\texture_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\texture_manager.h
# End Source File
# Begin Source File

SOURCE=.\VBCache.cpp
# End Source File
# Begin Source File

SOURCE=.\VBCache.h
# End Source File
# Begin Source File

SOURCE=.\VBHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\VBHeap.h
# End Source File
# Begin Source File

SOURCE=.\VBManager.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\factory.h
# End Source File
# Begin Source File

SOURCE=.\global_variable.h
# End Source File
# Begin Source File

SOURCE=.\hashsearch.h
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

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "RenderQueue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RenderQ.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderQ.h
# End Source File
# Begin Source File

SOURCE=.\RenderTriBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderTriBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SpriteBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteBuffer.h
# End Source File
# Begin Source File

SOURCE=.\TextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBuffer.h
# End Source File
# End Group
# Begin Group "Shaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\diff.psh
# End Source File
# Begin Source File

SOURCE=.\diffDot3.psh
# End Source File
# Begin Source File

SOURCE=.\diffDot3Pix.vsh
# End Source File
# Begin Source File

SOURCE=.\diffDot3Reflect.psh
# End Source File
# Begin Source File

SOURCE=.\diffOpasity.psh
# End Source File
# End Group
# Begin Group "ProjectionLight"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ProjectionLight.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectionLight.h
# End Source File
# End Group
# End Target
# End Project
