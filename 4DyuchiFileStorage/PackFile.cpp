// PackedFile.cpp: implementation of the CPackFile class.
//
//////////////////////////////////////////////////////////////////////

#include "PackFile.h"
#include "CoStorage.h"
#include <crtdbg.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



DWORD GetSearchFileName(char* szDest,char* szSrc)
{
	
	GetNameRemovePath(szDest,szSrc);
	CharToSmallASCIILen(szDest,szDest);

	DWORD	dwSearchFileNameLen;
	dwSearchFileNameLen = lstrlen(szDest);

	return dwSearchFileNameLen;
}


DWORD GetMulConst(DWORD dwSize,DWORD dwConst)
{
	DWORD dwNewSize = (dwSize / dwConst) * dwConst + dwConst;
	return dwNewSize;
}

DWORD BuildFileItemHeader(FSFILE_HEADER** ppfsHeader,DWORD dwFileSize,char* szSearchFileName)
{
	FSFILE_HEADER*	pHeader;
	DWORD	dwNameLen = lstrlen(szSearchFileName);
	DWORD	dwTotalSize = sizeof(FSFILE_HEADER) - (sizeof(char)*4) + dwNameLen + dwFileSize;

	pHeader = (FSFILE_HEADER*)new char[dwTotalSize];

	pHeader->dwFileNameLen = dwNameLen;
	pHeader->dwRealFileSize = dwFileSize;
	pHeader->dwTotalSize = dwTotalSize;
	pHeader->dwFileDataOffset = (DWORD)pHeader->szFileName[dwNameLen] - (DWORD)pHeader ;

	*ppfsHeader = pHeader;
	
	return dwTotalSize;
}


