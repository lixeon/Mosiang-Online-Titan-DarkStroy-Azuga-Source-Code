// HtmlViewNotice.cpp : implementation file
//

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "HtmlViewNotice.h"
#include <Mshtml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewNotice

IMPLEMENT_DYNCREATE(CHtmlViewNotice, CHtmlView)

CHtmlViewNotice::CHtmlViewNotice()
{
	//{{AFX_DATA_INIT(CHtmlViewNotice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bCanNotConnect = FALSE;
}

CHtmlViewNotice::~CHtmlViewNotice()
{
}

void CHtmlViewNotice::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlViewNotice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlViewNotice, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlViewNotice)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewNotice diagnostics

#ifdef _DEBUG
void CHtmlViewNotice::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlViewNotice::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewNotice message handlers
int CHtmlViewNotice::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	return 0;
}


void CHtmlViewNotice::OnDocumentComplete(LPCTSTR lpszURL) 
{
	// TODO: Add your specialized code here and/or call the base class

/*	//Hide Scroll Bar
	IDispatch *pDisp = GetHtmlDocument();
	if( pDisp != NULL )
	{
		IHTMLDocument2* pHTMLDocument2;
		
		HRESULT hr;
        hr = pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDocument2);
		
		if( hr == S_OK )
		{
			IHTMLElement *pIElement;
			
			hr = pHTMLDocument2->get_body(&pIElement);
			
			IHTMLBodyElement *pIBodyElement;
			
			hr = pIElement->QueryInterface(IID_IHTMLBodyElement,(void**)&pIBodyElement);
			
			pIBodyElement->put_scroll(L"no");
			pIBodyElement->Release();
		}
		pHTMLDocument2->Release();
		pDisp->Release();
    }
*/

	if( !bCanNotConnect )
		ShowWindow( SW_SHOW );

	CHtmlView::OnDocumentComplete(lpszURL);
}

void CHtmlViewNotice::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( strncmp( lpszURL, "res", 3 ) == 0 )
	{
		bCanNotConnect = TRUE;
		ShowWindow( SW_HIDE );
	}
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CHtmlViewNotice::OnDownloadBegin() 
{
	// TODO: Add your specialized code here and/or call the base class

	
	CHtmlView::OnDownloadBegin();
}

void CHtmlViewNotice::OnDownloadComplete() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnDownloadComplete();
}


void CHtmlViewNotice::OnNavigateComplete2(LPCTSTR strURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnNavigateComplete2(strURL);
}

void CHtmlViewNotice::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CHtmlView::OnPaint() for painting messages
}
