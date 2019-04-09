// CharacterCalcManager.h: interface for the CCharacterCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERCALCMANAGER_H__BD6E030A_4710_43A8_BF56_F115B3478332__INCLUDED_)
#define AFX_CHARACTERCALCMANAGER_H__BD6E030A_4710_43A8_BF56_F115B3478332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer;

class CCharacterCalcManager  
{

public:
	CCharacterCalcManager();
	virtual ~CCharacterCalcManager();
	
// taiyo
	static void CalcMaxLife(CPlayer * pPlayer);			// º¯¼ö : Ã¼·Â, ·¹º§, ÀåÂø ¾ÆÀÌÅÛ
	static void CalcMaxShield(CPlayer * pPlayer);			// º¯¼ö : ½É¸Æ, ·¹º§, ÀåÂø ¾ÆÀÌÅÛ
	static void CalcMaxNaeRyuk(CPlayer * pPlayer);			// º¯¼ö : ½É¸Æ, ·¹º§, ÀåÂø ¾ÆÀÌÅÛ

	static void ProcessLife(CPlayer * pPlayer);
	static void ProcessShield(CPlayer * pPlayer);
	static void ProcessNaeRyuk(CPlayer * pPlayer);

	static void ProcessShieldBoss(CBossMonster * pBoss);
	static void ProcessLifeBoss(CBossMonster * pBoss);

	static void General_LifeCount(CPlayer * pPlayer, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	static void General_ShieldCount(CPlayer * pPlayer, DWORD curTime, LEVELTYPE level, DWORD Shield, DWORD maxShield);
	static void General_NaeRyukCount(CPlayer* pPlayer, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	
	static void General_LifeCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD life, DWORD maxlife);
	static void General_ShieldCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD Shield, DWORD maxShield);

	static void	Ungi_LifeCount(CPlayer* pPlayer, DWORD life, DWORD maxlife, DWORD curTime);
	static void	Ungi_ShieldCount(CPlayer* pPlayer, DWORD Shield, DWORD maxShield, DWORD curTime);
	static void	Ungi_NaeRyukCount(CPlayer* pPlayer, DWORD curTime, DWORD energy, DWORD maxEnergy);
	static void KyungGong_NaeRyukCount(CPlayer* pPlayer, DWORD curTime,WORD KyungGongIdx);
	static BOOL SingleSpecialState_NaeRyukCount(CPlayer* pPlayer);

	static void UpdateLife(CObject* pObject);
	static void StartUpdateLife(CObject * pObject, DWORD plusLife, BYTE recoverInterval=RECOVERINTERVALUNIT, DWORD recoverTime=RECOVERTIMEUNIT);

	static void UpdateNaeRyuk(CPlayer* pPlayer);
	static void StartUpdateNaeRyuk(CPlayer* pPlayer, DWORD plusNaeryuk );
	
	static void UpdateShield(CObject* pObject);
	static void StartUpdateShield( CObject* pObject, DWORD plusShield, BYTE recoverInterval=RECOVERINTERVALUNIT, DWORD recoverTime=RECOVERTIMEUNIT);
};

#endif // !defined(AFX_CHARACTERCALCMANAGER_H__BD6E030A_4710_43A8_BF56_F115B3478332__INCLUDED_)
