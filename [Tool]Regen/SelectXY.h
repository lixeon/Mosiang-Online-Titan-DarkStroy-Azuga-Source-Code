// SelectXY.h: interface for the CSelectXY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTXY_H__4B36A4FE_356C_401B_94E6_E595975E4333__INCLUDED_)
#define AFX_SELECTXY_H__4B36A4FE_356C_401B_94E6_E595975E4333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelectXY  
{
	CWnd *m_pWnd;//ºÎ¸ð À©µµ¿ì ÇÚµé·¯
	HWND m_hWnd;
	BOOL m_bDown;//¸¶¿ì½º ¹öÆ° ÇÃ·¯±× 
	CRect m_rectOld;//ÀÌÀü ¿µ¿ª ÁÂÇ¥ ÀúÀå º¯¼ö
	CRect m_TargetRect;//Default Regen Point
	int m_nLindTick;
	
public:
	CSelectXY();
	CSelectXY(CWnd *pWnd,int dot=2,int tick=3);
	virtual ~CSelectXY();

	CRect CalcPoint(CRect* rect, CPoint point);
	void Draw(CDC *pDC, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

#endif // !defined(AFX_SELECTXY_H__4B36A4FE_356C_401B_94E6_E595975E4333__INCLUDED_)
