// QuickDialog.cpp: implementation of the CQuickDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickDialog.h"
#include "ObjectManager.h"
#include "MugongManager.h"

#include "QuickItem.h"
#include "QuickManager.h"
#include "GameIn.h"

#include "ItemManager.h"

#include "SkillManager_client.h"
#include "WindowIDEnum.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cButton.h"
#include "./Interface/cFont.h"

#include "InventoryExDialog.h"
#include "./Input/Mouse.h"

#include "ChatManager.h"
#include "MacroManager.h"
#define		CELLWIDTH				40
#define		CELLBORDER				5
#define		KEYSTROKE_DELAYTIME		40

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuickDialog::CQuickDialog()
{
	m_type			= WT_QUICKDIALOG;
	m_selectedPage	= 0;
	m_keyStrokeBit	= 0;
	SCRIPTMGR->GetImage( 47, &m_PressedImage  );
//	for( int i = 0 ; i < MAX_QUICKITEMPERSHEET_NUM ; ++i )
	for( int i = 0 ; i < TABCELL_QUICK_NUM ; ++i )
	{
		m_keyStrokeDTick[i] = KEYSTROKE_DELAYTIME;
	}
	
}

CQuickDialog::~CQuickDialog()
{

}
void CQuickDialog::Add(cWindow * window)
{
	if(window->GetID() == QI_PAGESHEET1)
	{
		window->SetActive((m_selectedPage == 0?TRUE:FALSE));
		m_pIconGridSheet[0] = (cIconGridDialog *)window;
	}
	else if(window->GetID() == QI_PAGESHEET2)
	{
		window->SetActive((m_selectedPage == 1?TRUE:FALSE));
		m_pIconGridSheet[1] = (cIconGridDialog *)window;
	}
	else if(window->GetID() == QI_PAGESHEET3)
	{
		window->SetActive((m_selectedPage == 2?TRUE:FALSE));
		m_pIconGridSheet[2] = (cIconGridDialog *)window;
	}
	else if(window->GetID() == QI_PAGESHEET4)
	{
		window->SetActive((m_selectedPage == 3?TRUE:FALSE));
		m_pIconGridSheet[3] = (cIconGridDialog *)window;
	}

	cDialog::Add(window);
}
void CQuickDialog::ViewSheet()
{
//	for(int i = 0 ; i < MAX_QUICKSHEET_NUM ; i++)
	for(int i = 0 ; i < TAB_QUICK_NUM ; i++)
	{
		if(i == m_selectedPage)
		{
			m_pIconGridSheet[i]->SetActive(TRUE);
		}
		else
		{
			m_pIconGridSheet[i]->SetActive(FALSE);
		}
			
	}
}

void CQuickDialog::Render()
{
	if(!m_bActive) return;

	cDialog::RenderWindow();

	// 페이지 번호 출력 and sheet숨기기
	cButton * win = (cButton * )this->GetWindowForID(QI_SHAPEBTN);
	win->SetTextValue(m_selectedPage+1);
	
	// Cell Number Render, Pressed Image
	static VECTOR2 Pos;
	Pos.x = m_absPos.x + 3;
	Pos.y = m_absPos.y + 36;
//	for( int i = 0 ; i < MAX_QUICKITEMPERSHEET_NUM ; ++i )
	for( int i = 0 ; i < TABCELL_QUICK_NUM ; ++i )
	{
		// image
		//int bits = (int)POW( 2, i - 1 );
		int bits = 1 << i;
		if( ( m_keyStrokeBit & bits ) )
		{
			m_PressedImage.RenderSprite( NULL, NULL, 0, &Pos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );	 
			
			//if( --m_keyStrokeDTick[ i - 1 ] == 0 )
			if( gCurTime - m_keyStrokeDTick[i] > KEYSTROKE_DELAYTIME)
			{
				m_keyStrokeDTick[i] = KEYSTROKE_DELAYTIME;
				m_keyStrokeBit &= ~bits;
			}
		}
		Pos.y += CELLWIDTH+CELLBORDER;		
	}

	cDialog::RenderComponent();

	RECT rect		= {(LONG)m_absPos.x+3, (LONG)m_absPos.y+38, 1, 1 };
	RECT rectShadow = {(LONG)m_absPos.x+4, (LONG)m_absPos.y+39, 1, 1 };
//	for( i = 0 ; i < MAX_QUICKITEMPERSHEET_NUM ; ++i )
	for( i = 0 ; i < TABCELL_QUICK_NUM ; ++i )
	{
		// text
		static char nums[4];
		if( !MACROMGR->IsChatMode() )
			sprintf( nums, "%d", (i+1)%10 );
		else
			sprintf( nums, "F%d", (i+1) );
//		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rect, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rectShadow, RGB_HALF(10, 10, 10) );
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rect, RGB_HALF(220, 220, 220) );
		rect.top += CELLWIDTH+CELLBORDER;
		rectShadow.top += CELLWIDTH+CELLBORDER;
	}
}


DWORD CQuickDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);

	if( we & WE_LBTNDBLCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();
		POSTYPE pos;
		if( m_pIconGridSheet[m_selectedPage]->GetPositionForXYRef( x, y, pos ) )
			QUICKMGR->UseQuickItem( pos );
	}

	return we;
}


void CQuickDialog::OnSelectAction( POSTYPE pos )
{
	m_keyStrokeBit |= 1 << pos;
	m_keyStrokeDTick[pos] = gCurTime;
}

void CQuickDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we == WE_BTNCLICK && lId == QI_PREVPAGEBTN)
	{
		int page = (int)(GetSelectedPageNumber()-1);
		if( page < 0 )
			page = TAB_QUICK_NUM-1;

		SetSelectedPageNumber((WORD)page);
		ViewSheet();
	}
	else if( we == WE_BTNCLICK && lId == QI_NEXTPAGEBTN)
	{
		WORD page = GetSelectedPageNumber()+1;
//		if(page >= MAX_QUICKSHEET_NUM)
//			page = MAX_QUICKSHEET_NUM-1;
		if(page >= TAB_QUICK_NUM)
			page = 0;
		SetSelectedPageNumber(page);
		ViewSheet();
	}
}

void CQuickDialog::SelectPageDirect( WORD wPage )
{
	if( wPage >= TAB_QUICK_NUM )	return;

	SetSelectedPageNumber(wPage);
	ViewSheet();
}
// 06. 01 이영준 - 단축창 변경
/* 기존 위치 연산 함수 모두 변경
//-----------------------------------------------------------------------------------
// data operation -------------------------------------------------------------------
CQuickItem * CQuickDialog::GetQuickItem(POSTYPE abs_pos)
{
	int page = GetSheetNumber(abs_pos);
	if(page == -1) return NULL;
	if(m_pIconGridSheet[page]->IsAddable(abs_pos - TP_QUICK_START - TABCELL_QUICK_NUM*page)) return NULL;
	return (CQuickItem *)m_pIconGridSheet[page]->GetIconForIdx(abs_pos - TP_QUICK_START-TABCELL_QUICK_NUM*page);
//	if(m_pIconGridSheet[page]->IsAddable(abs_pos - QUICK_STARTPOSITION - MAX_QUICKITEMPERSHEET_NUM*page)) return NULL;
//	return (CQuickItem *)m_pIconGridSheet[page]->GetIconForIdx(abs_pos - QUICK_STARTPOSITION-MAX_QUICKITEMPERSHEET_NUM*page);
}
// get absolute position from current activated grid sheet
BOOL CQuickDialog::GetAbsPositionForXYRef(LONG x, LONG y, POSTYPE & abs_pos)
{
	WORD position=0;
	if(m_pIconGridSheet[m_selectedPage]->GetPositionForXYRef(x,y,position))
	{
		abs_pos = position+TP_QUICK_START+TABCELL_QUICK_NUM*m_selectedPage;								// 절대 위치로 바꿈
//		abs_pos = position+QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*m_selectedPage;								// 절대 위치로 바꿈
		return TRUE;
	}
	return FALSE;
}
// if absolute position is valid, return true
BOOL CQuickDialog::ValidPos(POSTYPE abs_pos)
{
//	if(QUICK_STARTPOSITION <= abs_pos  && abs_pos < QUICK_ENDPOSITION)
	if(TP_QUICK_START <= abs_pos  && abs_pos < TP_QUICK_END)
		return TRUE;
	return FALSE;
}
// get sheet number for absolute position
int CQuickDialog::GetSheetNumber(POSTYPE abs_pos)
{
//	for(int i = 0 ; i < MAX_QUICKSHEET_NUM ; i++)
	for(int i = 0 ; i < TABCELL_QUICK_NUM ; i++)
	{
//		if(QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*i <= abs_pos && abs_pos < (QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*(i+1)))
		if(TP_QUICK_START+TABCELL_QUICK_NUM*i <= abs_pos && abs_pos < (TP_QUICK_START+TABCELL_QUICK_NUM*(i+1)))
			return i;
	}
	return -1;
}
*/

CQuickItem * CQuickDialog::GetQuickItem(POSTYPE abs_pos)
{
	int page = GetSheetNumber(abs_pos);
	int pos = GetPositionNumber(abs_pos);

	if(page == -1) return NULL;

	if(m_pIconGridSheet[page]->IsAddable(pos)) return NULL;
	return (CQuickItem *)m_pIconGridSheet[page]->GetIconForIdx(pos);
}

BOOL CQuickDialog::GetAbsPositionForXYRef(LONG x, LONG y, POSTYPE & abs_pos)
{
	WORD position=0;
	if(m_pIconGridSheet[m_selectedPage]->GetPositionForXYRef(x,y,position))
	{
		abs_pos = position + TABCELL_QUICK_NUM * m_selectedPage;
		return TRUE;
	}
	return FALSE;
}

BOOL CQuickDialog::ValidPos(POSTYPE abs_pos)
{
	if(0 <= abs_pos  && abs_pos < 40)
		return TRUE;
	return FALSE;
}

int CQuickDialog::GetSheetNumber(POSTYPE abs_pos)
{
	int page = abs_pos / TABCELL_QUICK_NUM;

	if(page >= 0 && page < 4)
		return page;
	else
		return -1;
}

WORD CQuickDialog::GetPositionNumber(POSTYPE abs_pos)
{
	return abs_pos % TABCELL_QUICK_NUM;
}

