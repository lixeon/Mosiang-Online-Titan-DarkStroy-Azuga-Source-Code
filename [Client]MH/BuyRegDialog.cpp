// BuyRegDialog.cpp: implementation of the CBuyRegDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuyRegDialog.h"
#include "WindowIDEnum.h"
#include "./Interface\cEditBox.h"
#include "./Interface\cListDialog.h"
#include "./Interface\cButton.h"
#include "./Interface\cStatic.h"
#include "./Input/Mouse.h"
#include "./Interface/cWindowManager.h"
#include "ExchangeItem.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "GameResourceStruct.h"
#include "PtrList.h"
#include "cIMEex.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "GameIn.h"
#include "StreetBuyStall.h"
#include "./Interface/cScriptManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuyRegDialog::CBuyRegDialog()
{
	m_pTypeList		=	NULL;

	for(int i = 0; i < ITEM_TYPE_COUNT; i++)
		m_pItemList[i]	=	NULL;

	m_pClassList	=	NULL;
	m_pVolumeEdit	=	NULL;
	m_pMoneyEdit	=	NULL;
	m_pRegBtn		=	NULL;
}

CBuyRegDialog::~CBuyRegDialog()
{

}

//////////////////////////////////////////////////////////////////////////
//구매 아이템 리스트 로딩
void CBuyRegDialog::LoadItemList()
{
	CMHFile fp;
	
#ifdef _FILE_BIN_
//	if( !fp.Init( ".\\Resource\\BRTList.bin", "rb" ) )	return;	
	if( !fp.Init( "./Resource/Client/BRTList.bin", "rb" ) )	return;
#else
	if( !fp.Init( ".\\Resource\\BRTList.txt", "rt" ) )	return;
#endif
	
	char buf[32] = {0,};

	while(1)
	{
		fp.GetString(buf);

		m_pTypeList->AddItem(buf, RGB_HALF(255, 255, 255));
		
		if(fp.IsEOF())
			break;
	}

	m_pTypeList->ResetGuageBarPos();

	fp.Release();
	
#ifdef _FILE_BIN_
//	if( !fp.Init( ".\\Resource\\BRList.bin", "rb" ) )	return;	
	if( !fp.Init( "./Resource/Client/BRList.bin", "rb" ) )	return;
#else
	if( !fp.Init( ".\\Resource\\BRList.txt", "rt" ) )	return;
#endif
	while(1)
	{
		WORD nType = fp.GetWord();
		BOOL bClass = fp.GetBool();
		WORD nItem = fp.GetWord();

		m_pItemList[nType - 1]->AddItem(ITEMMGR->GetItemInfo(nItem)->ItemName, RGB_HALF(255, 255, 255));
		
		if(fp.IsEOF())
			break;
	}

	for(int i = 0; i < ITEM_TYPE_COUNT; i++)
		m_pItemList[i]->ResetGuageBarPos();

	fp.Release();

	for(i = 0; i < 10; i++)
	{
		wsprintf(buf, "+%d", i);
		m_pClassList->AddItem(buf, RGB_HALF(255, 255, 255));
	}

	m_pClassList->ResetGuageBarPos();
}
//////////////////////////////////////////////////////////////////////////


void CBuyRegDialog::Linking()
{
	m_pTypeList		= (cListDialog*)GetWindowForID(BRS_TYPELIST);
	m_pTypeList->SetShowSelect(TRUE);
	
	m_pItemList[WEAPON]		= (cListDialog*)GetWindowForID(BRS_ITEMLIST1);
	m_pItemList[CLOTHES]	= (cListDialog*)GetWindowForID(BRS_ITEMLIST2);
	m_pItemList[ACCESSORY]	= (cListDialog*)GetWindowForID(BRS_ITEMLIST3);
	m_pItemList[POTION]		= (cListDialog*)GetWindowForID(BRS_ITEMLIST4);
	m_pItemList[MATERIAL]	= (cListDialog*)GetWindowForID(BRS_ITEMLIST5);
	m_pItemList[ETC]		= (cListDialog*)GetWindowForID(BRS_ITEMLIST6);
	m_pItemList[ITEM_MALL]	= (cListDialog*)GetWindowForID(BRS_ITEMLIST7);
	
	for(int i = 0; i < ITEM_TYPE_COUNT; i++)
		m_pItemList[i]->SetShowSelect(TRUE);
	
	m_pClassList	= (cListDialog*)GetWindowForID(BRS_CLASSLIST);
	m_pClassList->SetShowSelect(TRUE);
	
	m_pVolumeEdit	= (cEditBox*)GetWindowForID(BRS_VOLUMEEDIT);
	m_pVolumeEdit->SetValidCheck( VCM_NUMBER );
	m_pVolumeEdit->SetAlign( TXT_RIGHT );
	m_pVolumeEdit->SetReadOnly( FALSE );
	
	cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	m_pVolumeEdit->SetToolTip( CHATMGR->GetChatMsg(1215), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );

	m_pMoneyEdit	= (cEditBox*)GetWindowForID(BRS_MONEYEDIT);
	m_pMoneyEdit->SetValidCheck( VCM_NUMBER );
	m_pMoneyEdit->SetAlign( TXT_RIGHT );
	m_pMoneyEdit->SetReadOnly( FALSE );
	
	m_pRegBtn		= (cButton*)GetWindowForID(BRS_REGBTN);
	m_pDltBtn		= (cButton*)GetWindowForID(BRS_DELETEBTN);

	m_pClassText	= (cStatic*)GetWindowForID(BRS_CLASSTEXT);
	m_pClassRect1	= (cStatic*)GetWindowForID(BRS_CLASSRECT1);
	m_pClassRect2	= (cStatic*)GetWindowForID(BRS_CLASSRECT2);
	
	m_pItemBigRect		= (cStatic*)GetWindowForID(BRS_ITEMBIGRECT);
	m_pItemSmallRect1	= (cStatic*)GetWindowForID(BRS_ITEMSMALLRECT1);
	m_pItemSmallRect2	= (cStatic*)GetWindowForID(BRS_ITEMSMALLRECT2);
	
	LoadItemList();
	InitRegInfo();
}

