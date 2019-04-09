#include "stdafx.h"
#include ".\titan.h"
#include "Player.h"
#include "../[CC]Header/GameResourceManager.h"

CTitan::CTitan(void)
{
	m_pOwner = NULL;
	m_pBaseInfo = NULL;
	m_dwTitanSpellCheckTime = 0;
	m_dwTitanVampDelayCheckTime	= 0;
	m_bVamped	= FALSE;
}

CTitan::~CTitan(void)
{
}

void CTitan::InitTitan( TITAN_TOTALINFO* pInfo, CPlayer* pOwner )
{
	memcpy(&m_TitanTotalInfo, pInfo, sizeof(TITAN_TOTALINFO));
	m_pOwner = pOwner;
	m_pBaseInfo = GAMERESRCMNGR->GetTitanListInfo(pInfo->TitanKind);
	m_dwTitanSpellCheckTime = gCurTime;
	m_dwTitanVampDelayCheckTime	= gCurTime;
	m_bVamped	= FALSE;
}

titan_calc_stats* CTitan::GetItemStats()
{
	if(!m_pOwner)
	{
		ASSERT(0);
		return NULL;
	}

	titan_calc_stats* pStats = m_pOwner->GetTitanManager()->GetTitanItemStats();

	return pStats;
}

titan_calc_stats* CTitan::GetTitanStats()
{
	if(!m_pOwner)
	{
		ASSERT(0);
		return NULL;
	}

	titan_calc_stats* pStats = m_pOwner->GetTitanManager()->GetTitanStats();

	return pStats;
}

void CTitan::Process()
{
	if( gCurTime - m_dwTitanSpellCheckTime < TITAN_STATE_CHECKTIME )
		return;

	// 지속적 마력 소모
	ReduceSpellConstantly();

	m_dwTitanSpellCheckTime = gCurTime;
}

void CTitan::SendTitanInfoMsg()
{
	if( !m_pOwner )
	{
		char temp[256] = {0,};
		sprintf(temp, "Error!! <SendTitanInfoMsg> pOwner is NULL. MasterId : %d	PetDBId : %d",
			m_TitanTotalInfo.TitanMasterUserID, m_TitanTotalInfo.TitanDBIdx);
		ASSERTMSG(0, temp);
		return;
	}

	MSG_DWORD2	msg;
	msg.Category = MP_TITAN;
	msg.Protocol = MP_TITAN_VALUEINFO;
	msg.dwObjectID = m_pOwner->GetID();
	msg.dwData1 = m_TitanTotalInfo.Fuel;
	msg.dwData2 = m_TitanTotalInfo.Spell;

	m_pOwner->SendMsg(&msg, sizeof(msg));
}

void CTitan::ReduceSpell( DWORD rdcSpellAmount )
{
	DWORD life = GetLife();
	if( life < rdcSpellAmount )
	{
		SetLife( 0 );
	}
	else
	{
		SetLife( life - rdcSpellAmount );
	}

/*	if( GetNaeRyuk() < rdcSpellAmount )
	{
		SetNaeRyuk(0);
	}
	else
		SetNaeRyuk( GetNaeRyuk() - rdcSpellAmount );
		*/
}

void CTitan::ReduceSpellConstantly()
{
	CTitanManager* pTmgr = m_pOwner->GetTitanManager();
	DWORD reduceAmount = pTmgr->GetTitanSpellDecrease();
	// !!!장착템 옵션으로부터 가감.

	if(reduceAmount)
	{
		ReduceSpell(reduceAmount);
	}
}

