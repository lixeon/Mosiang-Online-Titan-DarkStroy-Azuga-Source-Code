// TacticManager.h: interface for the CTacticManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICMANAGER_H__B9AA99C7_F184_4721_A08A_783DD67A811A__INCLUDED_)
#define AFX_TACTICMANAGER_H__B9AA99C7_F184_4721_A08A_783DD67A811A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TacticObject.h"
#include "TacticStartInfo.h"
class CPlayer;

#define TACTICMGR USINGTON(CTacticManager)

class CTacticManager  
{
	CYHHashTable<CTacticStartInfo> m_TacticInfoTable;

public:
	//MAKESINGLETON(CTacticManager);
	CTacticManager();
	virtual ~CTacticManager();

	void AddTacticStartInfo(TACTICSTART_INFO* pList);
	CTacticStartInfo* GetTacticStartInfo(WORD TacticID);

	void HeroTacticStart(WORD TacticID);			// ÁÖÀÎ°ø ½ÃÀü
	void HeroTacticJoin();

	void OnFailedTacticStart(DWORD FailedReason);
	void OnFailedTacticJoin();

	void OnTacticJoinAcked(SEND_TACTIC_JOIN* pmsg);

	void RegistTacticObject(SEND_TACTIC_TOTALINFO* sendtacticinfo);
	CTacticObject* GetTacticObject(DWORD TacticObjectID);
	
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};
EXTERNGLOBALTON(CTacticManager);
#endif // !defined(AFX_TACTICMANAGER_H__B9AA99C7_F184_4721_A08A_783DD67A811A__INCLUDED_)
