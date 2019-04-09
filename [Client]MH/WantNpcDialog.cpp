// WantNpcDialog.cpp: implementation of the CWantNpcDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantNpcDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cButton.h"
#include "./Interface/cComboBox.h"
#include "./Interface/cStatic.h"
#include "./Interface/cListCtrl.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "cImeEx.h"
#include "cRitemEx.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "cMsgBox.h"
#include "./interface/cWindowManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantNpcDialog::CWantNpcDialog()
{
	m_type = WT_WANTNPCDIALOG;
	m_WantedOrderType = eWan_Date;
	m_CurPage = 0;
	m_PastPage = 0;
	m_Mode = -1;
	m_SelectRowIndex = -1;
	memset(&m_CurSelectWanted, 0, sizeof(WANTNPCLIST));
}

CWantNpcDialog::~CWantNpcDialog()
{
	for(int i=0; i<eWantNpcMode_Max; ++i)
		m_WantNpcCtlListArray[i].RemoveAll();
}

void CWantNpcDialog::Linking()
{
	pWantedListCtrl = (cListCtrl*)GetWindowForID(WANTNPC_WANTLCTRL);
	m_pMyWantNpcBtn = (cButton*)GetWindowForID(WANTNPC_MYWANTBTN);
	m_pBuyNpcBtn = (cButton*)GetWindowForID(WANTNPC_BUYNPCBTN);
	m_pBuySearchBtn = (cButton*)GetWindowForID(WANTNPC_BUYSEARCHBTN);
	m_pSearchModeBtn = (cButton*)GetWindowForID(WANTNPC_SEARCHMODEBTN);
	m_pSearchBtn = (cButton*)GetWindowForID(WANTNPC_SEARCHBTN);
	m_pGiveupBtn = (cButton*)GetWindowForID(WANTNPC_GIVEUPBTN);
	m_pOrderWantNpcCombo = (cComboBox *)GetWindowForID(WANTNPC_LISTORDERCOMBOBOX);
	m_pSortBtn = (cButton*)GetWindowForID(WANTNPC_SORTBTN);
	m_pSearchEdit = (cEditBox *)GetWindowForID(WANTNPC_SEARCHEDIT);
	m_pMyStc = (cStatic *)GetWindowForID(WANTNPC_MYSTC);
	m_pMyWantSearchBtn = (cButton*)GetWindowForID(WANTNPC_MYWANTSEARCHBTN);


	for(int i=0; i<MAX_WANTNPC_PAGEBTN; ++i)
	{
		m_OrderPageBtn[i] = (cPushupButton*)GetWindowForID(WANTNPC_FRONTLISTBTN+i);
		m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail(m_OrderPageBtn[i]);
		m_MyWantPageBtn[i] = (cPushupButton*)GetWindowForID(WANTNPC_MYFRONTLISTBTN+i);
		m_WantNpcCtlListArray[eWantNpcMode_My].AddTail(m_MyWantPageBtn[i]);
		m_SearchPageBtn[i] = (cPushupButton*)GetWindowForID(WANTNPC_SEARCHFRONTLISTBTN+i);
		m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_SearchPageBtn[i]);
	}

	m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail((cStatic *)GetWindowForID(WANTNPC_BUYPRICE));
	m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail(m_pOrderWantNpcCombo);	
	m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail(m_pSearchModeBtn);
	m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail(m_pBuyNpcBtn);
	m_WantNpcCtlListArray[eWantNpcMode_Npc].AddTail(m_pMyWantNpcBtn);
	
	m_WantNpcCtlListArray[eWantNpcMode_My].AddTail(m_pMyStc);
	m_WantNpcCtlListArray[eWantNpcMode_My].AddTail(m_pGiveupBtn);
	m_WantNpcCtlListArray[eWantNpcMode_My].AddTail(m_pSortBtn);
		
	m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_pSearchEdit);
	m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_pSearchBtn);
	m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_pBuySearchBtn);
	m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_pMyWantSearchBtn);
	m_WantNpcCtlListArray[eWantNpcMode_Search].AddTail(m_pSortBtn);
	
	m_pSearchEdit->SetValidCheck(VCM_CHARNAME);
}

