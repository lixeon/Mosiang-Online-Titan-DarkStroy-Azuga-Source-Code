// TacticStartInfo.cpp: implementation of the CTacticStartInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TacticStartInfo.h"
#include "Party.h"
#ifdef _MAPSERVER_
#include "SkillManager_server.h"
#else
#include "SkillManager_client.h"
#endif


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

BOOL CTacticStartInfo::IsTacticableState(CPlayer* pPlayer)
{
	if(pPlayer->GetState() != eObjectState_None)
		return FALSE;
	
	return TRUE;
}

BOOL CTacticStartInfo::IsTacticablePosition(CPlayer* pPlayer,float AngleDeg)
{
	// °¢µµ·Î µ¹·Á¼­ ÁÂÇ¥µéÀÌ ÀüºÎ ¿Ã¹Ù¸¥ Å¸ÀÏ¾È¿¡ ÀÖ´ÂÁö °Ë»ç
	
	return TRUE;
}

WORD CTacticStartInfo::GetTacticID()
{
	return m_Info.TacticID;
}
char* CTacticStartInfo::GetTacticName()
{
	return m_Info.Name;
}
DWORD CTacticStartInfo::GetLimitTime()
{
	return m_Info.WaitingTime;
}
WORD CTacticStartInfo::GetTacticPlayerNum()
{
	return m_Info.NeedMemberNum;
}
WORD CTacticStartInfo::GetReduceNaeRyuk()
{
	return m_Info.WaitingNaeryuk;
}
WORD CTacticStartInfo::GetSuccessNaeRyuk(int TacticLevel)
{
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(m_Info.TacticID);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return 1000;

	return pSkillInfo->GetNeedNaeRyuk(TacticLevel);
}

int CTacticStartInfo::GetTacticStartEffectNum()
{
	return m_Info.TacticStartEffect;
}

void CTacticStartInfo::GetTacticMemberPosition(VECTOR3* pPos,TACTIC_TOTALINFO* pTotalInfo)
{
	VECTOR3 OriPos;
	for(int n=0;n<GetTacticPlayerNum();++n)
	{
		OriPos.x = m_Info.PosX[n];
		OriPos.y = 0;
		OriPos.z = m_Info.PosZ[n];
		
		RotateVectorAxisY(&pPos[n],&OriPos,DIRTORAD(pTotalInfo->Direction));
		pPos[n] = pPos[n] + pTotalInfo->Pos;
	}
}