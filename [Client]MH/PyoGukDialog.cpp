// PyogukDialog.cpp: implementation of the CPyogukDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PyogukDialog.h"
#include "PrivateWarehouseDialog.h"

#include "WindowIDEnum.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrl.h"
#include "./Interface/cStatic.h"
#include "cDivideBox.h"
#include "cMsgBox.h"

#include "ObjectManager.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "./Interface/cWindowManager.h"
#include "MHFile.h"
#include "ChatManager.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPyogukDialog::CPyogukDialog()
{
	m_type = WT_PYOGUKDLG;
	m_CurPyogukMode = -1;
	for(int i=0; i<TAB_PYOGUK_NUM; ++i)
		m_PyogukItemDlg[i] = NULL;
	m_PyogukNum = 0;
	memset(&m_PyogukListInfo, 0, sizeof(PYOGUK_LISTINFO));
	m_bItemInit = FALSE;
}

CPyogukDialog::~CPyogukDialog()
{
	for(int n=0;n<ePyogukMode_Max;++n)
	{
		m_PyogukCtlListArray[n].RemoveAll();
	}

	m_pPyogukListInfo->DeleteAllItems();
}

void CPyogukDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_PYOGUKDLG;
	LoadPyogukListInfo();
}

void CPyogukDialog::LoadPyogukListInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/PyogukListInfo.bin","rb");
#else
	file.Init("Resource/PyogukListInfo.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0, "Fail To Load a File : PyogukListInfo.");
		return;
	}

	int Count = MAX_PYOGUKLIST_NUM;
#ifdef _JAPAN_LOCAL_
	Count = GIVEN_PYOGUK_SLOT;
#endif
#ifdef _HK_LOCAL_
	Count = GIVEN_PYOGUK_SLOT;
#endif
#ifdef _TL_LOCAL_
	Count = GIVEN_PYOGUK_SLOT;
#endif

	for(int n=0;n<Count;++n)
	{
		m_PyogukListInfo.Pyoguk[n].MaxCellNum = 30*(n+1);
		m_PyogukListInfo.Pyoguk[n].MaxMoney = file.GetDword();
		m_PyogukListInfo.Pyoguk[n].BuyPrice = file.GetDword();
	}

	file.Release();
}

void CPyogukDialog::Linking()
{
	m_pMoney = (cStatic*)GetWindowForID(PYO_MONEY); 

	
	//////////////////////////////////////////////////////////////////////////
	//PyogukListInfo
	m_pPyogukListInfo = (cListCtrl*)GetWindowForID(PYO_PYOGUKLCTRL);
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail(m_pPyogukListInfo);
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cButton*)GetWindowForID(PYO_BUYPYOGUKBTN));
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cButton*)GetWindowForID(PYO_NOBUYBTN));
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cStatic*)GetWindowForID(PYO_PYOGUKINFO1));
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cStatic*)GetWindowForID(PYO_PYOGUKINFO2));
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cStatic*)GetWindowForID(PYO_PYOGUKINFO3));
	m_pPyogukHaveStc = (cStatic*)GetWindowForID(PYO_PYOGUKHAVESTC);
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail(m_pPyogukHaveStc);
	m_PyogukCtlListArray[ePyogukMode_PyogukListInfo].AddTail((cStatic*)GetWindowForID(PYO_PYOGUKHAVE));
	//////////////////////////////////////////////////////////////////////////
	//noWare
	m_PyogukCtlListArray[ePyogukMode_NoWare].AddTail((cStatic*)GetWindowForID(PYO_NOWARE1));
	m_PyogukCtlListArray[ePyogukMode_NoWare].AddTail((cStatic*)GetWindowForID(PYO_NOWARE));
	m_PyogukCtlListArray[ePyogukMode_NoWare].AddTail((cButton*)GetWindowForID(PYO_PYOGUKLISTBTN));
	m_PyogukCtlListArray[ePyogukMode_NoWare].AddTail((cButton*)GetWindowForID(PYO_NOBUYPYOGUKBTN));
	
	//////////////////////////////////////////////////////////////////////////
	
	for(int i=0; i<MAX_PYOGUKLIST_NUM; ++i)
	{
		m_pPyogukBtn[i] = (cPushupButton*)GetWindowForID(PYO_PYOGUK1BTN+i);

		m_PyogukItemDlg[i] = (CPrivateWarehouseDialog*)GetWindowForID(PYO_WAREITEM1+i);
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail(m_PyogukItemDlg[i]);
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail((cButton*)GetWindowForID(PYO_PUTINMONEYBTN));
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail((cButton*)GetWindowForID(PYO_PUTOUTMONEYBTN));
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail((cStatic*)GetWindowForID(PYO_MONEYIMG)); //동전 그림
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail((cStatic*)GetWindowForID(PYO_MONEYBACKIMG));
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail(m_pMoney); //바탕 이미지
		m_PyogukCtlListArray[ePyogukMode_PyogukWare1+i].AddTail((cStatic*)GetWindowForID(PYO_WAREIMAGE));
	}
	
	SetPyogukListInfo();
}

