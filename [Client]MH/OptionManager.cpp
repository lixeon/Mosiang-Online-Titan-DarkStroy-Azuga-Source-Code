// OptionManager.cpp: implementation of the COptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionManager.h"
#include "MacroManager.h"
#include "MouseCursor.h"
#include "MousePointer.h"
#include "./Input/UserInput.h"
#include "ChatManager.h"
#include ".\Effect\EffectManager.h"
#include "cChatTipManager.h"
#include "ObjectManager.h"
#include "MHAudioManager.h"
#include "MHCamera.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "./interface/cWindowManager.h"
#include "OptionDialog.h"
#include "./interface/cDialog.h"
#include "./interface/cGuageBar.h"
#include "WindowIDEnum.h"
#include "WeatherManager.h" // 2005.12.30일 추가 눈 On/Off
#include "MHMap.h"
#include "GuildTraineeDialog.h"
#include "AppearanceManager.h"

#include "Hero.h"
//#include "UserInfoManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(COptionManager);

COptionManager::COptionManager()
{


}

COptionManager::~COptionManager()
{
}

void COptionManager::Init()
{
	SetDefaultOption();

	if( !LoadGameOption() )
		SetDefaultOption();

	m_bBGMSoundChanged	= FALSE;
	m_wSendOption		= 0;
	
	m_bShowMenberDamage = TRUE;


	AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );
}

void COptionManager::SendOptionMsg()
{
	WORD wOption = MakeOptionFlag();

//	if( m_wSendOption == wOption )
//		return;

	m_wSendOption = wOption;

	MSG_WORD msgOption;
	msgOption.Category		= MP_OPTION;
	msgOption.Protocol		= MP_OPTION_SET_SYN;
	msgOption.dwObjectID	= gHeroID;
	msgOption.wData			= wOption;
//	m_pOptionDlg->m_WeatherManager.SetWeather(m_GameOption.nEffectSnow);

	NETWORK->Send( &msgOption, sizeof( MSG_WORD ) );
}

WORD COptionManager::MakeOptionFlag()
{
	WORD wOption = 0;

	if( m_GameOption.bNoDeal )		wOption |= eOPT_NODEAL;
	if( m_GameOption.bNoParty )		wOption |= eOPT_NOPARTY;
	if( m_GameOption.bNoFriend )	wOption |= eOPT_NOFRIEND;
	if( m_GameOption.bNoVimu )		wOption |= eOPT_NOVIMU;
	if( m_GameOption.bNoWhisper )	wOption |= eOPT_NOWHISPER;
	if( m_GameOption.bNoAvatarView)	wOption |= eOPT_NOAVATARVIEW;

	return wOption;
}

void COptionManager::SetGameOption( sGAMEOPTION* pOption )
{
	if( m_GameOption.bSoundBGM != pOption->bSoundBGM )
		m_bBGMSoundChanged = TRUE;

	m_GameOption = *pOption;

	SaveGameOption();
}