// actually, add in grid
BOOL CQuickDialog::AddQuickItem(POSTYPE abs_pos, CQuickItem * pIcon)
{
	if(!ValidPos(abs_pos)) return FALSE;
	int sheetNum = GetSheetNumber(abs_pos);
	if(sheetNum == -1) return FALSE;

//	POSTYPE rel_pos = abs_pos - (QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*sheetNum);

	// 06. 01 이영준 - 단축창 변경
//	POSTYPE rel_pos = abs_pos - (TP_QUICK_START+TABCELL_QUICK_NUM*sheetNum);
	POSTYPE rel_pos = GetPositionNumber(abs_pos);
	if(!m_pIconGridSheet[sheetNum]->IsAddable(rel_pos)) return FALSE;
	
	SetToolTipIcon( pIcon );
	
	return m_pIconGridSheet[sheetNum]->AddIcon(rel_pos, (cIcon *)pIcon);
}/*

// actually, add in grid
BOOL CQuickDialog::AddQuickItem(POSTYPE abs_pos, ICONBASE * pIconInfo)
{
	if(!ValidPos(abs_pos)) return FALSE;
	int sheetNum = GetSheetNumber(abs_pos);
	if(sheetNum == -1) return FALSE;

	POSTYPE rel_pos = abs_pos - (QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*sheetNum);
	if(!m_pIconGridSheet[sheetNum]->IsAddable(rel_pos)) return FALSE;
	
	CQuickItem * item = QUICKMGR->NewQuickItem(pIconInfo);
	item->SetPosition(abs_pos);

	SetToolTipIcon( item );

	return m_pIconGridSheet[sheetNum]->AddIcon(rel_pos, (cIcon *)item);
}
*/

BOOL CQuickDialog::RemQuickItem(POSTYPE abs_pos, cIcon ** delItem)
{
	if(!ValidPos(abs_pos)) return FALSE;
	int sheetNum = GetSheetNumber(abs_pos);
	if(sheetNum == -1) return FALSE;

//	POSTYPE rel_pos = abs_pos - (QUICK_STARTPOSITION+MAX_QUICKITEMPERSHEET_NUM*sheetNum);

	// 06. 01 이영준 - 단축창 변경
//	POSTYPE rel_pos = abs_pos - (TP_QUICK_START+TABCELL_QUICK_NUM*sheetNum);
	POSTYPE rel_pos = GetPositionNumber(abs_pos);
	if(m_pIconGridSheet[sheetNum]->IsAddable(rel_pos)) return FALSE;

	if(!m_pIconGridSheet[sheetNum]->DeleteIcon(rel_pos, delItem))
		return FALSE;

	return TRUE;
}
BOOL CQuickDialog::MoveQuickItem(POSTYPE fromAbsPos, POSTYPE toAbsPos, bool DeleteToQuickPos)
{
	CQuickItem * fromItem = NULL;
	CQuickItem * toItem = NULL;

	//---------------------------------------------------
	// 06. 01 이영준 - 단축창 변경
	POSTYPE fromQuickPos = 0;
	POSTYPE toQuickPos = 0;

	WORD fromSheetNum = GetSheetNumber(fromAbsPos);
	WORD fromPosNum = GetPositionNumber(fromAbsPos);
	WORD toSheetNum = GetSheetNumber(toAbsPos);
	WORD toPosNum = GetPositionNumber(toAbsPos);
	//---------------------------------------------------

	if(!RemQuickItem(fromAbsPos, (cIcon **)&fromItem))
	{
		ASSERT(0);
		return FALSE;
	}
	WORD SheetNum = GetSheetNumber(toAbsPos);
	if( SheetNum != GetSheetNumber(fromAbsPos) )
	{
		for(WORD i = 0; i < TABCELL_QUICK_NUM; i++)
		{
			WORD pos = SheetNum * TABCELL_QUICK_NUM + i;

			CQuickItem * pQuickItem = GetQuickItem(pos);
			if(pQuickItem)
			if(pQuickItem->GetSrcIdx() == fromItem->GetSrcIdx())
			{
				CQuickItem* tempItem;
				RemQuickItem(pos, (cIcon **)&tempItem);
				QUICKMGR->ReleaseQuickItem(tempItem);
			}
		}
	}
	RemQuickItem(toAbsPos, (cIcon **)&toItem);

	//---------------------------------------------------
	// 퀵아이템의 원래 아이템의 퀵포지션값을 가져와서 재계산후 셋팅한다
	fromQuickPos = fromItem->GetItem()->GetQuickPosition();
	QUICKMGR->UpdateQuickPosition(fromQuickPos, toSheetNum, toPosNum + 1);
	fromItem->GetItem()->SetQuickPosition(fromQuickPos);
	//---------------------------------------------------
	
	fromItem->SetPosition(toAbsPos);
	if(!AddQuickItem(toAbsPos, fromItem))
	{
		return FALSE;
	}
	if(toItem)
	{
		if(DeleteToQuickPos)
		{
			QUICKMGR->ReleaseQuickItem(toItem);
		}
		else
		{
			//---------------------------------------------------
			// 퀵아이템의 원래 아이템의 퀵포지션값을 가져와서 재계산후 셋팅한다
			toQuickPos = toItem->GetItem()->GetQuickPosition();
			QUICKMGR->UpdateQuickPosition(toQuickPos, fromSheetNum, fromPosNum + 1);
			toItem->GetItem()->SetQuickPosition(toQuickPos);
			//---------------------------------------------------

			toItem->SetPosition(fromAbsPos);
			if(!AddQuickItem(fromAbsPos, toItem))
				return FALSE;
		}
	}
	return TRUE;
}


