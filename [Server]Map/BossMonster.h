// BossMonster.h: interface for the CBossMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSMONSTER_H__F3B40B9A_D0CC_4EFE_AB37_A90193EC7D17__INCLUDED_)
#define AFX_BOSSMONSTER_H__F3B40B9A_D0CC_4EFE_AB37_A90193EC7D17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "BossState.h"

class CBossMonsterInfo;
struct BOSSEVENTSTATE;

class CBossMonster  : public CMonster
{
	CBossMonsterInfo* m_pBossMonsterInfo;
	CBossState m_BossState;
	BOSSEVENTSTATE* m_BossEventState;
	
	int m_EventActionValue;

	DWORD m_CurAttackIdx;
	DWORD m_AttackStartTime;

	BOOL m_IsEventStating;
	
	/// 06. 09. 2차 보스 - 이영준
	/// 소멸시간
	DWORD	m_DeleteStartTime;
	BOOL	m_bDelete;

public:
	CBossMonster();
	virtual ~CBossMonster();
	
	void Attack();
	void Pursuit();
	void WalkAround();
	void Stand();
	void Search();

	void Recover();
	void Summon();

	virtual void DoDie(CObject* pAttacker);	
	virtual void DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife);
	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual float DoGetMoveSpeed();
	virtual void SetLife(DWORD Life, BOOL bSendMsg);
	virtual void SetShield(DWORD val, BOOL bSendMsg);
	virtual void AddStatus(CStatus* pStatus);
	virtual void StartSpecialState(DWORD SpecialStateKind,DWORD Time,
						WORD wParam1,WORD wParam2,float fParam3,
						WORD EffectNum,WORD StateIcon);
	virtual void DoAttack();
	virtual BOOL DoWalkAround();

	virtual void Release();	
	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);

	void SetBossInfo(CBossMonsterInfo* pInfo);
	void SetEventState(BYTE EventState, DWORD ActionValue);
	void Process();
	
	void SetCurAttackIdx(DWORD AttackIdx);
	DWORD GetCurAttackIdx();

	void ChangeCurAttackIdx();
	BOOL SetBossState(int state);

	int GetEventActionValue();
	void SetEventActionValue(DWORD value);

	WORD GetSummonFileNum();

	void SetBossNextState(int state);
	void OnLifeEvent(DWORD Life);

	void SetTargetObject(CObject* TObject);
	
	void ChangeTargetForSkill(VECTOR3 * pPivotPos);
	void Delete();
};

#endif // !defined(AFX_BOSSMONSTER_H__F3B40B9A_D0CC_4EFE_AB37_A90193EC7D17__INCLUDED_)
