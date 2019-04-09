// StrTokenizer.cpp: implementation of the CStrTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StrTokenizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrTokenizer::CStrTokenizer(char* String,char* Sep)
{
//	ASSERT(strlen(Sep) < MAX_TOKENIZER_SEPARATOR);
	strcpy(m_strSep,Sep);
	m_ReturnData = strtok(String,m_strSep);
}

CStrTokenizer::~CStrTokenizer()
{

}

char* CStrTokenizer::GetNextToken()
{
	char* pRt = m_ReturnData;
	if(m_ReturnData)
	{
		m_ReturnData = strtok(NULL,m_strSep);
	}
	return pRt;
}

char* CStrTokenizer::GetNextTokenUpper()
{
	char* tok = GetNextToken();
	if( tok )
		return strupr( tok );
	return NULL;
}