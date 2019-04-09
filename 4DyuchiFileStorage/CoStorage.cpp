#include "CoStorage.h"
#include "PackFile.h"
#include <crtdbg.h>




//DWORD g_dwPackCount = 0;
//CPackFile*	g_pPack[100];
extern LONG			g_lComponent;


char*	g_pMemLast = NULL;

BOOL IsSpace(int c)
{
	//0x09 ? 0x0D or 0x20
	BOOL	bResult;
	__asm 
	{
		xor		eax,eax

		

		mov		edx,dword ptr[c]

		cmp		dl,0x00
		jz		lb_true

		cmp		dl,0x20
		jz		lb_true

		cmp		dl,0x09
		jb		lb_exit

		cmp		dl,0x0d
		ja		lb_exit

lb_true:
		inc		eax
		
lb_exit:
		mov		dword ptr[bResult],eax
	}
	return bResult;

	
}

STDMETHODIMP CoStorage::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiFileStorage == refiid)
		*ppv = (I4DyuchiFileStorage*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoStorage::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoStorage::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CoStorage::CoStorage()
{
	InterlockedIncrement( &g_lComponent);
	
	m_dwRefCount = 0;
	
	m_pFileItemNameHash = NULL;
	m_pPackFileNameHash = NULL;
	m_pFileDescPool = NULL;
	m_dwClusterSize = 8192;

	m_dwMaxFileNum = 0;

	m_dwMaxFileNameLen = 0;
	m_dwMaxInsertedFileNameLen = 0;
	
	m_fpLog = NULL;

#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif
}
BOOL __stdcall CoStorage::Initialize(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod)
{
	m_dwMaxFileNameLen = dwMaxFileNameLen;
	m_dwMaxFileNum = dwMaxFileNum;


	m_pFileDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pFileDescPool,sizeof(FSFILE_DESC),m_dwMaxFileNum/16+1,m_dwMaxFileNum);

	m_pFileItemNameHash = VBHCreate();
	VBHInitialize(m_pFileItemNameHash,m_dwMaxFileNum / 4 + 4,m_dwMaxFileNameLen,m_dwMaxFileNum);

	m_pPackFileNameHash = VBHCreate();
	VBHInitialize(m_pPackFileNameHash,MAX_PACK_FILE_NUM / 4 + 4,_MAX_PATH,MAX_PACK_FILE_NUM);

	m_pFileHandlePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pFileHandlePool,sizeof(FSFILE_POINTER),dwMaxFileHandleNumAtSameTime/4+1,dwMaxFileHandleNumAtSameTime);

	m_fileAccessMethod = accessMethod;
	return TRUE;
}
BOOL __stdcall CoStorage::BeginLogging(char* szFileName,DWORD dwFlag)
{

	BOOL	bResult = FALSE;
	m_fpLog = fopen(szFileName,"wt");
	if (!m_fpLog)
		goto lb_return;

	bResult = TRUE;
lb_return:
	return bResult;

}
BOOL __stdcall CoStorage::EndLogging()
{
	BOOL		bResult = FALSE;

	if (!m_fpLog)
		goto lb_return;
	
	fclose(m_fpLog);
	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD __stdcall CoStorage::GetFileNum(void* pPackFileHandle)
{
	DWORD dwNum = 0;

	// 핸들체크 한번 해주자..
	//
	//////////////////////////
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	dwNum = pPackFile->GetFileNum();
	return dwNum;
}
DWORD __stdcall CoStorage::CreateFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO** ppInfoList,DWORD dwMaxNum)
{
	DWORD	dwFileNum = 0;
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	dwFileNum = pPackFile->CreateFileInfoList(ppInfoList,dwMaxNum);
	return dwFileNum;
}

