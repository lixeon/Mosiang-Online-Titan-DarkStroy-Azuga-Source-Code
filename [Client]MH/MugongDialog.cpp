// MugongDialog.cpp: implementation of the CMugongDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongDialog.h"
#include "MugongManager.h"

#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "ItemManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "input/UserInput.h"
#include "ChatManager.h"
#include "QuickManager.h"
#include "SkillManager_Client.h"
#include "ObjectManager.h"
#include "ObjectGuagen.h"
#include "CheatMsgParser.h"
#include "./interface/cWindowManager.h"
#include "cMsgBox.h"


//#include "TitanMugongMixDlg.h"	// magi82 - Titan(070611) 타이탄 무공변환 주석처리


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongDialog::CMugongDialog()
{
	m_type = WT_MUGONGDIALOG;
	m_pTempDeleteMugong = NULL;

	m_pWearedIconDlg = NULL;
	m_pExpGuage = NULL;
	for(int n=0;n<MGI_MAX;++n)
		m_pMugongInfo[n] = NULL;

	m_bMugongGridExtend = FALSE;
	m_bPushTabWithMouseOver = FALSE;
	m_BtnPushstartTime = 0;
}

CMugongDialog::~CMugongDialog()
{
//	DeleteIconAll();

	m_pWearedIconDlg = NULL;
	m_pExpGuage = NULL;
	for(int n=0;n<MGI_MAX;++n)
		m_pMugongInfo[n] = NULL;
}

void CMugongDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cIconDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_MUGONGDIALOG;
}

void CMugongDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	cDialog::SetActiveRecursive(val);

#ifdef _JAPAN_LOCAL_
	
	if( !val || !HERO )			return;
	
	DWORD ExtraSlot = HERO->GetExtraMugongSlot();
	
	m_pMugongRect[MUGONGWINDOW_NORMAL].SetDiableIcon( (POSTYPE)(GIVEN_MUGONG_SLOT+(MUGONG_SLOT_ADDCOUNT*ExtraSlot)), (POSTYPE)SLOT_MUGONG_NUM_JAP );

#elif defined _HK_LOCAL_
	
	if( !val || !HERO )			return;
	
	DWORD ExtraSlot = HERO->GetExtraMugongSlot();
	
	m_pMugongRect[MUGONGWINDOW_NORMAL].SetDiableIcon( (POSTYPE)(GIVEN_MUGONG_SLOT+(MUGONG_SLOT_ADDCOUNT*ExtraSlot)), (POSTYPE)SLOT_MUGONG_NUM_JAP );
#elif defined _TL_LOCAL_
	
	if( !val || !HERO )			return;
	
	DWORD ExtraSlot = HERO->GetExtraMugongSlot();
	
	m_pMugongRect[MUGONGWINDOW_NORMAL].SetDiableIcon( (POSTYPE)(GIVEN_MUGONG_SLOT+(MUGONG_SLOT_ADDCOUNT*ExtraSlot)), (POSTYPE)SLOT_MUGONG_NUM_JAP );
