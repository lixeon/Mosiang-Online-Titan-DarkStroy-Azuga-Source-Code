// PackedFile.h: interface for the CPackFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
#define AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "typedef.h"
#include "../4DyuchiGXGFunc/global.h"

class	CoStorage;

class CPackFile  
{
	friend class CoStorage;
	CoStorage*			m_pStorage;
	BOOL				m_bCreateType;
	DWORD				m_dwMaxFileNum;
	char				m_szFileName[_MAX_PATH];
	DWORD				m_dwFileNameLen;
	VBHASH_HANDLE		m_pPackFileNameHash;
	FSFILE_DESC*		m_pfsHead;
	DWORD				m_dwFileItemNum;

	HANDLE				m_hOriginalFile;
	BOOL				m_bExistOriginalFile;

	void*				m_pSearchHandle;
	BOOL				m_bMustRebuild;
	DWORD				m_dwCurrentVersion;


	void				Cleanup();
	void				InitializeMembers();

public:
	void				GetInfo(FSPACK_FILE_INFO* pPackFileInfo);
	BOOL				Initialize(CoStorage* pStorage,VBHASH_HANDLE pFileNameHash,char* szPackFileName);
	DWORD				GetFileNum() {return m_dwFileItemNum;}
	DWORD				CreateFileInfoList(FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum);
	void				DeleteFileInfoList(FSFILE_ATOM_INFO* pInfoList);
	BOOL				BeginCreatePackFile(DWORD dwFlag);
	BOOL				AddFileItem(char* szFileName,char* szSearchFileItemName,DWORD dwSearchFileItemLen,FSFILE_DESC* pNewDesc);
	BOOL				ExtractFile(FSFILE_DESC* pfsDesc);
	DWORD				ExtractAllFiles();
	BOOL				DeleteFileItem(FSFILE_DESC* pfsDesc);
	BOOL				EndCreatePackFile(LOAD_CALLBACK_FUNC pCallBackFunc);
	
	DWORD				PFReadFile(char* pDestBuffer,FSFILE_DESC* pDesc,DWORD dwOffset,DWORD dwLen);


	CPackFile();
	~CPackFile();

};

DWORD GetSearchFileName(char* szDest,char* szSrc);

#endif // !defined(AFX_PACKEDFILE_H__2B2CB799_1342_4849_A639_F783FF629097__INCLUDED_)
