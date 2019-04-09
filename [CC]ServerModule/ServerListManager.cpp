// ServerListManager.cpp: implementation of the CServerListManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerListManager.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CServerListManager)

CSimpleCodec::CSimpleCodec(char* filename,BOOL bEncoding,char* CodecKey)
{
	m_fp = fopen(filename,bEncoding ? "wb" : "rb");
	m_CurCodecKeyPos = 0;
	m_CodecKeyLen = strlen(CodecKey);
	m_CodecKeybuf = new char[m_CodecKeyLen+1];
	strcpy(m_CodecKeybuf,CodecKey);
}
CSimpleCodec::~CSimpleCodec()
{
	fclose(m_fp);
	delete [] m_CodecKeybuf;
}
BOOL CSimpleCodec::Encode(void* pBufIn,int length)
{
#define MAX_ENCODEBUF_LENGTH	1024
	char tempBuf[MAX_ENCODEBUF_LENGTH];
	ASSERT(length<=MAX_ENCODEBUF_LENGTH);

	memcpy(tempBuf,pBufIn,length);
	for(int n=0;n<length;++n,++m_CurCodecKeyPos)
	{
		if(m_CurCodecKeyPos==m_CodecKeyLen)
			m_CurCodecKeyPos = 0;
		tempBuf[n] += m_CodecKeybuf[m_CurCodecKeyPos];
	}

	int wrotelen = fwrite(tempBuf,length,1,m_fp);
	ASSERT(wrotelen == 1)
	return (wrotelen == 1);
}
BOOL CSimpleCodec::Decode(void* pBufOut,int length)
{
	int readlen = fread(pBufOut,length,1,m_fp);
	if(readlen != 1)
		return FALSE;
	
	char* pchBufOut = (char*)pBufOut;
	for(int n=0;n<length;++n,++m_CurCodecKeyPos)
	{
		if(m_CurCodecKeyPos==m_CodecKeyLen)
			m_CurCodecKeyPos = 0;
		pchBufOut[n] -= m_CodecKeybuf[m_CurCodecKeyPos];
	}

	return TRUE;
}

CServerListManager::CServerListManager()
{
    m_PublicServerInfoList.Initialize( 5 );
}

CServerListManager::~CServerListManager()
{
	Clear();
	PublicClear();
}

void CServerListManager::Clear()
{
	PTRLISTSEARCHSTART(m_ServerInfoList,SERVERINFO*,pInfo)
		delete pInfo;
	PTRLISTSEARCHEND
	m_ServerInfoList.RemoveAll();
}

void CServerListManager::PublicClear()
{
	SERVERINFO* pPublicInfo = NULL;
	m_PublicServerInfoList.SetPositionHead();
	while( pPublicInfo = m_PublicServerInfoList.GetData() )
	{
		delete pPublicInfo;
		pPublicInfo = NULL;
	}
	m_PublicServerInfoList.RemoveAll();
}

BOOL CServerListManager::Add(SERVERINFO* pInfo)
{
	SERVERINFO* pNewServerInfo = new SERVERINFO;
	memcpy( pNewServerInfo, pInfo, sizeof(SERVERINFO) );
	m_ServerInfoList.AddTail(pNewServerInfo);

	return TRUE;
}

BOOL CServerListManager::AddPublicIP(SERVERINFO* pInfo)
{
	SERVERINFO* pNewServerInfo = new SERVERINFO;
	memcpy( pNewServerInfo, pInfo, sizeof(SERVERINFO) );
	m_PublicServerInfoList.Add( pNewServerInfo, pNewServerInfo->wServerNum );

	return TRUE;
}

BOOL CServerListManager::Save(char* pFileName)
{
	BOOL rt;
	CSimpleCodec codec(pFileName,TRUE,"yunhozzang!#&");
	codec.Encode(&m_ServerSetNum,sizeof(m_ServerSetNum));
	PTRLISTSEARCHSTART(m_ServerInfoList,SERVERINFO*,pInfo)
		rt = codec.Encode(pInfo,sizeof(SERVERINFO));
		ASSERT(rt);
	PTRLISTSEARCHEND

	return TRUE;
}

BOOL CServerListManager::Load(char* pFileName)
{
	Clear();
	CSimpleCodec codec(pFileName,FALSE,"yunhozzang!#&");
	codec.Decode(&m_ServerSetNum,sizeof(m_ServerSetNum));
	SERVERINFO info;
	while(codec.Decode(&info,sizeof(SERVERINFO)))
	{
		if(info.wServerKind >= MAX_SERVER_KIND)
		{
			MessageBox(NULL,"",NULL,NULL);
		}
		
		Add(&info);
	}

	/*
	// BUDDY
	sprintf( info.szIPForServer, "211.115.76.248" );
	info.wPortForServer = 3001;
	info.wServerKind = BUDDYAUTH_SERVER;
	Add( &info );
	*/

	return TRUE;
}


BOOL CServerListManager::LoadPublicIP(char* pFileName)
{
	//PublicClear();
	CSimpleCodec codec(pFileName,FALSE,"yunhozzang!#&");
	codec.Decode(&m_ServerSetNum,sizeof(m_ServerSetNum));
	SERVERINFO info;
	while(codec.Decode(&info,sizeof(SERVERINFO)))
	{
		if(info.wServerKind == AGENT_SERVER)		
		{
			AddPublicIP(&info);			
		}		
	}

	return TRUE;
}

SERVERINFO* CServerListManager::GetServerInfoByNum(DWORD ServerKind,WORD ServerNum)
{
	PTRLISTSEARCHSTART(m_ServerInfoList,SERVERINFO*,pInfo)
		if(pInfo->wServerKind != ServerKind)
			continue;
		if(pInfo->wServerNum == ServerNum)
			return pInfo;
	PTRLISTSEARCHEND

	return NULL;
}
SERVERINFO* CServerListManager::GetServerInfoByPort(DWORD ServerKind,WORD Port)
{
	PTRLISTSEARCHSTART(m_ServerInfoList,SERVERINFO*,pInfo)
		if(pInfo->wServerKind != ServerKind)
			continue;
		if(pInfo->wPortForServer == Port)
			return pInfo;
	PTRLISTSEARCHEND

	return NULL;
}
SERVERINFO* CServerListManager::GetServerInfoByIp(DWORD ServerKind,char* ip)
{
	PTRLISTSEARCHSTART(m_ServerInfoList,SERVERINFO*,pInfo)
		if(pInfo->wServerKind != ServerKind)
			continue;
		if(strcmp(pInfo->szIPForServer,ip) == 0)
			return pInfo;
	PTRLISTSEARCHEND

	return NULL;
}

SERVERINFO* CServerListManager::GetPublicServerInfoByPort(DWORD ServerKind,WORD Port)
{	
	SERVERINFO* pInfo = NULL;

	m_PublicServerInfoList.SetPositionHead();	
	while( pInfo = m_PublicServerInfoList.GetData() )
	{
		if(pInfo->wServerKind != ServerKind)
			continue;
		if(pInfo->wPortForUser == Port)
			return pInfo;
	}
	
	return NULL;
}

void CServerListManager::SetPositionHead()
{
	m_Pos = m_ServerInfoList.GetHeadPosition();
}

SERVERINFO* CServerListManager::GetNextServerInfo()
{
	if(m_Pos == NULL)
		return NULL;
	
	return (SERVERINFO*)m_ServerInfoList.GetNext(m_Pos);
}