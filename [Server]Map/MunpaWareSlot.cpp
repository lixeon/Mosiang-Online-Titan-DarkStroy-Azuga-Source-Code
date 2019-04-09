// MunpaWareSlot.cpp: implementation of the CMunpaWareSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MunpaWareSlot.h"
#include "ItemManager.h"
#include "GuildManager.h"
#include "Player.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ClearItemBase(a)		a.dwDBIdx = 0;	\
								a.wIconIdx = 0;	\
								a.Position = 0;	\
								a.QuickPosition = 0;	\
								a.Durability = 0;

#define ClearSlotInfo(b)		b.bLock = 0;	\
								b.wPassword = 0;	\
								b.wState = 0;

CMunpaWareSlot::CMunpaWareSlot()
{

}

CMunpaWareSlot::~CMunpaWareSlot()
{

}

void CMunpaWareSlot::Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo)
{
	CItemSlot::Init(startAbsPos, slotNum, pInfo, pSlotInfo);

}
// ÁÖÀÇ!!!!
// ¹®ÆÄ´Â »ó´ë Æ÷Áö¼ÇÀ» »ç¿ë!!!!!
// absPos - m_StartAbsPos
ERROR_ITEM CMunpaWareSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	if( !IsPosIn(absPos) )		return EI_OUTOFPOS;
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
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos - m_StartAbsPos].wIconIdx) && m_ItemBaseArray[absPos - m_StartAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////

	if(pPlayer)
	{
		if(ITEMMGR->IsOptionItem(pItem->wIconIdx, pItem->Durability))
			GUILDMGR->AddGuildItemOption(pPlayer->GetItemOption(pItem->Durability));
		//SW050920 Rare
		if(ITEMMGR->IsRareOptionItem(pItem->wIconIdx, pItem->RareIdx))
			GUILDMGR->AddGuildItemRareOption(pPlayer->GetItemRareOption(pItem->RareIdx));
	}

	m_ItemBaseArray[absPos - m_StartAbsPos] = *pItem;
	m_SlotInfoArray[absPos - m_StartAbsPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[absPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}
ERROR_ITEM CMunpaWareSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag, WORD state, DWORD RareDBIdx)
{
	if( !IsPosIn(whatAbsPos) )	return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		//YH
		//return EI_NOTEQUALDATA;
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )  return EI_LOCKED;
	if( IsPassword(whatAbsPos) ) return EI_PASSWD;

	if(flag & UB_DBIDX)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].dwDBIdx = dwDBIdx;
	if(flag & UB_ICONIDX)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Position = position;

	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[whatAbsPos - m_StartAbsPos].wIconIdx) && m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;
	//////////////////////////////////////////////////////////////////////////
	

	if(flag & UB_DURA)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].Durability = Dur;

	//SW050920 Rare
	if(flag & UB_RARE)
	m_ItemBaseArray[whatAbsPos - m_StartAbsPos].RareIdx = RareDBIdx;
	
	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[whatAbsPos - m_StartAbsPos].bLock = FALSE;

	return EI_TRUE;
}
ERROR_ITEM CMunpaWareSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	if( IsEmptyInner(absPos) )	return EI_NOTEXIST;
	if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) ) return EI_PASSWD;

	if(pItemOut)
		*pItemOut = m_ItemBaseArray[absPos - m_StartAbsPos];

	ClearItemBase(m_ItemBaseArray[absPos - m_StartAbsPos]);
	ClearSlotInfo(m_SlotInfoArray[absPos - m_StartAbsPos]);

	return EI_TRUE;
}


BYTE CMunpaWareSlot::GetTabNum(POSTYPE absPos)
{
	return (absPos - m_StartAbsPos) / TABCELL_GUILDWAREHOUSE_NUM;
}

void CMunpaWareSlot::GetItemInfoTab(BYTE TabNum, ITEMBASE* pRtItemArray)
{
	int StartPos = TABCELL_GUILDWAREHOUSE_NUM * TabNum;
	for(int n=0;n<TABCELL_GUILDWAREHOUSE_NUM;++n)
	{
		pRtItemArray[n] = m_ItemBaseArray[StartPos+n];
	}
}

BOOL CMunpaWareSlot::IsEmptyInner(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );

	if( m_SlotInfoArray[absPos - m_StartAbsPos].wState != SS_NONE )
		return TRUE;
	else if( m_ItemBaseArray[absPos - m_StartAbsPos].dwDBIdx == 0 ) 
		return TRUE;
	else
		return FALSE;
}
BOOL CMunpaWareSlot::IsEmpty(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( !m_SlotInfoArray[absPos - m_StartAbsPos].bLock && m_SlotInfoArray[absPos - m_StartAbsPos].wState == SS_NONE && m_ItemBaseArray[absPos - m_StartAbsPos].dwDBIdx == 0)
		return TRUE;
	else
		return FALSE;
}
BOOL CMunpaWareSlot::IsLock(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( m_SlotInfoArray[absPos - m_StartAbsPos].bLock )
		return TRUE;
	else
		return FALSE;
}
BOOL CMunpaWareSlot::IsPassword(POSTYPE absPos)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	if( m_SlotInfoArray[absPos - m_StartAbsPos].wPassword ) 
		return TRUE;
	else
		return FALSE;
}
void CMunpaWareSlot::SetLock(POSTYPE absPos, BOOL val)
{
	ASSERT( m_StartAbsPos <= absPos && absPos < m_StartAbsPos + m_SlotNum );
	m_SlotInfoArray[absPos - m_StartAbsPos].bLock = val;
}
