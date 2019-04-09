// PartyWar.cpp: implementation of the CPartyWar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyWar.h"

#include "PartyManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "TileManager.h"
#include "MHMap.h"
#include "GameIn.h"
#include "PartyWarDialog.h"
#include "VimuManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPartyWar)

CPartyWar::CPartyWar()
{
	memset( &m_EnemyPartyInfo, 0, sizeof(m_EnemyPartyInfo) );
	memset( m_MemberIdx, 0 , sizeof(DWORD)*MAX_PARTY_LISTNUM );
	m_nWarState = ePartyWar_Null;
	m_dwWarTime = 0;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;
}

CPartyWar::~CPartyWar()
{
}

void CPartyWar::Init()
{
	memset( &m_EnemyPartyInfo, 0, sizeof(m_EnemyPartyInfo) );
	memset( m_MemberIdx, 0 , sizeof(DWORD)*MAX_PARTY_LISTNUM );
	m_nWarState = ePartyWar_Null;
	m_dwWarTime = 0;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;

	// image
	m_ImageNumber.Init( 32, 0 );
	m_ImageNumber.SetLimitCipher( 3 );
	SCRIPTMGR->GetImage( 60, &m_ImageStart, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 61, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 76, &m_ImageDraw, PFT_HARDPATH );

	m_vTitlePos.x = (1024 - 512) / 2;
	m_vTitlePos.y = 32;
	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;

	m_pCurShowImage = NULL;
	//
}

void CPartyWar::Release()
{
	memset( &m_EnemyPartyInfo, 0, sizeof(m_EnemyPartyInfo) );
	memset( m_MemberIdx, 0 , sizeof(DWORD)*MAX_PARTY_LISTNUM );
	m_nWarState = ePartyWar_Null;
	m_dwWarTime = 0;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;
}

BOOL CPartyWar::IsEnemy( CPlayer* pTarget )
{
	if( m_nWarState != ePartyWar_Fight )	return FALSE;

	DWORD dwEnemyId = pTarget->GetID();
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_EnemyPartyInfo.Member[i].bLogged )
		if( m_EnemyPartyInfo.Member[i].dwMemberID == dwEnemyId )
			return TRUE;
	}

	return FALSE;
}

void CPartyWar::SetPartyWarMember( DWORD dwMemberIdx, int nIndex, BOOL bParty, BOOL bAdd )
{
	if( bParty )
	{
		if( IsPartyMember( dwMemberIdx, nIndex ) )
		{
			if( bAdd )
			{
				m_MemberIdx[nIndex] = dwMemberIdx;
				GAMEIN->GetPartyWarDlg()->AddPartyWarMember( nIndex );
			}
			else
			{
				m_MemberIdx[nIndex] = 0;
				GAMEIN->GetPartyWarDlg()->RemovePartyWarMember( nIndex );
			}
		}
	}
	else
	{
		if( IsEnemyPartyMember( dwMemberIdx, nIndex ) )
		{
			if( bAdd )
			{
				m_EnemyPartyInfo.Member[nIndex].bLogged = TRUE;
				GAMEIN->GetPartyWarDlg()->AddPartyWarMember( nIndex + MAX_PARTY_LISTNUM );
			}
			else
			{
				m_EnemyPartyInfo.Member[nIndex].bLogged = FALSE;
				GAMEIN->GetPartyWarDlg()->RemovePartyWarMember( nIndex + MAX_PARTY_LISTNUM );
			}
		}
	}	
}

BOOL CPartyWar::IsPartyMember( DWORD dwMemberIdx, int nIndex )
{
	PARTY_MEMBER* pMember = PARTYMGR->GetPartyMemberInfo( nIndex );
	if( pMember->dwMemberID == dwMemberIdx )	return TRUE;
	return FALSE;
}

BOOL CPartyWar::IsPartyMember( int nIndex )
{
	PARTY_MEMBER* pMember = PARTYMGR->GetPartyMemberInfo( nIndex );
	if( pMember->dwMemberID != 0 )	return TRUE;
	return FALSE;
}

BOOL CPartyWar::IsEnemyPartyMember( DWORD dwMemberIdx, int nIndex )
{
	if( m_EnemyPartyInfo.Member[nIndex].dwMemberID == dwMemberIdx )	return TRUE;
	return FALSE;
}

BOOL CPartyWar::IsEnemyPartyMember( int nIndex )
{
	if( m_EnemyPartyInfo.Member[nIndex].dwMemberID != 0 )	return TRUE;
	return FALSE;
}

