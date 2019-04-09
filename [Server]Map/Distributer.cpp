// Distributer.cpp: implementation of the CDistributer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distributer.h"
#include "UserTable.h"
#include "CharMove.h"
#include "AttackManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "Distribute_Random.h"
#include "Monster.h"
#include "BossRewardsManager.h"
// 필드보스 - 05.12 이영준
#include "FieldBossMonsterManager.h"
#include "ItemManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

float gExpRate		= 1.0f;
float gItemRate		= 1.0f;
float gMoneyRate	= 1.0f;
// RaMa -04.11.24
float gDamageReciveRate	= 1.0f;		// 받는 데미지
float gDamageRate		= 1.0f;		// 주는 데미지
float gNaeRuykRate	= 1.0f;		// 내력소모
float gUngiSpeed	= 1.0f;		// 운기조식 스피드
float gPartyExpRate	= 1.0f;		// 파티경험치
float gAbilRate		= 1.0f;		// 특기치
float gGetMoney		= 1.0f;		// 얻는돈의 배수
float gMugongExpRate = 1.0f;	// 무공경험치 배수
// Etc - 아직안됨
float gShield		= 1.0f;		// 호신강기
float gDefence		= 1.0f;		// 방어력

float gEventRate[eEvent_Max];
float gEventRateFile[eEvent_Max];
PARTYEVENT gPartyEvent[ePartyEvent_Max];
BOOL g_bPlusTime = FALSE;


CDistributer::CDistributer()
{
	m_DamageObjectTableSolo.Initialize(MAX_POINTACCEPTOBJECT_NUM);
	m_DamageObjectTableParty.Initialize(MAX_POINTACCEPTOBJECT_NUM);
	m_1stPartyID = 0;
	m_1stPlayerID = 0;
	m_PlusDamage = 0;
	m_TotalDamage = 0;
}

CDistributer::~CDistributer()
{
	Release();
}

void CDistributer::Release()
{
	m_1stPartyID = 0;
	m_1stPlayerID = 0;
	m_PlusDamage = 0;
	DAMAGEOBJ *sobj, *pobj = NULL;
	m_DamageObjectTableSolo.SetPositionHead();
	while(sobj = m_DamageObjectTableSolo.GetData())
		delete sobj;
	m_DamageObjectTableSolo.RemoveAll();

	m_DamageObjectTableParty.SetPositionHead();
	while(pobj = m_DamageObjectTableParty.GetData())
		delete pobj;
	m_DamageObjectTableParty.RemoveAll();
}

void CDistributer::AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage)
{
	if(pPlayer->GetPartyIdx()) //파티원이면 파티데미지에 플러스
	{
		DoAddDamageObj(&m_DamageObjectTableParty, pPlayer->GetPartyIdx(), damage+plusdamage);
	}
	DoAddDamageObj(&m_DamageObjectTableSolo, pPlayer->GetID(), damage+plusdamage);	

	if(plusdamage != 0)
	{
		m_PlusDamage = plusdamage;
		m_1stPlayerID = pPlayer->GetID();
		m_1stPartyID = pPlayer->GetPartyIdx();
	}
	
	SetPlusTotalDamage(damage+plusdamage);
}

void CDistributer::DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage)
{
	DAMAGEOBJ* pDObj;
	pDObj = pTable->GetData(dwID);
	if(pDObj != NULL)
	{
		pDObj->dwData += damage;
	}
	else
	{
		DAMAGEOBJ * newObj = new DAMAGEOBJ;
		newObj->dwID = dwID;
		newObj->dwData = damage;
		
		pTable->Add(newObj, newObj->dwID);
	}
}

