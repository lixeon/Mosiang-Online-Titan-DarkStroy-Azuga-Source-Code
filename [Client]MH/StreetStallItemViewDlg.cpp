// StreetStallItemViewDlg.cpp: implementation of the CStreetStallItemViewDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StreetStallItemViewDlg.h"
#include "WindowIDEnum.h"
#include "ItemShow.h"
#include "ItemManager.h"
#include "INTERFACE/cIconGridDialog.h"
#include "ChatManager.h"
#include "PetManager.h"
#include "TitanManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStreetStallItemViewDlg::CStreetStallItemViewDlg()
{
	m_pStallGrid = NULL;

	cImage imgToolTip;
	for( int i = 0; i < SLOT_STREETSTALL_NUM; i++ )
	{
		m_ViewItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_ViewItem[i].SetMovable( FALSE );
	}
}

CStreetStallItemViewDlg::~CStreetStallItemViewDlg()
{

}

void CStreetStallItemViewDlg::Linking()
{
	m_CloseBtn = (cButton*)GetWindowForID(SVD_CLOSEBTN);
	m_pStallGrid = (cIconGridDialog*)GetWindowForID(SVD_ITEMGRID);
}

void CStreetStallItemViewDlg::SetActive( BOOL val )
{
	if( !val )
	{
		// 창 닫힐때 ItemShow의 내용 삭제
		DeleteItemInfo();
	}

	cDialog::SetActiveRecursive( val );
}


void CStreetStallItemViewDlg::SetItemInfo( STREETSTALL_ITEM_INFO* pInfo, WORD wMaxCount )
{
	char buf[64] = { 0, };
	char buf2[64] = { 0, };
	int nItemIdx = 0, nPrice = 0, nDur = 0, nDBIdx = 0, nRare = 0;

	memset( m_sItemInfo, 0, sizeof(m_sItemInfo) );
	memcpy( m_sItemInfo, pInfo, wMaxCount*sizeof(STREETSTALL_ITEM_INFO) );

	for( int i = 0; i < wMaxCount; i++ )
	{
		nItemIdx = m_sItemInfo[i].dwItemIdx;
		nPrice = m_sItemInfo[i].dwPrice;
		nDur = m_sItemInfo[i].dwDur;
		nDBIdx = m_sItemInfo[i].dwDBIdx;
		nRare = m_sItemInfo[i].dwRareIdx;


		if( 0 == nItemIdx )
			continue;

		// ItemShow 에 값 세팅
		m_ViewItem[i].SetData( nItemIdx );
		m_ViewItem[i].Init( nItemIdx, nDur );

		if( ITEMMGR->IsTitanEquipItem( nItemIdx ) )
		{
			ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewItem[i], NULL, NULL, nDBIdx );
		}
		else if( ITEMMGR->IsTitanCallItem( nItemIdx ) )
		{
			ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewItem[i], NULL, NULL, nDBIdx );
		}
		else if( ITEMMGR->IsPetSummonItem( nItemIdx ) )
		{
			ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewItem[i], NULL, NULL, nDBIdx );
		}
		else if( !ITEMMGR->IsDupItem(nItemIdx) && nDur !=0 )
		{
			ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewItem[i], ITEMMGR->GetItemOption(nDur),
				ITEMMGR->GetItemRareOption(nRare) );
		}
		else
			ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewItem[i], NULL, ITEMMGR->GetItemRareOption(nRare) );


		if( ITEMMGR->IsDupItem( (WORD)nItemIdx ) )
		{
			wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( nPrice ) );
			wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( nPrice * nDur ));
		}
		else
			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( nPrice ) );
		m_ViewItem[i].AddToolTipLine( buf, TTTC_BUYPRICE );

		if(buf2[0])
		{
			m_ViewItem[i].AddToolTipLine( buf2, TTTC_BUYPRICE );
			memset( buf2, 0, sizeof(buf2) );
		}


		// Grid Dialog에 추가
		m_pStallGrid->AddIcon( i, (cIcon*)&m_ViewItem[i] );
	}

	if( FALSE == IsActive() )
		SetActive( TRUE );
}


