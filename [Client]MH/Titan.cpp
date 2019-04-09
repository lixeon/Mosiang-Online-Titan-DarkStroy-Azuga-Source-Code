#include "stdafx.h"
#include ".\titan.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "TitanGuageDlg.h"

CTitan::CTitan(void)
{
}

CTitan::~CTitan(void)
{
}

void CTitan::InitTitan( TITAN_TOTALINFO* pTotalInfo )
{
	memcpy(&m_TitanTotalInfo, pTotalInfo, sizeof(TITAN_TOTALINFO));
	m_pSInfo = GAMERESRCMNGR->GetTitanListInfo(m_TitanTotalInfo.TitanKind);
}

void CTitan::SetLife(DWORD dwLife)
{
	m_TitanTotalInfo.Fuel = dwLife;
	GAMEIN->GetTitanGuageDlg()->SetLife(dwLife);
}

DWORD CTitan::GetLife()
{
	return m_TitanTotalInfo.Fuel;
}

void CTitan::SetNaeRyuk(DWORD dwNaeRyuk)
{
	m_TitanTotalInfo.Spell = dwNaeRyuk;
	GAMEIN->GetTitanGuageDlg()->SetNaeRyuk(dwNaeRyuk);
}

DWORD CTitan::GetNaeRyuk()
{
	return m_TitanTotalInfo.Spell;
}