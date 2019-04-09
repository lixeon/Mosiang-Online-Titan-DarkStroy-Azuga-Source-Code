// SocietyActManager.cpp: implementation of the CSocietyActManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocietyActManager.h"
#include "ObjectManager.h"
#include "../[CC]Ability/AbilityUse_Society.h"
#include "../[CC]Ability/AbilityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSocietyActManager);
CSocietyActManager::CSocietyActManager()
{

}

CSocietyActManager::~CSocietyActManager()
{

}

void CSocietyActManager::SendSocietyActMsg( WORD wAcionIdx )
{
	MSG_WORD msg;
	msg.Category	= MP_SOCIETYACT;
	msg.Protocol	= MP_SOCIETYACT_ACT_SYN;
	msg.dwObjectID	= HEROID;
	msg.wData		= wAcionIdx;

	NETWORK->Send( &msg, sizeof(msg) );
}

void CSocietyActManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_SOCIETYACT_ACT:
		{
			MSG_DWORD_WORD* pmsg = (MSG_DWORD_WORD*)pMsg;
			CObject* pObject = (CObject*)OBJECTMGR->GetObject( pmsg->dwData );
			if( pObject == NULL )
				return;

			CAbilityUse_Society use;
			use.Act( pObject, ABILITYMGR->GetAbilityInfo( pmsg->wData ) );
		}
		break;
	}
}