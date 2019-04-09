#include "stdafx.h"
#include "AutoNoteManager.h"
#include "UserTable.h"
#include "Player.h"
#include "AutoNoteRoom.h"
#include "Network.h"
#include "MapDBMsgParser.h"


GLOBALTON(CAutoNoteManager)

CAutoNoteManager::CAutoNoteManager(void)
{
	m_bInited = FALSE;
}

CAutoNoteManager::~CAutoNoteManager(void)
{
	Release();
}

void CAutoNoteManager::Init()
{
	m_htAutoNoteRoom.Initialize( 100 );
	m_pmpAutoNoteRoom = new CMemoryPoolTempl< CAutoNoteRoom >;
	m_pmpAutoNoteRoom->Init( 100, 100, "CAutoNoteRoom" );

	m_dwReplyTime = 120;	// 초
	m_nReplyChance = 3;

	//---이미지로드
	m_pOriRaster[0] = new BYTE[16*16*3];
	m_pOriRaster[1] = new BYTE[16*16*3];
	m_pOriRaster[2] = new BYTE[16*16*3];
	m_pOriRaster[3] = new BYTE[16*16*3];
	m_pBGRaster		= new BYTE[128*32*3];
	m_pNoiseRaster	= new BYTE[16*16*3];
	m_pSendRaster	= new BYTE[128*32*3];

	if( BMP_RasterLoad( "./AutoNoteImage/01Red.bmp", m_pOriRaster[0] ) == FALSE ) return;
	if( BMP_RasterLoad( "./AutoNoteImage/02Yellow.bmp", m_pOriRaster[1] ) == FALSE ) return;
	if( BMP_RasterLoad( "./AutoNoteImage/03Blue.bmp", m_pOriRaster[2] ) == FALSE ) return;
	if( BMP_RasterLoad( "./AutoNoteImage/04Black.bmp", m_pOriRaster[3] ) == FALSE ) return;
	if( BMP_RasterLoad( "./AutoNoteImage/00BG.bmp", m_pBGRaster ) == FALSE ) return;
	if( BMP_RasterLoad( "./AutoNoteImage/00Noise.bmp", m_pNoiseRaster ) == FALSE ) return;

	m_bInited = TRUE;

	return;
}

void CAutoNoteManager::Release()
{
	m_htAutoNoteRoom.SetPositionHead();
	CAutoNoteRoom* pRoom = NULL;
	while( pRoom = m_htAutoNoteRoom.GetData() )
	{		
		m_pmpAutoNoteRoom->Free( pRoom );
	}
	m_htAutoNoteRoom.RemoveAll();	
	
	SAFE_DELETE( m_pmpAutoNoteRoom );

	/*
	m_pmpAutoNoteRoom->Release();
	delete m_pmpAutoNoteRoom;
	m_htAutoNoteRoom.RemoveAll();
	*/

	//---이미지해제
	delete[] m_pOriRaster[0];
	delete[] m_pOriRaster[1];
	delete[] m_pOriRaster[2];
	delete[] m_pOriRaster[3];
	delete[] m_pBGRaster;
	delete[] m_pNoiseRaster;
	delete[] m_pSendRaster;
}

BOOL CAutoNoteManager::BMP_RasterLoad( char* filename, BYTE* pRaster )
{
	HANDLE hFile = CreateFile( filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	BYTE* data = new BYTE[dwFileSize];

	DWORD dwRead;
	ReadFile( hFile, data, dwFileSize, &dwRead, NULL );
	CloseHandle( hFile );

	BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*)data;
	BITMAPINFOHEADER* ih = (BITMAPINFOHEADER*)( data + sizeof(BITMAPFILEHEADER) );

	int RasterSize = ( ih->biWidth * ih->biHeight * ih->biBitCount ) / 8;
	if( RasterSize < 0 ) RasterSize = -RasterSize;

	memcpy( pRaster, data + ((BITMAPFILEHEADER*)fh)->bfOffBits, RasterSize );

	delete[] data;

	return TRUE;
}

