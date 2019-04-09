#include "stdafx.h"
#include "cWindowSystemFunc.h"

#include "WindowIDEnum.h"
#include "interface/cWindowManager.h"
#include "globaleventfunc.h"
#include "GameIn.h"
#include "CharMake.h"
#include "./Audio/MHAudioManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "PyogukDialog.h"
#include "QuickDialog.h"
#include "MugongDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetStall.h"
#include "MonsterGuageDlg.h"
#include "DebugDlg.h"
#include "ChannelDialog.h"
#include "QuickManager.h"
#include "ItemManager.h"
#include "Item.h"
#include "MugongBase.h"
#include "ExchangeItem.h"
#include "cMsgBox.h"
#include "cImeEx.h"

#include "CharMakeDialog.h"
#include "MainTitle.h"
#include "ServerListDialog.h"

#include "MenuSlotDialog.h"
#include "PKManager.h"

#ifdef TAIWAN_LOCAL
#include "ChinaAdviceDlg.h"
#endif

#ifdef _KOR_LOCAL_
#include "OptionManager.h"
#endif

// 
// Drag & drop Process callback Function
// 
void cbDragDropProcess(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY)
{
	cDialog * srcDlg = WINDOWMGR->GetWindowForID(id);
	if(!srcDlg) return;
	ASSERT(srcDlg);

	cDialog * ptDlg = WINDOWMGR->GetWindowForXYExceptIcon(beforeDragX,beforeDragY);
	cDialog * ptardlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
	if(ptDlg && !ptardlg)
	{
		if( ptDlg->GetType() == WT_ITEMSHOPDIALOG )
		{
			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
//---임시 : 진군에게 물어보기
			if(!srcDlg->GetParent())
				return;
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
			WINDOWMGR->SetMouseInputProcessed();
			return;
		}
	}

//	srcDlg->ExcuteCBFunc( WE_DRAGEND );
	
	if(srcDlg->GetType() & WT_ICON) // 아이콘타입이다
	{
		//////////////////////////////////////////////////////////////////////////
		// YHEDIT [8/27/2003]
		WORD wtype = srcDlg->GetType();
		if( !(wtype == WT_ITEM ||
			wtype == WT_MUGONG ||
			wtype == WT_JINBUB ||
			wtype == WT_QUICKITEM ||
			wtype == WT_EXCHANGEITEM ||
			wtype == WT_STALLITEM ||
			wtype == WT_DEALITEM ||
			wtype == WT_ABILITYICON) )
		{
			ASSERT(0);
		}
		cIcon* pIcon = (cIcon*)srcDlg;
		cDialog * targetdlg = WINDOWMGR->GetWindowForXYExceptIcon(curDragX,curDragY);
		if(targetdlg)
		{
			// quest 땜시... jsd(2004.09.14)			
			if( pIcon->CanMoveToDialog(targetdlg) == FALSE )
			{
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
			}
			else
			{				
				if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				{
					//되돌림
					//WINDOWMGR->BackDragWindow();
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
					
				}
				switch(targetdlg->GetType())
				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}				
			}
		}
		else
		{
			// drop in GROUND
			// or dump icon and item, mugong icon
			if( pIcon->CanDelete() )
			{
				if(GAMEIN->GetGameInInitKind() != eGameInInitKind_SuryunEnter)
					SendDeleteIconMsg((cIcon*)srcDlg);
			}
			else
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));

			srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
		}
/*		if(targetdlg)
		{
			// jds quest item (2004.09.08)
			if( ((CItem*)srcDlg)->GetItemKind() != eQUEST_ITEM_EQUIP )
			{
				if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
				{
					//되돌림
					//WINDOWMGR->BackDragWindow();
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
					
				}
				switch(targetdlg->GetType())
				{
				case WT_QUICKDIALOG:
					{
						AUDIOMGR->Play(61, HERO);
					}
					break;
				case WT_INVENTORYDIALOG:
					break;
				default:
					{
						AUDIOMGR->Play(58, HERO);
					}
				}
			}
			else
			{
				if( targetdlg->GetType() == WT_INVENTORYDIALOG )
				{
					if(!targetdlg->FakeMoveIcon(curDragX,curDragY,(cIcon*)srcDlg))
					{
						srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
					}					
				}
				else
				{
					srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
					WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
				}
			}				
		}
		else
		{
			// jds quest item (2004.09.08)
			if( ((CItem*)srcDlg)->GetItemKind() != eQUEST_ITEM_EQUIP )
			{
				// drop in GROUND
				// or dump icon and item, mugong icon
				SendDeleteIconMsg((cIcon*)srcDlg);
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필
			}
			else
			{
				srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);
				WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(652));
			}
		}
		*/
		//////////////////////////////////////////////////////////////////////////

		// NACK가 왔을 때 원래 자리로 되돌림
		//srcDlg->SetAbsXY((LONG)beforeDragX, (LONG)beforeDragY);	// SetDragEnd()가 있는데 필요한가???체크필

		ASSERT(srcDlg->GetParent());
		if(srcDlg->GetParent())		//임시다!!! 퀵아이이템 에러다. 이런경우 안생기게 수정하시오!
		{
			if(!srcDlg->GetParent()->IsActive())
				srcDlg->SetActive(FALSE);

			srcDlg->SetDepend(TRUE);
		}
		else if( srcDlg->GetType() == WT_QUICKITEM )
		{
			QUICKMGR->ReleaseQuickItem( (CQuickItem*)srcDlg );
		}

		WINDOWMGR->SetMouseInputProcessed();
	}
}

