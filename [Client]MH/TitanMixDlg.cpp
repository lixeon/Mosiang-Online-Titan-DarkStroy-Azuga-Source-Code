// TitanMixDlg.cpp: implementation of the CTitanMixDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanMixDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "cScriptManager.h"
#include "cWindowManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "TitanManager.h"
#include "Item.h"
#include "interface/cIconDialog.h"
#include "./Interface/cGuageBar.h"
#include "AppearanceManager.h"
#include "GameIn.h"

CTitanMixDlg::CTitanMixDlg()
{
	m_pTitanMixDlg = NULL;
	m_pTitanMixProgressBarDlg = NULL;
	m_TitanSize = 0;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	for( int i = 0; i < eTitanMixPos_Max; i++ )
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
}

CTitanMixDlg::~CTitanMixDlg()
{
	m_pTitanMixProgressBarDlg = NULL;
}

void CTitanMixDlg::Linking()
{
	m_pTitanMixDlg = (cIconDialog*)GetWindowForID(TITANMIX_MIXICONDLG);
	m_pTitanSizeBar = (cGuageBar*)GetWindowForID(TITANMIX_GB_GAMMA);

	m_TitanMixCtrl.titanType		= (cStatic*)GetWindowForID(TITANMIX_CHARTYPE);
	m_TitanMixCtrl.titanAtt			= (cStatic*)GetWindowForID(TITANMIX_CHARATTACK);
	m_TitanMixCtrl.titanCritical	= (cStatic*)GetWindowForID(TITANMIX_CRITICAL);
	m_TitanMixCtrl.titanLongAtt		= (cStatic*)GetWindowForID(TITANMIX_LONGATTACK);
	m_TitanMixCtrl.titanDistance	= (cStatic*)GetWindowForID(TITANMIX_DISTANCE);
	m_TitanMixCtrl.titanLife		= (cStatic*)GetWindowForID(TITANMIX_CHARLIFE);
	m_TitanMixCtrl.titanDef			= (cStatic*)GetWindowForID(TITANMIX_CHARDEFENSE);
	//m_TitanMixCtrl.titanMana		= (cStatic*)GetWindowForID(TITANMIX_CHARNAERYUK);
	m_TitanMixCtrl.titanMagicAttack	= (cStatic*)GetWindowForID(TITANMIX_MAGICATTACK);

	for( int i = 0; i < ATTR_MAX; i++ )
	{
		m_TitanMixCtrl.TitanAttrDef.pStatic.SetElement_Val(ATTR_FIRE+i,(cStatic *)GetWindowForID(TITANMIX_CHARHWA+i));
		m_TitanMixCtrl.TitanAttrDef.pGuage.SetElement_Val(ATTR_FIRE+i,(cGuagen *)GetWindowForID(TITANMIX_DEFENCE_HWA+i));
	}

	m_pTitanMixProgressBarDlg = GAMEIN->GetTitanMixProgressBarDlg();
	m_pTitanMixProgressBarDlg->Linking();
	m_pTitanMixProgressBarDlg->SetSuccessTime(5000); //5초
}

//2007. 10. 23. CBH - 게이지 바 처리를 위해 재정의
void CTitanMixDlg::Render()
{
	if(m_pTitanMixProgressBarDlg->GetSuccessProgress() == TRUE)
	{
		m_pTitanMixProgressBarDlg->InitProgress();
		SetDisable(FALSE);
		TitanMix();		
	}

	cDialog::Render();
}

