// MugongWindow.cpp: implementation of the CMugongWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongWindow.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "SkillManager_Client.h"
#include "ItemManager.h"
#include "MugongManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "MugongDialog.h"

#ifdef _JAPAN_LOCAL_
#include "./Interface/cStatic.h"
#include "ObjectGuagen.h"
#include "CheatMsgParser.h"
#endif

#ifdef _HK_LOCAL_
#include "./Interface/cStatic.h"
#include "ObjectGuagen.h"
#include "CheatMsgParser.h"
#endif

#ifdef _TL_LOCAL_
#include "./Interface/cStatic.h"
#include "ObjectGuagen.h"
#include "CheatMsgParser.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongWindow::CMugongWindow()
{
	m_pAllIconGrid = NULL;

#ifdef _JAPAN_LOCAL_

 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;

#endif // _JAPAN_LOCAL_

#ifdef _HK_LOCAL_
 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;
#endif // _HK_LOCAL_

#ifdef _TL_LOCAL_
 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;
#endif
}

CMugongWindow::~CMugongWindow()
{
	m_pAllIconGrid = NULL;

#ifdef _JAPAN_LOCAL_

 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;
	
#endif // _JAPAN_LOCAL_

#ifdef _HK_LOCAL_

 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;
	
#endif // _HK_LOCAL_

#ifdef _TL_LOCAL_

 	m_pWearedIconDlg = NULL;
 	m_pExpGuage = NULL;
 	for(int n=0;n<MGI_MAX;++n)
 		m_pMugongInfo[n] = NULL;
	
#endif
}

void CMugongWindow::Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,cDialog* pParent,DWORD GridID)
{
	m_SelectPos = 0;

	m_MugongType = mtype;

	m_StartPos = startpos;
	m_EndPos = endpos;

	m_pAllIconGrid = (cIconGridDialog*)pParent->GetWindowForID(GridID);
}

BOOL CMugongWindow::GetBlankPositionRef(WORD & absPos)
{
	for(WORD i = 0 ; i < m_pAllIconGrid->GetCellNum() ; ++i)
	{
		if(m_pAllIconGrid->IsAddable(i))
		{
			absPos = m_StartPos + i;
			return TRUE;
		}
	}

	return FALSE;
}
BOOL CMugongWindow::IaAddable(POSTYPE pos)
{
	if(m_StartPos <= pos && pos < m_EndPos)
		return m_pAllIconGrid->IsAddable(pos-m_StartPos);
		
	return FALSE;
}
void CMugongWindow::AddMugong(POSTYPE pos/*abs pos*/, CMugongBase * mugong)
{
	m_pAllIconGrid->AddIcon(pos - m_StartPos, (cIcon *)mugong);
}
BOOL CMugongWindow::DeleteMugong(POSTYPE pos/*abs pos*/, CMugongBase ** mugong)
{	
	if(IsSelectedMugong(pos))
		SelectMugong(0);

	m_pAllIconGrid->DeleteIcon(pos - m_StartPos, (cIcon **)mugong);

	return TRUE;
}
BOOL CMugongWindow::GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos)
{
	if(m_pAllIconGrid->GetPositionForXYRef(x,y,pos) == TRUE)
	{
		pos += m_StartPos;
		return TRUE;
	}

	return FALSE;
}

CMugongBase * CMugongWindow::GetCurSelMugong()
{
	if(m_pAllIconGrid->GetCurSelCellPos() != -1)
	{
		return GetMugong((POSTYPE)(m_pAllIconGrid->GetCurSelCellPos()+m_StartPos));
	}
	
	return NULL;
}