#else

	if( val )
	{
		for( DWORD i=0; i<MUGONGWINDOW_MAX; ++i )
		{
			if( m_CurSelectedGrid == MGI_MUGONG_PUSHUP1+i )
			{
				m_pMugongRect[i].GridActive( TRUE );
				m_pMugongRectBtn[i]->SetPush( TRUE );
				m_PreSelectedGrid = MGI_MUGONG_PUSHUP1+i;
				m_CurSelectedGrid = MGI_MUGONG_PUSHUP1+i;
			}
			else
			{
				m_pMugongRectBtn[i]->SetPush( FALSE );
				m_pMugongRect[i].GridActive( FALSE );
			}
		}

		BOOL bActive = FALSE;
		if( m_bMugongGridExtend )
		{
			m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->SetActive( TRUE );

			if( m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->IsPushed() )
				bActive = TRUE;
			m_pMugongRect[MUGONGWINDOW_EXTEND].GridActive( bActive );
			m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->SetPush( bActive );
			
			DWORD distrelx = (DWORD)(m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetRelX() - m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->GetRelX());
			DWORD distabsx = (DWORD)(m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetAbsX() - m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->GetAbsX());

			m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->SetRelXY( (LONG)(m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetRelX() + distrelx), (LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetRelY() );
			m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->SetAbsXY( (LONG)(m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetAbsX() + distabsx), (LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetAbsY() );		

			m_pMugongRectBtn[MUGONGWINDOW_TITAN]->SetRelXY( (LONG)(m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetRelX() + distrelx), (LONG)m_pMugongRectBtn[MUGONGWINDOW_TITAN]->GetRelY() );
			m_pMugongRectBtn[MUGONGWINDOW_TITAN]->SetAbsXY( (LONG)(m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetAbsX() + distabsx), (LONG)m_pMugongRectBtn[MUGONGWINDOW_TITAN]->GetAbsY() );		
		}
		else
		{
			m_pMugongRect[MUGONGWINDOW_EXTEND].GridActive( FALSE );
			m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->SetPush( FALSE );
			m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->SetActive( FALSE );

			m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->SetRelXY( (LONG)m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetRelX(),
				(LONG)m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetRelY() );
			m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->SetAbsXY((LONG)m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetAbsX(),
				(LONG)m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->GetAbsY() );

			// magi82 - Titan(070914) 타이탄 무공업데이트
			DWORD distrelx = (DWORD)(m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetRelX() - m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->GetRelX());
			DWORD distabsx = (DWORD)(m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetAbsX() - m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->GetAbsX());

			m_pMugongRectBtn[MUGONGWINDOW_TITAN]->SetRelXY( (LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetRelX() + distrelx,
				(LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetRelY() );
			m_pMugongRectBtn[MUGONGWINDOW_TITAN]->SetAbsXY((LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetAbsX() + distabsx,
				(LONG)m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->GetAbsY() );	}
		}
	/*
	else	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	{
        if(GAMEIN->GetTitanMugongMixDlg() && GAMEIN->GetTitanMugongMixDlg()->IsActive())
			GAMEIN->GetTitanMugongMixDlg()->DeleteMugongIcon();
	}
	*/

#endif	// _JAPAN_LOCAL
}

void CMugongDialog::Add(cWindow * window)
{

	cDialog::Add(window);
}

BOOL CMugongDialog::GetBlankPositionRef(BYTE type, WORD & absPos)
{
	MLOC loc = MUGONGWINDOW_MAX;
	switch(type)
	{
	case eMUGONGBOOK:
		{

#ifdef _JAPAN_LOCAL_
			if( absPos <= TP_MUGONG_START && absPos < TP_MUGONG_END )
				loc = MUGONGWINDOW_NORMAL;
#elif defined _HK_LOCAL_
			if( absPos <= TP_MUGONG_START && absPos < TP_MUGONG_END )
				loc = MUGONGWINDOW_NORMAL;
#elif defined _TL_LOCAL_
			if( absPos <= TP_MUGONG_START && absPos < TP_MUGONG_END )
				loc = MUGONGWINDOW_NORMAL;

#else
			if( absPos <= TP_MUGONG1_START && absPos < TP_MUGONG1_END )
				loc = MUGONGWINDOW_NORMAL;
			else if( absPos <= TP_MUGONG2_START && absPos < TP_MUGONG2_END )
				loc = MUGONGWINDOW_EXTEND;
#endif //_JAPAN_LOCAL_
		}
		break;
	case eJINBUBBOOK:
		loc = MUGONGWINDOW_JINBUB;
		break;
	}
	if(loc == MUGONGWINDOW_MAX)
	{
		ASSERT(0);
		return FALSE;
	}

	return m_pMugongRect[loc].GetBlankPositionRef(absPos);
}

void CMugongDialog::AddMugong(MUGONGBASE * mugongBase)
{
	if(mugongBase->dwDBIdx > 0)
	{
		// magi82 - Titan(070910) 타이탄 무공업데이트
		if(IsAddMugong(mugongBase->Position))// && mugongBase->wIconIdx < 10000)
		{
			CMugongBase * mugong = MUGONGMGR->NewMugongIcon(mugongBase);

			AddMugong(mugongBase->Position, mugong);
		}
		//else if(mugongBase->wIconIdx > 10000)
		//{
		//	// 타이탄 스킬은 등록만 할뿐 무공창에 아이콘을 생성하지 않는다.
		//	MUGONGMGR->NewMugongIcon(mugongBase);
		//}
	}
}

BOOL CMugongDialog::IsAddMugong(POSTYPE pos)
{
	MLOC wloc = GetWindowLocate(pos);
	switch(wloc)
	{
	case MUGONGWINDOW_NORMAL:
#ifdef _JAPAN_LOCAL_
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[wloc].IaAddable(pos);

#elif defined _HK_LOCAL_
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[wloc].IaAddable(pos);

#elif defined _TL_LOCAL_
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[wloc].IaAddable(pos);

#else
	case MUGONGWINDOW_EXTEND:
		{
			return m_pMugongRect[wloc].IaAddable(pos);
#endif
		}
		break;
	case MUGONGWINDOW_JINBUB:
		{
			return m_pMugongRect[wloc].IaAddable(pos);
		}
		break;
		// magi82 - Titan(070910) 타이탄 무공업데이트
	case MUGONGWINDOW_TITAN:
		{
			return m_pMugongRect[wloc].IaAddable(pos);
		}
		break;
	default:
		ASSERT(0);
	}

	return FALSE;
}

void CMugongDialog::AddMugong(POSTYPE pos/*abs pos*/, CMugongBase * mugong)
{
	ASSERT(mugong);
	
	MLOC loc = GetWindowLocate(pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
		{
			m_pMugongRect[loc].AddMugong(pos,mugong);
		}
		break;
	case MUGONGWINDOW_TITAN:
		{
			m_pMugongRect[loc].AddMugong(pos,mugong);
		}
		break;
	default:
		ASSERT(0);
	}
}

MLOC CMugongDialog::GetWindowLocate(POSTYPE abs_pos/*abs pos*/)
{
#ifdef _JAPAN_LOCAL_
	if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return MUGONGWINDOW_NORMAL;
	}
#elif defined _HK_LOCAL_
	if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return MUGONGWINDOW_NORMAL;
	}
#elif defined _TL_LOCAL_
	if(TP_MUGONG_START <= abs_pos && abs_pos < TP_MUGONG_END)
	{
		return MUGONGWINDOW_NORMAL;
	}
#else
	if(TP_MUGONG1_START <= abs_pos && abs_pos < TP_MUGONG1_END)
	{
		return MUGONGWINDOW_NORMAL;
	}
	else if(TP_MUGONG2_START <= abs_pos && abs_pos < TP_MUGONG2_END)
	{
		return MUGONGWINDOW_EXTEND;
	}
#endif // _JAPAN_LOCAL_
	else if(TP_JINBUB_START <= abs_pos && abs_pos < TP_JINBUB_END)
	{
		return MUGONGWINDOW_JINBUB;
	}
	// magi82 - Titan(070910) 타이탄 무공업데이트
	else if(TP_TITANMUGONG_START <= abs_pos && abs_pos < TP_TITANMUGONG_END)
	{
		return MUGONGWINDOW_TITAN;
	}
	
	return MUGONGWINDOW_MAX;
}
BOOL CMugongDialog::DeleteMugong(POSTYPE pos/*abs pos*/, CMugongBase ** mugong)
{
	ASSERT(mugong);
	
	MLOC loc = GetWindowLocate(pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
		{
			return m_pMugongRect[loc].DeleteMugong(pos,mugong);
		}
		break;
		// magi82 - Titan(070910) 타이탄 무공업데이트
	case MUGONGWINDOW_TITAN:
		{
			return m_pMugongRect[loc].DeleteMugong(pos,mugong);
		}
		break;
	default:
		ASSERT(0);
	}
	return FALSE;
}
BOOL CMugongDialog::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);

	if(icon->GetType() == WT_ITEM)
		FakeAddItem(mouseX, mouseY, (CItem *)icon);
	else if(icon->GetType() == WT_MUGONG || icon->GetType() == WT_JINBUB )
		return FakeMoveMugong(mouseX, mouseY, (CMugongBase *)icon);

	return FALSE;
}