void CTitanMixDlg::Release(eTitanMixReleaseOpt op, BOOL bFlag)
{
	CVirtualItem * pVItem;

	// 해당 Icon정보들을 삭제
	if(!m_pTitanMixDlg->IsAddable(eTitanMixPos_Head) && (op == eTMixHeadRelease))
	{
		m_pTitanMixDlg->DeleteIcon( eTitanMixPos_Head, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANMIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}
	if(!m_pTitanMixDlg->IsAddable(eTitanMixPos_Body) && (op == eTMixBodyRelease))
	{
		m_pTitanMixDlg->DeleteIcon( eTitanMixPos_Body, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANMIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}
	if(!m_pTitanMixDlg->IsAddable(eTitanMixPos_Hands) && (op == eTMixHandsRelease))
	{
		m_pTitanMixDlg->DeleteIcon( eTitanMixPos_Hands, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANMIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}
	if(!m_pTitanMixDlg->IsAddable(eTitanMixPos_Legs) && (op == eTMixLegsRelease))
	{
		m_pTitanMixDlg->DeleteIcon( eTitanMixPos_Legs, (cIcon**)&pVItem );
		pVItem->GetLinkItem()->SetLock(FALSE);
		if( bFlag )	// 서버에 락을 해체해야함
		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANMIX_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->SetLinkItem(NULL);
	}

	// 전부다 지울때
	if(op == eTMixAllRelease)
	{
		for( int i = 0; i < eTitanMixPos_Max; i++ )
		{
			if(!m_pTitanMixDlg->IsAddable(i))
			{
				m_pTitanMixDlg->DeleteIcon( i, (cIcon**)&pVItem );
				pVItem->GetLinkItem()->SetLock(FALSE);
				if( bFlag )	// 서버에 락을 해체해야함
				{
					MSG_WORD msg;
					msg.Category	= MP_ITEM;
					msg.Protocol	= MP_ITEM_TITANMIX_RELEASEITEM;
					msg.dwObjectID	= HEROID;
					msg.wData		= pVItem->GetLinkItem()->GetPosition();
					NETWORK->Send( &msg, sizeof(msg) );
				}
				pVItem->SetLinkItem(NULL);
			}
		}
	}
}

void CTitanMixDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	for( int i = 0; i < ATTR_MAX; i++ )
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_FIRE+i, 0, RGB_HALF(255,255,255));

	if( !val )
	{
		UpdateData(NULL, FALSE);
		Release();

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		if( HERO->IsTitanPreView() )
			HERO->ReleaseTitanPreView();
	}
}

DWORD CTitanMixDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < eTitanMixPos_Max ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}
	return we;
}

BOOL CTitanMixDlg::OnActionEvent( LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case TITANMIX_LOOKBTN:	// 미리보기
		{
			// 아이템이 전부 들어가있는지 체크
			CVirtualItem * pItem = NULL;
			DWORD chkTitanIdx = 0;
			for( int i = 0; i < eTitanMixPos_Max; i++ )
			{
				pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head+i);
				if(NULL == pItem)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(618) );
					return FALSE;
				}
			}

			// 아이템이 전부 들어가있을때 4개다 같은 속성인지 파악(기준은 머리로 한다)
			pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head);
			chkTitanIdx = TITANMGR->GetTitanPartsKind(pItem->GetLinkItem()->GetItemIdx())->dwResultTitanIdx;


			//
			ITEM_INFO* pinfo = ITEMMGR->GetItemInfo( (WORD)chkTitanIdx );
			if( !pinfo )				return FALSE;
			TITAN_APPEARANCEINFO	titaninfo;
            memset( &titaninfo, 0, sizeof(titaninfo) );
			titaninfo.TitanGrade = pinfo->Plus_Value;
			titaninfo.TitanKind = pinfo->Plus_MugongIdx;
			titaninfo.TitanScale = (WORD)m_pTitanSizeBar->GetCurValue();

			for( int i = 0; i < eTitanMixPos_Max; i++ )
			{
				pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head+i);
				TITAN_PARTS_KIND* pTemp = TITANMGR->GetTitanPartsKind(pItem->GetLinkItem()->GetItemIdx());
				if(chkTitanIdx != pTemp->dwResultTitanIdx)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1503) );
					return FALSE;
				}				
			}

			m_TitanSize = m_pTitanSizeBar->GetCurValue()*0.01f;			
			HERO->SetTitanPreView( &titaninfo );
		}
		break;
	case TITANMIX_STARTBTN:	// 제조시작
		{
			MixOkBtn();
		}
		break;	
	}

	return TRUE;
}