BOOL CPartyWar::IsMaster()
{
	if( HEROID != PARTYMGR->GetMasterID() )
		return FALSE;
	return TRUE;
}

BOOL CPartyWar::IsPartyWar()
{
	if( m_nWarState != ePartyWar_Null )
		return TRUE;
	return FALSE;
}

void CPartyWar::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_PARTYWAR_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch( pmsg->dwData )
			{
			case 1:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 871 ) );			break;
			case 2:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 872 ) );			break;
			case 3:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 873 ) );			break;
			case 4:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 874 ) );			break;
			case 5:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 875 ) );			break;
			case 6:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 876 ) );			break;
			case 7:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 877 ) );			break;
			case 8:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 878 ) );			break;
			case 9:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );			break;
			case 10:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );			break;
			case 11:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 880 ) );			break;
			case 99:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 866 ) );			break;
			}

			PartyWarEnd();			
		}
		break;
	case MP_PARTYWAR_SUGGEST:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;			
			WINDOWMGR->MsgBox( MBI_PARTYWAR_SUGGEST, MBT_YESNO, CHATMGR->GetChatMsg( 861 ), pmsg->Name );
			
			m_nWarState = ePartyWar_PreWait;
		}
		break;
	case MP_PARTYWAR_SUGGEST_WAIT:
		{		
			m_nWarState = ePartyWar_PreWait;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 862 ) );
		}
		break;
	case MP_PARTYWAR_SUGGEST_ACCEPT:
		{
			PARTY_INFO* pmsg = (PARTY_INFO*)pMsg;
			PartyWarWait( pmsg );
		}
		break;
	case MP_PARTYWAR_SUGGEST_DENY:
		{
			PartyWarEnd();
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 863 ) );
		}
		break;		
	case MP_PARTYWAR_ADDMEMBER_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if( HERO->GetPartyIdx() == pmsg->dwData1 )
				SetPartyWarMember( pmsg->dwData2, pmsg->dwData3, TRUE, TRUE );
			else if( m_EnemyPartyInfo.PartyDBIdx == pmsg->dwData1 )
				SetPartyWarMember( pmsg->dwData2, pmsg->dwData3, FALSE, TRUE );
		}
		break;
	case MP_PARTYWAR_ADDMEMBER_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 864 ) );
		}
		break;
	case MP_PARTYWAR_REMOVEMEMBER_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			if( HERO->GetPartyIdx() == pmsg->dwData1 )
				SetPartyWarMember( pmsg->dwData2, pmsg->dwData3, TRUE, FALSE );
			else if( m_EnemyPartyInfo.PartyDBIdx == pmsg->dwData1 )
				SetPartyWarMember( pmsg->dwData2, pmsg->dwData3, FALSE, FALSE );
		}
		break;
	case MP_PARTYWAR_REMOVEMEMBER_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 865 ) );
		}
		break;		
	case MP_PARTYWAR_LOCK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HERO->GetPartyIdx() == pmsg->dwData )
				GAMEIN->GetPartyWarDlg()->SetLock( TRUE, IsMaster() );
			else if( m_EnemyPartyInfo.PartyDBIdx == pmsg->dwData )
				GAMEIN->GetPartyWarDlg()->SetLock( FALSE, IsMaster() );
		}
		break;
	case MP_PARTYWAR_UNLOCK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( HERO->GetPartyIdx() == pmsg->dwData || m_EnemyPartyInfo.PartyDBIdx == pmsg->dwData )
				GAMEIN->GetPartyWarDlg()->SetUnLock( IsMaster() );
		}
		break;
	case MP_PARTYWAR_CANCEL:
		{
			PartyWarEnd();		

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 866 ) );
		}
		break;
	case MP_PARTYWAR_READY:
		{
			MSG_BATTLE_VIMU_CREATESTAGE* pmsg = (MSG_BATTLE_VIMU_CREATESTAGE*)pMsg;
			PartyWarReady( pmsg );			
		}
		break;
	case MP_PARTYWAR_FIGHT:
		{
			PartyWarFight();
		}
		break;
	case MP_PARTYWAR_RESULT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PartyWarResult( pmsg );			
		}
		break;
	case MP_PARTYWAR_END:
		{
			MSG_NAME2_DWORD* pmsg = (MSG_NAME2_DWORD*)pMsg;
			if( pmsg->dwData == 0 )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 870 ), pmsg->Name1, pmsg->Name2 );
			else if( pmsg->dwData == 1 )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 869 ), pmsg->Name1, pmsg->Name2, pmsg->Name1 );
			else if( pmsg->dwData == 2 )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 869 ), pmsg->Name1, pmsg->Name2, pmsg->Name2 );

			if( PARTYMGR->IsPartyMember( pmsg->dwObjectID ) )
				PartyWarEnd();
		}
		break;	
	}
}

