// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_)
#define AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"


class CNpc : public CObject  
{
	NPC_TOTALINFO m_NpcInfo;
	float m_Angle;
	DWORD m_FirstTalkCharacterID;
	DWORD m_DieTime;
public:
	CNpc();
	virtual ~CNpc();
	virtual void DoDie(CObject* pAttacker);
	virtual void OnEndObjectState(BYTE State);
	virtual void Release();
	void InitNpc(NPC_TOTALINFO* pTotalInfo);
	void GetNpcTotalInfo(NPC_TOTALINFO* pRtInfo);

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	void SetNpcAngle(float Angle){m_Angle = Angle;}
	float GetNpcAngle(){return m_Angle;}
	void SetFirstTalkCharacterID(DWORD CharacterIdx);
	DWORD GetFirstTalkCharacterID();
	void SetDieTime(DWORD time);
	void Process();
	WORD GetNpcJob();
	WORD GetUniqueIdx();
	
	void DoJob(CPlayer* pPlayer, DWORD ConnectIdx);
	
	void MapChangeRole(CPlayer* pPlayer, DWORD dwConnectionIndex);
	void BomulRole(CPlayer* pPlayer);
	
	void SendNackMsg(CPlayer* pPlayer, BYTE err);
	///////////////////////////////////////
	// Npc Çàµ¿ÇÔ¼öµé


	void AddChat(char * str);
	//////////////////////////////////////////////////////////////////////////
	// Á¤º¸ ¾ò¾î¿À´Â ÇÔ¼öµé
	virtual LEVELTYPE GetLevel()		{	return 0;	}
};

#endif // !defined(AFX_NPC_H__0E71DC08_2E3F_440C_8EB6_7A9D6D0F8CF3__INCLUDED_)
