// ShoutManager.h: interface for the CShoutManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOUTMANAGER_H__7803C0C0_490A_4269_A09C_660808ABAEBF__INCLUDED_)
#define AFX_SHOUTMANAGER_H__7803C0C0_490A_4269_A09C_660808ABAEBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SHOUTMGR CShoutManager::GetInstance()

class cPtrList;


class CShoutManager  
{
	CMemoryPoolTempl<SHOUTBASE>*		m_MsgPool;
	cPtrList		m_MsgList;	

	DWORD	m_lastbrodtime;



public:
	CShoutManager();
	virtual ~CShoutManager();
	GETINSTANCE(CShoutManager);


	void Process();
	void AddShoutMsg( SHOUTBASE* pSBase );
	BOOL AddShoutMsg( SHOUTBASE* pSBase, SHOUTRECEIVE* pSReceive );



};

#endif // !defined(AFX_SHOUTMANAGER_H__7803C0C0_490A_4269_A09C_660808ABAEBF__INCLUDED_)
