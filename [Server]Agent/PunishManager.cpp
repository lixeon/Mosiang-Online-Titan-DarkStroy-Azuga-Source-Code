#include "stdafx.h"
#include "PunishManager.h"


CPunishUnit::CPunishUnit(void)
{
}

CPunishUnit::~CPunishUnit(void)
{
}

void CPunishUnit::Init( DWORD dwUserIdx, int nPunishKind, DWORD dwEndTime )
{
	m_dwUserIdx	= dwUserIdx;
	m_nPunishKind = nPunishKind;
	m_dwEndTime = dwEndTime;
}

BOOL CPunishUnit::IsTimeEnd()
{
	if( gCurTime > m_dwEndTime )
		return TRUE;

	return FALSE;
}

DWORD CPunishUnit::GetRemainTime()
{
	if( gCurTime > m_dwEndTime )
		return 0;

	return m_dwEndTime - gCurTime;
}


//===================================================================//

GLOBALTON(CPunishManager)

CPunishManager::CPunishManager(void)
{
}

CPunishManager::~CPunishManager(void)
{
}

void CPunishManager::Init()
{
	m_dwAutoNoteUseTime = 60;	// Ка
    m_dwAutoBlockTime = 60;		// Ка

	m_pmpPunishUnit = new CMemoryPoolTempl< CPunishUnit >;
	m_pmpPunishUnit->Init( 100, 100, "CPunishUnit" );

	for( int i = 0 ; i < ePunish_Max ; ++i )
		m_htPunishUnit[i].Initialize( 50 );
}

void CPunishManager::Release()
{
	m_pmpPunishUnit->Release();
	delete m_pmpPunishUnit;

	for( int i = 0 ; i < ePunish_Max ; ++i )
		m_htPunishUnit[i].RemoveAll();
}

void CPunishManager::Process()
{
	static DWORD dwLastCheck = gCurTime;

	if( gCurTime - dwLastCheck < 1000 ) return;
	dwLastCheck = gCurTime;

	for( int i = 0 ; i < ePunish_Max ; ++i )
	{
		m_htPunishUnit[i].SetPositionHead();
		while( CPunishUnit* pPunishUnit = m_htPunishUnit[i].GetData() )
		{
			if( pPunishUnit->IsTimeEnd() )
			{
				m_htPunishUnit[i].Remove( pPunishUnit->GetUserIdx() );
				m_pmpPunishUnit->Free( pPunishUnit );
				break;
			}
		}
	}
}

void CPunishManager::AddPunishUnit( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime )
{
	CPunishUnit* pPunishUnit = GetPunishUnit( dwUserIdx, nPunishKind );
	DWORD milisec = dwPunishTime * 1000;

	if( pPunishUnit )
	{
		pPunishUnit->Init( dwUserIdx, nPunishKind, gCurTime + milisec );
	}
	else
	{
		CPunishUnit* pPunishUnitAdd = m_pmpPunishUnit->Alloc();
		pPunishUnitAdd->Init( dwUserIdx, nPunishKind, gCurTime + milisec );
		m_htPunishUnit[nPunishKind].Add( pPunishUnitAdd, dwUserIdx );
	}
}

void CPunishManager::RemovePunishUnit( DWORD dwUserIdx, int nPunishKind )
{
	CPunishUnit* pPunishUnit = GetPunishUnit( dwUserIdx, nPunishKind );
	if( pPunishUnit )
	{
		m_htPunishUnit[nPunishKind].Remove( pPunishUnit->GetUserIdx() );
		m_pmpPunishUnit->Free( pPunishUnit );
	}
}

void CPunishManager::RemovePunishUnitAll( DWORD dwUserIdx )
{
	for( int i = 0 ; i < ePunish_Max ; ++i )
	{
		CPunishUnit* pPunishUnit = GetPunishUnit( dwUserIdx, i );
		if( pPunishUnit )
		{
			m_htPunishUnit[i].Remove( pPunishUnit->GetUserIdx() );
			m_pmpPunishUnit->Free( pPunishUnit );
		}
	}
}

CPunishUnit* CPunishManager::GetPunishUnit( DWORD dwUserIdx, int nPunishKind )
{
	return m_htPunishUnit[nPunishKind].GetData( dwUserIdx );
}