BOOL COptionManager::LoadGameOption()
{
//	char strFilePath[MAX_PATH];
//	wsprintf( strFilePath, "%s\\User\\%s\\%s.OPT", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID(), USERINFOMGR->GetUserID() );

	HANDLE	hFile;

//	hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL,
	hFile = CreateFile( "./ini/GameOption.opt", GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwRead = 0;
	DWORD dwVersion;
	if( !ReadFile( hFile, &dwVersion, sizeof(DWORD), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	if( dwVersion != 0x00061208 )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	if( !ReadFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	// magi82 - Intro(070713) - Option 관련 수정
	if( dwRead > sizeof(m_GameOption) )
	{
		CloseHandle( hFile );
		return FALSE;
	}	

//사운드나 감마, 시야거리등의 수치가 유효범위인지를 체크하는 것을 추가해야한다.
//유효범위가 아니면 모두 초기화!	//CONFIRM
	

	CloseHandle( hFile );
	return TRUE;
}

void COptionManager::SaveGameOption()
{
//	char strFilePath[MAX_PATH];

	//make directory
//	wsprintf( strFilePath, "%s\\User", DIRECTORYMGR->GetFullDirectoryName(eLM_Root) );
//	CreateDirectory( strFilePath, NULL );
//	wsprintf( strFilePath, "%s\\User\\%s", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID() );
//	CreateDirectory( strFilePath, NULL );
//	wsprintf( strFilePath, "%s\\User\\%s\\%s.UII", DIRECTORYMGR->GetFullDirectoryName(eLM_Root), USERINFOMGR->GetUserID(), USERINFOMGR->GetUserID() );

	HANDLE	hFile;


//	hFile=CreateFile( strFilePath, GENERIC_WRITE, 0, NULL, 
	hFile=CreateFile( "./ini/GameOption.opt", GENERIC_WRITE, 0, NULL, 
					  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return;
	
	DWORD dwWritten;
	DWORD dwVersion = 0x00061208;
	WriteFile( hFile, &dwVersion, sizeof(DWORD), &dwWritten, NULL );
	WriteFile( hFile, &m_GameOption, sizeof(m_GameOption), &dwWritten, NULL );
	
	CloseHandle( hFile );
}

void COptionManager::ApplySettings()
{
////////
//사운드
	if( m_bBGMSoundChanged )
	{
		AUDIOMGR->SetBGMSoundOn( m_GameOption.bSoundBGM );

		if( m_GameOption.bSoundBGM )
			AUDIOMGR->PlayLastBGM();
		else
			AUDIOMGR->StopBGM();

		m_bBGMSoundChanged = FALSE;
	}

	if( m_GameOption.bSoundBGM )	//bgm이 play중이 아닐때 볼륨조절하면 뻑난다.
		AUDIOMGR->SetBGMVolume( (float)m_GameOption.nVolumnBGM / 100.0f );
	
	AUDIOMGR->SetEffectSoundOn( m_GameOption.bSoundEnvironment );
	if( m_GameOption.bSoundEnvironment )
		AUDIOMGR->SetSoundVolume( (float)m_GameOption.nVolumnEnvironment / 100.0f );
	else
		AUDIOMGR->StopAll();
	
////////
//그래픽

//-게임 감마
//-이름표시 옵션 
//-그래픽커서
	CURSOR->SetActive( !m_GameOption.bGraphicCursor );
//	MOUSE->SetGraphicCursor( m_GameOption.bGraphicCursor );
//	CURSOR->SetActive( TRUE );//MouseTest용
	//taiyo test
	//WINDOWMGR->GetMouseWindow()->SetActive( m_GameOption.bGraphicCursor );

//시야거리
	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );

//그림자 출력여부
	DWORD dwShadowOption = eOO_SHADOW_NONE;
	if( m_GameOption.bShadowHero )		dwShadowOption |= eOO_SHADOW_ONLYHERO;
	if( m_GameOption.bShadowMonster )	dwShadowOption |= eOO_SHADOW_MONSTER;
	if( m_GameOption.bShadowOthers )	dwShadowOption |= eOO_SHADOW_OTHERCHARACTER;
	OBJECTMGR->SetShadowOption( dwShadowOption );

//이름	
	DWORD dwOverInfoOption = eOO_NAME_CLICKEDONLY;	//---default
	if( m_GameOption.bNameOthers )	dwOverInfoOption |= eOO_NAME_WHOLE;
	if( m_GameOption.bNoMemberDamage )	dwOverInfoOption |= eOO_DAMAGE_MEMBERS;
	if( m_GameOption.bNameParty )	dwOverInfoOption |= eOO_NAME_PARTY;
	if( m_GameOption.bNameMunpa )	dwOverInfoOption |= eOO_NAME_MUNPA;
	if( m_GameOption.bNoBalloon )	dwOverInfoOption |= eOO_CHAT_NOBALLOON;
	OBJECTMGR->SetOverInfoOption( dwOverInfoOption );
	
//////
//채팅
	sChatOption ChatOption;
	ChatOption.bOption[CTO_AUTOHIDECHAT]	= m_GameOption.bAutoHide;
	ChatOption.bOption[CTO_NOSYSMSG]		= m_GameOption.bNoSystemMsg;
	ChatOption.bOption[CTO_NOITEMMSG]		= m_GameOption.bNoItemMsg;
	ChatOption.bOption[CTO_NOEXPMSG]		= m_GameOption.bNoExpMsg;
	ChatOption.bOption[CTO_NOCHATMSG]		= m_GameOption.bNoChatting;
	ChatOption.bOption[CTO_NOSHOUTMSG]		= m_GameOption.bNoShoutChat;
	ChatOption.bOption[CTO_NOGUILDMSG]		= m_GameOption.bNoGuildChat;
	ChatOption.bOption[CTO_NOALLIANCEMSG]	= m_GameOption.bNoAllianceChat;
	CHATMGR->SetOption( &ChatOption );

//SW050822 Add GraphicCtrl

	//LOD
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );	//SW 기획, 그래픽 전달은 0레벨이 최소 버텍스지만 여기서는 2 레벨이 최소이다.
	//Effect
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );
//!
	if( m_GameOption.bAutoCtrl )
	{
		
	}
//
	//SW060904 Add MapAmbientCtrlOptn
	if( m_GameOption.bAmbientMax )
	{
		g_pExecutive->GetGeometry()->SetAmbientColor(0,0xffffffff);
	}
	else
	{
		MAPDESC* pmapdesc = MAP->GetMapDesc();
		if(pmapdesc)
			g_pExecutive->GetGeometry()->SetAmbientColor(0,pmapdesc->Ambient);
	}


	CHATTIPMGR->SetActive( m_GameOption.bNoGameTip );
	OPTIONMGR->SetMemberDemageActive( m_GameOption.bNoMemberDamage );
	MACROMGR->SetMacroMode( m_GameOption.nMacroMode );

	// magi82 - 로그인시만 문파소개창 옵션지원 070108
	if( GAMEIN->GetGuildTraineeDialog() )
	{
		CHero* pHero = OBJECTMGR->GetHero();

		if(m_GameOption.bMunpaIntro && pHero->GetLevel() < 40 && !pHero->GetMunpaIdx())
			GAMEIN->GetGuildTraineeDialog()->SendPage(1, TRUE);
	}
	//////////////////////////////////////////////////////////////////////////
	
//////////////////
//단축키 툴팁 변경
	if( GAMEIN->GetMainInterfaceDialog() )
		GAMEIN->GetMainInterfaceDialog()->Refresh();

//////
//

	if( GAMEIN->IsGameInAcked() )
	if( HERO->GetAvatarView() != (bool)m_GameOption.bNoAvatarView )
	{
		HERO->SetAvatarView( (bool)m_GameOption.bNoAvatarView );
		APPEARANCEMGR->InitAppearance( HERO );
	}

//메시지보내기
//	if( bMsg )
//		SendOptionMsg();
}

void COptionManager::CancelSettings()
{
	//직접 영향 주었던 것들 제대로 돌려놓기
}

void COptionManager::SetDefaultOption()
{
	m_GameOption.bNoDeal			= FALSE;
	m_GameOption.bNoParty			= FALSE;
	m_GameOption.bNoFriend			= FALSE;
	m_GameOption.bNoVimu			= FALSE;
	m_GameOption.bNameMunpa			= TRUE;
	m_GameOption.bNameParty			= TRUE;
	m_GameOption.bNameOthers		= TRUE;
	m_GameOption.bNoMemberDamage	= TRUE;
	m_GameOption.bNoGameTip			= TRUE;
	m_GameOption.bMunpaIntro		= TRUE;	// magi82 - 로그인시만 문파소개창 옵션지원 070108

	m_GameOption.nMacroMode			= 1;	//MM_MACRO

	m_GameOption.bNoWhisper			= FALSE;
	m_GameOption.bNoChatting		= FALSE;
	m_GameOption.bNoBalloon			= FALSE;
	m_GameOption.bAutoHide			= FALSE;
	m_GameOption.bNoShoutChat		= FALSE;
	m_GameOption.bNoGuildChat		= FALSE;
	m_GameOption.bNoAllianceChat	= FALSE;

	m_GameOption.bNoSystemMsg		= FALSE;
	m_GameOption.bNoExpMsg			= FALSE;
	m_GameOption.bNoItemMsg			= FALSE;

	m_GameOption.nGamma				= 50;	//%?
	m_GameOption.nSightDistance		= 75;	//50~100%?
	m_GameOption.bGraphicCursor		= FALSE;
	m_GameOption.bShadowHero		= TRUE;
	m_GameOption.bShadowMonster		= TRUE;
	m_GameOption.bShadowOthers		= TRUE;


	//SW050822
	m_GameOption.bAutoCtrl			= FALSE;
	//SW060904 Add MapAmbientCtrlOptn
	m_GameOption.bAmbientMax		= TRUE;
	m_GameOption.nLODMode			= 0;	//MODEL_LOD_USING_MODE_DEFAULT
	m_GameOption.nEffectMode		= 0;	//eOO_EFFECT_DEFAULT
	m_GameOption.nEffectSnow        = 0;    // Snow_EFFECT_DEFAULT (2005.12.28추가)


	m_GameOption.bSoundBGM			= TRUE;
	m_GameOption.bSoundEnvironment	= TRUE;
	m_GameOption.nVolumnBGM			= 100;	//1-100
	m_GameOption.nVolumnEnvironment	= 100;	//1-100

//	m_GameOption.wKyungGongIdx		= 0;
//	m_GameOption.bIsKyungGongMode	= FALSE;

#ifdef _KOR_LOCAL_
	m_GameOption.nLoginCombo = 0;	// magi82
#endif

	m_GameOption.bIntroFlag = FALSE;		// magi82 - Intro(070713)
	m_GameOption.bNoAvatarView = FALSE;

}

void COptionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_OPTION_SET_ACK:
		{

		}
		break;

	case MP_OPTION_SET_NACK:
		{

		}
		break;
	case MP_OPTION_AVATARVIEW:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer )		return;

			pPlayer->SetAvatarView( pmsg->bData );
			APPEARANCEMGR->InitAppearance( pPlayer );
		}
		break;
	}
}
/*
void COptionManager::SetKyungGongIdx(WORD KyungGondIdx)
{ 
	m_GameOption.wKyungGongIdx = KyungGondIdx;
	SaveGameOption();
}

BOOL COptionManager::GetKyungGongMode()
{
	return m_GameOption.bIsKyungGongMode;
}

void COptionManager::SetKyungGongMode(BOOL bMode)
{
	m_GameOption.bIsKyungGongMode = bMode;
	SaveGameOption();
}

void COptionManager::ClrKyungGong()
{
	m_GameOption.wKyungGongIdx = 0;
	m_GameOption.bIsKyungGongMode = FALSE;
	SaveGameOption();
}
*/


