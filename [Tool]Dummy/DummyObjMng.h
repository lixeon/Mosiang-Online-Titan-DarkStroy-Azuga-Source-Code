// DummyObjMng.h: interface for the CDummyObjMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMMYOBJMNG_H__ED6153B9_0B7A_42FC_97B0_B9112657B395__INCLUDED_)
#define AFX_DUMMYOBJMNG_H__ED6153B9_0B7A_42FC_97B0_B9112657B395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MAX_ACTION_NUM	20

#include "DummyNetwork.h"
#include "ResourceMng.h"

enum eDUMMYOBJSTATE
{
	eDUMMYOBJSTATE_Alive = 0,
	eDUMMYOBJSTATE_Die,
};

enum eACTION
{
	eACTION_Revive = 0,
	eACTION_Peace,
	eACTION_Run,
	eACTION_Ungi,
	eACTION_MapChange,
	eACTION_SaveLogin,	
	eACTION_Hunt,
};

// dummy obj °ü·Ã ¸ðµç Ã³¸®¸¦ ÇÑ´Ù.
class CDummyObjMng  
{
protected:
	CDummyNetwork*			m_pNetwork;
	DWORD					m_dwHeroID;

	int						m_nObjectNum;		// »ý¼ºµÈ Ä³¸¯ÅÍ ¼ö(0~3)

	// hero info
	SEND_HERO_TOTALINFO		m_HeroTotalInfo;
	BASEOBJECT_INFO*		m_pHeroBaseObjInfo;
	CHARACTER_TOTALINFO*	m_pHeroChrTotalInfo;
	HERO_TOTALINFO*			m_pHeroTotalInfo;
	SEND_MOVEINFO*			m_pSendMoveInfo;
	MOVE_INFO				m_HeroMoveInfo;
//	MUGONG_TOTALINFO*		m_pHeroMugongTotalInfo;
//	ITEM_TOTALINFO*			m_pItemTotalInfo;
//	BASEMOVE_INFO*			m_pHeroBaseMoveInfo;
	eDUMMYOBJSTATE			m_DummyObjState;

	SEND_MONSTER_TOTALINFO	m_MonsterInfo;
		
	// map
	MAPDESC					m_MapDesc;
	DWORD					m_dwCurMapNum;	
	DWORD					m_dwToMap;
	DWORD					m_dwToMapNum;

	// action
	DWORD					m_dwActionStartTime;
	DWORD					m_dwActionEstimateTime;

	// move
	float					m_fMoveSpeed;
	VECTOR3					m_vOldPosition;
	int						m_nMoveFlag;

	// chatting
	char					m_sChat[256];

public:
	BOOL					m_bEnableMapChange;		// ¿ÜºÎ controlº¯¼ö
	BOOL					m_bEnableAttack;
	BOOL					m_bEnableChat;
		
public:
	CDummyObjMng();
	virtual ~CDummyObjMng();

	void	Init( int id, CDummyNetwork* pNetwork );
	void	Exit();
	void	Process();

	int		m_nID;

public:
	void	SetObjectNum( int num )							{ m_nObjectNum = num; }
	int		GetObjectNum();
	void	SetMapNum( DWORD mapnum );
	void	SetHeroTotalInfo( SEND_HERO_TOTALINFO info );	
	void	SetDummyObjStateDie( DWORD id );		
	void	SetLevelUp( int level );
	void	SetExp( int exp, int flag );	// flag 1 = get, 0 = loss
	void	SetMoney( int money, int flag );	// flag 0 = ¾ò±â, 1 = »ç¿ë, 2 = ÀÒ±â
	void	SetItem();
	
public:
	int		SelectAction();				// action ¼±ÅÃ
	void	ExeAction( int action );	// ½ÇÁ¦·Î action func ½ÇÇà
	
	// action
	// hero state flag
	BOOL	m_bPeace;
	BOOL	m_bUngi;	
	BOOL	m_bRun;
	BOOL	m_bMapChange;
	BOOL	m_bLogin;
	BOOL	m_bHunt;
	BOOL	m_bRevive;
	BOOL	m_bMustUngi;
	BOOL	m_bFirstMove;
	BOOL	m_bForcedToMapChange;

	int		m_nSkillNum;
	int		m_nHuntCount;
	int		m_nMaxHuntCount;

	void	ActionPeaceWarMode();
	void	ActionUngiMode();
	void	ActionRunMode();
	void	ActionMapChange();
	void	ActionSaveLogin();

	void	ActionMoveToMapchangePoint();
	void	ActionMoveToLoginPoint();
	void	ActionMoveToMonster();
	void	ActionMove();
	void	ActionFollowMonster();

	void	ActionRevive();
	void	ActionSkill();

	void	ActionChatting( char* str );

	// move
	void	CalMapChangePosition();
	void	CalLoginPosition();
	void	CalMonsterPosition();
	void	CalMovePosition();
	void	CalEstimateTime();

	void	ForcedToMapChange( int mapnum );

	BOOL	m_bFollow;
};

#endif // !defined(AFX_DUMMYOBJMNG_H__ED6153B9_0B7A_42FC_97B0_B9112657B395__INCLUDED_)
