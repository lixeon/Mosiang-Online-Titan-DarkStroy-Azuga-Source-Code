#include "stdafx.h"
#include "ChangeValue.h"
#include "MunpaDoc.h"
#include "CharacterDoc.h"
#include "RMNetwork.h"
//#include "Gameresourcemanager.h"

/*
´©°¡ : ¿î¿µÀÚ ID
¾îµð¼­ : Á¢¼Ó Ip

¹«¾úÀ» : item, mugong, money, characterInfo
¾î¶»°Ô : ÀÌÀüData / º¯°æData ÀúÀå (insert/update/delete)

¾ðÁ¦ : Getdate();
¿Ö : º¯°æ »çÀ¯ ÀÛ¼º... (Ãß°¡)
*/

////////////////////////////////////////////////////////////////////////////////////////////
//// Class CChangeVlue
CChangeValue::CChangeValue()
{
}

CChangeValue::~CChangeValue()
{

}

void CChangeValue::Init(OPER_KIND OperKind, TABLE_KIND TableKind, WORD wPos)
{
	m_sInfo.OperKind = OperKind;
	m_sInfo.TableKind = TableKind;

	m_wPos = wPos;
}
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////
//// Class CChangeData (EditÀÇ DataÀÇ º¯°æÀ» Á¦¾îÇÑ´Ù.)
CChangeData::CChangeData()
{
}

CChangeData::~CChangeData()
{
}

void CChangeData::SetData(char* strBefore, char* strAfter, BYTE bType)
{
	// bType-> 0:StringType, 1:ValueType
	m_strBeforeData = strBefore;
	m_strAfterData = strAfter;

	m_bType = bType;
}

