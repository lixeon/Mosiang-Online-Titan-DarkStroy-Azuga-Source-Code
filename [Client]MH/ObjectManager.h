// ObjectManager.h: interface for the CObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTMANAGER_H__90B9A123_674D_404D_AD96_AFBEEF1800A3__INCLUDED_)
#define AFX_OBJECTMANAGER_H__90B9A123_674D_404D_AD96_AFBEEF1800A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//class CHero;
#include "Player.h"
#include "Monster.h"
#include "Npc.h"
#include "TacticObject.h"
#include "BossMonster.h"
#include "Pet.h"
#include "PetBase.h"
#include "Titan.h"
#include "MapObject.h"

class CObject;
class CSkillArea;
class CSkillObject;

#include "PtrList.h"
#include "CharacterCalcManager.h"
#include "Hero.h"

#define OBJECTMGR USINGTON(CObjectManager)
#define HERO OBJECTMGR->GetHero()
#define HEROPET OBJECTMGR->GetHeroPet()
#define HEROTITAN	OBJECTMGR->GeTHeroTitan()
//#define BOSS001 OBJECTMGR->GetGetBoss()

//´©°¡Áö¿ü¾úÁö? Áö¿ìÁö ¸¶½Ã¿À!
enum eOPTION_OVERINFO
{
	eOO_NAME_NONE			= 0,
	eOO_NAME_CLICKEDONLY	= 1,
	eOO_NAME_WHOLE			= 2,
	eOO_NAME_PARTY			= 4,
	eOO_NAME_MUNPA			= 8,
	eOO_CHAT_NOBALLOON		= 16,
	eOO_DAMAGE_MEMBERS		= 32,
};

enum eOPTION_SHADOW
{
	eOO_SHADOW_NONE				= 0,
	eOO_SHADOW_ONLYHERO			= 1,
	eOO_SHADOW_MONSTER			= 2,
	eOO_SHADOW_OTHERCHARACTER	= 4,
};

//trustpak 2005/06/27

enum eOPTION_EFFECT
{
	eOO_EFFECT_DEFAULT			= 0,
	eOO_EFFECT_ONLYHERO			= 1,
	eOO_EFFECT_NOT_USE			= 2,
};

///


//중복
/////////
//enum eOPTION_WEEKDAY
//{
//	SUNDAY,
//	MONDAY,
//	TUESDAY,
//	WEDNESDAY,
//	THURSDAY,
//	FRIDAY,
//	SATURDAY,
//};
//////////
enum eWeekData{eWeekforGT=1, eWeekforSG=2,};	//flagValue..

class CObjectManager  
{

	CHero* m_pHero;
	CPet*	m_pHeroPet;
	CTitan* m_pHeroTitan;

	CBossMonster * m_pBoss[eBOSSTYPE_MAX];

	HEFFPROC m_hRestHandle[eBOSSTYPE_MAX];

	cPtrList m_GarbageObjectList;
	
	void RemoveObject(CObject** ppObject);
	CCharacterCalcManager * m_pCharCalcMgr;

	CYHHashTable<CObject> m_ObjectTable;

	CMemoryPoolTempl<CPlayer>		m_PlayerPool;
	CMemoryPoolTempl<CMonster>		m_MonsterPool;
	CMemoryPoolTempl<CNpc>			m_NpcPool;
	CMemoryPoolTempl<CTacticObject>	m_TacticObjectPool;
	CMemoryPoolTempl<CBossMonster>	m_BossMonsterPool;
	CMemoryPoolTempl<CPet>			m_PetPool;
//	CMemoryPoolTempl<CPetBase>		m_PetPool;
	CMemoryPoolTempl<CMapObject>	m_MapObjectPool;

	DWORD					m_SelectedObjectID;
	CObject					* m_pDPObjectName;

//---OverInfoOption
	DWORD		m_dwOverInfoOption;
	BOOL		m_bAllNameShow;
//---ShadowOption
	DWORD		m_dwShadowOption;

	//trustpak 2005/06/27
//---EffectOption
	DWORD		m_dwEffectOption;

	///

	//SW061205 깃발 NPC
	CNpc*		m_pCurGTFlg;	//문파
	CNpc*		m_pCurSGFlg;	//공성
	DWORD		m_dwWeekData;	//flag

public:
	//MAKESINGLETON(CObjectManager)
		
	CObjectManager();
	virtual ~CObjectManager();

	void RegisterHero(SEND_HERO_TOTALINFO * sendHeroTotalInfo);
#ifndef _RESOURCE_WORK_NOT_APPLY_
	CPlayer* AddPlayerThroughCache(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo, SHOPITEMOPTION* pShopItemOption, SEND_CHARACTER_TOTALINFO* pMsg);
	//CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption);
	CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption,TITAN_APPEARANCEINFO* pTitanAppearanceInfo = NULL,BOOL bInTitan = FALSE);
#else
	//CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption);
	CPlayer* AddPlayer(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,CHARACTER_TOTALINFO* pTotalInfo,SHOPITEMOPTION* pShopItemOption,TITAN_APPEARANCEINFO* pTitanAppearanceInfo = NULL,BOOL bInTitan = FALSE);
