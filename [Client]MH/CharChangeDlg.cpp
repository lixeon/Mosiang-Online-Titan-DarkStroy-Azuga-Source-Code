// CharaChangeDlg.cpp: implementation of the CCharaChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharChangeDlg.h"
#include "interface/cGuageBar.h"
#include "interface/cStatic.h"
#include "interface/cButton.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "AppearanceManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "ChatManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharChangeDlg::CCharChangeDlg()
{
	m_type = WT_CHARCHANGE_DLG;
	
	m_ItemPos		= 0;
	m_bShapeChange	= FALSE;
}

CCharChangeDlg::~CCharChangeDlg()
{

}


void CCharChangeDlg::Linking()
{
	m_pName = (cStatic*)GetWindowForID( CHA_NAME );
	m_pSex = (cStatic*)GetWindowForID( CHA_SexType );
	m_pHair = (cStatic*)GetWindowForID( CHA_HairType );
	m_pFace = (cStatic*)GetWindowForID( CHA_FaceType );

	m_pSexBtn[0] = (cButton*)GetWindowForID( CHA_SexType1 );
	m_pSexBtn[1] = (cButton*)GetWindowForID( CHA_SexType2 );

	m_pHeight = (cGuageBar*)GetWindowForID( CHA_Height );
	m_pWidth = (cGuageBar*)GetWindowForID( CHA_Width );
}


void CCharChangeDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		if( OBJECTSTATEMGR->GetObjectState( HERO ) == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
	else
	{
		if( !m_bShapeChange )
		{
			m_pSexBtn[0]->SetActive( TRUE );
			m_pSexBtn[1]->SetActive( TRUE );			
			m_pHeight->SetActive( TRUE );
			m_pWidth->SetActive( TRUE );
		}
		else
		{
			m_pSexBtn[0]->SetActive( FALSE );
			m_pSexBtn[1]->SetActive( FALSE );			
			m_pHeight->SetActive( FALSE );
			m_pWidth->SetActive( FALSE );
		}
	}
}


void CCharChangeDlg::Process()
{
	if( m_bShapeChange )		return;

	float bh, bw;
	bh = (float)(0.9 + m_pHeight->GetCurRate()*0.2);
	bw = (float)(0.9 + m_pWidth->GetCurRate()*0.2);

	if( bh == m_CharacterInfo.Height && bw == m_CharacterInfo.Width )			return;
	
	m_CharacterInfo.Height = bh;
	m_CharacterInfo.Width = bw;

	VECTOR3 scale;
	SetVector3( &scale, m_CharacterInfo.Width, m_CharacterInfo.Height, m_CharacterInfo.Width );
	HERO->GetEngineObject()->SetScale(&scale);
}


void CCharChangeDlg::SetCharacterInfo( CHARACTER_TOTALINFO* pInfo )
{
	memset( &m_CharacterInfoBackup, 0, sizeof(CHARACTERCHANGE_INFO) );
	memset( &m_CharacterInfo, 0, sizeof(CHARACTERCHANGE_INFO) );

	m_CharacterInfoBackup.Gender = pInfo->Gender;
	m_CharacterInfoBackup.HairType = pInfo->HairType;
	m_CharacterInfoBackup.FaceType = pInfo->FaceType;
	m_CharacterInfoBackup.Height = pInfo->Height;
	m_CharacterInfoBackup.Width = pInfo->Width;

	m_CharacterInfo = m_CharacterInfoBackup;
	m_pName->SetStaticText( HERO->GetObjectName() );

	char buf[32] = { 0, };
	if( m_CharacterInfo.Gender == 0 )
		m_pSex->SetStaticText( CHATMGR->GetChatMsg(1180) );
	else
		m_pSex->SetStaticText( CHATMGR->GetChatMsg(1181) );
	sprintf( buf, CHATMGR->GetChatMsg(1182), m_CharacterInfo.HairType+1 );
	m_pHair->SetStaticText( buf );
	sprintf( buf, CHATMGR->GetChatMsg(1183), m_CharacterInfo.FaceType+1 );
	m_pFace->SetStaticText( buf );
	
	m_pHeight->SetCurRate( (float)((pInfo->Height-0.9)*5) );
	m_pWidth->SetCurRate( (float)((pInfo->Width-0.9)*5) );

	RefreshCharacterShape();
}



void CCharChangeDlg::RefreshCharacterShape()
{
	HERO->SetCharChangeInfo( &m_CharacterInfo );
	APPEARANCEMGR->AddCharacterPartChange( HEROID );
}


void CCharChangeDlg::Reset( BOOL bSave )
{
	if( bSave )
	{
		HERO->SetCharChangeInfo( &m_CharacterInfo );
	}
	else
	{
		HERO->SetCharChangeInfo( &m_CharacterInfoBackup );
		APPEARANCEMGR->AddCharacterPartChange( HEROID );
	}

	m_ItemPos		= 0;
	m_bShapeChange	= FALSE;
	memset( &m_CharacterInfoBackup, 0, sizeof(CHARACTERCHANGE_INFO) );
	memset( &m_CharacterInfo, 0, sizeof(CHARACTERCHANGE_INFO) );
}


void CCharChangeDlg::ChangeSexType( BOOL bPrev )
{
	if( m_bShapeChange )		return;


	if( m_CharacterInfo.Gender == 0 )
	{
		m_CharacterInfo.Gender = 1;
		m_pSex->SetStaticText( CHATMGR->GetChatMsg(1181) );
	}
	else
	{
		m_CharacterInfo.Gender = 0;
		m_pSex->SetStaticText( CHATMGR->GetChatMsg(1180) );
	}

	RefreshCharacterShape();
}


void CCharChangeDlg::ChangeHairType( BOOL bPrev )
{
	char buf[32] = { 0, };

	if( bPrev )
	{
		if( m_CharacterInfo.HairType == 0 )
			m_CharacterInfo.HairType = 4;
		else
			--m_CharacterInfo.HairType;
	}
	else
	{
		if( m_CharacterInfo.HairType == 4 )
			m_CharacterInfo.HairType = 0;
		else
			++m_CharacterInfo.HairType;
	}

	sprintf( buf, CHATMGR->GetChatMsg(1182), m_CharacterInfo.HairType+1 );
	m_pHair->SetStaticText( buf );

	RefreshCharacterShape();
}


void CCharChangeDlg::ChangeFaceType( BOOL bPrev )
{
	char buf[32] = { 0, };

	if( bPrev )
	{
		if( m_CharacterInfo.FaceType == 0 )
			m_CharacterInfo.FaceType = 4;
		else
			--m_CharacterInfo.FaceType;		
	}
	else
	{
		if( m_CharacterInfo.FaceType == 4 )
			m_CharacterInfo.FaceType = 0;
		else
			++m_CharacterInfo.FaceType;
	}

	sprintf( buf, CHATMGR->GetChatMsg(1183), m_CharacterInfo.FaceType+1 );
	m_pFace->SetStaticText( buf );

	RefreshCharacterShape();
}


void CCharChangeDlg::CharacterChangeSyn()
{
	SEND_CHARACTERCHANGE_INFO msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE_SYN );
	msg.dwObjectID = HEROID;
	msg.Param = m_ItemPos;
	msg.Info = m_CharacterInfo;
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}
