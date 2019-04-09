#ifndef _MHAUTOPATCHDEFINES_H_
#define _MHAUTOPATCHDEFINES_H_

#pragma once

#include "Interface_RainFTP.h"
#include "resource.h"

#include <crtdbg.h>

#define _assert _ASSERT


#define FIND_DATA_MAX			500

#ifdef _FOR_CHINA_
#define FTP_USERID				""
#define FTP_PASSWORD			""
//#define FTP_USERID				"mxpatcher"
//#define FTP_PASSWORD			"uNk823hT"

#else
#define FTP_USERID				"dsnewuser1"
#define FTP_PASSWORD			"#RkfRmagkrp!!#"
#endif

#define FTP_INFO_FILE			"FTPInfo.mfi"

//	Window Message
#define SAFETY_RANGE			WM_USER + 5000


#define WM_RAIN_FTP_NOTIFY		SAFETY_RANGE
#define WM_RUNPROGRAM			SAFETY_RANGE+1
#define WM_JIERA				SAFETY_RANGE+2

enum
{
	FM_CONNECTED = 1,
	FM_GET_CURRENT_DIRECTORY,
	FM_GETFILELIST,
	FM_DOWNLOAD,
	FM_CONFIRM,
};

void StartPack3DData(HWND hDlg);
DWORD __stdcall PackingCallBackFunc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);

//
#define MH_VERSION_INFO_FILE				"MHVerInfo.ver"
#define MH_VERSION_INFO_FINE_NEW			"MHVerInfo_New.ver"

//
#define SELF_UPDATE_FILE					"selupdate.exe"
//

#define VERSION_HEADER_TEXT_LENGTH			4
#define VERSION_INFO_LENGTH					8
#define VERSION_TOTAL_LENGTH				VERSION_HEADER_TEXT_LENGTH + VERSION_INFO_LENGTH


#define	MAX_FILE_NUM		64
#define MAX_BUFFER_SIZE		MAX_PATH * MAX_FILE_NUM

//	Progress Bar Range 
#define PRO_RANGE_DN		255
#define PRO_RANGE_EX		255

//
#define TIME_INTERVAL		500
//	Global Text
const char* const	g_szMBErrCap		= "MHAutopatch (Error)";
const char* const	g_szMBInfoCap		= "MHAutopatch";

char* const			g_szMHpatchFile		= "MHPatch.mhp";

//	AS  Text
//char* const			g_sz_Initial		= " Initialized ";
//char* const			g_sz_Connect		= " Connect ... ";
//char* const			g_sz_DownLoad		= " DownLoad ... ";
//char* const			g_sz_Extract		= " Extract ... ";
//char* const			g_sz_Packing		= " DataPacking ... ";
//char* const			g_sz_Completed		= " Completed ";

//
extern WIN32_FIND_DATA*		pwfd;

struct FTP_FILE
{
	DWORD		dwBufferLength;
	DWORD		dwArraySize;

	char		szFileName[MAX_PATH];
	DWORD		dwFileSize;
};

struct FTP_CONTEXT
{
	DWORD				dwCurrentStatus;
	DWORD				dwConnectionResult;
	DWORD				dwGetListResult;
};

struct FTP_DOWN
{
	DWORD		dwReceived_CurrentTotal;
};




//	Global 
extern char MHFile[MAX_PATH];
extern char newverFilename[256];
extern char g_FTPID[256];
extern char g_FTPPW[256];
extern char* g_FTPIp;
extern WORD* g_FTPPort;

extern int g_nFTPCount;
extern int g_nCurFTPNum;

extern HANDLE g_hSession;

extern int					iR;
extern HMODULE				g_hDllModule;
extern IRainFTP*			g_pFtp;

extern char g_ExecutableFileName[MAX_PATH];

extern HMODULE g_hExecutiveHandle;
extern I4DyuchiGXExecutive* g_pExecutive;



#endif