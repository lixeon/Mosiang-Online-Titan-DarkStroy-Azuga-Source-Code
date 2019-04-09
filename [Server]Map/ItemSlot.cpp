// ItemSlot.cpp: implementation of the CItemSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemSlot.h"
#include "Purse.h"
#include "ItemManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ClearItemBase(a)		a.dwDBIdx = 0;	\
								a.wIconIdx = 0;	\
								a.Position = 0;	\
								a.QuickPosition = 0;	\
								a.Durability = 0;	\
								a.RareIdx = 0;

#define ClearSlotInfo(b)		b.bLock = 0;	\
								b.wPassword = 0;	\
								b.wState = 0;
								
CItemSlot::CItemSlot()
{
	m_ItemBaseArray = NULL;
	m_SlotInfoArray = NULL;
	m_pPurse		= NULL;
}

CItemSlot::~CItemSlot()
{
	Release();
}

char * CItemSlot::_ToStringSlot()
{
	static char slotBuf[2512];
	slotBuf[0] = 0;
	char tmp[64];
	for( POSTYPE i = 0 ; i < m_SlotNum ; ++i )
	{
		sprintf(tmp, "DBIDX:%d ITEMIDX:%d POS:%d QPOS:%d DURA:%d LOCK:%d PWD:%d\n", 
							m_ItemBaseArray[i].dwDBIdx,
							m_ItemBaseArray[i].wIconIdx, 
							m_ItemBaseArray[i].Position, 
							m_ItemBaseArray[i].QuickPosition, 
							m_ItemBaseArray[i].Durability, 
							m_SlotInfoArray[i].bLock, 
							m_SlotInfoArray[i].wPassword);
		strcat(slotBuf, tmp);
		if( strlen(slotBuf) > 512 ) break;
	}

	return slotBuf;
}

void CItemSlot::Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo)
{
	m_StartAbsPos		= startAbsPos;
	m_SlotNum			= slotNum;
	m_ItemBaseArray		= pInfo;
	m_SlotInfoArray		= pSlotInfo;
}

BOOL CItemSlot::CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max)
{
	if(!m_pPurse)
	m_pPurse = pNewPurse;
	return m_pPurse->InitPurse( pOwner, money, max );
}

void CItemSlot::Release()
{
	m_pPurse		= NULL;
}

ITEMBASE const * CItemSlot::GetItemInfoAbs(POSTYPE absPos)
{ 
	if( !IsPosIn(absPos) )	return NULL;

	return  &m_ItemBaseArray[absPos];	
}
void CItemSlot::GetItemInfoAll(ITEMBASE * pItemBaseAll)
{
	memcpy(pItemBaseAll, &m_ItemBaseArray[m_StartAbsPos], sizeof(ITEMBASE)*m_SlotNum);
}
void CItemSlot::SetItemInfoAll(ITEMBASE * pItemBaseAll)
{
	memcpy(&m_ItemBaseArray[m_StartAbsPos], pItemBaseAll, sizeof(ITEMBASE)*m_SlotNum);
}

ERROR_ITEM CItemSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	if( pItem->Position != absPos )
	{
		//YH
		//return EI_NOTEQUALDATA;
		pItem->Position = absPos;
	}
	if( !IsEmptyInner(absPos) )	return EI_EXISTED;
	if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) ) return EI_PASSWD;

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if((state & SS_PREINSERT) == FALSE)	
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos].wIconIdx) && m_ItemBaseArray[absPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	m_ItemBaseArray[absPos] = *pItem;
	m_SlotInfoArray[absPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[absPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	if( !IsPosIn(whatAbsPos) )	return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		//YH
		//return EI_NOTEQUALDATA;
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )  return EI_LOCKED;
	if( SS_CHKDBIDX & state )
	{
		if( dwDBIdx != m_ItemBaseArray[whatAbsPos].dwDBIdx )
			return EI_NOTEQUALDATA;
	}
	if( IsPassword(whatAbsPos) ) return EI_PASSWD;

	//if(flag & UB_DBIDX)
	//m_ItemBaseArray[whatAbsPos].dwDBIdx = dwDBIdx;
	if(flag & UB_ICONIDX)
	m_ItemBaseArray[whatAbsPos].wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)
	m_ItemBaseArray[whatAbsPos].QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
	m_ItemBaseArray[whatAbsPos].Position = position;
	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[whatAbsPos].wIconIdx) && m_ItemBaseArray[whatAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	if(flag & UB_DURA)
	m_ItemBaseArray[whatAbsPos].Durability = Dur;
	
	if(flag & UB_RARE)
	m_ItemBaseArray[whatAbsPos].RareIdx = RareDBIdx;

	// magi82 - 노점상 한번이라도 낱개로 팔면 그다음부터 통째로 팔아지지않는 버그 수정
	m_SlotInfoArray[whatAbsPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	m_SlotInfoArray[whatAbsPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	if( IsEmptyInner(absPos) )	return EI_NOTEXIST;
	if( !(absPos >= TP_SHOPITEM_START && absPos < TP_SHOPITEM_END) &&
		!(absPos >= TP_SHOPINVEN_START && absPos < TP_SHOPINVEN_END) )
		if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) ) return EI_PASSWD;

	if(pItemOut)
		*pItemOut = m_ItemBaseArray[absPos];

	ClearItemBase(m_ItemBaseArray[absPos]);
	ClearSlotInfo(m_SlotInfoArray[absPos]);
	return EI_TRUE;
}

/* Position이 현재 Slot에 포함되는 Position인지 Check*/
BOOL CItemSlot::IsPosIn(POSTYPE absPos)
{
	if( m_StartAbsPos > absPos || absPos >= m_StartAbsPos + m_SlotNum )
		return FALSE;
	else
		return TRUE;
}
BOOL CItemSlot::IsEmptyInner(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 

	if( m_SlotInfoArray[absPos].wState != SS_NONE )
		return TRUE;
	else if( m_ItemBaseArray[absPos].dwDBIdx == 0 ) 
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsEmpty(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 

	if( !m_SlotInfoArray[absPos].bLock && m_SlotInfoArray[absPos].wState == SS_NONE && m_ItemBaseArray[absPos].dwDBIdx == 0)
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsLock(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp); 

	if( m_SlotInfoArray[absPos].bLock )
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsPassword(POSTYPE absPos)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp ); 

	if( m_SlotInfoArray[absPos].wPassword ) 
		return TRUE;
	else
		return FALSE;
}
void CItemSlot::SetLock(POSTYPE absPos, BOOL val)
{
//	char temp[1024];
//	sprintf(temp, "Start:%d, End:%d, Cur:%d", m_StartAbsPos, m_StartAbsPos+m_SlotNum, absPos);
//	ASSERTMSG( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum, temp); 

	m_SlotInfoArray[absPos].bLock = val;
}
/*
MONEYTYPE CItemSlot::Addition(MONEYTYPE money)
{
	if( !m_pPurse ) return 0;
	return m_pPurse->Addition(money);
}

MONEYTYPE CItemSlot::Subtraction(MONEYTYPE money)
{
	if( !m_pPurse ) return 0;
	return m_pPurse->Subtraction(money);
}
*/
MONEYTYPE CItemSlot::GetMoney()
{
	if( !m_pPurse ) return 0;
	return m_pPurse->GetPurseCurMoney();
}

WORD CItemSlot::GetItemCount()
{
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	WORD ItemNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		if( IsEmpty(i) ) continue;

		++ItemNum;
	}

	return ItemNum;	
}
