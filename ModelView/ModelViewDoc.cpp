// ModelViewDoc.cpp : implementation of the CModelViewDoc class
//

#include "stdafx.h"
#include "ModelView.h"
#include "ModelViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelViewDoc

IMPLEMENT_DYNCREATE(CModelViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CModelViewDoc, CDocument)
	//{{AFX_MSG_MAP(CModelViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelViewDoc construction/destruction

CModelViewDoc::CModelViewDoc()
{
	// TODO: add one-time construction code here

}

CModelViewDoc::~CModelViewDoc()
{
	TerminateTool();
}

BOOL CModelViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Initailze Engine.

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);

//返回第一个视图窗口的指针
	if (FALSE == InitTool(pView->m_hWnd))
	{		
		MessageBox(pView->m_hWnd, "Super Shovel 3D initialize falied.", "Error!", MB_OK);
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CModelViewDoc serialization

void CModelViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewDoc diagnostics

#ifdef _DEBUG
void CModelViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CModelViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModelViewDoc commands
