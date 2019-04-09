#pragma once
#include "object.h"
#include "..\[CC]Header\GameResourceStruct.h"

#define TITAN_STATE_CHECKTIME	10000		//타이탄 상태 체크 시간, 내력 관련
//#define TITAN_VAMP_DELAYTIME	60000		//주인 체력 흡수 딜레이

class CTitan : public CObject
{
	CPlayer*	m_pOwner;
	BASE_TITAN_LIST*	m_pBaseInfo;
	DWORD		m_dwTitanSpellCheckTime;	// 10초 마다 지속적 마력 소모.
	DWORD		m_dwTitanVampDelayCheckTime;		// 캐릭터 체력 가져오기 60초 딜레이.
	TITAN_TOTALINFO		m_TitanTotalInfo;
	BOOL		m_bVamped;		// 소환 후 체력 흡수 여부. 첫 시간 체크시 사용.

public:
	CTitan(void);
	~CTitan(void);

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)	{};

	void	InitTitan(TITAN_TOTALINFO* pInfo, CPlayer* pOwner);
	TITAN_TOTALINFO* GetTotalInfo() {	return &m_TitanTotalInfo;	}

	titan_calc_stats* GetItemStats();
	titan_calc_stats* GetTitanStats();

	void	Process();
	const	BASE_TITAN_LIST* GetTitanBaseInfo()	{	return m_pBaseInfo;	}

	void	SendTitanInfoMsg();

	void	ReduceSpell(DWORD rdcSpellAmount);
	void	ReduceSpellConstantly();

	BOOL	CheckLifeForVamp(DWORD fuel);
	void	VampOwnerLife(DWORD vampRate);

	void	SetTitanRiding(BOOL bVal)	{	m_TitanTotalInfo.bRiding = bVal;	}

	DWORD	GetTitanKind()	{	return m_TitanTotalInfo.TitanKind;	}
	DWORD	GetTitanGrade()	{	return m_TitanTotalInfo.TitanGrade;	}
	DWORD	GetCallItemDBIdx()	{	return m_TitanTotalInfo.TitanCallItemDBIdx;	}

	virtual void	SetLife(DWORD fuel,BOOL bSendMsg = TRUE);
	virtual DWORD	GetLife();
	virtual void	SetNaeRyuk(DWORD spell,BOOL SendMsg = TRUE);
	virtual DWORD	GetNaeRyuk();
	virtual DWORD	DoGetMaxLife();
	virtual DWORD	DoGetMaxNaeRyuk();
	virtual DWORD	DoGetPhyDefense();
	virtual float	DoGetAttDefense(WORD Attrib);
	virtual float	GetAttribPlusPercent(WORD Attrib)	{	return 0;	}
	virtual DWORD	DoGetPhyAttackPowerMin();
	virtual DWORD	DoGetPhyAttackPowerMax();
	virtual float	DoGetMoveSpeed()	{	return 0;	}
	virtual float	DoGetAddAttackRange()	{	return 0;	}

	virtual void	DoDie(CObject* pAttacker);
	virtual void DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife);
	virtual DWORD	Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);
	//void	AbsorbDamage(CObject* pAttacker, RESULTINFO* pDamageInfo);
    
	BOOL	IsVampTitanfuelBySetdamage( DWORD fuel,BOOL bSendMsg = TRUE );
};