//----------------------------------------------------------------------------------------------
//--------------------- function associated FakeXXXXXX -----------------------------------------
// network msg only send, data manipulation is not operated
BOOL CQuickDialog::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	//whether move operation or add overation, Do check!
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
		return FALSE;
	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM )
		return FALSE;		//메시지 안찍을려구... 
	if(!QUICKMGR->CanEquip(icon))
	{
		return FALSE;
	}
	if(icon->GetType() == WT_QUICKITEM)
	{
		FakeMoveQuickItem(mouseX, mouseY, (CQuickItem *)icon);
	}
	else if(icon->GetType() == WT_ITEM)
		FakeAddItem(mouseX, mouseY, (CItem *)icon);
	else if(icon->GetType() == WT_MUGONG ||
			icon->GetType() == WT_JINBUB )
		FakeAddMugong(mouseX, mouseY, (CMugongBase *)icon);
	else if(icon->GetType() == WT_ABILITYICON)
		FakeAddAbility(mouseX, mouseY, (CAbilityIcon *)icon);
	return FALSE;
}

void CQuickDialog::FakeDeleteQuickItem( CQuickItem * pQuickItem )
{
	MSG_QUICK_REM_SYN msg;
	msg.Category	= MP_QUICK;
	msg.Protocol	= MP_QUICK_REM_SYN;
	msg.dwObjectID	= HEROID;
	msg.SrcPos		= pQuickItem->GetSrcPosition();
	msg.SrcItemIdx	= pQuickItem->GetSrcIdx();

	// 06. 01 이영준 - 단축창 변경
	msg.QuickPos	= pQuickItem->GetPosition();
	msg.SrcQuickPos = pQuickItem->GetItem()->GetQuickPosition();
	
	NETWORK->Send(&msg,sizeof(msg));
}
void CQuickDialog::FakeMoveQuickItem(LONG x, LONG y, CQuickItem * pItem)
{
	WORD ToAbsPos=0;
	if(!GetAbsPositionForXYRef(x,y,ToAbsPos)) return;

//	빈공간이 아니면 위치교환!!!
//	if(!m_pIconGridSheet[m_selectedPage]->IsAddable(ToAbsPos-QUICK_STARTPOSITION))
//		return;
	/*
	CQuickItem * pToQItem = GetQuickItem(ToAbsPos);

	//------------------------------------
	MSG_QUICK_MOVE_SYN msg;
	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_MOVE_SYN;
	msg.dwObjectID = HEROID;
	//------------------------------------
	msg.FromSrcPos			= pItem->GetSrcPosition();
	msg.wFromSrcItemIdx		= pItem->GetSrcIdx();
	msg.FromQuickPos		= pItem->GetPosition();
	msg.ToSrcPos			= (pToQItem?pToQItem->GetSrcPosition():0);
	msg.wToSrcItemIdx		= (pToQItem?pToQItem->GetSrcIdx():0);
	msg.ToQuickPos			= ToAbsPos;
		
	// 06. 01 이영준 - 단축창 변경
	msg.FromSrcQuickPos		= pItem->GetItem()->GetQuickPosition();
	msg.ToSrcQuickPos		= (pToQItem?pToQItem->GetItem()->GetQuickPosition():0);

	if(msg.FromQuickPos == msg.ToQuickPos)
		return ;

	NETWORK->Send(&msg,sizeof(msg));
	*/

	if(pItem->GetPosition() == ToAbsPos)
		return ;

	MSG_QUICK_MOVE_SYN msg;

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_MOVE_SYN;
	msg.dwObjectID = HEROID;
	msg.FromSrcPos			= pItem->GetSrcPosition();
	msg.wFromSrcItemIdx		= pItem->GetSrcIdx();
	msg.FromQuickPos		= pItem->GetPosition();
	msg.ToQuickPos			= ToAbsPos;
	msg.FromSrcQuickPos		= pItem->GetItem()->GetQuickPosition();

	CQuickItem * pToQItem = GetQuickItem(ToAbsPos);

	msg.ToSrcPos			= (pToQItem?pToQItem->GetSrcPosition():0);
	msg.wToSrcItemIdx		= (pToQItem?pToQItem->GetSrcIdx():0);
	msg.ToSrcQuickPos		= (pToQItem?pToQItem->GetItem()->GetQuickPosition():0);
	msg.DeleteToSrcQuickPos = false;

	if(pToQItem)
	{
		WORD SheetNum = GetSheetNumber(pItem->GetPosition());
		if( SheetNum != GetSheetNumber(ToAbsPos) )
		{
			for(WORD i = 0; i < TABCELL_QUICK_NUM; i++)
			{
				CQuickItem * pQuickItem = GetQuickItem(SheetNum * TABCELL_QUICK_NUM + i);
				if(pQuickItem)
				if(pQuickItem->GetSrcIdx() == pToQItem->GetSrcIdx())
				{
					msg.DeleteToSrcQuickPos = true;
				}
			}
		}
	}
	NETWORK->Send(&msg,sizeof(msg));
}