void CMugongDialog::FakeAddItem(LONG mouseX, LONG mouseY, CItem * pItem)
{
	// 어떤 무공책이냐?무공, 내공, 경공
	if((pItem->GetItemKind() & eMUGONG_ITEM) == FALSE)
		return;
	BYTE type = GetItemKindDetail(pItem->GetItemIdx());
	switch(type)
	{
	case eMUGONGBOOK:
	case eJINBUBBOOK:
		break;
	default:
		return;
	}

	WORD ToPos=0;
	if(!GetPositionForXYRef(mouseX,mouseY, ToPos))		// 절대위치 넘어 옴
	{
		 if(!GetBlankPositionRef(type, ToPos))
			 return;
	}

	// 변환 가능한가?? 조건,제한 체크, 레벨, 직업, 성??
	if(!CanBeMoved(pItem, ToPos))
		return;

	GAMEIN->GetInventoryDialog()->UseItem(pItem);
}
BOOL CMugongDialog::FakeMoveMugong(LONG mouseX, LONG mouseY, CMugongBase * icon)
{
	WORD ToPos=0;
	// 절대위치 넘어 옴
#ifdef _JAPAN_LOCAL_
	int grid = 0;
	if( icon->GetPosition() >= TP_JINBUB_START && icon->GetPosition() < TP_JINBUB_END )
		grid = 1;
	if(!m_pMugongRect[grid].GetPositionForXYRef(mouseX,mouseY, ToPos))
		return FALSE;
#elif defined _HK_LOCAL_
	int grid = 0;
	if( icon->GetPosition() >= TP_JINBUB_START && icon->GetPosition() < TP_JINBUB_END )
		grid = 1;
	if(!m_pMugongRect[grid].GetPositionForXYRef(mouseX,mouseY, ToPos))
		return FALSE;
#elif defined _TL_LOCAL_
	int grid = 0;
	if( icon->GetPosition() >= TP_JINBUB_START && icon->GetPosition() < TP_JINBUB_END )
		grid = 1;
	if(!m_pMugongRect[grid].GetPositionForXYRef(mouseX,mouseY, ToPos))
		return FALSE;
#else
	if(!m_pMugongRect[m_CurSelectedGrid-MGI_MUGONG_PUSHUP1].GetPositionForXYRef(mouseX,mouseY, ToPos))
		return FALSE;
#endif


	if(!CanBeMoved(icon, ToPos))
		return FALSE;

	// 무공,내공,경공
	CMugongBase * pMugong = (CMugongBase *)icon;
	//------------------------------------
	MSG_MUGONG_MOVE_SYN msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_MOVE_SYN;
	msg.dwObjectID = HEROID;
	//------------------------------------
	msg.FromPos = pMugong->GetPosition();
	msg.ToPos = ToPos;

	if( msg.FromPos == msg.ToPos )
		return FALSE;
/*
	msg.dwMugongDBIdx	= pMugong->GetDBIdx();
	msg.wToPosition		= ToPos;									//절대 위치로 back
	msg.wFromPosition	= pMugong->GetPosition();					//절대 위치로 back
	
	if(msg.wFromPosition == msg.wToPosition)
		return ;
*/
	ASSERT(ToPos != 0);
	NETWORK->Send(&msg,sizeof(msg));
	SetDisable(TRUE);

	return TRUE;
}

