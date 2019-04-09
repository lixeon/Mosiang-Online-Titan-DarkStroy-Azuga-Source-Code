// MunpaDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MunpaDoc.h"
#include "MunpaView.h"
#include "MunpaInfo.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMunpaDoc

IMPLEMENT_DYNCREATE(CMunpaDoc, CBaseDoc)

CMunpaDoc::CMunpaDoc()
{
	m_dwLoadingFlag = eML_None;
	m_bReadOnly = FALSE;

	m_pBaseInfo = NULL;
	m_pWareHouse = NULL;

	m_ItemOptionInfo.Initialize(100);
	m_OptionInfo_org.Initialize(100);

	m_OptionKeyGenerator.Init(2000, 101);
}

BOOL CMunpaDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMunpaDoc::~CMunpaDoc()
{
	SAFE_DELETE(m_pBaseInfo);
	SAFE_DELETE(m_pWareHouse);

	ITEM_OPTION_INFO* pOptionInfo = NULL;
	m_ItemOptionInfo.SetPositionHead();
	while(pOptionInfo = m_ItemOptionInfo.GetData())
		delete pOptionInfo;
	m_ItemOptionInfo.RemoveAll();

	m_OptionInfo_org.SetPositionHead();
	while(pOptionInfo = m_OptionInfo_org.GetData())
		delete pOptionInfo;
	m_OptionInfo_org.RemoveAll();

	m_OptionKeyGenerator.Release();
}


BEGIN_MESSAGE_MAP(CMunpaDoc, CBaseDoc)
	//{{AFX_MSG_MAP(CMunpaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMunpaDoc diagnostics

#ifdef _DEBUG
void CMunpaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMunpaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMunpaDoc serialization

void CMunpaDoc::Serialize(CArchive& ar)
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
// CMunpaDoc commands

void CMunpaDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	CMunpaView* pView = (CMunpaView*)GetView(0);

	switch(Protocol)
	{
	case MP_RMTOOL_MUNPASEARCH_NACK:
		{
			// 일치 정보 없음을 알림..
			pView->GetMunpaInfoPage()->InitMunpaInfo( FALSE );
			pView->GetMunpaInfoPage()->MessageBox("No Guild Info!", "Error");
		}
		break;
	case MP_RMTOOL_MUNPABASEINFO:
		{
			TMSG_MUNPABASEINFO* msg = (TMSG_MUNPABASEINFO*)pMsg;

			memcpy(&m_sBaseInfo_org, &msg->sBaseInfo, sizeof(m_sBaseInfo_org));
			SetLoadingFlag(eML_BaseInfo);
		}
		break;
	case MP_RMTOOL_MUNPAITEMINFO:
		{
			TMSG_ITEMINFO_LARGECOUNT* msg = (TMSG_ITEMINFO_LARGECOUNT*)pMsg;

			for(WORD i=0;i<msg->m_wCount;++i)
			{
				POSTYPE itempos = msg->m_Item[i].Position - TP_MUNPAWAREHOUSE_START;

				m_sWareHouse_org.WareHouse[itempos].wIconIdx		= msg->m_Item[i].wIconIdx;
				m_sWareHouse_org.WareHouse[itempos].dwDBIdx		= msg->m_Item[i].dwDBIdx;
				m_sWareHouse_org.WareHouse[itempos].Position		= msg->m_Item[i].Position;
				m_sWareHouse_org.WareHouse[itempos].Durability	= msg->m_Item[i].Durability;
				m_sWareHouse_org.WareHouse[itempos].QuickPosition	= msg->m_Item[i].QuickPosition;
			}

			if( msg->m_bEnd )
				SetLoadingFlag(eML_ItemInfo);
		}
		break;
	case MP_RMTOOL_MUNPAMEMBERINFO:
		{
			TMSG_MUNPA_MEMBERINFO* msg = (TMSG_MUNPA_MEMBERINFO*)pMsg;
			for(WORD i=0;i<msg->wCount;++i)
			{
				pView->GetMunpaInfoPage()->InsertMemberList( msg->sMember[i].strName, msg->sMember[i].level, 
										 msg->sMember[i].bJikChek, msg->sMember[i].strRegDate );
			}

			if( msg->bEnd )
				SetLoadingFlag(eML_MemberInfo);
		}
		break;
	default:
		break;
	}	
}

void CMunpaDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class

	CBaseDoc::OnCloseDocument();
}