void CChangeData::ApplyChangeValue( CBaseDoc* pDoc )
{
	// ¿î¿µ·Î±×¸¦ ³²±ä´Ù.
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//// Class CChangeItem (ItemÀÇ DataÀÇ º¯°æÀ» Á¦¾îÇÑ´Ù.)
CChangeItem::CChangeItem()
{
	memset(&m_BeforeItem, 0, sizeof(ITEMBASE));
	memset(&m_AfterItem, 0, sizeof(ITEMBASE));

	memset(&m_BeforeOption, 0, sizeof(ITEM_OPTION_INFO));
	memset(&m_AfterOption, 0, sizeof(ITEM_OPTION_INFO));
}

CChangeItem::~CChangeItem()
{
}

void CChangeItem::SetData( ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption )
{
	memcpy(&m_BeforeItem, pBItem, sizeof(ITEMBASE));
	memcpy(&m_AfterItem, pAItem, sizeof(ITEMBASE));

	if( pBOption )
		memcpy(&m_BeforeOption, pBOption, sizeof(ITEM_OPTION_INFO));

	memcpy(&m_AfterOption, pAOption, sizeof(ITEM_OPTION_INFO));
}

DWORD CChangeItem::GetTargetIdx( CBaseDoc* pDoc )
{
	if( m_sInfo.TableKind == eTK_InventoryItem )
		return ((CCharacterDoc*)pDoc)->m_sBaseInfo.dwObjectID;
	else if( m_sInfo.TableKind == eTK_PyogukItem )
		return ((CCharacterDoc*)pDoc)->m_sBaseInfo.dwUserID;
	else if( m_sInfo.TableKind == eTK_MunpaItem )
		return ((CMunpaDoc*)pDoc)->m_sBaseInfo_org.dwMunpaIdx;
	else
		ASSERT(0);

	return 0;
}

char* CChangeItem::GetTargetName( CBaseDoc* pDoc )
{
	if( m_sInfo.TableKind == eTK_InventoryItem )
		return ((CCharacterDoc*)pDoc)->m_sBaseInfo.ObjectName;
	else if( m_sInfo.TableKind == eTK_PyogukItem )
		return ((CCharacterDoc*)pDoc)->m_sBaseInfo.ObjectName;
	else if( m_sInfo.TableKind == eTK_MunpaItem )
		return ((CMunpaDoc*)pDoc)->m_sBaseInfo_org.strMunpaName;
	return NULL;	
}

void CChangeItem::ApplyChangeValue( CBaseDoc* pDoc )
{
	// Send To DB
	if(	m_sInfo.OperKind == eOp_Insert )
	{
		if( m_BeforeItem.dwDBIdx != 0 )
		{ // Update·Î ¹Ù²Û´Ù.
			TMSG_ITEM_UPDATE Msg;
			Msg.bDeleteOption = 0;
			if( GAMERESRCMNGR->IsOptionItem(m_BeforeItem.wIconIdx, m_BeforeItem.Durability) && m_AfterOption.dwOptionIdx == 0 )
					Msg.bDeleteOption = 1;
			Msg.Category = MP_RMTOOL_ITEM;
			Msg.Protocol = MP_RMTOOL_UPDATEITEM;
			Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
			Msg.dwCharIdx = GetTargetIdx(pDoc);
			Msg.dwDBIdx = m_BeforeItem.dwDBIdx;
			Msg.dwOptionIdx = m_BeforeOption.dwOptionIdx;
			Msg.Item = m_AfterItem;
			Msg.Option = m_AfterOption;
			strcpy( Msg.sCharName, GetTargetName( pDoc ) );

			NETWORK->Send(&Msg, sizeof(Msg));
			return;
		}

		TMSG_ITEM_INSERT Msg;
		Msg.Category = MP_RMTOOL_ITEM;
		Msg.Protocol = MP_RMTOOL_INSERTITEM;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.bTable = GetTableIdx();
		Msg.dwTagetIdx = GetTargetIdx(pDoc);
		Msg.Item = m_AfterItem;
		Msg.Option = m_AfterOption;
		strcpy( Msg.sCharName, GetTargetName( pDoc ) );
		
		NETWORK->Send(&Msg, sizeof(Msg));
	}
	else if( m_sInfo.OperKind == eOp_Update )
	{
		if( m_BeforeItem.dwDBIdx == 0 )
		{ // Insert·Î ¹Ù²Û´Ù.
			TMSG_ITEM_INSERT Msg;
			Msg.Category = MP_RMTOOL_ITEM;
			Msg.Protocol = MP_RMTOOL_INSERTITEM;
			Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
			Msg.bTable = GetTableIdx();
			Msg.dwTagetIdx = GetTargetIdx(pDoc);
			Msg.Item = m_AfterItem;
			Msg.Option = m_AfterOption;
			strcpy( Msg.sCharName, GetTargetName( pDoc ) );
			
			NETWORK->Send(&Msg, sizeof(Msg));
			return;
		}

		TMSG_ITEM_UPDATE Msg;
		Msg.bDeleteOption = 0;
		if( GAMERESRCMNGR->IsOptionItem(m_BeforeItem.wIconIdx, m_BeforeItem.Durability) && m_AfterOption.dwOptionIdx == 0 )
			Msg.bDeleteOption = 1;

		Msg.Category = MP_RMTOOL_ITEM;
		Msg.Protocol = MP_RMTOOL_UPDATEITEM;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwCharIdx = GetTargetIdx(pDoc);
		Msg.dwDBIdx = m_BeforeItem.dwDBIdx;
		Msg.dwOptionIdx = m_BeforeOption.dwOptionIdx;
		Msg.Item = m_AfterItem;
		Msg.Option = m_AfterOption;
		strcpy( Msg.sCharName, GetTargetName( pDoc ) );
		
		NETWORK->Send(&Msg, sizeof(Msg));
	}
	else if( m_sInfo.OperKind == eOp_Delete )
	{
		if( m_BeforeItem.dwDBIdx == 0 )
			return;

		TMSG_ITEM_DELETE Msg;
		Msg.Category = MP_RMTOOL_ITEM;
		Msg.Protocol = MP_RMTOOL_DELETEITEM;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwCharIdx = GetTargetIdx(pDoc);
		Msg.dwItemIdx = m_BeforeItem.wIconIdx;
		Msg.wItemPosition = m_BeforeItem.Position;
		Msg.dwDBIdx = m_BeforeItem.dwDBIdx;
		Msg.dwOptionIdx = m_BeforeOption.dwOptionIdx;
		strcpy( Msg.sCharName, GetTargetName( pDoc ) );

		NETWORK->Send(&Msg, sizeof(Msg));
	}
	else
	{
		ASSERT(0);
		return;
	}

	// ¿î¿µ·Î±×¸¦ ³²±ä´Ù.
}
////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////
//// Class CChangeMugong (MugongÀÇ DataÀÇ º¯°æÀ» Á¦¾îÇÑ´Ù.)
CChangeMugong::CChangeMugong()
{
}

CChangeMugong::~CChangeMugong()
{
}

void CChangeMugong::SetData( MUGONGBASE* pBMugong, MUGONGBASE* pAMugong )
{
	memcpy(&m_BeforeMugong, pBMugong, sizeof(MUGONGBASE));
	memcpy(&m_AfterMugong, pAMugong, sizeof(MUGONGBASE));
}

DWORD CChangeMugong::GetTargetIdx(CBaseDoc* pDoc)
{
	return ((CCharacterDoc*)pDoc)->m_sBaseInfo.dwObjectID;
}

char* CChangeMugong::GetTargetName(CBaseDoc* pDoc)
{
	return ((CCharacterDoc*)pDoc)->m_sBaseInfo.ObjectName;
}

void CChangeMugong::ApplyChangeValue( CBaseDoc* pDoc )
{
	// Send to DB 
	if(	m_sInfo.OperKind == eOp_Insert )
	{
		if( m_BeforeMugong.dwDBIdx != 0 )
		{ // Update·Î ¹Ù²Þ
			TMSG_MUGONG_EDIT Msg;
			Msg.Category = MP_RMTOOL_CHARACTER;
			Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATEMUGONG;
			Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
			Msg.dwData = GetTargetIdx(pDoc);
			Msg.Mugong = m_AfterMugong;
			strcpy( Msg.sName, GetTargetName(pDoc) );

			NETWORK->Send(&Msg, sizeof(Msg));

			return;
		}

		TMSG_MUGONG_EDIT Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_INSERTMUGONG;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData = GetTargetIdx(pDoc);
		Msg.Mugong = m_AfterMugong;
		strcpy( Msg.sName, GetTargetName(pDoc) );
		
		NETWORK->Send(&Msg, sizeof(Msg));

	}
	else if( m_sInfo.OperKind == eOp_Update )
	{
		if( m_BeforeMugong.dwDBIdx == 0 )
		{ // Insert ·Î ¹Ù²Þ
			TMSG_MUGONG_EDIT Msg;
			Msg.Category = MP_RMTOOL_CHARACTER;
			Msg.Protocol = MP_RMTOOL_CHARACTER_INSERTMUGONG;
			Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
			Msg.dwData = GetTargetIdx(pDoc);
			Msg.Mugong = m_AfterMugong;
			strcpy( Msg.sName, GetTargetName(pDoc) );

			NETWORK->Send(&Msg, sizeof(Msg));

			return;
		}

		TMSG_MUGONG_EDIT Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_UPDATEMUGONG;;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData = GetTargetIdx(pDoc);
		Msg.Mugong = m_AfterMugong;
		strcpy( Msg.sName, GetTargetName(pDoc) );
		
		NETWORK->Send(&Msg, sizeof(Msg));
	}
	else if( m_sInfo.OperKind == eOp_Delete )
	{
		if( m_BeforeMugong.dwDBIdx == 0 )
			return;

		TMSG_MUGONG_EDIT Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_DELETEMUGONG;;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData = GetTargetIdx(pDoc);
		Msg.Mugong = m_BeforeMugong;
		strcpy( Msg.sName, GetTargetName(pDoc) );

/*		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_DELETEMUGONG;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData = m_BeforeMugong.dwDBIdx;
		*/
		
		NETWORK->Send(&Msg, sizeof(Msg));
	}
	else
	{
		ASSERT(0);
		return;
	}

	// ¿î¿µ·Î±×¸¦ ³²±ä´Ù.
}

CChangePosition::CChangePosition()
{
	m_wKind = 0;		// 1 = Item, 2 = Mugong
	m_dwOwnerIdx = 0;
	m_dwDBIdx = 0;
	m_Position = 0;
}

CChangePosition::~CChangePosition()
{
}

void CChangePosition::ApplyChangeValue( CBaseDoc* pDoc )
{
	if( m_dwOwnerIdx == 0 || m_dwDBIdx == 0 )
		return;

	// Send to DB 
	if(	m_wKind == 0 )
	{
		TMSG_DWORD3 Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHANGE_ITEMPOSITION;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData1 = m_dwOwnerIdx;
		Msg.dwData2 = m_dwDBIdx;
		Msg.dwData3 = m_Position;

		NETWORK->Send(&Msg, sizeof(Msg));
		return;
	}
	else if( m_wKind == 1 )
	{
		TMSG_DWORD3 Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHANGE_WAREITEMPOSITION;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData1 = m_dwOwnerIdx;
		Msg.dwData2 = m_dwDBIdx;
		Msg.dwData3 = m_Position;

		NETWORK->Send(&Msg, sizeof(Msg));
		return;
	}
	else if( m_wKind == 2 )
	{
		TMSG_DWORD3 Msg;
		Msg.Category = MP_RMTOOL_CHARACTER;
		Msg.Protocol = MP_RMTOOL_CHANGE_MUGONGPOSITION;
		Msg.dwTemplateIdx = pDoc->GetDocumentIdx();
		Msg.dwData1 = m_dwOwnerIdx;
		Msg.dwData2 = m_dwDBIdx;
		Msg.dwData3 = m_Position;

		NETWORK->Send(&Msg, sizeof(Msg));
		return;
	}
}

void CChangePosition::SetData( WORD wType, DWORD dwOwnerIdx, DWORD dwDBIdx, POSTYPE position )
{
	m_wKind = wType;		// 0 = Inven, 1 = Ã¢°í, 2 = Mugong
	m_dwOwnerIdx = dwOwnerIdx;
	m_dwDBIdx = dwDBIdx;
	m_Position = position;
}