void CQuickDialog::FakeAddItem(LONG x, LONG y, CItem * pItem)
{
	WORD abs_pos;
	if(!GetAbsPositionForXYRef(x, y, abs_pos))
		return ;

	// 수량 아이템은 퀵포지션값을 하나에 몰아주자
	CItem* pItemInven = NULL;
	if(ITEMMGR->IsDupItem(pItem->GetItemIdx()))
	{
		pItemInven = GAMEIN->GetInventoryDialog()->GetItemLike(pItem->GetItemIdx());

		//SW060316 인벤해당아이템 있을때만..
		if(pItemInven)
			pItem = pItemInven;
	}

	WORD SheetNum = GetSheetNumber(abs_pos);
	
	if(!ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) && ITEMMGR->IsDupItem(pItem->GetItemIdx()))
	{
		// 06. 01 이영준 - 단축창 변경
//		for(POSTYPE i = TP_QUICK_START ; i < TP_QUICK_END ; ++i)
//		{
//			CQuickItem * pQuickItem = GetQuickItem(i);
		for(WORD i = 0; i < TABCELL_QUICK_NUM; i++)
		{
			CQuickItem * pQuickItem = GetQuickItem(SheetNum * TABCELL_QUICK_NUM + i);
			if(pQuickItem)
			if(pQuickItem->GetSrcIdx() == pItem->GetItemIdx())
			{
				FakeMoveIcon(x, y, pQuickItem);	//KES 040513
				return;
			}
		}
	}

	CQuickItem *pOldQuick = GetQuickItem(abs_pos);
	FakeAddItem(abs_pos, pItem, pOldQuick);
}
void CQuickDialog::FakeAddItem(POSTYPE abs_pos, CItem * pItem, CQuickItem * pOldQuick)
{
	if(pOldQuick)
	if(pOldQuick->GetSrcPosition() == pItem->GetPosition())
		return;
	
	if( !pItem )		return;
	if( pItem->IsLocked() || pItem->GetItemParam() & ITEM_PARAM_SEAL) return;
	
	if( pItem->GetItemKind() == eYOUNGYAK_ITEM )
	{
		if( !GAMEIN->GetInventoryDialog()->CheckItemLockForItemIdx(pItem->GetItemIdx()) )
			return;
	}

//	if(TP_MUNPAWAREHOUSE_START <=  pItem->GetItemBaseInfo()->Position && pItem->GetItemBaseInfo()->Position < TP_MUNPAWAREHOUSE_END )
//		return;
//	else if(TP_PYOGUK_START <= pItem->GetItemBaseInfo()->Position && pItem->GetItemBaseInfo()->Position < TP_PYOGUK_END)

	POSTYPE Pos = pItem->GetItemBaseInfo()->Position;
	if(TP_PYOGUK_START <= Pos && Pos < TP_PYOGUK_END)
		return;
	
	if(TP_PETINVEN_START <= Pos && Pos < TP_PETINVEN_END)
		return;

	MSG_QUICK_ADD_SYN msg;
	msg.Category		= MP_QUICK;
	msg.Protocol		= MP_QUICK_ADD_SYN;
	msg.dwObjectID		= HEROID;
	msg.QuickPos		= abs_pos;
	msg.SrcPos			= pItem->GetPosition();
	msg.SrcItemIdx		= pItem->GetItemIdx();
	msg.OldSrcPos		= (pOldQuick?pOldQuick->GetSrcPosition():0);
	msg.OldSrcItemIdx	= (pOldQuick?pOldQuick->GetSrcIdx():0);

	// 06. 01 이영준 - 단축창 변경
	msg.SrcQuickPos = pItem->GetQuickPosition();
	msg.OldSrcQuickPos = (pOldQuick?pOldQuick->GetItem()->GetQuickPosition():0);

	NETWORK->Send(&msg, sizeof(MSG_QUICK_ADD_SYN));

	//ITEM_LOCK
}
void CQuickDialog::FakeAddMugong(LONG x, LONG y, CMugongBase * pMugong)
{
	// 장착 무공 제한 없다. 모든 무공?

	WORD abs_pos;
	if(!GetAbsPositionForXYRef(x, y, abs_pos))
		return;
	WORD rel_pos;
	if(!m_pIconGridSheet[m_selectedPage]->GetPositionForXYRef(x,y,rel_pos))
		return;
//	if(!m_pIconGridSheet[m_selectedPage]->IsAddable(rel_pos))
//		return;
	if(pMugong->GetSung() == 0)
		return;

	CQuickItem* pOldQuick = GetQuickItem(abs_pos);
	if(pOldQuick)
	if(pOldQuick->GetSrcPosition() == pMugong->GetPosition())
		return;

	WORD SheetNum = GetSheetNumber(abs_pos);

	for(WORD i = 0; i < TABCELL_QUICK_NUM; i++)
	{
		CQuickItem * pQuickMugong = GetQuickItem(SheetNum * TABCELL_QUICK_NUM + i);
		if(pQuickMugong)
			if(pQuickMugong->GetSrcIdx() == pMugong->GetItemIdx())
			{
				FakeMoveIcon(x, y, pQuickMugong);	//KES 040513
				return;
			}
	}

	MSG_QUICK_ADD_SYN msg;
	msg.Category		= MP_QUICK;
	msg.Protocol		= MP_QUICK_ADD_SYN;
	msg.dwObjectID		= HEROID;
	msg.QuickPos		= abs_pos;
	msg.SrcPos			= pMugong->GetPosition();
	msg.SrcItemIdx		= pMugong->GetItemIdx();
	msg.OldSrcPos		= (pOldQuick?pOldQuick->GetSrcPosition():0);
	msg.OldSrcItemIdx	= (pOldQuick?pOldQuick->GetSrcIdx():0);
	
	// 06. 01 이영준 - 단축창 변경
	msg.SrcQuickPos = pMugong->GetQuickPosition();
	msg.OldSrcQuickPos = (pOldQuick?pOldQuick->GetItem()->GetQuickPosition():0);

	NETWORK->Send(&msg, sizeof(MSG_QUICK_ADD_SYN));
}

