#include "stdafx.h"
#include "OptionDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cCheckBox.h"
#include "./interface/cStatic.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "ObjectManager.h"
#include "./interface/cScriptManager.h"
#include "ChatManager.h"

#include "MacroDialog.h"

#include "MainBarDialog.h"


COptionDialog::COptionDialog()
{
//	m_pBtnPreview = NULL;
}

COptionDialog::~COptionDialog()
{

}

void COptionDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}

void COptionDialog::Linking()
{
//	m_pBtnPreview = (cButton*)GetWindowForID( OTI_BTN_PREVIEW );
//	m_pBtnPreview->SetDisable( TRUE );
	//SW050824 체크박스 툴팁 설명
#ifndef _JAPAN_LOCAL_
	cWindow* pWnd = ((cWindow*)((cDialog*)GetTabSheet(2))->GetWindowForID( OTI_CB_AUTOCONTROL ));
	cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	pWnd->SetToolTip( CHATMGR->GetChatMsg(1098), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );
#endif
}

void COptionDialog::SetActive(BOOL val)
{
	if( !m_bDisable )
	{
		if( val )
		{
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			UpdateData( FALSE );
		}

		cTabDialog::SetActive(val);
	}

//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_OPTIONDLGICON, m_bActive );
}

void COptionDialog::UpdateData( BOOL bSave )
{
	cDialog* pDlg;

	if( bSave )	// 세팅된 옵션 가져오기 
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	
		m_GameOption.bNoDeal		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->IsChecked();
		m_GameOption.bNoParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->IsChecked();
		m_GameOption.bNoFriend		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->IsChecked();
		m_GameOption.bNoVimu		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->IsChecked();
		m_GameOption.bNameMunpa		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->IsChecked();
		m_GameOption.bNameParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->IsChecked();
		m_GameOption.bNameOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->IsChecked();
		m_GameOption.bNoMemberDamage	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->IsChecked();
		m_GameOption.bNoGameTip		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->IsChecked();
		// magi82 - 로그인시만 문파소개창 옵션지원 070108
		m_GameOption.bMunpaIntro	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->IsChecked();
		//////////////////////////////////////////////////////////////////////////
		m_GameOption.nMacroMode		= ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->IsPushed();
		
		pDlg = (cDialog*)GetTabSheet( 1 );
		m_GameOption.bNoWhisper		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->IsChecked();
		m_GameOption.bNoChatting	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->IsChecked();
		m_GameOption.bNoBalloon		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->IsChecked();
		m_GameOption.bAutoHide		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->IsChecked();
		m_GameOption.bNoShoutChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->IsChecked();
		m_GameOption.bNoGuildChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->IsChecked();
		m_GameOption.bNoAllianceChat= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->IsChecked();
		m_GameOption.bNoSystemMsg	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->IsChecked();
		m_GameOption.bNoExpMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->IsChecked();
		m_GameOption.bNoItemMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->IsChecked();

		pDlg = (cDialog*)GetTabSheet( 2 ); // 옵션에서 그래픽설정부분이다.
		m_GameOption.nGamma			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->GetCurValue(); // 게임밝기
		m_GameOption.nSightDistance = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->GetCurValue(); // 시야거리
//		m_GameOption.bGraphicCursor = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_CURSOR ))->IsChecked();
		m_GameOption.bShadowHero	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->IsChecked(  ); // 자신
		m_GameOption.bShadowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->IsChecked(  ); // 몬스터/NPC
		m_GameOption.bShadowOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->IsChecked( ); // 다른케릭터
		m_GameOption.bNoAvatarView = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->IsChecked();
	


		//SW050822 Add RadioBtn for GraphicCtrl
#ifndef _JAPAN_LOCAL_		
		m_GameOption.nEffectSnow    = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->IsChecked( ); // 눈 On/Off 2005.12.28일추가	

		m_GameOption.bAutoCtrl		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->IsChecked();
		m_GameOption.bAmbientMax	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->IsChecked();
		
//		if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->IsPushed() )
//		{
//			m_GameOption.nLODMode = MODEL_LOD_USING_MODE_DEFAULT;
//		}
//		else if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed() )
//		{
//			m_GameOption.nLODMode = MODEL_LOD_USING_MODE_SET_LEVEL;
//		}

		m_GameOption.nLODMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed(); // 그랙픽 상태
		m_GameOption.nEffectMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->IsPushed();// 이펙트 표시	



		
		   


//		else if( ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ALLOFFEFFECT ))->IsPushed() )
//		{
//			m_GameOption.nEffectMode = eOO_EFFECT_NOT_USE;
//		}
#endif
		pDlg = (cDialog*)GetTabSheet( 3 );
		m_GameOption.bSoundBGM			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->IsChecked();
		m_GameOption.bSoundEnvironment	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->IsChecked();
		m_GameOption.nVolumnBGM			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->GetCurValue();
		m_GameOption.nVolumnEnvironment = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->GetCurValue();

	}
	else	//정보 가지고 와서 창 세팅 (기본설정이 창이 뜸)
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->SetChecked( m_GameOption.bNoDeal );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->SetChecked( m_GameOption.bNoParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->SetChecked( m_GameOption.bNoFriend );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->SetChecked( m_GameOption.bNoVimu );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->SetChecked( m_GameOption.bNameMunpa );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->SetChecked( m_GameOption.bNameParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->SetChecked( m_GameOption.bNameOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->SetChecked( m_GameOption.bNoMemberDamage );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->SetChecked( m_GameOption.bNoGameTip );
		// magi82 - 로그인시만 문파소개창 옵션지원 070108
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->SetChecked( m_GameOption.bMunpaIntro );
		//////////////////////////////////////////////////////////////////////////
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( !m_GameOption.nMacroMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( m_GameOption.nMacroMode );
		
		pDlg = (cDialog*)GetTabSheet( 1 );	
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->SetChecked( m_GameOption.bNoWhisper );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->SetChecked( m_GameOption.bNoChatting );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->SetChecked( m_GameOption.bNoBalloon );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->SetChecked( m_GameOption.bAutoHide );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->SetChecked( m_GameOption.bNoShoutChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->SetChecked( m_GameOption.bNoGuildChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->SetChecked( m_GameOption.bNoAllianceChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->SetChecked( m_GameOption.bNoSystemMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->SetChecked( m_GameOption.bNoExpMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->SetChecked( m_GameOption.bNoItemMsg );

		pDlg = (cDialog*)GetTabSheet( 2 );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->SetCurValue( m_GameOption.nGamma );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->SetCurValue( m_GameOption.nSightDistance );
//		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_CURSOR ))->SetChecked( m_GameOption.bGraphicCursor );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->SetChecked( m_GameOption.bShadowHero );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetChecked( m_GameOption.bShadowMonster );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetChecked( m_GameOption.bShadowOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->SetChecked( m_GameOption.bNoAvatarView );



#ifndef _JAPAN_LOCAL_
		// 눈 이펙트 추가 예정 2005.12.28 눈 On/Off 추가 
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->SetChecked( m_GameOption.nEffectSnow );

		//SW050822 Add RadioBtn for GraphicCtrl
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->SetChecked( m_GameOption.bAutoCtrl );
		//SW060904 Add MapAmbientCtrlOptn
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->SetChecked( m_GameOption.bAmbientMax );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( !m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( !m_GameOption.nEffectMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( m_GameOption.nEffectMode );

		DisableGraphicTab(m_GameOption.bAutoCtrl);
//		else if( m_GameOption.nEffectMode == eOO_EFFECT_NOT_USE )
//		{
//			((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ALLOFFEFFECT ))->SetPush( TRUE );
//		}
#endif
		pDlg = (cDialog*)GetTabSheet( 3 );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->SetChecked( m_GameOption.bSoundBGM );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->SetChecked( m_GameOption.bSoundEnvironment );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->SetCurValue( m_GameOption.nVolumnBGM );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->SetCurValue( m_GameOption.nVolumnEnvironment );
	}
}

void COptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == OTI_BTN_OK )
		{
			UpdateData( TRUE );
			// magi82 - 로그인시만 문파소개창 옵션지원 070108
			if( m_GameOption.bMunpaIntro )
			{
				// 옵션창 확인을 눌렀을때 문파소개창보기가 TRUE라서 같이 열리므로
				// 잠시 FLASE로 바꾸고 세팅해준후 다시 TRUE로 바꿔서 옵션값을 저장한다.
				m_GameOption.bMunpaIntro = FALSE;
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
				m_GameOption.bMunpaIntro = TRUE;
				OPTIONMGR->SetGameOption( &m_GameOption );
			}
			else
			{
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
			}
			//////////////////////////////////////////////////////////////////////////
			OPTIONMGR->SendOptionMsg();		
			SetActive( FALSE );
		}
//		else if( lId == OTI_BTN_PREVIEW )
//		{
//			UpdateData( TRUE );
//			OPTIONMGR->ApplySettings();
//			OPTIONMGR->SaveGameOption();
//			m_pBtnPreview->SetDisable( TRUE );
//		}
		else if( lId == OTI_BTN_CANCEL )
		{
			OPTIONMGR->CancelSettings();
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_RESET )
		{
			OPTIONMGR->SetDefaultOption();
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			UpdateData( FALSE );
		}
		else if( lId == OTI_BTN_SETCHAT )
		{
/*
			if( GAMEIN->GetMacroDialog()->IsActive() )
			{
				GAMEIN->GetMacroDialog()->SetActive( FALSE );
			}
			else
			{
				GAMEIN->GetMacroDialog()->SetMode( MM_CHAT );
				GAMEIN->GetMacroDialog()->SetActive( TRUE );
			}
*/
		}
		else if( lId == OTI_BTN_SETMACRO )
		{
/*
			if( GAMEIN->GetMacroDialog()->IsActive() )
			{
				GAMEIN->GetMacroDialog()->SetActive( FALSE );
			}
			else
			{
				GAMEIN->GetMacroDialog()->SetMode( MM_MACRO );
				GAMEIN->GetMacroDialog()->SetActive( TRUE );
			}
*/
		}
	}
	
	if( we & WE_PUSHUP )		//라디오 버튼 처리
	{
		if( lId == OTI_PB_CHATMODE || lId == OTI_PB_MACROMODE )
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( lId ))->SetPush( TRUE );
#ifndef _JAPAN_LOCAL_
		//SW050822 Add GraphicCtrl
		if( lId == OTI_PB_BASICGRAPHIC || lId == OTI_PB_DOWNGRAPHIC )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
		if( lId == OTI_PB_BASICEFFECT || lId == OTI_PB_ONEEFFECT )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
#endif
	}

	if( we & WE_PUSHDOWN )		//
	{
		if( lId == OTI_PB_CHATMODE )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_MACROMODE )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( FALSE );
		}
#ifndef _JAPAN_LOCAL_
		//SW050822 Add GraphicCtrl
		if( lId == OTI_PB_BASICGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_DOWNGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( FALSE );
		}
		if( lId == OTI_PB_BASICEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_ONEEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( FALSE );
		}
#endif
	}
#ifndef _JAPAN_LOCAL_
	if( we & WE_CHECKED )	//SW 자동 옵션을 키면 옵션버튼 비활성화 & 글자 흐리기로 비활성 표시
	{

		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(TRUE);
		}
	}
	else if( we & WE_NOTCHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(FALSE);
		}
	}
#endif
//	if( m_pBtnPreview->IsDisable() && lId != OTI_BTN_PREVIEW )
//		m_pBtnPreview->SetDisable( FALSE );
}

#ifndef _JAPAN_LOCAL_
void COptionDialog::DisableGraphicTab(BOOL bDisable)
{
			DWORD FullColor = 0xffffffff;
			DWORD HalfColor = RGBA_MAKE(200,200,200,255);

			if( bDisable )
			{
				FullColor = HalfColor;
			}

			cWindow* pWnd = NULL;
			cGuageBar* pGuag = NULL;
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ))->SetDisable(FALSE);
			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ));
			pGuag->SetDisable(bDisable);
			pGuag->SetImageRGB(FullColor);
			pGuag->SetGuageLock(bDisable, FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_GAMMA ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ))->SetDisable(FALSE);
			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ));
			pGuag->SetDisable(bDisable);
			pGuag->SetImageRGB(FullColor);
			pGuag->SetGuageLock(bDisable, FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_HEROSHADOW ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_MONSTERSHADOW ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_OTHERSSHADOW ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICGRAPHIC ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_DOWNGRAPHIC ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICEFFECT ))->SetFGColor(FullColor);
//			((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetDisable(FALSE);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_ONEEFFECT ))->SetFGColor(FullColor);	
}
#endif
