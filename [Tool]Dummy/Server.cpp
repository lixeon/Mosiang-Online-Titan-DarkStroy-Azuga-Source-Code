// Server.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ServerSystem.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char *sId, *eId, *sChannel;
	sId = strtok( lpCmdLine, " " );
	eId = strtok( NULL, " " );
	sChannel = strtok( NULL, " " );
	
	if( sId == "" || eId == "" )
		return 0;

	int channel = 0;
	if( sChannel != NULL )
		channel = atoi( sChannel );		

	g_pServerSystem = new CServerSystem;
	if( g_pServerSystem->Start( atoi(sId), atoi(eId), channel ) )
	{
		g_pServerSystem->End();
	}
	delete g_pServerSystem;
	g_pServerSystem = NULL;
	
	return 0;
}