void CQuickDialog::FakeAddAbility(LONG x, LONG y, CAbilityIcon * pAbility)
{
	WORD abs_pos;
	if(!GetAbsPositionForXYRef(x, y, abs_pos))
		return;
	WORD rel_pos;
	if(!m_pIconGridSheet[m_selectedPage]->GetPositionForXYRef(x,y,rel_pos))
		return;
	CQuickItem* pOldQuick = GetQuickItem(abs_pos);
	if(pOldQuick)
	if(pOldQuick->GetSrcPosition() == pAbility->GetAbilityInfo()->GetPosition() )
		return;

	WORD SheetNum = GetSheetNumber(abs_pos);

	for(WORD i = 0; i < TABCELL_QUICK_NUM; i++)
	{
		CQuickItem * pQuickAbility = GetQuickItem(SheetNum * TABCELL_QUICK_NUM + i);
		if(pQuickAbility)
			if(pQuickAbility->GetSrcIdx() == pAbility->GetItemIdx())
			{
				FakeMoveIcon(x, y, pQuickAbility);	//KES 040513
				return;
			}
	}

	MSG_QUICK_ADD_SYN msg;
	msg.Category		= MP_QUICK;
	msg.Protocol		= MP_QUICK_ADD_SYN;
	msg.dwObjectID		= HEROID;
	msg.QuickPos		= abs_pos;
//	msg.SrcPos			= pAbility->GetAbilityInfo()->GetPosition();
	msg.SrcPos			= pAbility->GetAbilityInfo()->GetAbsPosition();
	msg.SrcItemIdx		= pAbility->GetAbilityInfo()->GetIdx() + MIN_ABILITY_INDEX;
	msg.OldSrcPos		= (pOldQuick?pOldQuick->GetSrcPosition():0);
	msg.OldSrcItemIdx	= (pOldQuick?pOldQuick->GetSrcIdx():0);

	// 06. 01 이영준 - 단축창 변경
	msg.SrcQuickPos = pAbility->GetQuickPosition();
	msg.OldSrcQuickPos = (pOldQuick?pOldQuick->GetItem()->GetQuickPosition():0);

	NETWORK->Send(&msg, sizeof(MSG_QUICK_ADD_SYN));

}

void CQuickDialog::RefreshIcon()
{
//	for( int i = 0 ; i < MAX_QUICKSHEET_NUM ; ++i )
	for( int i = 0 ; i < TAB_QUICK_NUM ; ++i )
	{
//		for( int j = 0 ; j < MAX_QUICKITEMPERSHEET_NUM ; ++j )
		for( int j = 0 ; j < TABCELL_QUICK_NUM ; ++j )
		{
			SetToolTipIcon( (CQuickItem*)m_pIconGridSheet[i]->GetIconForIdx(j) );			
		}
	}
}

void CQuickDialog::RefreshIcon( POSTYPE pos )
{
	SetToolTipIcon( GetQuickItem( pos ) );
}

