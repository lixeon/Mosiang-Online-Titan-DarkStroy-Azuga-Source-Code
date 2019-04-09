#include "stdafx.h"
#include "AutoNoteManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "AutoAnswerDlg.h"
#include "AutoNoteDlg.h"


GLOBALTON(CAutoNoteManager)

CAutoNoteManager::CAutoNoteManager(void)
{
}

CAutoNoteManager::~CAutoNoteManager(void)
{
}

void CAutoNoteManager::ToggleAutoNote()
{
	if( GAMEIN->GetAutoNoteDlg()->IsActive() )
		GAMEIN->GetAutoNoteDlg()->SetActive(FALSE);
	else
		GAMEIN->GetAutoNoteDlg()->SetActive(TRUE);
}

//---오토 적발을 위해
void CAutoNoteManager::AskToAutoUser( DWORD dwAutoPlayerIdx, DWORD dwQuestion )
{
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		//1723 "[살생부]은신중에는 살생부를 사용할 수 없습니다."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1723 ) );
		return;
	}

	MSG_DWORD2 msg;
	msg.Category = MP_AUTONOTE;
	msg.Protocol = MP_AUTONOTE_ASKTOAUTO_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = dwAutoPlayerIdx;
	msg.dwData2 = dwQuestion;		//---허당

	NETWORK->Send( &msg, sizeof(msg) );
}

//---오토 답변을 위해
void CAutoNoteManager::AnswerToQuestion( DWORD dwData1, DWORD dwData2, DWORD dwData3, DWORD dwData4 )
{
	MSG_DWORD4 msg;
	msg.Category	= MP_AUTONOTE;
	msg.Protocol	= MP_AUTONOTE_ANSWER_SYN;
	msg.dwObjectID	= HEROID;

	msg.dwData1	 = dwData1;
	msg.dwData2	 = dwData2;
	msg.dwData3	 = dwData3;
	msg.dwData4	 = dwData4;

	NETWORK->Send(&msg, sizeof(msg));
}

void CAutoNoteManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_AUTONOTE_ASKTOAUTO_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1705 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상에게 문제를 전송하였습니다." );
		}
		break;

	case MP_AUTONOTE_ASKTOAUTO_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eAutoNoteError_CantUse:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1706 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]살생부를 사용할 수 없습니다." );
				break;
			case eAutoNoteError_CantUseMap:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1707 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]살생부를 사용할 수 없는 지역입니다." );
				break;
			case eAutoNoteError_AlreadyAsking:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1708 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]현재 이미 다른 대상에게 살생부를 사용 중입니다." );
				break;
			case eAutoNoteError_CantFind:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1709 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상을 찾을 수 없습니다." );
				break;
			case eAutoNoteError_AlreadyAsked:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1710 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상은 살생부 문제를 풀고 있는 중입니다." );
				break;
			case eAutoNoteError_NotProperState:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1711 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상은 살생부를 사용할 수 있는 상태가 아닙니다." );
				break;			
			default:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1712 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]살생부 사용에 실패하였습니다." );
				break;
			};
		}
		break;

	case MP_AUTONOTE_ANSWER_NACK:	//오토유저-답변이 틀렸다
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1713 ), pmsg->dwData );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]답이 틀렸습니다. %d번의 기회가 남았습니다.", pmsg->dwData );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->Retry();
		}
		break;

	case MP_AUTONOTE_NOTAUTO:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1714 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상이 답변을 맞추었습니다." );
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1715 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]답변을 맞추셨습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1716 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]답변이 틀려 접속을 끊습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1717 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]답변 제한시간이 지나 접속을 끊습니다." );

			if( GAMEIN->GetAutoAnswerDlg() )
				GAMEIN->GetAutoAnswerDlg()->SetActive( FALSE );
		}
		break;

	case MP_AUTONOTE_KILLAUTO:		//---상대 제재됨
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1718 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]선택한 대상이 답변을 맞추지 못하여 접속이 종료되었습니다." );
		}
		break;

	case MP_AUTONOTE_PUNISH:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			char buf[256] = {0};
			char temp[64];
			DWORD milisec = pmsg->dwData;
			DWORD hour = milisec / 60 / 60 / 1000;		milisec -= hour * 60 * 60 * 1000;
			DWORD min = milisec / 60 / 1000;			milisec -= min * 60 * 1000;			
			DWORD sec = milisec / 1000;

//			if( hour ) { sprintf( temp, CHATMGR->GetChatMsg( 1410 )/*"%d시간 "*/, hour ); strcat( buf, temp ); }
//			if( min )  { sprintf( temp, CHATMGR->GetChatMsg( 1411 )/*"%d분 "*/, min ); strcat( buf, temp ); }
//			sprintf( temp, CHATMGR->GetChatMsg( 1412 )/*"%d초"*/, sec ); strcat( buf, temp );
//
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1413 ), buf ); //"[오토노트]%s 후에 사용할 수 있습니다.", buf );

			if( hour ) { sprintf( temp, "%d시간 ", hour ); strcat( buf, temp ); }
			if( min )  { sprintf( temp, "%d분 ", min ); strcat( buf, temp ); }
			sprintf( temp, "%d초", sec ); strcat( buf, temp );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1719 ), buf );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]%s 후에 사용할 수 있습니다.", buf );
		}
		break;

	case MP_AUTONOTE_LIST_ADD:
		{
			MSG_AUTOLIST_ADD* pmsg = (MSG_AUTOLIST_ADD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1720 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[살생부]살생부에 이름이 추가되었습니다." );

			if( GAMEIN->GetAutoNoteDlg() )
				GAMEIN->GetAutoNoteDlg()->AddAutoList( pmsg->Name, pmsg->Date );				
		}
		break;

	case MP_AUTONOTE_LIST_ALL:
		{
 			MSG_AUTOLIST_ALL* pmsg = (MSG_AUTOLIST_ALL*)pMsg;

//			CHATMGR->AddMsg( CTC_SYSMSG, "[오토노트]오토노트 목록을 받아왔습니다." );

			int nCount = pmsg->nCount;

			if( GAMEIN->GetAutoNoteDlg() )
			{
				for( int i = 0 ; i < nCount ; ++i )
				{
					GAMEIN->GetAutoNoteDlg()->AddAutoList( pmsg->row[i].Name, pmsg->row[i].Date );
				}
			}			
		}
		break;

	case MP_AUTONOTE_ASKTOAUTO_IMAGE:
		{
			MSG_AUTONOTE_IMAGE* pmsg = (MSG_AUTONOTE_IMAGE*)pMsg;

			if( GAMEIN->GetAutoAnswerDlg() )
			{
				GAMEIN->GetAutoAnswerDlg()->Shuffle( m_dwColorTable );	//위치만 랜덤으로 잡아준다. (버튼위치 섞는것 주석처리되었음)
				GAMEIN->GetAutoAnswerDlg()->SaveImage( pmsg->Image );
				GAMEIN->GetAutoAnswerDlg()->SetQuestion( "" );
				GAMEIN->GetAutoAnswerDlg()->SetActiveWithTime( TRUE, pmsg->dwReplyTime );
			}
		}
		break;
	}
}
