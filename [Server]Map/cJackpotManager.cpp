// cJackpotManager.cpp: implementation of the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cJackpotManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "Network.h"
#include "packedData.h"
#include "UserTable.h"
#include "MapDBMsgParser.h"

#include "Battle.h"
#include "Player.h"
#include "Monster.h"
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cJackpotManager)

cJackpotManager::cJackpotManager()
{
	srand((unsigned)time(NULL));
}

cJackpotManager::~cJackpotManager()
{

}

BOOL	cJackpotManager::IsAppMap( WORD MapNum )
{
	//제외되는 맵의 idx 배열과 비교 처리
	//??swich 가 나은가..
//	WORD wExcpMap[EXCP_MAP_COUNT] =
//	{
//		28,		//길드 토너먼트
//		44,		//연무장
//		59,		//진시황릉 중앙
//		5,		//낙양공성
//		62,		//장안공성
//		16,		//개봉공성
//		24,		//북경공성
//	};
	
	WORD wExcpMap[EXCP_MAP_COUNT] =
	{
		6,		//낙양
		7,		//개봉
		12,		//장안
//		17,		//난주
		28,		//토너먼트_경기장
		44,		//연무장
		58,		//결투장
		72,		//수호장군의 방
		73,		//기연의 방
	};
	
	for(int i = 0; i < EXCP_MAP_COUNT; ++i)
	{
		if(wExcpMap[i] == MapNum)
		{
//			m_bIsAppMap = FALSE;
//			return FALSE;
			return m_bIsAppMap = FALSE;
		}
	}
	return m_bIsAppMap = TRUE;
}

BOOL	cJackpotManager::IsAppMon( CPlayer* pPlayer, CMonster* pMonster )
{
	//렙차가 6이상이고 이벤트 몹이 아니면(?이벤트몹은 비경험치)
	if( pPlayer->GetLevel() - pMonster->GetLevel() > 5 || !(pMonster->GetSMonsterList()->ExpPoint) )
		return FALSE;
	//난주_연무장만을 위한 예외처리
	if( pPlayer->GetBattle() != NULL )
	{
		if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN ) return FALSE;

		else return TRUE;
	}
	else
		return TRUE;
}

BOOL	cJackpotManager::IsUserIn()
{
	if( g_pUserTable->GetUserCount() ) 
		return TRUE;
	
	return FALSE;
}

BOOL	cJackpotManager::LoadJackpotInfo()
{
	CMHFile fp;
	char szBuf[256] = {0,};

	char filename[256];
#ifdef _FILE_BIN_
//	sprintf(filename,"Resource/Jackpot_Info.bin");
	sprintf(filename,"Resource/Server/Jackpot_Info.bin");
	if( !fp.Init( filename, "rb" ) )
		return FALSE;
#else
	sprintf(filename,"Resource/Jackpot_Info.txt");
	if( !fp.Init( filename, "rt" ) )
		return FALSE;
#endif

	while(TRUE)
	{
		if(fp.IsEOF())
			break;

		strcpy(szBuf,strupr(fp.GetString()));

		if(strcmp(szBuf,"#CHIPSPERMON") == 0)
		{
			m_dwChipPerMon = fp.GetDword();
		}
		else if(strcmp(szBuf,"#UPDATETIMELEN") == 0)
		{
			m_dwUpDateTimeLen = fp.GetDword() * 1000;
		}
		else if(strcmp(szBuf,"#GRADE") ==0)
		{
			fp.GetLine(szBuf,256);
			for( int i = 0; i < NUM_PRIZE; ++i )
			{
				char* cDum = fp.GetString();	// kindname
				DWORD dwAtom = fp.GetDword();	 
				cDum = fp.GetString();	// '/'
				DWORD dwDenomi = fp.GetDword();
				ASSERT(dwDenomi != 0);
				m_stPrize[i].m_dwProbability = SetProbToDword(&dwAtom, &dwDenomi);
				m_stPrize[i].m_dwPercent = fp.GetDword();
				//SW050905 4등 추가 작업 (상금 절대치)
				//DB 프로시져 내에서 AbsMoney가 최우선으로 되어있다. 0이면 확률값으로 계산.
				m_stPrize[i].AbsMoney = fp.GetDword();
			}
			break;
		}
	}
	fp.Release();
	return TRUE;
}

void	cJackpotManager::Process()
{
	SendMsgDBMapMoney();
}

