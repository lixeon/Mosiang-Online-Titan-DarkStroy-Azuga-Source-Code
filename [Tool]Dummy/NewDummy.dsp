# Microsoft Developer Studio Project File - Name="NewDummy" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NewDummy - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NewDummy.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NewDummy.mak" CFG="NewDummy - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NewDummy - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NewDummy - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NewDummy - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "NewDummy - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NewDummy___Win32_Debug"
# PROP BASE Intermediate_Dir "NewDummy___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NewDummy___Win32_Debug"
# PROP Intermediate_Dir "NewDummy___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\[Lib]YHLibrary" /I ".\\" /I "..\[CC]ServerModule" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /libpath:"..\YHLibrary"

!ENDIF 

# Begin Target

# Name "NewDummy - Win32 Release"
# Name "NewDummy - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ServerSystem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Module"

# PROP Default_Filter ""
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
# Begin Group "Dummy"

# PROP Default_Filter ""
# Begin Group "State"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharMake.cpp
# End Source File
# Begin Source File

SOURCE=.\CharMake.h
# End Source File
# Begin Source File

SOURCE=.\CharSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\CharSelect.h
# End Source File
# Begin Source File

SOURCE=.\GameEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEnd.h
# End Source File
# Begin Source File

SOURCE=.\GameIn.cpp
# End Source File
# Begin Source File

SOURCE=.\GameIn.h
# End Source File
# Begin Source File

SOURCE=.\GameLoading.cpp
# End Source File
# Begin Source File

SOURCE=.\GameLoading.h
# End Source File
# Begin Source File

SOURCE=.\GameState.cpp
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\MainTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\MainTitle.h
# End Source File
# Begin Source File

SOURCE=.\MapChange.cpp
# End Source File
# Begin Source File

SOURCE=.\MapChange.h
# End Source File
# Begin Source File

SOURCE=.\ServerConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerConnect.h
# End Source File
# Begin Source File

SOURCE=.\StateMng.cpp
# End Source File
# Begin Source File

SOURCE=.\StateMng.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DummyObjMng.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyObjMng.h
# End Source File
# Begin Source File

SOURCE=.\MHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MHFile.h
# End Source File
# Begin Source File

SOURCE=.\MoveMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveMng.h
# End Source File
# Begin Source File

SOURCE=.\ResourceMng.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceMng.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "TargetList"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetData.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetData.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetList.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetList.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetListIterator.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\TargetList\TargetListIterator.h"
# End Source File
# End Group
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
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DummyNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyNetwork.h
# End Source File
# End Group
# End Target
# End Project
