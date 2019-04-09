// MiniNoteDialog.cpp: implementation of the CMiniNoteDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniNoteDialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include "cImeEx.h"
#include ".\interface\cTextArea.h"
#include ".\interface\cEditBox.h"
#include ".\interface\cStatic.h"

#include "ItemManager.h"
#include "ChatManager.h"

#include "MHFile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniNoteDialog::CMiniNoteDialog()
{
	m_type = WT_MININOTEDLG;
	m_CurMiniNoteMode = -1;
	m_SelectedNoteID = 0;
	m_SetitemNameTable.Initialize( 10 );
}

CMiniNoteDialog::~CMiniNoteDialog()
{
	for(int n=0;n<eMinNoteMode_Max;++n)
	{
		m_MinNoteCtlListArray[n].RemoveAll();
	}

	SETSHOPITEM* pItem = NULL;
	m_SetitemNameTable.SetPositionHead();
	while( pItem = m_SetitemNameTable.GetData() )
	{
		delete pItem;
	}
	m_SetitemNameTable.RemoveAll();
}

void CMiniNoteDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_MININOTEDLG;

	LoadSetShopItemList();
}

void CMiniNoteDialog::Linking()
{
	m_pRTitleStc	= (cStatic*)GetWindowForID(NOTE_MRTITLE);	//제목
	m_pWTitleStc	= (cStatic*)GetWindowForID(NOTE_MWTITLE);	//제목

	//읽기 
	m_pRNoteText = (cTextArea*)GetWindowForID(NOTE_MRNOTETEXTREA);
	m_pRNoteText->SetEnterAllow(FALSE);
	m_pSender = (cStatic*)GetWindowForID(NOTE_MSENDER); //보낸이 : 
	m_pSenderStc = (cStatic*)GetWindowForID(NOTE_MSENDERSTC); //아무개
	m_pReplayBtn = (cButton*)GetWindowForID(NOTE_MREPLYBTN);
	m_pDeleteBtn = (cButton*)GetWindowForID(NOTE_MDELETEBTN);
	
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pRTitleStc);
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pRNoteText);
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pSender);
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pSenderStc);
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pReplayBtn);
	m_MinNoteCtlListArray[eMinNoteMode_Read].AddTail(m_pDeleteBtn);

	//쓰기
	m_pWNoteText = (cTextArea*)GetWindowForID(NOTE_MWNOTETEXTREA);
	m_pWNoteText->SetEnterAllow(FALSE);
	m_pReceiver = (cStatic*)GetWindowForID(NOTE_MRECEIVER); //받는이 :
	

	m_pMReceiverEdit = (cEditBox*)GetWindowForID(NOTE_MRECEIVEREDIT); //아무개
	m_pMReceiverEdit->SetValidCheck( VCM_CHARNAME );
	m_pMReceiverEdit->SetEditText("");
	
	m_pSendOkBtn = (cButton*)GetWindowForID(NOTE_MSENDOKBTN);
	m_pSendCancelBtn = (cButton*)GetWindowForID(NOTE_MSENDCANCELBTN); 
	


	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pWTitleStc);
	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pWNoteText);
	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pMReceiverEdit);
	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pSendOkBtn);
	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pSendCancelBtn);
	m_MinNoteCtlListArray[eMinNoteMode_Write].AddTail(m_pReceiver);
}


void CMiniNoteDialog::LoadSetShopItemList()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if( !file.Init( "./Image/Itemidx_Setitem.bin", "rb" ) )
		return;
#else
	if( !file.Init( "./Image/Itemidx_Setitem.txt", "rt" ) )
		return;
#endif

	WORD Count = file.GetWord();	

	for(int i=0; i<Count; i++)
	{
		SETSHOPITEM* pItem = new SETSHOPITEM;
		
		pItem->ItemIdx = file.GetDword();
//		file.GetString( pItem->Name );
		SafeStrCpy( pItem->Name, file.GetString(), MAX_NAME_LENGTH+1 );
		
		m_SetitemNameTable.Add( pItem, pItem->ItemIdx );
	}
}


void CMiniNoteDialog::ShowMiniNoteMode(int mode)
{
	if(m_CurMiniNoteMode == mode)
		return;

	if(m_CurMiniNoteMode != -1)
	{
		SetActiveMiniNoteMode(m_CurMiniNoteMode,FALSE);		
	}

	SetActiveMiniNoteMode(mode,TRUE);
	m_CurMiniNoteMode = mode;
}

void CMiniNoteDialog::SetMode(int mode)
{
	m_CurMiniNoteMode = mode;
}

void CMiniNoteDialog::SetActiveMiniNoteMode(int mode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_MinNoteCtlListArray[mode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}


void CMiniNoteDialog::SetMiniNote(char* Sender, char* Note, WORD ItemIdx)
{
	char buf[300] = { 0, };	
	//
	if( ItemIdx > 0 )
	{
		SETSHOPITEM* pItem = m_SetitemNameTable.GetData( ItemIdx );
		if( pItem )
		{
			sprintf( buf, CHATMGR->GetChatMsg(732), pItem->Name );
		}
		else
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
			if( pItemInfo )
				sprintf( buf, CHATMGR->GetChatMsg(732), pItemInfo->ItemName );
		}
	}

	strcat( buf, Note );
	m_pSenderStc->SetStaticText(Sender);
	m_pRNoteText->SetCaretMoveFirst();
	m_pRNoteText->SetScriptText(buf);
	m_pMReceiverEdit->SetEditText(Sender);
	m_pRNoteText->SetCaretMoveFirst();
	m_pWNoteText->SetScriptText(buf);
}


int CMiniNoteDialog::GetCurMode()
{
	return m_CurMiniNoteMode;
}

void CMiniNoteDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	if(val == TRUE)
	{
		m_pWNoteText->SetScriptText("");
		m_pMReceiverEdit->SetEditText("");		
	}
	else
	{
		m_pMReceiverEdit->SetFocusEdit(FALSE);
		m_pWNoteText->SetFocusEdit(FALSE);
	}
	cDialog::SetActive(val);
}

char* CMiniNoteDialog::GetSenderName()
{
	return m_pSenderStc->GetStaticText();
}