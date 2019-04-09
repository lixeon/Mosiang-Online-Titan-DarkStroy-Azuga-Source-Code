// GMPowerList.cpp: implementation of the CGMPowerList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMPowerList.h"
#include "Network.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGMPowerList);

CGMPowerList::CGMPowerList()
{
	m_bMonitorCheat = FALSE;
	m_bEvent1Start	= FALSE;
}

CGMPowerList::~CGMPowerList()
{
	Release();
}

void CGMPowerList::Init()
{
	FILE* fp = fopen( "./MonitorRevolution.gmp", "r" );
	if( fp )
	{
		m_bMonitorCheat = TRUE;
		fclose(fp);
	}
}

void CGMPowerList::AddGMList( DWORD dwConnectionIdx, int nPower, DWORD dwIndex, char* strID )
{
	GM_INFO* info = new GM_INFO;
	info->dwConnectionIndex		= dwConnectionIdx;
	info->nPower				= nPower;
	info->dwGMIndex				= dwIndex;
	SafeStrCpy( info->szGMID, strID, MAX_NAME_LENGTH+1 );

//국내 Monitor를 위해 임시 코드
	if( nPower == eGM_POWER_MONITOR )
	if( m_bMonitorCheat == TRUE )
	{
		info->nPower = eGM_POWER_MASTER;
	}

	m_listGPL.AddTail( info );	
}

void CGMPowerList::RemoveGMList( DWORD dwConnectionIdx )
{
	PTRLISTPOS pos = m_listGPL.GetHeadPosition();
	while(pos)
	{
		GM_INFO* info = (GM_INFO*)m_listGPL.GetNext(pos);
		if( info )
		if( info->dwConnectionIndex == dwConnectionIdx )
		{
			m_listGPL.Remove( info );
			delete info;
			break;
		}
	}
}

void CGMPowerList::Release()
{
	PTRLISTPOS pos = m_listGPL.GetHeadPosition();
	while(pos)
	{
		GM_INFO* info = (GM_INFO*)m_listGPL.GetNext(pos);
		if( info )
			delete info;
	}

	m_listGPL.RemoveAll();
}

int CGMPowerList::GetGMPower( DWORD dwConnectionIdx )
{
	PTRLISTPOS pos = m_listGPL.GetHeadPosition();
	while(pos)
	{
		GM_INFO* info = (GM_INFO*)m_listGPL.GetNext(pos);
		if( info )
		{
			if( info->dwConnectionIndex == dwConnectionIdx )
			{
				return info->nPower;
			}
		}
	}	
	return -1;
}

GM_INFO* CGMPowerList::GetGMInfo( DWORD dwConnectionIdx )
{
	PTRLISTPOS pos = m_listGPL.GetHeadPosition();
	while(pos)
	{
		GM_INFO* info = (GM_INFO*)m_listGPL.GetNext(pos);
		if( info )
		{
			if( info->dwConnectionIndex == dwConnectionIdx )
			{
				return info;
			}
		}
	}
	return NULL;
}

void CGMPowerList::SendGMLoginSuccess( DWORD dwObjectID, int nPower )
{
	MSG_DWORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_GM_LOGIN_ACK;
	msg.dwObjectID	= dwObjectID;
	msg.dwData		= (DWORD)nPower;
	g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
}

void CGMPowerList::SendGMLoginFail( DWORD dwObjectID )
{
	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
	msg.dwObjectID	= dwObjectID;
	g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
}
