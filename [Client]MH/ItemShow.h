// ItemShow.h: interface for the CItemShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_)
#define AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseItem.h"

class CItemShow  : public CBaseItem
{
protected:
	WORD m_wItemIdx;
	DURTYPE m_Dur;
public:
	CItemShow();
	virtual ~CItemShow();
	void Init(WORD wItemIdx, DURTYPE dur);
	virtual inline WORD		GetItemIdx(){ return m_wItemIdx; }
	virtual void Render();
};

#endif // !defined(AFX_ITEMSHOW_H__9E79636A_8195_4C5B_9F7E_4E8F2A1A1DA0__INCLUDED_)