DWORD	cJackpotManager::SetProbToDword( DWORD* atom, DWORD* denomi )
{
//	return DWORD(MAX_PROBABILITY_LIMIT / *denomi * *atom );
	return DWORD( *atom  * MAX_PROBABILITY_LIMIT / *denomi );
}

DWORD	cJackpotManager::MakeRndValue()
{
//	DWORD dwBig = rand()%10000;	//0 ~ 9999 //0, 0~99990000
//	DWORD dwLit = rand()%10000 + 1;	//1 ~ 10000
//
//	return (dwBig * 10000 + dwLit);

	return (rand()%10000) * 10000 + (rand()%10000 + 1);
}

int		cJackpotManager::CheckPrize( int kind )
{
	if( CalcAddProb(kind) >= MakeRndValue() )
	{
		return kind;
	}
	else
		return NO_PRIZE;
}

int		cJackpotManager::CheckPrize()
{
	DWORD Rnd = MakeRndValue();

	
	int kind = NO_PRIZE;
/*
	if( Rnd > CalcAddProb(kind) )
		return NO_PRIZE;
	// 확률 높은 3등 부터 비교한다.	// 약간이라도 루프 줄이기위해..
	while( Rnd <= CalcAddProb(kind) )
	{
		kind--;
	}

	return ++kind;*/
	if( Rnd > CalcAddProb(FTH_PRIZE)) return kind;

	for( int i = 0; i < NUM_PRIZE; ++ i )
	{
		if( Rnd <= CalcAddProb(i) )
			return i;
	}
	/*
	if( Rnd <= CalcAddProb(FST_PRIZE) )
	{
		kind = FST_PRIZE;
	}
	else if( Rnd <= CalcAddProb(SND_PRIZE) )
	{
		kind = SND_PRIZE;
	}
	else if( Rnd <= CalcAddProb(THR_PRIZE) )
	{
		kind = THR_PRIZE;
	}
	else if( Rnd <= CalcAddProb(FTH_PRIZE) )
	{
		kind = FTH_PRIZE;
	}*/
	return kind;
}

void	cJackpotManager::DoJackpot(CPlayer* pPlayer, CMonster* pMonster)
{
//	if(!GetIsAppMap())	return;
	if(!m_bIsAppMap)	return;

//	m_pPlayer = pPlayer;
//	m_pMonster = pMonster;

	if(!IsAppMon( pPlayer, pMonster ))	return;

	m_dwMapJPMoney += m_dwChipPerMon;

	//SW050803 for cheat
	if(!m_bDoPrize) return;

	m_nPrizeKind = CheckPrize();

	if(m_nPrizeKind != NO_PRIZE)	//if(m_nPrizeKind > NO_PRIZE)
	{
		ASSERT(m_nPrizeKind<NUM_PRIZE);
		JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), m_nPrizeKind, m_stPrize[m_nPrizeKind].m_dwPercent, m_stPrize[m_nPrizeKind].AbsMoney );
		m_dwMapJPMoney = 0;
	}
	else m_dwPercentage = 0;
}

void	cJackpotManager::SendMsgDBMapMoney()
{//SendMapMoneyInfo to DB regularly
	if( gCurTime - m_dwLastDBUpdateTime < m_dwUpDateTimeLen )
		return;
	// 050726 정팀장님 지시
	// 유저가 없을 시엔 보내지 말것	
	if(!IsUserIn())
	{
		m_dwLastDBUpdateTime = gCurTime;
		return;
	}
	else
	{
		//DB로 맵머니 보내고 (리턴으로 총액 받아온다.RJackpotAddTotalMoney())
		JackpotAddTotalMoney( m_dwMapJPMoney );
		m_dwMapJPMoney = 0;
		m_dwLastDBUpdateTime = gCurTime;
	}
}

//SW050802 for Cheat
void	cJackpotManager::SendMsgDBPrizeInfo( CPlayer* pPlayer, int PrizeKind )
{
	JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), PrizeKind, m_stPrize[PrizeKind].m_dwPercent, m_stPrize[PrizeKind].AbsMoney );
	m_dwMapJPMoney = 0;
}

void	cJackpotManager::SendMsgAllUserPrizeNotify( DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind )
{
	// SendMsg To AllUser
	// Msg description : PlayerId, CharName, PrizeMoney, RestMoney

//	SetTotalMoney(TotalMoney);
//	m_dwPrizeMoney = PrizeMoney;	//!전광판 출력용(예정)
	
	MSG_JACKPOT_PRIZE_NOTIFY msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_PRIZE_NOTIFY);
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	ASSERT(pPlayer);
	if(!pPlayer) return;	//!당첨금 다시 DB로 보내줄까...
	SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