CMugongBase * CMugongWindow::GetMugong(POSTYPE abs_Pos)
{
	if(m_StartPos <= abs_Pos && abs_Pos < m_EndPos)
	{
		return (CMugongBase*)m_pAllIconGrid->GetIconForIdx(abs_Pos-m_StartPos);
	}

	return NULL;
}
void CMugongWindow::SetMugongSung(POSTYPE abspos,BYTE sung)
{
	CMugongBase* pMugongBase = NULL;
	if(m_StartPos <= abspos && abspos < m_EndPos)
	{
		pMugongBase = (CMugongBase*)m_pAllIconGrid->GetIconForIdx(abspos-m_StartPos);
		pMugongBase->SetSung(sung);
	}
	
	if(IsSelectedMugong(abspos))
		RefreshSelectedMugong();
}
void CMugongWindow::SetMugongExpPoint(POSTYPE abspos,DWORD expPoint)
{
	CMugongBase* pMugongBase = NULL;
	if(m_StartPos <= abspos && abspos < m_EndPos)
	{
		pMugongBase = (CMugongBase*)m_pAllIconGrid->GetIconForIdx(abspos-m_StartPos);
		pMugongBase->SetExpPoint(expPoint);
	}
	
	if(IsSelectedMugong(abspos))
		RefreshSelectedMugong();
}
BOOL CMugongWindow::CanBeMoved(cIcon * pIcon, POSTYPE abs_pos)
{
	ASSERT(pIcon);
	if( m_StartPos > abs_pos || abs_pos >= m_EndPos )
		return FALSE;

	switch(pIcon->GetType())
	{
	case WT_MUGONG:
		{
			// 무공 이동 : 유효한 위치인지 체크
			CMugongBase * pMugong = (CMugongBase *)pIcon;		
#ifdef _JAPAN_LOCAL_
			if( m_StartPos > pMugong->GetPosition() || pMugong->GetPosition() >= m_EndPos )
				return FALSE;
#elif defined _HK_LOCAL_
			if( m_StartPos > pMugong->GetPosition() || pMugong->GetPosition() >= m_EndPos )
				return FALSE;
#elif defined _TL_LOCAL_
			if( m_StartPos > pMugong->GetPosition() || pMugong->GetPosition() >= m_EndPos )
				return FALSE;
#else
			// 임시로 막자. 왜 진법이 무공인가??  -0-;;
			if( pMugong->GetPosition() >= TP_JINBUB_START && pMugong->GetPosition() < TP_JINBUB_END )
				return FALSE;

			if( m_StartPos > abs_pos || abs_pos >= m_EndPos )
				return FALSE;
#endif
			return TRUE;
		}
		break;
	case WT_JINBUB:
		{
			// 무공 이동 : 유효한 위치인지 체크
			CMugongBase * pMugong = (CMugongBase *)pIcon;		
			if( m_StartPos > pMugong->GetPosition() || pMugong->GetPosition() >= m_EndPos )
				return FALSE;
#ifdef _JAPAN_LOCAL_
#elif defined _HK_LOCAL_
#elif defined _TL_LOCAL_
#else
			if( m_StartPos > abs_pos || abs_pos >= m_EndPos )
				return FALSE;			
#endif
			return TRUE;
		}
		break;
	case WT_ITEM:
		{
			// 무공 등록
			CItem * pItem = (CItem *)pIcon;
			if(MUGONGMGR->GetMugongByMugongIdx(pItem->GetItemInfo()->MugongNum) != NULL)
				return FALSE;
			
			if(MUGONGMGR->CheckMugongTree(pItem->GetItemInfo()->MugongNum) == FALSE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(699));
				return FALSE;
			}
			
			return ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),m_MugongType);
			
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

	return FALSE;
}