void __stdcall CoStorage::DeleteFileInfoList(void* pPackFileHandle,FSFILE_ATOM_INFO* pInfoList)
{
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	pPackFile->DeleteFileInfoList(pInfoList);
}
void* __stdcall CoStorage::MapPackFile(char* szPackFileName)
{
	CPackFile*	pPackFile = NULL;

	BOOL	bResult = FALSE;
	
	
	char	szSearchPackFileName[256];
	DWORD	dwSearchPackFileLen;
	memset(szSearchPackFileName,0,sizeof(szSearchPackFileName));

	dwSearchPackFileLen = GetSearchFileName(szSearchPackFileName,szPackFileName);


	if (VBHSelect(m_pPackFileNameHash,(DWORD*)&pPackFile,1,szSearchPackFileName,dwSearchPackFileLen))
	{
		pPackFile = NULL;
		goto lb_return;
	}
	pPackFile = new CPackFile;
	bResult = pPackFile->Initialize(this,m_pFileItemNameHash,szPackFileName);
	
	


	void*	pHashHandle;
	pHashHandle = VBHInsert(m_pPackFileNameHash,(DWORD)pPackFile,szSearchPackFileName,dwSearchPackFileLen);
	if (!pHashHandle)
	{
		delete pPackFile;
		pPackFile = NULL;
		goto lb_return;
	}
	pPackFile->m_pSearchHandle = pHashHandle;

lb_return:
	return pPackFile;
}
void __stdcall CoStorage::UnmapPackFile(void* pPackFileHandle)
{
	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;

	

 	VBHDelete(m_pPackFileNameHash,pPackFile->m_pSearchHandle);
	delete pPackFile;
}
/*
BOOL __stdcall CoStorage::LockPackFile(char* szPackFileName,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	char	szSearchPackFileName[256];
	DWORD	dwSearchPackFileLen;
	memset(szSearchPackFileName,0,sizeof(szSearchPackFileName));

	dwSearchPackFileLen = GetSearchFileName(szSearchPackFileName,szPackFileName);


	CPackFile*	pPackFile;
	if (!VBHSelect(m_pPackFileNameHash,(DWORD*)&pPackFile,1,szSearchPackFileName,dwSearchPackFileLen))
	{
		goto lb_return;
	}
	bResult = pPackFile->BeginCreatePackFile(dwFlag);
	
lb_return:
	return bResult;
}
*/
BOOL __stdcall CoStorage::LockPackFile(void* pPackFileHandle,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->BeginCreatePackFile(dwFlag);
	
lb_return:
	return bResult;
}

BOOL __stdcall CoStorage::UnlockPackFile(void* pPackFileHandle,LOAD_CALLBACK_FUNC pCallBackFunc)
{
	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->EndCreatePackFile(pCallBackFunc);
	
	
lb_return:
	return bResult;
}

BOOL __stdcall CoStorage::GetPackFileInfo(void* pPackFileHandle,FSPACK_FILE_INFO* pFileInfo)
{
	CPackFile* pPackFile = (CPackFile*)pPackFileHandle;
	pPackFile->GetInfo(pFileInfo);
	return TRUE;
}

BOOL __stdcall CoStorage::IsExistInFileStorage(char* szFileName)
{
	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);

	FSFILE_DESC*	pOldDesc;
	if (VBHSelect(m_pFileItemNameHash,(DWORD*)&pOldDesc,1,szSearchFileItemName,dwSearchFileItemLen))
	{
		bResult = TRUE;
	}
lb_return:
	return bResult;
}
BOOL __stdcall CoStorage::InsertFileToPackFile(void* pPackFileHandle,char* szFileName)
{
	

	BOOL	bResult = FALSE;

	CPackFile*	pPackFile = (CPackFile*)pPackFileHandle;
	bResult = AddFileToPackFile(pPackFile,szFileName,NULL);
lb_return:
	return bResult;
}



