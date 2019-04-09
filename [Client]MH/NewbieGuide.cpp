// NewbieGuide.cpp: implementation of the CNewbieGuide class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewbieGuide.h"

#include "MHFile.h"
#include "HelperManager.h"
#include "ObjectManager.h"
#include "MugongManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewbieGuide::CNewbieGuide()
{
	m_NewbieData.Initialize( 96 );
	m_dwFlag1 = m_dwFlag2 = m_dwFlag3 = 0;
}

CNewbieGuide::~CNewbieGuide()
{
	NEWBIEDATA* pData = NULL;
	m_NewbieData.SetPositionHead();
	while( pData = m_NewbieData.GetData() )
	{
		delete pData;
		pData = NULL;
	}
	m_NewbieData.RemoveAll();
}

void CNewbieGuide::LoadNewbieGuide( char* filename )
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( filename, "rb") )
		return;
#else
	if( !file.Init( filename, "rt") )
		return;
#endif

	char buf[512];
	DWORD dwKey = 0;
	while( !file.IsEOF() )
	{
		file.GetString( buf );
		if( buf[0] == '}' )
			break;
		if( buf[0] == '#' )
		{
//			strcpy( buf, (buf+1) );
			dwKey = (DWORD)atoi( buf+1 );
			m_NewbieData.Add( LoadNewbieGuideData( &file, dwKey ), dwKey );
		}
	}
}

#define IF( buf, str, kind, data )	\
		if( strcmp(buf, str) == 0 )	\
		{							\
			data = kind;			\
		}														
#define ELSEIF( buf, str, kind, data )		\
		else if( strcmp(buf, str) == 0 )	\
		{									\
			data = kind;					\
		}				
NEWBIEDATA* CNewbieGuide::LoadNewbieGuideData( CMHFile* pfile, DWORD dwBitFlag )
{
	NEWBIEDATA* pData = new NEWBIEDATA;
	pData->m_dwBitFlag = dwBitFlag;
	
	char buf[256];	
	pfile->GetString( buf );

	IF( buf, "&CHAR",			eGameEvent_CharInterface, pData->m_dwKind )
	ELSEIF( buf, "&INVEN",		eGameEvent_InvenInterface, pData->m_dwKind )
	ELSEIF( buf, "&MUGONG",		eGameEvent_MugongInterface, pData->m_dwKind )
	ELSEIF( buf, "&QUEST",		eGameEvent_QuestInterface, pData->m_dwKind )
	ELSEIF( buf, "&JIDO",		eGameEvent_MapInterface, pData->m_dwKind )
	ELSEIF( buf, "&MUNPA",		eGameEvent_MunpaInterface, pData->m_dwKind )
	ELSEIF( buf, "&FRIEND",		eGameEvent_FriendInterface, pData->m_dwKind )
	ELSEIF( buf, "&NOTE",		eGameEvent_NoteInterface, pData->m_dwKind )
	ELSEIF( buf, "&OPTION",		eGameEvent_OptionInterface, pData->m_dwKind )
	ELSEIF( buf, "&EXIT",		eGameEvent_ExitInterface, pData->m_dwKind )
	ELSEIF( buf, "&PK",			eGameEvent_PKInterface, pData->m_dwKind )
	ELSEIF( buf, "&HUNT",		eGameEvent_Hunt, pData->m_dwKind )
	ELSEIF( buf, "&DIE",		eGameEvent_Die, pData->m_dwKind )
	ELSEIF( buf, "&DIEAFTER5",	eGameEvent_DieAfter5, pData->m_dwKind )
	ELSEIF( buf, "&NPCCLICK",	eGameEvent_NpcClick, pData->m_dwKind )
	ELSEIF( buf, "&DIEFROMPK",	eGameEvent_DieFromPK, pData->m_dwKind )
	ELSEIF( buf, "&KILLPK",		eGameEvent_KillPK, pData->m_dwKind )
	ELSEIF( buf, "&OBTAINITEM",	eGameEvent_ObtainItem, pData->m_dwKind )
	ELSEIF( buf, "&ITEMSETTING",eGameEvent_ItemSetting, pData->m_dwKind )
	ELSEIF( buf, "&LOGINPOINT",	eGameEvent_LoginPoint, pData->m_dwKind )
	ELSEIF( buf, "&MUGONG0",	eGameEvent_Mugong0, pData->m_dwKind )
	ELSEIF( buf, "&MUGONG1",	eGameEvent_Mugong1, pData->m_dwKind )
	ELSEIF( buf, "&MUNPAUNDER",	eGameEvent_Munpaunder, pData->m_dwKind )//magi82 - 문하생 가입편리시스템
	ELSEIF( buf, "&ABILITY",	eGameEvent_Ability, pData->m_dwKind )
	ELSEIF( buf, "&LEVEL",		eGameEvent_LevelUp, pData->m_dwKind )
	ELSEIF( buf, "&MAP",		eGameEvent_MapChange, pData->m_dwKind )
	
	if( strcmp(buf, "&ABILITY") == 0 || strcmp(buf, "&LEVEL") == 0 || strcmp(buf, "&MAP") == 0 )
	{
		pData->m_dwParam = pfile->GetDword();
		pData->m_dwPage = pfile->GetDword();
	}
	else 
	{
		pData->m_dwPage = pfile->GetDword();
	}

	return pData;
}