void CMugongDialog::MoveMugong(POSTYPE ToPos/*abs pos*/, POSTYPE FromPos/*abs pos*/)
{
	CMugongBase * FromMugong = NULL;
	CMugongBase * ToMugong = NULL;
	DeleteMugong(FromPos, &FromMugong);
	DeleteMugong(ToPos, &ToMugong);

	if(FromMugong)
	{
		FromMugong->SetPosition(ToPos);
		AddMugong(ToPos, FromMugong);
	}
	
	if(ToMugong)
	{
		ToMugong->SetPosition(FromPos);
		AddMugong(FromPos, ToMugong);
	}
}

BOOL CMugongDialog::GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos)
{
	WORD position=0;
	for(int n=0;n<MUGONGWINDOW_MAX;++n)
	{
		if(m_pMugongRect[n].GetPositionForXYRef(x,y,pos) == TRUE)
			return TRUE;
	}
	
	return FALSE;
}

/*
void CMugongDialog::FakeDeleteMugong(POSTYPE absPos)
{
	m_TempDeleteMugong = absPos;
}
*/

void CMugongDialog::FakeDeleteMugong(CMugongBase* pMugong)
{
	m_pTempDeleteMugong = pMugong;
	m_nDeleteMsgBoxCount = 3;
}

void CMugongDialog::SendDeleteMugong()
{	
	if( m_pTempDeleteMugong == NULL ) return;
	if( m_nDeleteMsgBoxCount == 3 )
	{
		CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(m_pTempDeleteMugong->GetItemIdx());	
		if( pSkillInfo == NULL )
			return;

		WINDOWMGR->MsgBox(MBI_MUGONGDELETE2, MBT_YESNO, CHATMGR->GetChatMsg(1761), pSkillInfo->GetSkillName() );
        m_nDeleteMsgBoxCount = 2;
		return;
	}
	else if( m_nDeleteMsgBoxCount == 2 ) 
	{
		WINDOWMGR->MsgBox(MBI_MUGONGDELETE3, MBT_YESNO, CHATMGR->GetChatMsg(1762));
		m_nDeleteMsgBoxCount = 0;
		return;
	}

	MSG_MUGONG_REM_SYN msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_REM_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos = m_pTempDeleteMugong->GetPosition();
	msg.wMugongIdx = m_pTempDeleteMugong->GetItemIdx();

	NETWORK->Send(&msg,sizeof(msg));
	m_pTempDeleteMugong = NULL;
	SetDisable(TRUE);
}



CMugongBase * CMugongDialog::GetMugongAbs(POSTYPE abs_Pos)
{
	//*mugongOut = *(CMugongBase * )GetIconForIdx(relPos);
	MLOC loc = GetWindowLocate(abs_Pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
	case MUGONGWINDOW_TITAN:	// magi82 - Titan(070911) 타이탄 무공업데이트
		return m_pMugongRect[loc].GetMugong(abs_Pos);
		break;
	default:
		{
			ASSERT(0);
		}
	}
	return NULL;
}

void CMugongDialog::SetMugongSung(POSTYPE abspos, BYTE sung)
{
	MLOC loc = GetWindowLocate(abspos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
	case MUGONGWINDOW_TITAN:
		m_pMugongRect[loc].SetMugongSung(abspos,sung);
		break;
	default:
		{
			ASSERT(0);
		}
	}
}

void CMugongDialog::SetMugongExpPoint(POSTYPE abspos, DWORD expPoint)
{
	MLOC loc = GetWindowLocate(abspos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
	case MUGONGWINDOW_TITAN:
		m_pMugongRect[loc].SetMugongExpPoint(abspos,expPoint);
		break;
	default:
		{
			ASSERT(0);
		}
	}
	MUGONGMGR->RefreshMugong();
}

BOOL CMugongDialog::CanBeMoved(cIcon * pIcon, POSTYPE abs_pos)
{
	ASSERT(pIcon);	
	MLOC loc = GetWindowLocate(abs_pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifdef _JAPAN_LOCAL_			
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = abs_pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);
#elif defined _HK_LOCAL_			
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = abs_pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);
#elif defined _TL_LOCAL_			
		{
			DWORD Slot = HERO->GetExtraMugongSlot();			
			DWORD cp = abs_pos - TP_MUGONG_START;
			if( cp >= GIVEN_MUGONG_SLOT + MUGONG_SLOT_ADDCOUNT*Slot )
				return FALSE;
			else
				return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);

#else
	case MUGONGWINDOW_EXTEND:
		{
			return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);
#endif	
		}
		break;
	case MUGONGWINDOW_JINBUB:
		{
			return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);
		}
		break;
		// magi82 - Titan(070910) 타이탄 무공업데이트
	case MUGONGWINDOW_TITAN:
		{
			return m_pMugongRect[loc].CanBeMoved(pIcon,abs_pos);
		}
		break;
	default:
		ASSERT(0);
	}
	return FALSE;
}

void CMugongDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK || we == WE_LBTNDBLCLICK)
	{
		MLOC loc;
		BYTE type2 = 0;
		switch(lId)
		{
#ifdef _JAPAN_LOCAL_
		case MGI_MUGONG_GRID:
			{
				loc = MUGONGWINDOW_NORMAL;
				type2 = eMUGONGBOOK;
			}
			break;
#elif defined _HK_LOCAL_
		case MGI_MUGONG_GRID:
			{
				loc = MUGONGWINDOW_NORMAL;
				type2 = eMUGONGBOOK;
			}
			break;
#elif defined _TL_LOCAL_
		case MGI_MUGONG_GRID:
			{
				loc = MUGONGWINDOW_NORMAL;
				type2 = eMUGONGBOOK;
			}
			break;
#else
		case MGI_MUGONG1_GRID1:
			{
				loc = MUGONGWINDOW_NORMAL;
				type2 = eMUGONGBOOK;
			}
			break;
		case MGI_MUGONG1_GRID2:
			{
				loc = MUGONGWINDOW_EXTEND;
				type2 = eMUGONGBOOK;
			}
			break;
#endif
		case MGI_JINBUB_GRID:
			{
				loc = MUGONGWINDOW_JINBUB;
				type2 = eJINBUBBOOK;
			}
			break;
			// magi82 - Titan(070910) 타이탄 무공업데이트
		case MGI_TITAN_GRID:
			{
				loc = MUGONGWINDOW_TITAN;
				type2 = eMUGONGBOOK;
			}
			break;
		default:
			{
				return;
			}
		}
		
		POSTYPE pos;
		int mouseX = MOUSE->GetMouseX();
		int mouseY = MOUSE->GetMouseY();
#ifdef _JAPAN_LOCAL_
		if(!m_pMugongRect[loc].GetPositionForXYRef(mouseX,mouseY, pos))
#elif defined _HK_LOCAL_
		if(!m_pMugongRect[loc].GetPositionForXYRef(mouseX,mouseY, pos))
#elif defined _TL_LOCAL_
		if(!m_pMugongRect[loc].GetPositionForXYRef(mouseX,mouseY, pos))
#else
		if(!m_pMugongRect[loc].GetPositionForXYRef(mouseX,mouseY, pos))
#endif
			return;

		// magi82 - 무공창 락 안되는거 수정
		CMugongBase* pMugongBase = m_pMugongRect[loc].GetMugong(pos);
		if(pMugongBase && pMugongBase->IsLocked())
			return;

		if(we == WE_LBTNCLICK)
			SelectMugong(pos);
		else if(we == WE_LBTNDBLCLICK)
			ExcuteMugong(pos);
	}
	
}

void CMugongDialog::ExcuteMugong(POSTYPE pos)
{
	MLOC loc = GetWindowLocate(pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_TITAN:	// magi82 - Titan(070910) 타이탄 무공업데이트
	case MUGONGWINDOW_JINBUB:
		{
			if( HERO->GetCharacterTotalInfo()->bVisible == FALSE ) return;	//안보일땐 무공못쓴다.
			
			CActionTarget Target;
			WORD SkillInfoIdx;
			CMugongBase* pMugongBase = m_pMugongRect[loc].GetMugong(pos); 
			if( !pMugongBase )		return;

			SkillInfoIdx = pMugongBase->GetItemIdx();			
			CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(SkillInfoIdx);
			if( !pInfo )		return;

			int SkillLevel = HERO->GetMugongLevel(pInfo->GetSkillIndex());

			if( !SkillLevel )
				return;

			if( HERO->InTitan() )
			{
				// 타이탄 무공 정보로 세팅
				pInfo = SKILLMGR->GetSkillInfo(pInfo->GetSkillIndex());
			}

			//////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 무공 변환 추가
			WORD SkillOptionIndex = HERO->GetSkillOptionIndex(pInfo->GetSkillIndex());
			SKILLOPTION* pSkillOption = NULL;

			if(SkillOptionIndex)
				pSkillOption = SKILLMGR->GetSkillOption(SkillOptionIndex);
			//////////////////////////////////////////////////////////////////////////

			if(!pInfo->GetSpecialState())
			{
				if(HERO->GetSingleSpecialState(eSingleSpecialState_Hide))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
			}

			if(pInfo->IsExcutableSkillState(HERO,SkillLevel,pSkillOption) == FALSE)
			{
				HERO->SetCurComboNum(0);
				return;
			}
				
			if(pInfo->GetSkillInfo()->TargetKind == 0)			// 상대
			{
 				CObject * targetObj = OBJECTMGR->GetSelectedObject();
				
				if( targetObj )
				{
					if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
						return;
					
					Target.InitActionTarget(targetObj,NULL);
					HERO->EnableAutoAttack(&Target);
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );
					return;
				}
			}
			else if(pInfo->GetSkillInfo()->TargetKind == 1)		// 자신
			{
				Target.InitActionTarget(HERO,NULL);		// 자신을 타겟으로
			}
			
			SKILLMGR->SetSkill(pInfo);	// 스킬 매니저에 사용 무공을 등록한다
										// 퀵 매니저에서 선택한 무공 정보를 가지면서
										// 무공창에서 무공 사용시 단축창에서 사용했던 
										// 무공이 사용되는 버그 수정용
			SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );
		}
		break;
		
	default:
		ASSERT(0);
	}
	
}

