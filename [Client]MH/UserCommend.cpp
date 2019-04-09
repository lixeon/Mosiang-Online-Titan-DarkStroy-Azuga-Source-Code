//
//060727 - by wonju : UserCommendParser
//
#include "stdafx.h"
#include "UserCommend.h"
#include <vector>
#include "GameResourceManager.h"
#include "GuildManager.h"

CUserCommendParser::CUserCommendParser()
{
}

CUserCommendParser::~CUserCommendParser()
{
}

BOOL CUserCommendParser::Apply(char* str)
{
	//리소스 메니저에 등록된 액션목록과 비교하여
	//같은 값이면 실행한다.
	for(unsigned int i = 0; i < GAMERESRCMNGR->GetUserCommendList().size(); ++i)
	{
		int nResult = (GAMERESRCMNGR->GetUserCommendList())[i].compare(str);
		
		if(nResult == 0)
		{
			switch(i) //각자 맞는 행위를 한다.
			{
			case 0:
				{
					GUILDMGR->GetTotalHuntedMonsterCountFromMapServer();
				}
				break;
			case 1:
				break;
			default:break;
			}

			return TRUE;
		}
	}
	
	return FALSE;
}

