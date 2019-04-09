// MemoryChecker.h: interface for the CMemoryChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYCHECKER_H__D114ECD7_0A37_4E1F_9827_33DC812BCD42__INCLUDED_)
#define AFX_MEMORYCHECKER_H__D114ECD7_0A37_4E1F_9827_33DC812BCD42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <malloc.h>


#define MEMORYCHK CMemoryChecker::GetInstance()


class CMemoryChecker  
{
	CMemoryPoolTempl<stMemoryInfo>*		m_MemoryPool;
	CYHHashTable<stMemoryInfo>			m_MemoryTable;

	cPtrList		m_TempleteList;

	DWORD m_LastSaveTime;

public:
	CMemoryChecker();
	virtual ~CMemoryChecker();
	GETINSTANCE(CMemoryChecker);
	
	stMemoryInfo* GetMemoryInfo( DWORD Key );
//	void AddTemplete( CMemoryPoolTempl* pTplete );	

	void Process();
};

#endif // !defined(AFX_MEMORYCHECKER_H__D114ECD7_0A37_4E1F_9827_33DC812BCD42__INCLUDED_)
