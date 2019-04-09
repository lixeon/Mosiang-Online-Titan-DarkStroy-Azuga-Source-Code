// PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "LDPropSheet.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };

void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	CRect windowRect;

	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);

	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;

	if (nFlag != CDF_NONE)
	{
		// calculate new dialog window rectangle
		CRect clientRect, newClientRect, newWindowRect;

		pWnd->GetWindowRect(windowRect);
		pWnd->GetClientRect(clientRect);
		long xDiff = windowRect.Width() - clientRect.Width();
		long yDiff = windowRect.Height() - clientRect.Height();
	
		newClientRect.left = newClientRect.top = 0;
		newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;

		if (nFlag == CDF_TOPLEFT) // resize with origin at top/left of window
		{
			newWindowRect.left = windowRect.left;
			newWindowRect.top = windowRect.top;
			newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
		}
		else if (nFlag == CDF_CENTER) // resize with origin at center of window
		{
			newWindowRect.left = windowRect.left - 
							(newClientRect.right - clientRect.right)/2;
			newWindowRect.top = windowRect.top -
							(newClientRect.bottom - clientRect.bottom)/2;
			newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
		}
		pWnd->MoveWindow(newWindowRect);
	}

	pWnd->SetFont(pFont);

	// iterate through and move all child windows and change their font.
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);

		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}

		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.top = windowRect.top * newHeight / oldHeight;
		windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
		pChildWnd->MoveWindow(windowRect);
		
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLDPropPage property page

IMPLEMENT_DYNCREATE(CLDPropPage, CPropertyPage)

void CLDPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLDPropPage)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLDPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLDPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLDPropPage::GetOriginalRect(CRect *pRect)
{
	DLGTEMPLATE* pTmp = NULL;

	GetParent()->GetClientRect(pRect);
	return;

	pRect->SetRectEmpty();

	if(m_hDialogTemplate)
		pTmp = (DLGTEMPLATE*)m_hDialogTemplate;
	else if (m_lpDialogTemplate)
		pTmp = (DLGTEMPLATE*)m_lpDialogTemplate;

	if (pTmp) {
		pRect->SetRect(0, 0, pTmp->cx, pTmp->cy);
		MapDialogRect(pRect);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CLDPropSheet

IMPLEMENT_DYNAMIC(CLDPropSheet, CPropertySheet)

//CLDPropSheet::CLDPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
//	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
//{
//}

//CLDPropSheet::CLDPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
//	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
//{
//}

CLDPropSheet::CLDPropSheet(CWnd* pParentWnd)
{
}

CLDPropSheet::~CLDPropSheet()
{
//	m_imageTab.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CLDPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CLDPropSheet)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADJUST, OnAdjust)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLDPropSheet message handlers

void CLDPropSheet::AdjustPages()
{
	CRect rect;
	
	GetClientRect(&rect);

	CTabCtrl* pTab = GetTabControl();

	if (pTab)	{
		pTab->Invalidate(FALSE);
		pTab->MoveWindow(rect, FALSE);		// Resize the TabsCtrl to new size but don't draw now
		pTab->AdjustRect(FALSE, &rect);		// Get the display area below the Tabs
 
		int iTabHeight = rect.top;				// Save the Tab Height

		// Which Page is being adjusted?
		CPropertyPage* pPage = GetActivePage();
		if (pPage) {
			CRect pageRect;

			//Going to be moving things without redrawing
			pPage->Invalidate(FALSE); 

			// Return to Original Dialog Size
			((CLDPropPage*)pPage)->GetOriginalRect(&pageRect);

			pPage->MoveWindow(pageRect, FALSE); //Move but don't draw
		
			// Adjust for tab height
			pPage->GetWindowRect(&pageRect);
			ScreenToClient(&pageRect);
			pageRect.OffsetRect(0,iTabHeight);
			
			pageRect.IntersectRect(pageRect,rect);		// restrict to the display area
 
			// Fit Page to the new size
			pPage->MoveWindow(pageRect); 
		}
	}
}

BOOL CLDPropSheet::Create(CWnd *pParentWnd, DWORD dwStyle, DWORD dwExStyle) 
{
//	if (dwStyle == -1)
//		dwStyle = DS_3DLOOK | DS_CONTEXTHELP | DS_SETFONT |	WS_VISIBLE | WS_CHILD;
	
//	dwStyle &= ~WS_BORDER;

//	dwExStyle = dwExStyle | WS_EX_CONTROLPARENT; // Handle tab key
	
	if (!CPropertySheet::Create(pParentWnd,dwStyle,dwExStyle))
		return FALSE;

//	if(!m_Font.CreatePointFont(100, _T("Arial")))
//		return FALSE;

//	SetFont(&m_Font);

	GetClientRect(m_rectOriginal);	// save original size, which is optimized for all initial pages

	return TRUE;
}

LRESULT CLDPropSheet::OnAdjust(WPARAM wParam, LPARAM lParam)
{
	AdjustPages();
	return 0;
}

BOOL CLDPropSheet::OnInitDialog() 
{
/*	ModifyStyleEx(0,WS_EX_CONTROLPARENT);
	BOOL bResult = CPropertySheet::OnInitDialog();


//	m_imageTab.Create( IDB_BITMAP_TAB, 16, 1, RGB(255,0,0) );
	CTabCtrl *pTab = GetTabControl();
//	pTab->SetImageList( &m_imageTab );

	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE;
	for( int i = 0; i < 4; i++ )
	{
		tcItem.iImage = i;
		pTab->SetItem( i, &tcItem );
	}

	// Resize the CTabCtrl and PropertyPages
	PostMessage(WM_ADJUST);
		return bResult;
		*/


	CPropertySheet::OnInitDialog();

	// get the font for the first active page
	CPropertyPage* pPage = GetActivePage ();
	ASSERT (pPage);

	// change the font for the sheet
	ChangeDialogFont (this, &m_fntPage, CDF_CENTER);
	// change the font for each page
	for (int iCntr = 0; iCntr < GetPageCount (); iCntr++)
	{
		VERIFY (SetActivePage (iCntr));
		CPropertyPage* pPage = GetActivePage ();
		ASSERT (pPage);
		ChangeDialogFont (pPage, &m_fntPage, CDF_CENTER);
	}

	VERIFY (SetActivePage (pPage));

	// set and save the size of the page
	CTabCtrl* pTab = GetTabControl ();
	ASSERT (pTab);

	if (m_psh.dwFlags & PSH_WIZARD)
	{
		pTab->ShowWindow (SW_HIDE);
		GetClientRect (&m_rctPage);

		CWnd* pButton = GetDlgItem (ID_WIZBACK);
		ASSERT (pButton);
		CRect rc;
		pButton->GetWindowRect (&rc);
		ScreenToClient (&rc);
		m_rctPage.bottom = rc.top-2;
	}
	else
	{
		pTab->GetWindowRect (&m_rctPage);
		ScreenToClient (&m_rctPage);
		pTab->AdjustRect (FALSE, &m_rctPage);
	}

	// resize the page	
	pPage->MoveWindow (&m_rctPage);

	return TRUE;
}

BOOL CLDPropSheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnmh = (LPNMHDR)lParam;
	
	if (TCN_SELCHANGING == pnmh->code)			// save the current page index
		m_nLastActive = GetActiveIndex();       
	else if (TCN_SELCHANGE == pnmh->code) {		// just changed tabs
		if (0)
			PostMessage(PSM_SETCURSEL, m_nLastActive);       
		else {
			// NOTE: ON_NOTIFY_REFLECT does not seem to work for non-CDialog based windows, oh well...
			PostMessage(WM_ADJUST); // adjust the pages
			CView	*pView = (CView*)GetParent();
			static NMHDR	nmh = *pnmh;
//			pView->PostMessage(WM_NOTIFY, wParam, (LPARAM)&nmh);		// notify view of page change
		}
	}
	
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

void CLDPropSheet::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);
	
	PostMessage(WM_ADJUST); // adjust the pages
}


void CLDPropSheet::BuildPropPageArray()
{
	CPropertySheet::BuildPropPageArray();

	// get first page
	CPropertyPage* pPage = GetPage (0);
	ASSERT (pPage);
	
	// dialog template class in afxpriv.h
	CDialogTemplate dlgtemp;
	// load the dialog template
	VERIFY (dlgtemp.Load (pPage->m_psp.pszTemplate));
	// get the font information
	CString strFace;
	WORD	wSize;
	VERIFY (dlgtemp.GetFont (strFace, wSize));
	if (m_fntPage.m_hObject)
		VERIFY (m_fntPage.DeleteObject ());
	// create a font using the info from first page
	VERIFY (m_fntPage.CreatePointFont (wSize*10, strFace));
}
