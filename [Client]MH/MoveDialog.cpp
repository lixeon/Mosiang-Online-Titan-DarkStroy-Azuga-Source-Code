// MoveDialog.cpp: implementation of the CMoveDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveDialog.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "./Input/UserInput.h"
#include "./Interface/cButton.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrl.h"
#include "cRItemEx.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "./interface/cWindowManager.h"

#ifdef _KOR_LOCAL_
#include "ChannelDialog.h"
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveDialog::CMoveDialog()
{
	m_type = WT_MOVEDIALOG;
	m_bTownMove = FALSE;
	m_bMovePos = FALSE;
	m_SelectRowIdx = -1;
	m_SelectedTab = 0;
	m_bExtend = FALSE;
}

CMoveDialog::~CMoveDialog()
{
	PTRLISTSEARCHSTART(m_pTownList, MOVEDATA*, pData);
		m_pTownList.Remove(pData);
		delete pData;
		pData = NULL;
	PTRLISTSEARCHEND
	m_pTownList.RemoveAll();

	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pData);
		m_pDataList.Remove(pData);
		delete pData;
		pData = NULL;
	PTRLISTSEARCHEND
	m_pDataList.RemoveAll();
}


void CMoveDialog::Linking()
{
	m_pMoveLCtrl = (cListCtrl*)GetWindowForID(SA_MOVELIST);
	m_pNameBtn = (cButton*)GetWindowForID(SA_CHANGNAME);
	m_pMoveBtn = (cButton*)GetWindowForID(SA_MOVEOK);
	m_pDelBtn = (cButton*)GetWindowForID(SA_MOVEDEL);
	m_pPageBtn[0] = (cPushupButton*)GetWindowForID(SA_MOVETABBTN1);
	m_pPageBtn[1] = (cPushupButton*)GetWindowForID(SA_MOVETABBTN2);
	if( m_pPageBtn[0] )
		m_pPageBtn[0]->SetActive( FALSE );
	if( m_pPageBtn[1] )
		m_pPageBtn[1]->SetActive( FALSE );

	LoadTownMovePoint();
}


void CMoveDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );
	
	if( val )
	{
		if( m_bTownMove )
		{
			m_pNameBtn->SetActive( FALSE );
			m_pDelBtn->SetActive( FALSE );
		}
		else
		{
			m_pNameBtn->SetActive( TRUE );
			m_pDelBtn->SetActive( TRUE );
		}

		m_SelectedTab = 0;
		RefreshMoveInfo();

		if( m_bExtend )
		{
			if( m_pPageBtn[0] )
				m_pPageBtn[0]->SetPush( TRUE );
			if( m_pPageBtn[1] )
				m_pPageBtn[1]->SetPush( FALSE );
		}
		else
		{
			if( m_pPageBtn[0] )
				m_pPageBtn[0]->SetActive( FALSE );
			if( m_pPageBtn[1] )
				m_pPageBtn[1]->SetActive( FALSE );			
		}
	}
	else
	{
		m_ItemIdx = 0;
		m_ItemPos = 0;
		//ResetMoveIdx();

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);		

		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
}


DWORD CMoveDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	int rowidx = m_pMoveLCtrl->GetSelectedRowIdx();
	if(we & WE_ROWCLICK)
	{
		SelectMoveIdx(rowidx);
	}
	else if( we & WE_ROWDBLCLICK )
	{
		//OnMove();
	}
	return we;
}