void SendDeleteIconMsg(cIcon * pIcon)
{
	if(pIcon->GetType() == WT_ITEM)
	{
		if( PKMGR->IsPKLooted() ) return;
		// 착용아이템 버리지 못하게...
        if( ITEMMGR->FakeDeleteItem((CItem *)pIcon) == FALSE )
			return;
		WINDOWMGR->MsgBox( MBI_DELETEICONCHECK, MBT_YESNO, CHATMGR->GetChatMsg(363), ((CItem *)pIcon)->GetItemInfo()->ItemName );
		GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		GAMEIN->GetPyogukDialog()->SetDisable( TRUE );
	//	GAMEIN->GetGuildDlg()->SetDisable( TRUE );
	}
	else if(pIcon->GetType() == WT_QUICKITEM)
	{
		GAMEIN->GetQuickDialog()->FakeDeleteQuickItem( (CQuickItem *)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
	else if(pIcon->GetType() == WT_MUGONG || pIcon->GetType() == WT_JINBUB )
	{
		GAMEIN->GetMugongDialog()->FakeDeleteMugong(((CMugongBase*)pIcon));
		WINDOWMGR->MsgBox(MBI_MUGONGDELETE, MBT_YESNO, CHATMGR->GetChatMsg(190));
		GAMEIN->GetMugongDialog()->SetDisable( TRUE );
	}
	else if( pIcon->GetType() == WT_EXCHANGEITEM )
	{
		GAMEIN->GetExchangeDialog()->FakeDeleteItem(((CExchangeItem*)pIcon)->GetPosition());
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_STALLITEM )
	{
		GAMEIN->GetStreetStallDialog()->FakeDeleteItem( ((CExchangeItem*)pIcon)->GetPosition() );
		AUDIOMGR->Play(67, HERO);
	}
	else if( pIcon->GetType() == WT_MENUICON )
	{
		GAMEIN->GetMenuSlotDialog()->FakeDeleteIcon( (CMenuIcon*)pIcon );
		AUDIOMGR->Play(67, HERO);
	}
}

//
// Window Create and WindowSystem add Func
//
// 
void CreateMainTitle_m()
{
#ifdef _FILE_BIN_
//	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/IDDlg.bin", "rb");
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/IDDlg.bin", "rb");
#else
	cWindow* window = WINDOWMGR->GetDlgInfoFromFile("./image/IDDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);

	cEditBox* pEdit;
	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_IDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitID);

#ifdef _KOR_LOCAL_
	pEdit->SetValidCheck( VCM_DEFAULT );
#else
	pEdit->SetValidCheck( VCM_ID );
#endif

	pEdit = (cEditBox*)((cDialog*)window)->GetWindowForID( MT_PWDEDITBOX );
	if(GAMERESRCMNGR->m_GameDesc.LimitDay != 0 && GAMERESRCMNGR->m_GameDesc.LimitDay >= GetCurTimeValue())
		pEdit->SetEditText(GAMERESRCMNGR->m_GameDesc.LimitPWD);
	pEdit->SetValidCheck( VCM_PASSWORD );	

// magi82 - jjangfile login
#ifdef _KOR_LOCAL_
	cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);
	pCombo->SelectComboText(OPTIONMGR->GetGameOption()->nLoginCombo);
	pCombo->SetCurSelectedIdx(OPTIONMGR->GetGameOption()->nLoginCombo);	
#endif

#ifndef TAIWAN_LOCAL
#ifdef _FILE_BIN_
//	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/copyright.bin", "rb");
	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/copyright.bin", "rb");
#else
	cWindow* window1 = WINDOWMGR->GetDlgInfoFromFile("./image/copyright.txt");
#endif
	WINDOWMGR->AddWindow(window1);

#else	//TAIWAN_LOCAL
#ifdef	_FILE_BIN_
	cWindow* window2 = WINDOWMGR->GetDlgInfoFromFile( "./image/CNAdviceDlg.bin", "rb" );
#else
	cWindow* window2 = WINDOWMGR->GetDlgInfoFromFile( "./image/CNAdviceDlg.txt" );
#endif

	((CChinaAdviceDlg*)window2)->Linking();
	WINDOWMGR->AddWindow( window2 );
#endif
	
#ifdef	_FILE_BIN_
//	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/ServerListDlg.bin", "rb" );
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/ServerListDlg.bin", "rb" );
#else
	cWindow* window3 = WINDOWMGR->GetDlgInfoFromFile( "./image/ServerListDlg.txt" );
#endif

	WINDOWMGR->AddWindow( window3 );

	// dynamic dlg
#ifdef TAIWAN_LOCAL
#ifdef	_FILE_BIN_
	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.bin", "rb" );
#else
	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.txt" );
#endif
	WINDOWMGR->AddWindow( window4 );
#elif defined _HK_LOCAL_
#ifdef	_FILE_BIN_
	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.bin", "rb" );
#else
	cWindow* window4 = WINDOWMGR->GetDlgInfoFromFile( "./image/Dynamic.txt" );
#endif
	WINDOWMGR->AddWindow( window4 );
#endif

#ifdef	_FILE_BIN_
	cWindow* window5 = WINDOWMGR->GetDlgInfoFromFile( "./image/InterfaceScript/IntroReplay.bin", "rb" );
#else
	cWindow* window5 = WINDOWMGR->GetDlgInfoFromFile( "./image/IntroReplay.txt" );
#endif

	WINDOWMGR->AddWindow( window5 );

	// 2008. 3. 17. CBH - 보안번호 다이얼로그
	WINDOWMGR->CreateNumberPadDlg();
}


void NewCreateCharSelect_m()
{
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharSelectDlg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharSelectDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharSelectDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);

	// taiyo interface com
	//cWindow * window1 = WINDOWMGR->GetDlgInfoFromFile("./image/NewCharSelect/CharSelectMessage.txt");
	//WINDOWMGR->AddWindow(window1);

}
void CreateCharMakeDlg()
{
//#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeDlg.bin", "rb");
//#else
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeDlg.txt");
//#endif

//SW050811
//#ifdef _JAPAN_LOCAL_
//#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeNewDlg_JP.bin", "rb");
//#else
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeNewDlg_JP.txt");
//#endif

//#else
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeNewDlg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/CharMakeNewDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMakeNewDlg.txt");
#endif
//#endif
	
	WINDOWMGR->AddWindow(window);

	CHARMAKE->SetCharMakeDialog((cCharMakeDlg*)window);
	CHARMAKE->GetCharMakeDialog()->Linking();

	cEditBox* pEdit = (cEditBox*)CHARMAKE->GetCharMakeDialog()->GetWindowForID( CMID_IDEDITBOX );
	pEdit->SetValidCheck( VCM_CHARNAME );
}