CPlayer* CPartyWar::CanPartyWarSuggest()
{
	// check state
	if( m_nWarState != ePartyWar_Null )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 877 ) );		
		return NULL;
	}
	// hero
	if( !HERO->GetPartyIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 872 ) );		
		return NULL;
	}
	if( !IsMaster() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 874 ) );		
		return NULL;
	}
	if( HERO->IsPKMode() || VIMUMGR->IsVimuing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );		
		return NULL;
	}
	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );		
		return NULL;
	}	
	// target	
	CObject* pTarget = OBJECTMGR->GetSelectedObject();	
	if( pTarget == NULL || pTarget == HERO )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 18 ) );		
		return NULL;
	}
	if( pTarget->GetObjectKind() != eObjectKind_Player )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 18 ) );		
		return NULL;
	}
	if( ((CPlayer*)pTarget)->IsPKMode() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );		
		return NULL;
	}
	if( pTarget->GetState() != eObjectState_None && pTarget->GetState() != eObjectState_Move )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 879 ) );		
		return NULL;
	}
	if( PARTYMGR->IsPartyMember( pTarget->GetID() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 873 ) );		
		return NULL;
	}
	// check distance
	VECTOR3 vPos1 = HERO->GetCurPosition();
	VECTOR3 vPos2 = pTarget->GetCurPosition();
	if( CalcDistanceXZ( &vPos1, &vPos2 ) > 1000.0f )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 881 ) );		
		return NULL;
	}

	return (CPlayer*)pTarget;
}