void CMoveDialog::RefreshMoveInfo()
{
	int Startnum = 0;
	int Endnum = MAX_MOVEDATA_PERPAGE;
	
	m_pMoveLCtrl->DeleteAllItems();
	cPtrList* pList;
	if( m_bTownMove )
		pList = &m_pTownList;
	else
	{
		pList = &m_pDataList;
		Startnum = m_SelectedTab*MAX_MOVEDATA_PERPAGE;
		Endnum = Startnum + MAX_MOVEDATA_PERPAGE;
	}
	

	PTRLISTPOS pos = pList->GetHeadPosition();
	MOVEDATA* pData = NULL;

	int count = 0;
	for(int i=0; i<pList->GetCount(); i++)
	{		
		pData = (MOVEDATA*)pList->GetNext(pos);
		if(!pData)		break;

		++count;
		if( count <= Startnum )			continue;

		cRITEMEx* pItem = new cRITEMEx(2);		
		sprintf(pItem->pString[0], "%d.", i+1);
		strcpy(pItem->pString[1], pData->Name);

		m_pMoveLCtrl->InsertItem(i, pItem);
	}
}



void CMoveDialog::SetMoveInfo(SEND_MOVEDATA_INFO* pData)
{
	MOVEDATA* pListData = NULL;

	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pListData);
		m_pDataList.Remove(pListData);
		delete pListData;
		pListData = NULL;
	PTRLISTSEARCHEND
	m_pDataList.RemoveAll();
	m_pMoveLCtrl->DeleteAllItems();

	//	
	for(int i=0; i<pData->Count; i++)
	{
		MOVEDATA* pNewData = new MOVEDATA;
		memcpy(pNewData, &pData->Data[i], sizeof(MOVEDATA));
		m_pDataList.AddTail( pNewData );
	}
}


void CMoveDialog::AddMoveInfo(MOVEDATA* pData, BOOL bTown)
{
	MOVEDATA* pNewData = new MOVEDATA;
	memcpy(pNewData, pData, sizeof(MOVEDATA));

	if( bTown )
		m_pTownList.AddTail(pNewData);
	else
		m_pDataList.AddTail(pNewData);


	RefreshMoveInfo();
}


void CMoveDialog::DelMoveInfoSync()
{
	if( m_bTownMove )			return;

	int Startnum = m_SelectedTab*MAX_MOVEDATA_PERPAGE;
	int Endnum = Startnum + MAX_MOVEDATA_PERPAGE;
	int count = 0;

	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pSavedData);
		++count;
		if( count <= Startnum )			continue;
		if( (count-Startnum) == m_SelectRowIdx+1 )
		{
			SEND_MOVEDATA_SIMPLE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_DEL_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.Data = *pSavedData;
			NETWORK->Send( &msg, sizeof(msg) );			
			break;
		}
	PTRLISTSEARCHEND
}



void CMoveDialog::UpdateMoveInfo(MOVEDATA* pData)
{
	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pSavedData);
		if( pData->DBIdx == pSavedData->DBIdx)
		{
			strncpy(pSavedData->Name, pData->Name, MAX_SAVEDMOVE_NAME-1);
			break;
		}
	PTRLISTSEARCHEND

	ResetMoveIdx();	
	RefreshMoveInfo();
}



void CMoveDialog::DelMoveInfo(MOVEDATA* pData)
{
	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pSavedData);
		if( pData->DBIdx == pSavedData->DBIdx)
		{
			m_pDataList.Remove(pSavedData);
			delete pSavedData;
			pSavedData = NULL;
			break;
		}
	PTRLISTSEARCHEND

	ResetMoveIdx();
	RefreshMoveInfo();
}



void CMoveDialog::SelectMoveIdx(int rowidx)
{	
	cRITEMEx * pRItem = (cRITEMEx *)m_pMoveLCtrl->GetRItem(rowidx);
	
	if(pRItem != NULL)
	{
		if(m_SelectRowIdx != rowidx)
		{
			pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
			pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
			
			pRItem = (cRITEMEx*)m_pMoveLCtrl->GetRItem(m_SelectRowIdx);
			if(pRItem)
			{
				pRItem->rgb[0] = RGBA_MAKE( 255, 255 ,255 ,255);
				pRItem->rgb[1] = RGBA_MAKE( 255, 255 ,255 ,255);
			}
			
			m_SelectRowIdx = rowidx;
		}
	}
}



