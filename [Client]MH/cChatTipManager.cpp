// cChatTipManager.cpp: implementation of the cChatTipManager class.
//
//////////////////////////////////////////////////////////////////////

//#include <windows.h>	//*** GetTickCount	//콘솔 테스트하고 붙임.
//#include "stdio.h"
#include <StdAfx.h>
#include "MHFile.h"
#include "MHTimeManager.h"	//
#include "CommonDefine.h"
#include "OptionManager.h"

#include "./Interface/cResourceDef.h"	//리소스 경로

#include "ChatManager.h"
#include "ObjectManager.h"
#include "cChatTipManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cChatTipManager)

cChatTipManager::cChatTipManager()
{
//	Init();			//GameIn 
}

cChatTipManager::~cChatTipManager()
{
	Release();
}

void cChatTipManager::Init()
{
	m_bActive			= TRUE;
	m_dwSize			= 0;	//로딩 구조체 크기	//실시간
	m_dwMsgTimeLen		= 60000;
	m_dwLastMsgTime		= 0;	//먼저 메시지 출력 시각
	m_dwCurHeroLevel	= 0;	//IsInfoCharged()에서 초기 TRUE 반환//HEROINFO 구조체 대체
	m_nCurIndex		= -1;	//다음 메시지 전환을 위한 현재 인덱스 기억//초기값 -1
	m_spChatTipMsg		= NULL;

	LoadChatTipMsg();
}

void cChatTipManager::Release()
{
	ReleaseMsgList();
}

void cChatTipManager::LoadChatTipMsg()	// 스크립트 예외 처리 안됨. 
{
	char line[256] = {0,};

	CMHFile fp;
	int nIndex = 0;;
	int nLen = 0;

#ifdef _FILE_BIN_
	if( !fp.Init( FILE_CHATTIP_MSG, "rb" ) )
		return;
#else
	if( !fp.Init( FILE_CHATTIP_MSG, "rt" ) )
		return;
#endif
	strcpy( line, strupr( fp.GetString() ) );
	if( strcmp( line, "#LISTCOUNT" ) == 0 )
	{
		m_dwSize = fp.GetDword();
		m_spChatTipMsg = new sChatTip[ m_dwSize ];
	}
	fp.GetLine( line, 256 );

	strcpy( line, strupr( fp.GetString() ) );
	if( strcmp( line, "#MSGTIMELENGTH") == 0 )
	{
		m_dwMsgTimeLen = fp.GetDword();
	}
	fp.GetLine( line, 256 );
//	m_dwSize = fp.GetInt();
//	m_spChatTipMsg = new sChatTip[ m_dwSize ];
//	fp.GetLine( line, 256 );
//	m_dwMsgTimeLen = fp.GetInt();
//	fp.GetLine( line, 256 );

	while( TRUE )
	{
		nIndex = fp.GetInt();
		
		if( fp.IsEOF() ) break;

		if( nIndex < 0 || nIndex >= (int)m_dwSize ) break;

		m_spChatTipMsg[nIndex].FromLv	= fp.GetInt();
		m_spChatTipMsg[nIndex].ToLv		= fp.GetInt();

		fp.GetStringInQuotation( line );
		nLen = strlen( line );
		if( nLen > 0 )
		{
			m_spChatTipMsg[nIndex].pTipStr = new char[ nLen + 1 ];
			memcpy( m_spChatTipMsg[nIndex].pTipStr, line, nLen + 1 );
			fp.GetLine( line, 256 );
		}
	}

	fp.Release();
}

//char* cChatTipManager::GetStringInQuotation(char *buf)
//{
//	//" 표안 문자열 등록
//
//	static char temp[256];
//	char str[256] = {0,};
//	int i = 0;
//
//	while(buf[i] != '"' )
//	{
//		i++;
//	}
//
//	i++;
//	
//	int n = 0;
//	while(buf[i] != '"' )
//	{
//		str[n] = buf[i];
//		n++;
//		i++;
//	}
//
//	str[n] = 0;
//
//	memcpy( temp, str, (n+1) );
//
//  	return temp;
//
//}

char* cChatTipManager::GetChatTipMsg()
{
#ifdef _TESTCLIENT_
	m_dwCurHeroLevel = 25;
#else
	m_dwCurHeroLevel = HERO->GetLevel();
#endif

//	!!캐릭 정보는 변경할 때 한번만....매번x
	//시간 유효 검사하고

		int i = m_nCurIndex;
		
		bool bIsFirst = FALSE;	//6자 루프의 한계...
		bool bLooped = FALSE;	//한바퀴 돌았는지
		
		if( m_nCurIndex == -1 )
		{
			bIsFirst = TRUE;
		}
		
		//해당 메시지 없으면 꺼주기
		
		while(m_bActive)
		{
			if( ( i == m_nCurIndex || bIsFirst == TRUE ) && bLooped == TRUE )	//한바퀴 다시 돌동안 해당 사항 없으면
			{
				m_bActive = FALSE;
				break;
			}
			
			i++;
			
			if( i == (int)m_dwSize)//끝이면 처음으로
			{
				i = 0;
			}
			
			if( i == (int)m_dwSize - 1 )
			{
				bLooped = TRUE;
			}
			
			
			//캐릭 정보 가져와서 비교하고
//			if( m_dwCurHeroLevel >= (m_spChatTipMsg+i)->FromLv 
//				&& m_dwCurHeroLevel <= (m_spChatTipMsg+i)->ToLv )
			if( m_dwCurHeroLevel >= m_spChatTipMsg[i].FromLv
				&& m_dwCurHeroLevel <= m_spChatTipMsg[i].ToLv )
			{
				m_nCurIndex = i;	// 현재 인덱스로 등록
				break;
			}
		}
		
		if(m_bActive)
		{
			return m_spChatTipMsg[m_nCurIndex].pTipStr;
		}
		
// 	}
	return NULL;
}

bool cChatTipManager::IsTimePassed()	//출력할 시간이 되었는지
{
//	if( gCurTime - m_dwLastMsgTime > CHATTIPMSG_TIMELEN )
	//SW050804 시간 간격 스크립트에 추가
	if( gCurTime - m_dwLastMsgTime > m_dwMsgTimeLen )
	{
		m_dwLastMsgTime = gCurTime;
		return TRUE;
	}
	else
		return FALSE;
}

void cChatTipManager::ReleaseMsgList()
{
	if( m_spChatTipMsg != NULL )
	{
		for( int i = 0; i < (int)m_dwSize; i++ )
		{
			if( m_spChatTipMsg[i].pTipStr )
			{
				delete[] m_spChatTipMsg[i].pTipStr;
				m_spChatTipMsg[i].pTipStr = NULL;
			}
		}
		
		if(m_spChatTipMsg)
		{
			delete[] m_spChatTipMsg;
			m_spChatTipMsg = NULL;
		}
	}
}

void cChatTipManager::Process()
{
	if( !m_bActive || m_spChatTipMsg == NULL )
	{
		return;
	}

	if( !IsTimePassed() )
	{
		return;
	}

	char* str = GetChatTipMsg();
	if( str != NULL)
	{
		CHATMGR->AddMsg( CTC_TIPS, str );
	}
}