void CAutoNoteManager::MakeSendRaster( DWORD* pData )
{
	int x, y, cx, cy, rot;
	float fszx, fszy;

	//---Clear
	memset( m_pSendRaster, 0xee, 128*32*3 ); 

	//---Background
	fszx = (rand()%5 + 11) / 10.f;	fszy = (rand()%5 + 11) / 10.f;
	BlitImage( m_pSendRaster, m_pBGRaster, 128, 32, 128, 32, 0, 0, fszx, fszy );

	//---noise
	for( int i = 0 ; i < 4 ; ++i )
	{
		x = rand()%(128-16);				y = rand()%(32-16);
		fszx = (rand()%10 + 10) / 10.f;		fszy = (rand()%10 + 10) / 10.f;
		cx = rand()%4 + 6;					cy = rand()%4 + 6;
		rot = rand()%41 - 20;
		BlitImage( m_pSendRaster, m_pNoiseRaster, 128, 32, 16, 16, x, y, fszx, fszy, rot, cx, cy );
	}

	//---txt
	int firstpos = rand()%32;
	int xrand = ( ( 128 - firstpos ) - 90 ) / 4;
	for( int i = 0 ; i < 4 ; ++i )
	{
		x = rand()%xrand + firstpos + i*24;	//i*32;
		y = rand()%8 + 3;
		fszx = (rand()%5 + 11) / 10.f;		fszy = (rand()%5 + 11) / 10.f;
		cx = rand()%4 + 6;					cy = rand()%4 + 6;
		rot = rand()%61 - 30;
		BlitImage( m_pSendRaster, m_pOriRaster[pData[i]], 128, 32, 16, 16, x, y, fszx, fszy, rot, cx, cy, BIF_RANDOMCOLOR );
	}
}

void CAutoNoteManager::BlitImage( BYTE* pDest, BYTE* pSrc, int dw, int dh, int sw, int sh, int x, int y, float fszw, float fszh, float fRot, int cx, int cy, int Flag )
{
	int lsw = sw * fszw;
	int lsh = sh * fszh;

	for( int row = 0 ; row < lsh ; ++row )
	{
		for( int col = 0 ; col < lsw ; ++col )
		{
			int osx = col / fszw + 0.5f;
			int osy = row / fszh + 0.5f;

			if( osx >= sw || osy >= sh )
				continue;

			float fRad = 3.14f * fRot / 180.f;
			int ldx = ( col - cx ) * cos( fRad ) - ( row - cy ) * sin( fRad ) + cx;
			int ldy = ( col - cx ) * sin( fRad ) + ( row - cy ) * cos( fRad ) + cy;

			if( ldx + x >= dw || ldy + y >= dh )
				continue;

			BYTE Color[3]; 
			Color[0] = *(pSrc + osx * 3 + osy * 3 * sw);
			Color[1] = *(pSrc + osx * 3 + osy * 3 * sw + 1);
			Color[2] = *(pSrc + osx * 3 + osy * 3 * sw + 2);

			if( Color[0] != 255 || Color[1] != 255 || Color[2] != 255 )	//흰색이 아니면
			{
				if( Flag == BIF_RANDOMCOLOR )
				{
					//생상 랜덤 조정
					Color[0] = ( rand() % 100 );	//255 뒤집혀도 상관없자나?
					Color[1] = ( rand() % 100 );
					Color[2] = ( rand() % 100 );
				}

				memcpy( pDest + ( ldx + x ) * 3 + ( ldy + y ) * 3 * dw, Color, 3 );
			}
		}
	}
}

