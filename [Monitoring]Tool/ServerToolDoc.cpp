// ServerToolDoc.cpp : implementation of the CServerToolDoc class
//

#include "stdafx.h"
#include "ServerTool.h"

#include "ServerToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerToolDoc

IMPLEMENT_DYNCREATE(CServerToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerToolDoc, CDocument)
	//{{AFX_MSG_MAP(CServerToolDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerToolDoc construction/destruction

CServerToolDoc::CServerToolDoc()
{
	// TODO: add one-time construction code here

}

CServerToolDoc::~CServerToolDoc()
{
}

BOOL CServerToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CServerToolDoc serialization

void CServerToolDoc::Serialize(CArchive& ar)
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
// CServerToolDoc diagnostics

#ifdef _DEBUG
void CServerToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerToolDoc commands
