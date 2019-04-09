#pragma once

#include "./ZipArchive/ZipArchive.h"

/////////////////////////////////////////////////////
//
//	COMPLETION_ERROR.
//
/////////////////////////////////////////////////////

enum COMPLETION_ERROR
	{
		CE_INITIALIZED					= 0,			//	No Result Occur.

		CE_SUCCEED						,				//	ok. request completed.
		CE_EXCUTE_PENDING				,				//	Request is Pending.
		CE_DOWNLOAD_PROGRESS			,

		CE_ERROR						,				//	Error, but Session is valid.

		CE_SHUTDOWN_WININET				,				//	Shutdown by wininet internal.
		CE_SHUTDOWN_TIMEOUT				,				//	Shutdown by Timeout.
		CE_SHUTDOWN_USER				,				//	Shutdown by User.
		CE_SHUTDOWN_LOCAL
	};

/////////////////////////////////////////////////////
//
//	Internet Param.
//
/////////////////////////////////////////////////////

struct InternetParam
{
	DWORD		dwAccessType;
    char*		pszProxyName;
    char*		pszProxyBypass;
};


/////////////////////////////////////////////////////
//
//	Connect_data.
//
/////////////////////////////////////////////////////

struct Connect_data
{
	PVOID				pvExcuteContext;
	COMPLETION_ERROR	CompletionError;

	char				szServerName	[ MAX_PATH];
	WORD				wServerPort;

	char				szUserName		[ MAX_PATH];
	char				szUserPassword	[ MAX_PATH];

	BOOL				bPassiveMode;
	DWORD				dwTimeOut;
};


/////////////////////////////////////////////////////
//
//	Directory_data.
//
/////////////////////////////////////////////////////

struct Directory_data
{
	PVOID				pvExcuteContext;
	COMPLETION_ERROR	CompletionError;

	//	OUT
	char				szCurrentDirectory[ MAX_PATH];
	//	dwLength value is ignored in SetCurrentDirectory.
	DWORD				dwLength;
};


/////////////////////////////////////////////////////
//
//	List_data.
//
/////////////////////////////////////////////////////

struct List_data
{
	PVOID				pvExcuteContext;
	COMPLETION_ERROR	CompletionError;

	char*				pszSearchFile;

	WIN32_FIND_DATA*	pFile;
	DWORD				dwFileMax;

	//	OUT
	DWORD				dwFileNum;
};


/////////////////////////////////////////////////////
//
//	File_data.
//
/////////////////////////////////////////////////////

struct File_data
{
	PVOID				pvExcuteContext;
	COMPLETION_ERROR	CompletionError;
	
	char				szRemoteFile	[ MAX_PATH];
	char				szLocalFile		[ MAX_PATH];
	BOOL				bResume;

	//	OUT
	DWORD				dwTransferBytes;
};

