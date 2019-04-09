// DebugDlg.cpp: implementation of the CDebugDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DebugDlg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugDlg::CDebugDlg()
{
	m_bAttackFlag = FALSE;
	m_bItemFlag = FALSE;
	m_bMoveFlag = FALSE;
	m_bMugongFlag = FALSE;
	m_bChatFlag = FALSE;
}

CDebugDlg::~CDebugDlg()
{

}

void CDebugDlg::DebugMsgParser(BYTE type, char* msg, ...)
{	
	char Dbgmsg[255];
	char Dbgmsg2[255];
	va_list argList;

	va_start(argList, msg);
	vsprintf(Dbgmsg2,msg,argList);
	va_end(argList);
	
	switch(type)
	{
	case DBG_ATTACK:
		{
			if(m_bAttackFlag)
			{
				sprintf(Dbgmsg, "ATTACK: %s",Dbgmsg2);
				AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));		
			}
			
		}
		break;
	case DBG_ITEM:
		{
			if(m_bItemFlag)
			{
				sprintf(Dbgmsg, "ITEM: %s",Dbgmsg2);
				AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));		
			}
			
		}
		break;
	case DBG_MOVE:
		{
			if(m_bMoveFlag)
			{	
				sprintf(Dbgmsg, "MOVE: %s",Dbgmsg2);
				AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));		
			}
		}
		break;
	case DBG_MUGONG:
		{
			if(m_bMugongFlag)
			{
				sprintf(Dbgmsg, "MUGONG: %s",Dbgmsg2);
				AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));		
			}
		}
		break;
	case DBG_CHAT:
		{
			if(m_bChatFlag)
			{
				sprintf(Dbgmsg, "CHAT: %s",Dbgmsg2);
				AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));	
			}
		}
		break;
	default:
		{
			sprintf(Dbgmsg, "NORMAL: %s",Dbgmsg2);
			AddItem(Dbgmsg, RGBA_MAKE(255,0,0,255));		
		}
		break;
	}
}