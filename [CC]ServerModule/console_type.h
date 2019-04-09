#pragma once

typedef void (*VOIDFUNC)(void);

enum LOG_PRIORITY_LEVEL
{
	LOG_FATAL		=	0,
	LOG_IMPORTANT	=	1,
	LOG_NORMAL		=	2,
	LOG_IGNORE		=	3,
	LOG_DEBUG		=	4,
	
};

struct CUSTOM_BUTTON
{
	VOIDFUNC	pButtonProc;
	char		szButtonName[32];
};

struct DISPLAY_INFO
{
	DWORD				dwMaxAccessibleThreadNum;
	DWORD				dwMaxStringBufferNum;
	DWORD				dwMinMaintainBufferNum;
	DWORD				dwDisplayRefreshRate;
	LOG_PRIORITY_LEVEL	dwFilteredLevel;

};
struct WRITE_FILE_INFO
{
	DWORD				dwMaxAccessibleThreadNum;
	DWORD				dwLimitedBufferSizeToWriteFile;
	LOG_PRIORITY_LEVEL	dwFilteredLevel;

};

struct DESC_CONSOLE
{
	void				(*OnCommandFunc)(char* szCommand);
	DWORD				dwCustomButtonNum;
	CUSTOM_BUTTON*		pCustomButton;				
	char*				szFileName;
	
	DISPLAY_INFO*		pDisplayInfo;
	WRITE_FILE_INFO*	pWriteFileInfo;

};