void CPyogukDialog::SetPyogukListInfo()
{
	m_pPyogukListInfo->DeleteAllItems();
	
	int Count = MAX_PYOGUKLIST_NUM;
#ifdef _JAPAN_LOCAL_
	if( HERO )
		Count = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
#endif

#ifdef _HK_LOCAL_
	if( HERO )
		Count = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
#endif

#ifdef _TL_LOCAL_
	if( HERO )
		Count = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
#endif

	for(BYTE i=0; i<Count; ++i)
	{
		RITEM * ritem = new RITEM(4);
		sprintf(ritem->pString[0], "%d", i+1);
		sprintf(ritem->pString[1], "%d", m_PyogukListInfo.Pyoguk[i].MaxCellNum);

//		sprintf(ritem->pString[2], "%d", m_PyogukListInfo.Pyoguk[i].MaxMoney);
//		sprintf(ritem->pString[3], "%d", m_PyogukListInfo.Pyoguk[i].BuyPrice);
		sprintf(ritem->pString[2], "%s", AddComma(m_PyogukListInfo.Pyoguk[i].MaxMoney));

#ifdef _TL_LOCAL_
		if( m_PyogukListInfo.Pyoguk[i].BuyPrice == 0 )
			sprintf(ritem->pString[3], "%s", CHATMGR->GetChatMsg(1436));
		else
			sprintf(ritem->pString[3], "%s", AddComma(m_PyogukListInfo.Pyoguk[i].BuyPrice));
#else
		sprintf(ritem->pString[3], "%s", AddComma(m_PyogukListInfo.Pyoguk[i].BuyPrice));
#endif

		
//		if((HERO->GetPyogukNum() - i) > 0 )
//		{
//			for(int n =0; n<4; ++n)
//				ritem->rgb[n] = RGBA_MAKE(180, 180, 180, 255);
//		}
		m_pPyogukListInfo->InsertItem(i, ritem);
	}
}

void CPyogukDialog::SetMode(int mode)
{
	m_CurPyogukMode = mode;
}

void CPyogukDialog::ShowPyogukMode(int mode)
{
	if(m_CurPyogukMode != -1)
	{
		SetActivePyogukMode(m_CurPyogukMode,FALSE);
	}
	
	m_CurPyogukMode = mode;
	SetActivePyogukMode(mode,TRUE);
		
	RefreshDlg();
}

void CPyogukDialog::AddPyogukMode(int mode)
{
	switch(mode)
	{
	case ePyogukMode_PyogukListInfo:
		{
			m_pPyogukHaveStc->SetStaticValue(HERO->GetPyogukNum());
		}
		break;
	case ePyogukMode_PyogukWare1: case ePyogukMode_PyogukWare2: case ePyogukMode_PyogukWare3:
	case ePyogukMode_PyogukWare4: case ePyogukMode_PyogukWare5:
		{
			SelectedBtnPushDown(mode - ePyogukMode_PyogukWare1);
			SetSelectedPyoguk(mode-ePyogukMode_PyogukWare1);
		}
		break;
	}
}

void CPyogukDialog::SetActivePyogukMode(int mode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_PyogukCtlListArray[mode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}

void CPyogukDialog::RefreshDlg()
{
	if(m_CurPyogukMode != ePyogukMode_PyogukListInfo)
	{
		for(int i=0; i<HERO->GetPyogukNum(); ++i)
			m_pPyogukBtn[i]->SetActive(TRUE);
	}
	else
	{
		for(int i=0; i<MAX_PYOGUKLIST_NUM; ++i)
			m_pPyogukBtn[i]->SetActive(FALSE);
	}

}

void CPyogukDialog::MoveItem(MSG_ITEM_MOVE_SYN* pInfo)
{
}

void CPyogukDialog::SelectedBtnPushDown(int i)
{
	for(int n=0; n<MAX_PYOGUKLIST_NUM; ++n)
	{
		if(n == i)
			m_pPyogukBtn[n]->SetPush(TRUE);
		else
			m_pPyogukBtn[n]->SetPush(FALSE);
	}		
}

BOOL CPyogukDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if(m_CurPyogukMode >= ePyogukMode_NoWare)
	{
		char buf[128];
		sprintf(buf, "CurPyogukMode : %d", m_CurPyogukMode);
		ASSERTMSG(0,buf);
		return FALSE;
	}

	if(m_CurPyogukMode == 0)
		return FALSE;

	return m_PyogukItemDlg[m_CurPyogukMode-1]->FakeMoveIcon(x,y,icon);
}