void CNewbieGuide::SetFlag( DWORD dwBit )
{
	if( 55 == dwBit )	//초보자 무공 배우기 관련 반복.	// 55 <- 무공미습득관련
	{
		int n = MUGONGMGR->GetMugongNum();
		if( 0 == n )
			return;
	}


	DWORD count = dwBit/32;
	DWORD bit = dwBit%32;
	DWORD value = 1;

	switch( count )
	{
	case 0:
		m_dwFlag1 = m_dwFlag1 | ( value << (31-bit) );
		break;
	case 1:
		m_dwFlag2 = m_dwFlag2 | ( value << (31-bit) );
		break;
	case 2:
		m_dwFlag3 = m_dwFlag3 | ( value << (31-bit) );
		break;
	default:
		return;
	}

	// network send
	SEND_QUEST_IDX msg;
	msg.Category = MP_QUEST;
	msg.Protocol = MP_QUEST_CHANGESTATE;
	msg.dwObjectID = HERO->GetID();
	msg.MainQuestIdx = 0;
	msg.dwFlag = m_dwFlag1;
	msg.dwData = m_dwFlag2;
	msg.dwTime = m_dwFlag3;
	NETWORK->Send(&msg, sizeof(msg));
}

BOOL CNewbieGuide::IsComplete( DWORD dwBit )
{
	DWORD count = dwBit/32;
	DWORD bit = dwBit%32;
	DWORD value = 1;

	switch( count )
	{
	case 0:
		return ( m_dwFlag1 & ( value << (31-bit) ) ) ? TRUE : FALSE;
	case 1:
		return ( m_dwFlag2 & ( value << (31-bit) ) ) ? TRUE : FALSE;
	case 2:
		return ( m_dwFlag3 & ( value << (31-bit) ) ) ? TRUE : FALSE;
	default:
		return TRUE;
	}
}

