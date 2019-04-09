// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMHFile::CMHFile()
{
	fp = NULL;
}

CMHFile::~CMHFile()
{
	Release();
}


BOOL CMHFile::Init(char* filename,char* mode, DWORD dwFlag)
{
	fp = fopen(filename,mode);
	if(fp)
		return TRUE;
	else
	{
		if(!(dwFlag & MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
		{			
			char buffer[255];
			sprintf(buffer, "[%s] file is not found",filename);
			MessageBox(NULL, buffer, 0,0);
		}		
		return FALSE;
	}
}

void CMHFile::Release()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
}

char* CMHFile::GetString()
{
	static char buf[256];
	fscanf(fp,"%s",buf);
	return buf;
}
void CMHFile::GetString(char* pBuf)
{
	fscanf(fp,"%s",pBuf);
}
void CMHFile::GetLine(char * pBuf, int len)
{
	fgets(pBuf, len, fp);
}
int CMHFile::GetInt()
{
	return atoi(GetString());
}
LONG CMHFile::GetLong()
{
	return (LONG)atoi(GetString());
}
float CMHFile::GetFloat()
{
	return (float)atof(GetString());
}
DWORD CMHFile::GetDword()
{
	return (DWORD)atoi(GetString());
}
WORD CMHFile::GetWord()
{
	return (WORD)atoi(GetString());
}
BYTE CMHFile::GetByte()
{
	return (BYTE)atoi(GetString());
}
BOOL CMHFile::GetBool()
{
	return (BOOL)atoi(GetString());
}
BOOL CMHFile::GetExpPoint()
{
	return (EXPTYPE)atoi(GetString());
}
BOOL CMHFile::GetLevel()
{
	return (LEVELTYPE)atoi(GetString());
}


void CMHFile::GetFloat(float* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetFloat();
	}
}
void CMHFile::GetWord(WORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetWord();
	}
}
void CMHFile::GetDword(DWORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetDword();
	}
}

BOOL CMHFile::IsEOF()
{
	return feof(fp);
}

BOOL CMHFile::Seek(int n)
{
	return fseek(fp,n,SEEK_CUR);
}

char* CMHFile::GetStringInQuotation()
{
	char buf[256]={0,};
	static char buf2[256];
	int c;
	while( (c=fgetc(fp)) != '"')	// 첫따옴표 나올때까지 스킵
	{
		if( c == 10 )//첫따옴표 나오기전에 엔터가 있으면 중지
		{
			Seek(-1);
			buf2[0] = 0;
			return buf2;
		}
	}
	int n=0;
	while(1)
	{
		ASSERT(!feof(fp));
		buf[n] = fgetc(fp);
		if(buf[n] == '"' )
		{
			buf[n] = 0;
			break;
		}
		if(buf[n] == 10) // 엔터
		{
			Seek(-1);
			buf[n] = 0;
			break;
		}
		++n;
	}
	char * aa = &buf2[0];
	memcpy( buf2, buf, n+1 );
	return buf2;

}
void CMHFile::GetStringInQuotation(char* pBuf)
{
	strcpy(pBuf,GetStringInQuotation());
}

void CMHFile::SetString(char * str)
{
	fprintf(fp, "%s", str);
}