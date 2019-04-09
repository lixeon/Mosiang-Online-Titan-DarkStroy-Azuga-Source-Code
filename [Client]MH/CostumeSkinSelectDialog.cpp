// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameIn.h"
#include "CostumeSkinSelectDialog.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "AppearanceManager.h"

CCostumeSkinSelectDialog::CCostumeSkinSelectDialog()
{
	m_pCostumeSkinListDlg = NULL;
	m_pCostumeSkinIconDlg = NULL;

	m_dwSelectIdx = 0;	
	//InitSkinDelayTime();

	m_CostumeSkinHat.Initialize(50);	//머리 종류 데이터
	m_CostumeSkinDress.Initialize(50);	//옷 종류 데이터
	m_CostumeSkinAccessory.Initialize(50);	//악세사리 종류 데이터
}

CCostumeSkinSelectDialog::~CCostumeSkinSelectDialog()
{
	m_pCostumeSkinListDlg->RemoveAll();

	m_pCostumeSkinListDlg = NULL;
	m_pCostumeSkinIconDlg = NULL;

	m_CostumeSkinHat.RemoveAll();	
	m_CostumeSkinDress.RemoveAll();
	m_CostumeSkinAccessory.RemoveAll();
}

void CCostumeSkinSelectDialog::Linking()
{
	m_pCostumeSkinListDlg = (cListDialog*)GetWindowForID(COSTUME_SKIN_SELECTLIST);
	m_pCostumeSkinListDlg->SetShowSelect(TRUE);
	
	m_pCostumeSkinIconDlg = (cIconDialog*)GetWindowForID(COSTUME_SKIN_SELECT_ITEMVIEW);			

	m_CostumeSkinView.SetMovable(FALSE);	

	m_pCostumTabBtn[eTabBtn_Hat] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_HAT);
	m_pCostumTabBtn[eTabBtn_Dress] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_DRESS);
	m_pCostumTabBtn[eTabBtn_Accessory] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_ACCESSORY);

	CostumeSkinKindData();	//종류별로 데이터를 나눈다.
}

void CCostumeSkinSelectDialog::SetActive( BOOL val )
{	
	cDialog::SetActive( val );
	if( val == FALSE )	
	{
		m_pCostumeSkinListDlg->RemoveAll();
		m_pCostumeSkinListDlg->SetCurSelectedRowIdx(-1);
		m_pCostumeSkinIconDlg->DeleteIconAll();
		m_dwSelectIdx = 0;
	}
	else
	{
		SetCostumTabBtnFocus(eTabBtn_Hat);
		CostumeSkinListInfo(eTabBtn_Hat);
	}
}

BOOL CCostumeSkinSelectDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{			
	case WE_CLOSEWINDOW:
		{	
			return TRUE;
		}
		break;
	}

	switch( lId )
	{		
	case COSTUME_SKIN_SELECT_OK:	//확인 버튼
		{
			if(m_dwSelectIdx > 0)
			{
				SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GetCurrentSkinInfo(m_dwSelectIdx);
				if(NULL == pSkinItemInfo)
					break;

				//성별체크
				AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pSkinItemInfo->wEquipItem[0] );
				if(!pAvatarItem)
					break;

				if( (pAvatarItem->Gender < GENDER_MAX) && (pAvatarItem->Gender != APPEARANCEMGR->GetGenderFromMap(HERO)) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(778) );
					break;
				}

				//딜레이 체크
				if( FALSE == HERO->CheckSkinDelay() )
					break;

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pSkinItemInfo->dwIndex;
				msg.dwData2 = eSHOP_ITEM_COSTUME_SKIN;

				NETWORK->Send(&msg, sizeof(MSG_DWORD2));
			}			
		}
		break;
	case COSTUME_SKIN_SELECT_CANCEL:	//취소 버튼
		{
			SetActive( FALSE );
		}
		break;
	case COSTUME_SKIN_SELECT_RECOVERY:	//원래대로 되돌리기 버튼
		{
			//딜레이 체크
			if( FALSE == HERO->CheckSkinDelay() )
				break;		

			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
			msg.dwObjectID = HEROID;
			msg.dwData1 = 0;	//0번으로 셋팅하면 원래대로 되돌리기
			msg.dwData2 = eSHOP_ITEM_COSTUME_SKIN;

			NETWORK->Send(&msg, sizeof(MSG_DWORD2));
		}
		break;
	case COSTUME_TABBTN_HAT:
		{
			SetCostumTabBtnFocus(eTabBtn_Hat);			
			CostumeSkinListInfo(eTabBtn_Hat);
		}
		break;
	case COSTUME_TABBTN_DRESS:
		{			
			SetCostumTabBtnFocus(eTabBtn_Dress);			
			CostumeSkinListInfo(eTabBtn_Dress);
		}
		break;
	case COSTUME_TABBTN_ACCESSORY:
		{
			SetCostumTabBtnFocus(eTabBtn_Accessory);
			CostumeSkinListInfo(eTabBtn_Accessory);
		}
		break;
	}

	return TRUE;
}

