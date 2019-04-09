// BaseDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "BaseDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc

IMPLEMENT_DYNCREATE(CBaseDoc, CDocument)

CBaseDoc::CBaseDoc()
{
	DWORD	m_dwDocumentIdx = 0;
}

BOOL CBaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	m_ChangeMgr.Init(this);

	return TRUE;
}

CBaseDoc::~CBaseDoc()
{
}


BEGIN_MESSAGE_MAP(CBaseDoc, CDocument)
	//{{AFX_MSG_MAP(CBaseDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc diagnostics

#ifdef _DEBUG
void CBaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc serialization

void CBaseDoc::Serialize(CArchive& ar)
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
// CBaseDoc commands

void CBaseDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	if( pApp )
		pApp->ExitFile(m_dwDocumentIdx);
	else
		ASSERT(0);
	
	CDocument::OnCloseDocument();
}

CView* CBaseDoc::GetView(WORD wIdx)
{
	POSITION pos = GetFirstViewPosition();

	for(int i=0;;++i)
	{
		CView* pView = this->GetNextView(pos);

		if( pView == NULL ) return NULL;

		if( i == wIdx ) return pView;
	}

	return NULL;
}

void CBaseDoc::SetLoadingFlag(DWORD flag)
{
	m_dwLoadingFlag |= flag; 

	if( IsLoadingAll() )
	{
		// View창 초기화
		AfxGetMainWnd()->EndModalLoop(0);
		AfxGetMainWnd()->EndModalState();

		// 사본을 생성하고 컨트롤을 초기화한다.
		OpenData();
		m_dwLoadingFlag = 0x00000000;
		
	}	
}

void CBaseDoc::ProcessChangeManager()
{
	m_ChangeMgr.ProcessList();
}