void CMugongDialog::SelectMugong(POSTYPE pos)
{
	MLOC loc = GetWindowLocate(pos);
	switch(loc)
	{
	case MUGONGWINDOW_NORMAL:
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
	case MUGONGWINDOW_EXTEND:
#endif
#endif
#endif
	case MUGONGWINDOW_JINBUB:
		{
			m_pMugongRect[loc].SelectMugong(pos);
		}
		break;
		// magi82 - Titan(070910) 타이탄 무공업데이트
	case MUGONGWINDOW_TITAN:
		{
            m_pMugongRect[loc].SelectMugong(pos);
		}
		break;
	default:
		ASSERT(0);
	}

}


#ifdef _JAPAN_LOCAL_

void CMugongDialog::Linking_JP()
{
 	m_pMugongRect[MUGONGWINDOW_NORMAL].Init(MUGONGTYPE_NORMAL,
	 //		MUGONG_WEAREDPOSITION,MUGONG_STARTPOSITION,MUGONG_ENDPOSITION,
 		TP_MUGONG_START,TP_MUGONG_END,
 		this,
 		MGI_MUGONG_WEAR,
 		MGI_MUGONG_GRID,
 		MGI_MUGONG_NAME_STATIC,
 		MGI_MUGONG_LEVEL_STATIC,
 		MGI_MUGONG_EXP_STATIC,
 		MGI_MUGONG_EXP_GUAGE);
 			
 	((cIconDialog*)GetWindowForID(MGI_MUGONG_WEAR))->SetDragOverIconType( WT_MUGONG );
 	((cIconGridDialog*)GetWindowForID(MGI_MUGONG_GRID))->SetDragOverIconType( WT_MUGONG );

 	m_pMugongRect[MUGONGWINDOW_JINBUB].Init(MUGONGTYPE_JINBUB,
	 //		JINBUB_WEAREDPOSITION,JINBUB_STARTPOSITION,JINBUB_ENDPOSITION,
 		TP_JINBUB_START,TP_JINBUB_END,
 		this,
 		MGI_JINBUB_WEAR,
 		MGI_JINBUB_GRID,
 		MGI_JINBUB_NAME_STATIC,
 		MGI_JINBUB_LEVEL_STATIC,
 		MGI_JINBUB_EXP_STATIC,
 		MGI_JINBUB_EXP_GUAGE);
 
 	// Wear도 JINBUB으로 바꿔줘야 하는가?
 	((cIconDialog*)GetWindowForID(MGI_JINBUB_WEAR))->SetDragOverIconType( WT_JINBUB );
   	((cIconGridDialog*)GetWindowForID(MGI_JINBUB_GRID))->SetDragOverIconType( WT_JINBUB );
}

#elif defined _HK_LOCAL_

 void CMugongDialog::Linking_JP()
{
 	m_pMugongRect[MUGONGWINDOW_NORMAL].Init(MUGONGTYPE_NORMAL,
	 //		MUGONG_WEAREDPOSITION,MUGONG_STARTPOSITION,MUGONG_ENDPOSITION,
 		TP_MUGONG_START,TP_MUGONG_END,
 		this,
 		MGI_MUGONG_WEAR,
 		MGI_MUGONG_GRID,
 		MGI_MUGONG_NAME_STATIC,
 		MGI_MUGONG_LEVEL_STATIC,
 		MGI_MUGONG_EXP_STATIC,
 		MGI_MUGONG_EXP_GUAGE);
 			
 	((cIconDialog*)GetWindowForID(MGI_MUGONG_WEAR))->SetDragOverIconType( WT_MUGONG );
 	((cIconGridDialog*)GetWindowForID(MGI_MUGONG_GRID))->SetDragOverIconType( WT_MUGONG );

 	m_pMugongRect[MUGONGWINDOW_JINBUB].Init(MUGONGTYPE_JINBUB,
	 //		JINBUB_WEAREDPOSITION,JINBUB_STARTPOSITION,JINBUB_ENDPOSITION,
 		TP_JINBUB_START,TP_JINBUB_END,
 		this,
 		MGI_JINBUB_WEAR,
 		MGI_JINBUB_GRID,
 		MGI_JINBUB_NAME_STATIC,
 		MGI_JINBUB_LEVEL_STATIC,
 		MGI_JINBUB_EXP_STATIC,
 		MGI_JINBUB_EXP_GUAGE);
 
 	// Wear도 JINBUB으로 바꿔줘야 하는가?
 	((cIconDialog*)GetWindowForID(MGI_JINBUB_WEAR))->SetDragOverIconType( WT_JINBUB );
   	((cIconGridDialog*)GetWindowForID(MGI_JINBUB_GRID))->SetDragOverIconType( WT_JINBUB );
}
#elif defined _TL_LOCAL_

 void CMugongDialog::Linking_JP()
{
 	m_pMugongRect[MUGONGWINDOW_NORMAL].Init(MUGONGTYPE_NORMAL,
	 //		MUGONG_WEAREDPOSITION,MUGONG_STARTPOSITION,MUGONG_ENDPOSITION,
 		TP_MUGONG_START,TP_MUGONG_END,
 		this,
 		MGI_MUGONG_WEAR,
 		MGI_MUGONG_GRID,
 		MGI_MUGONG_NAME_STATIC,
 		MGI_MUGONG_LEVEL_STATIC,
 		MGI_MUGONG_EXP_STATIC,
 		MGI_MUGONG_EXP_GUAGE);
 			
 	((cIconDialog*)GetWindowForID(MGI_MUGONG_WEAR))->SetDragOverIconType( WT_MUGONG );
 	((cIconGridDialog*)GetWindowForID(MGI_MUGONG_GRID))->SetDragOverIconType( WT_MUGONG );

 	m_pMugongRect[MUGONGWINDOW_JINBUB].Init(MUGONGTYPE_JINBUB,
	 //		JINBUB_WEAREDPOSITION,JINBUB_STARTPOSITION,JINBUB_ENDPOSITION,
 		TP_JINBUB_START,TP_JINBUB_END,
 		this,
 		MGI_JINBUB_WEAR,
 		MGI_JINBUB_GRID,
 		MGI_JINBUB_NAME_STATIC,
 		MGI_JINBUB_LEVEL_STATIC,
 		MGI_JINBUB_EXP_STATIC,
 		MGI_JINBUB_EXP_GUAGE);
 
 	// Wear도 JINBUB으로 바꿔줘야 하는가?
 	((cIconDialog*)GetWindowForID(MGI_JINBUB_WEAR))->SetDragOverIconType( WT_JINBUB );
   	((cIconGridDialog*)GetWindowForID(MGI_JINBUB_GRID))->SetDragOverIconType( WT_JINBUB );
}
#else