void CDistributer::Distribute(DWORD KillerID,VECTOR3* pKilledPosition, WORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
{
	DAMAGEOBJ *obj;
	DWORD Exp = 0;
	CPlayer* pReceivePlayer;
	
	DWORD BigPlayerID; //데미지를 가장 많이 줘서 수련치와 아이템 가져갈 사람/파티
	DWORD BigPartyID;
	DWORD BigPlayerDamage, BigPartyDamage;
	BigPlayerID = BigPartyID = BigPlayerDamage = BigPartyDamage = 0;
	
	//솔로 테이블 검사
	m_DamageObjectTableSolo.SetPositionHead();
	while(obj = m_DamageObjectTableSolo.GetData())
	{
		if(obj->dwData == 0)
			continue;
		////아이템과 수련치 받을 사람 선택하기 위해 가장 큰 데미지 준 사람 선택
		ChooseOne(obj, &BigPlayerDamage, &BigPlayerID);

		pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
		if(Chk(pReceivePlayer, pKilledPosition, pMob->GetGridID()) == FALSE)
			continue;
		if(obj->dwID == m_1stPlayerID)
		{
			obj->dwData -= m_PlusDamage;
		}
		if(pReceivePlayer->GetPartyIdx())
			continue;
		/*
		if( g_pServerSystem->GetMapNum() >= TITANMAP_START_INDEX )
			Exp = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( pMob->GetLevel() )->stDropInfo[0].wExpPoint;
		else if( pMob->GetSMonsterList()->ExpPoint)	//호박 산타
			Exp = CalcObtainExp(pMob->GetLevel() ,pReceivePlayer->GetLevel(),pMob->GetSMonsterList()->Life,obj->dwData);
		*/
			//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//			Exp = CalcObtainExp(pMob->GetLevel(), pReceivePlayer->GetLevel(), pMob->GetSMonsterList()->Life, obj->dwData, pMob->GetSMonsterList()->ExpPoint );
		//2007. 11. 3. CBH - 경험치 주는 방식 수정
		//타이탄 맵에서 데미지 비율로 경험치 주는 사람 정하는 과정에서 빠져 있어서 CalcObtainExp() 함수로 이동
		if( pMob->GetSMonsterList()->ExpPoint)
		{
			Exp = CalcObtainExp(pMob->GetLevel() ,pReceivePlayer->GetLevel(),pMob->GetSMonsterList()->Life,obj->dwData, 1);
		}
		else
			Exp = 0;
		if(Exp == 0)
			continue;

		// 개인에게 경험치 보냄
		DISTRIBUTERAND->SendToPersonalExp(pReceivePlayer, Exp);
	}
	
	//파티
	DAMAGEOBJ* pPobj = NULL;
	CParty* pParty;
	m_DamageObjectTableParty.SetPositionHead();
	while(pPobj = m_DamageObjectTableParty.GetData())
	{
		////큰 데미지 준 파티 선택
		ChooseOne(pPobj, &BigPartyDamage, &BigPartyID);

		pParty = PARTYMGR->GetParty(pPobj->dwID);
		if(pParty==NULL) //파티가 해산됐으면 continue
			continue;
		if(pPobj->dwData == 0)
			continue;
		if(pParty->GetPartyIdx() == m_1stPartyID)
		{
			pPobj->dwData -= m_PlusDamage;
		}

		if( pMob->GetSMonsterList()->ExpPoint)	//호박 산타		
			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel());
//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel(), pMob->GetSMonsterList()->ExpPoint);
	}

	//abil and item
	pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(BigPlayerID);
	pParty = PARTYMGR->GetParty(BigPartyID);

	if(BigPartyDamage < BigPlayerDamage) //개인에게 보냄
	{
		SendPersonalAbilandItem(pReceivePlayer, pMob, DropItemID, DropItemRatio);
	}
	else if(BigPartyDamage == BigPlayerDamage) //데미지가 같으면 랜덤으로 선택
	{ 
		int rate = rand()%2;
		if(pParty)
		if(pParty->IsPartyMember(BigPlayerID) == TRUE)
		{
			SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
		}		
		else if(rate == 0)
		{
			SendPersonalAbilandItem(pReceivePlayer, pMob, DropItemID, DropItemRatio);
		}
		else //파티에게
		{	
			SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
		}
	}
	else //파티에게
	{
		SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
	}
	Release();
}