void CMugongWindow::SelectMugong(POSTYPE abspos)
{
	m_SelectPos = abspos;
	RefreshSelectedMugong();
}
BOOL CMugongWindow::IsSelectedMugong(POSTYPE abspos)
{
	if(m_SelectPos==0)
		return FALSE;
	return abspos == m_SelectPos;
}
void CMugongWindow::RefreshSelectedMugong()
{
	if(m_SelectPos==0)
	{
#ifdef _JAPAN_LOCAL_
		SetSkillInfoStatic(NULL);
#elif defined _HK_LOCAL_
		SetSkillInfoStatic(NULL);
#elif defined _TL_LOCAL_
		SetSkillInfoStatic(NULL);
#else
		GAMEIN->GetMugongDialog()->SetSkillInfoStatic(NULL);
#endif // _JAPAN_LOCAL_
		return;
	}

	CMugongBase* pMugong = GetMugong(m_SelectPos);
	if(pMugong == NULL)
	{
		return;
	}
	
#ifdef _JAPAN_LOCAL_
	SetSkillInfoStatic(pMugong);
#elif defined _HK_LOCAL_
	SetSkillInfoStatic(pMugong);
#elif defined _TL_LOCAL_
	SetSkillInfoStatic(pMugong);
#else
	GAMEIN->GetMugongDialog()->SetSkillInfoStatic(pMugong);
#endif // _JAPAN_LOCAL_
}






void CMugongWindow::GridActive( BOOL val )
{
	if( m_pAllIconGrid )
	{
		if( val )
		{
			m_pAllIconGrid->SetDisable( !val );
			m_pAllIconGrid->SetActive( val );
		}
		else
		{
			m_pAllIconGrid->SetActive( val );
			m_pAllIconGrid->SetDisable( !val );			
		}
		
	}
}




#ifdef _JAPAN_LOCAL_

void CMugongWindow::SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos )
{
	if( m_pAllIconGrid )
		m_pAllIconGrid->SetDisableIcon( FromPos, EndPos );
}


void CMugongWindow::Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
						 cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID)
{
	m_SelectPos = 0;
	
	m_MugongType = mtype;
	
	m_StartPos = startpos;
	m_EndPos = endpos;
	
	m_pWearedIconDlg = (cIconDialog*)pParentDlg->GetWindowForID(wearWndID);
	m_pAllIconGrid = (cIconGridDialog*)pParentDlg->GetWindowForID(gridWndID);
	m_pMugongInfo[MGI_NAME] = (cStatic*)pParentDlg->GetWindowForID(nameWndID);
	m_pMugongInfo[MGI_SUNG] = (cStatic*)pParentDlg->GetWindowForID(levelWndID);
	m_pMugongInfo[MGI_EXPPOINT] = (cStatic*)pParentDlg->GetWindowForID(expWndID);
	m_pExpGuage = (CObjectGuagen*)pParentDlg->GetWindowForID(guageID);
	
	SetSkillInfoStatic(NULL);
}
 
void CMugongWindow::SetSkillInfoStatic(CMugongBase* pMugongBase)
{
 	if( m_pExpGuage == NULL ) return;
 	
 	m_pWearedIconDlg->DeleteIcon(0,NULL);
 	if(pMugongBase == NULL)
 	{
 		m_pMugongInfo[MGI_NAME]->SetStaticText("");
 		m_pMugongInfo[MGI_SUNG]->SetStaticText("");
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("0/0");
 		m_pExpGuage->SetValue(0, 0);
 		return;
 	}
 
 	m_pWearedIconDlg->AddIcon(0,pMugongBase,TRUE);
 
 	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetItemIdx());	
 	if(pSkillInfo == NULL)
 		return;
 
 	char temp[256];
 	WORD Sung = pMugongBase->GetSung();
 	DWORD Exp = pMugongBase->GetExpPoint();
 
 	m_pMugongInfo[MGI_NAME]->SetStaticText(pSkillInfo->GetSkillName());
 	m_pMugongInfo[MGI_SUNG]->SetStaticValue(pMugongBase->GetSung());
 	
 	if(Sung == 0)
 	{
 		m_pExpGuage->SetValue(0, 0);
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("");
 	}
 	else if(Sung != 12)
 	{
		float percent = Exp * 100.0f / pSkillInfo->GetNeedExp(Sung);
		if( percent > 99.99f )
		{
			if( Exp != pSkillInfo->GetNeedExp(Sung)-1 )
				percent = 99.99f;
		}

// 		sprintf(temp,"%4.2f%%",100*Exp/(float)pSkillInfo->GetNeedExp(Sung));
 		sprintf(temp,"%4.2f%%",percent);
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 
 #ifdef _CHEATENABLE_		
 		if(CHEATMGR->IsCheatEnable())
 		{
// 			sprintf(temp,"%4.2f%%[%d/%d]",100*Exp/(float)pSkillInfo->GetNeedExp(Sung),Exp,pSkillInfo->GetNeedExp(Sung));
 			sprintf(temp,"%4.2f%%[%d/%d]",percent,Exp,pSkillInfo->GetNeedExp(Sung));
 			m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 		}
 #endif
 		if(Exp == 0)
 		{
 			m_pExpGuage->SetValue(0, 0);
 		}
 		else
 			m_pExpGuage->SetValue((float)Exp/(float)pSkillInfo->GetNeedExp(Sung), 0);
 	}
 	else
 	{
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(CHATMGR->GetChatMsg(480));	//"完成"
 	}
}