//////////////////////////////////////////////////////////////////////////
//아이템 종류 갱신
void CBuyRegDialog::UpdateType()
{
	if(m_nType != m_pTypeList->GetCurSelectedRowIdx())
	{
		m_pItemList[m_nType]->SetActive(FALSE);					//선택되어 있던 아이템 리스트를 비활성화

		m_nType = m_pTypeList->GetCurSelectedRowIdx();			//선택된 아이템 종류를 가져온다

		m_nItem = 0;											//아이템 리스트 번호 초기화
		m_pItemList[m_nType]->SetCurSelectedRowIdx(m_nItem);

		m_pItemList[m_nType]->SetActive(TRUE);					//선택된 아이템 리스트 활성화
				
		//선택된 아이템 리스트가 등급이 없는 아이템 종류이면 등급 리스트를 비활성화
		if(m_nType < ACCESSORY)
		{
			m_nClass = 0;
			m_pClassList->SetCurSelectedRowIdx(m_nClass);
			m_pClassList->SetActive(TRUE);
			m_pClassText->SetActive(TRUE);
			m_pClassRect1->SetActive(TRUE);
			m_pClassRect2->SetActive(TRUE);
			
			m_pItemBigRect->SetActive(FALSE);
			m_pItemSmallRect1->SetActive(TRUE);
			m_pItemSmallRect2->SetActive(TRUE);
			
		}
		else
		{
			m_pClassList->SetActive(FALSE);
			m_nClass = 0;

			m_pClassText->SetActive(FALSE);
			m_pClassRect1->SetActive(FALSE);
			m_pClassRect2->SetActive(FALSE);
			
			m_pItemBigRect->SetActive(TRUE);
			m_pItemSmallRect1->SetActive(FALSE);
			m_pItemSmallRect2->SetActive(FALSE);
		}

		if(m_nType < POTION)
		{
			m_nVolume = 1;
			m_pVolumeEdit->SetEditText( AddComma(m_nVolume) );
			m_pVolumeEdit->SetDisable(TRUE);
		}
		else
		{
			m_nVolume = 0;
			m_pVolumeEdit->SetEditText( DEFAULT_VOLUME_TEXT );
			m_pVolumeEdit->SetDisable(FALSE);
		}
		
		m_nMoney = 0;
		m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	}
}
//////////////////////////////////////////////////////////////////////////

void CBuyRegDialog::UpdateItem()
{
	//기존에 선택되어진 아이템과 다른 아이템이 선택 되었다면
	//선택값을 바꾸고 등급 리스트를 리셋 한다
	if(m_nItem != m_pItemList[m_nType]->GetCurSelectedRowIdx())
	{
		m_nItem = m_pItemList[m_nType]->GetCurSelectedRowIdx();

		m_nVolume = 0;

		if(m_nType < ACCESSORY)
		{
			m_nClass = 0;
			m_pClassList->SetCurSelectedRowIdx(m_nClass);
		}
		
		if(m_nType < POTION)
			m_nVolume = 1;
	
		m_nMoney = 0;
		m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
		m_pVolumeEdit->SetEditText( AddComma(m_nVolume) );
	}
}

void CBuyRegDialog::UpdateClass()
{
	if( m_nClass != m_pClassList->GetCurSelectedRowIdx() )
	{
		m_nClass = m_pClassList->GetCurSelectedRowIdx();
		
		m_nMoney = 0;
		m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	}
}


