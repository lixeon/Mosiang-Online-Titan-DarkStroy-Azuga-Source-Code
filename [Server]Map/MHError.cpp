// MHError.cpp: implementation of the CMHError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMHError);

CMHError::CMHError()
{

}

CMHError::~CMHError()
{

}

char * CMHError::GetStringArg(char * buff,...)
{
	static char msg2[512];
	va_list argList;
	va_start(argList, buff);
	vsprintf(msg2, buff, argList);
	va_end(argList);

	return msg2;
}

void CMHError::OutputFile(char * fileName, char * buff, int type)
{
#ifndef _IGNORE_ASSERT_
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/Debug_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
	
	FILE * fp;
	if(type == 0)
	{
		//trucate
		fp = fopen(szFile, "wt");
		if( fp )
		{
			fwrite(buff, strlen(buff), 1, fp);
			fclose(fp);
		}
	}
	else
	{
		// append
		fp = fopen(szFile, "a+t");
		if( fp )
		{ 
			fprintf(fp, "\n%s", buff);
			fclose(fp);
		}
	}
#endif
}