BOOL CPyogukDialog::AddItem(ITEMBASE* pItemInfo)
{
	int Num = GetPyogukNum(pItemInfo->Position);
	return m_PyogukItemDlg[Num]->AddItem(pItemInfo);
}

BOOL CPyogukDialog::AddItem(CItem* pItem)
{	
	int Num = GetPyogukNum(pItem->GetPosition());
	return m_PyogukItemDlg[Num]->AddItem(pItem);
}

BOOL CPyogukDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	int Num = GetPyogukNum(Pos);
	return m_PyogukItemDlg[Num]->DeleteItem(Pos, ppItem);
}

void CPyogukDialog::DeleteAllPyogukItem()
{
	for(POSTYPE i=0; i<TABCELL_PYOGUK_NUM; ++ i)
	{
		for(int j=0; j<MAX_PYOGUKLIST_NUM; ++j)
			m_PyogukItemDlg[j]->DeleteItem(TP_PYOGUK_START+i+j*TABCELL_PYOGUK_NUM, NULL);
	}
}

int CPyogukDialog::GetPyogukNum(POSTYPE absPos)
{
	return ( absPos - TP_PYOGUK_START ) / TABCELL_PYOGUK_NUM;
}

/*
void CPyogukDialog::SetPyogukItem(SEND_PYOGUK_ITEM_INFO* pInfo)
{
	// taiyo 안쓰임
	return;

	for(int i=0;i<pInfo->wCount;++i)
	{
		if( pInfo->PyogukItem[i].dwDBIdx != 0 )
		{
			int Num = (pInfo->PyogukItem[i].Position-TP_PYOGUK_START)/TABCELL_PYOGUK_NUM;
			m_PyogukItemDlg[Num]->AddItem(&pInfo->PyogukItem[i]);
		}
	}
	char temp[50] = {0,};
	sprintf(temp,"%14u", pInfo->money);
	m_pMoney->SetStaticText(temp);

}
*/


void CPyogukDialog::SetPyogukMoney(DWORD Money)
{
	m_pMoney->SetStaticText( AddComma( Money ) );
}

CItem * CPyogukDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		int num = GetPyogukNum(absPos);
		return (CItem *)m_PyogukItemDlg[num]->GetIconForIdx(absPos-TP_PYOGUK_START-TABCELL_PYOGUK_NUM*num);
	}
	else
		return NULL;
}

void CPyogukDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;

	if(val == FALSE && m_PyogukItemDlg[0] != NULL)
	{
		for(int i=0; i<TAB_PYOGUK_NUM; ++i)
			m_PyogukItemDlg[i]->SetActive(val);
	}
	if( val == FALSE)
	{
		if( HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY );
		if( pDivideBox )
		{
			pDivideBox->SetDisable(TRUE);
			pDivideBox->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow( pDivideBox );
		}

		PyoGukMsgBoxClr();
	}
	cDialog::SetActive(val);
