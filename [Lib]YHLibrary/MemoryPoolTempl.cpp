#include "stdafx.h"
#include <stdio.h>
#include "MemoryPoolTempl.h"



void OutputFile(char* str)
{
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );
	sprintf( szFile, "./Log/MemoryPoolTempl_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );

	FILE* fp = fopen(szFile,"a+");
	if(!fp)		return;	


	fprintf(fp, str);

	fclose(fp);	
}