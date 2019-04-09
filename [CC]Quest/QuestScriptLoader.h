// QuestScriptLoader.h: interface for the CQuestScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_)
#define AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StrTokenizer.h"

class CMHFile;
class CQuestInfo;
class CSubQuestInfo;
class CQuestString;

class CQuestLimitKind;
class CQuestCondition;
class CQuestEvent;
class CQuestExecute;

class CQuestNpcScriptData;
class CQuestNpcData;

class CQuestScriptLoader  
{
public:
	CQuestScriptLoader();
	virtual ~CQuestScriptLoader();

	static CQuestInfo*		LoadQuestInfo( CMHFile* pFile, DWORD dwQuestIdx );
	static CSubQuestInfo*	LoadSubQuestInfo( CMHFile* pFile, DWORD dwQuestIdx, DWORD dwSubQuestIdx, CQuestInfo* pQuestInfo  );
	static CQuestString*	LoadQuestString(CMHFile* pFile);

	static CQuestLimitKind*	LoadQuestLimitKind( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	static CQuestCondition*	LoadQuestCondition( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	static CQuestEvent*		LoadQuestEvent( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );	
	static CQuestExecute*	LoadQuestExecute( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	
	static CQuestNpcScriptData*	LoadQuestNpcScriptData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	static CQuestNpcData*		LoadQuestNpcData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
};

template<class T> void GetScriptParam( T& dataOut, CStrTokenizer* pTokens )
{
	char* pOneToken = pTokens->GetNextTokenUpper();
	dataOut = (T)atol(pOneToken);
}

#endif // !defined(AFX_QUESTSCRIPTLOADER_H__19DDC2D1_0947_4501_B70B_C636D9250099__INCLUDED_)
