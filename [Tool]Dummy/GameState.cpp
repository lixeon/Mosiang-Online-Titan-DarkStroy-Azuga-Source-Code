// GameState.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameState::CGameState()
{
}

CGameState::~CGameState()
{
}

void CGameState::CommonNetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )
{
	switch( Category )
	{
	case MP_USERCONN:
		{
			switch( Protocol )
			{
			case MP_USERCONN_CONNECTION_CHECK:
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CONNECTION_CHECK_OK;
					m_pNetwork->Send(&msg,sizeof(msg));
				}
			}
		}
	}
}