void COptionManager::GraphicAutoCtrlProcess()
{
	// 기샹예보 2005.12.30 추가 
	WEATHERMGR->SetWeather(m_GameOption.nEffectSnow);
	if( !m_GameOption.bAutoCtrl ) return;

	

	DWORD Frame = 0;
	if(gTickTime)
	{
		Frame = 1000 / gTickTime;
		//10초간의 평균 프레임을 구해 기준 프레임과 비교하여 처리한다.
		++m_dwCountFrame;
		m_dwFrameAverage += Frame;
	}
	//Effect
	//State counter
	if( gCurTime - m_dwLastCheckTime > 10000 )
	{
		m_dwFrameAverage /= m_dwCountFrame;
		if( STANDARD_FRAME > m_dwFrameAverage ) m_nCurOptionState--;
		else if( NICE_FRAME < m_dwFrameAverage ) m_nCurOptionState++;
		m_dwFrameAverage = m_dwCountFrame = 0;
		if( m_nCurOptionState < LOWEST )
			m_nCurOptionState = 0;
		else if( m_nCurOptionState > HIGH )
			m_nCurOptionState = 5;
		m_dwLastCheckTime = gCurTime;
	}

	switch( m_nCurOptionState )
	{
	case LOWEST:
		{
			if( m_dwCheck == LOWEST ) return;
			//시야거리 최저
			m_GameOption.nSightDistance = m_SigntGBMinValue;
			//LOD 낮추기
			m_GameOption.nLODMode = TRUE;
			//그림자 끄기
			m_GameOption.bShadowHero = FALSE;
			m_GameOption.bShadowMonster = FALSE;
			m_GameOption.bShadowOthers = FALSE;
			m_dwShdowOption = eOO_SHADOW_NONE;
			//이펙트 끄기
			m_GameOption.nEffectMode = TRUE;

			
			ApplyGraphicSetting();
			
			m_dwCheck = LOWEST;			
		}
		break;
	case LOW:
		{
			if( m_dwCheck == LOW ) return;
			//시야거리 최저
			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 4;
			//LOD 낮추기
			m_GameOption.nLODMode = TRUE;
			//그림자 낮추기
			m_GameOption.bShadowHero = FALSE;
			m_GameOption.bShadowMonster = FALSE;
			m_GameOption.bShadowOthers = FALSE;
			m_dwShdowOption = eOO_SHADOW_NONE;
			//이펙트 켜기
			m_GameOption.nEffectMode = FALSE;
			
			ApplyGraphicSetting();
			
			m_dwCheck = LOW;			
		}
		break;
	case MID:
		{
			if( m_dwCheck == MID ) return;
			//시야거리 최저
			m_GameOption.nSightDistance = m_SigntGBMinValue + ( m_SigntGBMaxValue - m_SigntGBMinValue ) / 2;
			//LOD	낮추기
			m_GameOption.nLODMode = TRUE;
			//그림자 켜기
			m_GameOption.bShadowHero = TRUE;
			m_GameOption.bShadowMonster = FALSE;
			m_GameOption.bShadowOthers = FALSE;
			m_dwShdowOption = eOO_SHADOW_ONLYHERO;
			//이펙트 켜기
			m_GameOption.nEffectMode = FALSE;
			
			ApplyGraphicSetting();
			
			m_dwCheck = MID;
		}
		break;
	case HIGH:
		{
			if( m_dwCheck == HIGH ) return;
			//시야거리 최고
			m_GameOption.nSightDistance = m_SigntGBMaxValue;
			//LOD 원상태
			m_GameOption.nLODMode = FALSE;
			//그림자 켜기
			m_GameOption.bShadowHero = TRUE;
			m_GameOption.bShadowMonster = TRUE;
			m_GameOption.bShadowOthers = TRUE;
			m_dwShdowOption = (eOO_SHADOW_ONLYHERO|eOO_SHADOW_MONSTER|eOO_SHADOW_OTHERCHARACTER);
			//이펙트 켜기
			m_GameOption.nEffectMode = FALSE;
			
			ApplyGraphicSetting();
			
			m_dwCheck = HIGH;			
		}
		break;

	default:
		return;
	}
}

