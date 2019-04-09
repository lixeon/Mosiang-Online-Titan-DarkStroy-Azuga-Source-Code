// LogOperDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogOperDoc.h"
#include "LogOperView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogOperDoc

IMPLEMENT_DYNCREATE(CLogOperDoc, CBaseDoc)

CLogOperDoc::CLogOperDoc()
{
}

BOOL CLogOperDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CLogOperDoc::~CLogOperDoc()
{
}


BEGIN_MESSAGE_MAP(CLogOperDoc, CDocument)
	//{{AFX_MSG_MAP(CLogOperDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogOperDoc diagnostics

#ifdef _DEBUG
void CLogOperDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogOperDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogOperDoc serialization

void CLogOperDoc::Serialize(CArchive& ar)
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
// CLogOperDoc commands
void CLogOperDoc::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	CLogOperView* pView = (CLogOperView*)GetView(0);

	switch( Protocol )
	{
	case MP_RMTOOL_TOOLLOG_ACK:
		{
			TMSG_TOOLLOG* pmsg = (TMSG_TOOLLOG*)pMsg;
			pView->SetOperLog( pmsg );
		}
		break;
	case MP_RMTOOL_GAMELOGSEARCH_NACK:
		{
			MessageBox( NULL, "No Data!!", "Confirm", MB_OK );
		}
		break;
	}
}