// 이것은 팩파일 클래스쪽에서 역으로 호출하는 함수이다. 
// 이 함수가 호출될 시점에서 add행위는 이미 결정되어있다.
// 같은 이름의 파일이 존재하면 덮어버린다.
BOOL CoStorage::AddFileToPackFile(CPackFile* pPackFile,char* szFileName,FSFILE_HEADER* pfsHeader)
{


	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);

	FSFILE_DESC*	pNewDesc;
	pNewDesc = (FSFILE_DESC*)LALAlloc(m_pFileDescPool);

	if (!pNewDesc)
		goto lb_return;

	memset(pNewDesc,0,sizeof(FSFILE_DESC));

	FSFILE_DESC*	pOldDesc;
	if (VBHSelect(m_pFileItemNameHash,(DWORD*)&pOldDesc,1,szSearchFileItemName,dwSearchFileItemLen))
	{

		// 팩 파일에서 삭제한다.
		pOldDesc->pOwnerPackFile->DeleteFileItem(pOldDesc);
		
		// 기존파일 디스크립터를 삭제한다.
		VBHDelete(m_pFileItemNameHash,pOldDesc->pHashHandle);
		LALFree(m_pFileDescPool,pOldDesc);


	}

	// 해쉬에다 넣는다.
	void*	pHashHandle;
	pHashHandle = VBHInsert(m_pFileItemNameHash,(DWORD)pNewDesc,szSearchFileItemName,dwSearchFileItemLen);
	if (!pHashHandle)
	{
		memset(pNewDesc,0,sizeof(FSFILE_DESC));
		LALFree(m_pFileDescPool,pNewDesc);
		pNewDesc = NULL;
		goto lb_return;
	}


	// 팩파일에도 넣는다.
	pNewDesc->pHashHandle = pHashHandle;
	
	
	char*	pszFileName;
	pszFileName = (char*)VBHGetKeyPtr(pNewDesc->pHashHandle);

	if (pfsHeader)
	{
		pNewDesc->bSourceFromPackFile = TRUE;
		pNewDesc->dwTotalSize = pfsHeader->dwTotalSize;
		pNewDesc->dwRealFileSize = pfsHeader->dwRealFileSize;
		pNewDesc->dwPositionInPackFile = pfsHeader->dwFileDataOffset;
	}
		
	pPackFile->AddFileItem(szFileName,szSearchFileItemName,dwSearchFileItemLen,pNewDesc);

	DWORD	dwFileNameLen;

	dwFileNameLen = lstrlen(szSearchFileItemName);
	if (m_dwMaxInsertedFileNameLen < dwFileNameLen)
		m_dwMaxInsertedFileNameLen = dwFileNameLen;

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL __stdcall CoStorage::DeleteFileFromPackFile(char* szFileName)
{

	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));
	
	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);

	FSFILE_DESC*	pDelDesc;
	if (!VBHSelect(m_pFileItemNameHash,(DWORD*)&pDelDesc,1,szSearchFileItemName,dwSearchFileItemLen))
		goto lb_return;

	bResult = RemoveFileFromPackFile(pDelDesc);

lb_return:
	return bResult;
}
BOOL __stdcall CoStorage::ExtractAllFiles()
{
	FSFILE_DESC**	ppDescList = new FSFILE_DESC*[m_dwMaxFileNum];
	
	DWORD dwNum = VBHGetAllItem(m_pFileItemNameHash,(DWORD*)ppDescList,m_dwMaxFileNum);

	for (DWORD i=0; i<dwNum; i++)
	{
		ppDescList[i]->pOwnerPackFile->ExtractFile(ppDescList[i]);
	}
	delete [] ppDescList;

	return TRUE;
}
BOOL __stdcall CoStorage::ExtractFile(char* szFileName)
{
	BOOL	bResult = FALSE;

	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));
	
	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);

	FSFILE_DESC*	pDesc;
	if (!VBHSelect(m_pFileItemNameHash,(DWORD*)&pDesc,1,szSearchFileItemName,dwSearchFileItemLen))
		goto lb_return;

	bResult = pDesc->pOwnerPackFile->ExtractFile(pDesc);