BOOL CTitan::CheckLifeForVamp( DWORD fuel )
{	//줄었을 때에만 호출.
	TITAN_RULE* pRule = GAMERESRCMNGR->GetTitanRule();

	if( m_bVamped && gCurTime - m_dwTitanVampDelayCheckTime < pRule->TitanVampValidTime )
		return FALSE;

	WORD rate = WORD(fuel * 100 / DoGetMaxLife());

	if( 0 > rate || 100 < rate )
	{
		ASSERT(0);
	}

	if( rate <= pRule->TitanFuelRate_ForVamp )	//TitanFuelRate_ForVamp 5
	{
		// 주인의 체력 상태 확인후
		WORD ownerLifeRate = WORD( m_pOwner->GetLife() * 100 / m_pOwner->GetMaxLife() );

		if( 0 == ownerLifeRate || 100 < ownerLifeRate )
		{
			ASSERT(0);
		}

		if( ownerLifeRate >= pRule->MasterLifeRate_AllowForTitanVamp )	//MasterLifeRate_AllowForTitanVamp 80
		{
			VampOwnerLife(pRule->MasterLifeRate_FromTitanVamp);	//MasterLifeRate_FromTitanVamp 50
			m_bVamped = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

void CTitan::VampOwnerLife( DWORD vampRate )
{
	TITAN_RULE* pRule = GAMERESRCMNGR->GetTitanRule();

	DWORD vampLife = m_pOwner->GetLife() * vampRate / 100;

	m_pOwner->SetLife(m_pOwner->GetLife() - vampLife);

	DWORD convertLife = vampLife * pRule->FuelConvertRate_FromTitanVamp / 100;

	this->SetLife( GetLife() + convertLife );

	// 재흡수 딜레이. 흡수 일어날 때에만 세팅.
	m_dwTitanVampDelayCheckTime = gCurTime;
}

void CTitan::SetLife( DWORD fuel, BOOL bSendMsg )
{
	DWORD MaxLife = DoGetMaxLife();
	if( fuel > MaxLife )
		fuel = MaxLife;

	int chageAmount = fuel - GetLife();
	
	if( TRUE == bSendMsg && chageAmount/*GetLife() != fuel*/ )
	{
		MSG_INT msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_FUEL_ACK;
		msg.dwObjectID = m_pOwner->GetID();
		//msg.nData = fuel - GetLife();
		msg.nData = fuel;
		m_pOwner->SendMsg(&msg, sizeof(msg));
	}

	m_TitanTotalInfo.Fuel = fuel;

	/* //SW070912 기획변경
	if( chageAmount < 0 && fuel != 0 )	// 연료 감소시.
	{
		CheckLifeForVamp( fuel );
	}*/

	if( 0 == fuel )
	{
		// 타이탄 강제 해제.
		CTitanManager* pTitanMgr = m_pOwner->GetTitanManager();

		pTitanMgr->GetOffTitan(eExhaustFuel);

	}
}

DWORD CTitan::GetLife()
{
	return m_TitanTotalInfo.Fuel;
}

void CTitan::SetNaeRyuk( DWORD spell ,BOOL bSendMsg )
{
	DWORD MaxNaeryuk = DoGetMaxNaeRyuk();
	if( spell > MaxNaeryuk )
		spell = MaxNaeryuk;

	if( TRUE == bSendMsg && GetNaeRyuk() != spell )
	{
		MSG_INT msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_SPELL_ACK;
		msg.dwObjectID = m_pOwner->GetID();
		//msg.nData = spell - GetNaeRyuk();
		msg.nData = spell;
		m_pOwner->SendMsg(&msg, sizeof(msg));
	}

	m_TitanTotalInfo.Spell = spell;

	if( 0 == spell )
	{
		// 타이탄 강제 해제.
		CTitanManager* pTitanMgr = m_pOwner->GetTitanManager();

		pTitanMgr->GetOffTitan(eExhaustSpell);

	}
}

DWORD CTitan::GetNaeRyuk()
{
	return m_TitanTotalInfo.Spell;
}

DWORD CTitan::DoGetMaxLife()
{
	//DWORD maxfuel = m_pBaseInfo->GradeInfo[m_TitanTotalInfo.TitanGrade-1].MaxFuel + GetItemStats()->MaxFuel;
	DWORD maxfuel = GetTitanStats()->MaxFuel;
	return maxfuel;
}

DWORD CTitan::DoGetMaxNaeRyuk()
{
	//DWORD maxSpell = m_pBaseInfo->GradeInfo[m_TitanTotalInfo.TitanGrade-1].MaxSpell + GetItemStats()->MaxSpell;
	DWORD maxSpell = GetTitanStats()->MaxSpell;
	return maxSpell;
}

DWORD CTitan::DoGetPhyDefense()
{
	return GetTitanStats()->PhysicalDefense;
}

DWORD CTitan::DoGetPhyAttackPowerMin()
{
	titan_calc_stats* pStats = GetTitanStats();
	WORD ownerGenGol = m_pOwner->GetGenGol();

	WORD WeaponKind = m_pOwner->GetWeaponEquipType();

	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	WORD ownerStat = 0;

	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		ownerStat = m_pOwner->GetMinChub();
	}
	else
	{
		ownerStat = m_pOwner->GetGenGol();
	}

	DWORD pwr = (DWORD)(( pStats->MeleeAttackPowerMin * (ownerStat + 100)/400 + ownerStat ) * 0.74f);

	return pwr;
}

DWORD CTitan::DoGetPhyAttackPowerMax()
{
	titan_calc_stats* pStats = GetTitanStats();
	WORD ownerGenGol = m_pOwner->GetGenGol();

	WORD WeaponKind = m_pOwner->GetWeaponEquipType();

	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	WORD ownerStat = 0;

	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		ownerStat = m_pOwner->GetMinChub();
	}
	else
	{
		ownerStat = m_pOwner->GetGenGol();
	}

	DWORD pwr = (DWORD)(( pStats->MeleeAttackPowerMax * (ownerStat + 100)/400 + ownerStat ) * 0.74f);

	return pwr;
}

float CTitan::DoGetAttDefense( WORD Attrib )
{
	return GetTitanStats()->AttributeResist.GetElement_Val(Attrib);
}

void CTitan::DoDie( CObject* pAttacker )
{

}

void CTitan::DoDamage( CObject* pAttacter, RESULTINFO* pDamageInfo, DWORD beforeLife )
{
	//
}

DWORD CTitan::Damage( CObject* pAttacker, RESULTINFO* pDamageInfo )
{
	DWORD fuel = GetLife();
	DWORD beforefuel = fuel;

	if( fuel > pDamageInfo->TitanObsorbDamage )
	{
		fuel -= pDamageInfo->TitanObsorbDamage;
	}
	else
	{
		fuel = 0;
	}

	if( IsVampTitanfuelBySetdamage(fuel,FALSE) )
		pDamageInfo->TitanObsorbDamage = 0;


	// 장착템 내구도 하락
	CTitanManager* pTmgr = m_pOwner->GetTitanManager();
	pTmgr->DoRandomDecrease(AtDef);

	DoDamage(pAttacker,pDamageInfo,beforefuel);

	return fuel;
}

BOOL CTitan::IsVampTitanfuelBySetdamage( DWORD fuel,BOOL bSendMsg )
{
	DWORD MaxLife = DoGetMaxLife();
	if( fuel > MaxLife )
		fuel = MaxLife;

	int chageAmount = fuel - GetLife();

	if( TRUE == bSendMsg && chageAmount/*GetLife() != fuel*/ )
	{
		MSG_INT msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_FUEL_ACK;
		msg.dwObjectID = m_pOwner->GetID();
		//msg.nData = fuel - GetLife();
		msg.nData = fuel;
		m_pOwner->SendMsg(&msg, sizeof(msg));
	}

	m_TitanTotalInfo.Fuel = fuel;

	if( chageAmount < 0 && fuel != 0 )	// 연료 감소시.
	{
		if( CheckLifeForVamp( fuel ) )
			return TRUE;
	}

	if( 0 == fuel )
	{
		// 타이탄 강제 해제.
		CTitanManager* pTitanMgr = m_pOwner->GetTitanManager();

		pTitanMgr->GetOffTitan(eExhaustFuel);
	}

	return FALSE;
}