void CPartyWar::SendPartyWarSuggest()
{
	CPlayer* pTarget = CanPartyWarSuggest();
	if( !pTarget )	return;

	//if( MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP || MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP || MAP->IsMapKind(eSurvivalMap) )
		return;

	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_SUGGEST;
	Msg.dwObjectID = HEROID;
	Msg.dwData = pTarget->GetID();

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CPartyWar::SendPartyWarSuggestReturn( BYTE Protocol )
{
	if( !HERO->GetPartyIdx() )	return;
	if( !IsMaster() )			return; 

	MSGBASE Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = Protocol;
	Msg.dwObjectID = HEROID;
	
	NETWORK->Send( &Msg, sizeof(Msg) );
	
	if( Protocol == MP_PARTYWAR_SUGGEST_DENY )
		PartyWarEnd();
}

void CPartyWar::SendPartyWarMember( int nIndex )
{
	BOOL bAdd = GAMEIN->GetPartyWarDlg()->NoChangeCheckBoxState( nIndex );

	if( nIndex > 6 )							return;
	if( !IsMaster() )							return;
	if( !IsPartyMember( nIndex ) )				return;
	if( GAMEIN->GetPartyWarDlg()->IsLock() )	return;
	if( !PARTYMGR->IsMemberLogIn( nIndex ) )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = PARTYMGR->GetPartyMemberID( nIndex );
	Msg.dwData2 = (DWORD)nIndex;	
	if( bAdd )
		Msg.Protocol = MP_PARTYWAR_ADDMEMBER_SYN;
	else
		Msg.Protocol = MP_PARTYWAR_REMOVEMEMBER_SYN;

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CPartyWar::SendPartyWarLock( BOOL bLock )
{
	if( !HERO->GetPartyIdx() )	return;
	if( !IsMaster() )			return; 

	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.dwObjectID = HEROID;
	Msg.dwData = HERO->GetPartyIdx();
	if( bLock )
		Msg.Protocol = MP_PARTYWAR_LOCK;
	else
		Msg.Protocol = MP_PARTYWAR_UNLOCK;

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CPartyWar::SendPartyWarStart()
{
	if( !HERO->GetPartyIdx() )	return;
	if( !IsMaster() )			return; 

	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_START;
	Msg.dwObjectID = HEROID;
	Msg.dwData = HERO->GetPartyIdx();

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );	
}

void CPartyWar::SendPartyWarCancel()
{
	if( !HERO->GetPartyIdx() )	return;
	if( !IsMaster() )			return; 

	MSG_DWORD Msg;
	Msg.Category = MP_PARTYWAR;
	Msg.Protocol = MP_PARTYWAR_CANCEL;
	Msg.dwObjectID = HEROID;
	Msg.dwData = HERO->GetPartyIdx();

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CPartyWar::PartyWarWait( PARTY_INFO* pInfo )
{
	if( !HERO->GetPartyIdx() )	return;

	GAMEIN->GetPartyWarDlg()->ShowPartyWarDlg( IsMaster() );

	// info
	memcpy( &m_EnemyPartyInfo, pInfo, sizeof(PARTY_INFO) );
	for( int i = 0; i < MAX_PARTY_LISTNUM; ++i )
	{
		if( m_EnemyPartyInfo.Member[i].dwMemberID != 0 )
			GAMEIN->GetPartyWarDlg()->SetPartyMemberName( m_EnemyPartyInfo.Member[i].Name, i+MAX_PARTY_LISTNUM, pInfo->Member[i].bLogged );
		if( IsPartyMember( i ) )
			GAMEIN->GetPartyWarDlg()->SetPartyMemberName( PARTYMGR->GetPartyMemberInfo( i )->Name, i, PARTYMGR->IsMemberLogIn( i ) );

		m_EnemyPartyInfo.Member[i].bLogged = FALSE;
	}

	m_nWarState = ePartyWar_Wait;
	m_dwWarTime = gCurTime;
}

void CPartyWar::PartyWarReady( MSG_BATTLE_VIMU_CREATESTAGE* pMsg )
{
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pMsg->dwObjectID );
	if( !pPlayer )	return;

	if( HEROID == pMsg->dwObjectID )
	{
		m_nWarState = ePartyWar_Ready;
		m_dwWarTime = gCurTime;

		// stage
		m_vWarPos = pMsg->vPosStage;
		CreateStage();

		// image
		m_pCurShowImage = NULL;
		m_ImageNumber.SetFillZero( FALSE );
		m_ImageNumber.SetFadeOut( 0 );
		m_ImageNumber.SetScale( 3.0f, 3.0f );
		m_ImageNumber.SetPosition( 512 - 16, 80 );
	}	

	// effect
	if( pMsg->dwBattleID == 1 )
	{
		if( pPlayer->InTitan() )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
		}
	}
	else if( pMsg->dwBattleID == 2 )
	{
		if( pPlayer->InTitan() )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
		}
	}

	GAMEIN->GetPartyWarDlg()->HidePartyWarDlg();		
}

void CPartyWar::PartyWarFight()
{
	m_nWarState = ePartyWar_Fight;
	m_dwWarTime = gCurTime;

	// image
	m_pCurShowImage = &m_ImageStart;
	m_ImageNumber.SetFillZero( TRUE );
	m_ImageNumber.SetFadeOut( 0 );
	m_ImageNumber.SetScale( 1.0f, 1.0f );
	m_ImageNumber.SetPosition( 512 + 42, 130 );
}

void CPartyWar::PartyWarResult( MSG_DWORD* pMsg )
{
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pMsg->dwObjectID );
	if( !pPlayer )	return;

	if( HEROID == pMsg->dwObjectID )
	{
		m_nWarState = ePartyWar_Result;
		m_dwWarTime = gCurTime;

		// stage
		DestroyStage();

		// image
		if( pMsg->dwData == 0 )			m_pCurShowImage = &m_ImageLose;
		else if( pMsg->dwData == 1 )	m_pCurShowImage = &m_ImageWin;
		else if( pMsg->dwData == 2 )	m_pCurShowImage = &m_ImageDraw;		
	}
	
	// effect
	pPlayer->RemoveObjectEffect( BATTLE_TEAMEFFECT_ID );
}

void CPartyWar::PartyWarEnd()
{
	memset( &m_EnemyPartyInfo, 0, sizeof(m_EnemyPartyInfo) );
	memset( m_MemberIdx, 0 , sizeof(DWORD)*MAX_PARTY_LISTNUM );
	m_nWarState = ePartyWar_Null;
	m_dwWarTime = 0;
	m_vWarPos.x = m_vWarPos.y = m_vWarPos.z = 0.0f;

	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_PARTYWAR_SUGGEST );
	if( pMsgBox )	WINDOWMGR->AddListDestroyWindow( (cWindow*)pMsgBox );
	GAMEIN->GetPartyWarDlg()->HidePartyWarDlg();
	HERO->RemoveObjectEffect( PARTYWAR_STAGEEFFECT_ID );
}