lb_return:
	return bResult;
}
DWORD __stdcall CoStorage::ExtractAllFilesFromPackFile(void* pPackFileHandle)
{
	BOOL	bResult = FALSE;

	CPackFile*		pPackFile = (CPackFile*)pPackFileHandle;
	bResult = pPackFile->ExtractAllFiles();
	return bResult;

}
BOOL CoStorage::RemoveFileFromPackFile(FSFILE_DESC* pDelDesc)
{

	BOOL	bResult = FALSE;
	
	CPackFile*	pPackFile;

	pPackFile = pDelDesc->pOwnerPackFile;
	if (!pPackFile)
		__asm int 3

	pPackFile->DeleteFileItem(pDelDesc);
	

	// 기존파일 디스크립터를 삭제한다.
	VBHDelete(m_pFileItemNameHash,pDelDesc->pHashHandle);
	LALFree(m_pFileDescPool,pDelDesc);

	bResult = TRUE;


lb_return:
	return bResult;
}

void* __stdcall	CoStorage::FSOpenFile(char* szFileName,DWORD dwAccessMode)
{
	FSFILE_POINTER*	pFP = NULL;
	BOOL	bResult = FALSE;
	FILE*	fp = NULL;
	FSFILE_DESC*	pDesc = NULL;

	pFP = (FSFILE_POINTER*)LALAlloc(m_pFileHandlePool);
	if (!pFP)
		goto lb_return;

	memset(pFP,0,sizeof(FSFILE_POINTER));

		
	if (m_fileAccessMethod == FILE_ACCESS_METHOD_ONLY_FILE)
		goto lb_create_file;


	char	szSearchFileItemName[256];
	DWORD	dwSearchFileItemLen;
	memset(szSearchFileItemName,0,sizeof(szSearchFileItemName));


	dwSearchFileItemLen = GetSearchFileName(szSearchFileItemName,szFileName);
	
	
	if (VBHSelect(m_pFileItemNameHash,(DWORD*)&pDesc,1,szSearchFileItemName,dwSearchFileItemLen))
	{
		pFP->bFromPack = TRUE;
		pFP->pFSDesc = pDesc;
		if (dwAccessMode == FSFILE_ACCESSMODE_TEXT)
		{
			// 팩 파일에 들어있는데 텍스트 모드로 열었을 경우..
			if (!pDesc->pEntryInMemory)
			{
#ifdef _DEBUG
				if (pDesc->dwOpenCount)
					__asm int 3
#endif

				pDesc->pEntryInMemory = new char[pDesc->dwRealFileSize + 1];
				pDesc->pOwnerPackFile->PFReadFile(pDesc->pEntryInMemory,pDesc,0,pDesc->dwRealFileSize);			
				pDesc->pEntryInMemory[pDesc->dwRealFileSize] = 0;
			}
		}
		pDesc->dwOpenCount++;
		goto lb_return;
	}

	if (m_fileAccessMethod == FILE_ACCESS_METHOD_FILE_OR_PACK)
	{
lb_create_file:
		char* pAccessText = "rt";
		char* pAccessBin = "rb";
		char*	pAccessStr;
		if (dwAccessMode == FSFILE_ACCESSMODE_TEXT)
			pAccessStr = pAccessText;
		else
			pAccessStr = pAccessBin;
		pFP->fp = fopen(szFileName,pAccessStr);
		if (!pFP->fp)
			goto lb_fail;

		pFP->bFromPack = FALSE;
		goto lb_return;
		
	}

lb_fail:
	// 여기까지 왔으면 파일을 여는데 실패했다.
	LALFree(m_pFileHandlePool,pFP);
	pFP = NULL;
	if (m_fpLog)
	{
		WriteFileNotFoundLog(szFileName);
	}

lb_return:

	return pFP;
}
void CoStorage::WriteFileNotFoundLog(char* szFileName)
{
	fprintf(m_fpLog,"File Not Found : %s \n",szFileName);
}
DWORD __stdcall CoStorage::FSRead(void* pFP,void* pDest,DWORD dwLen)
{

	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;
	DWORD	dwReadBytes = 0;
	if (pScanFP->bFromPack)
	{
#ifdef _DEBUG
		if (!pScanFP->pFSDesc)
			__asm int 3

		if (!pScanFP->pFSDesc->pOwnerPackFile)
			__asm int 3

#endif
		dwReadBytes = pScanFP->pFSDesc->pOwnerPackFile->PFReadFile((char*)pDest,pScanFP->pFSDesc,pScanFP->dwOffset,dwLen);
		pScanFP->dwOffset += dwReadBytes;
	}
	else
	{
#ifdef _DEBUG
		if (!pScanFP->fp)
			__asm int 3
#endif
		dwReadBytes = fread(pDest,sizeof(char),dwLen,pScanFP->fp);
	}
	return dwReadBytes;
}

