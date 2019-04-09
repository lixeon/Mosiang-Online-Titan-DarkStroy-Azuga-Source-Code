//
//			New Dialog Bar
//
//			2003/01/24
//			Trust Pak


#include "StdAfx.h"
#include "DialogBar.h"
#include "EngineRelation.h"
#include "Resource.h"

CNewDialogBar* g_pDlgBar = NULL;

//////////////////////////////////////////////////////////////////////////////
// Class Name : CNewDialogBar
// Description :
//
//----------------------------------------------------------------------------
// Name : CNewDialogBar::CNewDialogBar()
// Description : Contructor.
//----------------------------------------------------------------------------
CNewDialogBar::CNewDialogBar()
{
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::~CNewDialogBar()
// Description : Destructor.
//----------------------------------------------------------------------------
CNewDialogBar::~CNewDialogBar()
{
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::CreateNewDialogBar
// Description : 
//----------------------------------------------------------------------------
BOOL CNewDialogBar::CreatNewDialogBar(CWnd* pWnd)
{
	Create(pWnd, IDD_DIALOGBAR, CBRS_TOP | WS_HSCROLL , IDD_DIALOGBAR);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::UpdateDialogBar(VOID)
// Description : Contructor.
//----------------------------------------------------------------------------
BOOL CNewDialogBar::UpdateDialogBar(VOID)
{
	SetScrollBar(g_ToolInfo.m_iScrollMin,
				 g_ToolInfo.m_iScrollMax,
				 g_ToolInfo.m_iScrollPos);

	SetFrameStatic();

	SetPlaySoundButton(g_ToolInfo.m_bPlaySound);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::SetScrollBar
// Description :
//----------------------------------------------------------------------------
BOOL CNewDialogBar::SetScrollBar(INT iMin, INT iMax, INT iCurrent)
{
	CScrollBar* pwndScrollBar = (CScrollBar*) GetDlgItem(IDC_FRAME_SCROLL);

	SCROLLINFO scrollinfo;

	scrollinfo.cbSize = sizeof(SCROLLINFO);
	scrollinfo.fMask = SIF_ALL;
	scrollinfo.nMax = iMax;
	scrollinfo.nMin = iMin;
	scrollinfo.nPage = 0;
	scrollinfo.nPos = iCurrent;
	scrollinfo.nTrackPos = 1;

	pwndScrollBar->SetScrollInfo(&scrollinfo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::SetFrameStatic
// Description : 
//----------------------------------------------------------------------------
BOOL CNewDialogBar::SetFrameStatic(VOID)
{
	char szNowFrame[128];
	char szTotalFrame[128];

	memset(szNowFrame, 0, 128);
	memset(szTotalFrame, 0, 128);

	wsprintf(szNowFrame, "%d", g_ToolInfo.m_iScrollPos);
	wsprintf(szTotalFrame, "%d", g_ToolInfo.m_iScrollMax);	

	CString strFrame = szNowFrame;
	strFrame += "/";
	strFrame += szTotalFrame;	

	CStatic* pwndStatic = (CStatic*) GetDlgItem(IDC_FRAME_STATIC);
	pwndStatic->SetWindowText((LPSTR)(LPCTSTR)strFrame);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CNewDialogBar::
// Description : 
//----------------------------------------------------------------------------
BOOL CNewDialogBar::SetPlaySoundButton(BOOL bCheck)
{
	CButton* pwndCheckButton = (CButton*)GetDlgItem(IDC_PLAY_SOUND_CHECK);

	pwndCheckButton->SetCheck(static_cast<int>(bCheck));

	return TRUE;
}

//----------------------------------------------------------------------------




BEGIN_MESSAGE_MAP(CNewDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CLightDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CNewDialogBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollBar* pwndScrollBar = (CScrollBar*) GetDlgItem(IDC_FRAME_SCROLL);

	if (pScrollBar == pwndScrollBar)
	{
		// Clicked left arrow button.

		if (SB_LINEUP == nSBCode)
		{
			if (g_ToolInfo.m_iScrollPos > g_ToolInfo.m_iScrollMin)
			{
				--g_ToolInfo.m_iScrollPos;
			}


			g_ToolInfo.StopAnimation();

			SetCurrentAnimationFrame(g_ToolInfo.m_iScrollPos);
			UpdateDialogBar();

			ToolMain();
			
		}


		// Clicked right arrow button.

		if (SB_LINEDOWN == nSBCode)
		{
			if (g_ToolInfo.m_iScrollPos < g_ToolInfo.m_iScrollMax)
			{
				++g_ToolInfo.m_iScrollPos;
			}

			g_ToolInfo.StopAnimation();

			SetCurrentAnimationFrame(g_ToolInfo.m_iScrollPos);
			UpdateDialogBar();

			ToolMain();
		}


		// Rectange moved.

		if (SB_THUMBTRACK == nSBCode)
		{

			g_ToolInfo.StopAnimation();

			SCROLLINFO info;
			pScrollBar->GetScrollInfo(&info);

			g_ToolInfo.m_iScrollPos = info.nTrackPos;
			SetCurrentAnimationFrame(g_ToolInfo.m_iScrollPos);
			UpdateDialogBar();

			ToolMain();
		}	
			
	}
	
	//CDialogBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

