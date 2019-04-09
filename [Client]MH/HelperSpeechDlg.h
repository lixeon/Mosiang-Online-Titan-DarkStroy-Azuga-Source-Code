#ifndef _HELPER_SPEECH_DIALOG_
#define _HELPER_SPEECH_DIALOG_

#include "./Interface/cDialog.h"
#include "./Interface/cListItem.h"
#include "./Interface/cWindowHeader.h"
#include "BalloonOutline.h"
#include "PtrList.h"

class cPageBase;

class cHelperSpeechDlg : public cDialog, public cListItem, public cBalloonOutline
{
protected:
	BOOL			m_bUseComponent;

//	cButton *		m_pNextBtn;
//	cButton *		m_pPrevBtn;
//	cButton *		m_pExitBtn;

	RECT			m_textRelRect;	// ÅØ½ºÆ® ¿µ¿ª..
	int				m_nLineHeight;	// ¶óÀÎÀÇ ³ôÀÌ°ª
	int				m_nLineNum;		// ¶óÀÎÀÇ °¹¼ö

	cPageBase*		m_pCurPage;

	VECTOR2			m_vHelperPos;

	BOOL			m_bFadeIn;
	BOOL			m_bFadeOut;
	BOOL			m_bClose;

	DWORD			m_dwStartTime;
	DWORD			m_dwCurTime;

	cPtrList		m_NextPagelist;

	void Exit();

public:
	cHelperSpeechDlg();
	virtual ~cHelperSpeechDlg();

	virtual void	Init( int nx, int ny, int nwid, int nhei, int nlinehei, LONG ID );
	virtual DWORD	ActionEvent(CMouse * mouseInfo);

	virtual void	Render();
	virtual void	RenderComponent();
	virtual void	Linking();

	BOOL			OpenDialog( DWORD dwPageId );
	void			ResetDialog();
	void			CloseDialog();

	void	AddPage( DWORD dwPageId );// { m_NextPagelist.AddTail((void*)dwPageId); }
	void	UseComponent( BOOL bUse ) { m_bUseComponent = bUse; } // µî·Ï ComponentÀÇ »ç¿ë¿©ºÎ..

	void	SetHelperPos( float x, float y ) { m_vHelperPos.x = x; m_vHelperPos.y = y; }
	void	SetTextRect(RECT* rt) { CopyRect(&m_textRelRect, rt); }

	BOOL	StartFadeOut( DWORD dwNextIdx );
	void	StartFadeIn();
	
	DWORD	GetCurPageIdx();
	
	//SW050816 For CharMake
	BOOL	IsListEmpty() {return m_NextPagelist.IsEmpty();}
};

#endif //_HELPER_SPEECH_DIALOG_