DWORD __stdcall CoStorage::FSSeek(void* pFP,DWORD dwOffset,FSFILE_SEEK seekBase)
{


	DWORD	dwCurOffset;
	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;

	if (pScanFP->bFromPack)
	{
		if (dwOffset > pScanFP->pFSDesc->dwRealFileSize)
			dwOffset = pScanFP->pFSDesc->dwRealFileSize;

		if (seekBase == FSFILE_SEEK_SET)
		{
			pScanFP->dwOffset = dwOffset;
		} 
		else if (seekBase == FSFILE_SEEK_CUR)
		{
			pScanFP->dwOffset += dwOffset;
		}
		else if (seekBase == FSFILE_SEEK_END)
		{
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize - dwOffset;
		}

		if (pScanFP->dwOffset > pScanFP->pFSDesc->dwRealFileSize)
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize;


		dwCurOffset = pScanFP->dwOffset;
	}
	else
	{
#ifdef _DEBUG
		if (!pScanFP->fp)
			__asm int 3
#endif
		dwCurOffset = fseek(pScanFP->fp,dwOffset,seekBase);

	}
	return dwCurOffset;
}


int ConvertHexToBin(char* pResult4BinArray,char cHexCode)
{
	int		iCount,iResult,i;
	char	decimal;
	char	div_value;
	char	mod_value;


	iResult = -1;
	
	
	if (cHexCode >= 'a' && cHexCode <= 'z')
	{
		decimal = cHexCode - 'a' + 10;
		goto lb_convert;
	}

	if (cHexCode >= 'A' && cHexCode <= 'Z')
	{
		decimal = cHexCode - 'A' + 10;
		goto lb_convert;
	}
	if (cHexCode >= '0' && cHexCode <= '9')
	{
		decimal = cHexCode - '0';
		goto lb_convert;
	}

	goto lb_return;

lb_convert:
	pResult4BinArray[0] = 0;
	pResult4BinArray[1] = 0;
	pResult4BinArray[2] = 0;
	pResult4BinArray[3] = 0;

	iCount = 0;
	for (i=0; i<4; i++)
	{
//	while (decimal)
//	{
		div_value = decimal >> 1;
		mod_value = decimal - (div_value<<1);
		decimal = div_value;
		pResult4BinArray[iCount] = mod_value;
		iCount++;
	}
lb_return:
	return iResult;
}
int ConvertHexToInt(char* pStr)
{
	char		pBin[4];
	int	iChrNum;
	
	iChrNum = lstrlen(pStr);
	int	pow = 1;

	int			iResult = 0;
	for (int i=0; i<iChrNum; i++)
	{
		ConvertHexToBin(pBin,pStr[iChrNum-i-1]);
		for (int j=0; j<4; j++)
		{
			int	temp = (int)pBin[j];
			iResult += ( (~(temp - 1)) & pow);
			pow = pow << 1;

		}
	}
	
	return iResult;

}

////////////////////////////////////////////////////////////////////////////////////////////

int __stdcall SearchFloat(BYTE* pMem,float* pFloat)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	BYTE*	p = pMem;

	BYTE	temp[64];
	temp[63] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;

	// 공백문자가 아닌 문자가 나올때까지 옵셋 이동
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 63)
			__asm int 3