void CPartyWar::Render()
{
	DWORD ElapsedTime = gCurTime - m_dwWarTime;

	switch( m_nWarState )
	{
	case ePartyWar_Wait:
		{
			DWORD RemainTime = 180000 > ElapsedTime ? 180000 - ElapsedTime : 0;

			GAMEIN->GetPartyWarDlg()->SetTime( RemainTime / 1000 );			
		}
		break;
	case ePartyWar_Ready:
		{
			DWORD RemainTime = 10000 > ElapsedTime ? 10000 - ElapsedTime : 0;

			m_ImageNumber.SetNumber( RemainTime / 1000 );
			if( m_ImageNumber.IsNumberChanged() )
				m_ImageNumber.SetFadeOut( 500 );
			m_ImageNumber.Render();
		}
		break;
	case ePartyWar_Fight:
		{
			DWORD RemainTime = 120000 > ElapsedTime ? 120000 - ElapsedTime : 0;

			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );			
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			m_ImageNumber.Render();				
		}
		break;
	case ePartyWar_Result:
		{
			if( ElapsedTime > 2000 )
			{
				if( m_pCurShowImage )
					m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );
			}
			if( ElapsedTime > 5000 )
				PartyWarEnd();
		}
		break;			
	}	
}

void CPartyWar::CreateStage()
{
	CTileManager* pTileManager = MAP->GetTileManager();

	VECTOR3 vStgPos;
	float fx, fz = m_vWarPos.z;
	float fAdd = -50.0f;

	for( fx = m_vWarPos.x - WARSTAGE; fx <= m_vWarPos.x + WARSTAGE + fAdd; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - WARSTAGE;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + WARSTAGE + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = m_vWarPos.x;
	for( fz = m_vWarPos.z - WARSTAGE; fz <= m_vWarPos.z + WARSTAGE + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - WARSTAGE;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + WARSTAGE + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fz = m_vWarPos.z;
	for( fx = m_vWarPos.x - WARSTAGEBORDER; fx <= m_vWarPos.x + WARSTAGEBORDER + fAdd; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - WARSTAGEBORDER;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + WARSTAGEBORDER + fAdd;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = m_vWarPos.x;
	for( fz = m_vWarPos.z - WARSTAGEBORDER; fz <= m_vWarPos.z + WARSTAGEBORDER + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - WARSTAGEBORDER;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + WARSTAGEBORDER + fAdd;
		vStgPos.z = fz;
		pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	OBJECTEFFECTDESC desc( FindEffectNum("eff_ptbmu_side_s.beff"), EFFECT_FLAG_ABSOLUTEPOS, &m_vWarPos );
	HERO->AddObjectEffect( PARTYWAR_STAGEEFFECT_ID, &desc, 1 );
}

void CPartyWar::DestroyStage()
{
	CTileManager* pTileManager = MAP->GetTileManager();

	VECTOR3 vStgPos;
	float fx, fz = m_vWarPos.z;
	float fAdd = -50.0f;

	for( fx = m_vWarPos.x - WARSTAGE; fx <= m_vWarPos.x + WARSTAGE + fAdd; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - WARSTAGE;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + WARSTAGE + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = m_vWarPos.x;
	for( fz = m_vWarPos.z - WARSTAGE; fz <= m_vWarPos.z + WARSTAGE + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - WARSTAGE;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + WARSTAGE + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fz = m_vWarPos.z;
	for( fx = m_vWarPos.x - WARSTAGEBORDER; fx <= m_vWarPos.x + WARSTAGEBORDER + fAdd; fx += 50.0f )
	{
		vStgPos.x = fx;
		vStgPos.z = fz - WARSTAGEBORDER;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + WARSTAGEBORDER + fAdd;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = m_vWarPos.x;
	for( fz = m_vWarPos.z - WARSTAGEBORDER; fz <= m_vWarPos.z + WARSTAGEBORDER + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - WARSTAGEBORDER;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + WARSTAGEBORDER + fAdd;
		vStgPos.z = fz;
		pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}	
}





// TEST-------------------------------------------------------------------------------------------------
void CPartyWar::TestPartyWarDialog()
{
	GAMEIN->GetPartyWarDlg()->TestPartyWarDialog();
}

