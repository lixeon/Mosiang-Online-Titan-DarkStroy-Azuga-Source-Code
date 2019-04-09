# Microsoft Developer Studio Project File - Name="MonitoringServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MonitoringServer - Win32 DebugForRMTool
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MonitoringServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MonitoringServer.mak" CFG="MonitoringServer - Win32 DebugForRMTool"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MonitoringServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MonitoringServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MonitoringServer - Win32 DebugForRMTool" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MonitoringServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Obj\MonitoringServer\Release"
# PROP Intermediate_Dir "..\Obj\MonitoringServer\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\[CC]Header" /I "..\[Lib]YHLibrary" /I "..\[CC]ServerModule" /I ".\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\[Lib]YHLibrary\yhlibrary.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"..\Output\MAS\MonitoringServer.exe"

!ELSEIF  "$(CFG)" == "MonitoringServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\MonitoringServer\Debug"
# PROP Intermediate_Dir "..\Obj\MonitoringServer\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\yhlibrary" /I "..\[CC]Header" /I "..\[Lib]YHLibrary" /I "..\[CC]ServerModule" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MONITORINGSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\[Lib]YHLibrary\yhlibrary.lib Ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\SWorking\MonitoringServer.exe" /pdbtype:sept /libpath:"..\yhlibrary"

!ELSEIF  "$(CFG)" == "MonitoringServer - Win32 DebugForRMTool"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MonitoringServer___Win32_DebugForRMTool"
# PROP BASE Intermediate_Dir "MonitoringServer___Win32_DebugForRMTool"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\RMToolServer\Debug"
# PROP Intermediate_Dir "..\Obj\RMToolServer\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\yhlibrary" /I "..\[CC]Header" /I "..\[Lib]YHLibrary" /I "..\[CC]ServerModule" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MONITORINGSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\yhlibrary" /I "..\[CC]Header" /I "..\[Lib]YHLibrary" /I "..\[CC]ServerModule" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_MONITORINGSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\[Lib]YHLibrary\yhlibrary.lib Ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\SWorking\MonitoringServer.exe" /pdbtype:sept /libpath:"..\yhlibrary"
# ADD LINK32 ..\[Lib]YHLibrary\yhlibrary.lib Ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\SWorking\RMToolServer.exe" /pdbtype:sept /libpath:"..\yhlibrary"

!ENDIF 

# Begin Target

# Name "MonitoringServer - Win32 Release"
# Name "MonitoringServer - Win32 Debug"
# Name "MonitoringServer - Win32 DebugForRMTool"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MonitoringServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitoringServer.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\MonitoringServer.ico
# End Source File
# Begin Source File

SOURCE=.\small.ico
# End Source File
# End Group
# Begin Group "module"

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

SOURCE="..\[CC]ServerModule\network_guid.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\ServerListManager.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\ServerListManager.h"
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
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CMSDBMsgParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CMSDBMsgParser.h
# End Source File
# Begin Source File

SOURCE=.\MASManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MASManager.h
# End Source File
# Begin Source File

SOURCE=.\MonitoringServer.h
# End Source File
# Begin Source File

SOURCE=.\MonitorNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorNetwork.h
# End Source File
# Begin Source File

SOURCE=.\MonitorServerTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorServerTable.h
# End Source File
# Begin Source File

SOURCE=.\MonitorUserTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorUserTable.h
# End Source File
# Begin Source File

SOURCE=.\MSDBMsgID.h
# End Source File
# Begin Source File

SOURCE=.\MSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MSManager.h
# End Source File
# Begin Source File

SOURCE=.\MSSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\MSSystem.h
# End Source File
# Begin Source File

SOURCE=.\UserIPCheckMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UserIPCheckMgr.h
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "TagetList"

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
# End Group
# Begin Source File

SOURCE=.\MHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MHFile.h
# End Source File
# Begin Source File

SOURCE=.\MSDefine.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
