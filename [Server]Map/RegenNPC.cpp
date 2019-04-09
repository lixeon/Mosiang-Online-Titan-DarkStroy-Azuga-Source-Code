
/* 
	filename:	RegenNPC.cpp
	coder	:	Park Jung Hwan(ilil5@hanmail.net) 
	comp.	:	esofnet 
	compiler:	vc++ 6.0
	date	:	2002-11-12¿ÀÈÄ 3:58:18
	title	:	
*/
#include "stdafx.h"
#include "RegenNPC.h"
#include "Npc.h"

CRegenNPC::CRegenNPC()
{
	m_NpcObjectID = START_NPCID;
	m_MonsterObjectID = START_MONSTERID;
	m_dwCurTime = 0;
	m_dwTmpTime = 0;
	m_dwLastTime = 0;
	m_bFirstFlag = TRUE;

	memset(m_MonsterRegen, 0, sizeof(NPC_REGEN*)*MAX_MONSTER_GROUPNUM*MAX_MONSTER_REGEN_NUM);
	memset(m_NpcRegen, 0, sizeof(NPC_REGEN*)*MAX_MONSTER_GROUPNUM*MAX_NPC_REGEN_NUM);
}

CRegenNPC::~CRegenNPC()
{
	
}

void CRegenNPC::MonsterRegen(int GroupNum)
{
	// ¸÷À» ¸®Á¨ÇÑ´Ù
/*
	MONSTER_TOTALINFO Monsterinfo;
	BASEOBJECT_INFO Baseinfo;
	MONSTER_LIST *Monsterlist;

	for(int i = 0; i < MAX_MONSTER_REGEN_NUM; i++)
	{
		//NPC_REGEN * tmpNPC = GAMERESRCMNGR->m_MonsterRegen[GroupNum];
		//NPC_REGEN* pMonster = &tmpNPC[i];
		NPC_REGEN* pMonster = &m_MonsterRegen[GroupNum][i];
		if(pMonster->NpcKind == 0)
			continue;

		// ¸¸¾à Ãæµ¹ Å¸ÀÏ¾È¿¡¼­ »ý¼º µÇ¸é...
		VECTOR3 temppos;
		if(g_pServerSystem->GetMap()->CollisionCheck(&pMonster->Pos,&pMonster->Pos,&temppos) == TRUE)
		{
#ifdef TAIWAN_LOCAL
			g_Console.LOG(4, "Regen Failed : %s", pMonster->Name);
#else
			g_Console.LOG(4, "Regen ½ÇÆÐ : %s", pMonster->Name);
#endif
			continue;
		}
		//list
		Monsterlist = GAMERESRCMNGR->GetMonsterListInfo(pMonster->NpcKind);
		AddMonsterObjectID();
		Baseinfo.dwObjectID = GetMonsterObjectID();
		strcpy(Baseinfo.ObjectName, Monsterlist->Name);
		Monsterinfo.Life = Monsterlist->Life;
		Monsterinfo.MonsterKind = pMonster->NpcKind;
		Monsterinfo.Group = GroupNum;
		
		AddMonsterGroup(GroupNum);	// ±×·ì¹è¿­¿¡ Ä«¿îÅÍ¸¦ Ãß°¡ÇÑ´Ù
		g_pServerSystem->AddMonster(&Baseinfo, &Monsterinfo, &pMonster->Pos);
		g_Console.LOG(4, "Mop Regen: %s", Monsterlist->Name );


	}*/
}

void CRegenNPC::NpcRegen(int GroupNum)
{
	// »ç¶÷ npc¸¦ ¸®Á¨ÇÑ´Ù
	NPC_TOTALINFO Npcinfo;
	BASEOBJECT_INFO Baseinfo;
	NPC_LIST* Npclist;

	for(int i = 0; i < MAX_NPC_REGEN_NUM; i++)
	{
		NPC_REGEN* pNpc = &m_NpcRegen[GroupNum][i]; 
		if(pNpc->NpcKind == 0)
			continue;

		// ¸¸¾à Ãæµ¹ Å¸ÀÏ¾È¿¡¼­ »ý¼º µÇ¸é...
		
		MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
		if(g_pServerSystem->GetMap()->CollisionTilePos(pNpc->Pos.x,pNpc->Pos.z,MapNum) == TRUE)
		{
			g_Console.LOG(4, "NPC Regen Failed... %s", pNpc->Name);
			continue;
		}

		Npclist = GAMERESRCMNGR->GetNpcInfo(pNpc->NpcKind);
		AddNpcObjectID();
		Baseinfo.dwObjectID = GetNpcObjectID();
		Npcinfo.MapNum = pNpc->MapNum;
		//strcpy(Baseinfo.ObjectName, Npclist->Name);
		SafeStrCpy(Baseinfo.ObjectName, Npclist->Name, MAX_NAME_LENGTH+1);
		Npcinfo.Group = GroupNum;
		Npcinfo.NpcKind = pNpc->NpcKind;
		Npcinfo.NpcJob = Npclist->JobKind;
		Npcinfo.NpcUniqueIdx = pNpc->NpcIndex;
		AddNpcGroup(GroupNum);	// ±×·ì¹è¿­¿¡ Ä«¿îÅÍ¸¦ Ãß°¡ÇÑ´Ù
		CNpc* pAddNpc = g_pServerSystem->AddNpc(&Baseinfo, &Npcinfo, &pNpc->Pos);
		pAddNpc->SetNpcAngle(pNpc->Angle);
		
	}

}