void CNewbieGuide::CheckEvent( GAMEEVENT* pGE )
{
	switch( pGE->bEventKind )
	{
	case eGameEvent_CharInterface:		CheckElse( eGameEvent_CharInterface );		break;
	case eGameEvent_InvenInterface:		CheckElse( eGameEvent_InvenInterface );		break;
	case eGameEvent_MugongInterface:	CheckElse( eGameEvent_MugongInterface );	break;	
	case eGameEvent_QuestInterface:		CheckElse( eGameEvent_QuestInterface );		break;
	case eGameEvent_MapInterface:		CheckElse( eGameEvent_MapInterface );		break;
	case eGameEvent_MunpaInterface:		CheckElse( eGameEvent_MunpaInterface );		break;
	case eGameEvent_FriendInterface:	CheckElse( eGameEvent_FriendInterface );	break;
	case eGameEvent_NoteInterface:		CheckElse( eGameEvent_NoteInterface );		break;
	case eGameEvent_OptionInterface:	CheckElse( eGameEvent_OptionInterface );	break;
	case eGameEvent_ExitInterface:		CheckElse( eGameEvent_ExitInterface );		break;
	case eGameEvent_PKInterface:		CheckElse( eGameEvent_PKInterface );		break;

	case eGameEvent_Hunt:				CheckElse( eGameEvent_Hunt );				break;
	case eGameEvent_Die:				CheckElse( eGameEvent_Die );				break;
	case eGameEvent_DieAfter5:			CheckElse( eGameEvent_DieAfter5 );			break;
	case eGameEvent_NpcClick:			CheckElse( eGameEvent_NpcClick );			break;
	case eGameEvent_DieFromPK:			CheckElse( eGameEvent_DieFromPK );			break;
	case eGameEvent_KillPK:				CheckElse( eGameEvent_KillPK );				break;
	case eGameEvent_ObtainItem:			CheckElse( eGameEvent_ObtainItem );			break;
	case eGameEvent_ItemSetting:		CheckElse( eGameEvent_ItemSetting );		break;
	case eGameEvent_LoginPoint:			CheckElse( eGameEvent_LoginPoint );			break;
	case eGameEvent_Mugong0:			CheckElse( eGameEvent_Mugong0 );			break;
	case eGameEvent_Mugong1:			CheckElse( eGameEvent_Mugong1 );			break;
	case eGameEvent_Munpaunder:			CheckElse( eGameEvent_Munpaunder );			break;//magi82 - 문하생 가입편리시스템
		
	case eGameEvent_Ability:			CheckAbility( pGE->dwData );				break;
	case eGameEvent_LevelUp:			CheckLevelUp( pGE->dwData );				break;
	case eGameEvent_MapChange:			CheckMapChange( pGE->dwData );				break;		
	}
}

void CNewbieGuide::CheckLevelUp( DWORD dwLevel )
{
	NEWBIEDATA* pData = NULL;
	m_NewbieData.SetPositionHead();
	while( pData = m_NewbieData.GetData() )
	{
		if( pData->m_dwKind == eGameEvent_LevelUp && pData->m_dwParam == dwLevel )
		{
			if( !IsComplete( pData->m_dwBitFlag ) )
			{
				SetFlag( pData->m_dwBitFlag );
				HELPERMGR->ShowRespect( pData->m_dwPage );
				return;
			}
			else
				return;
		}
	}
}

void CNewbieGuide::CheckMapChange( DWORD dwMapNum )
{
	NEWBIEDATA* pData = NULL;
	m_NewbieData.SetPositionHead();
	while( pData = m_NewbieData.GetData() )
	{
		if( pData->m_dwKind == eGameEvent_MapChange && pData->m_dwParam == dwMapNum )
		{
			if( !IsComplete( pData->m_dwBitFlag ) )
			{
				SetFlag( pData->m_dwBitFlag );
				HELPERMGR->ShowRespect( pData->m_dwPage );
				return;
			}
			else
				return;
		}
	}
}

void CNewbieGuide::CheckAbility( DWORD dwAbilityNum )
{
	NEWBIEDATA* pData = NULL;
	m_NewbieData.SetPositionHead();
	while( pData = m_NewbieData.GetData() )
	{
		if( pData->m_dwKind == eGameEvent_Ability && pData->m_dwParam == dwAbilityNum )
		{
			if( !IsComplete( pData->m_dwBitFlag ) )
			{
				SetFlag( pData->m_dwBitFlag );
				HELPERMGR->ShowRespect( pData->m_dwPage );
				return;
			}
			else
				return;
		}
	}
}

void CNewbieGuide::CheckElse( DWORD dwEventKind )
{
	NEWBIEDATA* pData = NULL;
	m_NewbieData.SetPositionHead();
	while( pData = m_NewbieData.GetData() )
	{
		if( pData->m_dwKind == dwEventKind )
		{
			if( !IsComplete( pData->m_dwBitFlag ) )
			{
				SetFlag( pData->m_dwBitFlag );
				HELPERMGR->ShowRespect( pData->m_dwPage );
				return;
			}
			else
				return;
		}
	}
}

