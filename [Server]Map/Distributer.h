// Distributer.h: interface for the CDistributer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_)
#define AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_POINTACCEPTOBJECT_NUM 2
struct DAMAGEOBJ
{
	DWORD dwID;
	DWORD dwData;
};
class CParty;
class CDistributeWay;
class MONSTEREX_LIST;

class CDistributer  //responsible for decision who will get compensation when monster died
{
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableSolo;
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableParty;

	DWORD	m_PlusDamage;
	DWORD	m_1stPlayerID;
	DWORD	m_1stPartyID;
	DWORD	m_TotalDamage;

	void SetPlusTotalDamage(DWORD Damage);
	DWORD GetTotalDamage();
	
public:
	CDistributer();
	virtual ~CDistributer();

	void Release();
	void DamageInit();
	void CalcAndSendPartyExp(CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel);
	//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//	void CalcAndSendPartyExp(CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel, DWORD MonsterExp=0);
	void AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage);
	void DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage);
	void Distribute(DWORD KillerID,VECTOR3* pKilledPosition, WORD DropItemID, DWORD DropItemRatio,CMonster* pMob);
	DWORD CalcObtainExp(LEVELTYPE MonsterLevel,LEVELTYPE KillerLevel,DWORD TotalLife,DWORD Damage, DWORD dwPlayerMaxNum);
	//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//	DWORD CalcObtainExp(LEVELTYPE MonsterLevel,LEVELTYPE KillerLevel,DWORD TotalLife,DWORD Damage,DWORD MonsterExp=0);

	DWORD CalcObtainAbilityExp(LEVELTYPE MonsterLevel,LEVELTYPE KillerLevel);
	
	BOOL Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID);
	//거리에 따른 경험치 얻기를 뺀 체크
	BOOL ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID);//060628 - Add by wonju
	
	void ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID);
	void GetReceivePartyMember(PARTY_RECEIVE_MEMBER* rtInfo, CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE* pMaxLevel);
	void SendPartyAbilandItem(CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE MonsterLevel,
							WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind);
	void SendPersonalAbilandItem(CPlayer* pReceivePlayer, CMonster* pMonster, WORD DropItemID, DWORD DropItemRatio);
	void DistributePerDamage(CMonster* pMob);
	
	// 필드보스 - 05.12 이영준
	// 필드보스용 아이템 분배 함
	void DistributeItemPerDamage(CMonster* pMob);
	void DeleteDamagedPlayer(DWORD CharacterID);
	
	void CalcAndSendObtainPerDamage(CPlayer* pReceivePlayer, DWORD DamageRate, WORD MonsterKind);
};

#endif // !defined(AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_)