/*
#ifdef _JAPAN_LOCAL_

	
	if( !val )				return;

	DWORD nActive = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
	for(int i=GIVEN_PYOGUK_SLOT; i<MAX_PYOGUKLIST_NUM; ++i)
	{
		cWindow* pBtn = GetWindowForID( PYO_PYOGUK1BTN+i );
		cWindow* pGrid = GetWindowForID( PYO_WAREITEM1+i );
		
		if( i < nActive )
		{
			if( pBtn )		pBtn->SetActive( TRUE );
			//if( pGrid)		pGrid->SetActive( TRUE );
		}
		else
		{
			if( pBtn )		pBtn->SetActive( FALSE );
			if( pGrid)		pGrid->SetActive( FALSE );
		}
	}
	
	
#endif	// _JAPAN_LOCAL_


#ifdef _HK_LOCAL_

	if( !val )				return;

	DWORD nActive = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
	for(int i=GIVEN_PYOGUK_SLOT; i<MAX_PYOGUKLIST_NUM; ++i)
	{
		cWindow* pBtn = GetWindowForID( PYO_PYOGUK1BTN+i );
		cWindow* pGrid = GetWindowForID( PYO_WAREITEM1+i );
		
		if( i < nActive )
		{
			if( pBtn )		pBtn->SetActive( TRUE );
			//if( pGrid)		pGrid->SetActive( TRUE );
		}
		else
		{
			if( pBtn )		pBtn->SetActive( FALSE );
			if( pGrid)		pGrid->SetActive( FALSE );
		}
	}
	
#endif

#ifdef _TL_LOCAL_

	if( !val )				return;

	DWORD nActive = GIVEN_PYOGUK_SLOT + HERO->GetExtraPyogukSlot();
	for(int i=GIVEN_PYOGUK_SLOT; i<MAX_PYOGUKLIST_NUM; ++i)
	{
		cWindow* pBtn = GetWindowForID( PYO_PYOGUK1BTN+i );
		cWindow* pGrid = GetWindowForID( PYO_WAREITEM1+i );
		
		if( i < nActive )
		{
			if( pBtn )		pBtn->SetActive( TRUE );
//			if( pGrid)		pGrid->SetActive( TRUE );
		}
		else
		{
			if( pBtn )		pBtn->SetActive( FALSE );
			if( pGrid)		pGrid->SetActive( FALSE );
		}
	}
	
#endif
*/
}

void CPyogukDialog::SetDividMsg( CItem* pFromItem, POSTYPE ToPos )
{
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return;

	m_divideMsg.Category			= MP_ITEM;
	m_divideMsg.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID			= HEROID;
	m_divideMsg.FromPos				= pFromItem->GetPosition();
	m_divideMsg.FromDur				= pFromItem->GetDurability();
	m_divideMsg.ToPos				= ToPos;
	m_divideMsg.ToDur				= 0;
	m_divideMsg.wItemIdx			= pFromItem->GetItemIdx();
}

void CPyogukDialog::SetSelectedPyoguk(BYTE PyogukNum)
{	
	m_PyogukNum = PyogukNum;
	SelectedBtnPushDown(PyogukNum);
}

void CPyogukDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	int Num = GetPyogukNum(ToPos);
	m_PyogukItemDlg[Num]->FakeGeneralItemMove(ToPos, pFromItem, pToItem);
}

int CPyogukDialog::CanBuyPyoguk()
{
	BYTE pyoguknum = HERO->GetPyogukNum();
	if(pyoguknum >= TAB_PYOGUK_NUM)
	{
		ASSERT(0);
		return 3;
	}
	
#ifdef _JAPAN_LOCAL_
	/*
	if( pyoguknum >= GIVEN_PYOGUK_SLOT )
		return 2;
	DWORD ExtraSlot = HERO->GetExtraPyogukSlot();
	if( (pyoguknum+ExtraSlot) >= TAB_PYOGUK_NUM )
		return 3;
	*/
	DWORD ExtraSlot = HERO->GetExtraPyogukSlot();
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) && (pyoguknum-ExtraSlot) >= GIVEN_PYOGUK_SLOT )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1303) );
		return 2;
	}	
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) )
		return 3;



#endif
	


#ifdef _HK_LOCAL_
/*
	if( pyoguknum >= GIVEN_PYOGUK_SLOT )
		return 2;
	DWORD ExtraSlot = HERO->GetExtraPyogukSlot();
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) && (pyoguknum-ExtraSlot) >= GIVEN_PYOGUK_SLOT )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1303) );
		return 3;
	}	
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) )
		return 3;
	}
*/
	DWORD ExtraSlot = HERO->GetExtraPyogukSlot();
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) && (pyoguknum-ExtraSlot) >= GIVEN_PYOGUK_SLOT )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1303) );
		return 2;
	}
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) )
		return 3;
#endif

#ifdef _TL_LOCAL_
	DWORD ExtraSlot = HERO->GetExtraPyogukSlot();

	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) && (pyoguknum-ExtraSlot) >= GIVEN_PYOGUK_SLOT )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1303) );
		return 2;
	}
	if( pyoguknum >= (GIVEN_PYOGUK_SLOT+ExtraSlot) )
		return 3;
#endif


	if(HERO->GetMoney() < m_PyogukListInfo.Pyoguk[HERO->GetPyogukNum()].BuyPrice)
		return 1;
	return 0;
}

void CPyogukDialog::PyoGukMsgBoxClr()
{
	cMsgBox * pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_PYOGUKBUY);
	if(pMsgBox)
		WINDOWMGR->AddListDestroyWindow(pMsgBox);
}
