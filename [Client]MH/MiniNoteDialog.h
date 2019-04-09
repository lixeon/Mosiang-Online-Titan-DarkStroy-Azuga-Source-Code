// MiniNoteDialog.h: interface for the CMiniNoteDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MININOTEDIALOG_H__11F671E3_42AB_4140_945A_71A1FEABEDDA__INCLUDED_)
#define AFX_MININOTEDIALOG_H__11F671E3_42AB_4140_945A_71A1FEABEDDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"

class cStatic;
class cTextArea;
class cButton;
class cEditBox;
struct SETSHOPITEM;

enum eMiniNote_Mode
{
	eMinNoteMode_Read,
	eMinNoteMode_Write,

	eMinNoteMode_Max
};


class CMiniNoteDialog : public cDialog  
{
	cStatic *	m_pRTitleStc;
	cStatic *	m_pWTitleStc;
	cTextArea * m_pWNoteText;
	cTextArea * m_pRNoteText;
	cButton *	m_pSendOkBtn;
	cButton *	m_pSendCancelBtn;
	cButton *	m_pReplayBtn;
	cButton *	m_pDeleteBtn;
	cEditBox *	m_pMReceiverEdit; //¹Þ´ÂÀÌ ÀÌ¸§
	cStatic *	m_pReceiver; //¹Þ´ÂÀÌ : 
	cStatic *	m_pSender; //º¸³»´Â ÀÌ :
	cStatic *	m_pSenderStc; //º¸³»´ÂÀÌ ÀÌ¸§

	DWORD m_SelectedNoteID;

	cPtrList m_MinNoteCtlListArray[eMinNoteMode_Max];

	int m_CurMiniNoteMode;
	
	void SetMode(int mode);
	void SetActiveMiniNoteMode(int mode,BOOL bActive);
	
	// 
	CYHHashTable<SETSHOPITEM>		m_SetitemNameTable;

public:
	CMiniNoteDialog();
	virtual ~CMiniNoteDialog();
	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive(BOOL val);
	void Linking();
	void ShowMiniNoteMode(int mode);
	void SetMiniNote(char* Sender, char* Note, WORD ItemIdx);
	int GetCurMode();
	char* GetSenderName();

	DWORD GetNoteID() { return m_SelectedNoteID; }
	void SetNoteID(DWORD NoteID) { m_SelectedNoteID = NoteID;	}
	void LoadSetShopItemList();
};

#endif // !defined(AFX_MININOTEDIALOG_H__11F671E3_42AB_4140_945A_71A1FEABEDDA__INCLUDED_)
