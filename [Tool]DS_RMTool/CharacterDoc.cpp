// CharacterDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "CharacterDoc.h"
#include "Characterinfo.h"
#include "CharacterView.h"
#include "RMNetwork.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc

IMPLEMENT_DYNCREATE(CCharacterDoc, CBaseDoc)

CCharacterDoc::CCharacterDoc()
{
	m_dwLoadingFlag = eCL_None;
	m_dwSaveFlag = eCF_None;
	m_bReadOnly = FALSE;

	m_ItemOptionInfo_org.Initialize(100);
	m_ItemOptionInfo.Initialize(100);	

	m_OptionKeyGenerator.Init(2000, 101);

	m_dwUserIdx = m_dwCharIdx = 0;
}

BOOL CCharacterDoc::OnNewDocument()
{
	if (!CBaseDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCharacterDoc::~CCharacterDoc()
{
	ITEM_OPTION_INFO* pOptionInfo = NULL;

	m_ItemOptionInfo_org.SetPositionHead();
	while(pOptionInfo = m_ItemOptionInfo_org.GetData())
		delete pOptionInfo;
	m_ItemOptionInfo_org.RemoveAll();

	m_ItemOptionInfo.SetPositionHead();
	while(pOptionInfo = m_ItemOptionInfo.GetData())
		delete pOptionInfo;
	m_ItemOptionInfo.RemoveAll();

	m_OptionKeyGenerator.Release();
}


BEGIN_MESSAGE_MAP(CCharacterDoc, CBaseDoc)
	//{{AFX_MSG_MAP(CCharacterDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc diagnostics

#ifdef _DEBUG
void CCharacterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCharacterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharacterDoc serialization

void CCharacterDoc::Serialize(CArchive& ar)
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
// CCharacterDoc commands

void CCharacterDoc::InitData()
{
	memset( &m_sBaseInfo, 0, sizeof(m_sBaseInfo) );
	memset( &m_sTotalInfo, 0, sizeof(m_sTotalInfo) );
	memset( &m_sHTotalInfo, 0, sizeof(m_sHTotalInfo) );
	memset( &m_sInven_org, 0, sizeof(m_sInven_org) );
	memset( &m_sMugong_org, 0, sizeof(m_sMugong_org) );
	memset( &m_sPyoguk_org, 0, sizeof(m_sPyoguk_org) );
	memset( &m_sAbility_org, 0, sizeof(m_sAbility_org) );
	memset( &m_sInven, 0, sizeof(m_sInven) );
	memset( &m_sMugong, 0, sizeof(m_sMugong) );
	memset( &m_sPyoguk, 0, sizeof(m_sPyoguk) );
	memset( &m_sAbility, 0, sizeof(m_sAbility) );
	
	memset( &m_InvenInfoError, 0, sizeof(TMSG_CHARACTER_INVENINFO2) );
	memset( &m_WareError, 0, sizeof(PHYOKUK_TOTALINFO) );
	memset( &m_MugongError, 0 , sizeof(TMSG_CHARACTER_MUGONGINFO2) );
}

void CCharacterDoc::OpenData()
{
	memcpy( &m_sInven, &m_sInven_org, sizeof(ITEM_TOTALINFO) );
	memcpy( &m_sMugong, &m_sMugong_org, sizeof(MUGONG_TOTALINFO) );
	memcpy( &m_sPyoguk, &m_sPyoguk_org, sizeof(PHYOKUK_TOTALINFO) );
	memcpy( &m_sAbility, &m_sAbility_org, sizeof(ABILITY_TOTALINFO) );

	// 컨트롤 초기화 지시..
	CCharacterView* pView = (CCharacterView*)GetView(0);
	pView->GetCharacterInfoPage()->InitControl(this);
}

void CCharacterDoc::UpdateData(DWORD dwParam1, DWORD dwParam2)
{
	CCharacterView* pView = (CCharacterView*) GetView(0);

	WORD pos = HIWORD(dwParam1);
	WORD kind = LOWORD(dwParam1);

	switch(kind)
	{
	case eTK_CharacterInfo:
		{
			pView->GetCharacterInfoPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eCF_CharacterInfo);
		}
		break;
	case eTK_PyogukInfo:
		{
			pView->GetCharacterInfoPage()->m_StatusEdit[pos].ResetData();
			SetSaveFlag(eCF_PyogukInfo);
		}
		break;
	case eTK_AbilityInfo:
		{
			SetSaveFlag(eCF_AbilityInfo);
		}
		break;
//	case eTK_InventoryItem:
//		{
//			SetSaveFlag(eCF_InventoryItem);
//		}
//		break;	
//	case eTK_PyogukItem:
//		{
//			SetSaveFlag(eCF_PyogukItem);
//		}
//		break;
//	case eTK_MugongInfo:
//		{
//			SetSaveFlag(eCF_MugongInfo);
//		}
//		break;
	}
}

void CCharacterDoc::SaveData()
{
	if( IsSetSaveFlag(eCF_CharacterInfo) )
	{
		CharacterInfoSaveDataSend();
	}

	if( IsSetSaveFlag(eCF_MugongInfo) )
	{
	}

	if( IsSetSaveFlag(eCF_PyogukInfo) )
	{
		PyogukInfoSaveDataSend();
	}

	if( IsSetSaveFlag(eCF_AbilityInfo) )
	{
		AbilitySaveDataSend();
	}

//	if( IsSetSaveFlag(eCF_InventoryItem) )
//	{
//	}

//	if( IsSetSaveFlag(eCF_PyogukItem) )
//	{
//	}

//	if( IsSetSaveFlag(eCF_PyogukAdd) )
//	{
//	}

	m_dwSaveFlag = eCF_None;
	InitData();
	OpenCharacterInfo( 0, m_dwUserIdx, m_dwCharIdx );
//	CCharacterView* pView = (CCharacterView*)GetView(0);
//	(pView->GetParentFrame())->SendMessage(WM_CLOSE);
}

void CCharacterDoc::CharacterInfoSaveDataSend()
{
	CCharacterView* pView = (CCharacterView*)GetView(0);
	TMSG_CHARACTER_UPDATEBASEINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_BASEINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.CharIdx = m_sBaseInfo.dwObjectID;

	pView->GetCharacterInfoPage()->GetChangeBaseInfo(&Msg);
	
	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::PyogukInfoSaveDataSend()
{
	CCharacterView* pView = (CCharacterView*)GetView(0);
	TMSG_CHARACTER_PYOGUKINFO Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_PYOGUKINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.m_dwUserIdx = m_sBaseInfo.dwUserID;
	strcpy( Msg.m_sUserName, m_sBaseInfo.ObjectName );

	pView->GetCharacterInfoPage()->GetChangePyogukInfo(&Msg);

	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::AbilitySaveDataSend()
{
	TMSG_CHARACTER_UPDATE_ABILITY Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATE_ABILLITYINFO;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwCharIdx = m_sBaseInfo.dwObjectID;
	strcpy( Msg.sName, m_sBaseInfo.ObjectName );
	memcpy(&Msg.sAbility, &m_sAbility, sizeof(ABILITY_TOTALINFO));

	NETWORK->Send(&Msg, sizeof(Msg));
}

void CCharacterDoc::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch(Protocol)
	{
	case MP_RMTOOL_CHARACTER_BASEINFO:
		{
			TMSG_CHARACTER_BASEINFO* msg = (TMSG_CHARACTER_BASEINFO*)pMsg;
			SetCharacterBaseInfo(&msg->m_sObjinfo, &msg->m_sTotalinfo, &msg->m_sHeroinfo);
		}
		break;
	case MP_RMTOOL_CHARACTER_INVENINFO:
		{
			TMSG_CHARACTER_INVENINFO* msg = (TMSG_CHARACTER_INVENINFO*)pMsg;
			SetCharacterInvenInfo(&msg->m_sIteminfo);
		}
		break;
	case MP_RMTOOL_CHARACTER_INVENINFO2:
		{
			TMSG_CHARACTER_INVENINFO2* msg = (TMSG_CHARACTER_INVENINFO2*)pMsg;
			SetCharacterInvenInfo2( msg );
		}
		break;
	case MP_RMTOOL_CHARACTER_PYOGUKINFO:
		{
			TMSG_CHARACTER_PYOGUKINFO* msg = (TMSG_CHARACTER_PYOGUKINFO*)pMsg;
			SetCharacterPyogukInfo(msg->m_wPyogukNum, msg->m_dwMoney);
		}
		break;
	case MP_RMTOOL_CHARACTER_PYOGUKITEMINFO:
		{
			TMSG_ITEMINFO_LARGECOUNT* msg = (TMSG_ITEMINFO_LARGECOUNT*)pMsg;
			SetCharacterPyogukItemInfo(msg->m_wCount, msg->m_Item, msg->m_bEnd );
		}
		break;
	case MP_RMTOOL_CHARACTER_MUGONGINFO:
		{
			TMSG_CHARACTER_MUGONGINFO* msg = (TMSG_CHARACTER_MUGONGINFO*)pMsg;
			SetCharacterMugongInfo(&msg->m_sMugongInfo);
		}
		break;
	case MP_RMTOOL_CHARACTER_MUGONGINFO2:
		{
			TMSG_CHARACTER_MUGONGINFO2* msg = (TMSG_CHARACTER_MUGONGINFO2*)pMsg;
			SetCharacterMugongInfoError( msg );
		}
		break; 
	case MP_RMTOOL_CHARACTER_ABILLITYINFO:
		{
			TMSG_CHARACTER_ABILITYINFO* msg = (TMSG_CHARACTER_ABILITYINFO*)pMsg;
			SetCharacterAbillityInfo(&msg->sAbility);
		}
		break;
	case MP_RMTOOL_CHARACTERINFO_ITEMOPTIONINFO:
		{
			TMSG_ITEMOPTIONINFO* msg = (TMSG_ITEMOPTIONINFO*)pMsg;
			SetCharacterItemOptionInfo( msg->wCount, msg->OptionInfo, msg->bEnd );
		}
		break;
	case MP_RMTOOL_CHARACTERINFO_NACK:
		{
			TMSG_WORD* msg = (TMSG_WORD*)pMsg;
			switch(msg->wData)
			{
			case 0:
				break;
			case 1:
				break;
			default:
				break;
			}
		}
		break;
		
	case MP_RMTOOL_CHARACTER_MAINQUESTINFO_ACK:
		{
			TMSG_MAINQUESTINFO* pmsg = (TMSG_MAINQUESTINFO*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateMainQuestData( pmsg );
		}
		break;
	case MP_RMTOOL_CHARACTER_SUBQUESTINFO_ACK:
		{
			TMSG_SUBQUESTINFO* pmsg = (TMSG_SUBQUESTINFO*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->UpdateSubQuestData( pmsg );
		}
		break;

	case MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_ACK:
		{
			TMSG_DWORD2* pmsg = (TMSG_DWORD2*)pMsg;
			CCharacterView* pView = (CCharacterView*)GetView(0);
			pView->ChangeLoginPoint( pmsg->dwData2 );
		}
		break;
	case MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_NACK:
		break;
	}
}

void CCharacterDoc::OpenCharacterInfo(DWORD dwServer, DWORD dwUserIdx, DWORD dwCharIdx, BOOL bReadOnly)
{
	m_dwUserIdx = dwUserIdx;
	m_dwCharIdx = dwCharIdx;

	m_bReadOnly = bReadOnly;

	// 캐릭터 정보 요청..
	TMSG_DWORD3 Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTERINFO_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData1 = dwServer;			//서버 정보
	Msg.dwData2 = dwUserIdx;		//유저 인덱스
	Msg.dwData3 = dwCharIdx;		//캐릭터 인덱스

	NETWORK->Send(&Msg, sizeof(Msg));
}

//-------------------------------------------------------------------
// set fn
//-------------------------------------------------------------------
void CCharacterDoc::SetCharacterBaseInfo(BASEOBJECT_INFO* objInfo, CHARACTER_TOTALINFO* totalInfo, HERO_TOTALINFO* heroInfo)
{
	memcpy(&m_sBaseInfo, objInfo, sizeof(BASEOBJECT_INFO));
	memcpy(&m_sTotalInfo, totalInfo, sizeof(CHARACTER_TOTALINFO));
	memcpy(&m_sHTotalInfo, heroInfo, sizeof(HERO_TOTALINFO));

	SetLoadingFlag(eCL_BaseInfo);
}

void CCharacterDoc::SetCharacterInvenInfo(ITEM_TOTALINFO* info)
{
	memcpy(&m_sInven_org, info, sizeof(ITEM_TOTALINFO));
	
	SetLoadingFlag(eCL_InvenInfo);
}

void CCharacterDoc::SetCharacterInvenInfo2(TMSG_CHARACTER_INVENINFO2* info)
{
	memcpy(&m_InvenInfoError, info, info->GetSize());
	
	SetLoadingFlag(eCL_InvenInfo);
}

void CCharacterDoc::SetCharacterPyogukInfo(WORD wNum, MONEYTYPE money)
{
	m_sPyoguk_org.PyokukCount = wNum;
	m_sPyoguk_org.Money = money;

	if( wNum == 0 )
		SetLoadingFlag(eCL_PyogukInfo | eCL_PyogukItemInfo);
	else
		SetLoadingFlag(eCL_PyogukInfo);
}

void CCharacterDoc::SetCharacterPyogukItemInfo(WORD wCount, ITEMBASE* item, BOOL bEnd)
{
	WORD count = 0;
	for(WORD i=0;i<wCount;++i)
	{
		POSTYPE itempos = item[i].Position - TP_PYOGUK_START;

		if( m_sPyoguk_org.Pyokuk[itempos].dwDBIdx == 0 )
		{
			m_sPyoguk_org.Pyokuk[itempos].wIconIdx = item[i].wIconIdx;
			m_sPyoguk_org.Pyokuk[itempos].dwDBIdx = item[i].dwDBIdx;
			m_sPyoguk_org.Pyokuk[itempos].Position = item[i].Position;
			m_sPyoguk_org.Pyokuk[itempos].Durability = item[i].Durability;
			m_sPyoguk_org.Pyokuk[itempos].QuickPosition	= item[i].QuickPosition;
		}
		else
		{
			m_WareError.Pyokuk[count].wIconIdx = item[i].wIconIdx;
			m_WareError.Pyokuk[count].dwDBIdx = item[i].dwDBIdx;
			m_WareError.Pyokuk[count].Position = item[i].Position;
			m_WareError.Pyokuk[count].Durability = item[i].Durability;
			m_WareError.Pyokuk[count].QuickPosition	= item[i].QuickPosition;

			++count;
			m_WareError.PyokukCount = count;
		}
	}

	if( bEnd )
		SetLoadingFlag(eCL_PyogukItemInfo);
}

void CCharacterDoc::SetCharacterMugongInfo(MUGONG_TOTALINFO* info)
{
	memcpy(&m_sMugong_org, info, sizeof(MUGONG_TOTALINFO));

	SetLoadingFlag(eCL_MugongInfo);
}

void CCharacterDoc::SetCharacterMugongInfoError( TMSG_CHARACTER_MUGONGINFO2* pinfo )
{
	memcpy(&m_MugongError, pinfo, sizeof(TMSG_CHARACTER_MUGONGINFO2));

	SetLoadingFlag(eCL_MugongInfo);
}

void CCharacterDoc::SetCharacterAbillityInfo(ABILITY_TOTALINFO* info)
{
	memcpy(&m_sAbility_org, info, sizeof(ABILITY_TOTALINFO));

	SetLoadingFlag(eCL_AbillityInfo);
}

void CCharacterDoc::SetCharacterItemOptionInfo(WORD wCount, ITEM_OPTION_INFO* info, BOOL bEnd)
{
	ITEM_OPTION_INFO* pOption = NULL;
	for(int i=0;i<wCount;++i)
	{
		pOption = new ITEM_OPTION_INFO;
		memcpy(pOption, &info[i], sizeof(ITEM_OPTION_INFO));
		m_ItemOptionInfo_org.Add(pOption, pOption->dwOptionIdx);
	}

	for(i=0;i<wCount;++i)
	{
		pOption = new ITEM_OPTION_INFO;
		memcpy(pOption, &info[i], sizeof(ITEM_OPTION_INFO));
		m_ItemOptionInfo.Add(pOption, pOption->dwOptionIdx);
	}

	if( bEnd )
		SetLoadingFlag(eCL_ItemOptionInfo);
}

//-------------------------------------------------------------------
BYTE CCharacterDoc::GetCharacterAbillityLevel(BYTE kind, WORD Index)
{
	BYTE Level;
	char* pLevels;

	pLevels = m_sAbility.AbilityDataArray[kind];

	WORD n = Index - (ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*kind);
	ASSERT(n<MAX_ABILITY_NUM_PER_GROUP);

	Level = GetAbilityLevel_WithDBValue(pLevels[n]);

	return Level;
}

void CCharacterDoc::SetAbilityLevel(BYTE kind, WORD Index, char level)
{
	char* pLevels;
	pLevels = m_sAbility.AbilityDataArray[kind];

	WORD n = Index - (ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*kind);
	ASSERT(n<MAX_ABILITY_NUM_PER_GROUP);

	pLevels[n] = level;
}

ITEM_OPTION_INFO* CCharacterDoc::GetItemOptionInfo(WORD wOptionIdx)
{
	return m_ItemOptionInfo.GetData(wOptionIdx);
}

ITEM_OPTION_INFO* CCharacterDoc::GetItemOptionInfoOrigin(WORD wOptionIdx)
{
	return m_ItemOptionInfo_org.GetData(wOptionIdx);
}

WORD CCharacterDoc::CreateLocalOptionIndex()
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

WORD CCharacterDoc::AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew )
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

void CCharacterDoc::DelLocalOptionInfo(WORD wIndex)
{
	ITEM_OPTION_INFO* pOption = m_ItemOptionInfo.GetData(wIndex);

	if( pOption )
	{
		delete pOption;

		m_ItemOptionInfo.Remove(wIndex);
		m_OptionKeyGenerator.ReleaseIndex(wIndex);
	}
}

void CCharacterDoc::ChangeLoginPoint( DWORD dwLoginPoint )
{
	TMSG_NAME_DWORD Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_SYN;
	Msg.dwTemplateIdx = GetDocumentIdx();
	Msg.dwChrIdx = m_sBaseInfo.dwObjectID;
	strcpy( Msg.sName, m_sBaseInfo.ObjectName );
	Msg.dwData = dwLoginPoint;
	
	NETWORK->Send( &Msg, sizeof(Msg) );
}