BYTE CWantNpcDialog::GetOrderType()
{
	return m_WantedOrderType;
}

DWORD CWantNpcDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_ROWCLICK)
	{
		cRITEMEx * pRItem = (cRITEMEx *)pWantedListCtrl->GetRItem(pWantedListCtrl->GetSelectedRowIdx());
		if(pRItem != NULL)
		{	
			CloseMsgBox();

			WANTNPCLIST Info;
			SafeStrCpy(Info.WantedName, pRItem->pString[0], MAX_NAME_LENGTH+1);
			Info.Prize = atoi(pRItem->pString[1]);
			Info.VolunteerNum = atoi(pRItem->pString[2]);
			Info.WantedIDX = pRItem->dwID;
			
			SetWhiteFontColor();
			for(int i=0; i<3; ++i)
			{
				pRItem->rgb[i] = RGBA_MAKE(255,255,0,255);
			}
			SetCurSelectWanted(&Info, pWantedListCtrl->GetSelectedRowIdx());
		}
	}
	return we;
}

void CWantNpcDialog::SetWhiteFontColor()
{
	cRITEMEx * pPastRItem = (cRITEMEx *)pWantedListCtrl->GetRItem(m_SelectRowIndex);
	if(pPastRItem == 0)
	{ 	return;	}
	else
	{
		for(int i=0; i<3; ++i)
		{
			pPastRItem->rgb[i] = RGBA_MAKE(255,255,255,255);
		}
	}
}

void CWantNpcDialog::SetOrderType(BYTE OrderType)
{
	if(OrderType < eWan_Date && OrderType >= eWan_OrderTypeMax)
	{
		ASSERTMSG(0, "Çö»ó¹ü Á¤·Ä Å¸ÀÔÀÌ ¾ø½À´Ï´Ù. No Sort Type in Wanted List");
		return;
	}
	m_WantedOrderType = OrderType;
}

void CWantNpcDialog::SetWantedList(SEND_WANTNPCLIST* pInfo)
{
	pWantedListCtrl->DeleteAllItems();
	
	switch(GetMode())
	{
	case eWantNpcMode_Npc: ShowWantPageBtn(pInfo->TotalPage, m_OrderPageBtn);
		break;
	case eWantNpcMode_Search: ShowWantPageBtn(pInfo->TotalPage, m_SearchPageBtn);
		break;
	case eWantNpcMode_My: ShowWantPageBtn(pInfo->TotalPage, m_MyWantPageBtn);
		break;
	default:
		ASSERT(0); break;
	}
	
	for(int i =0; i<WANTUNIT_PER_PAGE; ++i)
	{
		if(pInfo->WantNpcList[i].WantedIDX == 0)
			break;
		
		cRITEMEx *ritem = new cRITEMEx(3);
		strcpy(ritem->pString[0], pInfo->WantNpcList[i].WantedName);
		sprintf(ritem->pString[1], "%d", pInfo->WantNpcList[i].Prize);
		sprintf(ritem->pString[2], "%d", pInfo->WantNpcList[i].VolunteerNum);
		
		ritem->dwID = pInfo->WantNpcList[i].WantedIDX;
		pWantedListCtrl->InsertItem(i, ritem);
	}
	
	WANTNPCLIST Info; 
	memset(&Info, 0, sizeof(WANTNPCLIST));
	SetCurSelectWanted(&Info, -1);
}