DWORD CCostumeSkinSelectDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pCostumeSkinListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{		
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pCostumeSkinListDlg->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx + 1; //스킨 인덱스는 1부터 시작하기 떄문에 +1을 해준다

				// 해당 인덱스의 아이콘 이미지를 설정
				if(m_dwSelectIdx > 0)
				{					
					SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GetCurrentSkinInfo(m_dwSelectIdx);
					if(pSkinItemInfo != NULL)
					{
						m_pCostumeSkinIconDlg->DeleteIconAll();

						m_CostumeSkinView.SetData(pSkinItemInfo->wEquipItem[0]);
						m_CostumeSkinView.Init(pSkinItemInfo->wEquipItem[0], NULL);
							
						m_pCostumeSkinIconDlg->AddIcon(0, (cIcon*)&m_CostumeSkinView);
					}					
				}
			}
		}
	}	

	return we;
}

void CCostumeSkinSelectDialog::CostumeSkinListInfo(TAB_BTN eCostumeKind)
{	
	m_pCostumeSkinListDlg->RemoveAll();

	switch(eCostumeKind)
	{
	case eTabBtn_Hat:
		m_pCurrentSkinTable = &m_CostumeSkinHat;
		break;
	case eTabBtn_Dress:
		m_pCurrentSkinTable = &m_CostumeSkinDress;
		break;
	case eTabBtn_Accessory:
		m_pCurrentSkinTable = &m_CostumeSkinAccessory;
		break;
	}
	
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
	m_pCurrentSkinTable->SetPositionHead();
	while(pSkinInfo = m_pCurrentSkinTable->GetData())
	{
		if(pSkinInfo == NULL)
			continue;

		m_pCostumeSkinListDlg->AddItem(pSkinInfo->szSkinName, RGBA_MAKE(255,255,255,255));
	}
}
/*
void CCostumeSkinSelectDialog::Render()
{
	cDialog::Render();

	//딜레이 처리 Process
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwCurrentTime = gCurTime;
		if(dwCurrentTime > m_dwSkinDelayTime)
		{
			InitSkinDelayTime();
		}
	}
}
*/
/*
void CCostumeSkinSelectDialog::InitSkinDelayTime()
{
	m_dwSkinDelayTime = 0;	//딜레이 총 시간 변수
	m_bSkinDelayResult = FALSE;	//딜레이중인지 체크하는 변수
}

void CCostumeSkinSelectDialog::StartSkinDelayTime()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}

BOOL CCostumeSkinSelectDialog::CheckDelay()
{
	//딜레이 처리
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwTime = m_dwSkinDelayTime - gCurTime;
		if(gCurTime > m_dwSkinDelayTime)
		{
			dwTime = 0;
		}		
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1355), dwTime/1000+1);

		return FALSE;
	}

	return TRUE;
}
*/
//스킨의 부위별로 각 해당 테이블에 등록한다.
void CCostumeSkinSelectDialog::CostumeSkinKindData()
{
	DWORD dwTotalNum = GAMERESRCMNGR->GetCostumeSkinListCountNum();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{
		SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetCostumeSkinList(i+1);
		if(pSkinInfo == NULL)
			continue;

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pSkinInfo->wEquipItem[0]);
		if(pInfo == NULL)
			continue;

		WORD wType = pInfo->Part3DType;
		
		//0 : 모자 , 2 : 옷, 1 : 나머지 악세서리로...
		//테이블에 등록할때 리스트다이얼로그의 인덱스로 아이탬 정보를 꺼내와야 하므로 i값을 키값으로 쓴다.
		if(wType == 0)	//모자
			m_CostumeSkinHat.Add(pSkinInfo, i);
		else if(wType == 2)	//옷
			m_CostumeSkinDress.Add(pSkinInfo, i);
		else			//악세사리
			m_CostumeSkinAccessory.Add(pSkinInfo, i);
	}
}

//현재 탭버튼의 종류에 해당하는 테이블에서 해당 인덱스의 스킨 정보를 얻어온다.
SKIN_SELECT_ITEM_INFO* CCostumeSkinSelectDialog::GetCurrentSkinInfo(DWORD dwSelectIdx)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;

	DWORD dwTotalNum = m_pCurrentSkinTable->GetDataNum();
	m_pCurrentSkinTable->SetPositionHead();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{		
		pSkinInfo = m_pCurrentSkinTable->GetData();

		if(pSkinInfo == NULL)
			continue;

		//스킨 리스트는 1부터 시작하므로 i+1을 해준다
		if(i+1 == dwSelectIdx)
			return pSkinInfo;

		pSkinInfo = NULL;		
	}

	return pSkinInfo;
}

//탭버튼의 포커스 지정
void CCostumeSkinSelectDialog::SetCostumTabBtnFocus(TAB_BTN eCostumeKind)
{
	for(int i = 0 ; i < eTabBtn_Max ; i++)
	{
		BOOL bFlag = FALSE;

		if(i == eCostumeKind)
			bFlag = TRUE;
		else
			bFlag = FALSE;

		m_pCostumTabBtn[i]->SetPush(bFlag);

	}
}