void CAutoNoteManager::Process()
{
	CAutoNoteRoom* pANRoom = NULL;
	m_htAutoNoteRoom.SetPositionHead();
	while( pANRoom = m_htAutoNoteRoom.GetData() )
	{
		if( pANRoom->IsTimeOver() )
		{
			CPlayer* pAutoPlayer = (CPlayer*)g_pUserTable->FindUser( pANRoom->GetAutoCharacterIdx() );

			if( pANRoom->GetAutoNoteRoomState() == eANRS_WAITANSWER )	//---답변 입력을 기다리는 중이다. 답변이 늦었다.
			{
				if( pAutoPlayer )
				{
//					MSG_DWORD msg1;
//					msg1.Category = MP_AUTONOTE;
//					msg1.Protocol = MP_AUTONOTE_ANSWER_TIMEOUT;
//					msg1.dwData = pANRoom->GetAutoUserIdx();
//					pAutoPlayer->SendMsg( &msg1, sizeof(msg1) );

					MSG_AUTOUSER_BLOCK msg1;
					msg1.Category = MP_AUTONOTE;
					msg1.Protocol = MP_AUTONOTE_ANSWER_TIMEOUT;
					msg1.dwAskUserIdx = pANRoom->GetAskUserIdx();
					msg1.dwAskCharacterIdx = pANRoom->GetAskCharacterIdx();
					SafeStrCpy( msg1.AskCharacterName, pANRoom->GetAskCharacterName(), MAX_NAME_LENGTH+1 );
					msg1.dwAutoUserIdx = pANRoom->GetAutoUserIdx();
					msg1.dwAutoCharacterIdx = pANRoom->GetAutoCharacterIdx();
					SafeStrCpy( msg1.AutoCharacterName, pANRoom->GetAutoCharacterName(), MAX_NAME_LENGTH+1 );
					pAutoPlayer->SendMsg( &msg1, sizeof(msg1) );
				}

				//---신고자에게 신고대상이 제재되었음을 알림 (어디에 있을지 모르므로 모든 에이전트로 보냄)
				MSG_DWORD msg2;
				msg2.Category = MP_AUTONOTE;
				msg2.Protocol = MP_AUTONOTE_KILLAUTO;
				msg2.dwData = pANRoom->GetAskUserIdx();
				g_Network.Broadcast2AgentServer( (char*)&msg2, sizeof(msg2) );

				//---DB 신고자 오토리스트에 등록
				AutoNoteListAdd( pANRoom->GetAskCharacterIdx(), pANRoom->GetAutoUserIdx(), pANRoom->GetAutoCharacterIdx(), pANRoom->GetAutoCharacterName() );

				//---대상 접속 끊기
				MSG_DWORD msg3;
				msg3.Category = MP_AUTONOTE;
				msg3.Protocol = MP_AUTONOTE_DISCONNECT;
				msg3.dwData = pANRoom->GetAutoUserIdx();
				g_Network.Broadcast2AgentServer( (char*)&msg3, sizeof(msg3) );
			}
			else if( pANRoom->GetAutoNoteRoomState() == eANRS_FASTANSWER )	// 빠른 답변의 경우 오토로 체크
			{
				//---신고자에게 신고대상이 제재되었음을 알림 (어디에 있을지 모르므로 모든 에이전트로 보냄)
				MSG_DWORD msg2;
				msg2.Category = MP_AUTONOTE;
				msg2.Protocol = MP_AUTONOTE_KILLAUTO;
				msg2.dwData = pANRoom->GetAskUserIdx();
				g_Network.Broadcast2AgentServer( (char*)&msg2, sizeof(msg2) );

				//---DB 신고자 오토리스트에 등록
				AutoNoteListAdd( pANRoom->GetAskCharacterIdx(), pANRoom->GetAutoUserIdx(), pANRoom->GetAutoCharacterIdx(), pANRoom->GetAutoCharacterName() );

				//---대상 접속 끊기
				MSG_DWORD msg3;
				msg3.Category = MP_AUTONOTE;
				msg3.Protocol = MP_AUTONOTE_DISCONNECT;
				msg3.dwData = pANRoom->GetAutoUserIdx();
				g_Network.Broadcast2AgentServer( (char*)&msg3, sizeof(msg3) );
			}

			if( pAutoPlayer )	pAutoPlayer->SetAutoAskPlayerIdx( 0 );
			m_htAutoNoteRoom.Remove( pANRoom->GetAskCharacterIdx() );
			m_pmpAutoNoteRoom->Free( pANRoom );
			break;	//한번에 하나씩만 지우자 (안전하게)
		}
	}
}

void CAutoNoteManager::AutoPlayerLogOut( CPlayer* pAutoPlayer )
{
	CAutoNoteRoom* pANRoom = m_htAutoNoteRoom.GetData( pAutoPlayer->GetAutoAskPlayerIdx() );
	if( pANRoom == NULL ) return;

	//---신고대상이 나가버렸다.
//	MSG_DWORD msg;
//	msg.Category = MP_AUTONOTE;
//	msg.Protocol = MP_AUTONOTE_ANSWER_LOGOUT;
//	msg.dwData = pANRoom->GetAutoUserIdx();
//	pAutoPlayer->SendMsg( &msg, sizeof(msg) );

	MSG_AUTOUSER_BLOCK msg1;
	msg1.Category = MP_AUTONOTE;
	msg1.Protocol = MP_AUTONOTE_ANSWER_LOGOUT;
	msg1.dwAskUserIdx = pANRoom->GetAskUserIdx();
	msg1.dwAskCharacterIdx = pANRoom->GetAskCharacterIdx();
	SafeStrCpy( msg1.AskCharacterName, pANRoom->GetAskCharacterName(), MAX_NAME_LENGTH+1 );
	msg1.dwAutoUserIdx = pANRoom->GetAutoUserIdx();
	msg1.dwAutoCharacterIdx = pANRoom->GetAutoCharacterIdx();
	SafeStrCpy( msg1.AutoCharacterName, pANRoom->GetAutoCharacterName(), MAX_NAME_LENGTH+1 );
	pAutoPlayer->SendMsg( &msg1, sizeof(msg1) );

	//---신고자에게 신고대상이 제재되었음을 알림 (어디에 있을지 모르므로 모든 에이전트로 보냄)
	MSG_DWORD msg2;
	msg2.Category = MP_AUTONOTE;
	msg2.Protocol = MP_AUTONOTE_KILLAUTO;
	msg2.dwData = pANRoom->GetAskUserIdx();
	g_Network.Broadcast2AgentServer( (char*)&msg2, sizeof(msg2) );

	//---DB 신고자 오토리스트에 등록
	AutoNoteListAdd( pANRoom->GetAskCharacterIdx(), pANRoom->GetAutoUserIdx(), pANRoom->GetAutoCharacterIdx(), pANRoom->GetAutoCharacterName() );

	//---오토 질답방 해제
	pAutoPlayer->SetAutoAskPlayerIdx( 0 );
	m_htAutoNoteRoom.Remove( pANRoom->GetAskCharacterIdx() );
	m_pmpAutoNoteRoom->Free( pANRoom );
}

void CAutoNoteManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL )	return;

			int error = eAutoNoteError_None;

			if( CanUseAutoNote() == FALSE )						//---이미지 읽는 초기화를 실패하였으므로 오토노트 기능을 사용할 수 없다.
			{
				error = eAutoNoteError_CantUse;					// 이미지 읽기 실패 및 설정에 따라...
				goto AutoNoteAskError;
			}

			if( pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				error = eAutoNoteError_CantUse;					// 은신중 사용못함...
				goto AutoNoteAskError;
			}

			if( g_pServerSystem->GetMap()->IsAutoNoteAllow() == FALSE )
			{
				error = eAutoNoteError_CantUseMap;				//---사용할 수 없는 맵
				goto AutoNoteAskError;				
			}

			if( CAutoNoteRoom* pRoom = m_htAutoNoteRoom.GetData( pPlayer->GetID() ) )
			{
				error = eAutoNoteError_AlreadyAsking;			//---신고중(한번에 한캐릭터만 신고할 수 있다)
				goto AutoNoteAskError;
			}

			CPlayer* pAutoPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
			if( pAutoPlayer == NULL )
			{
				error = eAutoNoteError_CantFind;				//---상대가 없다
				goto AutoNoteAskError;
			}

			if( pAutoPlayer->GetAutoAskPlayerIdx() )		
			{
				error = eAutoNoteError_AlreadyAsked;			//---누가 벌써 신고한 캐릭터다
				goto AutoNoteAskError;
			}

			if( gCurTime > pAutoPlayer->GetLastAttackTimeForAutoNote() + 10*1000 )
			{
				error = eAutoNoteError_NotProperState;			//---상대가 마지막 스킬 사용 후 10초가 지났다. 신고할 수 없는 상태.
				goto AutoNoteAskError;
			}

			CAutoNoteRoom* pANRoom = m_pmpAutoNoteRoom->Alloc();
			if( pANRoom == NULL )								//---방생성 실패
			{
				goto AutoNoteAskError;
			}

			m_htAutoNoteRoom.Add( pANRoom, pPlayer->GetID() );

			pANRoom->CreateRoom( pPlayer, pAutoPlayer, pmsg->dwData2 );
			pANRoom->AskToAuto();
			return;