void CTitanMixDlg::TitanMix()
{
	CVirtualItem * pItem = NULL;

	MSG_TITANMIX msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_TITANMIX_SYN );
	msg.dwObjectID = HEROID;
	msg.wTitanScale = (WORD)m_pTitanSizeBar->GetCurValue();
	for( int i = 0; i < eTitanMixPos_Max; i++ )
	{
		pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head+i);

		msg.iconInfo[i].dwItemIdx = pItem->GetSrcItemIdx();
		msg.iconInfo[i].itempos = pItem->GetSrcPosition();
	}

	NETWORK->Send(&msg, sizeof(MSG_TITANMIX));

	if( HERO->IsTitanPreView() )
		HERO->ReleaseTitanPreView();		
}

void CTitanMixDlg::MixOkBtn()
{
	// 아이템이 전부 들어가있는지 체크
	CVirtualItem * pItem = NULL;
	DWORD chkTitanIdx = 0;
	for( int i = 0; i < eTitanMixPos_Max; i++ )
	{
		pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head+i);
		if(NULL == pItem)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(618) );
			return;
		}
	}

	// 아이템이 전부 들어가있을때 4개다 같은 속성인지 파악(기준은 머리로 한다)
	pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head);
	chkTitanIdx = TITANMGR->GetTitanPartsKind(pItem->GetLinkItem()->GetItemIdx())->dwResultTitanIdx;

	for( int i = 0; i < eTitanMixPos_Max; i++ )
	{
		pItem = (CVirtualItem *)m_pTitanMixDlg->GetIconForIdx(eTitanMixPos_Head+i);
		TITAN_PARTS_KIND* pTemp = TITANMGR->GetTitanPartsKind(pItem->GetLinkItem()->GetItemIdx());
		if(chkTitanIdx != pTemp->dwResultTitanIdx)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1503) );
			return;
		}
	}

	m_pTitanMixProgressBarDlg->SetActive(TRUE);
	m_pTitanMixProgressBarDlg->StartProgress();
	SetDisable(TRUE);
}

BOOL CTitanMixDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if(icon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pItem = (CItem*)icon;
	if( pItem->IsLocked() )	return FALSE;

	// 인벤토리 아이템이 아니면 FALSE
	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pItem->GetPosition()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(612) );
		return FALSE;
	}

	// 옵션 아이템이면 FALSE
	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	eITEM_KINDBIT bits = pItem->GetItemKind();
	if(bits != eTITAN_ITEM_PARTS)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	// 타이탄 파츠 속성을 가져온다.
	TITAN_PARTS_KIND* pTitanPartsKind = TITANMGR->GetTitanPartsKind(pItem->GetItemIdx());
	if(!pTitanPartsKind)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );	
		return FALSE;
	}

	// 속성이 다른 타이탄 파츠 아이템이 이미 입력되어있으면 안됨.
	for( int i = 0; i < eTitanMixPos_Max; i++ )
	{
		if(m_VirtualItem[i].GetLinkItem())
		{
			TITAN_PARTS_KIND* pTemp = TITANMGR->GetTitanPartsKind(m_VirtualItem[i].GetLinkItem()->GetItemIdx());

			if(pTemp->dwResultTitanIdx != pTitanPartsKind->dwResultTitanIdx)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1503) );
				return FALSE;
			}
		}
	}

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_TITANMIX_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);
	
	UpdateData(pTitanPartsKind->dwResultTitanIdx);	// 해당 파츠의 완성 타이탄 정보를 보여준다.

	// 인벤에서 드래그한 아이템에게 락을 건다.
	pItem->SetLock(TRUE);

	AddVirtualItem((POSTYPE)pTitanPartsKind->dwPartsKind, pItem);	// 파츠 추가

	return FALSE;
}

void CTitanMixDlg::AddVirtualItem(POSTYPE pos, CItem* pItem)
{
	Release((eTitanMixReleaseOpt)pos);

	m_VirtualItem[pos].SetData(pItem->GetItemIdx());
	m_VirtualItem[pos].SetLinkItem(pItem);

	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[pos], ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	}	
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[pos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()));

	if(m_pTitanMixDlg->IsAddable(pos))
		m_pTitanMixDlg->AddIcon(pos, &m_VirtualItem[pos]);
}

