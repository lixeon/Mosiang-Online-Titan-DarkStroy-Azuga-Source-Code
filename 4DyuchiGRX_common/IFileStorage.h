#pragma once

#include "FileStorage_typedef.h"
#include "IFileStorage_GUID.h"
#include "typedef.h"

interface I4DyuchiFileStorage : public IUnknown
{	
	virtual 	BOOL	__stdcall	Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod) = 0;
	virtual		void*	__stdcall	MapPackFile(char* szPackFileName) = 0;
	virtual		void	__stdcall	UnmapPackFile(void* pPackFileHandle) = 0;
	
	virtual		DWORD	__stdcall	GetFileNum(void* pPackFileHandle) = 0;
	virtual 	DWORD	__stdcall	CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum) = 0;
	virtual		void	__stdcall	DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList) = 0;
	virtual 	BOOL	__stdcall	IsExistInFileStorage(char* szFileName) = 0;

	virtual		BOOL	__stdcall	LockPackFile(void* pPackFileHandle,DWORD dwFlag) = 0;
	virtual		BOOL	__stdcall	InsertFileToPackFile(void* pPackFileHandle,char* szFileName) = 0;
	virtual		BOOL	__stdcall	DeleteFileFromPackFile(char* szFileName) = 0;
	virtual		BOOL	__stdcall	UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc) = 0;

	virtual 	BOOL	__stdcall	ExtractFile(char* szFileName) = 0;
	virtual		BOOL	__stdcall	ExtractAllFiles() = 0;
	virtual		DWORD	__stdcall	ExtractAllFilesFromPackFile(void* pPackFileHandle) = 0;

	virtual		void*	__stdcall	FSOpenFile(char* szFileName,DWORD dwAccessMode) = 0;
	virtual		int		__stdcall	FSScanf(void* pFP,char* szFormat, ...) = 0;
	virtual		DWORD	__stdcall	FSRead(void* pFP,void* pDest,DWORD dwLen) = 0;
	virtual		DWORD	__stdcall	FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase) = 0;
	virtual		BOOL	__stdcall	FSCloseFile(void* pFP) = 0;

	virtual		BOOL	__stdcall	GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo) = 0;

	virtual		BOOL	__stdcall	BeginLogging(char* szFileName,DWORD dwFlag) = 0;
	virtual		BOOL	__stdcall	EndLogging() = 0;
	
};
