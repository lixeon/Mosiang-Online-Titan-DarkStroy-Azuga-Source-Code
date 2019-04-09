// AppearanceManager.cpp: implementation of the CAppearanceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#include "AppearanceManager.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"
//#include "Player.h"
//#include "Monster.h"
//#include "Npc.h"
//#include "ItemGround.h"
#include ".\Engine\EngineObject.h"
#include "LoadList.h"
//#include "ObjectManager.h"
//#include "GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppearanceManager::CAppearanceManager()
{

}

CAppearanceManager::~CAppearanceManager()
{

}

void CAppearanceManager::Init()
{	
//	g_pLoadList->LoadModFileList(g_pLoadList->m_ModFileList);
//	g_pLoadList->LoadHairModFileList(g_pLoadList->m_HairModList);
}

void CAppearanceManager::Release()
{
}

void CAppearanceManager::SetCharacterAppearance(CPlayer* pPlayer)
{
	/*
	BYTE gender = pPlayer->m_CharacterInfo.Gender;
	ASSERT(gender < 2);
	BYTE job = pPlayer->m_CharacterInfo.Job;
	ASSERT(job < eJOB_Max);
	BYTE hair = pPlayer->m_CharacterInfo.HairType;
	BYTE face = pPlayer->m_CharacterInfo.FaceType;
	WORD *WearedItemIdx = pPlayer->m_CharacterInfo.WearedItemIdx;
	MOD_LIST* pHairList = &GAMERESRCMNGR->m_HairModList[gender];
	MOD_LIST* pModList = &GAMERESRCMNGR->m_ModFileList[gender][job];


	ASSERT(pHairList->MaxModFile > hair);


	pPlayer->m_EngineObject.Release();	

	pPlayer->m_EngineObject.Init(pModList->BaseObjectFile,pPlayer,eEngineObjectType_Character);

	pPlayer->m_EngineObject.ChangePart(eAppearPart_Hair,pHairList->ModFile[hair]);

	int PartType,PartModelNum;
	for(int n=0;n<eWearedItem_Max;++n)
	{
		if(WearedItemIdx[n])
		{
			PartType = GAMERESRCMNGR->GetEquipItemInfo(WearedItemIdx[n])->Part3DType;
			PartModelNum = GAMERESRCMNGR->GetEquipItemInfo(WearedItemIdx[n])->Part3DModelNum;
			if(PartType != -1)
				pPlayer->m_EngineObject.ChangePart(PartType,pModList->ModFile[PartModelNum]);
		}
	}
	*/
	/*
	//for dummy test

	pPlayer->m_EngineObject.Release();	

	pPlayer->m_EngineObject.Init("dummyman.chx",pPlayer,eEngineObjectType_Character);
	CEngineObject* pWeapon = new CEngineObject;
	pWeapon->Init("weapon.chr",NULL,eEngineObjectType_Weapon);
	BOOL rt = pWeapon->Attach(&pPlayer->m_EngineObject,"Bone29");
	ASSERT(rt);
*/

//	pPlayer->m_EngineObject.ApplyHeightField(TRUE);
//	pPlayer->m_EngineObject.SetPosition(&pPlayer->m_MoveInfo.CurPosition);
//	pPlayer->m_EngineObject.SetAngle(pPlayer->m_RotateInfo.Angle.ToRad());

}

void CAppearanceManager::SetMonsterAppearance(WORD MonsterKind)
{
/*	pMonster->m_EngineObject.Release();
	
	WORD mkind = pMonster->GetMonsterKind();
	MONSTER_LIST* pList = g_pLoadList->GetMonsterInfo(mkind);
	int ChxNum = pList->D3Num;
	char* pMonsterChxName = g_pLoadList->GetMonsterChxName(ChxNum);
	pMonster->m_EngineObject.Init(pMonsterChxName,pMonster,eEngineObjectType_Monster);
	pMonster->m_EngineObject.ApplyHeightField(TRUE);
*/	
}
void CAppearanceManager::SetNpcAppearance(CNpc* pNpc)
{
/*	pNpc->m_EngineObject.Release();
	
	
	WORD nkind = pNpc->GetNpcKind();
	NPC_LIST* pList = GAMERESRCMNGR->GetNpcInfo(nkind);
	int ChxNum = pList->ModelNum;
	char* pMonsterChxName = GAMERESRCMNGR->GetMonsterChxName(ChxNum);
	
	pNpc->m_EngineObject.Init(pMonsterChxName,pNpc,eEngineObjectType_Npc);
	pNpc->m_EngineObject.SetPosition(&pNpc->m_MoveInfo.CurPosition);
	pNpc->m_EngineObject.ApplyHeightField(TRUE);
	
	pNpc->m_EngineObject.ChangeMotion(1);
*/	
}

void CAppearanceManager::InitAppearance(CObject* pObject)
{
/*	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		SetCharacterAppearance((CPlayer*)pObject);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Monster)
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Npc)
	{
		SetNpcAppearance((CNpc*)pObject);
	}
*/
}


void CAppearanceManager::AddCharacterPartChange(DWORD PlayerID)
{
//	m_PtrList.AddHead((void*)PlayerID);
}

void CAppearanceManager::ProcessAppearance()
{
/*	DWORD PlayerID;
	CObject* pPlayer;
	while(PlayerID = (DWORD)m_PtrList.RemoveTail())
	{
		pPlayer = OBJECTMGR->GetObject(PlayerID);
		if(pPlayer == NULL)
			continue;
		ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
		SetCharacterAppearance((CPlayer*)pPlayer);
	}
*/
}
/*
void CAppearanceManager::ChangeCharacterPart(APPEAR_CHANGE* pAC)
{
	//////////////////////////////////////////////////////////////////////////
	// test
	static DWORD TEST[eAppearPart_Max] = {1,1,1,1,1,1};
	
	ASSERT(pAC);
	static char temp[64];
	BOOL rt = GetPartName(pAC->AppearPart,TEST[pAC->AppearPart]++,temp);
	ASSERT(rt);

	if(TEST[pAC->AppearPart] > 3)
		TEST[pAC->AppearPart] = 1;
	
	rt = pAC->pObject->GetEngineObject()->ChangePart(pAC->AppearPart,temp);
	ASSERT(rt);


	//////////////////////////////////////////////////////////////////////////
}

BOOL CAppearanceManager::GetPartName(DWORD AppearPart,DWORD PartNum,char* RtFileName)
{
	switch(AppearPart)
	{
	case eAppearPart_Hair:
		sprintf(RtFileName,"M_Hair%02d.mod",PartNum);
		break;
	case eAppearPart_Face:
		sprintf(RtFileName,"M_Face%02d.mod",PartNum);
		break;
	case eAppearPart_Body:
		sprintf(RtFileName,"M_Bd%02d.mod",PartNum);
		break;
	case eAppearPart_ForeArm:
		sprintf(RtFileName,"C_HandGard%02d.mod",PartNum);
		break;
	case eAppearPart_Hand:
		sprintf(RtFileName,"C_Hand%02d.mod",PartNum);
		break;
	case eAppearPart_Foot:
		sprintf(RtFileName,"C_Shoes%02d.mod",PartNum);
		break;
	}

	return TRUE;
}
*/

