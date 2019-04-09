// DissloveDlg.cpp: implementation of the CDissloveDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissloveDlg.h"
#include "Item.h"
#include "INTERFACE\cStatic.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "Gamein.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
#include "reinforcemanager.h"
#include "InventoryExDialog.h"
#include "AbilityManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDissloveDlg::CDissloveDlg()
{
	m_pDissolveDlg = NULL;
	m_pDissolveGridDlg = NULL;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < MAX_DISSOLVEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
}

CDissloveDlg::~CDissloveDlg()
{
	// don't call Release() 
	// pre delete item 
}

void CDissloveDlg::Release(eDissolveReleaseOpt op)
{
	CVirtualItem * pVItem = NULL;

	if((op & eDissolveBasicRelease) && !IsEmpty(0))
	{
		m_pDissolveDlg->DeleteIcon(0, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}


	if( (op & eDissolveViewRelease) )
	{
		for(int i = 1 ; i < MAX_DISSOLVEGRID_NUM+1 ; ++i)
		{
			if(!IsEmpty(i))
			{
				m_pDissolveGridDlg->DeleteIcon(i-1, (cIcon **)&pVItem);
				pVItem->GetLinkItem()->SetLock(FALSE);
				pVItem->SetLinkItem(NULL);
			}
		}		
	}
}
void CDissloveDlg::Linking()
{
	m_pDissolveDlg			= (cIconDialog *)GetWindowForID(DIS_DISSOLVEICONDLG);
	m_pDissolveGridDlg		= (cIconGridDialog *)GetWindowForID(DIS_DISSOLVEICONGRIDDLG);
	m_pDissolveCostStatic	= (cStatic *)GetWindowForID(DIS_DISSOLVECOST);
}

BOOL CDissloveDlg::IsEmpty(POSTYPE pos)
{
	if(pos == 0)
	{
		if(m_pDissolveDlg->IsAddable(0))
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(m_pDissolveGridDlg->IsAddable(pos-1))
			return TRUE;
		else
			return FALSE;
	}
}
DWORD CDissloveDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < MAX_DISSOLVEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}

	return we;
}

