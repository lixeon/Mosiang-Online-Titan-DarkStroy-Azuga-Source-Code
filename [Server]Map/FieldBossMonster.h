#ifndef __FIELDBOSSMONSTER_H__
#define __FIELDBOSSMONSTER_H__

#pragma once
#include "monster.h"
#include "FieldBossMonsterManager.h"

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

class CFieldBossMonster :
	public CMonster
{
	// 소멸/회복 관련 추가 상태값
	FIELDMONSTERINFO m_Info;

	//필보의 리젠위치 인덱스 변수
	int m_nRegenPosIndex;

public:
	CFieldBossMonster(void);
	virtual ~CFieldBossMonster(void);

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);

	// CMonster에서는 메세지 전송을 안해준다. 회복프로세스를 위해 재정의 필요
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);

	virtual void DoDie(CObject* pAttacker);

	void SetInfo(FIELDMONSTERINFO* Info)
	{
		memcpy(&m_Info, Info, sizeof(FIELDMONSTERINFO));
	}

	BOOL IsBattle();	// 전투상황 체크
	BOOL IsDistruct();	// 소멸시간 체크 - 소멸시간을 카운트해서 소멸되야 하면 TRUE를 리턴한다

	virtual void AddStatus(CStatus* pStatus);
	virtual void StartSpecialState(DWORD SpecialStateKind,DWORD Time,
						WORD wParam1,WORD wParam2,float fParam3,
						WORD EffectNum,WORD StateIcon);
	
	void Recover();		// 회복 프로세스

	void SetRegenPosIndex(int nIndex)	{ m_nRegenPosIndex = nIndex; }
	int GetRegenPosIndex()				{ return m_nRegenPosIndex; }	

	virtual BOOL DoWalkAround();
};

#endif
