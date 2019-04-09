// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameIn.h"
#include "SkinSelectDialog.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"

CSkinSelectDialog::CSkinSelectDialog()
{
	m_pNomalSkinListDlg = NULL;
	m_pNomalSkinIconDlg = NULL;

	m_dwSelectIdx = 0;	
	//InitSkinDelayTime();
}

CSkinSelectDialog::~CSkinSelectDialog()
{
	m_pNomalSkinListDlg->RemoveAll();

	m_pNomalSkinListDlg = NULL;
	m_pNomalSkinIconDlg = NULL;
}

void CSkinSelectDialog::Linking()
{
	m_pNomalSkinListDlg = (cListDialog*)GetWindowForID(SKIN_SELECTLIST);
	m_pNomalSkinListDlg->SetShowSelect(TRUE);

	m_pNomalSkinIconDlg = (cIconDialog*)GetWindowForID(SKIN_SELECT_ITEMVIEW);		

	m_NomalSkinView[0].SetMovable(FALSE);
	m_NomalSkinView[1].SetMovable(FALSE);
	m_NomalSkinView[2].SetMovable(FALSE);
}

void CSkinSelectDialog::SetActive( BOOL val )
{	
	cDialog::SetActive( val );
	if( val == FALSE )	
	{
		m_pNomalSkinListDlg->RemoveAll();
		m_pNomalSkinListDlg->SetCurSelectedRowIdx(-1);		
		m_pNomalSkinIconDlg->DeleteIconAll();
		m_dwSelectIdx = 0;
	}
	else
		SkinItemListInfo();
}

BOOL CSkinSelectDialog::OnActionEvent(LONG lId, void * p, DWORD we)
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
	case SKIN_SELECT_OK:	//확인 버튼
		{
			if(m_dwSelectIdx > 0)
			{
				SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GAMERESRCMNGR->GetNomalClothesSkinList(m_dwSelectIdx);
				if(NULL == pSkinItemInfo)
					break;

				//레벨 체크
				if(HERO->GetLevel() < pSkinItemInfo->dwLimitLevel)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(238), pSkinItemInfo->dwLimitLevel);
					break;
				}

				//딜레이 체크
				if( FALSE == HERO->CheckSkinDelay() )
					break;

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = m_dwSelectIdx;
				msg.dwData2 = eSHOP_ITEM_NOMALCLOTHES_SKIN;

				NETWORK->Send(&msg, sizeof(MSG_DWORD2));
			}			
		}
		break;
	case SKIN_SELECT_CANCEL:	//취소 버튼
		{
			SetActive( FALSE );
		}
		break;
	case SKIN_SELECT_RECOVERY:	//원래대로 되돌리기 버튼
		{
			//딜레이 체크
			if( FALSE == HERO->CheckSkinDelay() )
				break;		

			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
			msg.dwObjectID = HEROID;
			msg.dwData1 = 0;	//0번으로 셋팅하면 원래대로 되돌리기
			msg.dwData2 = eSHOP_ITEM_NOMALCLOTHES_SKIN;

			NETWORK->Send(&msg, sizeof(MSG_DWORD2));
		}
		break;
	}

	return TRUE;
}

DWORD CSkinSelectDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pNomalSkinListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pNomalSkinListDlg->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx + 1; //스킨 인덱스는 1부터 시작하기 떄문에 +1을 해준다

				// 해당 인덱스의 아이콘 이미지를 설정
				if(m_dwSelectIdx > 0)
				{
					SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GAMERESRCMNGR->GetNomalClothesSkinList(m_dwSelectIdx);
					if(pSkinItemInfo != NULL)
					{
						m_pNomalSkinIconDlg->DeleteIconAll();

						for(int i = 0 ; i < SKINITEM_LIST_MAX ; i++)
						{
							m_NomalSkinView[i].SetData(pSkinItemInfo->wEquipItem[i]);
							m_NomalSkinView[i].Init(pSkinItemInfo->wEquipItem[i], NULL);
							
							m_pNomalSkinIconDlg->AddIcon(i, (cIcon*)&m_NomalSkinView[i]);
						}
					}					
				}
			}
		}
	}	

	return we;
}

void CSkinSelectDialog::SkinItemListInfo()
{
	DWORD dwTotalNum = GAMERESRCMNGR->GetNomalClothesSkinListCountNum();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{
		DWORD dwColor;
		char buf[64] = { 0, };
		char szSkinItemName[MAX_NAME_LENGTH+1];
		
		int nLen = 0;

		SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(i+1);	//스킨 인덱스는 1부터 시작하기 때문에 +1 해준다.
		if(pSkinInfo == NULL)
			continue;

		//레젤제한에 따라 문자 색 변경 ( 착용가능 : 흰색, 착용불가능 : 빨간색 )
		if(HERO->GetLevel() < pSkinInfo->dwLimitLevel)
			dwColor = RGBA_MAKE(255,50,50,255);
		else
			dwColor = RGBA_MAKE(255,255,255,255);			

		//스킨 세트 이름 뒤에 공백을 넣어서 다음 문자와 정렬을 한다.
		SafeStrCpy(szSkinItemName, pSkinInfo->szSkinName, MAX_NAME_LENGTH+1);
		nLen = strlen(pSkinInfo->szSkinName);
		for( int i = nLen; i < MAX_NAME_LENGTH; i++ )
		{
			szSkinItemName[i] = 0x20;
		}

		sprintf(buf, "%s %15d", szSkinItemName, pSkinInfo->dwLimitLevel);
		m_pNomalSkinListDlg->AddItem(buf, dwColor);
	}	
}
/*
void CSkinSelectDialog::Render()
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
void CSkinSelectDialog::InitSkinDelayTime()
{
	m_dwSkinDelayTime = 0;	//딜레이 총 시간 변수
	m_bSkinDelayResult = FALSE;	//딜레이중인지 체크하는 변수
}

void CSkinSelectDialog::StartSkinDelayTime()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}

BOOL CSkinSelectDialog::CheckDelay()
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