void CQuickDialog::SetToolTipIcon( CQuickItem * item )
{
	if( item == NULL )	return;
	
	char ToolTipText[256] = { 0, };
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	item->SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	
	ITEM_INFO*	pInfo		= ITEMMGR->GetItemInfo( item->GetSrcIdx() );
	
	if( pInfo )
	{
		wsprintf(ToolTipText, "[%s]", pInfo->ItemName );
		item->AddToolTipLine( ToolTipText, TTTC_DEFAULT );

		//영약의 경우 회복력
		if( pInfo->ItemKind == eYOUNGYAK_ITEM )
		{
			if( pInfo->GenGol != 0)
			{
				wsprintf(ToolTipText, "%s +%d", CHATMGR->GetChatMsg(261), pInfo->GenGol);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( pInfo->MinChub != 0)
			{
				wsprintf(ToolTipText, "%s +%d", CHATMGR->GetChatMsg(262), pInfo->MinChub);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( pInfo->CheRyuk != 0)
			{
				wsprintf(ToolTipText, "%s +%d", CHATMGR->GetChatMsg(263), pInfo->CheRyuk);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( pInfo->SimMek != 0)
			{
				wsprintf(ToolTipText, "%s +%d", CHATMGR->GetChatMsg(264), pInfo->SimMek);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) !=0 )
			{
				wsprintf(ToolTipText, "%s +%d%%", CHATMGR->GetChatMsg(265), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)) !=0 )
			{
				wsprintf(ToolTipText, "%s +%d%%", CHATMGR->GetChatMsg(266), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)) !=0 )
			{
				wsprintf(ToolTipText, "%s +%d%%", CHATMGR->GetChatMsg(267), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)) !=0 )
			{
				wsprintf(ToolTipText, "%s +%d%%", CHATMGR->GetChatMsg(268), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)) !=0 )
			{
				wsprintf(ToolTipText, "%s +%d%%", CHATMGR->GetChatMsg(269), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( pInfo->LifeRecover !=0 )
			{
				wsprintf(ToolTipText, "%s %d", CHATMGR->GetChatMsg(270), pInfo->LifeRecover);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->LifeRecoverRate*100) !=0 )
			{
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(270), (int)(pInfo->LifeRecoverRate*100));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( pInfo->NaeRyukRecover !=0 )
			{
				wsprintf(ToolTipText, "%s %d", CHATMGR->GetChatMsg(271), pInfo->NaeRyukRecover);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			if( (int)(pInfo->NaeRyukRecoverRate*100) !=0 )
			{
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(271), (int)(pInfo->NaeRyukRecoverRate*100));
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
#ifdef TAIWAN_LOCAL
//========================= 달걀 하드코딩
			if( pInfo->ItemIdx == 53031 || pInfo->ItemIdx == 53094 )
			{
				wsprintf(ToolTipText, "%s %d%%", "빱竟뿟릿", 100);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			else if( pInfo->ItemIdx == 53032 || pInfo->ItemIdx == 53095 )
			{
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(270), 100);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(271), 100);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d%%", "빱竟뿟릿", 100);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
//==========================
#else 
			if( pInfo->ItemIdx == 53102 || pInfo->ItemIdx == 53232 || pInfo->ItemIdx == 53234 )
			{
				WORD plus = 1000;
#ifdef _HK_LOCAL_
				if( pInfo->ItemIdx == 53232 || pInfo->ItemIdx == 53234 )
					plus = 400;
#else
				if( pInfo->ItemIdx == 53232 )
					plus = 2000;
				else if( pInfo->ItemIdx == 53234 )
					plus = 400;
#endif

				wsprintf(ToolTipText, "%s %d", CHATMGR->GetChatMsg(270), plus);	//생명력회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d", CHATMGR->GetChatMsg(271), plus);	//내력회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d", CHATMGR->GetChatMsg(181), plus);	//호신강기회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			else if( pInfo->ItemIdx == 53031 || pInfo->ItemIdx == 53094 )
			{
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
			else if( pInfo->ItemIdx == 53032 || pInfo->ItemIdx == 53095 
				|| pInfo->ItemIdx == 53103 )
			{
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(270), 100);	//생명력회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(271), 100);	//내력회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
				wsprintf(ToolTipText, "%s %d%%", CHATMGR->GetChatMsg(181), 100);	//호신강기회복
				item->AddToolTipLine( ToolTipText, TTTC_EXTRAATTR );
			}
#endif
		}
		else if( pInfo->ItemKind & eEQUIP_ITEM )
		{
			if( ITEMMGR->CanEquip( pInfo->ItemIdx ) )
			{
				item->SetImageRGB( ICONCLR_USABLE );
			}
			else
			{
				item->SetImageRGB( ICONCLR_DISABLE );
			}
		}
		///////
		
		return;
	}

	CSkillInfo* pCommSkill	= SKILLMGR->GetSkillInfo( item->GetSrcIdx() );

	if( pCommSkill )
	{
		// magi82 - 함수를 반복할 필요없다. 바깥에 하나만 빼두자.
		WORD wCommSkillWeaponType = pCommSkill->GetWeaponType();
		WORD wCommSkillKind = pCommSkill->GetSkillKind();

		wsprintf(ToolTipText, "[%s]", pCommSkill->GetSkillName() );
//		item->AddToolTipLine( ToolTipText, TTTC_DEFAULT );

		CMugongBase* pMugongBase = GAMEIN->GetMugongDialog()->GetMugongAbs( item->GetSrcPosition() );

		if( pMugongBase )
		{
			if(pMugongBase->GetOption() == eSkillOption_None)
				item->AddToolTipLine( ToolTipText, TTTC_MUGONGNAME );
			else
				item->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGNAME );

			//무공의 경우 성 / 필요내력
			int Sung = pMugongBase->GetSung();
			int NeedExp = pCommSkill->GetNeedExp( Sung );

			if( NeedExp > 0 )
			{				
				if( Sung < 12 )
				{
					float percent = pMugongBase->GetExpPoint() * 100.0f / NeedExp;
					if( percent > 99.99f )
					{
						if( pMugongBase->GetExpPoint() != NeedExp-1 )
							percent = 99.99f;
					}

//					sprintf( ToolTipText, CHATMGR->GetChatMsg(471), pMugongBase->GetSung(),
//						pMugongBase->GetExpPoint() * 100 / (float)pCommSkill->GetNeedExp(pMugongBase->GetSung()) );
					sprintf( ToolTipText, CHATMGR->GetChatMsg(471), Sung, percent );
				}
				else
				{
					if(SKILLMGR->IsDeadlyMugong(pMugongBase->GetItemIdx()) == FALSE)
						sprintf( ToolTipText, CHATMGR->GetChatMsg(405), pMugongBase->GetSung() );
				}
				item->AddToolTipLine( ToolTipText, TTTC_MUGONGEXP );
			}

			if( pMugongBase->GetSung() > 0 )
			{
				wsprintf( ToolTipText, CHATMGR->GetChatMsg(472), pCommSkill->GetNeedNaeRyuk( pMugongBase->GetSung() ) );
				item->AddToolTipLine( ToolTipText, TTTC_MUGONGNEEDMANA );
			}
			
		}

		BOOL bUsable = TRUE;
		
		// magi82 - Titan(070911) 타이탄 무공업데이트
		// 타이탄 무공
		if( wCommSkillKind == SKILLKIND_TITAN && HERO->InTitan() == FALSE )
		{
			item->SetImageRGB( ICONCLR_DISABLE );
			bUsable = FALSE;
			item->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_LIMIT );
		}
		// 캐릭터 무공
		if( wCommSkillKind != SKILLKIND_TITAN && HERO->InTitan() == TRUE )
		{
			item->SetImageRGB( ICONCLR_DISABLE );
			bUsable = FALSE;
			item->AddToolTipLine( CHATMGR->GetChatMsg(1656), TTTC_LIMIT );
		}

		BOOL bFlag = FALSE;
		WORD wHeroWeaponType = HERO->GetWeaponEquipType();
		WORD wTitanWeaponType = HERO->GetTitanWeaponEquipType();

		if(HERO->InTitan() == TRUE)	// 타이탄 탑승중일때..
		{
			if( wCommSkillKind == SKILLKIND_TITAN )
			{
				if( wCommSkillWeaponType != WP_WITHOUT )	// 내공은 무기가 없다
				{
					if( wCommSkillWeaponType != wTitanWeaponType )
						bFlag = TRUE;					
				}
			}			
		}
		else	// 타이탄 탑승중이 아닐때..
		{
			if( wCommSkillKind == SKILLKIND_OUTERMUGONG && wCommSkillWeaponType != wHeroWeaponType )
			{
				bFlag = TRUE;
			}
		}

		if( bFlag == TRUE )
		{
			item->SetImageRGB( ICONCLR_DISABLE );
			bUsable = FALSE;

			switch(wCommSkillWeaponType)
			{
			case WP_GUM:
				item->AddToolTipLine( CHATMGR->GetChatMsg(465), TTTC_LIMIT );
				break;
			case WP_GWUN:	
				item->AddToolTipLine( CHATMGR->GetChatMsg(466), TTTC_LIMIT );
				break;
			case WP_DO:	
				item->AddToolTipLine( CHATMGR->GetChatMsg(467), TTTC_LIMIT );
				break;
			case WP_CHANG:	
				item->AddToolTipLine( CHATMGR->GetChatMsg(468), TTTC_LIMIT );
				break;
			case WP_GUNG:	
				item->AddToolTipLine( CHATMGR->GetChatMsg(469), TTTC_LIMIT );
				break;
			case WP_AMGI:	
				item->AddToolTipLine( CHATMGR->GetChatMsg(470), TTTC_LIMIT );
				break;
			}
		}

		// magi82 - Titan(071011) 타이탄 무공업데이트
		// 타이탄과 캐릭터의 무공이 다르면 사용 못하게 해야한다.
		if( wCommSkillKind == SKILLKIND_TITAN )
		{
			if( wCommSkillWeaponType != WP_WITHOUT )	// 내공은 무기가 없다
			{
				if( wHeroWeaponType != wTitanWeaponType )
				{
					item->SetImageRGB( ICONCLR_DISABLE );
					bUsable = FALSE;
					//item->AddToolTipLine( "magi82 - 타이탄과 캐릭터의 무기가 다름", TTTC_LIMIT );
				}
			}
		}

		//제한레벨 체크해서 이미지 rgb 바꾸기
		if( pCommSkill->GetSkillInfo()->RestrictLevel > HERO->GetLevel() )
		{
			wsprintf( ToolTipText, CHATMGR->GetChatMsg(460), pCommSkill->GetSkillInfo()->RestrictLevel );
			item->AddToolTipLine( ToolTipText, TTTC_LIMIT );
			item->SetImageRGB( ICONCLR_DISABLE );
			bUsable = FALSE;
		}

		// 화경 극마 체크			
		if( HERO->GetStage() == eStage_Hwa )
		{
			if( SKILLMGR->IsGeukMaSkill( pCommSkill->GetSkillInfo()->SkillIdx ) )
			{
				item->SetImageRGB( ICONCLR_DISABLE );
				item->SetAlpha( ICONALPHA_DISABLE );
				item->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
				bUsable = FALSE;
			}
		}
		else if( HERO->GetStage() == eStage_Geuk )
		{
			if( SKILLMGR->IsHwaKungSkill( pCommSkill->GetSkillInfo()->SkillIdx ) )
			{
				item->SetImageRGB( ICONCLR_DISABLE );
				item->SetAlpha( ICONALPHA_DISABLE );
				item->SetToolTipImageRGB( TTCLR_MUGONG_CANNOTUSE );
				bUsable = FALSE;
			}
		}		

		if( bUsable )
			item->SetImageRGB( RGBA_MAKE( 255, 255, 255, 255 ) );				

		return;
	}

	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( item->GetSrcIdx() - MIN_ABILITY_INDEX );

	if( pAbility )
	{
		CAbilityInfo* pAbilityInfo = pAbility->GetAbilityInfo();
		if( pAbilityInfo )
		{
			wsprintf( ToolTipText, "[%s]", pAbilityInfo->GetInfo()->Ability_name );
			item->AddToolTipLine( ToolTipText, TTTC_DEFAULT );
		}
	}
}
/*
BOOL CQuickDialog::IsAddable(POSTYPE absPos)
{
	int tabNum = absPos/TABCELL_QUICK_NUM;
	return m_pIconGridSheet[tabNum]->IsAddable(absPos);	
}
	*/

