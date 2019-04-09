// MsgTable.h: interface for the CMsgTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTABLE_H__7AD9135A_5FE2_48B3_8798_2CE252A57E74__INCLUDED_)
#define AFX_MSGTABLE_H__7AD9135A_5FE2_48B3_8798_2CE252A57E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "PtrList.h"


class CMsgTable  
{
	CMemoryPoolTempl<MSG_CHAT>		m_mpMsg;	
	CYHHashTable<MSG_CHAT>			m_htMsg;

	INDEXCR_HANDLE		m_ICHandle;

public:
	CMsgTable();
	virtual ~CMsgTable();

	BOOL AddMsg( MSG_CHAT* pMsg, DWORD* pdwKeyOut );
	BOOL AddMsg( TESTMSG* pMsg, DWORD* pdwKeyOut );
	void RemoveMsg( DWORD dwKey );
	MSG_CHAT* GetMsg( DWORD dwKey ) { return m_htMsg.GetData( dwKey ); }


};

extern CMsgTable g_MsgTable;
#endif // !defined(AFX_MSGTABLE_H__7AD9135A_5FE2_48B3_8798_2CE252A57E74__INCLUDED_)