void CStreetStallItemViewDlg::LinkItem( MSG_STREETSTALL_ITEMVIEW* pStallInfo )
{
	DeleteItemInfo();

	CAddableInfoIterator iter(&pStallInfo->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				ITEM_OPTION_INFO OptionInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&OptionInfo);
				ITEMMGR->InitItemOption(OptionInfo, pStallInfo->wOptioncount);
			}
			break;
		case(CAddableInfoList::ItemRareOption):
			{
				ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&RareOptionInfo);
				ITEMMGR->InitItemRareOption(RareOptionInfo, pStallInfo->wRareCount);
			}
			break;
		case(CAddableInfoList::PetTotalInfo):
			{
				PET_TOTALINFO PetInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&PetInfo);
				PETMGR->InitPetInfo(PetInfo, pStallInfo->wPetItemCount);
			}
			break;
		case(CAddableInfoList::TitanTotalInfo):
			{
				TITAN_TOTALINFO TitanInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&TitanInfo);
				TITANMGR->InitTitanInfo(TitanInfo, pStallInfo->wTitanItemCount);
			}
			break;
		case(CAddableInfoList::TitanEndrncInfo):
			{
				TITAN_ENDURANCE_ITEMINFO TItanEquipInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&TItanEquipInfo);
				TITANMGR->InitTitanEquipItemEnduranceInfo(TItanEquipInfo, pStallInfo->wTitanEquipItemCount);
			}
			break;
		}
		iter.ShiftToNextData();
	}

	SetItemInfo( pStallInfo->sInfo , pStallInfo->wCount );
}

void CStreetStallItemViewDlg::DeleteItemInfo()
{
	int nCnt = m_pStallGrid->GetCellNum();
	CItemShow * pSItemOut = NULL;
	ITEM_INFO* pInfo;
	for( int i = 0 ; i < nCnt; i++ )
	{
		pSItemOut = NULL;
		m_pStallGrid->DeleteIcon(i, (cIcon**)&pSItemOut);

		// 삭제된 아이콘이 없다는 말은 아이템이 없었다는 말이므로 넘겨준다.
		if( NULL == pSItemOut )
			continue;

		pInfo = ITEMMGR->GetItemInfo(m_sItemInfo[i].dwItemIdx);
		if( !pInfo )
			ASSERT(0);

		// 샵아이템이 아닐때만 삭제
		if( !( pInfo->ItemKind & eSHOP_ITEM ) )
		{
			// 강화 아이템 정보 삭제
			if( !ITEMMGR->IsDupItem( m_sItemInfo[i].dwItemIdx ) && m_sItemInfo[i].dwDur != 0 )
				ITEMMGR->RemoveItemOption( m_sItemInfo[i].dwDur );			
			// 레어 아이템 정보 삭제
			else if( ITEMMGR->IsRareOptionItem(m_sItemInfo[i].dwItemIdx, m_sItemInfo[i].dwRareIdx ) )
				ITEMMGR->RemoveItemRareOption(m_sItemInfo[i].dwRareIdx);

			// 펫 아이템 정보 삭제
			if( ITEMMGR->IsPetSummonItem(m_sItemInfo[i].dwItemIdx) )
				PETMGR->RemovePetInfo(m_sItemInfo[i].dwDBIdx);

			// 타이탄 장착 아이템 정보 삭제
			if( TITANMGR->GetTitanEnduranceInfo(m_sItemInfo[i].dwDBIdx) )
				TITANMGR->RemoveTitanEquipInfo(m_sItemInfo[i].dwDBIdx);
		}
	}
}

BOOL CStreetStallItemViewDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case SVD_CLOSEBTN:
		{
			SetActive(FALSE);
		}
		break;
	}

	return TRUE;
}