void CWantNpcDialog::ShowWantPageBtn(WORD TotalPage, cPushupButton* pPUBtnArray[])
{
	if(TotalPage == 0)
	{
		for(DWORD i=0; i<MAX_WANTNPC_PAGEBTN; ++i)
			pPUBtnArray[i]->SetActive(FALSE);
		SetCurPage(0);
		return;
	}
		
	DWORD MaxUnit = (TotalPage-1) / MAX_WANTNPC_PAGEUNIT;
	DWORD CurUnit = (GetCurPage()-1) / MAX_WANTNPC_PAGEUNIT;

	int num = GetCurPage()%MAX_WANTNPC_PAGEUNIT;
	if(num == 0)
		num = MAX_WANTNPC_PAGEUNIT;
	SetPushUpBtn(pPUBtnArray, num);

	char buf[16] = {0,};
	DWORD page;
	pPUBtnArray[0]->SetActive(CurUnit != 0);
	for(int i=1; i<=MAX_WANTNPC_PAGEUNIT; ++i)
	{
		page = CurUnit*MAX_WANTNPC_PAGEUNIT + i;
		wsprintf(buf, "%d", page);
		pPUBtnArray[i]->SetText(buf, RGBA_MAKE(255,255,255,255));
		pPUBtnArray[i]->SetActive(page <= TotalPage);
	}
	pPUBtnArray[6]->SetActive(MaxUnit > CurUnit);
}

void CWantNpcDialog::ShowWantMode(int mode)
{
	if(m_Mode == mode)
		return;

	if(m_Mode != -1)
		SetActiveWantMode(m_Mode,FALSE);

	SetActiveWantMode(mode,TRUE);
	m_Mode = mode;
		
#ifdef _TESTCLIENT_
	WANTNPCLIST Info;
	memset(&Info, 0, sizeof(WANTNPCLIST));
	SetCurSelectWanted(&Info, -1);
#endif
	AddShowWantMode(mode);
}

void CWantNpcDialog::AddShowWantMode(int mode)
{
	switch(mode)
	{
	case eWantNpcMode_Search:
		m_pSearchEdit->SetEditText("");
		m_pSearchEdit->SetFocusEdit(TRUE);
		break;
	default:
		m_pSearchEdit->SetFocusEdit(FALSE);
		break;
	}
}

void CWantNpcDialog::SetActiveWantMode(int mode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_WantNpcCtlListArray[mode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}

void CWantNpcDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;

	if(val == FALSE)
	{
		if( HERO )
		if( HERO->GetState() == eObjectState_Deal ) 
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		
		CloseMsgBox();
		
		m_pSearchEdit->SetFocusEdit(FALSE);
	}
	cDialog::SetActive(val);
}

void CWantNpcDialog::SetPushUpBtn(cPushupButton* pArray[], int num)
{
	for(int i=0; i<MAX_WANTNPC_PAGEBTN; ++i)
	{
		if(i == num)
			pArray[i]->SetPush(TRUE);
		else
			pArray[i]->SetPush(FALSE);
	}
}

void CWantNpcDialog::SetCurPage(WORD page)
{
	m_CurPage = page;
}

DWORD CWantNpcDialog::GetCurWantedIdx()
{
	return m_CurSelectWanted.WantedIDX;
}

char* CWantNpcDialog::GetCurWantedName()
{
	return m_CurSelectWanted.WantedName;
}

void CWantNpcDialog::SetCurSelectWanted(WANTNPCLIST * pInfo, int SelectRowIndex)
{
	m_SelectRowIndex = SelectRowIndex;

	m_CurSelectWanted.Prize = pInfo->Prize;
	m_CurSelectWanted.VolunteerNum = pInfo->VolunteerNum;
	m_CurSelectWanted.WantedIDX = pInfo->WantedIDX;
	SafeStrCpy(m_CurSelectWanted.WantedName, pInfo->WantedName, MAX_NAME_LENGTH+1);
}

void CWantNpcDialog::CloseMsgBox()
{
	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_WANTGIVEUP);
	if(pMsgBox)
		WINDOWMGR->AddListDestroyWindow( pMsgBox );
	pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_WANTBUY);
	if(pMsgBox)
		WINDOWMGR->AddListDestroyWindow( pMsgBox );
}

WORD CWantNpcDialog::GetCurPage() 
{ 
	return m_CurPage;	
}