void CTitanMixDlg::UpdateData(DWORD titanIdx, BOOL bFlag)
{
	if( bFlag == TRUE )	// 해당 파츠를 넣었을때 능력치 정보를 입력한다.
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo((WORD)titanIdx);
		BASE_TITAN_LIST* pTitanBaseInfo = GAMERESRCMNGR->GetTitanListInfo(pItemInfo->Plus_MugongIdx);	// 타이탄 리스트

		if(!pTitanBaseInfo)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );	
			return;
		}

		char buf[64] = { 0, };

		// 타이탄 타입
		ZeroMemory(buf, sizeof(buf));
		sprintf( buf, CHATMGR->GetChatMsg(1500), pItemInfo->Plus_Value );
		m_TitanMixCtrl.titanType->SetStaticText(buf);

		// 타이탄 최소/최대 공격력
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d ~ %d", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].MeleeAttackPowerMin, pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].MeleeAttackPowerMax);
		m_TitanMixCtrl.titanAtt->SetStaticText(buf);

		// 타이탄 원거리 공격력
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d ~ %d", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].RangeAttackPowerMin, pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].RangeAttackPowerMax);
		m_TitanMixCtrl.titanLongAtt->SetStaticText(buf);

		// 타이탄 외/내공 일격
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].Critical);
		m_TitanMixCtrl.titanCritical->SetStaticText(buf);

		// 타이탄 사정거리
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%.2f", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttackRange);
		m_TitanMixCtrl.titanDistance->SetStaticText(buf);

		// 타이탄 생명력
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].MaxFuel);
		m_TitanMixCtrl.titanLife->SetStaticText(buf);

		// 타이탄 방어력
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].PhyDef);
		m_TitanMixCtrl.titanDef->SetStaticText(buf);
		/*
		// 타이탄 내력
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%d", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].MaxSpell);
		m_TitanMixCtrl.titanMana->SetStaticText(buf);
		*/
		// 타이탄 마법공격(화 수 목 금 토 전부 동일한 수치를 넣었으므로 어떤 값을 넣어도상관없음)
		ZeroMemory(buf, sizeof(buf));
		sprintf(buf, "%.1f", pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttAtk.GetElement_Val(ATTR_FIRE));
		m_TitanMixCtrl.titanMagicAttack->SetStaticText(buf);

		// 타이탄 저항력
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_FIRE, (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_FIRE), RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_WATER, (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_WATER), RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_TREE, (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_TREE), RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_IRON, (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_IRON), RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_EARTH, (WORD)pTitanBaseInfo->GradeInfo[pItemInfo->Plus_Value - 1].AttReg.GetElement_Val(ATTR_EARTH), RGB_HALF(255,255,255));
	}
	else	// 다이얼로그창을 종료할때 모든 정보를 초기화한다.
	{
		m_TitanMixCtrl.titanType->SetStaticText("");
		m_TitanMixCtrl.titanType->SetStaticText("");
		m_TitanMixCtrl.titanAtt->SetStaticText("");
		m_TitanMixCtrl.titanCritical->SetStaticText("");
		m_TitanMixCtrl.titanLongAtt->SetStaticText("");
		m_TitanMixCtrl.titanDistance->SetStaticText("");
		m_TitanMixCtrl.titanLife->SetStaticText("");
		m_TitanMixCtrl.titanDef->SetStaticText("");
		//m_TitanMixCtrl.titanMana->SetStaticText("");
		m_TitanMixCtrl.titanMagicAttack->SetStaticText("");

		// 타이탄 저항력
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_FIRE, 0, RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_WATER, 0, RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_TREE, 0, RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_IRON, 0, RGB_HALF(255,255,255));
		m_TitanMixCtrl.TitanAttrDef.SetValue(ATTR_EARTH, 0, RGB_HALF(255,255,255));
	}
}

void CTitanMixDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}
