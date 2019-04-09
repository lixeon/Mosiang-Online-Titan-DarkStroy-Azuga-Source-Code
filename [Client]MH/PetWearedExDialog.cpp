#include "stdafx.h"
#include ".\petwearedexdialog.h"
#include "ItemManager.h"

CPetWearedExDialog::CPetWearedExDialog(void)
{
}

CPetWearedExDialog::~CPetWearedExDialog(void)
{
}

BOOL CPetWearedExDialog::AddItem(WORD relPos, cIcon * InIcon)
{
	CItem * item = (CItem * )InIcon;
	if( AddIcon(relPos, InIcon) )
	{
		//!!!아이템 옵션 적용
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CPetWearedExDialog::DeleteItem(WORD relPos, cIcon ** outIcon)
{
	if( DeleteIcon(relPos, outIcon) )
	{
		//!!!아이템 옵션 해제
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CPetWearedExDialog::GetBlankPositionRestrictRef(WORD & absPos)
{
	for( int i = 0; i < SLOT_PETWEAR_NUM; ++i )
	{
		if(IsAddable(i))
		{
			absPos = TP_PETWEAR_START+i;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CPetWearedExDialog::CheckDuplication( DWORD ItemIdx )
{
	for( int i = 0; i < SLOT_PETWEAR_NUM; ++i )
	{
		CItem* pItem = (CItem*)(GetIconForIdx( i ));
		if(pItem)
		{
			if(pItem->GetItemIdx() == ItemIdx)
				return TRUE;
		}
	}
	return FALSE;
}

