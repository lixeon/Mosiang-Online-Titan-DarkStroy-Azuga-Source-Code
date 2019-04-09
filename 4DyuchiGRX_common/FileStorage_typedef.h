#pragma once


#include "stdafx.h"

#define MAX_PACK_FILE_NUM	32

enum FILE_ACCESS_METHOD
{
	FILE_ACCESS_METHOD_ONLY_PACK	= 0,
	FILE_ACCESS_METHOD_FILE_OR_PACK	= 1,
	FILE_ACCESS_METHOD_ONLY_FILE	= 2
};

enum FSFILE_SEEK
{
	FSFILE_SEEK_SET	=	SEEK_SET,
	FSFILE_SEEK_CUR	=	SEEK_CUR,
	FSFILE_SEEK_END	=	SEEK_END
};

enum FSFILE_ACCESSMODE
{
	FSFILE_ACCESSMODE_BINARY	= 0x00000000,
	FSFILE_ACCESSMODE_TEXT		= 0x00000001
};

struct FSFILE_ATOM_INFO
{
	DWORD			dwTotalSize;
	DWORD			dwPositionInPackFile;
	DWORD			dwRealFileSize;
	char*			pszFileName;				// full path 파일명 
	DWORD			dwFileNameLen;			// full path 파일명 길이
	void*			pExtData;
};

struct FSPACK_FILE_INFO
{
	DWORD	dwVersion;
	DWORD	dwFileItemNum;
	DWORD	dwFlag;
	DWORD	dwFileNameLen;
	char	szFileName[_MAX_PATH];
};