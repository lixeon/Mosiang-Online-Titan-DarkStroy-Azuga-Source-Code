// StringLoader.cpp: implementation of the CStringLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringLoader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CStringLoader);
CStringLoader::CStringLoader()
{
	m_MaxLoadedString = 0;
	m_LoadedString = NULL;
}

CStringLoader::~CStringLoader()
{
	Release();
}

BOOL CStringLoader::Init(char* strFileName)
{
	FILE* fp = fopen(strFileName,"r");
	if(fp == NULL)
		return FALSE;

	char temp[256];
	
	fgets(temp,256,fp);
	m_MaxLoadedString = atoi(temp);

	m_LoadedString = new StaticString[m_MaxLoadedString];
	
	for(int n=0;n<m_MaxLoadedString;++n)
	{
		fgets(temp,256,fp);
		if(temp[strlen(temp) - 1] == 0x0a)
			temp[strlen(temp) - 1] = 0;
		m_LoadedString[n] = temp;				
	}

	return TRUE;
}

void CStringLoader::Release()
{
	SAFE_DELETE_ARRAY(m_LoadedString);
}

char* CStringLoader::GetString(DWORD StrNum, ...)
{
	va_list argList;
	va_start(argList, StrNum);
	vsprintf(lpszReturn,m_LoadedString[StrNum],argList);
	va_end(argList);

	return lpszReturn;
}