

#include "stdafx.h"
#include "BuddyAuth.h"
#include "DistributeDBMsgParser.h"
#include "ServerTable.h"


CBuddyAuth g_BuddyAuth;
extern int g_nServerSetNum;
extern DWORD g_dwMaxUser;
extern int g_nServerSetNum;


CBuddyAuth::CBuddyAuth()
{
	memset( m_BuddyAuthInfoTable, 0, sizeof(stBUDDYAUTHINFO)*MAX_BUDDYAUTH_NUM );
	m_ConnectTryNum = 0;
}

CBuddyAuth::~CBuddyAuth()
{
	for( int i=0; i<MAX_BUDDYAUTH_NUM; ++i )
	{
		closesocket(m_BuddyAuthInfoTable[i].conn_socket);
		m_BuddyAuthInfoTable[i].conn_socket = 0;
	}
}


BOOL CBuddyAuth::ConnectToBuddyAuthServerAll()
{
	WORD Version;
	WSADATA wsaData;

	WSACleanup();
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);

	for( int i=0; i<MAX_BUDDYAUTH_NUM; ++i )
	{
		if( m_BuddyAuthInfoTable[i].conn_socket )
		{
			closesocket(m_BuddyAuthInfoTable[i].conn_socket);
			m_BuddyAuthInfoTable[i].conn_socket = 0;
		}

		sockaddr_in addr;
		//DWORD	dwIndex;

		m_BuddyAuthInfoTable[i].conn_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(m_BuddyAuthInfoTable[i].conn_socket == INVALID_SOCKET )
			return FALSE;

		addr.sin_family = AF_INET;
		addr.sin_port   = htons(3001);
		addr.sin_addr.s_addr = inet_addr("1.2.3.4");//211.115.76.247

		if(connect(m_BuddyAuthInfoTable[i].conn_socket, (LPSOCKADDR)&addr, sizeof(addr)) < 0)
		{
			closesocket(m_BuddyAuthInfoTable[i].conn_socket);
			m_BuddyAuthInfoTable[i].conn_socket = 0;
			continue; 
		}
	}

	return TRUE;
}


BOOL CBuddyAuth::ResetBuddyAuthInfo( stBUDDYAUTHINFO* pAuthInfo, BOOL bOnlySocket )
{
	if( !bOnlySocket )
	{
		pAuthInfo->dwConnectIdx = 0;
		pAuthInfo->dwAuthKey = 0;
		memset( pAuthInfo->szAuthCode, 0, AUTHCODE_LENGTH );
		memset( pAuthInfo->szIP, 0, IP_LENGTH );
	}

	if( pAuthInfo->conn_socket )
	{
		closesocket( pAuthInfo->conn_socket );
		pAuthInfo->conn_socket = 0;
	}

	WORD Version;
	WSADATA wsaData;

	WSACleanup();
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);


	sockaddr_in addr;
	pAuthInfo->conn_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if( pAuthInfo->conn_socket == INVALID_SOCKET )
		return FALSE;

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(3001);
	addr.sin_addr.s_addr = inet_addr("1.2.3.4");

	if(connect(pAuthInfo->conn_socket, (LPSOCKADDR)&addr, sizeof(addr)) < 0)
	{
		closesocket(pAuthInfo->conn_socket);
		pAuthInfo->conn_socket = 0;
		return FALSE;
	}

	return TRUE;
}


void CBuddyAuth::RevMsgProcess()
{
	if( m_ConnectTryNum <= 0 )
		return;

	int res = 0;
	int command = 0;
	char revcbuf[512] = { 0, };

	for(int i=0; i< MAX_BUDDYAUTH_NUM; ++i )
	{
		if( !m_BuddyAuthInfoTable[i].dwConnectIdx )
			continue;

		res = recv( m_BuddyAuthInfoTable[i].conn_socket, revcbuf, 512, 0 );
		if( res != SOCKET_ERROR )
		{
			command = GetResultCommand<int>( revcbuf, COMMAND_DISTANCE, sizeof(int) );
			if( command == 0x02002001 )
				SendIDandAuthCode( &m_BuddyAuthInfoTable[i], revcbuf );
			else if( command == 0x02002004 )
				SendAuthResulttoUser( &m_BuddyAuthInfoTable[i], revcbuf );
		}
		else
		{
		}	
	}
}