#endif //_JAPAN_LOCAL_


#ifdef _HK_LOCAL_

void CMugongWindow::SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos )
{
	if( m_pAllIconGrid )
		m_pAllIconGrid->SetDisableIcon( FromPos, EndPos );
}


void CMugongWindow::Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
						 cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID)
{
	m_SelectPos = 0;
	
	m_MugongType = mtype;
	
	m_StartPos = startpos;
	m_EndPos = endpos;
	
	m_pWearedIconDlg = (cIconDialog*)pParentDlg->GetWindowForID(wearWndID);
	m_pAllIconGrid = (cIconGridDialog*)pParentDlg->GetWindowForID(gridWndID);
	m_pMugongInfo[MGI_NAME] = (cStatic*)pParentDlg->GetWindowForID(nameWndID);
	m_pMugongInfo[MGI_SUNG] = (cStatic*)pParentDlg->GetWindowForID(levelWndID);
	m_pMugongInfo[MGI_EXPPOINT] = (cStatic*)pParentDlg->GetWindowForID(expWndID);
	m_pExpGuage = (CObjectGuagen*)pParentDlg->GetWindowForID(guageID);
	
	SetSkillInfoStatic(NULL);
}
 
void CMugongWindow::SetSkillInfoStatic(CMugongBase* pMugongBase)
{
 	if( m_pExpGuage == NULL ) return;
 	
 	m_pWearedIconDlg->DeleteIcon(0,NULL);
 	if(pMugongBase == NULL)
 	{
 		m_pMugongInfo[MGI_NAME]->SetStaticText("");
 		m_pMugongInfo[MGI_SUNG]->SetStaticText("");
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("0/0");
 		m_pExpGuage->SetValue(0, 0);
 		return;
 	}
 
 	m_pWearedIconDlg->AddIcon(0,pMugongBase,TRUE);
 
 	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetItemIdx());	
 	if(pSkillInfo == NULL)
 		return;
 
 	char temp[256];
 	WORD Sung = pMugongBase->GetSung();
 	DWORD Exp = pMugongBase->GetExpPoint();
 
 	m_pMugongInfo[MGI_NAME]->SetStaticText(pSkillInfo->GetSkillName());
 	m_pMugongInfo[MGI_SUNG]->SetStaticValue(pMugongBase->GetSung());
 	
 	if(Sung == 0)
 	{
 		m_pExpGuage->SetValue(0, 0);
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("");
 	}
 	else if(Sung != 12)
 	{
		float percent = Exp * 100.0f / pSkillInfo->GetNeedExp(Sung);
		if( percent > 99.99f )
		{
			if( Exp != pSkillInfo->GetNeedExp(Sung)-1 )
				percent = 99.99f;
		}

// 		sprintf(temp,"%4.2f%%",100*Exp/(float)pSkillInfo->GetNeedExp(Sung));
 		sprintf(temp,"%4.2f%%",percent);
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 
 #ifdef _CHEATENABLE_		
 		if(CHEATMGR->IsCheatEnable())
 		{
// 			sprintf(temp,"%4.2f%%[%d/%d]",100*Exp/(float)pSkillInfo->GetNeedExp(Sung),Exp,pSkillInfo->GetNeedExp(Sung));
 			sprintf(temp,"%4.2f%%[%d/%d]",percent,Exp,pSkillInfo->GetNeedExp(Sung));
 			m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 		}
 #endif
 		if(Exp == 0)
 		{
 			m_pExpGuage->SetValue(0, 0);
 		}
 		else
 			m_pExpGuage->SetValue((float)Exp/(float)pSkillInfo->GetNeedExp(Sung), 0);
 	}
 	else
 	{
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(CHATMGR->GetChatMsg(480));	//"完成"
 	}
}