//void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel, DWORD MonsterExp )
void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel )
{
	DAMAGEOBJ *obj = NULL;
	CPlayer* pPlayer[MAX_PARTY_LISTNUM] = {0,};
	DWORD PlayerID;
	float levelavg = 0;
	LEVELTYPE maxlevel = 0;  
	LEVELTYPE curlevel = 0;
	DWORD onlinenumconfirm = 0;	
	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		PlayerID = pParty->GetMemberID(n);
		if(PlayerID == 0)
			continue; 
		pPlayer[n] = (CPlayer*)g_pUserTable->FindUser(PlayerID);


//#ifdef _TW_LOCAL_	//hk block
//		if(Chk(pPlayer[n], pKilledPosition,GridID) == FALSE)
//			continue;
//#else
		if(ChkExp(pPlayer[n], pKilledPosition,GridID) == FALSE) //060628 - Fixed by wonju
			continue;
//#endif
		
		++onlinenumconfirm;
		curlevel = pPlayer[n]->GetLevel();
		if(maxlevel < curlevel)
			maxlevel = curlevel;
		levelavg += curlevel;		
	}
	
	levelavg /= (float)onlinenumconfirm;
	
	DWORD partyexp = 0;
	/*
	if( g_pServerSystem->GetMapNum() > TITANMAP_START_INDEX )
	{
		MONSTERDROP_TIMAP* pdropinfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( MonsterLevel );
		if( pdropinfo && onlinenumconfirm > 0 )
			partyexp = pdropinfo->stDropInfo[onlinenumconfirm-1].wExpPoint;
	}
	else
		partyexp = CalcObtainExp(MonsterLevel,maxlevel,MonsterTotalLife ,TotalDamage);
	*/
	
	//2007. 11. 3. CBH - 경험치 주는 방식 수정
	//타이탄 맵에서 데미지 비율로 경험치 주는 사람 정하는 과정에서 빠져 있어서 CalcObtainExp() 함수로 이동
	partyexp = CalcObtainExp(MonsterLevel,maxlevel,MonsterTotalLife ,TotalDamage, onlinenumconfirm);

	//SW050806 기존 데이타 테이블 파싱에서 공식화로 변경
//	EXPTYPE partyexp = CalcObtainExp( MonsterLevel,maxlevel,MonsterTotalLife, TotalDamage, MonsterExp );
	DWORD exp=0;
	if(partyexp == 0)
		return;
	
	for(n=0;n<MAX_PARTY_LISTNUM;++n)
	{	
//#ifdef _TW_LOCAL_	//hk block
//		if(Chk(pPlayer[n],pKilledPosition,GridID) == FALSE)
//			continue;
//#else
		if(ChkExp(pPlayer[n],pKilledPosition,GridID) == FALSE) //060629 - Fixed by wonju
			continue;
//#endif
				
		curlevel = pPlayer[n]->GetLevel();
		if(onlinenumconfirm != 1)
			exp = (DWORD)(partyexp * ( curlevel * (10 + onlinenumconfirm) / 9.f / (float)levelavg ) 
									/ (float)onlinenumconfirm);
		else
			exp = partyexp;
		if(exp == 0)
			continue;

		// 06. 06. 파티 경험치 분배 수정 - 이영준
		// 분배대상이 2명이상일때만 추가 경험치 부여
		if(onlinenumconfirm > 1)
		{
			// 파티경험치 비율 추가 - RaMa 04.11.24 	
			exp = (DWORD)(exp*gEventRate[eEvent_PartyExpRate]);
		}

		DISTRIBUTERAND->SendToPersonalExp(pPlayer[n], exp);
	}
}

//DWORD CDistributer::CalcObtainExp( LEVELTYPE MonsterLevel, LEVELTYPE KillerLevel, DWORD TotalLife, DWORD Damage, DWORD MonsterExp )
DWORD CDistributer::CalcObtainExp( LEVELTYPE MonsterLevel, LEVELTYPE KillerLevel, DWORD TotalLife, DWORD Damage, DWORD dwPlayerMaxNum )
{
	//2007. 11. 3. CBH - 타이탄 맵에서 경험치 수정
	//이쪽 함수로 이동...여기서 경험치 받고 데미지 비율로 계산
	DWORD ObtainPoint = 0;
	if( g_pServerSystem->GetMapNum() > TITANMAP_START_INDEX )
	{
		MONSTERDROP_TIMAP* pdropinfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( MonsterLevel );
		if( pdropinfo && dwPlayerMaxNum > 0 )
			ObtainPoint = pdropinfo->stDropInfo[dwPlayerMaxNum-1].wExpPoint;
	}
	else
	{
		if(KillerLevel >= MonsterLevel + 6)
			return 0;
		else if(KillerLevel <= MonsterLevel - 9)
			MonsterLevel = KillerLevel + 9;

		ObtainPoint = ATTACKMGR->GetAttackCalc().GetPlayerPoint(MonsterLevel,MonsterLevel - KillerLevel);
	}	
//	DWORD ObtainPoint = ATTACKMGR->GetAttackCalc().GetPlayerExpPoint( KillerLevel - MonsterLevel, MonsterExp );
	/*
	가)	1		= 100% 데미지 : 경험치 100%
	나)	0.8~1	= 80%~100%미만 데미지 : 경험치 80%
	다)	0.6~0.8	= 60%~80% 미만 데미지 : 경험치 60%
	라)	0.4~0.6			40%~60% 미만 데미지 : 경험치 40%
	마)	0.2~0.4			20%~40% 미만 데미지 : 경험치 20%
	바)	0~0.2			20%미만 : 경험치 없음.
	*/
	DWORD perc = Damage*100 / TotalLife;
	if( (100 <= perc) )
	{
		ObtainPoint = ObtainPoint;
	}
	else if( (80 <= perc) && (perc < 100) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.8);
	}
	else if( (60 <= perc) && (perc < 80) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.6);
	}
	else if( (40 <= perc) && (perc < 60) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.4);
	}
	else if( (20 <= perc) && (perc < 40) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.2);
	}
	else
	{
		ObtainPoint = 0;
	}

	return ObtainPoint;
}

