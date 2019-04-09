// HackCheck.cpp: implementation of the CHackCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HackCheck.h"
#include "ExitManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MHTimeManager.h"

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CHackCheck);

CHackCheck::CHackCheck()
{
	m_bIsHackUser	= FALSE;
	m_dwLastTime	= gCurTime;
}

CHackCheck::~CHackCheck()
{
	PTRLISTPOS pos = m_ListSpeedHackCheck.GetHeadPosition();
	while( pos )
	{
		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetNext( pos );
		delete pTime;
	}
	m_ListSpeedHackCheck.RemoveAll();
}

void CHackCheck::Release()
{
	PTRLISTPOS pos = m_ListSpeedHackCheck.GetHeadPosition();
	while( pos )
	{
		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetNext( pos );
		delete pTime;
	}
	m_ListSpeedHackCheck.RemoveAll();
	
	m_bIsHackUser	= FALSE;
	m_dwLastTime	= gCurTime;
}

void CHackCheck::Process()
{
	if( m_ListSpeedHackCheck.IsEmpty() )
		return;

	if( gCurTime - m_dwLastTime >= SPEEDHACK_CHECKTIME )	//gCurTime이 옛날시간(작은시간)이라도 상관없다.
	{

//#ifdef _NPROTECT_
//		NPROTECTMGR->Check();
//#endif

		sCHECKTIME* pTime = (sCHECKTIME*)m_ListSpeedHackCheck.GetHead();
		if( pTime )
		{
			MSG_DWORD msg;
			msg.Category	= MP_HACKCHECK;
			msg.Protocol	= MP_HACKCHECK_SPEEDHACK;
			msg.dwObjectID	= HEROID;
			msg.dwData		= pTime->dwServerSendTime;
			NETWORK->Send( &msg, sizeof( msg ) );

			m_ListSpeedHackCheck.RemoveHead();
			delete pTime;

			if( !m_ListSpeedHackCheck.IsEmpty() )
			{
				m_dwLastTime = ((sCHECKTIME*)m_ListSpeedHackCheck.GetHead())->dwArrivedTime;
			}
		}
	}
}

void CHackCheck::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_HACKCHECK_SPEEDHACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			sCHECKTIME* pTime = new sCHECKTIME;
			//pTime->dwArrivedTime = gCurTime;	//여기에 옛날 시간이?
			pTime->dwArrivedTime	= MHTIMEMGR->GetNewCalcCurTime();
			pTime->dwServerSendTime = pmsg->dwData;
			m_ListSpeedHackCheck.AddTail( pTime );
			m_dwLastTime = ((sCHECKTIME*)m_ListSpeedHackCheck.GetHead())->dwArrivedTime;
		}
		break;
	case MP_HACKCHECK_BAN_USER:
		{
			m_bIsHackUser = TRUE;
			//타이머 10초 이후에 종료해야 하는건가?
		}
		break;
	}
}
