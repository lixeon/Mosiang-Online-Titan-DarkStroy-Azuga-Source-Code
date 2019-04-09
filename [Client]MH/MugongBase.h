// MugongBase.h: interface for the CMugongBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGBASE_H__BE364663_8C09_40C0_B3FD_382650BE2A66__INCLUDED_)
#define AFX_MUGONGBASE_H__BE364663_8C09_40C0_B3FD_382650BE2A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./BaseItem.h"
#include "interface/cGuagen.h"

class CSkillInfo;

class CMugongBase  : public CBaseItem
{
protected:
	cGuagen m_Guage;
	MUGONGBASE m_MugongBaseInfo;
	cImage	m_pHighLayerImage;

	DWORD	m_dwFontColor;

	//////////////////////////////////////////////////////////////////////////
	// 딜레이 표시를 위한 변수들
	cImage  m_DelayImage;
	BOOL	m_bRemainTime;
	DWORD	m_dwLastSecond;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	cImage	m_OptionImage;
	//////////////////////////////////////////////////////////////////////////
	
public:
	CMugongBase();
	virtual ~CMugongBase();

	CSkillInfo* m_pSkillInfo;

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * lowImage, cImage * highImage, LONG ID=0);
	virtual void Render();
	
	virtual inline DWORD	GetLastSecond()					{	return m_dwLastSecond; }
	virtual inline DWORD	GetDBIdx()					{	return m_MugongBaseInfo.dwDBIdx;	}
	virtual inline WORD		GetItemIdx()				{	return m_MugongBaseInfo.wIconIdx;	}
	virtual inline POSTYPE	GetPosition()				{	return m_MugongBaseInfo.Position;	}
	virtual inline void		SetPosition(POSTYPE pos)	{	m_MugongBaseInfo.Position = pos;	}

	virtual inline POSTYPE	GetQuickPosition()				{	return m_MugongBaseInfo.QuickPosition;	}
	virtual inline void		SetQuickPosition(POSTYPE pos)	{	m_MugongBaseInfo.QuickPosition = pos;		}

	virtual inline void		SetExpPoint(DWORD point);
	virtual inline DWORD	GetExpPoint()					{	return m_MugongBaseInfo.ExpPoint;	}
	virtual inline void		SetSung(BYTE sung)				{	m_MugongBaseInfo.Sung = sung;		}
	virtual inline BYTE		GetSung()						{	return m_MugongBaseInfo.Sung;		}
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	virtual inline WORD		GetOption()						{	return m_MugongBaseInfo.OptionIndex;	}
	//////////////////////////////////////////////////////////////////////////
	
	void SetMugongBase(MUGONGBASE * mugong);
	MUGONGBASE * GetMugongBase() { return &m_MugongBaseInfo; }

	void SetFontColor( DWORD dwFontColor ) { m_dwFontColor = dwFontColor; }
};

#endif // !defined(AFX_MUGONGBASE_H__BE364663_8C09_40C0_B3FD_382650BE2A66__INCLUDED_)