CPackFile::CPackFile()
{	
	InitializeMembers();
}
void CPackFile::InitializeMembers()
{

	m_dwCurrentVersion = 0;
	m_dwMaxFileNum = 0;
	memset(m_szFileName,0,sizeof(m_szFileName));
	m_dwFileNameLen = 0;
	m_pPackFileNameHash = NULL;
	m_pfsHead = NULL;
	m_dwFileItemNum = NULL;
	m_bExistOriginalFile = FALSE;
}
BOOL CPackFile::Initialize(CoStorage* pStorage,VBHASH_HANDLE pFileNameHash,char* szPackFileName)
{
	InitializeMembers();

	m_pStorage = pStorage;
	
	BOOL	bResult = FALSE;
	m_pPackFileNameHash = pFileNameHash;

	m_dwFileNameLen = lstrlen(szPackFileName);
	memset(m_szFileName,0,sizeof(m_szFileName));
	memcpy(m_szFileName,szPackFileName,m_dwFileNameLen);


	DWORD	dwOpenFlag = OPEN_EXISTING;

	DWORD	dwTryCreateCount = 0;
	m_bExistOriginalFile = TRUE;


lb_create_file:
	// 작업용 팩파일을 만든다.이미 생성되어있을 수도 있고 처음 만들수도 있다.
	
	m_hOriginalFile = CreateFile(szPackFileName,GENERIC_READ | GENERIC_WRITE,0,NULL,dwOpenFlag,FILE_ATTRIBUTE_NORMAL,NULL);

	dwTryCreateCount++;
	if (INVALID_HANDLE_VALUE == m_hOriginalFile)
	{
		m_bExistOriginalFile = FALSE;
		dwOpenFlag = CREATE_NEW;
		if (dwTryCreateCount>1)
			goto lb_return;

		goto lb_create_file;		
	}


	if (!m_bExistOriginalFile)
		goto lb_skip_create_index;

	PACK_FILE_HEADER	header;
	DWORD				dwReadBytes;
	dwReadBytes = 0;
		
	ReadFile(m_hOriginalFile,&header,sizeof(header),&dwReadBytes,NULL);
	if (dwReadBytes != sizeof(header))
	{
		goto lb_fail_after_create_file;
	}

	m_dwCurrentVersion = header.dwVersion;
	DWORD	i;
	for (i=0; i<header.dwFileItemNum; i++)
	{
		// 현재의 파일포인터를 얻어온다.
		DWORD dwCurrentFilePosition; 
		dwCurrentFilePosition = SetFilePointer(m_hOriginalFile,0,NULL,FILE_CURRENT);

		char	pHeaderBuffer[FSFILE_HEADER_SIZE + _MAX_PATH];
		memset(pHeaderBuffer,0,sizeof(pHeaderBuffer));

		FSFILE_HEADER*	pHeader;
		ReadFile(m_hOriginalFile,pHeaderBuffer,FSFILE_HEADER_SIZE,&dwReadBytes,NULL);
		
		pHeader = (FSFILE_HEADER*)pHeaderBuffer;



		if (!dwReadBytes)
		{
			goto lb_fail_after_create_file;

		}
		// 파일포인터가 헤더에 들어있는 값과 일치하지 않으면 잘못된 파일이다.
		if (dwCurrentFilePosition != pHeader->dwFileDataOffset)
		{
			goto lb_fail_after_create_file;
		}

		// 파일 이름 길이가 비정상적인 경우도 잘못된 파일이다.
		if (pHeader->dwFileNameLen > _MAX_PATH)
		{
	lb_fail_after_create_file:
			CloseHandle(m_hOriginalFile);
#ifdef _DEBUG
			char	szErrTxt[128];
			wsprintf(szErrTxt,"\n %s Courrupted\n",szPackFileName);
			OutputDebugString(szErrTxt);
			__asm int 3
#endif
			goto lb_return;
		}
		ReadFile(m_hOriginalFile,pHeaderBuffer+FSFILE_HEADER_SIZE,pHeader->dwFileNameLen+1,&dwReadBytes,NULL);
		SetFilePointer(m_hOriginalFile,pHeader->dwRealFileSize,NULL,FILE_CURRENT);

		// 메모리상에 인덱스를 구성하기 위하여 스토리지에 넣는다.
		if (!m_pStorage->AddFileToPackFile(this,pHeader->szFileName,pHeader))
		{
#ifdef _DEBUG
			char	szErrTxt[128];
			wsprintf(szErrTxt,"\n Fail to m_pStorage->AddFileToPackFile() \n");
			OutputDebugString(szErrTxt);
			__asm int 3
#endif
		}
	
	}



lb_skip_create_index:
	bResult = TRUE;
lb_return:
	return bResult;
}