//거리에 따른 경험치 얻기를 뺀 체크
BOOL CDistributer::ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	
	if(!pPlayer)	
		return FALSE;
	if(pPlayer->GetInitState() != PLAYERINITSTATE_INITED)
		return FALSE;
	if(pPlayer->IsVimuing() == TRUE)
		return FALSE;
	
	if(pPlayer->GetState() == eObjectState_Die)
		return FALSE;
	if(pPlayer->GetGridID() != GridID)
		return FALSE;
	return TRUE;

}

//거리에 따른 경험치를 얻을수 있는 체크
#ifdef _TW_LOCAL_
#define POINT_VALID_DISTANCE		10000	//대만은 100m
#else
#define POINT_VALID_DISTANCE		3000
#endif


BOOL CDistributer::Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	VECTOR3* PlayerPosition;
	if(!pPlayer)	
		return FALSE;
	if(pPlayer->GetInitState() != PLAYERINITSTATE_INITED)
		return FALSE;
	if(pPlayer->IsVimuing() == TRUE)
		return FALSE;
	PlayerPosition = CCharMove::GetPosition(pPlayer);
	if(CalcDistanceXZ(pKilledPosition, PlayerPosition) > POINT_VALID_DISTANCE)
		return FALSE;
	if(pPlayer->GetState() == eObjectState_Die)
		return FALSE;
	if(pPlayer->GetGridID() != GridID)
		return FALSE;
	return TRUE;
}

void CDistributer::ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID)
{
	if(*pBigDamage < pobj->dwData)
	{
		*pBigDamage = pobj->dwData;
		*pBigID = pobj->dwID;
	}
	else if(*pBigDamage == pobj->dwData)
	{
		int rate = rand() % 2;
		if(rate == 1)
			*pBigID = pobj->dwID;
	}
}

void CDistributer::GetReceivePartyMember( PARTY_RECEIVE_MEMBER* rtInfo, CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE* pMaxLevel)
{
	int count=0;
	DWORD PlayerID;
	DAMAGEOBJ* pobj;
	CPlayer* pPlayer;
	LEVELTYPE maxlevel = 0;  
	LEVELTYPE curlevel = 0;
	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		PlayerID = pParty->GetMemberID(n);
		if(PlayerID == 0)
			continue; 
		pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
		if(Chk(pPlayer,pKilledPosition,GridID) == FALSE)
			continue;
		
//		pobj = m_DamageObjectTableSolo.GetData(PlayerID);
//		if(pobj == 0)
//			continue;
		
		rtInfo->pPlayer[count] = pPlayer;
		pobj = m_DamageObjectTableSolo.GetData(PlayerID);
		if( pobj )
			rtInfo->Damage[count] = pobj->dwData;
		else
			rtInfo->Damage[count] = 0;
		count++;

		curlevel = pPlayer->GetMaxLevel();
		if(maxlevel < curlevel)
			maxlevel = curlevel;
	}
	rtInfo->count = count;
	*pMaxLevel = maxlevel;
}

void CDistributer::SendPersonalAbilandItem(CPlayer* pReceivePlayer, CMonster* pMonster, WORD DropItemID, DWORD DropItemRatio)
{
	if(pReceivePlayer == 0)
	{		
		Release();
		return;
	}
	//경험치가 0이면 특기 수련치도 안준다. (ex: 호박이벤트)
	if(pMonster->GetSMonsterList()->ExpPoint)
		DISTRIBUTERAND->SendToPersonalAbil(pReceivePlayer, pMonster->GetLevel());
	DISTRIBUTERAND->SendToPersonalItem(pReceivePlayer, DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMonster->GetSMonsterList(), pMonster->GetMonsterKind());
}

