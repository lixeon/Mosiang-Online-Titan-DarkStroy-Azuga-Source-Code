// UserIPCheckMgr.cpp: implementation of the CUserIPCheckMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserIPCheckMgr.h"
#include "MonitorNetwork.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CUserIPCheckMgr)

CUserIPCheckMgr::CUserIPCheckMgr()
{
	memset( m_IPArray, 0, sizeof(IP)*MAX_IPARRAY );
	m_nMaxIP = 0;

	memset( m_IDArray, 0, sizeof(OPERID)*MAX_IPARRAY );
}

CUserIPCheckMgr::~CUserIPCheckMgr()
{
}

BOOL CUserIPCheckMgr::IsThereIP( char* pIP )
{
	if( m_nMaxIP == 0 )
		return FALSE;

	for( int i = 0; i < m_nMaxIP; ++i )
	{
		if( strcmp( m_IPArray[i].sIP, pIP ) == 0 )
			return TRUE;
	}
	
	return FALSE;
}

void CUserIPCheckMgr::RegistIP( char* pIP )
{
	if( IsThereIP( pIP ) )
		return;

	strcpy( m_IPArray[m_nMaxIP].sIP, pIP );
	++m_nMaxIP;
}

void CUserIPCheckMgr::UnRegistIP( char* pIP )
{
	for( int i = 0; i < m_nMaxIP; ++i )
	{
		if( strcmp( m_IPArray[i].sIP, pIP ) == 0 )
		{
			--m_nMaxIP;
			m_IPArray[i] = m_IPArray[m_nMaxIP];
			return;
		}
	}
}

void CUserIPCheckMgr::Release()
{
	memset( m_IPArray, 0, sizeof(IPINFO)*MAX_IPARRAY );
	m_nMaxIP = 0;
}

BOOL CUserIPCheckMgr::CheckIP( DWORD dwConnectionIndex )
{
	char strIP[32] = {0,};
	WORD wPort;
	MNETWORK->GetUserAddress( dwConnectionIndex, strIP, &wPort );

	if( IsThereIP( strIP ) )
		return TRUE;

	MSGROOT msg;
	msg.Category = MP_RMTOOL_CONNECT;
	msg.Protocol = MP_CHECKIP_ERROR;
	
	MNETWORK->SendToMC( dwConnectionIndex, (char*)&msg, sizeof(MSGROOT) );

	return FALSE;
}

void CUserIPCheckMgr::RegistID( DWORD dwConnectionIdx, DWORD dwIdx, char* pName )
{
	if( dwConnectionIdx > 99 )
		return;

	m_IDArray[dwConnectionIdx].dwIdx = dwIdx;
	strcpy( m_IDArray[dwConnectionIdx].sID, pName );
}

OPERID* CUserIPCheckMgr::GetID( DWORD dwConnectionIdx )
{
	if( dwConnectionIdx > 99 )
		return NULL;

	return &m_IDArray[dwConnectionIdx];
}