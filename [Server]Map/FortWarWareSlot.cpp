#include "stdafx.h"
#include "FortWarWareSlot.h"
#include "ItemManager.h"


#define ClearItemBase(a)	a.dwDBIdx = 0;			\
							a.wIconIdx = 0;			\
							a.Position = 0;			\
                            a.QuickPosition = 0;	\
                            a.Durability = 0;

#define ClearSlotInfo(b)	b.bLock = 0;		\
							b.wPassword = 0;	\
							b.wState = 0;

CFortWarWareSlot::CFortWarWareSlot(void)
{
}

CFortWarWareSlot::~CFortWarWareSlot(void)
{
}

void CFortWarWareSlot::Initialize()
{
	memset( m_TotalItemBase, 0, sizeof(ITEMBASE)*SLOT_SEIGEFORTWARE_NUM );
	memset( m_TotalSlotInfo, 0, sizeof(SLOTINFO)*SLOT_SEIGEFORTWARE_NUM );

	CItemSlot::Init( TP_SIEGEFORTWARE_START, SLOT_SEIGEFORTWARE_NUM, m_TotalItemBase, m_TotalSlotInfo );
}

ERROR_ITEM CFortWarWareSlot::InsertItemAbs( CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state/*=SS_NONE*/ )
{
	if( !IsPosIn(absPos) )
		return EI_OUTOFPOS;
	if( pItem->Position != absPos )
        pItem->Position = absPos;
	if( !IsEmptyInner(absPos) )
		return EI_EXISTED;
	if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )
		return EI_LOCKED;
	if( IsPassword(absPos) )
		return EI_PASSWD;

	//itemmaxcheck
	if( (state & SS_PREINSERT) == FALSE )
    if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos - m_StartAbsPos].wIconIdx) && m_ItemBaseArray[absPos - m_StartAbsPos].Durability > 5 )
        return EI_NOTEQUALDATA;

	m_ItemBaseArray[absPos - m_StartAbsPos] = *pItem;
	m_SlotInfoArray[absPos - m_StartAbsPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[absPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CFortWarWareSlot::DeleteItemAbs( CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state/*=SS_NONE*/ )
{
	if( !IsPosIn(absPos) )
		return EI_OUTOFPOS;
	if( IsEmptyInner(absPos) )
		return EI_NOTEXIST;
	if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )
		return EI_LOCKED;
	if( IsPassword(absPos) )
		return EI_PASSWD;

	if( pItemOut )
		*pItemOut = m_ItemBaseArray[absPos - m_StartAbsPos];

	ClearItemBase(m_ItemBaseArray[absPos - m_StartAbsPos]);
	ClearSlotInfo(m_SlotInfoArray[absPos - m_StartAbsPos]);

	return EI_TRUE;
}

ERROR_ITEM CFortWarWareSlot::UpdateItemAbs( CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag/*=UB_ALL*/, WORD state/*=SS_NONE*/, DWORD RareDBIdx/* = 0*/ )
{
	if( !IsPosIn(whatAbsPos) )
		return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )
		return EI_LOCKED;
	if( IsPassword(whatAbsPos) )
		return EI_PASSWD;

	if(flag & UB_DBIDX)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].dwDBIdx = dwDBIdx;
	if(flag & UB_ICONIDX)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Position = position;
	if(flag & UB_DURA)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Durability = Dur;
	//SW050920 Rare
	if(flag & UB_RARE)
		m_ItemBaseArray[whatAbsPos - m_StartAbsPos].RareIdx = RareDBIdx;

	//itemmaxcheck
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[whatAbsPos - m_StartAbsPos].wIconIdx) && m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Durability > 5 )
        return EI_NOTEQUALDATA;

	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}

WORD CFortWarWareSlot::GetEmptySlotPos()
{
	WORD wEndPos = m_StartAbsPos + m_SlotNum;
    for( WORD i = m_StartAbsPos; i < wEndPos; ++i )
	{
        if( IsEmpty(i) )
		{
			return i;
		}
	}
	return 0;
}

void CFortWarWareSlot::GetItemInfoAll( MSG_FORTWAR_WAREHOUSE_INFO* pRtInfo )
{
	WORD wEndPos = m_StartAbsPos + m_SlotNum;
	for( WORD i = m_StartAbsPos; i < wEndPos; ++i )
	{
		if( IsEmpty(i) == FALSE )
		{
			pRtInfo->AddItem( &m_ItemBaseArray[i-m_StartAbsPos] );
		}
	}
}

//
void CFortWarWareSlot::SetLock( POSTYPE absPos, BOOL val )
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	m_SlotInfoArray[absPos - m_StartAbsPos].bLock = val;
}

BOOL CFortWarWareSlot::IsLock( POSTYPE absPos )
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( m_SlotInfoArray[absPos - m_StartAbsPos].bLock )
		return TRUE;
	else
		return FALSE;
}

BOOL CFortWarWareSlot::IsEmpty( POSTYPE absPos )
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( !m_SlotInfoArray[absPos - m_StartAbsPos].bLock && m_SlotInfoArray[absPos - m_StartAbsPos].wState == SS_NONE && m_ItemBaseArray[absPos - m_StartAbsPos].dwDBIdx == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL CFortWarWareSlot::IsEmptyInner(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );

	if( m_SlotInfoArray[absPos - m_StartAbsPos].wState != SS_NONE )
		return TRUE;
	else if( m_ItemBaseArray[absPos - m_StartAbsPos].dwDBIdx == 0 ) 
		return TRUE;
	else
		return FALSE;
}

BOOL CFortWarWareSlot::IsPassword(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( m_SlotInfoArray[absPos - m_StartAbsPos].wPassword ) 
		return TRUE;
	else
		return FALSE;
}

ITEMBASE* CFortWarWareSlot::GetSameItemForDupItem( DWORD dwItemIdx, WORD wItemNum )
{
	WORD wEndPos = m_StartAbsPos + m_SlotNum;
	for( WORD i = m_StartAbsPos; i < wEndPos; ++i )
	{
		if( m_ItemBaseArray[i].wIconIdx == dwItemIdx )
		if( m_ItemBaseArray[i].Durability + wItemNum <= MAX_YOUNGYAKITEM_DUPNUM )
			return &m_ItemBaseArray[i];
	}
	return NULL;
}

