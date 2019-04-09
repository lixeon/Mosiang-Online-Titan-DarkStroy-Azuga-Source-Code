// GTRegistcancelDialog.cpp: implementation of the CGTRegistcancelDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTRegistcancelDialog.h"
#include "WindowIDEnum.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTRegistcancelDialog::CGTRegistcancelDialog()
{
	m_type = WT_GTREGISTCANCEL_DLG;

}

CGTRegistcancelDialog::~CGTRegistcancelDialog()
{

}


void CGTRegistcancelDialog::Linking()
{
	m_pCancelBtn = (cButton*)GetWindowForID( GDT_CANCELBTN );
}


void CGTRegistcancelDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
}


void CGTRegistcancelDialog::TournamentRegistCancelSyn()
{
	MSGBASE msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_REGISTCANCEL_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send( &msg, sizeof(msg) );
}
