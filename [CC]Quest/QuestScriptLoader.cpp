// QuestScriptLoader.cpp: implementation of the CQuestScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestScriptLoader.h"

#include "MHFile.h"
#include "QuestInfo.h"
#include "SubQuestInfo.h"
#include "QuestString.h"
#include "QuestEvent.h"

#include "QuestLimit.h"
#include "QuestLimitKind.h"
#include "QuestLimitKind_Level.h"
#include "QuestLimitKind_Quest.h"

#include "QuestTrigger.h"
#include "QuestCondition.h"
#include "QuestExecute.h"
#include "QuestExecute_Quest.h"
#include "QuestExecute_Count.h"
#include "QuestExecute_Item.h"
#include "QuestExecute_RandomItem.h"
#include "QuestExecute_Time.h"

#include "QuestNpcScript.h"
#include "QuestNpcScriptData.h"
#include "QuestNpc.h"
#include "QuestNpcData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define LOADUNIT(ekind,strkind,classname,questidx,subquestidx)	\
		if( strcmp(strKind,strkind) == 0 )						\
			return new classname(ekind,pTokens,questidx,subquestidx);

CQuestScriptLoader::CQuestScriptLoader()
{
}

CQuestScriptLoader::~CQuestScriptLoader()
{
}

CQuestInfo* CQuestScriptLoader::LoadQuestInfo( CMHFile* pFile, DWORD dwQuestIdx )
{
	CQuestInfo* pQuestInfo = new CQuestInfo( dwQuestIdx );
	CSubQuestInfo* pSub;
	char tok[256];
	DWORD dwSubQuestIdx = 0;
	while( !pFile->IsEOF() )
	{
		pFile->GetString( tok ); 
		if( tok[0] == '}' )
			break;
		if( strcmp( tok, "$SUBQUEST" ) == 0 )
		{
			dwSubQuestIdx = pFile->GetDword();
			pSub = LoadSubQuestInfo( pFile, dwQuestIdx, dwSubQuestIdx, pQuestInfo );
			ASSERT(pSub);
			pQuestInfo->AddSubQuestInfo( dwSubQuestIdx, pSub );
		}
	}

	return pQuestInfo;
}

CSubQuestInfo* CQuestScriptLoader::LoadSubQuestInfo( CMHFile* pFile, DWORD dwQuestIdx, DWORD dwSubQuestIdx, CQuestInfo* pQuestInfo )
{
	CSubQuestInfo* pSub = new CSubQuestInfo;
	
	char buf[1024];
	char Token[256];
	while( !pFile->IsEOF() )
	{
		pFile->GetString( Token );
		if( Token[0] == '}' )
			break;

		if( strcmp(Token,"#LIMIT") == 0 )
		{
			pFile->GetLine( buf, 1024 );
			CStrTokenizer tok( buf, " \t\n" );
			pSub->AddQuestLimit( new CQuestLimit( &tok, dwQuestIdx, dwSubQuestIdx ) );
		}
#ifdef _MAPSERVER_
		else if( strcmp(Token,"#TRIGGER") == 0 )
		{
			pFile->GetLine( buf, 1024 );
			CStrTokenizer tok( buf, " \t\n" );
			// 반복퀘스트의 확인을 위해서 추가 RaMa - 04.10.26
			CQuestTrigger* pQuestTrigger = new CQuestTrigger( &tok, dwQuestIdx, dwSubQuestIdx );
			if(pQuestTrigger)
				pQuestInfo->SetEndParam( pQuestTrigger->GetEndParam() );
			pSub->AddQuestTrigger( pQuestTrigger );			
		}		
#else
		else if( strcmp(Token,"#NPCSCRIPT") == 0 )
		{
			pFile->GetLine( buf, 1024 );
			CStrTokenizer tok( buf, " \t\n" );
			pSub->AddQuestNpcScipt( new CQuestNpcScript( &tok, dwQuestIdx, dwSubQuestIdx ) );
		}
		else if( strcmp(Token,"#NPCADD") == 0 )
		{
			pFile->GetLine( buf, 1024 );
			CStrTokenizer tok( buf, " \t\n" );
			pSub->AddQuestNpc( new CQuestNpc( &tok, dwQuestIdx, dwSubQuestIdx ) );
		}		
		else if( strcmp(Token, "#MAXCOUNT") == 0 )
		{
			pSub->SetMaxCount( pFile->GetDword() );
		}
#endif
	}

	return pSub;
}

CQuestString* CQuestScriptLoader::LoadQuestString(CMHFile* pFile)
{
	CQuestString* pQStr = new CQuestString;

	char* pChekString = NULL;
	char buf[1024];
	char Token[1024];
	int	tline=0;
	int dline=0;

	while( !pFile->IsEOF() )
	{
		pFile->GetString(Token);

		if( pChekString = strstr( Token, "}" ) )
			break;
		
		if(strcmp(Token,"#TITLE") == 0)
		{
			pFile->GetLine(buf, 1024);
			pQStr->AddLine(buf, tline, TRUE);
		}
		if(strcmp(Token,"#DESC") == 0)
		{
			while( !pFile->IsEOF() ) 
			{
				pFile->GetLine(buf, 1024);

				char * p = buf;
				int nRt = 0;
				while( *p )
				{
					if( IsDBCSLeadByte( *p ) )
					{
						++p;
					}
					else
					{
						if( *p == '{' )
						{
							nRt = 1;
							break;
						}
						else if(  *p == '}' )
						{
							nRt = 2;
							break;
						}
					}

					++p;
				}

				if( nRt == 1 ) continue;
				else if( nRt == 2 ) break;

//				if( pChekString = strstr( buf, "{" ) )			continue;
//				else if( pChekString = strstr( buf, "}" ) )		break;

				pQStr->AddLine(buf, dline);
				++dline;
			}
		}
	}

	return pQStr;
}