void CharMessageDlg_m()
{
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/CharMessage.txt");
//	WINDOWMGR->AddWindow(window);

}
cWindow * CreateGameLoading_m()
{
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/NewLoadDlg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/NewLoadDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/NewLoadDlg.txt");
#endif
	WINDOWMGR->AddWindow(window);

	return window;
}

//KES LOGO 031107
void CreateInitLogo_m()
{
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InitDlg.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/InitDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InitDlg.txt");
#endif

	//SW061111 저해상도옵션
	/*if( GAMERESRCMNGR->IsLowResolution() )
	{
		cImageScale scale;
		float scaleRate = (float)LOWRSLTN_W / MIDRSLTN_W;
		
		scale.x = scaleRate;
		scale.y = scaleRate;

		window->SetScale(&scale);
	}*/	//기본 창 크기부터가 해상도 크기와 맞지가 않아 제외. 스크립트에서 처리.

	WINDOWMGR->AddWindow(window);
}



void CreateMoneyDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/moneyDlg.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/moneyDlg.txt");
#endif
	//window->SetID();
	WINDOWMGR->AddWindow(window);
	//GAMEIN->SetMoneyDialog((CMoneyDlg * )window);
}

void CreateMonsterGuageDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/51.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/51.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/51.txt");
#endif
	WINDOWMGR->AddWindow(window);
	GAMEIN->SetMonsterGuageDlg((CMonsterGuageDlg * )window);
	GAMEIN->GetMonsterGuageDlg()->Linking();
}

void CreateChannelDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/Channel.bin", "rb");
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/Channel.bin", "rb");
#else
	cWindow * window = WINDOWMGR->GetDlgInfoFromFile("./image/Channel.txt");
#endif

	WINDOWMGR->AddWindow(window);

#ifdef _KOR_LOCAL_
	CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
	if(pChannelDlg)
	{
		pChannelDlg->Linking();
	}
#endif	
}