void CMoveDialog::ResetMoveIdx()
{
	cRITEMEx * pRItem = NULL;

	for(int i=0; i<m_pMoveLCtrl->GetItemCount(); i++)
	{
		pRItem = (cRITEMEx *)m_pMoveLCtrl->GetRItem(i);
		if(!pRItem)		continue;

		pRItem->rgb[0] = RGBA_MAKE( 255, 255 ,255 ,255);
		pRItem->rgb[1] = RGBA_MAKE( 255, 255 ,255 ,255);
	}

	m_pMoveLCtrl->SetSelectedRowIdx( -1 );
	m_SelectRowIdx = -1;
}


DWORD CMoveDialog::GetSelectedDBIdx()
{
	int Startnum = 0;
	int Endnum = MAX_MOVEDATA_PERPAGE;

	cPtrList* pList;
	if( m_bTownMove )
		pList = &m_pTownList;
	else
	{
		pList = &m_pDataList;
		Startnum = m_SelectedTab*MAX_MOVEDATA_PERPAGE;
		Endnum = Startnum + MAX_MOVEDATA_PERPAGE;
	}

	PTRLISTPOS pos = pList->GetHeadPosition();
	MOVEDATA* pData = NULL;

	int count = 0;
	for(int i=0; i<pList->GetCount(); i++)
	{
		pData = (MOVEDATA*)pList->GetNext(pos);
		if(!pData)		break;

		++count;
		if( count <= Startnum )			continue;

		if((i-Startnum) == m_SelectRowIdx )
			return pData->DBIdx;

	}

	return 0;
}




void CMoveDialog::MapMoveOK()
{
	if( m_ItemIdx == 0 || m_ItemPos == 0 )			return;

	int Startnum = 0;
	int Endnum = MAX_MOVEDATA_PERPAGE;

	cPtrList* pList;
	if( m_bTownMove )
		pList = &m_pTownList;
	else
	{
		pList = &m_pDataList;
		Startnum = m_SelectedTab*MAX_MOVEDATA_PERPAGE;
		Endnum = Startnum + MAX_MOVEDATA_PERPAGE;
	}

	PTRLISTPOS pos = pList->GetHeadPosition();
	MOVEDATA* pData = NULL;

	int count = 0;
	for(int i=0; i<pList->GetCount(); i++)
	{
		pData = (MOVEDATA*)pList->GetNext(pos);
		if(!pData)		break;

		++count;
		if( count <= Startnum )		continue;

		if((i-Startnum) == m_SelectRowIdx)
		{
			// magi82(35) MapChange State
			// 맵이동 할때 어떤 상태의 맵이동인지 체크하게 수정
			//2008. 5. 9. CBH - 채널 변경 기능 추가 수정
#ifdef _KOR_LOCAL_
			CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
			if(pChannelDlg != NULL)
				pChannelDlg->SendMapChannelInfoSYN(pData->MapNum, eMapChange_Item);
			SetDisable(TRUE);//쉥굶눗왯쐐岺듐샌
#else
			g_UserInput.GetHeroMove()->MapChange( pData->MapNum, m_ItemIdx, m_ItemPos, eMapChange_Item );
			SetActive( FALSE );			
#endif
			break;
		}
	}
}



DWORD CMoveDialog::GetSavedPoint()
{
	int count = 0;

	if( m_bTownMove )
	{
		PTRLISTSEARCHSTART(m_pTownList, MOVEDATA*, pData);
			if( count == m_SelectRowIdx )
				return pData->Point.value;
			++count;
		PTRLISTSEARCHEND
	}
	else
	{
		int Startnum = m_SelectedTab*MAX_MOVEDATA_PERPAGE;
		int Endnum = Startnum + MAX_MOVEDATA_PERPAGE;

		PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pData);
			++count;
			if( count <= Startnum )			continue;
			if( (count-Startnum) == m_SelectRowIdx+1 )
				return pData->Point.value;
		PTRLISTSEARCHEND
	}

	return 0;
}


