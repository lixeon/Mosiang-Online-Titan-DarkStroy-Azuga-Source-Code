# Microsoft Developer Studio Project File - Name="ServerTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ServerTool - Win32 Master
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ServerTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerTool.mak" CFG="ServerTool - Win32 Master"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ServerTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ServerTool - Win32 DebugForChina" (based on "Win32 (x86) Application")
!MESSAGE "ServerTool - Win32 Master" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ServerTool - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Obj\ServerTool\Release"
# PROP Intermediate_Dir "..\Obj\ServerTool\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ServerTool - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\ServerTool\Debug"
# PROP Intermediate_Dir "..\Obj\ServerTool\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\ServerToolWorking/ServerTool.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ServerTool - Win32 DebugForChina"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ServerTool___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "ServerTool___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\ServerTool\DebugForChina"
# PROP Intermediate_Dir "..\Obj\ServerTool\DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINA" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\ServerToolWorking/ServerTool.exe" /pdbtype:sept
# ADD LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\ServerToolWorking/ServerToolForChina.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "ServerTool - Win32 Master"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ServerTool___Win32_Master"
# PROP BASE Intermediate_Dir "ServerTool___Win32_Master"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\ServerTool\Master"
# PROP Intermediate_Dir "..\Obj\ServerTool\Master"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MASTER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\ServerToolWorking/ServerTool.exe" /pdbtype:sept
# ADD LINK32 Ws2_32.lib ..\[Lib]YHLibrary\YHLibrary.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\ServerToolWorking/ServerToolMaster.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ServerTool - Win32 Release"
# Name "ServerTool - Win32 Debug"
# Name "ServerTool - Win32 DebugForChina"
# Name "ServerTool - Win32 Master"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Dark.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Gray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Green.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\ServerTool.ico
# End Source File
# Begin Source File

SOURCE=.\ServerTool.rc
# End Source File
# Begin Source File

SOURCE=.\res\ServerToolDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\State.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\[CC]Header\AddableInfo.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\ClientGameDefine.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\ClientGameStruct.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonCalcFunc.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonCalcFunc.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonDefine.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonGameDefine.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonGameFunc.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonGameFunc.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonGameStruct.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonStruct.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonStructMS.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\Protocol.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\ServerGameDefine.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\ServerGameStruct.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\vector.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\vector.h"
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileBin.cpp
# End Source File
# Begin Source File

SOURCE=.\FileBin.h
# End Source File
# Begin Source File

SOURCE=.\mhfile.cpp
# End Source File
# Begin Source File

SOURCE=.\mhfile.h
# End Source File
# Begin Source File

SOURCE=.\mhnetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\mhnetwork.h
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MapNameManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MapNameManager.h
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\ServerListManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\ServerListManager.h"
# End Source File
# Begin Source File

SOURCE=.\ServerSet.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSet.h
# End Source File
# Begin Source File

SOURCE=.\ServerSetMng.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSetMng.h
# End Source File
# Begin Source File

SOURCE=.\ServerToolCommon.h
# End Source File
# End Group
# Begin Group "MFC"

# PROP Default_Filter ""
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsgBoxOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBoxOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\MSInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MSInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\NotifyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NotifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserCount.cpp
# End Source File
# Begin Source File

SOURCE=.\UserCount.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\NotifyMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\NotifyMessage.h
# End Source File
# Begin Source File

SOURCE=.\ServerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerTool.h
# End Source File
# Begin Source File

SOURCE=.\ServerToolDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerToolDoc.h
# End Source File
# Begin Source File

SOURCE=.\ServerToolView.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerToolView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
