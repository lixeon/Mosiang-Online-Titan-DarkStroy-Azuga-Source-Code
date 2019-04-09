// PartyIconManager.h: interface for the PartyIconManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYICONMANAGER_H__421EE703_830B_4120_90BC_20663DA263E2__INCLUDED_)
#define AFX_PARTYICONMANAGER_H__421EE703_830B_4120_90BC_20663DA263E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PartyIconMoveInfo.h"

#define PICONMGR USINGTON(CPartyIconManager)


class CPartyIconManager
{
	DWORD m_MInfoIndex[MAX_PARTY_LISTNUM-1];
	CPartyIconMoveInfo	m_MoveInfo[MAX_PARTY_LISTNUM-1];


public:
	CPartyIconManager();
	virtual ~CPartyIconManager();

	void Process();
	void CreatePartyIcon(DWORD ID, PARTY_MEMBER* pMember);
	void DeletePartyIcon(DWORD ID);
	void DeleteAllPartyIcon();
	void SetMoveInfo(PARTYICON_MOVEINFO* pMoveInfo);
	void SetCurPosition(DWORD ID, MoveCoord Pos);
	void SetMoveState(DWORD ID, int State);
	void SetKyungGong(DWORD ID, int Index);

	int	GetPartyMemberCountofMap();
};

EXTERNGLOBALTON(CPartyIconManager)

#endif // !defined(AFX_PARTYICONMANAGER_H__421EE703_830B_4120_90BC_20663DA263E2__INCLUDED_)