BOOL CDissloveDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;

	// 인벤토리이외에 아이템 FALSE
	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	if(ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()))
	{
		if(!IsEmpty(0))
		{
			Release(eDissolveBasicRelease);
		}

		AddVirtualItemWrap(0, pOrigItem);

		const ITEM_INFO * pInfo = pOrigItem->GetItemInfo();
		// 분해 비용 출력 
		SetCost(ITEM_DISSOLVE_PRICE_VALUE*pInfo->LimitLevel);
	}
	else
	{
		// 분해 할 수 없는 아이템 
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(222) );
	}
	return FALSE;
}
void CDissloveDlg::SetCost(DWORD Cost)
{
	char costTxt[128];
	sprintf(costTxt, CHATMGR->GetChatMsg(227), Cost);
	m_pDissolveCostStatic->SetStaticText(costTxt);
}
BOOL CDissloveDlg::EnoughSpace(WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * wMatItemOut, WORD & wMatItemNumOut)
{
	ITEM_OPTION_INFO * pInfo = ITEMMGR->GetItemOption(wOptionIdx);
	if(!pInfo) return FALSE;


	wMatItemNumOut = 0;

	float var = 0;
	DWORD num = 0;
	BOOL bConvert = FALSE;
	sITEM_REINFORCE_INFO * pRInfo = NULL;
	REINFORCEMGR->SetHeadPos();
	while(pRInfo = REINFORCEMGR->GetData())
	{
		switch(pRInfo->ReinforceType)
		{
		case eRJP_GENGOL:
			{
				if(pInfo->GenGol > 0)
				{ 
					var = pInfo->GenGol;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_MINCHUB:
			{
				if(pInfo->MinChub > 0)
				{ 
					var = pInfo->MinChub;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_CHERYUK:
			{
				if(pInfo->CheRyuk > 0)
				{ 
					var = pInfo->CheRyuk;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_SIMMEK:
			{
				if(pInfo->SimMek > 0)
				{ 
					var = pInfo->SimMek;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_LIFE:
			{
				if(pInfo->Life > 0)
				{ 
					var = (float)pInfo->Life;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_NAERYUK:
			{
				if(pInfo->NaeRyuk > 0)
				{ 
					var = pInfo->NaeRyuk;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_SHIELD:
			{
				if(pInfo->Shield > 0)
				{ 
					var = (float)pInfo->Shield;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_FIREREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0.009)
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_FIRE);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_WATERREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0.009)
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_WATER);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_TREEREGIST:
			{								
				if(pInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_TREE);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_GOLDREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_IRON);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_EARTHREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_EARTH);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_PHYATTACK:
			{
				if(pInfo->PhyAttack > 0) 
				{ 
					var = pInfo->PhyAttack;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_CRITICAL:
			{
				if(pInfo->CriticalPercent > 0) 
				{ 
					var = pInfo->CriticalPercent;
					bConvert = TRUE;
				}
			}
			break;		
		case eRJP_FIREATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_FIRE);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_WATERATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_WATER) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_WATER);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_TREEATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_TREE) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_TREE);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_GOLDATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_IRON) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_IRON);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_EARTHATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_EARTH) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_EARTH);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_PHYDEFENSE:
			{
				if(pInfo->PhyDefense > 0)
				{
					var = pInfo->PhyDefense;
					bConvert = TRUE;
				}
			}
			break;	
		default:
			{
				return FALSE;
			}	
		}
		if(bConvert)
		{
//check plz 1020
//			WORD plusNum = var*100;
//			WORD seed = (pRInfo->w*(random(0, 100) - ItemLevel/2+wAbilityGrade));
//			num = plusNum/seed;


			while(num)
			{
				if(ITEMMGR->IsDupItem(pRInfo->wItemIdx))
				{
					if(num > MAX_YOUNGYAKITEM_DUPNUM)
					{
						num -= MAX_YOUNGYAKITEM_DUPNUM;
						wMatItemOut[wMatItemNumOut].wItemIdx = pRInfo->wItemIdx;
						wMatItemOut[wMatItemNumOut].Dur = MAX_YOUNGYAKITEM_DUPNUM;
						wMatItemOut[wMatItemNumOut].ItemPos = 0;
					}
					else
					{
						num = 0;
						wMatItemOut[wMatItemNumOut].wItemIdx = pRInfo->wItemIdx;
						wMatItemOut[wMatItemNumOut].Dur = num;
						wMatItemOut[wMatItemNumOut].ItemPos = 0;
					}
				}
				else
				{
					--num;
					wMatItemOut[wMatItemNumOut].wItemIdx = pRInfo->wItemIdx;
					wMatItemOut[wMatItemNumOut].Dur = 0;
					wMatItemOut[wMatItemNumOut].ItemPos = 0;
				}

				++wMatItemNumOut;
			}
			bConvert = FALSE;	
		}
	}

	ASSERT(wMatItemNumOut<=MAX_DISSOLVEGRID_NUM);
	if(!EnoughInvSpace(wMatItemNumOut))
		return FALSE;
	else
		return TRUE;
}
BOOL CDissloveDlg::EnoughInvSpace(WORD wMatItemNum)
{
	for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && wMatItemNum != 0 ; ++i)
	{
		if(!GAMEIN->GetInventoryDialog()->IsExist(i))
		{
			--wMatItemNum;
		}
	}

	if(wMatItemNum == 0)
		return TRUE;
	else
		return FALSE;
}
BOOL CDissloveDlg::EnoughMoney(LEVELTYPE ItemLevel)
{
	if(HERO->GetMoney() > (MONEYTYPE)(ItemLevel*ITEM_DISSOLVE_PRICE_VALUE))
		return TRUE;
	return FALSE;
}
void CDissloveDlg::DissolveCancelBtn(CDissloveDlg * pThis)
{
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(223) );
}	

BOOL CDissloveDlg::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);
	//!!!기본 NULL 인지 확인 SW050920 Rare
	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()),
		ITEMMGR->GetItemRareOption( pItem->GetRareness()) );
	}
	else
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem[relPos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()) );
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}

BOOL CDissloveDlg::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	//empty 체크후 
	if(relPos == 0)
	{
		if(IsEmpty(0))
		{
			m_pDissolveDlg->AddIcon(0, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(IsEmpty(relPos))
		{
			m_pDissolveGridDlg->AddIcon(relPos-1, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}
}

BOOL CDissloveDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	CDissloveDlg * pDissloveDlg = (CDissloveDlg *)p;
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
			pDissloveDlg->Release();
			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case DIS_DISSOLVECANCELBTN:
		{
			CDissloveDlg::DissolveCancelBtn(pDissloveDlg);
		}
		break;
		
	}

	return TRUE;
}
