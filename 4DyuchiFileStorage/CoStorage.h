#pragma once

#include "../4DyuchiGRX_Common/IFileStorage.h"
#include "typedef.h"
#include "../4DyuchiGXGFunc/global.h"

#include "PackFile.h"



class CoStorage : public I4DyuchiFileStorage
{
	DWORD				m_dwRefCount;
	BOOL				m_bEnableUsePackFile;
	VBHASH_HANDLE		m_pFileItemNameHash;
	VBHASH_HANDLE		m_pPackFileNameHash;
	STMPOOL_HANDLE		m_pFileDescPool;
	STMPOOL_HANDLE		m_pFileHandlePool;
	DWORD				m_dwClusterSize;

	DWORD				m_dwMaxFileNum;

	DWORD				m_dwMaxFileNameLen;
	DWORD				m_dwMaxInsertedFileNameLen;
	FILE_ACCESS_METHOD	m_fileAccessMethod;

	char				m_szLogFileName[_MAX_PATH];
	FILE*				m_fpLog;

	void				Cleanup();
	void				WriteFileNotFoundLog(char* szFileName);
public:
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL				AddFileToPackFile(CPackFile* pPackFile,char* szFileName,FSFILE_HEADER* pfsHeader);
	BOOL				RemoveFileFromPackFile(FSFILE_DESC* pDesc);


	void*	__stdcall	MapPackFile(char* szPackFileName);
	void	__stdcall	UnmapPackFile(void* pPackFileHandle);
	BOOL	__stdcall	Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod);
	DWORD	__stdcall	GetFileNum(void* pPackFileHandle);
	DWORD	__stdcall	CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum);
	void	__stdcall	DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList);

	BOOL	__stdcall	IsExistInFileStorage(char* szFileName);

	BOOL	__stdcall	LockPackFile(void* pPackFileHandle,DWORD dwFlag);
	BOOL	__stdcall	InsertFileToPackFile(void* pPackFileHandle,char* szFileName);
	BOOL	__stdcall	DeleteFileFromPackFile(char* szFileName);
	BOOL	__stdcall	UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc);

	BOOL	__stdcall	ExtractFile(char* szFileName);
	BOOL	__stdcall	ExtractAllFiles();
	DWORD	__stdcall	ExtractAllFilesFromPackFile(void* pPackFileHandle);

	void*	__stdcall	FSOpenFile(char* szFileName,DWORD dwAccessMode);
	int		__stdcall	FSScanf(void* pFP,char* szFormat, ...);
	DWORD	__stdcall	FSRead(void* pFP,void* pDest,DWORD dwLen);
	DWORD	__stdcall	FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase);
	BOOL	__stdcall	FSCloseFile(void* pFP);

	BOOL	__stdcall	GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo);
	BOOL	__stdcall	BeginLogging(char* szFileName,DWORD dwFlag);
	BOOL	__stdcall	EndLogging();
	


	CoStorage();
	~CoStorage();
	
};
//extern DWORD g_dwPackCount;
//class CPackFile;
//extern CPackFile*	g_pPack[100];