void CDistributer::SendPartyAbilandItem(CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE MonsterLevel,
										WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind)
{
	if(!pParty)
	{
		Release();
		return;
	}
	PARTY_RECEIVE_MEMBER MemberInfo;
	LEVELTYPE MaxLevel = 1;
	GetReceivePartyMember(&MemberInfo, pParty, pKilledPosition, GridID, &MaxLevel);

	//경험치가 0이면 특기 수련치도 안준다. (ex: 호박이벤트)
	if(pMonInfo->ExpPoint)
		pParty->SendAbil(MonsterLevel, &MemberInfo, MaxLevel);
	pParty->SendItem(&MemberInfo, DropItemId, DropItemRatio, pMonInfo, MonsterKind, MaxLevel);
}

// 필드보스 - 05.12 이영준
// 필드보스용 아이템 분배 함수
void CDistributer::DistributeItemPerDamage(CMonster* pMob)
{
	DAMAGEOBJ *obj;
	CPlayer* pReceivePlayer;
	WORD MonsterKind;

	CPlayer* TargetPlayerTable[ 100 ]; // 한칸당 1%
	WORD TargetPos = 0;
	
	MonsterKind = pMob->GetMonsterKind();

	// 드랍 아이템 리스트를 가져온다
	FIELDBOSSDROPITEMLIST* pDropItemList = FIELDBOSSMONMGR->GetFieldBossDropItemList( MonsterKind );
    
	// 드랍 아이템 정보가 없으면 종료한다
	if( pDropItemList == NULL )
		return;

	for(WORD index = 0; index < MAX_FIELDBOSS_DROPITEM_GROUP_NUM; index++)
	{
		// 드랍 아이템 정보가 있는가? 없다면 종료
		if( pDropItemList->m_ItemList[ index ].m_wItemIndex[0] == 0 )
			break;
        
		// 아이템 그룹 내의 어떤 아이템을 줄지 결정
		for( WORD dropitemnum = 0; dropitemnum < MAX_FIELDBOSS_DROPITEM_NUM; dropitemnum++)
		{
			if(pDropItemList->m_ItemList[ index ].m_wItemIndex[dropitemnum] == 0)
				break;
		}

		WORD select = rand() % dropitemnum;

		// 드랍될 횟수를 가져온다
		WORD count = pDropItemList->m_ItemList[ index ].m_wCount;

		for(WORD i = 0; i < count; i++)
		{
			// 드랍 할것인가?
			if( (rand() % 100) < pDropItemList->m_ItemList[ index ].m_wDropRate )
			{
				// 변수초기화
				memset( TargetPlayerTable, 0, sizeof(CPlayer*) * 100 );
				TargetPos = 0;

				// 공격한 사람들을 검색해서 기준치 이상 데미지를 준 사람들 가려낸다
				m_DamageObjectTableSolo.SetPositionHead();
				while(obj = m_DamageObjectTableSolo.GetData())
				{
					if(obj->dwData == 0)
						continue;		
					pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
					if(!pReceivePlayer)	
						continue;
					if(pReceivePlayer->GetInitState() != PLAYERINITSTATE_INITED)
						continue;
					if(pReceivePlayer->GetGridID() != pMob->GetGridID())
						continue;
					// 몬스터와 +6 이상 혹은 -9 이하의 Player는 분배에서 제외
					if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
						continue;
					//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
					//	continue;
					
					// 데미지 비율 계산
					WORD DamageRate = (WORD)(((double)obj->dwData / m_TotalDamage) * 100);

					// 기준치 이상 데미지를 주었다면
					if( DamageRate >= pDropItemList->m_ItemList[ index ].m_wDamageRate )
					{
						// 데미지 비율만큼 대상 테이블에 등록한다
						for(DWORD rate = 0; rate < DamageRate; rate++)
						{
							TargetPlayerTable[ TargetPos++ ] = pReceivePlayer;
						}
					}
				}

				// 대상 테이블에 등록된 유저가 있다면
				if( TargetPos > 0 )
				{
					// 랜덤으로 테이블 위치를 결정한다
					WORD TargetIndex = rand() % TargetPos;

					// 결정된 위치에 등록된 유저를 가져온다
					CPlayer* TargetPlayer = TargetPlayerTable[ TargetIndex ];

					// 유저 정보가 유효하면 아이템을 넣어준다
					if( TargetPlayer )
						ITEMMGR->MonsterObtainItem( TargetPlayer, 
													pDropItemList->m_ItemList[ index ].m_wItemIndex[ select ],
													MonsterKind,
													pDropItemList->m_ItemList[ index ].m_wItemCount[ select ]);
				}
			}
		}
	}
}

