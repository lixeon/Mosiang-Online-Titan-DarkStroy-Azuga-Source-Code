// cDivideBox.cpp: implementation of the cDivideBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cDivideBox.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "./Interface/cSpin.h"
#include "WindowIDEnum.h"
#include "cIMEEX.h"
#include "ChatManager.h"

#include "./Input/Keyboard.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cDivideBox::cDivideBox()
{
	m_pOKBtn		= NULL;
	m_pCancelBtn	= NULL;
	m_pSpin			= NULL;
	m_cbDivideFunc	= NULL;
	m_cbCancelFunc	= NULL;
	m_vData1		= NULL;
	m_vData2		= NULL;
//	m_nPos			= -1;
//	m_type			= WT_DIVIDEBOX;
}

cDivideBox::~cDivideBox()
{

}

void cDivideBox::CreateDivideBox( LONG x, LONG y, LONG ID, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char* strTitle )
{
	m_cbDivideFunc = cbFc1;
	m_cbCancelFunc = cbFc2;
	m_vData1 = vData1;
	m_vData2 = vData2;
	SetAbsXY(x,y);
	SetID(ID);

	m_pOKBtn = (cButton *)GetWindowForID(CMI_MONEYOK);
	m_pCancelBtn = (cButton *)GetWindowForID(CMI_MONEYCANCEL);

	m_pSpin = (cSpin *)GetWindowForID(CMI_MONEYSPIN);
	m_pSpin->SetValidCheck(VCM_NUMBER); // 숫자만 입력 되도록 한다.
	m_pSpin->SetAlign( TXT_RIGHT );		// 오른정렬
	m_pSpin->SetFocusEdit( TRUE );
	m_pSpin->SetUnit( 1 );

	// Title Text 입력 LBS 03.11.19 : 수정 KES
	if( strTitle )
	{
		cStatic* pStatic = new cStatic;
		
		pStatic->Init( 13, 2, 0, 0, NULL );
		pStatic->SetShadow( TRUE );
		pStatic->SetFontIdx( 2 );
		pStatic->SetAlign( TXT_LEFT );
		pStatic->SetStaticText( strTitle );
		
		Add( pStatic );
	}

/*
		char imagePath[256]={0,};
		cImage * pImageTmp	= WINDOWMGR->GetImagePath(472, imagePath, 256 );
		cDialog::Init( x, y, 173, 40, pImageTmp, ID );
		
		RECT rtCaption = { 0, 0, 173, 10 };	//임시 
		SetCaptionRect(&rtCaption);
	
		// OK BUTTON
		m_pOKBtn = new cButton;
		m_pOKBtn->Init( 128, 15, 40, 20, NULL, NULL, NULL, NULL, CMI_MONEYOK );
		Add( m_pOKBtn );
	
		// SPIN BUTTON
		m_pSpin = new cSpin;
		m_pSpin->Init( 30, 15, 93, 20, NULL, NULL, CMI_MONEYSPIN );
		m_pSpin->InitSpin( 83, 10 );
		m_pSpin->SetMinMax( 0, 4294967295 );
		m_pSpin->SetFocusEdit( TRUE );
	
		cButton * m_pSpinBtn = new cButton;
		pImageTmp = WINDOWMGR->GetImagePath(473, imagePath, 256 );
		m_pSpinBtn->Init( 83, 0, 10, 10, NULL, NULL, pImageTmp, NULL, CMI_MONEYSPINUP );
		m_pSpin->Add( m_pSpinBtn );
		m_pSpinBtn = new cButton;
		pImageTmp = WINDOWMGR->GetImagePath(474, imagePath, 256 );
		m_pSpinBtn->Init( 83, 10, 10, 10, NULL, NULL, pImageTmp, NULL, CMI_MONEYSPINDOWN );
		m_pSpin->Add( m_pSpinBtn );
	
		Add( m_pSpin );
*/
	
}

void cDivideBox::ChangeKind( int nKind )
{
	if( nKind == 0 )
	{
		m_pOKBtn->SetText( CHATMGR->GetChatMsg(418), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
		m_pCancelBtn->SetText( CHATMGR->GetChatMsg(419), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
	else if( nKind == 1 )
	{
		m_pOKBtn->SetText( CHATMGR->GetChatMsg(658), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
	else if( nKind == 2 )
	{
		m_pCancelBtn->SetText( CHATMGR->GetChatMsg(632), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
}

DWORD cDivideBox::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	DWORD weBtn = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;
	
	we |= cWindow::ActionEvent( mouseInfo );
	we |= cDialog::ActionEventWindow( mouseInfo );

	BOOL bClicked = FALSE;

	if( we & WE_CLOSEWINDOW )
	{
		bClicked = TRUE;
	}

	we |= m_pSpin->ActionEvent( mouseInfo);

	weBtn = m_pOKBtn->ActionEvent( mouseInfo);
	if( weBtn  & WE_BTNCLICK )
	{
		if( m_cbDivideFunc )
			m_cbDivideFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
		bClicked = TRUE;
	}
	weBtn = m_pCancelBtn->ActionEvent( mouseInfo );
	if( weBtn  & WE_BTNCLICK )
	{
		if( m_cbCancelFunc )
			m_cbCancelFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
		bClicked = TRUE;
	}

	if( bClicked )
	{
		m_pSpin->SetFocusEdit( FALSE );
		SetActive( FALSE );
		SetDisable( TRUE );
		WINDOWMGR->AddListDestroyWindow( this );
	}
	

	return we;
}


DWORD cDivideBox::ActionKeyboardEvent( CKeyboard* keyInfo )
{
/*	//editbox가 처리한다.
	DWORD we = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;
	
	if( keyInfo->GetKeyDown( KEY_RETURN ) )
	{
		if( cbWindowFunc )
		{
			m_cbDivideFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
			m_pSpin->SetFocusEdit( FALSE );
			WINDOWMGR->AddListDestroyWindow( this );

			we |= WE_RETURN;
		}
	}

	return we;
*/
	return WE_NULL;
}

void cDivideBox::ExcuteDBFunc( DWORD we )
{
	if( we == WE_RETURN )
	{
		if( m_cbDivideFunc )
		{
			m_cbDivideFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
			m_pSpin->SetFocusEdit( FALSE );
			SetActive( FALSE );
			SetDisable( TRUE );

			WINDOWMGR->AddListDestroyWindow( this );
		}
	}
	else if( we == 0 )	//임시로 0사용
	{
		if( m_cbCancelFunc )
		{
			m_cbCancelFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
			m_pSpin->SetFocusEdit( FALSE );
			SetActive( FALSE );
			SetDisable( TRUE );

			WINDOWMGR->AddListDestroyWindow( this );
		}
	}
}

void cDivideBox::SetValue( DWORD val )
{
	m_pSpin->SetValue(val);
}

LONG cDivideBox::GetValue()
{	
	if(m_pSpin)
		return m_pSpin->GetValue();
	else 
		return 0;
}

void cDivideBox::SetMaxValue( DWORD val )
{
	m_pSpin->SetMax( val );
}

void cDivideBox::SetMinValue( DWORD val )
{
	m_pSpin->SetMin( val );
}