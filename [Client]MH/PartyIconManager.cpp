// PartyIconManager.cpp: implementation of the PartyIconManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyIconManager.h"
#include "GameIn.h"
#include "MiniMapDlg.h"
#include "BigMapDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPartyIconManager)

CPartyIconManager::CPartyIconManager()
{
//	memset(m_MInfoIndex, 0, sizeof(DWORD)*(MAX_PARTY_LISTNUM-1));
//	memset(m_MoveInfo, 0, sizeof(PartyIconMoveInfo)*(MAX_PARTY_LISTNUM-1));

}

CPartyIconManager::~CPartyIconManager()
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		m_MInfoIndex[i] = 0;		
	}
}

void CPartyIconManager::CreatePartyIcon(DWORD ID, PARTY_MEMBER* pMember)
{
	// 기존에 이미 있을경우엔 위치만 수정해준다.
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			GAMEIN->GetMiniMapDialog()->SetPartyIconObjectPos(ID, pMember->posX, pMember->posZ);
			m_MoveInfo[i].SetPosition(pMember->posX, pMember->posZ);
			m_MoveInfo[i].SetIconPosition(pMember->posX, pMember->posZ);
			return;
		}
	}
	
	// 없으면 새로 추가해준다.
	for(i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == 0)
		{
			m_MInfoIndex[i] = ID;

			// Icon 초기위치 설정
			GAMEIN->GetMiniMapDialog()->AddPartyMemberIcon(pMember);
			GAMEIN->GetMiniMapDialog()->SetPartyIconObjectPos(ID, pMember->posX, pMember->posZ);
			m_MoveInfo[i].SetPosition(pMember->posX, pMember->posZ);
			m_MoveInfo[i].SetIconPosition(pMember->posX, pMember->posZ);

			break;
		}
	}
}

void CPartyIconManager::DeletePartyIcon(DWORD ID)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			GAMEIN->GetMiniMapDialog()->RemoveIcon(ID);
			GAMEIN->GetBigMapDialog()->RemoveIcon(ID);
			m_MInfoIndex[i] = 0;
			m_MoveInfo[i].Reset();
		}
	}
}

void CPartyIconManager::DeleteAllPartyIcon()
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i])
		{
			GAMEIN->GetMiniMapDialog()->RemoveIcon(m_MInfoIndex[i]);
			GAMEIN->GetBigMapDialog()->RemoveIcon(m_MInfoIndex[i]);
			m_MInfoIndex[i] = 0;
			m_MoveInfo[i].Reset();
		}
	}
}

void CPartyIconManager::SetMoveInfo(PARTYICON_MOVEINFO* pMoveInfo)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == pMoveInfo->dwMoverID)
		{
			m_MoveInfo[i].SetMoveInfo(pMoveInfo);			
			break;
		}
	}	
}


void CPartyIconManager::SetCurPosition(DWORD ID, MoveCoord Pos)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			m_MoveInfo[i].SetPosition(Pos.posX, Pos.posZ);
			m_MoveInfo[i].SetIconPosition(Pos.posX, Pos.posZ);			
			m_MoveInfo[i].Reset();
			GAMEIN->GetMiniMapDialog()->SetPartyIconObjectPos(ID, Pos.posX, Pos.posZ);
		}
	}
}


void CPartyIconManager::SetKyungGong(DWORD ID, int Index)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			m_MoveInfo[i].SetKoungGongIdx(Index);
			break;
		}
	}	
}


void CPartyIconManager::SetMoveState(DWORD ID, int State)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			m_MoveInfo[i].SetMoveState(State);
			break;
		}
	}
}

/*
void CPartyIconManager::MoveStop(DWORD ID)
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i] == ID)
		{
			m_MoveInfo[i].MoveStop();
			break;
		}
	}
}
*/

void CPartyIconManager::Process()
{
	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if(m_MInfoIndex[i])
		{
			m_MoveInfo[i].Process();
			GAMEIN->GetMiniMapDialog()->SetPartyIconTargetPos(m_MInfoIndex[i],
				 m_MoveInfo[i].GetIconPos().posX, m_MoveInfo[i].GetIconPos().posZ);
		}
	}
}


int CPartyIconManager::GetPartyMemberCountofMap()
{
	int count = 0;

	for(int i=0; i<MAX_PARTY_LISTNUM-1; i++)
	{
		if( m_MInfoIndex[i] )
            ++count;
	}

	return count;
}
