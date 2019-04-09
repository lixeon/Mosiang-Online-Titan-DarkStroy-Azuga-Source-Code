// LogDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogDoc.h"
#include "LogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDoc

IMPLEMENT_DYNCREATE(CLogDoc, CBaseDoc)

CLogDoc::CLogDoc()
{
	m_dwPage = eItemMoney_Page;
}

BOOL CLogDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CLogDoc::~CLogDoc()
{
}


BEGIN_MESSAGE_MAP(CLogDoc, CDocument)
	//{{AFX_MSG_MAP(CLogDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDoc diagnostics

#ifdef _DEBUG
void CLogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogDoc serialization

void CLogDoc::Serialize(CArchive& ar)
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
// CLogDoc commands
void CLogDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	CLogView* pView = (CLogView*)GetView(0);

	EndDlg();

	switch( Protocol )
	{
	case MP_RMTOOL_GAMELOGSEARCH_NACK:
		{
			MessageBox( NULL, "No Data!!", "Error", MB_OK );
			return;
		}
		break;
	case MP_RMTOOL_QUERY_CHARIDX_ACK:
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;
			if( m_dwPage == eItemMoney_Page )
				pView->SetItemMoneyLogCharacterIdx( pmsg->dwData );
			else if( m_dwPage == eTotal_Page )
				pView->SetTotalLogChacterIdx( pmsg->dwData );
			else if( m_dwPage == eEtc_Page)
				pView->SetEtcLogChacterIdx( pmsg->dwData );
		}
		break;
	case MP_RMTOOL_QUERY_CHARIDX_NACK:
		{
			MessageBox( NULL, "No Character Data!!", "Error", MB_OK );
			return;
		}
		break;
	case MP_RMTOOL_ITEMMONEYLOGFROMTYPE_ACK:
		{
			TMSG_LOGITEMMONEY* pmsg = (TMSG_LOGITEMMONEY*)pMsg;
			pView->SetLogItemMoney( pmsg );
		}
		break;	
	case MP_RMTOOL_CHARACTERLOG_ACK:
		{
			TMSG_CHARACTERLOG* pmsg = (TMSG_CHARACTERLOG*)pMsg;
			pView->SetCharacterLog( pmsg );
		}
		break;
	case MP_RMTOOL_EXPPOINTLOG_ACK:
		{
			TMSG_EXPPOINTLOG* pmsg = (TMSG_EXPPOINTLOG*)pMsg;
			pView->SetExpPointLog( pmsg );
		}
		break;
	case MP_RMTOOL_MUGONGLOG_ACK:
		{
			TMSG_MUGONGLOG* pmsg = (TMSG_MUGONGLOG*)pMsg;
			pView->SetMugongLog( pmsg );
		}
		break;
	case MP_RMTOOL_MUGONGEXPLOG_ACK:
		{
			TMSG_MUGONGEXPLOG* pmsg = (TMSG_MUGONGEXPLOG*)pMsg;
			pView->SetMugongExpLog( pmsg );
		}
		break;		
	case MP_RMTOOL_MONEYLOG_ACK:
		{
			TMSG_MONEYLOG* pmsg = (TMSG_MONEYLOG*)pMsg;
			pView->SetMoneyLog( pmsg );
		}
		break;
	case MP_RMTOOL_MONEYWRONGLOG_ACK:
		{
			TMSG_MONEYWRONGLOG* pmsg = (TMSG_MONEYWRONGLOG*)pMsg;
			pView->SetMoneyWrongLog( pmsg );
		}
		break;
	case MP_RMTOOL_ABILITYLOG_ACK:
		{
			TMSG_ABILITYLOG* pmsg = (TMSG_ABILITYLOG*)pMsg;
			pView->SetAbilityLog( pmsg );
		}
		break;	
	case MP_RMTOOL_CHEATLOG_ACK:
		{
			TMSG_CHEATLOG* pmsg = (TMSG_CHEATLOG*)pMsg;
			pView->SetCheatLog( pmsg );
		}
		break;
	case MP_RMTOOL_HACKINGLOG_ACK:
		{
			TMSG_HACKINGLOG* pmsg = (TMSG_HACKINGLOG*)pMsg;
			pView->SetHackingLog( pmsg );
		}
		break;
	}
}

void CLogDoc::StartDlg()
{
	CLogView* pView = (CLogView*)GetView(0);
	pView->StartDlg();
}

void CLogDoc::EndDlg()
{
	CLogView* pView = (CLogView*)GetView(0);
	pView->EndDlg();
}