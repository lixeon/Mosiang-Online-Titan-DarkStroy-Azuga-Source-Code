// GameInEventFunc.cpp: implementation of the GameInEventFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameInEventFunc.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_

#include "GameIn.h"
#include "DebugDlg.h"

///// ½ºÅ³ Ã¢ ////////////////////////////////////////////////////////////
void SDI_SkillPushupBtnFunc(LONG lId, void * p, DWORD we)
{
	
}

/////// Ã¤ÆÃ Ã¢ /////////////////////////////////////////////////////////////
void CTI_WideBtnFunc(LONG lId, void * p, DWORD we)
{
	cListDialog * list = (cListDialog *)p;
	int cnt = list->GetMiddleImageNum();
	if(cnt > 8)
		cnt = 8;
	list->Resize(cnt+1);
}
void CTI_NarrowBtnFunc(LONG lId, void * p, DWORD we)
{
	cListDialog * list = (cListDialog *)p;
	int cnt = list->GetMiddleImageNum();
	if(cnt < 3)
		cnt = 3;
	list->Resize(cnt-1);
}

/////// Debug Ã¢ /////////////////////////////////////////////////////////////
void DI_AttackBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetAttackBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetAttackBtnFlag(TRUE);
	}
}
void DI_ItemBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetItemBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetItemBtnFlag(TRUE);
	}
}
void DI_MoveBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetMoveBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetMoveBtnFlag(TRUE);
	}
}
void DI_MugongBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetMugongBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetMugongBtnFlag(TRUE);
	}
}
void DI_ChatBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetChatBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetChatBtnFlag(TRUE);
	}
}
void DI_UserConnBtnFunc(LONG iId, void* p, DWORD we)
{
	CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
	if(we == WE_PUSHUP)
	{
		pDbg->SetUserConnBtnFlag(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		pDbg->SetUserConnBtnFlag(TRUE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////


