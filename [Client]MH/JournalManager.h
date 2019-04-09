// JournalManager.h: interface for the CJournalManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOURNALMANAGER_H__B018A9BD_6B03_48BF_8B38_2AD317960D1F__INCLUDED_)
#define AFX_JOURNALMANAGER_H__B018A9BD_6B03_48BF_8B38_2AD317960D1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define JOURNALMGR CJournalManager::GetInstance()

class CJournalManager  
{
	BOOL m_bListReceived;
	DWORD m_RefreshTime;
	
public:
	MAKESINGLETON(CJournalManager);

	CJournalManager();
	virtual ~CJournalManager();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void Init();
	void GetListSyn();
};

#endif // !defined(AFX_JOURNALMANAGER_H__B018A9BD_6B03_48BF_8B38_2AD317960D1F__INCLUDED_)
