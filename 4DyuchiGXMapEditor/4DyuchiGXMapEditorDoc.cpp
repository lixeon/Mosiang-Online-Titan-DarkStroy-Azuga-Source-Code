// 4DyuchiGXMapEditorDoc.cpp : implementation of the CMy4DyuchiGXMapEditorDoc class
//

#include "stdafx.h"
#include "4DyuchiGXMapEditor.h"

#include "4DyuchiGXMapEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorDoc

IMPLEMENT_DYNCREATE(CMy4DyuchiGXMapEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy4DyuchiGXMapEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CMy4DyuchiGXMapEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorDoc construction/destruction



CMy4DyuchiGXMapEditorDoc::CMy4DyuchiGXMapEditorDoc()
{
	// TODO: add one-time construction code here


}

CMy4DyuchiGXMapEditorDoc::~CMy4DyuchiGXMapEditorDoc()
{
	if (g_pTool)
	{
		delete g_pTool;
		g_pTool = NULL;
	}

}

BOOL CMy4DyuchiGXMapEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;



	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView( pos );
		
	g_pTool = new CTool;
	g_pTool->Initialize(pView);
	

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorDoc serialization

void CMy4DyuchiGXMapEditorDoc::Serialize(CArchive& ar)
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
// CMy4DyuchiGXMapEditorDoc diagnostics

#ifdef _DEBUG
void CMy4DyuchiGXMapEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy4DyuchiGXMapEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorDoc commands