void CRegenNPC::MerchantRegen(int GroupNum)
{
	// »óÀÎÀ» ¸®Á¨ÇÑ´Ù
}

void CRegenNPC::ObjectNpcRegen()
{
/*	// Àå½Ä npc¸¦ ¸®Á¨ÇÑ´Ù
	NPC_TOTALINFO Npcinfo;
	BASEOBJECT_INFO Baseinfo;
	NPC_LIST* Npclist;
	MAPCHANGE_INFO* pChangeMapInfo;
	for(int i = 0; i < MAX_NPC_REGEN_NUM; i++)
	{
		NPC_REGEN* pNpc = &m_ObjectRegen[i]; 
		if(pNpc->NpcKind == 0)
			continue;

		// ¸¸¾à Ãæµ¹ Å¸ÀÏ¾È¿¡¼­ »ý¼º µÇ¸é...
		VECTOR3 temppos;
		if(g_pServerSystem->GetMap()->CollisionCheck(&pNpc->Pos,&pNpc->Pos,&temppos) == TRUE)
		{
			g_Console.LOG(4, "NPC Regen Failed... %s", pNpc->Name);
			continue;
		}

		Npclist = GAMERESRCMNGR->GetNpcInfo(pNpc->NpcKind);
		AddNpcObjectID();
		Baseinfo.dwObjectID = GetNpcObjectID();
		Npcinfo.MapNum = pNpc->MapNum;
		strcpy(Baseinfo.ObjectName, Npclist->Name);
		Npcinfo.Group = GroupNum;
		Npcinfo.NpcKind = pNpc->NpcKind;
		Npcinfo.NpcJob = Npclist->JobKind;		
		Npcinfo.NpcUniqueIdx = pNpc->NpcIndex;
		AddNpcGroup(GroupNum);	// ±×·ì¹è¿­¿¡ Ä«¿îÅÍ¸¦ Ãß°¡ÇÑ´Ù
		CNpc* pAddNpc = g_pServerSystem->AddNpc(&Baseinfo, &Npcinfo, &pNpc->Pos);
		pAddNpc->SetNpcAngle(pNpc->Angle);
		
	}
*/
}

void CRegenNPC::CheckProcess()
{
	// ÀÏÁ¤½Ã°£¸¶´Ù µé¾î¿Í¼­ ½ÇÇàµÇ´Â ÇÔ¼ö
	if(m_bFirstFlag)
	{
		m_dwCurTime = m_dwTmpTime = m_dwLastTime = gCurTime;
		m_bFirstFlag = FALSE;
	//	CheckMonsterRegen();
		CheckNpcRegen();
	}
	else
	{
		m_dwCurTime = gCurTime;
		m_dwTmpTime = m_dwCurTime - m_dwLastTime;
		if(m_dwTmpTime > 1000 * 10)
		{
//			CheckMonsterRegen();
//			CheckNpcRegen();
			m_dwLastTime = m_dwCurTime;
		}
	}
}

BOOL CRegenNPC::CheckMonsterRegen()
{
	// MonseterÀÇ ¸®Á¨ ±ÔÄ¢¿¡ µû¶ó Ã¼Å©ÈÄ ´Ù½Ã »Ñ¸²
	for(WORD i = 0; i < MAX_MONSTER_GROUPNUM; i++)
	{
		WORD MonsterCount = GetMonsterGroupCount(i);
		if(MonsterCount < 1)
		{
			// ±×·ì¿¡¼­ ³²Àº ¸ó½ºÅÍ°¡ 3ÀÌÇÏÀÌ¸é ´Ù½Ã»Ñ¸°´Ù.
			MonsterRegen(i);
		}
	}
	return TRUE;
}

BOOL CRegenNPC::CheckNpcRegen()
{
	// »ç¶÷ NPCÀÇ ¸®Á¨ ±ÔÄ¢¿¡ µû¶ó Ã¼Å©ÈÄ ´Ù½Ã »Ñ¸²
	for(WORD i = 0; i < MAX_NPC_GROUPNUM; i++)
	{
		// ±×·ì¿¡¼­ ³²Àº NPC°¡ 3ÀÌÇÏÀÌ¸é ´Ù½Ã»Ñ¸°´Ù.
		WORD NpcCount = GetNpcGroupCount(i);
		if(NpcCount < 1)
		{
			NpcRegen(i);
		}
		
	}

	return TRUE;
}

NPC_REGEN* CRegenNPC::GetNpcRegenInfo(WORD index)
{
	for(WORD i = 0; i < MAX_NPC_GROUPNUM; i++)
	{
		for(WORD j = 0; j < MAX_NPC_REGEN_NUM; j++)
		{
			if(m_NpcRegen[i][j].NpcIndex == index)
				return &m_NpcRegen[i][j];
		}
	}
	return NULL;
}