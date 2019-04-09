// PyogukSlot.cpp: implementation of the CPyogukSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PyogukSlot.h"
#include "Purse.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPyogukSlot::CPyogukSlot()
{
	m_ExtraSlotCount = 0;
	m_pyogukNum = 0;
}

CPyogukSlot::~CPyogukSlot()
{

}
/*
void CPyogukSlot::SetPyogukMoney(MONEYTYPE m)
{
	m_pPurse->Addition(m);
}
MONEYTYPE	CPyogukSlot::GetPyogukMoney()
{
	return m_pPurse->GetPurseCurMoney();
}
*/

#ifdef _JAPAN_LOCAL_

void CPyogukSlot::SetExtraSlotCount( DWORD Count )
{
	m_ExtraSlotCount = Count;
	//m_SlotNum = TABCELL_PYOGUK_NUM*(GIVEN_PYOGUK_SLOT+m_ExtraSlotCount);
}

#endif

#ifdef _HK_LOCAL_

void CPyogukSlot::SetExtraSlotCount( DWORD Count )
{
	m_ExtraSlotCount = Count;
	//m_SlotNum = TABCELL_PYOGUK_NUM*(GIVEN_PYOGUK_SLOT+m_ExtraSlotCount);
}

#endif

#ifdef _TL_LOCAL_

void CPyogukSlot::SetExtraSlotCount( DWORD Count )
{
	m_ExtraSlotCount = Count;
	m_SlotNum = TABCELL_PYOGUK_NUM*(GIVEN_PYOGUK_SLOT+m_ExtraSlotCount);
}

#endif
