// Data.cpp: implementation of the CDataBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "DataBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD CDataBlock::Max_DataBlock_Size;

CDataBlock::CDataBlock()
{

}

void CDataBlock::Init(DWORD DataBlockNum,char* pData)
{
	m_DataBlockNum = DataBlockNum;
	m_pData = pData;
}

CDataBlock::~CDataBlock()
{

}

void CDataBlock::Clear(BOOL bIsStore)
{
	m_bIsStoring = bIsStore;
	m_Index = 0;
}

void CDataBlock::AddData(void* pData,int DataSize)
{
	ASSERT(m_bIsStoring);
	ASSERT(m_Index + DataSize < Max_DataBlock_Size);
	
	memcpy(&m_pData[m_Index],pData,DataSize);
	m_Index += DataSize;
}

/*
template<class T>
void CDataBlock::operator << (T& data)
{
	ASSERT(m_bIsStoring);
	ASSERT(m_Index + sizeof(T) < Max_DataBlock_Size);

	memcpy(&[m_Index],&data,sizeof(T));
	m_Index += sizeof(T);
}

template<class T>
void CDataBlock::operator >> (T& data)
{
	ASSERT(!m_bIsStoring);
	ASSERT(m_Index + sizeof(T) < Max_DataBlock_Size);

	memcpy(&data,&m_pData[m_Index],sizeof(T));
	m_Index += sizeof(T);
}
*/
/*
void operator >> (CDataBlock& DataBlock,float& data)
{
	ASSERT(!DataBlock.m_bIsStoring);
	ASSERT(DataBlock.m_Index + sizeof(float) < DataBlock.Max_DataBlock_Size);

	memcpy(&data,&DataBlock.m_pData[DataBlock.m_Index],sizeof(float));
	DataBlock.m_Index += sizeof(float);
}*/