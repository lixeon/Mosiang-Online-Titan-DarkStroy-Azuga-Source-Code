// SeigeWarDialog.cpp: implementation of the CSeigeWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeigeWarDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./interface/cEditBox.h"
#include "./interface/cTextArea.h"
#include "./interface/cListDialog.h"
#include "./ObjectGuagen.h"
#include "../Input/Mouse.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "NpcScriptDialog.h"
#include "GameIn.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSWTimeRegDlg::CSWTimeRegDlg()
{
	m_type = WT_SEIGEWAR_TIMEREG_DLG;

}

CSWTimeRegDlg::~CSWTimeRegDlg()
{
}

void CSWTimeRegDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if( val )
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		char temp[256];
		sprintf( temp, "%d", time.wYear );
		m_pYearEdit->SetEditText( temp );
		sprintf( temp, "%d", time.wMonth );
		m_pMonEdit->SetEditText( temp );
		sprintf( temp, "%d", time.wDay );
		m_pDayEdit->SetEditText( temp );
		sprintf( temp, "%d", time.wHour );
		m_pHourEdit->SetEditText( temp );
	}
	else
	{
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void CSWTimeRegDlg::Linking()
{
	m_pText = (cTextArea*)GetWindowForID(SW_TIMEREG_TEXT);
	m_pYearEdit = (cEditBox*)GetWindowForID(SW_TIME_YEAR);
	m_pMonEdit = (cEditBox*)GetWindowForID(SW_TIME_MON);
	m_pDayEdit = (cEditBox*)GetWindowForID(SW_TIME_DAY);
	m_pHourEdit = (cEditBox*)GetWindowForID(SW_TIME_HOUR);
	m_pRegistBtn = (cButton*)GetWindowForID(SW_TIMEREG_OKBTN);
	
	m_pText->SetScriptText( CHATMGR->GetChatMsg( 1124 ) );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSWProtectRegDlg::CSWProtectRegDlg()
{
	m_type = WT_SEIGEWAR_PROTECTREG_DLG;
	m_nSelectedIdx = -1;
}

CSWProtectRegDlg::~CSWProtectRegDlg()
{
}

DWORD CSWProtectRegDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive )	return we;
	we = cDialog::ActionEvent( mouseInfo );	

	if( m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1 )
	{
		if( (we&WE_LBTNCLICK) && (WINDOWMGR->IsMouseDownUsed()==FALSE) )
		{		
			int Idx = m_pListDlg->GetCurSelectedRowIdx();
			if( Idx != -1 )
			{
				m_nSelectedIdx = Idx;
			}
		}
	}
	return we;
}

void CSWProtectRegDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if( val == FALSE )
	{
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void CSWProtectRegDlg::Linking()
{
	m_pListDlg = (cListDialog*)GetWindowForID(SW_PROTECTREG_LIST);
	m_pRegBtn = (cButton*)GetWindowForID(SW_PROTECTREG_OKBTN);
}

void CSWProtectRegDlg::ShowList( SEND_SW_PROPOSALGUILDLIST* pMsg )
{
	memset( m_GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM );

	m_pListDlg->RemoveAll();
	for( int i = 0; i < pMsg->Count; ++i )
	{
		m_GuildList[i] = pMsg->GuildList[i];

		m_pListDlg->AddItem( m_GuildList[i].GuildName, 0xffffffff );
	}

	SetActive( TRUE );
}

char* CSWProtectRegDlg::GetSelectedGuildName( int nIdx )
{
	if( nIdx == -1 )	return NULL;

	return m_GuildList[nIdx].GuildName;
}

DWORD CSWProtectRegDlg::GetSelectedGuildIdx()
{
	if( m_nSelectedIdx == -1 )	return 0;

	return m_GuildList[m_nSelectedIdx].Info.GuildIdx;
}

void CSWProtectRegDlg::Test()
{
	memset( m_GuildList, 0, sizeof(SIEGEWAR_GUILDSENDINFO)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM );

	m_pListDlg->RemoveAll();
	m_pListDlg->AddItem( "testroooooo!!", 0xffffffff );
	m_pListDlg->AddItem( "테스트랍니다!!", 0xffffffff );

	SetActive( TRUE );
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSWInfoDlg::CSWInfoDlg()
{
	m_type = WT_SEIGEWAR_INFO_DLG;
}

CSWInfoDlg::~CSWInfoDlg()
{
}

void CSWInfoDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val == FALSE )
	{
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void CSWInfoDlg::Linking()
{
	m_pDefList = (cListDialog*)GetWindowForID(SW_DEFENSE_LIST);
	m_pOffList = (cListDialog*)GetWindowForID(SW_OFFENSE_LIST);
	m_pCloseBtn = (cButton*)GetWindowForID(SW_INFOCLOSEBTN);
}

void CSWInfoDlg::ShowSWInfo( SEND_SW_GUILDLIST* pMsg )
{
	m_pDefList->RemoveAll();
	m_pOffList->RemoveAll();
	WORD wCount = pMsg->DefenceCount + pMsg->AttackCount;
	for( WORD i = 0; i < wCount; ++i )
	{
		if( pMsg->GuildList[i].Info.Type == 1 || pMsg->GuildList[i].Info.Type == 2 )
		{
			m_pDefList->AddItem( pMsg->GuildList[i].GuildName, 0xffffffff );
		}
		else if( pMsg->GuildList[i].Info.Type == 4 )
		{
			m_pOffList->AddItem( pMsg->GuildList[i].GuildName, 0xffffffff );
		}		
	}

	SetActive( TRUE );
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSWProfitDlg::CSWProfitDlg()
{
	m_type = WT_SEIGEWAR_PROFIT_DLG;
}

CSWProfitDlg::~CSWProfitDlg()
{
}

void CSWProfitDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val == FALSE )
	{
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		m_pChangeTaxEdit->SetFocus( FALSE );
		m_pOutMoneyEdit->SetFocus( FALSE );
	}
}

void CSWProfitDlg::Linking()
{
	m_pstBasicTax = (cStatic*)GetWindowForID(SW_BACIS_TAX);
	m_pChangeTaxEdit = (cEditBox*)GetWindowForID(SW_CHANGE_TAX);
	m_pstBasicMoney = (cStatic*)GetWindowForID(SW_BACIS_MONEY);
	m_pOutMoneyEdit = (cEditBox*)GetWindowForID(SW_OUT_MONEY);
	m_pTaxChangeOkBtn = (cButton*)GetWindowForID(SW_TAXCHANGE_OKBTN);
	m_pProfitOkBtn = (cButton*)GetWindowForID(SW_PROFIT_OKBTN);
	m_pCancelBtn = (cButton*)GetWindowForID(SW_PROFIT_CANCELBTN);

	m_pstBasicTax->SetStaticText( "0" );
	m_pChangeTaxEdit->SetEditText( "0" );
	m_pstBasicMoney->SetStaticText( "0" );
	m_pOutMoneyEdit->SetEditText( "0" );
}

void CSWProfitDlg::ShowProfitDlg( int nTaxRate, DWORD dwMoney ) 
{
	char temp[256] ={0,};
	m_pstBasicTax->SetStaticText( itoa(nTaxRate, temp, 10) );
	m_pChangeTaxEdit->SetEditText( itoa(0, temp, 10) );
	m_pstBasicMoney->SetStaticText( AddComma(dwMoney) );
	m_pOutMoneyEdit->SetEditText( itoa(0, temp, 10) );

	SetActive( TRUE );
}





//
CSWEngraveDialog::CSWEngraveDialog()
{
	m_type = WT_SIEGEWAR_ENGRAVE_DLG;
	m_bEngrave = FALSE;
}

CSWEngraveDialog::~CSWEngraveDialog()
{
}

void CSWEngraveDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		m_bEngrave = FALSE;
		m_ProcessTime = 0;
		m_BackupTime = 0;
	}
}