void CMugongDialog::Linking()
{
	m_pMugongRectBtn[MUGONGWINDOW_NORMAL] = (cPushupButton*)GetWindowForID(MGI_MUGONG_PUSHUP1);
	m_pMugongRectBtn[MUGONGWINDOW_EXTEND] = (cPushupButton*)GetWindowForID(MGI_MUGONG_PUSHUP2);
	m_pMugongRectBtn[MUGONGWINDOW_JINBUB] = (cPushupButton*)GetWindowForID(MGI_JINBUB_PUSHUP3);
	m_pMugongRectBtn[MUGONGWINDOW_TITAN] = (cPushupButton*)GetWindowForID(MGI_MUGONG_TITAN_PUSHUP);	// magi82 - Titan(070910) 타이탄 무공업데이트

	m_pMugongRect[MUGONGWINDOW_NORMAL].Init(MUGONGTYPE_NORMAL,TP_MUGONG1_START,TP_MUGONG1_END, this, MGI_MUGONG1_GRID1);
	m_pMugongRect[MUGONGWINDOW_EXTEND].Init(MUGONGTYPE_NORMAL,TP_MUGONG2_START,TP_MUGONG2_END, this, MGI_MUGONG1_GRID2);
	m_pMugongRect[MUGONGWINDOW_JINBUB].Init(MUGONGTYPE_NORMAL,TP_JINBUB_START,TP_JINBUB_END, this, MGI_JINBUB_GRID);
	// magi82 - Titan(070910) 타이탄 무공업데이트
	m_pMugongRect[MUGONGWINDOW_TITAN].Init(MUGONGTYPE_NORMAL,TP_TITANMUGONG_START,TP_TITANMUGONG_END, this, MGI_TITAN_GRID);
	
	((cIconDialog*)GetWindowForID(MGI_MUGONGJINBUB_WEAR))->SetDragOverIconType( WT_MUGONG );
	((cIconGridDialog*)GetWindowForID(MGI_MUGONG1_GRID1))->SetDragOverIconType( WT_MUGONG );
	((cIconGridDialog*)GetWindowForID(MGI_MUGONG1_GRID2))->SetDragOverIconType( WT_MUGONG );
	((cIconGridDialog*)GetWindowForID(MGI_TITAN_GRID))->SetDragOverIconType( WT_MUGONG );

	// Wear도 JINBUB으로 바꿔줘야 하는가?
	((cIconGridDialog*)GetWindowForID(MGI_JINBUB_GRID))->SetDragOverIconType( WT_JINBUB );

	//
	m_pWearedIconDlg = (cIconDialog*)GetWindowForID(MGI_MUGONGJINBUB_WEAR);
	m_pMugongInfo[MGI_NAME] = (cStatic*)GetWindowForID(MGI_MUGONGJINBUB_NAME_STATIC);
	m_pMugongInfo[MGI_SUNG] = (cStatic*)GetWindowForID(MGI_MUGONGJINBUB_LEVEL_STATIC);
	m_pMugongInfo[MGI_EXPPOINT] = (cStatic*)GetWindowForID(MGI_MUGONGJINBUB_EXP_STATIC);
	m_pExpGuage = (CObjectGuagen*)GetWindowForID(MGI_MUGONGJINBUB_EXP_GUAGE);

	m_pMugongRect[MUGONGWINDOW_NORMAL].GridActive( TRUE );
	m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->SetPush( TRUE );
	m_pMugongRect[MUGONGWINDOW_EXTEND].GridActive( FALSE );
	m_pMugongRectBtn[MUGONGWINDOW_EXTEND]->SetActive( FALSE );
	m_pMugongRect[MUGONGWINDOW_JINBUB].GridActive( FALSE );
	m_pMugongRectBtn[MUGONGWINDOW_JINBUB]->SetPush( FALSE );
	// magi82 - Titan(070910) 타이탄 무공업데이트
	m_pMugongRect[MUGONGWINDOW_TITAN].GridActive( FALSE );
	m_pMugongRectBtn[MUGONGWINDOW_TITAN]->SetPush( FALSE );
	m_CurSelectedGrid = m_PreSelectedGrid = MGI_MUGONG_PUSHUP1;

	m_bMugongGridExtend = FALSE;
	SetSkillInfoStatic(NULL);
}

