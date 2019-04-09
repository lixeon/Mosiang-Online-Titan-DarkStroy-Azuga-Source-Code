// OptionManager.cpp: implementation of the OptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionManager.h"

#include "UserTable.h"
#include "Player.h"
#include "PackedData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionManager::COptionManager()
{

}

COptionManager::~COptionManager()
{

}


void COptionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_OPTION_SET_SYN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);

			if( pPlayer == NULL ) break;	//errorÃ³¸®

			sGAMEOPTION GameOption;
			MakeGameOption( msg->wData, &GameOption );
			pPlayer->SetGameOption( &GameOption );
			if( GameOption.bNoAvatarView != pPlayer->GetAvatarView() )
			{
				MSG_DWORDBYTE msg;
				SetProtocol( &msg, MP_OPTION, MP_OPTION_AVATARVIEW );
				msg.dwData = pPlayer->GetID();
				msg.bData = GameOption.bNoAvatarView;
				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
			}
			pPlayer->SetAvatarView( GameOption.bNoAvatarView );
		}
		break;
	}
}

void COptionManager::MakeGameOption( WORD wOption, sGAMEOPTION* pOutGameOption )
{
	//ÃÊ±âÈ­»ý·«(pOutGameOption)
	if( wOption & eOPT_NODEAL )		pOutGameOption->bNoDeal = TRUE;
	if( wOption & eOPT_NOPARTY )	pOutGameOption->bNoParty = TRUE;
//	if( wOption & eOPT_NOFRIEND )	pOutGameOption->bNoFriend = TRUE;
	if( wOption & eOPT_NOVIMU )		pOutGameOption->bNoVimu = TRUE;
//	if( wOption & eOPT_NOWHISPER )	pOutGameOption->bNoWhisper = TRUE;
	if( wOption & eOPT_NOAVATARVIEW)	pOutGameOption->bNoAvatarView = TRUE;
}