#endif	

		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pFloat = atof((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	return iOffset;
}

int __stdcall SearchInt(BYTE* pMem,int* pInt)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif


	BYTE*	p = pMem;

	BYTE	temp[16];
	temp[15] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;
	
	// 공백문자가 아닌 문자가 나올때까지 옵셋 이동
	while (IsSpace(*p))
	{		
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;

		p++;
	};


	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 15)
			__asm int 3
#endif

		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pInt = atoi((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	return iOffset;
}

int __stdcall SearchHex(BYTE* pMem,int* pHex)
{
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	BYTE*	p = pMem;

	BYTE	temp[9];
	temp[8] = 0;
	
	int		iOffset;
	DWORD	dwCount = 0;
	
	// 공백문자가 아닌 문자가 나올때까지 옵셋 이동
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 8)
			__asm int 3
#endif
		temp[dwCount] = *p;
		p++;
		dwCount++;
	}
	temp[dwCount] = 0;
	*pHex = ConvertHexToInt((char*)temp);

lb_return:
	iOffset = (DWORD)p - (DWORD)pMem;

	
	return iOffset;
}
int __stdcall SearchString(BYTE* pMem,BYTE* pStr)
{

//	if ((DWORD)pMem == 0x018be009)
//		__asm int 3
#ifdef _DEBUG
	if ((int)pMem >= (int)g_pMemLast)
		__asm int 3
#endif

	int		iOffset;
	DWORD	dwCount = 0;

	BYTE*	p = pMem;
	*pStr = NULL;


	// 공백문자가 아닌 문자가 나올때까지 옵셋 이동
	while (IsSpace(*p))
	{
		if ((int)p >= (int)g_pMemLast)
			goto lb_return;
		p++;
	};

	if (NULL == *p)
		goto lb_return;

	while (!IsSpace(*p))
	{
		
#ifdef _DEBUG
		if (dwCount >= 4096)
			__asm int 3
#endif
		pStr[dwCount] = *p;
		p++;
		dwCount++;
	}
	

lb_return:
	pStr[dwCount] = 0;
	iOffset = (DWORD)p - (DWORD)pMem;
	return iOffset;
}

/////////////////////////////////////////////////////////////////////////////////////////////