BOOL CBuddyAuth::SendAuthInfotoBuddy( DWORD dwConnectIdx, DWORD dwAuthKey, char* szID, char* szPassword, char* szIP, char* szAuthCode )
{
	stBUDDYAUTHINFO* pAuthInfo = GetBlankAuthInfo();
	if( !pAuthInfo )
	{
		LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_BUDDY_IDAUTHFAILED, 101 );
		DisconnectUser( pAuthInfo->dwConnectIdx );
		ResetBuddyAuthInfo( pAuthInfo );
		return FALSE;		
	}

	int size = 4;
	int bodysize = 0;
	char msg[128] = { 0, };


	// Command
	int command = 0x02001001;
	AttendSendMsg( msg, &command, sizeof(command), size, bodysize );

	// APPTYPE
	byte apptype = 0;
	AttendSendMsg( msg, &apptype, sizeof(apptype), size, bodysize );
	char buf[1] = { 0, };
	AttendSendMsg( msg, buf, 1, size, bodysize );

	// IDSIZE
	byte idsize = 4;
	AttendSendMsg( msg, &idsize, sizeof(idsize), size, bodysize );

	// ID
	char* id = "dark";
	//AttendSendMsg( msg, szID, strlen(szID)+1, size, bodysize );
	AttendSendMsg( msg, id, strlen(id)+1, size, bodysize );

	// PASSSIZE
	byte passsize = 6;
	AttendSendMsg( msg, &passsize, sizeof(passsize), size, bodysize );

	// PASSWORD
	char* pass = "ekzm!@";
	//AttendSendMsg( msg, szPassword, strlen(szPassword)+1, size, bodysize );
	AttendSendMsg( msg, pass, strlen(pass)+1, size, bodysize );

	// BODYSIZE
	memcpy( &msg[0], &bodysize, 4 );


	// SEND
	int res = send( pAuthInfo->conn_socket, msg, size, 0 );
	if( res == SOCKET_ERROR )
	{
		LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_BUDDY_IDAUTHFAILED, 100 );
		DisconnectUser( pAuthInfo->dwConnectIdx );
		ResetBuddyAuthInfo( pAuthInfo );
		return FALSE;
	}

	pAuthInfo->dwConnectIdx = dwConnectIdx;
	pAuthInfo->dwAuthKey = dwAuthKey;
	SafeStrCpy( pAuthInfo->szID, szID, MAX_NAME_LENGTH+1 );
	SafeStrCpy( pAuthInfo->szIP, szIP, strlen(szIP)+1 );
	SafeStrCpy( pAuthInfo->szAuthCode, szAuthCode, strlen(szAuthCode)+1 );
	++m_ConnectTryNum;

	return TRUE;
}


void CBuddyAuth::SendIDandAuthCode( stBUDDYAUTHINFO* pAuthInfo, char* szData )
{
	byte res = GetResultCommand<byte>( szData, LOGINRESULT_DISTANCE, sizeof(byte) );

	if( res == eBuddyLoginRes_Success )
	{
		int size = 4;
		int bodysize = 0;
		char msg[256] = { 0, };

		// COMMAND
		int command = 0x02001004;
		AttendSendMsg( msg, &command, sizeof(command), size, bodysize );

		// VERSION
		int version = 3;
		AttendSendMsg( msg, &version, sizeof(version), size, bodysize );

		// PPV
		int ppv = 1;
		AttendSendMsg( msg, &ppv, sizeof(ppv), size, bodysize );

		// AUTHCODE
		WORD authcode = strlen( pAuthInfo->szAuthCode );
		AttendSendMsg( msg, &authcode, sizeof(authcode), size, bodysize );
		AttendSendMsg( msg, pAuthInfo->szAuthCode, strlen(pAuthInfo->szAuthCode)+1, size, bodysize );

		// IP
		unsigned long ip = 0;
		//ConvertIPtoUINT( pAuthInfo->szIP, ip );
		ip = inet_addr( pAuthInfo->szIP );
		AttendSendMsg( msg, &ip, sizeof(ip), size, bodysize );

		// BODYSIZE
		memcpy( &msg[0], &bodysize, 4 );

		int res = 0;
		res = send( pAuthInfo->conn_socket, msg, size, 0 );
		if( res != SOCKET_ERROR )
		{
			return;
		}
	}

	LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_BUDDY_IDAUTHFAILED, res );
	DisconnectUser( pAuthInfo->dwConnectIdx );
	ResetBuddyAuthInfo( pAuthInfo );
	--m_ConnectTryNum;
}


