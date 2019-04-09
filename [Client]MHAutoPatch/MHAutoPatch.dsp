# Microsoft Developer Studio Project File - Name="MHAutoPatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MHAutoPatch - Win32 Debug_Server
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MHAutoPatch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MHAutoPatch.mak" CFG="MHAutoPatch - Win32 Debug_Server"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MHAutoPatch - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MHAutoPatch - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MHAutoPatch - Win32 Debug_Server" (based on "Win32 (x86) Application")
!MESSAGE "MHAutoPatch - Win32 Release_Server" (based on "Win32 (x86) Application")
!MESSAGE "MHAutoPatch - Win32 Release_China" (based on "Win32 (x86) Application")
!MESSAGE "MHAutoPatch - Win32 Release_Server_China" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MHAutoPatch - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_FOR_CLIENT_" /d "_FOR_KOREA_CLIENT_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ZipArchive/ZipArchive_Release.lib Audio/SoundLib.lib /nologo /subsystem:windows /machine:I386 /out:"../CWorking/MHAutoPatch.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "MHAutoPatch - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_FOR_CLIENT_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ZipArchive/ZipArchive_Debug.lib Audio/SoundLib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../CWorking/MHAutoPatch.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "MHAutoPatch - Win32 Debug_Server"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MHAutoPatch___Win32_Debug_Server"
# PROP BASE Intermediate_Dir "MHAutoPatch___Win32_Debug_Server"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MHAutoPatch___Win32_Debug_Server"
# PROP Intermediate_Dir "MHAutoPatch___Win32_Debug_Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_SERVER_" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_SERVER_" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_FOR_SERVER_"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_FOR_SERVER_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ZipArchive/ZipArchive_Debug.lib Audio/SoundLib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug_Server/MHAutoPatch_Server.exe" /pdbtype:sept
# ADD LINK32 ZipArchive/ZipArchive_Debug.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug_Server/MHAutoPatch_Server.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "MHAutoPatch - Win32 Release_Server"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MHAutoPatch___Win32_Release_Server"
# PROP BASE Intermediate_Dir "MHAutoPatch___Win32_Release_Server"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MHAutoPatch___Win32_Release_Server"
# PROP Intermediate_Dir "MHAutoPatch___Win32_Release_Server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_FOR_CLIENT_"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_FOR_SERVER_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ZipArchive/ZipArchive_Release.lib Audio/SoundLib.lib /nologo /subsystem:windows /machine:I386 /out:"../CWorking/MHAutoPatch.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ZipArchive/ZipArchive_Release.lib /nologo /subsystem:windows /machine:I386 /out:"Release_Server/MHAutoPatch_Server.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "MHAutoPatch - Win32 Release_China"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MHAutoPatch___Win32_Release_China"
# PROP BASE Intermediate_Dir "MHAutoPatch___Win32_Release_China"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MHAutoPatch___Win32_Release_China"
# PROP Intermediate_Dir "MHAutoPatch___Win32_Release_China"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_FOR_CLIENT_"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_FOR_CLIENT_" /d "_FOR_CHINA_CLIENT_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ZipArchive/ZipArchive_Release.lib Audio/SoundLib.lib /nologo /subsystem:windows /machine:I386 /out:"../CWorking/MHAutoPatch.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ZipArchive/ZipArchive_Release.lib Audio/SoundLib.lib /nologo /subsystem:windows /machine:I386 /out:"../CWorking/MHAutoPatch.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "MHAutoPatch - Win32 Release_Server_China"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MHAutoPatch___Win32_Release_Server_China"
# PROP BASE Intermediate_Dir "MHAutoPatch___Win32_Release_Server_China"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MHAutoPatch___Win32_Release_Server_China"
# PROP Intermediate_Dir "MHAutoPatch___Win32_Release_Server_China"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_SERVER_" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_SERVER_" /D "_FOR_CLIENT_" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_FOR_SERVER_"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_FOR_SERVER_"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ZipArchive/ZipArchive_Release.lib /nologo /subsystem:windows /machine:I386 /out:"Release_Server/MHAutoPatch_Server.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 ZipArchive/ZipArchive_Release.lib /nologo /subsystem:windows /machine:I386 /out:"Release_Server/MHAutoPatch_Server.exe"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "MHAutoPatch - Win32 Release"
# Name "MHAutoPatch - Win32 Debug"
# Name "MHAutoPatch - Win32 Debug_Server"
# Name "MHAutoPatch - Win32 Release_Server"
# Name "MHAutoPatch - Win32 Release_China"
# Name "MHAutoPatch - Win32 Release_Server_China"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BGMManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlViewNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatch.rc
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BGMManager.h
# End Source File
# Begin Source File

SOURCE=.\HtmlViewNotice.h
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatch.h
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatchDefines.h
# End Source File
# Begin Source File

SOURCE=.\MHAutoPatchDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkinProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\SkinStatic.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MHAutoPatch.ico
# End Source File
# Begin Source File

SOURCE=.\res\MHAutoPatch.rc2
# End Source File
# Begin Source File

SOURCE=".\res\patch-06.bmp"
# End Source File
# End Group
# Begin Group "YHLibrary"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirectoryName.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectoryName.h
# End Source File
# Begin Source File

SOURCE=.\FileName.cpp
# End Source File
# Begin Source File

SOURCE=.\FileName.h
# End Source File
# Begin Source File

SOURCE=.\Strclass.cpp
# End Source File
# Begin Source File

SOURCE=.\Strclass.h
# End Source File
# Begin Source File

SOURCE=.\YHFileFind.cpp
# End Source File
# Begin Source File

SOURCE=.\YHFileFind.h
# End Source File
# End Group
# Begin Group "FTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Interface_RainFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\Interface_RainFTP.h
# End Source File
# Begin Source File

SOURCE=.\RainFTP_TypeDefine.h
# End Source File
# End Group
# Begin Group "Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Audio\SoundLib.h
# End Source File
# Begin Source File

SOURCE=.\Audio\SoundLib.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
