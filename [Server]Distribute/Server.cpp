// Server.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ServerSystem.h"
#include "MHFile.h"
#ifdef _MAPSERVER_
#include "GameResourceManager.h"
#endif

HANDLE g_hMap = NULL;
char g_szMapName[64];
void LeaveServerApplication();
BOOL EnterServerApplication();
char * GetCommandLineArg(int idx, char * szCommandLine);
char g_szHeroIDName[17] = { 0, };

BOOL CheckUpdateFile();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CreateDirectory( "./Log", NULL );

	SEND_CHARACTER_TOTALINFO ss;
	SEND_MONSTER_TOTALINFO mm;

	WORD mapnum = atoi(lpCmdLine);
#ifdef _MAPSERVER_
	#ifndef _MURIMNET_

	GAMERESRCMNGR->SetLoadMapNum(mapnum);
	if(mapnum == 0)
	{
		MessageBox(NULL,"Give Parameter as a Map Number",NULL,NULL);
		return 0;
	}
	wsprintf(g_szMapName, "MapServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
	#endif
#elif _DISTRIBUTESERVER_
	wsprintf(g_szMapName, "DistributeServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
#elif _AGENTSERVER
	wsprintf(g_szMapName, "AgentServer_%d", mapnum); 
	if(!EnterServerApplication()) return 0;
#endif

	if( CheckUpdateFile() == FALSE )
		return 0;

	g_pServerSystem = new CServerSystem;
	g_pServerSystem->Start(mapnum);
	g_pServerSystem->End();
	delete g_pServerSystem;
	g_pServerSystem = NULL;
	
	LeaveServerApplication();

	return 0;
}

char * GetCommandLineArg(int idx, char * szCommandLine)
{
	char sep[]   = " ";
	char line[255];
//	strcpy(line, szCommandLine);
	SafeStrCpy( line, szCommandLine, 255 );
	char * token = strtok( line, sep );
	while( token != NULL && idx != 0)
	{
		token = strtok( NULL, sep );
		--idx;
	}
	return token;
}

BOOL EnterServerApplication()
{//taiyo
    g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, g_szMapName);
    if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
    { 
		CloseHandle(g_hMap); 
		g_hMap = NULL;
		return FALSE;
    }
	
	return TRUE;
}

void LeaveServerApplication()
{//taiyo
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}

BOOL CheckUpdateFile()
{
	CMHFile file;
	char temp[256] = {0,};

	if( !file.Init( "./Resource/Server/TitanServer.bin", "rb" ) )
		return FALSE;

	file.GetStringInQuotation( temp );

	if( strcmp( temp, "이 파일이 없으면 타이탄 업데이트 안돼요~" ) != 0 )
		return FALSE;

	file.Release();

	return TRUE;
}