void CBuddyAuth::SendAuthResulttoUser( stBUDDYAUTHINFO* pAuthInfo, char* szData )
{
	byte res = GetResultCommand<byte>( szData, LOGINRESULT_DISTANCE, sizeof(byte) );

	if( res == eBuddyLoginRes_Success )
	{
		// 실명인증확인
		// ID (로그인결과까지 + 로그인결과 +  PPV )
		int dist = LOGINRESULT_DISTANCE+1+4;
		byte idlen = GetResultCommand<byte>( szData, dist, sizeof(byte) );
		dist += idlen+2;

		// 실명
		byte realnamelen = GetResultCommand<byte>( szData, dist, sizeof(byte) );
		dist += realnamelen+2;

		// 주민번호
		byte juminlen = GetResultCommand<byte>( szData, dist, sizeof(byte) );
		dist += juminlen+2;

		// 성별, 지역
		dist += 1+1;

		// 실명인증여부
		byte nameauth = GetResultCommand<byte>( szData, dist, sizeof(byte) );
		if( g_nServerSetNum != 4 && nameauth != 89 )  // 'Y'
		{
			LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_BUDDY_AUTHCODEFAILED, 99 );
			DisconnectUser( pAuthInfo->dwConnectIdx );
			ResetBuddyAuthInfo( pAuthInfo );
			--m_ConnectTryNum;
			return;
		}

		SERVERINFO* FastInfo = g_pServerTable->GetFastServer(AGENT_SERVER);
		if(FastInfo == NULL)
		{
			LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_NOAGENTSERVER );
			DisconnectUser( pAuthInfo->dwConnectIdx );
			g_Console.Log(eLogDisplay,4, "Can't Find Agent Server for Buddy" );
			return;
		}

		// LogIn프로시져호출
		char name[32] = { 0, };
		char pass[32] = "ekzmdnsdud";
		sprintf( name, "b_%s", pAuthInfo->szID );
		LoginCheckQuery( name, pass, pAuthInfo->szIP, g_nServerSetNum, pAuthInfo->dwAuthKey, pAuthInfo->dwConnectIdx, 		
			g_dwMaxUser, FastInfo->wServerNum );

		ResetBuddyAuthInfo( pAuthInfo );
	}
	else
	{
		LoginError( pAuthInfo->dwConnectIdx, pAuthInfo->dwAuthKey, LOGIN_ERROR_BUDDY_AUTHCODEFAILED, res );
		DisconnectUser( pAuthInfo->dwConnectIdx );
		ResetBuddyAuthInfo( pAuthInfo );
		--m_ConnectTryNum;
	}
}



stBUDDYAUTHINFO* CBuddyAuth::GetBlankAuthInfo()
{
	for(int i=0; i< MAX_BUDDYAUTH_NUM; ++i )
	{
		if( !m_BuddyAuthInfoTable[i].dwConnectIdx )
			return &m_BuddyAuthInfoTable[i];
	}

	return NULL;
}


void CBuddyAuth::AttendSendMsg( char* szMsg, void* pData, int nLength, int& nSize, int& nBodySize )
{
	if( !szMsg || !pData )			return;

	memcpy( &szMsg[nSize], pData, nLength );
	nSize += nLength;
	nBodySize += nLength;
}


void CBuddyAuth::ConvertIPtoUINT( char* szIP, unsigned int& nRV )
{
	int len = strlen( szIP );
	char dznum[3];
	char numct = 0;
	unsigned int nip[4];
	int ipct = 0;


	for( int i=0; i<len; ++i )
	{
		if( szIP[i] == '.' )
		{
			nip[ipct] = (unsigned int)atoi( dznum );
			++ipct;
			numct = 0;
		}
		else
		{
			dznum[numct] = szIP[i];
			++numct;
		}
	}
	nip[ipct] = (unsigned int)atoi( dznum );

	nRV = ( nRV | nip[0]<<24 );
	nRV = ( nRV | nip[1]<<16 );
	nRV = ( nRV | nip[2]<<8 );
	nRV = ( nRV | nip[3] );
}