_CRTIMP int __cdecl SScanf(char* pMem,char* szFormat, ...)
{
	// 이 함수에서 처리 가능한 포맷은 
	// %s %d %u %x %f 이다.이외의 포맷은 지원하지 않는다.
	int	iResult;
	va_list argptr;
	DWORD	dwArgCount = 0;

#define SearchString_INDEX		0
#define SearchFloat_INDEX		1
#define SearchInt_INDEX			2
#define SearchHex_INDEX			3

	char*		pError = "Fail to SScanf(),처리불가한 포맷입니다.";
	void*		pSearchFunc[8];
	pSearchFunc[SearchString_INDEX] = SearchString;
	pSearchFunc[SearchFloat_INDEX] = SearchFloat;
	pSearchFunc[SearchInt_INDEX] = SearchInt;
	pSearchFunc[SearchHex_INDEX] = SearchHex;
	pSearchFunc[4] = 0;
	pSearchFunc[5] = 0;
	pSearchFunc[6] = 0;
	pSearchFunc[7] = 0;

		
	// 세번째 인자의 주소를 얻어온다
	argptr = (va_list)&szFormat + _INTSIZEOF(szFormat);
	__asm
	{
		xor			ecx,ecx		
		mov			esi,dword ptr[szFormat]
		mov			edi,dword ptr[argptr]
		mov			ebx,dword ptr[pMem]
		
lb_begin_count_arg:

		mov			al,byte ptr[esi]
		or			al,al
		jz			lb_exit_count;

		cmp			al,'%'
		jnz			lb_loop_count_arg



		inc			esi
		mov			al,byte ptr[esi]	; %f 또는 %d등등...
		inc			ecx					; 파라미터 개수 증가



lb_check_string:


		cmp			al,'s'
		jnz			lb_check_float

		; string 이면..
		mov			edx,SearchString_INDEX
		jmp			lb_call_searchfunc
		

lb_check_float:
		cmp			al,'f'
		jnz			lb_check_int

		; float면 
		mov			edx,SearchFloat_INDEX
		jmp			lb_call_searchfunc

		
lb_check_int:
		cmp			al,'d'
		jnz			lb_check_dword
		jmp			lb_detect_int

lb_check_dword:

		cmp			al,'u'
		jnz			lb_check_hex

lb_detect_int:
		; int 이면..
		mov			edx,SearchInt_INDEX
		jmp			lb_call_searchfunc

lb_check_hex:
		cmp			al,'x'
		jnz			lb_check_invalid

		; hex 이면..
		mov			edx,SearchHex_INDEX
		jmp			lb_call_searchfunc

lb_check_invalid:
		; 여기까지 왔으면 처리불가..
		mov			eax,dword ptr[pError]
		push		eax
		call		dword ptr[OutputDebugString]
		int			3
		int			3
		int			3
		jmp			lb_return;
		
		
lb_call_searchfunc:
		mov			eax,dword ptr[edi]

		; 레지스터 백업
		push		esi
		push		edi
		push		ebx
		push		ecx

		; 함수 호출 


		// 버그체크
		mov			ecx,dword ptr[g_pMemLast]
		cmp			ecx,ebx
		jg			lb_valid
#ifdef _DEBUG
		// 디버그모드면 인터럽트 걸고..
		int			3
#else
		jmp			lb_skip_call
#endif
		// 버그체크
lb_valid:
		push		eax
		push		ebx

		; 함수포인터 로드 
		lea			ebx,pSearchFunc
		shl			edx,2
		add			edx,ebx
		mov			ebx,dword ptr[edx]

		call		ebx

lb_skip_call:
		; 레지스터 복원 
		pop			ecx
		pop			ebx
		pop			edi
		pop			esi

		add			ebx,eax				; 리턴된 오프셋만큼 메모리 포인터를 이동시킨다.
		add			edi,4

lb_loop_count_arg:
		inc			esi
		jmp			lb_begin_count_arg


lb_exit_count:
		mov			dword ptr[dwArgCount],ecx	
		sub			ebx,dword ptr[pMem]

		; 이동한 메모리 오프셋,즉 결과 저장
		mov			dword ptr[iResult],ebx
	}
lb_return:
	return iResult;

}

