// QuickItem.h: interface for the CQuickItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKITEM_H__921201F4_97A9_47AC_A701_8D20E8668E01__INCLUDED_)
#define AFX_QUICKITEM_H__921201F4_97A9_47AC_A701_8D20E8668E01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./BaseItem.h"
class CItem;
class CMugongBase;
class CQuickItem : public CBaseItem  
{
	//---------------------------------------------------------------------------
	// private를 protected, public로 바꾸지 마라 : taiyo
	//---------------------------------------------------------------------------
	CQuickItem(CBaseItem * pItemLink);
	virtual ~CQuickItem();
	friend class CQuickManager;

	cImage m_HighLayerImage;
	cImage m_LinkImage;
	cImage m_DelayImage;
	CBaseItem * m_pItemLink;
	QUICKBASE	m_quickBase;
	DURTYPE		m_QDur;

	BOOL	m_bRemainTime;
	DWORD	m_dwLastSecond;
	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	cImage	m_OptionImage;
	//////////////////////////////////////////////////////////////////////////

public:

	void			SetImage2(cImage * low, cImage * high);
	CBaseItem *		GetItem(){ return m_pItemLink;	}

	virtual void	Render();
	void SetLinkItem(CBaseItem * pItem) { m_pItemLink = pItem;	}
	WORD GetSrcIdx()			{ return m_pItemLink->GetItemIdx();		}
	POSTYPE GetSrcPosition()	{ return m_pItemLink->GetPosition(); }
	POSTYPE GetPosition()		{	return m_quickBase.Position;	}
	DURTYPE GetSrcDurability()	{ return m_pItemLink->GetDurability(); }
	void SetDurability(DURTYPE dur) { m_QDur = dur; }
	DURTYPE GetDurability() { return m_QDur; }
	void SetPosition(POSTYPE pos) 
	{ 
		m_quickBase.Position = pos;
		// 06. 01 이영준 - 단축창 변경
		// 퀵아이템의 포지션값과 실제 아이템의 퀵포지션이 다르게 셋팅되어야함
		//m_pItemLink->SetQuickPosition(pos);
	}
	void SetMugongIconToolTip( DWORD dwRemainTime );
};

#endif // !defined(AFX_QUICKITEM_H__921201F4_97A9_47AC_A701_8D20E8668E01__INCLUDED_)