CQuestLimitKind* CQuestScriptLoader::LoadQuestLimitKind( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestLimitKind_Level, "&LEVEL", CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestLimitKind_Money, "&MONEY", CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );
#ifdef _JAPAN_LOCAL_
	LOADUNIT( eQuestLimitKind_Attr, "&ATTR", CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );	
#endif
	LOADUNIT( eQuestLimitKind_Quest, "&QUEST", CQuestLimitKind_Quest, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestLimitKind_SubQuest, "&SUBQUEST", CQuestLimitKind_Quest, dwQuestIdx, dwSubQuestIdx );	
	LOADUNIT( eQuestLimitKind_Stage, "&STAGE", CQuestLimitKind_Level, dwQuestIdx, dwSubQuestIdx );
	return NULL;
}

CQuestCondition* CQuestScriptLoader::LoadQuestCondition( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	CQuestEvent* pEvent = LoadQuestEvent( strKind, pTokens, dwQuestIdx, dwSubQuestIdx );
	ASSERT(pEvent);
	CQuestCondition* pCondition = new CQuestCondition( pEvent, dwQuestIdx, dwSubQuestIdx );

	return pCondition;
}

CQuestEvent* CQuestScriptLoader::LoadQuestEvent( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestEvent_NpcTalk, "@TALKTONPC", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_Hunt, "@HUNT", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_Count, "@COUNT", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_GameEnter, "@GAMEENTER", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_Level, "@LEVEL", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_UseItem, "@USEITEM", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_MapChange, "@MAPCHANGE", CQuestEvent, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestEvent_Die, "@DIE", CQuestEvent, dwQuestIdx, dwSubQuestIdx );	
	LOADUNIT( eQuestEvent_HuntAll, "@HUNTALL", CQuestEvent, dwQuestIdx, dwSubQuestIdx );	
	return NULL;
}

CQuestExecute* CQuestScriptLoader::LoadQuestExecute( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( eQuestExecute_EndQuest, "*ENDQUEST", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_StartSub, "*STARTSUB", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_EndSub, "*ENDSUB", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_EndOtherSub, "*ENDOTHERSUB", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	
	LOADUNIT( eQuestExecute_AddCount, "*ADDCOUNT", CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_AddCountFQW, "*ADDCOUNTFQW", CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_AddCountFW, "*ADDCOUNTFW", CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_LevelGap, "*ADDCOUNTLEVELGAP", CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_MonLevel, "*ADDCOUNTMONLEVEL", CQuestExecute_Count, dwQuestIdx, dwSubQuestIdx );

	LOADUNIT( eQuestExecute_GiveQuestItem, "*GIVEQUESTITEM", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeQuestItem, "*TAKEQUESTITEM", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_GiveItem, "*GIVEITEM", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_GiveMoney, "*GIVEMONEY", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeItem, "*TAKEITEM", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeMoney, "*TAKEMONEY", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeExp, "*TAKEEXP", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeSExp, "*TAKESEXP", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeQuestItemFQW, "*TAKEQUESTITEMFQW", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeQuestItemFW, "*TAKEQUESTITEMFW", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_TakeMoneyPerCount, "*TAKEMONEYPERCOUNT", CQuestExecute_Item, dwQuestIdx, dwSubQuestIdx );	
	
	LOADUNIT( eQuestExecute_RandomTakeItem, "*RANDOMTAKEITEM", CQuestExecute_RandomItem, dwQuestIdx, dwSubQuestIdx );

	LOADUNIT( eQuestExecute_RegenMonster, "*REGENMONSTER", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	LOADUNIT( eQuestExecute_MapChange, "*MAPCHANGE", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	
	LOADUNIT( eQuestExecute_ChangeStage, "*CHANGESTAGE", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );
	
	LOADUNIT( eQuestExecute_RegistTime, "*REGISTTIME", CQuestExecute_Time, dwQuestIdx, dwSubQuestIdx );

	LOADUNIT( eQuestExecute_EndOtherQuest, "*ENDOTHERQUEST", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );

	LOADUNIT( eQuestExecute_SaveLoginPoint, "*SAVELOGINPOINT", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );

#ifdef _JAPAN_LOCAL_
	LOADUNIT( eQuestExecute_ChangeSubAttr, "*CHANGESUBATTR", CQuestExecute_Quest, dwQuestIdx, dwSubQuestIdx );	
#endif
	
	return NULL;	
}

CQuestNpcScriptData* CQuestScriptLoader::LoadQuestNpcScriptData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( 0, "@NPC", CQuestNpcScriptData, dwQuestIdx, dwSubQuestIdx );
	return NULL;
}

CQuestNpcData* CQuestScriptLoader::LoadQuestNpcData( char* strKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
	LOADUNIT( 0, "@NPC", CQuestNpcData, dwQuestIdx, dwSubQuestIdx );
	return NULL;
}
