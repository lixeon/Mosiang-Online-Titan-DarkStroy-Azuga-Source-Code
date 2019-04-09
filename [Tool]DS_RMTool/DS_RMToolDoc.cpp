// DS_RMToolDoc.cpp : implementation of the CDS_RMToolDoc class
//

#include "stdafx.h"
#include "DS_RMTool.h"

#include "DS_RMToolDoc.h"
#include "RMNetwork.h"
#include "DS_RMToolView.h"
#include "MasInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolDoc

IMPLEMENT_DYNCREATE(CDS_RMToolDoc, CBaseDoc)

BEGIN_MESSAGE_MAP(CDS_RMToolDoc, CBaseDoc)
	//{{AFX_MSG_MAP(CDS_RMToolDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolDoc construction/destruction

CDS_RMToolDoc::CDS_RMToolDoc()
{
	// TODO: add one-time construction code here
	m_sUserInfo = NULL;	
}

CDS_RMToolDoc::~CDS_RMToolDoc()
{
	SAFE_DELETE(m_sUserInfo);
}

BOOL CDS_RMToolDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolDoc serialization

void CDS_RMToolDoc::Serialize(CArchive& ar)
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
// CDS_RMToolDoc diagnostics

#ifdef _DEBUG
void CDS_RMToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDS_RMToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolDoc commands

void CDS_RMToolDoc::SetUserInfo(USERINFO* user)
{
	SAFE_DELETE(m_sUserInfo);

	m_sUserInfo = new USERINFO;
	memcpy(m_sUserInfo, user, sizeof(USERINFO));
	memcpy(&m_sUserInfoOrg, user, sizeof(USERINFO));
}

void CDS_RMToolDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	CDS_RMToolView* pView = (CDS_RMToolView*)GetView(0);

	switch(Protocol)
	{
	case MP_RMTOOL_QUERY_MEMBERNUM_ACK:				// 1
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;

			TMSG_DWORD2 Msg;
			Msg.Category = MP_RMTOOL_USER;
			Msg.Protocol = MP_RMTOOL_USERSEARCH_SYN;
			Msg.dwTemplateIdx = GetDocumentIdx();
			Msg.dwData1 = pmsg->dwData;
			Msg.dwData2 = MASINFOMGR->GetCurMasInfo()->nServerSet;

			NETWORK->Send( &Msg, sizeof(TMSG_DWORD2) );
		}
		break;
	case MP_RMTOOL_QUERY_MEMBERNUM_NACK:
		{
			pView->InitUserInfo();
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	case MP_RMTOOL_USERSEARCH_ACK:					// 2
		{
			TMSG_USERSEARCHRESULT* msg = (TMSG_USERSEARCHRESULT*)pMsg;
			pView->InitUserInfo();
			pView->SetUserInfo( &msg->sUser );

			// 다시 캐릭정보를 요청한다.
			TMSG_SIMPLECHARACTERINFO Msg;
			Msg.Category = MP_RMTOOL_USER;
			Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_SYN;
			Msg.dwTemplateIdx = GetDocumentIdx();
			Msg.wServer = msg->wServer;
			Msg.dwMemberNo = msg->sUser.dwMemberNo;

			NETWORK->Send(&Msg, sizeof(TMSG_SIMPLECHARACTERINFO));
		}
		break;
	case MP_RMTOOL_USERSEARCH_NACK:
		{
			pView->InitUserInfo();
			pView->MessageBox("No Data!!", "Error");
		}
		break;
	case MP_RMTOOL_SIMPLECHARACTER_ACK:				// 3
		{
			TMSG_SIMPLERESULT* pmsg = (TMSG_SIMPLERESULT*)pMsg;
			pView->SetSimpleCharInfo( pmsg->Count, pmsg->sSimple );			
		}
		break;
	case MP_RMTOOL_SIMPLECHARACTER_NACK:
		{
		}
		break;	
	case MP_RMTOOL_CHARACTER_CREATEINFO_ACK:
		{
			TMSG_CHARACTERCREATEINFO* msg = (TMSG_CHARACTERCREATEINFO*)pMsg;
			pView->SetCharacterCreateInfo( msg->Count, msg->sInfo );
		}
		break;
	case MP_RMTOOL_CHARACTER_CREATEINFO_NACK:
		{
		}
		break;

	case MP_RMTOOL_CHECKNAME_ACK:
		{
			pView->EnableRecoverButton();
		}
		break;
	case MP_RMTOOL_CHECKNAME_NACK:
		{
			pView->RecoverError( 2 );
		}
		break;
	case MP_RMTOOL_CHARACTER_RECOVER_ACK:
		{
			TMSG_SIMPLECHARACTERINFO Msg;
			Msg.Category = MP_RMTOOL_USER;
			Msg.Protocol = MP_RMTOOL_SIMPLECHARACTER_SYN;
			Msg.dwTemplateIdx = GetDocumentIdx();
			Msg.wServer = 0;
			Msg.dwMemberNo = m_sUserInfoOrg.dwMemberNo;

			NETWORK->Send(&Msg, sizeof(TMSG_SIMPLECHARACTERINFO));			
		}
		break;
	case MP_RMTOOL_CHARACTER_RECOVER_NACK:
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;
			pView->RecoverError( pmsg->dwData );
		}
		break;

	case MP_RMTOOL_CHANGE_USERLEVEL_ACK:
		{
			TMSG_DWORD2* pmsg = (TMSG_DWORD2*)pMsg;
			pView->ChangeUserLevel( pmsg->dwData2 );
		}
		break;
	case MP_RMTOOL_CHANGE_USERLEVEL_NACK:
		break;
	}
}

void CDS_RMToolDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CBaseDoc::OnCloseDocument();
}

void CDS_RMToolDoc::ChangeUserLevel( DWORD dwUserLevel )
{
	TMSG_NAME_DWORD Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_CHANGE_USERLEVEL_SYN;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwChrIdx = m_sUserInfoOrg.dwMemberNo;
	strncpy( Msg.sName, m_sUserInfoOrg.strID, 17 );
	Msg.dwData = dwUserLevel;

	NETWORK->Send( &Msg, sizeof(TMSG_NAME_DWORD) );
}