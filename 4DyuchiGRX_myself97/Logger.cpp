#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

int LogInit(void)
{
/*
    FILE *LogFile;

    LogFile = fopen("c:/Log.txt", "w");

    fclose( LogFile);

*/
    return 1;
}

int Log(char* words)
{
#ifdef	LogEnable
	OutputDebugString( words);
/*
    int StringLength = strlen(words);
    FILE *LogFile;

	time_t lTime;
	time(&lTime);
	struct tm* LogTime = localtime(&lTime);

    LogFile = fopen("c:/Log.txt", "a+");
	fprintf(LogFile, "[%4d-%2d-%2d %2d:%2d:%2d] ",
			1900+LogTime->tm_year, (LogTime->tm_mon)+1, LogTime->tm_mday,
			LogTime->tm_hour, LogTime->tm_min, LogTime->tm_sec);
    fwrite( words, StringLength, 1, LogFile);

    fprintf(LogFile, "\n");

    fclose( LogFile);
*/
#endif
    return true;
}

int LogPrintf( char* szFormat, ...)
{
#ifdef	LogEnable
	va_list argptr;
	char cBuf[1024];


	va_start( argptr, szFormat );
	vsprintf( cBuf, szFormat, argptr );
	va_end( argptr );

	Log( cBuf);
#endif
	return 0;
}

