// PyogukSlot.h: interface for the CPyogukSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PYOGUKSLOT_H__E1C39D74_F581_42E9_9C2B_74E61B2C8ABF__INCLUDED_)
#define AFX_PYOGUKSLOT_H__E1C39D74_F581_42E9_9C2B_74E61B2C8ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemSlot.h"

class CPyogukSlot : public CItemSlot  
{
	DWORD		m_ExtraSlotCount;

public:
	CPyogukSlot();
	virtual ~CPyogukSlot();

	BYTE		GetPyogukNum()		{ return m_pyogukNum;	}
	void		SetPyogukNum(BYTE num)		{ m_pyogukNum = num;	}
//	void		SetPyogukMoney(MONEYTYPE m);
//	MONEYTYPE	GetPyogukMoney();

#ifdef _JAPAN_LOCAL_	
	void SetExtraSlotCount( DWORD Count );
#endif

#ifdef _HK_LOCAL_	
	void SetExtraSlotCount( DWORD Count );
#endif

#ifdef _TL_LOCAL_	
	void SetExtraSlotCount( DWORD Count );
#endif

protected:

	BYTE		m_pyogukNum; // Ç¥±¹Ã¢¿¡ °³¼ö ÇÏ³ª·Î ÅëÇÕ
};

#endif // !defined(AFX_PYOGUKSLOT_H__E1C39D74_F581_42E9_9C2B_74E61B2C8ABF__INCLUDED_)