#endif //_HK_LOCAL_

#ifdef _TL_LOCAL_

void CMugongWindow::SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos )
{
	if( m_pAllIconGrid )
		m_pAllIconGrid->SetDisableIcon( FromPos, EndPos );
}


void CMugongWindow::Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
						 cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID)
{
	m_SelectPos = 0;
	
	m_MugongType = mtype;
	
	m_StartPos = startpos;
	m_EndPos = endpos;
	
	m_pWearedIconDlg = (cIconDialog*)pParentDlg->GetWindowForID(wearWndID);
	m_pAllIconGrid = (cIconGridDialog*)pParentDlg->GetWindowForID(gridWndID);
	m_pMugongInfo[MGI_NAME] = (cStatic*)pParentDlg->GetWindowForID(nameWndID);
	m_pMugongInfo[MGI_SUNG] = (cStatic*)pParentDlg->GetWindowForID(levelWndID);
	m_pMugongInfo[MGI_EXPPOINT] = (cStatic*)pParentDlg->GetWindowForID(expWndID);
	m_pExpGuage = (CObjectGuagen*)pParentDlg->GetWindowForID(guageID);
	
	SetSkillInfoStatic(NULL);
}
 
void CMugongWindow::SetSkillInfoStatic(CMugongBase* pMugongBase)
{
 	if( m_pExpGuage == NULL ) return;
 	
 	m_pWearedIconDlg->DeleteIcon(0,NULL);
 	if(pMugongBase == NULL)
 	{
 		m_pMugongInfo[MGI_NAME]->SetStaticText("");
 		m_pMugongInfo[MGI_SUNG]->SetStaticText("");
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("0/0");
 		m_pExpGuage->SetValue(0, 0);
 		return;
 	}
 
 	m_pWearedIconDlg->AddIcon(0,pMugongBase,TRUE);
 
 	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugongBase->GetItemIdx());	
 	if(pSkillInfo == NULL)
 		return;
 
 	char temp[256];
 	WORD Sung = pMugongBase->GetSung();
 	DWORD Exp = pMugongBase->GetExpPoint();
 
 	m_pMugongInfo[MGI_NAME]->SetStaticText(pSkillInfo->GetSkillName());
 	m_pMugongInfo[MGI_SUNG]->SetStaticValue(pMugongBase->GetSung());
 	
 	if(Sung == 0)
 	{
 		m_pExpGuage->SetValue(0, 0);
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("");
 	}
 	else if(Sung != 12)
 	{
 		sprintf(temp,"%4.2f%%",100*Exp/(float)pSkillInfo->GetNeedExp(Sung));
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 
 #ifdef _CHEATENABLE_		
 		if(CHEATMGR->IsCheatEnable())
 		{
// 			sprintf(temp,"%4.2f%%[%d/%d]",100*Exp/(float)pSkillInfo->GetNeedExp(Sung),Exp,pSkillInfo->GetNeedExp(Sung));
 			sprintf(temp,"%4.2f%%[%d/%d]",percent,Exp,pSkillInfo->GetNeedExp(Sung));
 			m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);
 		}
 #endif
 		if(Exp == 0)
 		{
 			m_pExpGuage->SetValue(0, 0);
 		}
 		else
 			m_pExpGuage->SetValue((float)Exp/(float)pSkillInfo->GetNeedExp(Sung), 0);
 	}
 	else
 	{
 		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(CHATMGR->GetChatMsg(480));	//"完成"
 	}
}


#endif //_HK_LOCAL_
