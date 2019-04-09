//
//			New Dialog Bar
//
//			2003/01/24
//			Trust Pak



#include "StdAfx.h"
#include "ToolInfo.h"


#ifndef _CUSTOM_TOOL_DIALOG_BY_TRUSTPAK_
#define _CUSTOM_TOOL_DIALOG_BY_TRUSTPAK_


//----------------------------------------------------------------------------
class CNewDialogBar : public CDialogBar
{
public:
	CNewDialogBar();
	~CNewDialogBar();

	BOOL CreatNewDialogBar(CWnd* pWnd);

	BOOL UpdateDialogBar(VOID);
	BOOL SetScrollBar(INT iMin, INT iMax, INT iCurrent);
	BOOL SetFrameStatic(VOID);	

	BOOL SetPlaySoundButton(BOOL bCheck);


	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

extern CNewDialogBar* g_pDlgBar;


//----------------------------------------------------------------------------



#endif