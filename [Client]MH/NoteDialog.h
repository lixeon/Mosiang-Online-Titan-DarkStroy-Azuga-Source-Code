// NoteDialog.h: interface for the CNoteDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTEDIALOG_H__4E8E5805_6BD3_4FDC_9675_86F2F2D9F58D__INCLUDED_)
#define AFX_NOTEDIALOG_H__4E8E5805_6BD3_4FDC_9675_86F2F2D9F58D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"
#include ".\interface\cTextArea.h"


class CNoteDialog : public cDialog  
{
	cListCtrl* m_pNoteListLCtrl;
	
	cButton* m_pNotePageBtn[MAX_NOTE_PAGE];
	cButton* m_pWriteNoteBtn;
	cButton* m_pDelNoteBtn;
	cPushupButton* m_pNoteBtn;
	cPushupButton* m_pPsNoteBtn;
	
	cCheckBox* m_pNoteChk[NOTENUM_PER_PAGE];
	cCheckBox* m_pNoteChkAll;

	WORD m_SelectedNotePge;
	WORD m_CurNoteMode;

	DWORD m_CurNoteID;

public:
	CNoteDialog();
	virtual ~CNoteDialog();
	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive( BOOL val );

	void SetNoteList(MSG_FRIEND_NOTE_LIST* pmsg);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void Linking();
	void CheckedNoteID();
	void RefreshDlg();

	WORD GetSelectedNotePge() { return m_SelectedNotePge;	}
	void SetSelectedNotePge(WORD FPge) { m_SelectedNotePge = FPge;	}

	void GetCheckedNoteIDs(int count,cCheckBox** pChkBtnArray,cListCtrl* pListCtrl,DWORD* pOutIDs);
	void SelectedNoteIDChk(int count, LONG ChkBtnIDStar,cListCtrl* pListCtrl,DWORD* IDs);

//	void ShowNoteMode(int mode);
	void SetMode(WORD mode);
	WORD GetMode()			{ return m_CurNoteMode;	}
	void ShowNotePageBtn(BYTE TotalPage);
	
	DWORD GetCurNoteID() { return m_CurNoteID; };
	void SetCurNoteID(DWORD id) { m_CurNoteID = id; };

	void SetChkAll();
};

#endif // !defined(AFX_NOTEDIALOG_H__4E8E5805_6BD3_4FDC_9675_86F2F2D9F58D__INCLUDED_)
