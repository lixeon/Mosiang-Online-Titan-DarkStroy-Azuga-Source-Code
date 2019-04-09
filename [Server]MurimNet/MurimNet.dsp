# Microsoft Developer Studio Project File - Name="MurimNet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MurimNet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MurimNet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MurimNet.mak" CFG="MurimNet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MurimNet - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MurimNet - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MurimNet - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Obj\Murimnet\Release"
# PROP Intermediate_Dir "..\Obj\Murimnet\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\4DyuchiGXGFunc\SS3DGFunc.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\[Lib]yhlibrary\yhlibrary.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MurimNet - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\Murimnet\Debug"
# PROP Intermediate_Dir "..\Obj\Murimnet\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I "..\[CC]ServerModule" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MAPSERVER_" /D "__MAPSERVER__" /D "_USINGTOOL_" /D "_MURIMNET_" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\4DyuchiGXGFunc\SS3DGFunc.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\[Lib]yhlibrary\yhlibrary.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\YHLibrary"

!ENDIF 

# Begin Target

# Name "MurimNet - Win32 Release"
# Name "MurimNet - Win32 Debug"
# Begin Group "MNPlayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MNPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\MNPlayer.h
# End Source File
# Begin Source File

SOURCE=.\MNPlayerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MNPlayerManager.h
# End Source File
# End Group
# Begin Group "StdAfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "ShareFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ShareDefines.h
# End Source File
# Begin Source File

SOURCE=.\ShareStruct.h
# End Source File
# End Group
# Begin Group "Channel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Channel.cpp
# End Source File
# Begin Source File

SOURCE=.\Channel.h
# End Source File
# Begin Source File

SOURCE=.\ChannelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelManager.h
# End Source File
# End Group
# Begin Group "PlayRoom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PlayRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayRoom.h
# End Source File
# Begin Source File

SOURCE=.\PlayRoomManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayRoomManager.h
# End Source File
# End Group
# Begin Group "MurimNetSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonNetworkMsgParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonNetworkMsgParser.h
# End Source File
# Begin Source File

SOURCE=.\Crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\Crypt.h
# End Source File
# Begin Source File

SOURCE=.\MHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MHFile.h
# End Source File
# Begin Source File

SOURCE=.\MHTimeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MHTimeManager.h
# End Source File
# Begin Source File

SOURCE=.\MNDBMsgParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MNDBMsgParser.h
# End Source File
# Begin Source File

SOURCE=.\MNNetworkMsgParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MNNetworkMsgParser.h
# End Source File
# Begin Source File

SOURCE=.\MurimNetSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MurimNetSystem.h
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSystem.h
# End Source File
# Begin Source File

SOURCE=.\ServerTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerTable.h
# End Source File
# End Group
# Begin Group "MNServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MNServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MNServer.h
# End Source File
# Begin Source File

SOURCE=.\MNServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MNServerManager.h
# End Source File
# End Group
# Begin Group "Front"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Front.cpp
# End Source File
# Begin Source File

SOURCE=.\Front.h
# End Source File
# End Group
# Begin Group "DefineFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MNDefines.h
# End Source File
# End Group
# Begin Group "common"

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

SOURCE="..\[CC]Header\GameResourceManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\GameResourceManager.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\GameResourceStruct.h"
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
# Begin Group "module"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\[CC]ServerModule\BootManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\BootManager.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\CommonDBMsgParser.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\CommonDBMsgParser.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\Console.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\Console.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\console_type.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\DataBase.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\DataBase.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\DB.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\DBThreadInterface.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\define.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\iconsole.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\iconsole_guid.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\inetwork.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\net_define.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\Network.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\Network.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\network_guid.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\TConsole_Def.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\TConsole_Guids.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\TConsole_Interface.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\typedef.h"
# End Source File
# End Group
# End Target
# End Project