void CMoveDialog::LoadTownMovePoint()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/StaticNpc.bin","rb");
#else
	file.Init("Resource/StaticNpc.txt","rt");
#endif

	if(file.IsInited() == FALSE)
		return;

	DWORD MapNum;
	DWORD NpcKind;
	DWORD NpcUniqueIdx;
	DWORD PosX, PosZ;
	char Name[128] = { 0, };

	// 마을의 맵번호 -하드코딩
#ifdef _HK_LOCAL_
	DWORD Town[10] = { 2006, 2007, 2012, 2017, 2001, 0, };
#elif defined _TL_LOCAL_
	DWORD Town[10] = { 2006, 2007, 2012, 2017, 0, };
#else
	DWORD Town[10] = { 2006, 2007, 2012, 2017, 2001, 0, };
#endif

	while(1)
	{
		if(file.IsEOF())
			break;

		MapNum = file.GetDword();
		NpcKind = file.GetWord();
		file.GetString(Name);
		NpcUniqueIdx = file.GetWord();
		
		PosX = file.GetDword();		
		PosZ = file.GetDword();
		
		//방향.....
		float fDir = file.GetFloat();

		// 마을의 저장포인트
		if( NpcUniqueIdx > 2000 )
		{
			for(int i=0; i< 10; i++)
			{
				if( Town[i] == NpcUniqueIdx )
				{
					// Add Town
					MOVEDATA data;
					data.DBIdx = 0;
					data.MapNum = (WORD)(NpcUniqueIdx-2000);
					strcpy(data.Name, Name);
					data.Point.SetMovePoint( (WORD)(PosX+50), (WORD)(PosZ+50) );
					AddMoveInfo( &data, TRUE );
				}
			}
		}
	}
	
	file.Release();
}


BOOL CMoveDialog::CheckSameName( char* pName )
{
	if(!pName)		return TRUE;

	PTRLISTSEARCHSTART(m_pDataList, MOVEDATA*, pData);
		if( strcmp(pData->Name, pName) == 0 )
			return TRUE;
	PTRLISTSEARCHEND

	return FALSE;
}


void CMoveDialog::SetExtend( BOOL bExtend )
{
	m_bExtend = bExtend;
}


void CMoveDialog::SetButton( DWORD Idx )
{
	if( m_SelectedTab == Idx || Idx > MAX_MOVEPOINT_PAGE )		return;

	if( m_pPageBtn[m_SelectedTab] )
		m_pPageBtn[m_SelectedTab]->SetPush( FALSE );	
	if( m_pPageBtn[Idx] )
		m_pPageBtn[Idx]->SetPush( TRUE );
	m_SelectedTab = Idx;

	RefreshMoveInfo();
}


void CMoveDialog::SetTownMoveView( BOOL bTown )
{
	m_bTownMove = bTown;

	if( bTown )
	{
		m_SelectedTab = 0;
		if( m_pPageBtn[0] )
			m_pPageBtn[0]->SetActive( FALSE );
		if( m_pPageBtn[1] )
			m_pPageBtn[1]->SetActive( FALSE );
	}
	else if( m_bExtend )
	{
		m_SelectedTab = 0;
		if( m_pPageBtn[0] )
			m_pPageBtn[0]->SetActive( TRUE );
		if( m_pPageBtn[1] )
			m_pPageBtn[1]->SetActive( TRUE );
	}
	else
	{
		m_SelectedTab = 0;
	}
}

//2008. 5. 9. CBH - 채널 맵이동 기능 추가
#ifdef _KOR_LOCAL_
void CMoveDialog::MapChange(WORD wMoveMapNum)
{
	if( m_ItemIdx == 0 || m_ItemPos == 0 )			return;

	if(wMoveMapNum != 0)
	{
		g_UserInput.GetHeroMove()->MapChange( wMoveMapNum, m_ItemIdx, m_ItemPos, eMapChange_Item );
		SetActive( FALSE );
	}

	if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
}
#endif
