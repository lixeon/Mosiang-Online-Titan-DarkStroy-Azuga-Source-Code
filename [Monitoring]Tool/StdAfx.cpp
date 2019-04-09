// stdafx.cpp : source file that includes just the standard includes
//	ServerTool.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

char g_szHeroIDName[] = "hi";


char dir[256];
char* GetDirectory()
{
	GetCurrentDirectory( 256, dir );
	return dir;
}
char* GetDataDirectory()
{
	GetCurrentDirectory( 256, dir );
	strcat( dir, "/Data/" );
	return dir;
}

char* GetUserCountDirectory()
{
	GetCurrentDirectory( 256, dir );
	strcat( dir, "/UserCount/" );
	return dir;
}

void LOG( char* msg, ... )
{
	char buff[512];
	
	va_list vl;
	va_start( vl, msg );
	vsprintf( buff, msg, vl );
	va_end( vl );

	SYSTEMTIME time;
	char szTime[256];
	GetLocalTime( &time );
	sprintf( szTime, "%4d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );

	FILE* file = fopen( "Log.txt", "a+" );
	if( file )
	{
		fprintf( file, "%s - %s\n", szTime, buff );
		fclose( file );
	}
}
