#ifndef __FIELDBOSSMONSTERMANAGER_H__
#define __FIELDBOSSMONSTERMANAGER_H__

#pragma once

#define MAX_FIELDBOSS_DROPITEM_GROUP_NUM 5
#define MAX_FIELDBOSS_DROPITEM_NUM 10

class CFieldBossMonster;
class CFieldSubMonster;

typedef struct stFieldMonsterInfo
{
	// 몬스터 종류 및 부하
	WORD	m_MonsterKind;
	WORD	m_ObjectKind;
	WORD	m_SubKind;
	WORD	m_SubCount;

	// 리젠시간
	DWORD	m_dwRegenTimeMin;
	DWORD	m_dwRegenTimeMax;

	// 소멸 관련 변수
	DWORD	m_dwDistructTime;
	DWORD	m_dwCountTime;
	DWORD	m_dwLastCheckTime;

	// 회복 관련 변수
	DWORD	m_dwPieceTime;
	DWORD	m_dwRecoverStartTime;
	DWORD	m_dwRecoverDelayTime;
	float	m_fLifeRate;
	float	m_fShieldRate;

	// 초기화
	stFieldMonsterInfo()
	{
		m_MonsterKind = 0;
		m_ObjectKind = 0;
		m_SubKind = 0;
		m_SubCount = 0;
		
		m_dwDistructTime = 0;
		m_dwCountTime = 0;
		m_dwLastCheckTime = 0;

		m_dwRegenTimeMin = 0;
		m_dwRegenTimeMax = 0;

		m_dwPieceTime = 0;
		m_dwRecoverStartTime = 0;
		m_dwRecoverDelayTime = 0;
		m_fLifeRate = 0.0f;
		m_fShieldRate = 0.0f;
	}

} FIELDMONSTERINFO;

typedef struct stFieldGroup
{
	WORD	m_GroupID;
	DWORD	m_GridID;
	DWORD	m_BossObjectID;

	CFieldBossMonster*	m_pBoss;
	cPtrList			m_SubList;

	DWORD	m_dwRegenTimeMin;
	DWORD	m_dwRegenTimeMax;

	stFieldGroup()
	{
		m_GroupID = 0;
		m_GridID = 0;

		m_pBoss = NULL;

		m_dwRegenTimeMin = 0;
		m_dwRegenTimeMax = 0;
	}
} FIELDGROUP;

// 필드 보스 드랍 아이템 정보
typedef struct stFieldBossDropItem
{
	WORD	m_wItemIndex[MAX_FIELDBOSS_DROPITEM_NUM];
	WORD	m_wItemCount[MAX_FIELDBOSS_DROPITEM_NUM];
	
	WORD	m_wCount;
	WORD	m_wDropRate;
	WORD	m_wDamageRate;

	stFieldBossDropItem()
	{
		memset( m_wItemIndex, 0, sizeof(WORD) * MAX_FIELDBOSS_DROPITEM_NUM );
		memset( m_wItemCount, 0, sizeof(WORD) * MAX_FIELDBOSS_DROPITEM_NUM );
		
		m_wCount = 0;
		m_wDropRate = 0;
		m_wDamageRate = 0;
	}
} FIELDBOSSDROPITEM;

// 필드 보스 드랍 아이템 리스트
typedef struct stFieldBossDropItemList
{
	WORD	m_wMonsterKind;

	FIELDBOSSDROPITEM m_ItemList[MAX_FIELDBOSS_DROPITEM_GROUP_NUM];

	stFieldBossDropItemList()
	{
		m_wMonsterKind = 0;
	}
} FIELDBOSSDROPITEMLIST;

#define FIELDBOSSMONMGR USINGTON(CFieldBossMonsterManager)
#define MAX_REGENPOS_NUM 10

/**
 * \ingroup MapServer
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-12-14
 *
 * \author Administrator
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */

class CFieldBossMonsterManager
{
	CYHHashTable<FIELDGROUP>					m_FieldGroupTable;
	CMemoryPoolTempl<FIELDGROUP>*				m_pFieldGroupPool;

	CYHHashTable<FIELDMONSTERINFO>				m_FieldMonsterInfoTable;
	CMemoryPoolTempl<FIELDMONSTERINFO>*			m_pFieldMonsterInfoPool;

	CYHHashTable<FIELDBOSSDROPITEMLIST>			m_FieldBossDropItemTable;
	CMemoryPoolTempl<FIELDBOSSDROPITEMLIST>*	m_pFieldBossDropItemPool;

	VECTOR3 m_RegenPos[ MAX_REGENPOS_NUM ];
	WORD m_RegnePosCount;

	char m_LogFile[256];

public:
	CFieldBossMonsterManager(void);
	virtual ~CFieldBossMonsterManager(void);

	void Init();	// 필드 보스 관련 데이터 로딩

	void AddRegenPosition(float x, float z);	// 리젠 위치를 리스트에 추가
	VECTOR3 GetRegenPosition(int index = -1);	// 리젠 위치를 가져온다 -1일 경우 랜덤
	WORD GetRegenPositionCount() { return m_RegnePosCount; }
	
	void BossDead(CFieldBossMonster* pBoss);	// 보스 죽었음
	void SubDead(CFieldSubMonster* pSub);		// 부하 죽었음

	void AddFieldBossMonster( CFieldBossMonster* pBoss );	// 리젠된 필드보스추가
	
	void Process();

	FIELDBOSSDROPITEMLIST* GetFieldBossDropItemList( WORD MonsterKind );
};

EXTERNGLOBALTON(CFieldBossMonsterManager);

#endif