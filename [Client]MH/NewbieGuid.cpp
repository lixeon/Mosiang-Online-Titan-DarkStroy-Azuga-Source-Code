#include "stdafx.h"
#include "NewbieGuid.h"
#include "HelperManager.h"

CNewbieGuid::CNewbieGuid()
{
	
}

CNewbieGuid::~CNewbieGuid()
{

}

void CNewbieGuid::CheckCondition(GAMEEVENT* event)
{
	switch(event->bEventKind)
	{
	case eGameEvent1_LevelUp:
			LevelUpCheck(event->dwData);
		break;
	case eGameEvent1_HeroDie:
		{
			if( m_State.IsSet(2) ) return;

			SetState(2);
			HELPERMGR->ShowRespect(19);
		}
		break;
	case eGameEvent1_SavePoint:
		{
			if( m_State.IsSet(3) ) return;

			SetState(3);
			HELPERMGR->ShowRespect(29);
		}
		break;
	case eGameEvent1_Ungijosik:
		{
			if( m_State.IsSet(4) ) return;

			SetState(4);
			HELPERMGR->ShowRespect(24);
		}
		break;
	case eGameEvent1_GameIn:
		{
			if( m_State.IsSet(5) )
			{
				MapChangeCheck(event->dwData);
				return;
			}

			SetState(5);
			HELPERMGR->ShowRespect(18);
		}
		break;
	case eGameEvent1_MonsterKill:
		{
			if( m_State.IsSet(6) ) return;
			SetState(6);
			HELPERMGR->ShowRespect(21);
		}
		break;
	case eGameEvent1_ObtainItem:
		{
			if( m_State.IsSet(7) ) return;
			SetState(7);
			HELPERMGR->ShowRespect(23);
		}
		break;
	default:
		return;
	}
}

void CNewbieGuid::LevelUpCheck(DWORD level)
{
	switch(level)
	{
	case 2:
			HELPERMGR->ShowRespect(12);
		break;
	case 3:
			HELPERMGR->ShowRespect(13);
		break;
	case 4:
			HELPERMGR->ShowRespect(14);
		break;
	case 5:
			HELPERMGR->ShowRespect(15);
		break;
	case 8:
			HELPERMGR->ShowRespect(16);
		break;
	case 10:	 // Á¾·á
		{
			SetValue( (~((QSTATETYPE)0)) );
			HELPERMGR->ShowRespect(17);
		}
		break;
	default:
		return;
	}
}

void CNewbieGuid::MapChangeCheck(DWORD map)
{
	switch(map)
	{
	case 17:
		{
			if( m_State.IsSet(8) ) break;

			HELPERMGR->ShowRespect(28);
			SetState(8);
		}
		break;
	}
}