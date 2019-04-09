// KyungGongManager.cpp: implementation of the CKyungGongManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KyungGongManager.h"

#ifdef _MAPSERVER_
#include "UserTable.h"
#include "Player.h"
#include "IconManager.h"
#else
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CKyungGongManager)
CKyungGongManager::CKyungGongManager()
{
	m_InfoTable.Initialize(16);
}

CKyungGongManager::~CKyungGongManager()
{
	Release();
}

void CKyungGongManager::Init()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(file.Init("Resource/KyungGongInfo.bin","rb") == FALSE)
		return;
#else
	if(file.Init("Resource/KyungGongInfo.txt","rt") == FALSE)
		return;
#endif

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		CKyungGongInfo* pKGInfo = new CKyungGongInfo;
		pKGInfo->Init(&file);

		ASSERT(m_InfoTable.GetData(pKGInfo->GetKyungGongIdx()) == FALSE);
		m_InfoTable.Add(pKGInfo,pKGInfo->GetKyungGongIdx());
	}
}
void CKyungGongManager::Release()
{
	CKyungGongInfo* pKGInfo;
	m_InfoTable.SetPositionHead();
	while(pKGInfo = m_InfoTable.GetData())
	{
		delete pKGInfo;
	}
	m_InfoTable.RemoveAll();
}

CKyungGongInfo* CKyungGongManager::GetKyungGongInfo(WORD idx)
{
	CKyungGongInfo* p = m_InfoTable.GetData(idx);
	ASSERT(p);
	return p;
}

BOOL CKyungGongManager::IsSetableKyungGong(PLAYERTYPE* pHero,ITEMTYPE* pItem)
{
#ifdef _MAPSERVER_
	const ITEM_INFO* pInfo = pItem;
#else
	const ITEM_INFO* pInfo = pItem->GetItemInfo();
#endif
	
	if(pInfo->LimitLevel > pHero->GetLevel())
		return FALSE;

	// 근력.민첩,심맥,체력
	if(pInfo->LimitGenGol > pHero->GetGenGol())
		return FALSE;
	if(pInfo->LimitMinChub > pHero->GetMinChub())
		return FALSE;
	if(pInfo->LimitSimMek > pHero->GetSimMek())
		return FALSE;
	if(pInfo->LimitCheRyuk > pHero->GetCheRyuk())
		return FALSE;

	CKyungGongInfo* pKGInfo = GetKyungGongInfo(pInfo->MugongNum);
	if(pKGInfo == NULL)
		return FALSE;
	if(pKGInfo->GetKyungGongIdx() <= pHero->GetKyungGongGrade())		// 예전 경공으로는 돌아갈수 없다.
		return FALSE;
	
	return TRUE;
}

void CKyungGongManager::SetKyungGong(PLAYERTYPE* pHero,WORD KyungGongIdx)
{
	ASSERT(GetKyungGongInfo(KyungGongIdx));
	pHero->SetKyungGongGrade(KyungGongIdx);

#ifdef _MAPSERVER_
	MSG_WORD msg;
	msg.Category = MP_KYUNGGONG;
	msg.Protocol = MP_KYUNGGONG_CHANGE_NOTIFY;
	msg.wData = KyungGongIdx;
	pHero->SendMsg(&msg,sizeof(msg));	
#endif
}

void CKyungGongManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
#ifdef _MAPSERVER_
#else
	switch(Protocol)
	{
	case MP_KYUNGGONG_CHANGE_NOTIFY:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			HERO->SetKyungGongGrade(pmsg->wData);
		}
		break;
	case MP_KYUNGGONG_ABILITY_CHANGE_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject( pmsg->dwData1 );
			if( pObject )
				pObject->SetKyungGongLevel( (WORD)pmsg->dwData2 );
		}
		break;
	}
#endif
}