DWORD CMugongDialog::ActionEvent(CMouse * mouseInfo)
{
	//아이콘 드래그중일때.. 다른 탭으로 이동해야한다.
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( WINDOWMGR->GetDragDlg()->GetType() == WT_MUGONG )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( DWORD i = 0 ; i < MUGONGWINDOW_JINBUB ; i++ )
			{
				m_pMugongRectBtn[i]->ActionEvent( mouseInfo );
				
				if( we & WE_MOUSEOVER )
				if( m_pMugongRectBtn[i]->IsActive() && m_pMugongRectBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
				{
					bMouseOver = TRUE;
					
					if( m_bPushTabWithMouseOver )
					{
						if( MGI_MUGONG_PUSHUP1+i != m_CurSelectedGrid )
							ActiveMogongGrid( MGI_MUGONG_PUSHUP1+i );
					}
					else
					{
						if( m_BtnPushstartTime == 0 )
							m_BtnPushstartTime = gCurTime;
						else if( gCurTime - m_BtnPushstartTime > 500 )
							m_bPushTabWithMouseOver = TRUE;
					}				
				}
			}
			
			if( !bMouseOver )
				m_BtnPushstartTime = 0;
			
			//we |= m_pMugongRect[i].ActionEvent( mouseInfo );
			return we;
		}
	}

	m_BtnPushstartTime		= 0;
	m_bPushTabWithMouseOver = FALSE;
	return cIconDialog::ActionEvent( mouseInfo );	
}


void CMugongDialog::SetSkillInfoStatic(CMugongBase* pMugongBase)
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
	//2007. 7. 13. CBH - 전문기술 절초 관련 처리 부분 수정
	WORD wMugongIndex = pSkillInfo->GetSkillIndex();
	WORD Sung = 0;
	// 절초 전문기술이면.....
	if( (wMugongIndex == 6001) || (wMugongIndex == 6003) || (wMugongIndex == 6005) )
	{
		//전문기술 절초는 13성으로 셋팅했기 때문에 무공 다이얼로그의 비정상적인 처리를
		//막기위해 12성으로 강제 셋팅한다.
		Sung = 12;
	}
	else
	{
		Sung = pMugongBase->GetSung();
	}
	//////////////////////////////////////////////////////////////////////

	DWORD Exp = pMugongBase->GetExpPoint();

	m_pMugongInfo[MGI_NAME]->SetStaticText(pSkillInfo->GetSkillName());	
	m_pMugongInfo[MGI_SUNG]->SetStaticValue(Sung);
	
	if(Sung == 0)
	{
		m_pExpGuage->SetValue(0, 0);
		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText("");
	}
	else if(Sung < 12)
	{
		float percent = Exp * 100.0f / pSkillInfo->GetNeedExp(Sung);
		if( percent > 99.99f )
		{
			if( Exp != pSkillInfo->GetNeedExp(Sung)-1 )
				percent = 99.99f;
		}

//		sprintf(temp,"%4.2f%%",100*Exp/(float)pSkillInfo->GetNeedExp(Sung));
		sprintf(temp,"%4.2f%%",percent);
		m_pMugongInfo[MGI_EXPPOINT]->SetStaticText(temp);

#ifdef _CHEATENABLE_		
		if(CHEATMGR->IsCheatEnable())
		{
//			sprintf(temp,"%4.2f%%[%d/%d]",100*Exp/(float)pSkillInfo->GetNeedExp(Sung),Exp,pSkillInfo->GetNeedExp(Sung));
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


void CMugongDialog::ActiveMogongGrid( LONG lId )
{
	if( m_CurSelectedGrid == (DWORD)lId )
	{
		m_pMugongRectBtn[m_CurSelectedGrid-MGI_MUGONG_PUSHUP1]->SetPush( TRUE );
		return;
	}

	BOOL bActiveGrid = FALSE;
	for( DWORD i=0; i<MUGONGWINDOW_MAX; ++i )
	{
		if( lId == (LONG)(MGI_MUGONG_PUSHUP1+i) )
		{
			m_pMugongRectBtn[i]->SetPush( TRUE );
			m_pMugongRect[i].GridActive( TRUE );
			m_PreSelectedGrid = m_CurSelectedGrid;
			m_CurSelectedGrid = MGI_MUGONG_PUSHUP1+i;
			bActiveGrid = TRUE;
		}
		else
		{
			m_pMugongRectBtn[i]->SetPush( FALSE );
			m_pMugongRect[i].GridActive( FALSE );
		}
	}

	if( bActiveGrid == FALSE )
	{
		m_pMugongRectBtn[MUGONGWINDOW_NORMAL]->SetPush( TRUE );
		m_pMugongRect[MUGONGWINDOW_NORMAL].GridActive( TRUE );
		m_PreSelectedGrid = MGI_MUGONG_PUSHUP1;
		m_CurSelectedGrid = MGI_MUGONG_PUSHUP1;
	}
	

	SetSkillInfoStatic( NULL );
}

#endif //_JAPAN_LOCAL_