void CDistributer::DistributePerDamage(CMonster* pMob)
{	
	DAMAGEOBJ *obj = NULL;
	CPlayer* pReceivePlayer = NULL;;
	DWORD damagerate, Exp, Money, Abil;
	EXPTYPE MaxExp;
	DWORD MaxAbil, MaxMoney;
	WORD MonsterKind;
	
	damagerate = Exp = Abil = Money = 0;
	MaxExp = MaxAbil = MaxMoney = 0;
	MonsterKind = 0;
	MonsterKind = pMob->GetMonsterKind();
	BOSSREWARDSMGR->GetBossRewardsInfo(MonsterKind, MaxExp, MaxAbil, MaxMoney);	

	m_DamageObjectTableSolo.SetPositionHead();
	while(obj = m_DamageObjectTableSolo.GetData())
	{
		if(obj->dwData == 0)
			continue;		
		pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
		if(!pReceivePlayer)	
			continue;
		if(pReceivePlayer->GetInitState() != PLAYERINITSTATE_INITED)
			continue;
		if(pReceivePlayer->GetGridID() != pMob->GetGridID())
			continue;
		//얘네도 준다.
	/*	if(pReceivePlayer->IsVimuing() == TRUE)
			continue;		
		if(pPlayer->GetState() == eObjectState_Die)
			return FALSE; */
				
		Exp = (DWORD)(((double)obj->dwData * MaxExp) / m_TotalDamage);
		Abil = (DWORD)(((double)obj->dwData * MaxAbil) / m_TotalDamage);
		Money = (DWORD)(((double)obj->dwData * MaxMoney) / m_TotalDamage);

		// 필드보스 - 05.12 이영준
		// 필드보스의 경험치 수련치 돈 분
		if(pMob->GetObjectKind() == eObjectKind_FieldBossMonster)
		{
			// 몬스터와 +6 이상 혹은 -9 이하의 Player는 분배에서 제외
			if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
				continue;
			//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
			//	continue;

			if(Exp == 0) Exp = 1;
			if(Abil == 0) Abil = 1;
			if(Money == 0) Money = 1;

			pReceivePlayer->AddPlayerExpPoint(Exp);
			pReceivePlayer->AddAbilityExp(Abil);
			DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
		}
		else if(pMob->GetMonsterKind() == eBOSSKIND_EMPERIORTOMB_EVENT)
		{
			if(Exp == 0) Exp = 1;
			if(Abil == 0) Abil = 1;
			if(Money == 0) Money = 1;

			pReceivePlayer->AddPlayerExpPoint(Exp);
			pReceivePlayer->AddAbilityExp(Abil);
			DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
		}
		else
		{
			if(Exp >= 10000)
				pReceivePlayer->AddPlayerExpPoint(Exp);
			if(Abil >= 50000)
				pReceivePlayer->AddAbilityExp(Abil);
			if(Money >= 100000)
				DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
		}
	//	DISTRIBUTERAND->SendExpAbilMoney(pReceivePlayer, Exp, Abil, Money);
	}
}

void CDistributer::SetPlusTotalDamage(DWORD Damage)
{
	m_TotalDamage += Damage;
}

DWORD CDistributer::GetTotalDamage()
{
	return m_TotalDamage;
}

void CDistributer::DamageInit()
{
	m_TotalDamage = 0;

	m_DamageObjectTableSolo.RemoveAll();
}

void CDistributer::DeleteDamagedPlayer(DWORD CharacterID)
{	
	DAMAGEOBJ* pData = m_DamageObjectTableSolo.GetData( CharacterID );
	if( pData )
	{
		if( m_TotalDamage >= pData->dwData )
			m_TotalDamage -= pData->dwData;
		else
			m_TotalDamage = 0;
	} 
	m_DamageObjectTableSolo.Remove(CharacterID);
}
