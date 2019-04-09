// DataRelayManager.cpp: implementation of the CDataBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "DataBlockManager.h"
#include "DataBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataBlockManager::CDataBlockManager()
:	m_pDataBlockOccupied(NULL)
,	m_pDataBlockArray(NULL)
,	m_pDataStorage(NULL)
{

}

CDataBlockManager::~CDataBlockManager()
{
	Release();
}

void CDataBlockManager::Init(DWORD DataBlockCount,DWORD MaxDataBlockSize)
{
	Release();

	m_DataBlockCount = DataBlockCount;
	m_pDataBlockOccupied = new BOOL[m_DataBlockCount];
	m_pDataBlockArray = new CDataBlock[m_DataBlockCount];
	m_pDataStorage = new char[DataBlockCount*MaxDataBlockSize];

	CDataBlock::Max_DataBlock_Size = MaxDataBlockSize;
	for(DWORD n=0;n<DataBlockCount;++n)
	{
		m_pDataBlockOccupied[n] = FALSE;
		m_pDataBlockArray[n].Init(n,m_pDataStorage + MaxDataBlockSize * n);
	}
}

void CDataBlockManager::Release()
{
	SAFE_DELETE_ARRAY(m_pDataBlockOccupied);
	SAFE_DELETE_ARRAY(m_pDataBlockArray);
	SAFE_DELETE_ARRAY(m_pDataStorage);
}

CDataBlock* CDataBlockManager::GetEmptyDataBlock()
{
	for(DWORD n=0;n<m_DataBlockCount;++n)
	{
		if(m_pDataBlockOccupied[n] == FALSE)
		{
			m_pDataBlockOccupied[n] = TRUE;
			m_pDataBlockArray[n].Clear(TRUE);
			return &m_pDataBlockArray[n];
		}
	}

	ASSERT(0);
	return NULL;
}

CDataBlock* CDataBlockManager::GetDataBlock(DWORD DataBlockNum)
{
	ASSERT(m_pDataBlockOccupied[DataBlockNum]);

	m_pDataBlockArray[DataBlockNum].Clear(FALSE);
	return &m_pDataBlockArray[DataBlockNum];
}

void CDataBlockManager::ReleaseDataBlock(DWORD DataBlockNum)
{
	m_pDataBlockOccupied[DataBlockNum] = FALSE;
}