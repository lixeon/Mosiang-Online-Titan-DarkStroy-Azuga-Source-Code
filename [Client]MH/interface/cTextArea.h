
// cTextArea.h: interface for the cTextArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXTAREA_H__59295534_FC7D_4D12_BA33_ABE97B827CD0__INCLUDED_)
#define AFX_CTEXTAREA_H__59295534_FC7D_4D12_BA33_ABE97B827CD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cDialog.h"

class cIMEex;

class cTextArea : public cDialog
{

protected:

	cButton*	m_pUpwardBtn;
	cButton*	m_pDownwardBtn;
	cGuageBar*	m_pGuageBar;

	cIMEex*		m_pIMEex;

	int			m_nTopLineIdx;
	int			m_nLineNum;
	int			m_nLineHeight;
	RECT		m_rcTextRelRect;

	BOOL		m_bReadOnly;
	
	BOOL		m_bCaret;

	DWORD		m_dwCaretCurTick;
	DWORD		m_dwCaretLastTick;
	
	cImage m_TopImage;			WORD m_topHeight;
	cImage m_MiddleImage;		WORD m_middleHeight;
	cImage m_DownImage;			WORD m_downHeight;

	cImageScale m_MiddleScale;

	BOOL		m_bUseBorderImage;

	DWORD		m_dwTextColor;

public:

	cTextArea();
	virtual ~cTextArea();

	void InitTextArea( RECT* pTextRelRect, int nBufSize, cImage* topimg, WORD tophei, cImage* middleimg, WORD middlehei, cImage* downimg, WORD downhei );
	void InitTextArea( RECT* pTextRelRect, int nBufSize );
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	virtual void Add(cWindow * window);
	
	virtual void SetActive( BOOL val );

	virtual void SetFocusEdit(BOOL val);
	void SetFocus( BOOL val );
	void SetMiddleScale( cImageScale scale ) { m_MiddleScale = scale; }
	
	void OnUpwardItem();
	void OnDownwardItem();
	
	void GetScriptText(char * outText);
	void SetScriptText(char * inText);

	void SetReadOnly(BOOL val) { m_bReadOnly = val;	}
	
	BOOL SetLimitLine( int nMaxLine );
	
	void SetCaretMoveFirst();

	void SetEnterAllow( BOOL bAllow );
	
//	void SetValidCheck( int nSetNum );

	void GetCaretPos( LONG* X, LONG* Y );
	void SetTextColor( DWORD dwColor )	{ m_dwTextColor = dwColor; }
};


#endif // !defined(AFX_CTEXTAREA_H__59295534_FC7D_4D12_BA33_ABE97B827CD0__INCLUDED_)