//	msg.dwPrizeUserID = m_pPlayer->GetID();	// 해당 캐릭은 이펙트 렌더	// 연이은 잿팟의 경우를 생각해 mPlayer 사용 못함
	msg.dwPrizeUserID = PlayerID;
	ASSERT(PrizeKind < 3);
	msg.nPrizeKind = PrizeKind;
	msg.dwPrizeMoney = PrizeMoney;
	msg.dwRestTotalMoney = TotalMoney;

	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );

	// 정팀장님 지시 050725
	// merge SendMsgPrizeEffect() 
	//PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(MSG_JACKPOT_PRIZE_NOTIFY));	
}

/*
void	cJackpotManager::SendMsgMapUserTotalMoney( DWORD TotalMoney )
{
	//Send Msg To MapConnectingUsers
	// Time Control is up to RJackpotAddTotalMoney()

	SetTotalMoney(TotalMoney);

	MSG_DWORD msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);
	msg.dwData = m_dwTotalJPMoney;
	
	g_pUserTable->SetPositionUserHead();
	while(CObject* pObject = g_pUserTable->GetData())
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		CPlayer* pPlayer = (CPlayer*) pObject;
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}

}*/

/*
void	cJackpotManager::SendMsgAddUserTotalMoney( CObject* pObject )
{	// Send TotalMoney Msg To AddUser
	MSG_DWORD msg;
	SetProtocol((MSGBASE*)&msg, MP_JACKPOT, MP_JACKPOT_TOTALMONEY_NOTIFY);
	msg.dwData = m_dwTotalJPMoney;
	
	CPlayer* pPlayer = (CPlayer*) pObject;
	pPlayer->SendMsg( &msg, sizeof(msg) );
}*/

/*
void	cJackpotManager::SendMsgPrizeEffect(DWORD PlayerID)
{
	MSG_DWORD msg;
	SetProtocol(&msg, MP_JACKPOT, MP_JACKPOT_PRIZE_EFFECT);
	msg.dwObjectID = PlayerID;
	msg.dwData = PlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	if(!pPlayer) return;

	PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(MSG_DWORD));	
}*/

void	cJackpotManager::SetPrizeInfo( DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind )
{
	//해당 ID의 창고로 MONEY 등록
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( PlayerID );
	if(!pPlayer) return;
	pPlayer->SetMoney( PrizeMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetPrize, 0 );
	//전 서버에 공지
	SendMsgAllUserPrizeNotify( TotalMoney, PrizeMoney, PlayerID, PrizeKind );
	// 이펙트 렌더(In Grid)
	// SW050808 공지에서 처리
//	SendMsgPrizeEffect(PlayerID);	
}

DWORD	cJackpotManager::CalcAddProb(int kind)
{//몹레벨 비례 확률 추가
//	return m_stPrize[kind].m_dwProbability * (1 + (m_pMonster->GetLevel())/10 );
	// 050728 동훈씨 요청
	return m_stPrize[kind].m_dwProbability;
}

void	cJackpotManager::Init( WORD Mapnum )
{
	m_dwTotalJPMoney	= 0;	//서버 총적립금 Fr_DB
	m_dwMapJPMoney		= 0;	//맵 누적금
	m_dwPrizeMoney		= 0;	//당첨 금액(Fr_DB)
	m_dwPercentage		= 0;

	m_pPlayer			= NULL;
	m_pMonster			= NULL;

	m_nPrizeKind		= NO_PRIZE;	//당첨 종류(NO_PRIZE = -1)
	
	m_dwChipPerMon		= 0;	//몹당 적립금(Fr_Script) //-적립가능
	m_dwUpDateTimeLen	= 0;	//적립 금액 Update 시간 간격(Fr_Script)
	m_dwLastDBUpdateTime	= 0;	//이전 Update 시각
	
//	m_bActive			= TRUE;		//JPMgr active
	m_bDoPrize			= TRUE;		//Prize active
	m_bIsAppMon			= TRUE;	//적용 몹인지
	m_bIsAppMap			= IsAppMap(Mapnum);

	// DB Query
//	JackpotLoadTotalMoney();
}
