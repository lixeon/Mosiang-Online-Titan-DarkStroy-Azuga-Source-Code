// JournalManager.h: interface for the CJournalManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOURNALMANAGER_H__FEB231D5_C485_4C89_A7A6_056B33D5E283__INCLUDED_)
#define AFX_JOURNALMANAGER_H__FEB231D5_C485_4C89_A7A6_056B33D5E283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define JOURNALMGR CJournalManager::GetInstance()

class CJournalManager  
{
public:
	MAKESINGLETON(CJournalManager);
	
	CJournalManager();
	virtual ~CJournalManager();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

};

#endif // !defined(AFX_JOURNALMANAGER_H__FEB231D5_C485_4C89_A7A6_056B33D5E283__INCLUDED_)