AutoNoteAskError:
			MSG_DWORD msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_ASKTOAUTO_NACK;
			msg.dwData = error;
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CPlayer* pAutoPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pAutoPlayer == NULL ) return;

			CAutoNoteRoom* pANRoom = m_htAutoNoteRoom.GetData( pAutoPlayer->GetAutoAskPlayerIdx() );
			if( pANRoom == NULL ) return;	//ACK MSG?

			//---정답 체크
			BOOL bCorrect = pANRoom->CheckAnswerFromAuto( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4 );

			if( bCorrect )	//---정답
			{
				//---답변시간 체크
				DWORD dwAnswerTime = 0;
				if( pANRoom->GetChance() == 3 )
                    dwAnswerTime = pANRoom->GetAnswerTime();
				else
                    dwAnswerTime = pANRoom->GetLastAnswerTime();
				if( dwAnswerTime <= 2000 ) //2초내로 답변을 했다. 보통의 경우엔 불가능
				{
					//---오토에겐 맞추었다고 알리고 안심시킨다.
					//---신고대상에게 답변을 맞추었다고 알림
					MSG_DWORD msg;
					msg.Category = MP_AUTONOTE;
					msg.Protocol = MP_AUTONOTE_ANSWER_ACK;
					msg.dwData = pANRoom->GetAskUserIdx();
					pAutoPlayer->SendMsg( &msg, sizeof(msg) );

					pANRoom->FastAnswer();	//적당한 시간이 후 오토를 끊어버린다. process()함수에서 처리한다.
					return;
				}

				//---신고한 사람에게 대상이 오토가 아님을 알림 (다른 맵으로 갔을 수도 있으니, 모든 에이젼트로 날려준다.)
				MSG_DWORD msg1;
				msg1.Category = MP_AUTONOTE;
				msg1.Protocol = MP_AUTONOTE_NOTAUTO;
				msg1.dwData = pANRoom->GetAskUserIdx();
				g_Network.Broadcast2AgentServer( (char*)&msg1, sizeof(msg1) );

				//---신고대상에게 답변을 맞추었다고 알림
				MSG_DWORD msg2;
				msg2.Category = MP_AUTONOTE;
				msg2.Protocol = MP_AUTONOTE_ANSWER_ACK;
				msg2.dwData = pANRoom->GetAskUserIdx();
				pAutoPlayer->SendMsg( &msg2, sizeof(msg2) );

				//---오토 질답방 해제
				pAutoPlayer->SetAutoAskPlayerIdx( 0 );
				m_htAutoNoteRoom.Remove( pANRoom->GetAskCharacterIdx() );
				m_pmpAutoNoteRoom->Free( pANRoom );
			}
			else	//---오답
			{
				if( pANRoom->GetChance() <= 0 )							//---3번 모두 틀렸다.
				{
					//---대상에게 답변 실패했음을 알림
//					MSG_DWORD msg1;
//					msg1.Category = MP_AUTONOTE;
//					msg1.Protocol = MP_AUTONOTE_ANSWER_FAIL;
//					msg1.dwData	= pANRoom->GetAutoUserIdx();
//					pAutoPlayer->SendMsg( &msg1, sizeof(msg1) );

					MSG_AUTOUSER_BLOCK msg1;
					msg1.Category = MP_AUTONOTE;
					msg1.Protocol = MP_AUTONOTE_ANSWER_FAIL;
					msg1.dwAskUserIdx = pANRoom->GetAskUserIdx();
					msg1.dwAskCharacterIdx = pANRoom->GetAskCharacterIdx();
					SafeStrCpy( msg1.AskCharacterName, pANRoom->GetAskCharacterName(), MAX_NAME_LENGTH+1 );
					msg1.dwAutoUserIdx = pANRoom->GetAutoUserIdx();
                    msg1.dwAutoCharacterIdx = pANRoom->GetAutoCharacterIdx();
					SafeStrCpy( msg1.AutoCharacterName, pANRoom->GetAutoCharacterName(), MAX_NAME_LENGTH+1 );
					pAutoPlayer->SendMsg( &msg1, sizeof(msg1) );

					//---대상 접속 끊기
					MSG_DWORD msg2;
					msg2.Category = MP_AUTONOTE;
					msg2.Protocol = MP_AUTONOTE_DISCONNECT;
					msg2.dwData	= pANRoom->GetAutoUserIdx();
					pAutoPlayer->SendMsg( &msg2, sizeof(msg2) );	//---에이젼트로 보내진다

					//---신고자에게 신고대상이 제재되었음을 알림 (어디에 있을지 모르므로 모든 에이전트로 보냄)
					MSG_DWORD msg3;
					msg3.Category = MP_AUTONOTE;
					msg3.Protocol = MP_AUTONOTE_KILLAUTO;
					msg3.dwData	= pANRoom->GetAskUserIdx();
					g_Network.Broadcast2AgentServer( (char*)&msg3, sizeof(msg3) );

					//---DB 신고자 오토리스트에 등록
					AutoNoteListAdd( pANRoom->GetAskCharacterIdx(), pANRoom->GetAutoUserIdx(), pANRoom->GetAutoCharacterIdx(), pANRoom->GetAutoCharacterName() );

					//---오토 질답방 해제
					pAutoPlayer->SetAutoAskPlayerIdx( 0 );
					m_htAutoNoteRoom.Remove( pANRoom->GetAskCharacterIdx() );
					m_pmpAutoNoteRoom->Free( pANRoom );
				}
				else
				{
					MSG_DWORD msg;
					msg.Category = MP_AUTONOTE;
					msg.Protocol = MP_AUTONOTE_ANSWER_NACK;
					msg.dwData = pANRoom->GetChance();
					pAutoPlayer->SendMsg( &msg, sizeof(msg) );
				}
			}
		}
		break;
	}
}