void CBuyRegDialog::Show()
{
	SetActive(TRUE);
	GAMEIN->GetStreetBuyStallDialog()->SetDisable( TRUE );
	
	m_pTypeList->SetCurSelectedRowIdx(m_nType);

	//등급이 있는 아이템만 등급 리스트를 보여준다
	if(m_nType < ACCESSORY)
	{
		m_pClassList->SetActive(TRUE);
		m_pClassList->SetCurSelectedRowIdx(m_nClass);

		m_pClassText->SetActive(TRUE);
		m_pClassRect1->SetActive(TRUE);
		m_pClassRect2->SetActive(TRUE);
		
		m_pItemBigRect->SetActive(FALSE);
		m_pItemSmallRect1->SetActive(TRUE);
		m_pItemSmallRect2->SetActive(TRUE);
	}
	else
	{
		m_pClassList->SetActive(FALSE);
		m_nClass = 0;

		m_pClassText->SetActive(FALSE);
		m_pClassRect1->SetActive(FALSE);
		m_pClassRect2->SetActive(FALSE);
		
		m_pItemBigRect->SetActive(TRUE);
		m_pItemSmallRect1->SetActive(FALSE);
		m_pItemSmallRect2->SetActive(FALSE);
	}
	
	m_pVolumeEdit->SetDisable(FALSE);

	// 수량이 1개로 제한되는 아이템
	if(m_nType < POTION)
	{	
		m_nVolume = 1;
		m_pVolumeEdit->SetEditText( AddComma(m_nVolume) );
		m_pVolumeEdit->SetDisable(TRUE);
	}
	

	//선택된 아이템 리스트만 활성화
	for(int i = 0; i < ITEM_TYPE_COUNT; i++)
		if(i == m_nType)
		{
			m_pItemList[i]->SetActive(TRUE);
			m_pItemList[i]->SetCurSelectedRowIdx(m_nItem);
		}
		else
		{
			m_pItemList[i]->SetActive(FALSE);
		}
}

void CBuyRegDialog::InitRegInfo()
{
	m_nType = 0;
	m_nItem = 0;
	m_nClass = 0;
	m_nVolume = 0;
	m_nMoney = 0;

	m_pVolumeEdit->SetEditText( DEFAULT_VOLUME_TEXT );
	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
}

void CBuyRegDialog::SetRegInfo(BUY_REG_INFO& RegInfo)
{
	m_nType = RegInfo.Type;
	m_nItem = RegInfo.Item;
	m_nClass = RegInfo.Class;
	m_nVolume = RegInfo.Volume;
	m_nMoney = RegInfo.Money;

	m_pVolumeEdit->SetEditText( AddComma( m_nVolume ) );
	m_pMoneyEdit->SetEditText( AddComma( m_nMoney ) );
}

void CBuyRegDialog::Close()
{
	InitRegInfo();

	SetActive(FALSE);
	GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
}

DWORD CBuyRegDialog::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;
	
	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);
	
	if( !m_bDisable )
	{
		we |= ActionEventWindow(mouseInfo);
		we |= ActionEventComponent(mouseInfo);
	}
	return we;	
}

void CBuyRegDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK && lId == BRS_REGBTN )
	{
		Close();
	}
	else if( we & WE_BTNCLICK && lId == BRS_CLOSEBTN )
	{
		Close();
	}
}

DWORD CBuyRegDialog::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	
	if( m_bDisable ) return we;
	we = cDialog::ActionEventWindow( mouseInfo );
	
	if( mouseInfo->LButtonDown() )
		if( !PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
		{
			
		}
		
		return we;
}

BOOL CBuyRegDialog::GetBuyRegInfo(BUY_REG_INFO& BuyInfo)
{
	m_nVolume = (WORD)atoi(RemoveComma(m_pVolumeEdit->GetEditText()));
	m_nMoney = (DWORD)atol(RemoveComma(m_pMoneyEdit->GetEditText()));

	if(m_nVolume < 1)	// 수량이 1개 미만일때 오류 메세지
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1209));
		return FALSE;
	}
	else if(m_nVolume > 20)	// 수량이 20개 이상일때 오류 메세지
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1210));
		m_nVolume = 20;
		m_pVolumeEdit->SetEditText( MAX_VOLUME_TEXT );
		return FALSE;
	}
	else if( m_nMoney < 1)	// 금액이 0원 이하일때 오류 메세지
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1207));
		m_nMoney = 0;
		m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
		return FALSE;
	}
	else if( m_nMoney > 1000000000 )	//금액이 10억 이상일때 오류 메세지
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1208));
		m_nMoney = 0;
		m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
		return FALSE;
	}

	BuyInfo.Type = m_nType;
	BuyInfo.Item = m_nItem;
	BuyInfo.Class = m_nClass;
	BuyInfo.Volume = m_nVolume;
	BuyInfo.Money = m_nMoney;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 선택된 정보들로 아이템 정보를 찾는 함수
ITEM_INFO* CBuyRegDialog::GetSelectItemInfo()
{
	PTRLISTPOS pos = m_pItemList[m_nType]->GetListItem()->FindIndex(m_nItem);
	ITEM* pItem = (ITEM*)(m_pItemList[m_nType]->GetListItem()->GetAt(pos));

	if(!pItem)
		return NULL;

	//등급이 없는 아이템은 등급을 0으로 초기화
	//혹시나 해서 한번더!
	if(m_nType >= ACCESSORY)
		m_nClass = 0;

	if(m_nClass > 0 && m_nClass < 10)
	{
		char ItemName[128] = {0,};
		wsprintf(ItemName, "%s+%d", pItem->string, m_nClass);
		
		return ITEMMGR->FindItemInfoForName(ItemName);
	}
	else
		return ITEMMGR->FindItemInfoForName(pItem->string);
}
//////////////////////////////////////////////////////////////////////////
