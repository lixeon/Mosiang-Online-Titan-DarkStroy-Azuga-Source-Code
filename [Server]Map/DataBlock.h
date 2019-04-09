// Data.h: interface for the CDataBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__109F1FF8_6075_4BF1_AF3E_6E7E80D5EA56__INCLUDED_)
#define AFX_DATA_H__109F1FF8_6075_4BF1_AF3E_6E7E80D5EA56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataBlock  
{
	DWORD m_DataBlockNum;
	static DWORD Max_DataBlock_Size;
	BOOL m_bIsStoring;
	DWORD m_Index;
	char* m_pData;

	void Init(DWORD DataBlockNum,char* pData);
public:
	inline DWORD GetDataBlockNum()	{	return m_DataBlockNum;	}
	friend class CDataBlockManager;
	
	CDataBlock();
	virtual ~CDataBlock();

	void Clear(BOOL bIsStore);

	void AddData(void* pData,int DataSize);

	template<class T> void operator << (T& data)
	{
		AddData(&data,sizeof(T));
	}
	template<class T> void operator >> (T& data)
	{
		ASSERT(!m_bIsStoring);
		ASSERT(m_Index + sizeof(T) < Max_DataBlock_Size);
		
		memcpy(&data,&m_pData[m_Index],sizeof(T));
		m_Index += sizeof(T);
	}
	
	inline char* GetData()	{	return m_pData;		}
	inline int GetDataLen()	{	return m_Index;		}
	//friend void operator >> (CDataBlock& DataBlock,float& data);
};

#endif // !defined(AFX_DATA_H__109F1FF8_6075_4BF1_AF3E_6E7E80D5EA56__INCLUDED_)
