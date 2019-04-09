// DataRelayManager.h: interface for the CDataBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATARELAYMANAGER_H__6DDD5B48_E2D5_4810_B2B0_B207D3B48589__INCLUDED_)
#define AFX_DATARELAYMANAGER_H__6DDD5B48_E2D5_4810_B2B0_B207D3B48589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DataBlock.h"
class CDataBlock;


class CDataBlockManager  
{
	char* m_pDataStorage;

	CDataBlock* m_pDataBlockArray;
	BOOL* m_pDataBlockOccupied;
	DWORD m_DataBlockCount;
	
public:
	MAKESINGLETON(CDataBlockManager);

	CDataBlockManager();
	virtual ~CDataBlockManager();

	void Init(DWORD DataBlockCount,DWORD MaxDataBlockSize);
	void Release();

	CDataBlock* GetEmptyDataBlock();
	CDataBlock* GetDataBlock(DWORD DataBlockNum);
	void ReleaseDataBlock(DWORD DataBlockNum);
};

#endif // !defined(AFX_DATARELAYMANAGER_H__6DDD5B48_E2D5_4810_B2B0_B207D3B48589__INCLUDED_)
