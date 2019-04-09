// ChinaAdviceDlg.cpp: implementation of the CChinaAdviceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChinaAdviceDlg.h"
#include "./Interface/cTextArea.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChinaAdviceDlg::CChinaAdviceDlg()
{

}

CChinaAdviceDlg::~CChinaAdviceDlg()
{

}

void CChinaAdviceDlg::Linking()
{
	cTextArea* pTextArea = (cTextArea*)GetWindowForID( CNA_TEXTAREA );
	if( pTextArea )
		pTextArea->SetScriptText( CHATMGR->GetChatMsg(30) );
}

void CChinaAdviceDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	
}
