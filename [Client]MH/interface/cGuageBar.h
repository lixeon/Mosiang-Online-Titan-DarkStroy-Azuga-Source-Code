// cGuageBar.h: interface for the cGuageBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGUAGEBAR_H__C557D4E0_B717_4B6A_99B8_7115562D8D3D__INCLUDED_)
#define AFX_CGUAGEBAR_H__C557D4E0_B717_4B6A_99B8_7115562D8D3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"
class cButton;

class cGuageBar : public cWindow  
{
public:

	cGuageBar();
	virtual ~cGuageBar();
	virtual void InitGuageBar(LONG interval, BOOL vertical);
	virtual void Add(cWindow * btn);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	virtual void SetAbsXY(LONG x, LONG y);
	virtual void SetAlpha(BYTE al);

	//-------custom-------------------------------------------------------------------
	void SetInterval(LONG val){ m_interval = val; repositioning(); }	
	LONG GetInterval(){ return m_interval; }
	void InitValue(LONG minv, LONG maxv, LONG curv);	// ÃÊ±â¿¡ SetxxxValue()¸¦ È£ÃâÇÏ¸é ¾ÈµÈ´Ù!!!!
	void SetMinValue(LONG minv);
	void SetMaxValue(LONG maxv);
	LONG GetMaxValue() { return m_maxValue; }
	LONG GetCurValue();
	void SetCurValue(LONG val);
	float GetCurRate(){ return m_barRelPos/(float)m_interval; }
	void SetCurRate( float fRate )	{	m_barRelPos = (float)m_interval * fRate;	}
	
	BOOL IsDrag() { return m_fBarDrag; }

	//SW050823
	void SetGuageLock(BOOL bDo, DWORD BtnColor);
	LONG GetMinValue() {return m_minValue;}
		
private:

	void repositioning();
	cButton * m_pbarBtn;			// ¼öÁ÷(top), ¼öÆò(left) ±âÁØ!

	// guage logical property
	LONG	m_minValue;
	LONG	m_maxValue;
	LONG	m_curValue;
	// guage physical property

	LONG	m_startPos;
	LONG	m_interval;				// ½ÇÁ¦ÀûÀÎ °ÔÀÌÁö °£°Ý BarBtnÀÌ AddµÇ¾úÀ» ¶§ ´Ù½Ã °è»ê m_interval -= (btn->GetWidth() or btn->GetHeight())
	//LONG	m_barRelPos;
	float	m_barRelPos;
	BOOL	m_fBarDrag;
	BOOL	m_fVertical;			// ¼öÁ÷ ( default : ¼öÆò °ÔÀÌÁö ¹Ù)

	float	m_fDragRelX;
	float	m_fDragRelY;

	BOOL	m_bLock;
	
};

#endif // !defined(AFX_CGUAGEBAR_H__C557D4E0_B717_4B6A_99B8_7115562D8D3D__INCLUDED_)
