
/* 
	filename:	RegenNPC.h
	coder	:	Park Jung Hwan(ilil5@hanmail.net) 
	comp.	:	esofnet 
	compiler:	vc++ 6.0
	date	:	2002-11-12¿ÀÈÄ 3:57:59
	title	:	
*/

#ifndef __RegenNPC_h__
#define __RegenNPC_h__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Header\GameResourceManager.h"


#define START_MONSTERID		100000000
#define START_NPCID			300000000
#define REGENNPC_OBJ CRegenNPC::GetInstance()

class CRegenNPC
{
	
	DWORD	m_NpcObjectID;
	DWORD	m_MonsterObjectID;
	WORD	m_MonsterCurRegenCount;
	WORD	m_NpcCurRegenCount;
	DWORD	m_MonsterTotalRegenCount;
	DWORD	m_NpcTotalRegenCount;
	DWORD	m_dwCurTime;
	DWORD	m_dwTmpTime;
	DWORD	m_dwLastTime;
	BOOL	m_bFirstFlag;
	WORD	m_MaxMonsterRegenListCount;
	WORD	m_MaxNpcRegenListCount;

	WORD	m_MonsterGroupCount[MAX_MONSTER_GROUPNUM];
	WORD	m_NpcGroupCount[MAX_NPC_GROUPNUM];

public:
	// RegenList ÀúÀå
	NPC_REGEN m_MonsterRegen[MAX_MONSTER_GROUPNUM][MAX_MONSTER_REGEN_NUM];
	NPC_REGEN m_NpcRegen[MAX_MONSTER_GROUPNUM][MAX_NPC_REGEN_NUM];
	NPC_REGEN m_ObjectRegen[MAX_NPC_REGEN_NUM];

	GETINSTANCE(CRegenNPC)
	CRegenNPC();
	virtual ~CRegenNPC();
	
	//////////////////////////////////////////////////////////////////////////
	void MonsterRegen(int GroupNum);			// ¸ó½ºÅÍ ¸®Á¨
	void NpcRegen(int GroupNum);				// »ç¶÷npc ¸®Á¨
	void MerchantRegen(int GroupNum);			// »óÀÎnpc ¸®Á¨
	void ObjectNpcRegen();						// Àå½Änpc ¸®Á¨
	//////////////////////////////////////////////////////////////////////////
	void CheckProcess();
	//////////////////////////////////////////////////////////////////////////
	BOOL CheckMonsterRegen();
	BOOL CheckNpcRegen();
	//////////////////////////////////////////////////////////////////////////
	void AddMonsterObjectID(){m_MonsterObjectID++;}
	DWORD GetMonsterObjectID(){return m_MonsterObjectID;}
	void AddNpcObjectID(){m_NpcObjectID++;}
	DWORD GetNpcObjectID(){return m_NpcObjectID;}
	//////////////////////////////////////////////////////////////////////////
	void AddMonsterGroup(WORD Group){m_MonsterGroupCount[Group] += 1;}
	void AddNpcGroup(WORD Group){m_NpcGroupCount[Group] += 1;}
	void AddMonsterCurCount(){m_MonsterCurRegenCount++;}
	void AddNpcCurCount(){m_NpcCurRegenCount++;}
	void AddMonsterTotalCount(){m_MonsterTotalRegenCount++;}
	void AddNpcTotalCount(){m_NpcTotalRegenCount++;}

	void ReleaseMonsterGroup(WORD Group){m_MonsterGroupCount[Group] -= 1;}
	void ReleaseNpcGroup(WORD Group){m_NpcGroupCount[Group] -= 1;}
	void ReleaseMonsterCurCount(){m_MonsterCurRegenCount--;}
	void ReleaseNpcCurCount(BYTE Group){m_NpcCurRegenCount--;}

	WORD GetMonsterGroupCount(WORD Group){return m_MonsterGroupCount[Group];}
	WORD GetNpcGroupCount(WORD Group){return m_NpcGroupCount[Group];}
	WORD GetMonsterCurRegenCount(){return m_MonsterCurRegenCount;}
	WORD GetNpcGroupCount(){return m_NpcCurRegenCount;}
	DWORD GetMonsterTotalCount(){return m_MonsterTotalRegenCount;}
	DWORD GetNpcTotalCount(){return m_NpcTotalRegenCount;}

	void SetMaxMonsterRegenListCount(WORD count){m_MaxMonsterRegenListCount = count;}
	void SetMaxNpcRegenListCount(WORD count){m_MaxNpcRegenListCount = count;}
	WORD GetMaxMonsterRegenListCount(){return m_MaxMonsterRegenListCount;}
	WORD GetMaxNpcRegenListCount(){return m_MaxNpcRegenListCount;}
	
	NPC_REGEN* GetNpcRegenInfo(WORD index);
};
#endif //__RegenNPC_h__
