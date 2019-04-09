# Microsoft Developer Studio Project File - Name="DS_RMTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DS_RMTool - Win32 DebugForChina
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DS_RMTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DS_RMTool.mak" CFG="DS_RMTool - Win32 DebugForChina"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DS_RMTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DS_RMTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "DS_RMTool - Win32 DebugForChina" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DS_RMTool", AOGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DS_RMTool - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 yhlibrary.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "DS_RMTool - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\DS_RMTool\Debug"
# PROP Intermediate_Dir "..\Obj\DS_RMTool\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\[Lib]YHLibrary\YHLibrary.lib ws2_32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\DS_RMToolWorking\DS_RMTool.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "DS_RMTool - Win32 DebugForChina"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DS_RMTool___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "DS_RMTool___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Obj\DS_RMTool\DebugForChina"
# PROP Intermediate_Dir "..\Obj\DS_RMTool\DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_CHINA" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\[Lib]YHLibrary\YHLibrary.lib ws2_32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\DS_RMToolWorking\DS_RMTool.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 ..\[Lib]YHLibrary\YHLibrary.lib ws2_32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\DS_RMToolWorking\RMToolForChina.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "DS_RMTool - Win32 Release"
# Name "DS_RMTool - Win32 Debug"
# Name "DS_RMTool - Win32 DebugForChina"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DS_RMTool.ico
# End Source File
# Begin Source File

SOURCE=.\DS_RMTool.rc
# End Source File
# Begin Source File

SOURCE=.\res\DS_RMTool.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DS_RMToolDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Main.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\rmtoolba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Group "Temp Account"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DS_RMToolDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DS_RMToolDoc.h
# End Source File
# Begin Source File

SOURCE=.\DS_RMToolView.cpp
# End Source File
# Begin Source File

SOURCE=.\DS_RMToolView.h
# End Source File
# End Group
# Begin Group "Temp Guild"

# PROP Default_Filter ""
# Begin Group "MunpaPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MunpaInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MunpaInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MunpaDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MunpaDoc.h
# End Source File
# Begin Source File

SOURCE=.\MunpaFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MunpaFrm.h
# End Source File
# Begin Source File

SOURCE=.\MunpaView.cpp
# End Source File
# Begin Source File

SOURCE=.\MunpaView.h
# End Source File
# End Group
# Begin Group "Temp Admin"

# PROP Default_Filter ""
# Begin Group "AdminPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdminInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminInfo.h
# End Source File
# Begin Source File

SOURCE=.\IpInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\IpInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdminDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminDoc.h
# End Source File
# Begin Source File

SOURCE=.\AdminFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminFrm.h
# End Source File
# Begin Source File

SOURCE=.\AdminView.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminView.h
# End Source File
# End Group
# Begin Group "ID&Pass Dlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Operater.cpp
# End Source File
# Begin Source File

SOURCE=.\Operater.h
# End Source File
# End Group
# Begin Group "Temp CharacterInfo"

# PROP Default_Filter ""
# Begin Group "CharacterPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CharacterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterInfo.h
# End Source File
# Begin Source File

SOURCE=.\QuestInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubQuestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubQuestDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CharacterDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterDoc.h
# End Source File
# Begin Source File

SOURCE=.\CharacterFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterFrm.h
# End Source File
# Begin Source File

SOURCE=.\CharacterView.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterView.h
# End Source File
# End Group
# Begin Group "My Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDoc.h
# End Source File
# Begin Source File

SOURCE=.\CommonRMControl.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonRMControl.h
# End Source File
# Begin Source File

SOURCE=.\LDPropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\LDPropSheet.h
# End Source File
# Begin Source File

SOURCE=.\PropertyList.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyList.h
# End Source File
# Begin Source File

SOURCE=.\RMListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RMListCtrl.h
# End Source File
# End Group
# Begin Group "EditDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AbilityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AbilityDlg.h
# End Source File
# Begin Source File

SOURCE=.\ItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\MugongDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MugongDlg.h
# End Source File
# End Group
# Begin Group "Temp GameLog"

# PROP Default_Filter ""
# Begin Group "LogPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LogEtcPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LogEtcPage.h
# End Source File
# Begin Source File

SOURCE=.\LogItemMoneyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LogItemMoneyPage.h
# End Source File
# Begin Source File

SOURCE=.\LogTotalPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LogTotalPage.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewerOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewerOptionDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\LogDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LogDoc.h
# End Source File
# Begin Source File

SOURCE=.\LogFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFrm.h
# End Source File
# Begin Source File

SOURCE=.\LogView.cpp
# End Source File
# Begin Source File

SOURCE=.\LogView.h
# End Source File
# End Group
# Begin Group "Temp OperLog"

# PROP Default_Filter ""
# Begin Group "LogOperPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LogOperPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOperPage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\LogOperDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOperDoc.h
# End Source File
# Begin Source File

SOURCE=.\LogOperFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOperFrm.h
# End Source File
# Begin Source File

SOURCE=.\LogOperView.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOperView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DS_RMTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DS_RMTool.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "Ability"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\[CC]Ability\AbilityCommonHeader.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Ability\AbilityCommonHeader.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Ability\AbilityInfo.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Ability\AbilityInfo.h"
# End Source File
# End Group
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
# Begin Group "Quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\[CC]Quest\QuestString.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Quest\QuestString.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\[CC]Header\AddableInfo.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonCalcFunc.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\CommonCalcFunc.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\CommonDBMsgParser.cpp"
# End Source File
# Begin Source File

SOURCE="..\[CC]ServerModule\CommonDBMsgParser.h"
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

SOURCE="..\[CC]Header\GameResourceStruct.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\Protocol.h"
# End Source File
# Begin Source File

SOURCE="..\[CC]Header\ServerGameStruct.h"
# End Source File
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonToolDefine.h
# End Source File
# Begin Source File

SOURCE=.\MASInfoManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MASInfoManager.h
# End Source File
# Begin Source File

SOURCE=.\MHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MHFile.h
# End Source File
# Begin Source File

SOURCE=.\ResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=.\RMNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\RMNetwork.h
# End Source File
# End Group
# Begin Group "ChangeValue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChangeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeManager.h
# End Source File
# Begin Source File

SOURCE=.\ChangeValue.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeValue.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ItemRecoverDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemRecoverDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
