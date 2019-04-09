// NoteDialog.cpp: implementation of the CNoteDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoteDialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include "NoteManager.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "cRitemEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoteDialog::CNoteDialog()
{
	m_type = WT_NOTEDLG;
	m_SelectedNotePge = 1;
	m_CurNoteMode = eMode_NormalNote;
	m_CurNoteID = 0;
}

CNoteDialog::~CNoteDialog()
{

}

void CNoteDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_NOTEDLG;	
}

void CNoteDialog::Linking()
{
	//////////////////////////////////////////////////////////////////////////
	//eNoteMode_List
	m_pWriteNoteBtn = (cButton*)GetWindowForID(NOTE_WRITENOTEBTN);
	m_pDelNoteBtn = (cButton*)GetWindowForID(NOTE_DELNOTEBTN);
	m_pNoteBtn = (cPushupButton*)GetWindowForID(NOTE_TABBTN1);
	m_pPsNoteBtn = (cPushupButton*)GetWindowForID(NOTE_TABBTN2);

	m_pNoteListLCtrl = (cListCtrl*)GetWindowForID(NOTE_NOTELISTLCTL);
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		m_pNoteChk[i] = (cCheckBox*)GetWindowForID(NOTE_NOTELISTCHK1+i);
	}
	m_pNoteChkAll = (cCheckBox*)GetWindowForID(NOTE_NOTELISTCHK11);

	for(i=0; i<MAX_NOTE_PAGE; ++i)
	{
		m_pNotePageBtn[i] = (cButton*)GetWindowForID(NOTE_PAGE1BTN+i);
	}

	SetMode( eMode_NormalNote );

	//////////////////////////////////////////////////////////////////////////
}

void CNoteDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
	{
		pDlg->SetPushBarIcon( OPT_NOTEDLGICON, m_bActive );
		if( val == TRUE )
		{
			pDlg->SetAlram( OPT_NOTEDLGICON, FALSE );
		}
	}
			
	if( !val )
		SetMode( eMode_NormalNote );
	else
	{
		if( m_pNoteChkAll )
			m_pNoteChkAll->SetChecked( FALSE );
	}
}


void CNoteDialog::SetMode(WORD mode)
{
	m_CurNoteMode = mode;
	
	if( mode == eMode_NormalNote )
	{
#ifndef TAIWAN_LOCAL
		m_pNoteBtn->SetPush( TRUE );
		m_pPsNoteBtn->SetPush( FALSE );
#endif
	}
	else
	{
#ifndef TAIWAN_LOCAL
		m_pNoteBtn->SetPush( FALSE );
		m_pPsNoteBtn->SetPush( TRUE );
#endif
	}
}


void CNoteDialog::SetNoteList(MSG_FRIEND_NOTE_LIST* pmsg)
{
	ShowNotePageBtn(pmsg->TotalPage);

	m_pNoteListLCtrl->DeleteAllItems();
	
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		m_pNoteChk[i]->SetChecked(FALSE);
		m_pNoteChk[i]->SetActive(FALSE);

		if(pmsg->NoteList[i].NoteID != 0)
		{
			cRITEMEx *ritem = new cRITEMEx(2);
			
			//이름 6글자 까지 보여준다. 
			char Name[13] = {0,};
			int Len = strlen( pmsg->NoteList[i].FromName ); 
			if(Len > 12)
			{
				strncpy( Name, pmsg->NoteList[i].FromName, 12 );
				Name[12] = 0;
				strcpy(ritem->pString[0],Name);
			}
			else 
				strcpy(ritem->pString[0],pmsg->NoteList[i].FromName);

			sprintf(ritem->pString[1], pmsg->NoteList[i].SendDate);
			
			DWORD color = TTTC_DEFAULT;
			if( pmsg->NoteList[i].bIsRead )
				color = RGBA_MAKE( 175, 178 ,192 ,255);

			for( int r=0; r<2; r++)
				ritem->rgb[r] = color;

			ritem->dwID = pmsg->NoteList[i].NoteID; 
			m_pNoteListLCtrl->InsertItem(i, ritem);
			m_pNoteChk[i]->SetActive(TRUE);
		}
	}

	SetActive(TRUE);
}

void CNoteDialog::RefreshDlg()
{

}

void CNoteDialog::CheckedNoteID()
{
	DWORD IDs[NOTENUM_PER_PAGE];
	SelectedNoteIDChk(NOTENUM_PER_PAGE,NOTE_NOTELISTCHK1,m_pNoteListLCtrl,IDs);	
	
	int lastidx = 0;
	for(int i=0;i<NOTENUM_PER_PAGE;++i)
	{
		if(IDs[i] != 0)
			lastidx = i;
	}
	
	for(i=0;i<NOTENUM_PER_PAGE;++i)
	{
		if(IDs[i] != 0)
		{
			BOOL bLast;
			if(i==lastidx)
				bLast = TRUE;
			else
				bLast = FALSE;
			NOTEMGR->DelNoteSyn(IDs[i],bLast);
		}
	} 
}

void CNoteDialog::SelectedNoteIDChk(int count, LONG ChkBtnIDStar,cListCtrl* pListCtrl,DWORD* IDs)
{
	cCheckBox* pChkArray[NOTENUM_PER_PAGE];
	for(int i=0; i<count; ++i)
	{
		pChkArray[i] = (cCheckBox*)GetWindowForID(ChkBtnIDStar+i);
	}
	GetCheckedNoteIDs(count,pChkArray,pListCtrl,IDs);
}


void CNoteDialog::GetCheckedNoteIDs(int count,cCheckBox** pChkBtnArray,cListCtrl* pListCtrl,DWORD* pOutIDs)
{
	memset(pOutIDs,0,sizeof(DWORD)*count);
	for(int i=0; i<count; ++i)
	{	
		cCheckBox* Chk = pChkBtnArray[i];
		
		if(Chk->IsChecked())
		{
			cRITEMEx * rItem = (cRITEMEx *)pListCtrl->GetRItem(i);
			if(!rItem) return;
			if(rItem->dwID)
			{
				pOutIDs[i] = rItem->dwID;
			}
		}
	}
}


DWORD CNoteDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_ROWCLICK)
	{
		cRITEMEx * pRItem = (cRITEMEx *)m_pNoteListLCtrl->GetRItem(m_pNoteListLCtrl->GetSelectedRowIdx());
		if(pRItem != NULL)
		{	
			NOTEMGR->ReadNote(pRItem->dwID, 1);
		}
	}
	return we;
}

void CNoteDialog::ShowNotePageBtn(BYTE TotalPage)
{
	if(TotalPage == 0)
	{
		for(BYTE i=0; i<MAX_NOTE_PAGE; ++i)
		{
			if( m_pNotePageBtn[i] )
				m_pNotePageBtn[i]->SetActive(FALSE);
		}
		return;
	}
	
	for(DWORD i=0; i<MAX_NOTE_PAGE; ++i)
	{
		if( m_pNotePageBtn[i] )
		{
			m_pNotePageBtn[i]->SetTextValue(i+1);
			m_pNotePageBtn[i]->SetActive(i+1 <= TotalPage);
		}
	}
}

void CNoteDialog::SetChkAll()
{
	if( m_pNoteChkAll == NULL ) return;

	BOOL Chk = m_pNoteChkAll->IsChecked();
	
	for(int i=0; i<NOTENUM_PER_PAGE; ++i)
	{
		if( m_pNoteChk[i]->IsActive() )		
			m_pNoteChk[i]->SetChecked( Chk );
	}
}