void COptionManager::ApplyGraphicSetting()
{
	CAMERA->SetSightDistanceRate( (float)m_GameOption.nSightDistance );
	g_pExecutive->SetModelLODUsingMode( m_GameOption.nLODMode, 2 );
	OBJECTMGR->SetShadowOption( m_dwShdowOption );
	OBJECTMGR->SetEffectOption( m_GameOption.nEffectMode );
//	m_dwLowestCount = m_dwLowCount = m_dwMidCount = m_dwHighCount = 0;
}

void COptionManager::InitForGameIn()
{
	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()
	m_pOptionDlg = (COptionDialog*)WINDOWMGR->GetWindowForID( OTI_TABDLG );
	cDialog* pDlg = (cDialog*)m_pOptionDlg->GetTabSheet(2);
	m_pCammaGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_GAMMA ));		//현재 적용은 안됨
	m_pSightGB = (cGuageBar*)(pDlg->GetWindowForID( OTI_GB_SIGHT ));
	m_SigntGBMinValue = m_pSightGB->GetMinValue();
	m_SigntGBMaxValue = m_pSightGB->GetMaxValue();
	m_dwShdowOption = eOO_SHADOW_NONE;
	m_dwLastCheckTime = m_dwCheck = m_dwFrameAverage = m_dwCountFrame = 0;
	m_nCurOptionState = HIGH;
}