int __stdcall CoStorage::FSScanf(void* pFP,char* szFormat, ...)
{



	int iResult = EOF;
	FSFILE_POINTER*	pScanFP = (FSFILE_POINTER*)pFP;

	va_list argptr;
		
	// 세번째 인자의 주소를 얻어온다
	argptr = (va_list)&szFormat + _INTSIZEOF(szFormat);

	BOOL		bFromPack = pScanFP->bFromPack;
	
	void*		pScanFuncPtr;
	void*		pFilePtr;

	char*	pMemLastPos = NULL;
	char*	pMem = NULL;
	

	DWORD	dwOffset = 0;		// 팩파일일때 이동해야할 옵셋을 누적시킨다.
	if (bFromPack)
	{
		pMem = (char*)pScanFP->pFSDesc->pEntryInMemory + pScanFP->dwOffset;
		pMemLastPos = pScanFP->pFSDesc->pEntryInMemory + pScanFP->pFSDesc->dwRealFileSize;

		if (pMem >= pMemLastPos)
			goto lb_return;


		pScanFuncPtr = (void*)SScanf;
		pFilePtr = pMem;
		g_pMemLast = pMemLastPos;
	}
	else
	{
		pScanFuncPtr = (void*)fscanf;
		pFilePtr = pScanFP->fp;

	}
	// 스택에 인자 넣기..
	DWORD	dwArgCount;
	__asm
	{
		xor			ecx,ecx		
		mov			esi,dword ptr[szFormat]
		
lb_begin_count_arg:
		mov			al,byte ptr[esi]
		or			al,al
		jz			lb_exit_count;

		cmp			al,'%'
		jnz			lb_loop_count_arg

		inc			ecx

lb_loop_count_arg:
		inc			esi
		jmp			lb_begin_count_arg


lb_exit_count:
		mov			dword ptr[dwArgCount],ecx	

lb_call:		
		or			ecx,ecx
		jz			lb_return;
		
		mov			esi,dword ptr[argptr]
		mov			ebx,ecx
		dec			ebx
		shl			ebx,2
		add			esi,ebx
		
		add			ebx,4+(4+4)					; 함수호출 후 esp + 인자 개수*4하기 위해.

lb_loop_push_arg:
		mov			eax,dword ptr[esi]
		push		eax
		sub			esi,4

		loop		lb_loop_push_arg
	}
	// 실제적으로 함수 호출하기
	__asm
	{
		
		mov			edx,dword ptr[szFormat]
		mov			eax,dword ptr[pFilePtr]
		
		push		edx
		push		eax
		
		//call		sscanf
		call		dword ptr[pScanFuncPtr]
		mov			dword ptr[iResult],eax	; return value
		mov			dword ptr[dwOffset],eax	; pack파일일 경우 eax에는 움직여야할 옵셋이 들어있다
		add			esp,ebx
	}

	// 팩파일인 경우에 파일포인터 이동시키기
	if (bFromPack)
	{
		pScanFP->dwOffset += dwOffset;
		if (pScanFP->dwOffset > pScanFP->pFSDesc->dwRealFileSize)
		{
			pScanFP->dwOffset = pScanFP->pFSDesc->dwRealFileSize;
	//		iResult = EOF;
		}
	}

lb_return:


	return iResult;

}
BOOL CoStorage::FSCloseFile(void* pFP)
{


	FSFILE_POINTER*	pDelFP = (FSFILE_POINTER*)pFP;

	if (pDelFP->bFromPack)
	{
		pDelFP->pFSDesc->dwOpenCount--;
		if (!pDelFP->pFSDesc->dwOpenCount)
		{
			if (pDelFP->pFSDesc->pEntryInMemory)
			{
				delete [] pDelFP->pFSDesc->pEntryInMemory;
				pDelFP->pFSDesc->pEntryInMemory = NULL;
			}
		}
	}
	else
	{
#ifdef _DEBUG
		if (!pDelFP->fp)
			__asm int 3
#endif	
		fclose(pDelFP->fp);
	}

	LALFree(m_pFileHandlePool,pFP);

	return TRUE;
}

void CoStorage::Cleanup()
{
	if (m_fpLog)
	{
		fclose(m_fpLog);
		m_fpLog = NULL;
	}

	CPackFile*		pPackFile[MAX_PACK_FILE_NUM];
	DWORD dwNum = VBHGetAllItem(m_pPackFileNameHash,(DWORD*)pPackFile,MAX_PACK_FILE_NUM);

	for (DWORD i=0; i<dwNum; i++)
	{
		delete pPackFile[i];
	}




	if (m_pFileItemNameHash)
	{
		VBHRelease(m_pFileItemNameHash);
		m_pFileItemNameHash = NULL;

	}
	if (m_pPackFileNameHash)
	{
		VBHRelease(m_pPackFileNameHash);
		m_pPackFileNameHash = NULL;
	}
	if (m_pFileDescPool)
	{
		ReleaseStaticMemoryPool(m_pFileDescPool);
		m_pFileDescPool = NULL;
	}
	if (m_pFileHandlePool)
	{
		ReleaseStaticMemoryPool(m_pFileHandlePool);
		m_pFileHandlePool = NULL;
	}

}

CoStorage::~CoStorage()
{
#ifdef _DEBUG
	char	szDebugString[]	=	"CoStorage::~CoStorage()\n";
	OutputDebugString( szDebugString);
#endif

	Cleanup();
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	InterlockedDecrement( &g_lComponent);

}