DWORD CPackFile::CreateFileInfoList(FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum)
{
	DWORD	dwNum = m_dwFileItemNum;

	if (dwNum > dwMaxNum)
		dwNum = dwMaxNum;

	*ppInfoList = new FSFILE_ATOM_INFO[dwNum];

	FSFILE_DESC*	pCur = m_pfsHead;

	DWORD	dwCount = 0;
	while (pCur)
	{
		(*ppInfoList)[dwCount].dwFileNameLen = pCur->dwFileNameLen;
		(*ppInfoList)[dwCount].dwPositionInPackFile =  pCur->dwPositionInPackFile;
		(*ppInfoList)[dwCount].dwRealFileSize = pCur->dwRealFileSize;
		(*ppInfoList)[dwCount].pszFileName = pCur->pszFileName;
		(*ppInfoList)[dwCount].dwTotalSize = pCur->dwTotalSize;
		dwCount++;
		pCur = pCur->pNextInPackFile;
	}
	return dwNum;
}
void CPackFile::DeleteFileInfoList(FSFILE_ATOM_INFO* pInfoList)
{
	delete [] pInfoList;
}
BOOL CPackFile::BeginCreatePackFile(DWORD dwFlag)
{
	BOOL		bResult = FALSE;

	m_bMustRebuild = FALSE;

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CPackFile::AddFileItem(char* szFileName,char* szSearchFileItemName,DWORD dwSearchFileItemLen,FSFILE_DESC* pNewDesc)
{
	// 파일을 추가한다.추가할때 같은 파일이름이 존재하면 기존의 것을 지우고 덮어쓴다.
	// 추가할때 기존의 파일의 디스크에서의 사이즈보다 나중 파일의 디스크 사이즈가 크면 
	// 팩파일 전체를 재구성해야한다. 이 경우는 갱신될 파일리스트를 작성하고 나중에 한번에
	// 업데이트 한다.

	BOOL	bResult = FALSE;
	
	pNewDesc->pPrvInPackFile = NULL;
	if (!m_pfsHead)
	{
		m_pfsHead = pNewDesc;
		pNewDesc->pNextInPackFile = NULL;
		
	}
	else
	{
		pNewDesc->pNextInPackFile = m_pfsHead;
		m_pfsHead->pPrvInPackFile = pNewDesc;
		m_pfsHead = pNewDesc;
	}
	
	pNewDesc->pOwnerPackFile = this;
	pNewDesc->dwFileNameLen = lstrlen(szFileName);
	pNewDesc->pszFileName = new char[pNewDesc->dwFileNameLen+1];
	memset(pNewDesc->pszFileName,0,pNewDesc->dwFileNameLen+1);
	memcpy(pNewDesc->pszFileName,szFileName,pNewDesc->dwFileNameLen);

	m_bMustRebuild	= TRUE;
	bResult = TRUE;
	m_dwFileItemNum++;

lb_return:
	

lb_fail_create_file:
	return bResult;
}

BOOL CPackFile::DeleteFileItem(FSFILE_DESC* pfsDesc)
{
	FSFILE_DESC*	pPrv = pfsDesc->pPrvInPackFile;
	FSFILE_DESC*	pNext = pfsDesc->pNextInPackFile;

	if (pPrv)
		pPrv->pNextInPackFile = pNext;
	

	if (pNext)
		pNext->pPrvInPackFile = pPrv;
	

	if (pfsDesc == m_pfsHead)
	{
		m_pfsHead = pNext;

	}

	delete [] pfsDesc->pszFileName;
	m_bMustRebuild	= TRUE;
	m_dwFileItemNum--;

	return TRUE;

}
/*
GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	();
GLOBAL_FUNC_DLL void			__stdcall	(STMPOOL_HANDLE pool);


GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
*/
BOOL CPackFile::EndCreatePackFile(LOAD_CALLBACK_FUNC pCallBackFunc)
{
	// 갱신될 파일 리스트를 가지고 있다가 이때 한번에 갱신해야된다.

	BOOL	bResult = FALSE;

	if (!m_bMustRebuild)
		goto lb_return;

	char	szWriteFileName[_MAX_PATH];
	memset(szWriteFileName,0,sizeof(szWriteFileName));
	memcpy(szWriteFileName,m_szFileName,m_dwFileNameLen);
	lstrcat(szWriteFileName,".tmp");
	
	HANDLE hWriteFile;
	hWriteFile = CreateFile(szWriteFileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE == hWriteFile)
	{
		goto lb_return;
	}

	PACK_FILE_HEADER	fileHeader;
	memset(&fileHeader,0,sizeof(fileHeader));
	fileHeader.dwVersion = CURRENT_VERSION;
	fileHeader.dwFileItemNum = m_dwFileItemNum;

	DWORD	dwWriteBytes,dwReadBytes;
	WriteFile(hWriteFile,&fileHeader,sizeof(PACK_FILE_HEADER),&dwWriteBytes,NULL);

	DWORD	dwCount;
	dwCount = 0;
	FSFILE_DESC*	pDesc;

	pDesc = m_pfsHead;
	FSFILE_HEADER*	pWriteHeader;

	DWORD			dwSuccessedFileItemNum;


	dwSuccessedFileItemNum = 0;
	while (pDesc)
	{
		if (pDesc->bSourceFromPackFile)
		{
			// 이 경우는 워크 파일에 있는 데이타를 그대로 긁어다 복사하면 된다.
			char*	pBuffer = new char[pDesc->dwTotalSize];
			
			// 읽어서..
			SetFilePointer(m_hOriginalFile,pDesc->dwPositionInPackFile,NULL,FILE_BEGIN);
			ReadFile(m_hOriginalFile,pBuffer,pDesc->dwTotalSize,&dwReadBytes,NULL);

			// 세이브용 파일의 현재 위치를 얻는다.
			DWORD dwWriteBytes,dwCurrentFilePosition; 
			dwCurrentFilePosition = SetFilePointer(hWriteFile,0,NULL,FILE_CURRENT);


			// 써넣는다
			pWriteHeader = (FSFILE_HEADER*)pBuffer;
			pWriteHeader->dwFileDataOffset = dwCurrentFilePosition;

			WriteFile(hWriteFile,pBuffer,pDesc->dwTotalSize,&dwWriteBytes,NULL);
			delete [] pBuffer;
			dwSuccessedFileItemNum++;

		}
		else
		{
			
			HANDLE	hSrcFile = CreateFile(pDesc->pszFileName,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (INVALID_HANDLE_VALUE == hSrcFile)
			{
				// 파일을 추가하지 못했음을 알리는 메시지 박스를 띄우고..
				goto lb_skip;
			}
			DWORD	dwRealFileSize;
			dwRealFileSize = GetFileSize(hSrcFile,NULL);
			pDesc->dwRealFileSize = dwRealFileSize;

			pDesc->dwFileNameLen = GetSearchFileName(pDesc->pszFileName,pDesc->pszFileName);
			pDesc->dwTotalSize = FSFILE_HEADER_SIZE + dwRealFileSize + pDesc->dwFileNameLen + 1;
			char* pszFileName;
			pszFileName = (char*)VBHGetKeyPtr(pDesc->pHashHandle);

#ifdef _DEBUG
			
			if (memcmp(pszFileName,pDesc->pszFileName,pDesc->dwFileNameLen))
				__asm int 3
#endif
			

			pDesc->bSourceFromPackFile = TRUE;

			// 세이브용 파일의 현재 위치를 얻는다.
			DWORD dwWriteBytes,dwCurrentFilePosition; 
			dwCurrentFilePosition = SetFilePointer(hWriteFile,0,NULL,FILE_CURRENT);

			pDesc->dwPositionInPackFile = dwCurrentFilePosition;
						

			char*	pBuffer = new char[pDesc->dwTotalSize];
			memset(pBuffer,0,pDesc->dwTotalSize);

			// 써넣는다
			pWriteHeader = (FSFILE_HEADER*)pBuffer;
			pWriteHeader->dwFileDataOffset = dwCurrentFilePosition;
			pWriteHeader->dwFileNameLen = pDesc->dwFileNameLen;
			
			pWriteHeader->dwRealFileSize = dwRealFileSize;
			pWriteHeader->dwTotalSize = pDesc->dwTotalSize;
			memcpy(pWriteHeader->szFileName,pszFileName,pDesc->dwFileNameLen);

			ReadFile(hSrcFile,pWriteHeader->szFileName+pDesc->dwFileNameLen+1,dwRealFileSize,&dwReadBytes,NULL);
			_ASSERT(_CrtCheckMemory());

			WriteFile(hWriteFile,pBuffer,pDesc->dwTotalSize,&dwWriteBytes,NULL);
			delete [] pBuffer;
			dwSuccessedFileItemNum++;
			CloseHandle(hSrcFile);
		}
		
lb_skip:
		if (pCallBackFunc)
		{
			pCallBackFunc(dwCount,m_dwFileItemNum,NULL);
		}
		pDesc = pDesc->pNextInPackFile;
		dwCount++;
	}

	// 성공한 파일 개수를 다시 써넣는다
	SetFilePointer(hWriteFile,0,NULL,FILE_BEGIN);
	fileHeader.dwFileItemNum = dwSuccessedFileItemNum;
	WriteFile(hWriteFile,&fileHeader,sizeof(PACK_FILE_HEADER),&dwWriteBytes,NULL);


	CloseHandle(m_hOriginalFile);
	m_hOriginalFile = INVALID_HANDLE_VALUE;
	CloseHandle(hWriteFile);

	DeleteFile(m_szFileName);
	MoveFile(szWriteFileName,m_szFileName);

	Cleanup();
	char	szFileName[_MAX_PATH];
	memset(szFileName,0,sizeof(szFileName));
	memcpy(szFileName,m_szFileName,m_dwFileNameLen);
	Initialize(m_pStorage,m_pPackFileNameHash,szFileName);

	bResult = TRUE;
	
lb_return:
	return bResult;
}

DWORD CPackFile::ExtractAllFiles()
{
	DWORD	dwCount = 0;
	FSFILE_DESC*	pDesc = m_pfsHead;
	while (pDesc)
	{
		
		if (ExtractFile(pDesc))
			dwCount++;

		pDesc = pDesc->pNextInPackFile;
	}
	return dwCount;
}

BOOL CPackFile::ExtractFile(FSFILE_DESC* pfsDesc)
{
	BOOL	bResult = FALSE;
	if (!pfsDesc->bSourceFromPackFile)
	{
		__asm int 3
		goto lb_return;
	}


	DWORD	dwReadBytes;
	char*	pBuffer;
	pBuffer = new char[pfsDesc->dwTotalSize];
	
	// 위치 지정
	SetFilePointer(m_hOriginalFile,pfsDesc->dwPositionInPackFile,NULL,FILE_BEGIN);

	// 읽어서..
	ReadFile(m_hOriginalFile,pBuffer,pfsDesc->dwTotalSize,&dwReadBytes,NULL);

	FSFILE_HEADER*	pWriteHeader;
	pWriteHeader = (FSFILE_HEADER*)pBuffer;
	
	HANDLE	hWriteFile;
	DWORD	dwWriteBytes;

	hWriteFile = CreateFile(pWriteHeader->szFileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(hWriteFile,pWriteHeader->szFileName+pWriteHeader->dwFileNameLen+1,pWriteHeader->dwRealFileSize,&dwWriteBytes,NULL);
	
	delete [] pBuffer;

	CloseHandle(hWriteFile);


	bResult = TRUE;
lb_return:
	return bResult;
}


DWORD CPackFile::PFReadFile(char* pDestBuffer,FSFILE_DESC* pDesc,DWORD dwOffset,DWORD dwLen)
{

	DWORD	dwRealDataOffset,dwLastOffset,dwFileOffset;

	DWORD	dwReadBytes = 0;
	if (dwOffset >= pDesc->dwRealFileSize)
		goto lb_return;

	dwRealDataOffset = pDesc->dwPositionInPackFile + FSFILE_HEADER_SIZE + pDesc->dwFileNameLen + 1;
	dwLastOffset = dwRealDataOffset + pDesc->dwRealFileSize;
	dwFileOffset = dwRealDataOffset + dwOffset;
		
	SetFilePointer(m_hOriginalFile,dwFileOffset,NULL,FILE_BEGIN);


	if ( dwFileOffset+dwLen  > dwLastOffset)
		dwLen = dwLastOffset - dwFileOffset;
		
	ReadFile(m_hOriginalFile,pDestBuffer,dwLen,&dwReadBytes,NULL);

lb_return:
	return dwReadBytes;

}



void CPackFile::Cleanup()
{
	
	while(m_pfsHead)
	{
#ifdef _DEBUG
		if (m_pfsHead->pOwnerPackFile != this)
			__asm int 3
#endif

		m_pStorage->RemoveFileFromPackFile(m_pfsHead);
	}
	if (m_hOriginalFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hOriginalFile);
		m_hOriginalFile = INVALID_HANDLE_VALUE;

	}

}

void CPackFile::GetInfo(FSPACK_FILE_INFO* pPackFileInfo)
{
	pPackFileInfo->dwVersion = m_dwCurrentVersion;
	pPackFileInfo->dwFileItemNum = m_dwFileItemNum;
	pPackFileInfo->dwFlag = 0;
	lstrcpy(pPackFileInfo->szFileName,m_szFileName);
	pPackFileInfo->dwFileNameLen = m_dwFileNameLen;
}
CPackFile::~CPackFile()
{
	Cleanup();

}