void CSWEngraveDialog::Linking()
{
	m_pEngraveGuagen = (CObjectGuagen*)GetWindowForID( SW_ENGRAVEGUAGE );
	m_pRemaintimeStatic = (cStatic*)GetWindowForID( SW_ENGRAVETIME );
}

void CSWEngraveDialog::Process()
{
	if( !m_bEngrave )							return;
	if( m_ProcessTime >= 1000*ENGRAVETIME )		return;

	m_ProcessTime += gTickTime;	
	m_pEngraveGuagen->SetValue( (GUAGEVAL)(m_ProcessTime/(1000*ENGRAVETIME)), gTickTime );

	if( m_BackupTime == (DWORD)(m_ProcessTime/1000) )		return;

	m_BackupTime = (DWORD)(m_ProcessTime/1000);
		
	if( (ENGRAVETIME-(m_BackupTime))  < 1 )
	{
		m_ProcessTime = ENGRAVETIME-1;
		m_BackupTime = ENGRAVETIME-1;
	}

	char buf[128];
	sprintf( buf, CHATMGR->GetChatMsg(1043), ENGRAVETIME-(m_BackupTime+1) );
	m_pRemaintimeStatic->SetStaticText( buf );
}

void CSWEngraveDialog::StartEngrave()
{
	SetActive( TRUE );

	m_bEngrave = TRUE;
	m_ProcessTime = 0;
	m_BackupTime = 0;

	char buf[128];
	sprintf( buf, CHATMGR->GetChatMsg(1043), ENGRAVETIME-1 );
	m_pRemaintimeStatic->SetStaticText( buf );
}

void CSWEngraveDialog::CancelEngraveSyn()
{
	if( !m_bEngrave )			return;

	MSGBASE msg;
	SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_CANCEL );
	msg.dwObjectID = HEROID;
	NETWORK->Send( &msg, sizeof(msg) );
}

void CSWEngraveDialog::CancelEngrave()
{
	m_bEngrave = FALSE;
	m_ProcessTime = 0;
	m_BackupTime = 0;

	SetActive( FALSE );
}








CSWTimeDialog::CSWTimeDialog()
{
	m_type = WT_SIEGEWAR_TIME_DLG;
	m_FightTime = 0;
	m_bStartBattle = FALSE;
}

CSWTimeDialog::~CSWTimeDialog()
{
}


void CSWTimeDialog::Linking()
{
	m_pTimeStatic = (cStatic*)GetWindowForID( SW_TIME );	
	m_pMunpaName = (cStatic*)GetWindowForID( SWTIME_MUNPA );
}


void CSWTimeDialog::Process()
{
	if( !m_bStartBattle )			return;

	if( m_FightTime && gTickTime < m_FightTime )	m_FightTime -= gTickTime;
	else											m_FightTime = 0;

	char buf[32];
	sprintf(buf, "%02d:%02d", m_FightTime/(HOURTOSECOND*1000), (((m_FightTime%(HOURTOSECOND*1000))/(MINUTETOSECOND*1000))+1));
	m_pTimeStatic->SetStaticText(buf);
}


void CSWTimeDialog::StartTime( DWORD Time )
{
	m_FightTime = Time;
	m_bStartBattle = TRUE;

	SetActive( TRUE );
}


void CSWTimeDialog::EndTime()
{
	m_FightTime = 0;
	m_bStartBattle = FALSE;

	SetActive( FALSE );
}

void CSWTimeDialog::SetMunpaName( char* strName )
{
	m_pMunpaName->SetStaticText( strName );
}












CSWStartTimeDialog::CSWStartTimeDialog()
{
	m_type = WT_SIEGEWAR_TIME_DLG;
}

CSWStartTimeDialog::~CSWStartTimeDialog()
{
}

void CSWStartTimeDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val == FALSE )
	{
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

void CSWStartTimeDialog::Linking()
{
	m_pTime0 = (cStatic*)GetWindowForID( SW_STARTTIME );
	m_pTime1 = (cStatic*)GetWindowForID( SW_REGTIMESTART );
	m_pTime2 = (cStatic*)GetWindowForID( SW_REGTIMEEND );
	m_pTime3 = (cStatic*)GetWindowForID( SW_ADMITTIMESTART );
	m_pTime4 = (cStatic*)GetWindowForID( SW_ADMITTIMEEND );
	m_pOkBtn = (cButton*)GetWindowForID( SW_STARTTIME_OKBTN );
}

void CSWStartTimeDialog::ShowStartTimeDlg( DWORD dwTime0, DWORD dwTime1 )
{
	stTIME time;
	time.SetTime( 0, 0, 2, 0, 0, 0 );

	stTIME time0, time1;
	time0.value = dwTime0;
	time1.value = dwTime1;

	char temp[256];

	sprintf( temp, CHATMGR->GetChatMsg(1174), time1.GetMonth(), time1.GetDay(), time1.GetHour() );
	m_pTime0->SetStaticText( temp );

	sprintf( temp, CHATMGR->GetChatMsg(1175), time0.GetMonth(), time0.GetDay(), time0.GetHour() );
	m_pTime3->SetStaticText( temp );

	time0 += time;
	//SW071130 강PM님 요청으로 수정.
	DWORD dayEcp = time0.GetDay();
	DWORD hourEcp = time0.GetHour();

	if( 1 != dayEcp )
	{
		dayEcp -= 1;
		hourEcp += 24;
	}

	sprintf( temp, CHATMGR->GetChatMsg(1176), time0.GetMonth(), time0.GetDay()-1, time0.GetHour()+24 );
	m_pTime4->SetStaticText( temp );
	
	sprintf( temp, CHATMGR->GetChatMsg(1175), time0.GetMonth(), time0.GetDay(), time0.GetHour() );
	m_pTime1->SetStaticText( temp );

	time.SetTime( 0, 0, 3, 0, 0, 0 );
	time0 += time;
	//SW071130 강PM님 요청으로 수정.
	dayEcp = time0.GetDay();
	hourEcp = time0.GetHour();

	if( 1 != dayEcp )
	{
		dayEcp -= 1;
		hourEcp += 24;
	}
	sprintf( temp, CHATMGR->GetChatMsg(1176), time0.GetMonth(), time0.GetDay()-1, time0.GetHour()+24 );
	m_pTime2->SetStaticText( temp );	
	
	SetActive( TRUE );
}

void CSWStartTimeDialog::Test()
{
	stTIME time;
	time.SetTime( 0, 0, 2, 0, 0, 0 );

	stTIME time0, time1;
	time0.SetTime( 2005, 11, 5, 11, 0, 0 );
	time1.SetTime( 2005, 10, 31, 11, 0, 0 );

	char temp[256];

	sprintf( temp, "%02d월 %02d일 %02d시", time1.GetMonth(), time1.GetDay(), time1.GetHour() );
	m_pTime0->SetStaticText( temp );

	sprintf( temp, "%02d월 %02d일 %02d시 부터", time0.GetMonth(), time0.GetDay(), time0.GetHour() );
	m_pTime3->SetStaticText( temp );

	time0 += time;
	sprintf( temp, "%02d월 %02d일 %02d시 까지", time0.GetMonth(), time0.GetDay()-1, time0.GetHour()+24 );
	m_pTime4->SetStaticText( temp );
	
	sprintf( temp, "%02d월 %02d일 %02d시 부터", time0.GetMonth(), time0.GetDay(), time0.GetHour() );
	m_pTime1->SetStaticText( temp );

	time0 += time;
	sprintf( temp, "%02d월 %02d일 %02d시 까지", time0.GetMonth(), time0.GetDay()-1, time0.GetHour()+24 );
	m_pTime2->SetStaticText( temp );

	SetActive( TRUE );
}
