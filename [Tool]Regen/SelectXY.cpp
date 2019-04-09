// SelectXY.cpp: implementation of the CSelectXY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegenTool.h"
#include "SelectXY.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectXY::CSelectXY()
{
	m_TargetRect = CRect(0,0,10,10);
	m_nLindTick = 5;
	m_pWnd = NULL;
}

CSelectXY::CSelectXY(CWnd *pWnd,int dot,int tick)
{
	m_pWnd=pWnd;
	
	m_TargetRect.SetRectEmpty();
	m_nLindTick = 5;
}
CSelectXY::~CSelectXY()
{

}

void CSelectXY::Draw(CDC *pDC, CPoint point)
{
	//ÃßÀû¹Ú½ºÀÇ ¸ÞÀÎ ¹Ú½º¸¦ ±×¸°´Ù.
/*	CPen pen,*oldPen;
	CBrush brush,*oldBrush;
	pen.CreatePen(PS_DOT,3,RGB(125,125,125));
	oldPen=pDC->SelectObject(&pen);
	
	CalcPoint(&m_TargetRect, point);
	if(m_TargetRect.left != 0 || m_TargetRect.right != 0 
		|| m_TargetRect.top != 0 || m_TargetRect.bottom != 0)
	{
		pDC->Ellipse(&m_TargetRect);
	}
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	brush.CreateSolidBrush(RGB(0,0,0));
	oldBrush=pDC->SelectObject(&brush);
	//8°³ ¹æÇâÀÇ ¹Ú½º¸¦ ±×¸°´Ù.
	pDC->SelectObject(oldBrush);
	brush.DeleteObject();
*/	
}
void CSelectXY::OnMouseMove(UINT nFlags, CPoint point)
{
	
}
void CSelectXY::OnLButtonDown(UINT nFlags, CPoint point)
{

}
void CSelectXY::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDC *pDC=m_pWnd->GetDC();
//	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
//	HDC hdc = ::GetDC(wnd->m_hWnd);
	CRect wrect, crect;
	POINT pos;
	m_pWnd->GetWindowRect(&wrect);
	m_pWnd->GetClientRect(&crect);
	pos.x = wrect.left;
	pos.y = wrect.top;

	
	CPen pen,*oldPen;
	pDC->SetROP2(R2_XORPEN);
	pen.CreatePen(PS_DOT,3,RGB(125,125,125));
	oldPen=pDC->SelectObject(&pen);
	
	
	CalcPoint(&m_TargetRect, point);
	if(m_TargetRect.left != 0 || m_TargetRect.right != 0 
		|| m_TargetRect.top != 0 || m_TargetRect.bottom != 0)
	{
		pDC->Ellipse(&m_TargetRect);
	}

	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	m_pWnd->ReleaseDC(pDC);
	m_rectOld=CRect(0,0,0,0);
	m_bDown=FALSE;
}

CRect CSelectXY::CalcPoint(CRect* rect, CPoint point)
{

//	HWND hWnd = GetDlgItem(m_pWnd->m_hWnd, IDC_MAPIMAGE);
//	HDC hdc = ::GetDC(m_pWnd->m_hWnd);
	CRect wrect, crect, prect;
//	POINT pos;
	m_pWnd->GetWindowRect(&wrect);
	m_pWnd->GetClientRect(&crect);
	CWnd* pWnd = m_pWnd->GetParent();
	pWnd->GetWindowRect(&prect);
	
	CRect ImageBox;
	ImageBox.left = wrect.left - prect.left;
	ImageBox.top = wrect.top - prect.top;
	ImageBox.SetRect(ImageBox.TopLeft(), wrect.BottomRight());
//	m_pWnd->GetClientRect(&ImageBox);//GetClientRect(m_hWnd, &ImageBox);

	if(point.x > ImageBox.left && point.x < ImageBox.right &&
		point.y > ImageBox.top && point.y < ImageBox.bottom)
	{
		if(point.x - m_nLindTick > ImageBox.left)
			rect->left = point.x - m_nLindTick;
		else
			rect->left = ImageBox.left;
		if(point.y - m_nLindTick > ImageBox.top)
			rect->top = point.y - m_nLindTick;
		else
			rect->top = ImageBox.top;
		if(point.x + m_nLindTick < ImageBox.right)
			rect->right = point.x + m_nLindTick;
		else
			rect->right = ImageBox.right;
		if(point.y + m_nLindTick < ImageBox.bottom)
			rect->bottom = point.y + m_nLindTick;
		else
			rect->bottom = ImageBox.bottom;
	}
	else
	{
		rect->SetRectEmpty();
	}
	return rect;
}