#endif

	CMonster* AddMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo);
	CBossMonster* AddBossMonster(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,MONSTER_TOTALINFO* pTotalInfo);
	CNpc* AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,BASEMOVE_INFO* pMoveInfo,NPC_TOTALINFO* pTotalInfo);
	CTacticObject* AddTacticObject(BASEOBJECT_INFO* pBaseObjectInfo,TACTIC_TOTALINFO* pTotalInfo);
	void AddSkillObject(CSkillObject* pSObj);
	//CPet* AddPet(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, PET_TOTALINFO* pTotalInfo, DWORD dwMasterIdx);
	CPet* AddPet(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, PET_TOTALINFO* pTotalInfo, char* pMasterName);
//	CPetBase* AddPet(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo );
	CMapObject* AddMapObject(BASEOBJECT_INFO* pBaseObjectInfo, BASEMOVE_INFO* pMoveInfo, char* strData, float fScale, float fAngle );

	CObject* GetObject(DWORD dwObjectID);
	
	void RemoveAllObject();
	
	void AddGarbageObject(CObject* pObject);
	void ProcessGarbage();

	void GetTargetInRange(VECTOR3* pPivotPos,float Radius,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK);
	void GetTargetInArea(CHero* pHero,CSkillArea* pSkillArea,ObjectArray<LPOBJECT>* pObjectArray,BOOL bPK);

	void UpdateTargetList_Monster(CSkillObject* pSkillObject);

	CObject*	GetSelectedObject();
	void		SetSelectedObject(CObject* pObject);
	DWORD		GetSelectedObjectID() { return m_SelectedObjectID; }
	void		SetSelectedObjectID(DWORD dwId);

////
	void		SetDPObject(CObject* pObject) { m_pDPObjectName = pObject; }
	CObject *	GetDPObject() { return m_pDPObjectName; }

	void OnMouseOver( CObject* pObject );
	void OnMouseLeave( CObject* pObject );

///---overinfo
	void SetOverInfoOption( DWORD dwOption );
	void ApplyOverInfoOption( CObject* pObject );
	void ApplyOverInfoOptionToAll();
	BOOL CanOverInfoChange( CObject* pObject );
	DWORD GetObjectNameColor( CObject* pObject );
	void ShowChatBalloon( CObject* pObject, char* pStrChat, DWORD dwColor = RGB_HALF(70,70,70), DWORD dwAliveTime = 5000 );
	void HideChatBalloonAll();
	void SetAllNameShow( BOOL bShow );

//---Shadow
	void SetShadowOption( DWORD dwOption );
	void ApplyShadowOption( CObject* pObject );
	void ApplyShadowOptionToAll();
	void AddShadow(CObject* pObject);
	void RemoveShadow(CObject* pObject);


	//trustpak 2005/06/27
//---Effect
	void SetEffectOption( DWORD dwOption )			{ m_dwEffectOption = dwOption; }
	DWORD GetEffectOption( VOID )					{ return m_dwEffectOption; }
	///	

//---pk mode
	void AllPlayerPKOff();
	void RefreshSiegeNamePannel();
//
	inline CHero* GetHero()	{	return m_pHero;	}
	inline CBossMonster * GetGetBoss(eBOSSMONSTERTYPE eBossType)	{	return m_pBoss[eBossType];	}
	void SetHeroPet(CPet* pPet)	{	m_pHeroPet = pPet;	}
	CPet* GetHeroPet()		{	return m_pHeroPet;	}

	//SW070127 타이탄
	inline CTitan* GetTitan()	{	return m_pHeroTitan;	}

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	// 06. 02. 타겟팅 - 이영준
	BOOL TargetByName(const char* name);

	CPet* FindPlayerOwnPet(DWORD CharacterIdx);

	// 06. 05 HIDE NPC - 이영준
	void HideNpc(WORD UniqueIdx, BOOL bOn);

	///////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	void ApplySingleSpecialState(CObject* pObject, WORD State, BOOL bVal);
	void SetHide(CObject* pObject, BOOL bVal);
	void SetDetect(CObject* pObject, BOOL bVal);
	///////////////////////////////////////////////////////////////////

	//061009 BB
	//요일별 깃발 바꾸기

	void ChangeFlag( BYTE Category, WORD NpcKind );

	//SW061205 깃발 NPC
	void ChangeFlagNPC(int flagKind, DWORD flagIdx);
	void SetWeekData(int flg) {	m_dwWeekData = flg;	}
	BOOL Weekfor(int flg)	{	return m_dwWeekData & flg;	}

	CMapObject* AddDecoration(CPlayer* pPlayer, DWORD dwStreetStallKind);
	void SetRemoveDecorationInfo(CPlayer* pPlayer);
	void RemoveDecoration(CPlayer* pPlayer);

};
EXTERNGLOBALTON(CObjectManager);


#endif // !defined(AFX_OBJECTMANAGER_H__90B9A123_674D_404D_AD96_AFBEEF1800A3__INCLUDED_)