void CMunpaDoc::InitData()
{
	SAFE_DELETE(m_pBaseInfo);
	SAFE_DELETE(m_pWareHouse);

	memset(&m_sBaseInfo_org, 0, sizeof(m_sBaseInfo_org));
	memset(&m_sWareHouse_org, 0, sizeof(m_sWareHouse_org));
}

void CMunpaDoc::OpenData()
{
	SAFE_DELETE(m_pBaseInfo);
	SAFE_DELETE(m_pWareHouse);

	m_pBaseInfo = new MUNPABASEINFO;
	memcpy(m_pBaseInfo, &m_sBaseInfo_org, sizeof(m_sBaseInfo_org));

	m_pWareHouse = new MUNPAWAREHOUSEINFO;
	memcpy(m_pWareHouse, &m_sWareHouse_org, sizeof(m_sWareHouse_org));

	CMunpaView* pView = (CMunpaView*)GetView(0);
	CMunpaInfo* pMunpa = pView->GetMunpaInfoPage();

	// 초기화 호출..
	pMunpa->InitMunpaInfo(TRUE);
	pMunpa->SetMunpaBaseInfo();
	pMunpa->SetMunpaItemInfo();
}

void CMunpaDoc::UpdateData(DWORD dwParam1, DWORD dwParam2)
{
	CMunpaView* pView = (CMunpaView*) GetView(0);

	WORD pos = HIWORD(dwParam1);
	WORD kind = LOWORD(dwParam1);

	switch(kind)
	{
	case eTK_MunpaInfo:
		{
			pView->GetMunpaInfoPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eMF_MunpaInfo);
		}
		break;
	case eTK_MunpaItem:
		{
			SetSaveFlag(eMF_MunpaItem);
		}
		break;
	}
}

void CMunpaDoc::SaveData()
{
	if( IsSetSaveFlag(eMF_MunpaInfo) )
	{
	}

	if( IsSetSaveFlag(eMF_MunpaItem) )
	{
	}

	m_dwSaveFlag = eCF_None;

	CMunpaView* pView = (CMunpaView*) GetView(0);

	if( !m_pBaseInfo )
		return;
	
	DWORD MunpaIdx = m_pBaseInfo->dwMunpaIdx;
	InitData();
	pView->GetMunpaInfoPage()->InitMunpaInfo();

	TMSG_DWORD MunpaSearch;
	MunpaSearch.Category = MP_RMTOOL_MUNPA;
	MunpaSearch.Protocol = MP_RMTOOL_MUNPAINFO_RESYN;
	MunpaSearch.dwTemplateIdx = GetDocumentIdx();
	MunpaSearch.dwData = MunpaIdx;

	NETWORK->Send(&MunpaSearch, sizeof(MunpaSearch));	
}

ITEM_OPTION_INFO* CMunpaDoc::GetItemOptionInfo(DWORD wOptionIdx)
{
	return m_ItemOptionInfo.GetData(wOptionIdx);
}

ITEM_OPTION_INFO* CMunpaDoc::GetItemOptionInfoOrigin(DWORD wOptionIdx)
{
	return m_OptionInfo_org.GetData(wOptionIdx);
}


WORD CMunpaDoc::CreateLocalOptionIndex()
{
	WORD wIndex = (WORD)m_OptionKeyGenerator.GenerateIndex();

	while( m_ItemOptionInfo.GetData(wIndex) )
	{
		wIndex = (WORD)m_OptionKeyGenerator.GenerateIndex();
		if( wIndex == 0 )
		{
			ASSERT(0);
			return 0;
		}
	}
	
	return wIndex;
}

WORD CMunpaDoc::AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew )
{
	ITEM_OPTION_INFO* pOption = NULL;
	WORD Index = 0;

	pOption = new ITEM_OPTION_INFO;
	memcpy(pOption, pLocalOption, sizeof(ITEM_OPTION_INFO));

	if( bNew )
		Index = CreateLocalOptionIndex();
	else
		Index = (WORD)pLocalOption->dwOptionIdx;

	if( Index == 0 )
	{
		delete pOption;
		return 0;
	}

	pOption->dwOptionIdx = Index;

	m_ItemOptionInfo.Add(pOption, Index);

	return Index;
}

void CMunpaDoc::DelLocalOptionInfo(WORD wIndex)
{
	ITEM_OPTION_INFO* pOption = m_ItemOptionInfo.GetData(wIndex);

	if( pOption )
	{
		delete pOption;

		m_ItemOptionInfo.Remove(wIndex);
		m_OptionKeyGenerator.ReleaseIndex(wIndex);
	}
}



