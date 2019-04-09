// TacticStartInfo.cpp: implementation of the CTacticInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticStartInfo.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "Hero.h"
#include "PartyManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTacticStartInfo::CTacticStartInfo(TACTICSTART_INFO* pInfo)
{
	memcpy(&m_Info,pInfo,sizeof(TACTICSTART_INFO));
}

CTacticStartInfo::~CTacticStartInfo()
{

}

BOOL CTacticStartInfo::IsTacticableState(CHero* pHero)
{
	if(pHero->GetState() != eObjectState_None && pHero->GetState() != eObjectState_Immortal)
		return FALSE;
	
	return TRUE;
}

BOOL CTacticStartInfo::IsTacticablePosition(CHero* pHero)
{
	float angleRad = pHero->GetAngle();
	// °¢µµ·Î µ¹·Á¼­ ÁÂÇ¥µéÀÌ ÀüºÎ ¿Ã¹Ù¸¥ Å¸ÀÏ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
	
	return TRUE;
}

WORD CTacticStartInfo::GetTacticID()
{
	return m_Info.TacticID;
}
DWORD CTacticStartInfo::GetLimitTime()
{
	return m_Info.WaitingTime;
}
BYTE CTacticStartInfo::GetTacticPlayerNum()
{
	return m_Info.NeedMemberNum;
}
WORD CTacticStartInfo::GetReduceNaeRyuk()
{
	return m_Info.WaitingNaeryuk;
}

int CTacticStartInfo::GetTacticStartEffectNum()
{
	return m_Info.TacticStartEffect;
}

void CTacticStartInfo::GetTacticMemberPosition(VECTOR3* pPos,TACTIC_TOTALINFO* pTotalInfo)
{
	VECTOR3 OriPos;
	pPos[0] = pTotalInfo->Pos;
	float AngleRad = DIRTORAD(pTotalInfo->Direction);
	for(int n=1;n<GetTacticPlayerNum();++n)
	{
		OriPos.x = m_Info.PosX[n-1];
		OriPos.y = 0;
		OriPos.z = m_Info.PosZ[n-1];
		
		RotateVectorAxisY(&pPos[n],&OriPos,AngleRad);
		pPos[n] = pPos[n] + pTotalInfo->Pos;
	}
}