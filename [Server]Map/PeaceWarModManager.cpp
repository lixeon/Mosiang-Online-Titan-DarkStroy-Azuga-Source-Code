// PeaceWarModManager.cpp: implementation of the CPeaceWarModManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeaceWarModManager.h"
#include "CharMove.h"
#include "PackedData.h"
#include "..\[CC]Header\GameResourceManager.h"

#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPeaceWarModManager::CPeaceWarModManager()
{

}

CPeaceWarModManager::~CPeaceWarModManager()
{

}

void CPeaceWarModManager::Peace_WarMode(CPlayer* pPlayer, BOOL bPeace)
{
	pPlayer->SetPeaceMode(bPeace);

	MSGBASE msg;
	msg.Category = MP_PEACEWARMODE;
	msg.dwObjectID	 = pPlayer->GetID();
	if(bPeace ==  TRUE)
	{
		msg.Protocol = MP_PEACEWARMODE_PEACE;
	}
	else 
	{		
		msg.Protocol = MP_PEACEWARMODE_WAR;
	}
	
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,sizeof(msg));
}

void CPeaceWarModManager::Init_PeaceWarMode(CPlayer* pPlayer)
{
	WORD MapNum = GAMERESRCMNGR->GetLoadMapNum();

	//YH2DO
	if(MapNum == 15)
	{	//진시
		pPlayer->SetPeaceMode(FALSE);
		CCharMove::SetRunMode(pPlayer);
	}
	if(MapNum == 17)
	{	//난주
		pPlayer->SetPeaceMode(TRUE);
		CCharMove::SetRunMode(pPlayer);
	}
	else if(MapNum == 19)
	{	//옥문관
		pPlayer->SetPeaceMode(FALSE);
		CCharMove::SetRunMode(pPlayer);
	}


	return;

	if(MapNum<200)
	{
		pPlayer->SetPeaceMode(TRUE);
		CCharMove::SetRunMode(pPlayer);
	}
	else 
	{
		pPlayer->SetPeaceMode(FALSE);
		CCharMove::SetRunMode(pPlayer);
	}
}
