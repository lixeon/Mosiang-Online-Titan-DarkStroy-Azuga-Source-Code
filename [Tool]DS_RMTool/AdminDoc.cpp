// AdminDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "AdminDoc.h"
#include "AdminInfo.h"
#include "IpInfo.h"
#include "RMNetwork.h"
#include "AdminView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc

IMPLEMENT_DYNCREATE(CAdminDoc, CBaseDoc)

CAdminDoc::CAdminDoc()
{
	m_dwLoadingFlag = eAL_None;
	memset(&m_sOperator, 0, sizeof(m_sOperator));
}

BOOL CAdminDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAdminDoc::~CAdminDoc()
{
}


BEGIN_MESSAGE_MAP(CAdminDoc, CBaseDoc)
	//{{AFX_MSG_MAP(CAdminDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc diagnostics

#ifdef _DEBUG
void CAdminDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdminDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc serialization

void CAdminDoc::Serialize(CArchive& ar)
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
// CAdminDoc commands

void CAdminDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch(Protocol)
	{
	case MP_RMTOOL_ADMIN_OPERATORINFO:
		{
			TMSG_OPERTOTALINFO* msg = (TMSG_OPERTOTALINFO*)pMsg;
			SetOperatorTotalInfo(msg->wCount, msg->sOper, msg->bEnd);
		}
		break;
	case MP_RMTOOL_ADMIN_IPINFO:
		{
			TMSG_IPTOTALINFO* msg = (TMSG_IPTOTALINFO*)pMsg;
			SetIpTotalInfo(msg->wCount, msg->sIP, msg->bEnd);
		}
		break;
	case MP_RMTOOL_ADMIN_INSERTOPERATOR_ACK:
		{
			InitData();
		}
		break;
	case MP_RMTOOL_ADMIN_INSERTOPERATOR_NACK:
		{
			CAdminView* pView = (CAdminView*)GetView(0);
			pView->GetAdminInfoPage()->OperatorErrorMsg();
		}
		break;
	case MP_RMTOOL_ADMIN_INSERTIP_ACK:
		{
			InitData();
		}
		break;
	case MP_RMTOOL_ADMIN_INSERTIP_NACK:
		{
			CAdminView* pView = (CAdminView*)GetView(0);
			pView->GetIpInfoPage()->IPErrorMsg();
		}
		break;
	}
}

void CAdminDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CBaseDoc::OnCloseDocument();
}

void CAdminDoc::InitData()
{
	// Data 요청
	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_TOTALINFO_SYN;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwData = ((CDS_RMToolApp*)AfxGetApp())->GetCurOperatorIdx();

	NETWORK->Send(&Msg, sizeof(Msg));		
}

void CAdminDoc::OpenData()
{
	// 컨트롤 초기화 지시..
	CAdminView* pView = (CAdminView*)GetView(0);
	pView->GetAdminInfoPage()->InitControl(this);
	pView->GetAdminInfoPage()->UpdateOperList();
	pView->GetIpInfoPage()->InitControl(this);
	pView->GetIpInfoPage()->UpdateIpList();
}

void CAdminDoc::SetOperatorTotalInfo(WORD count, OPERATOR* pOper, BOOL bEnd)
{
	m_sOperator.wOperCount = 0;
	memset( m_sOperator.pOper, 0, sizeof(OPERATOR)*100 );

	memcpy(&m_sOperator.pOper[m_sOperator.wOperCount], pOper, sizeof(OPERATOR)*count);
	m_sOperator.wOperCount = count;

	SetLoadingFlag(eAL_OperatorInfo);
}

void CAdminDoc::SetIpTotalInfo(WORD count, IPINFO* pIP, BOOL bEnd)
{
	m_sOperator.wIPCount = 0;
	memset( m_sOperator.pIp, 0, sizeof(IPINFO)*100 );	

	memcpy(&m_sOperator.pIp[m_sOperator.wIPCount], pIP, sizeof(IPINFO)*count);
	m_sOperator.wIPCount = count;

	SetLoadingFlag(eAL_IpInfo);
}
