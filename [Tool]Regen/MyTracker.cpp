#include "stdafx.h"
#include "RegenTool.h"
#include "MyTracker.h"

//»ý¼ºÀÚ
CMyTracker::CMyTracker()
{
	m_rect=CRect(0,0,0,0);
	m_bDown=FALSE;
	m_pWnd=NULL;
	m_nProcess=0;
	m_nLineDot=2;
	m_nLindTick=3;
	m_nChoNum=-1;
	m_bSet=FALSE;
	m_OldPoint=CPoint(0,0);
}
CMyTracker::CMyTracker(CWnd *pWnd,int dot,int tick)
{
	m_rect=CRect(0,0,0,0);
	m_pWnd=pWnd;
	m_bDown=FALSE;
	m_nProcess=0;
	m_nLineDot=dot;
	m_nLindTick=tick;
	m_nChoNum=-1;
	m_bSet=FALSE;
	m_OldPoint=CPoint(0,0);
}
//ÃßÀû¹Ú½º±×¸®±â
void CMyTracker::Draw(CDC *pDC)
{
	if(m_rect.left==0 && m_rect.top==0 && m_rect.bottom==0 && m_rect.right==0)
		return;
	//8°³ ¹æÇâÀÇ ¿µ¿ªÀ» ¼³Á¤ÇÑ´Ù.
	MakeDirect();
	//8°³ ¹æÇâÀÇ ¿µ¿ªÀÌ ¼³Á¤µÇ¸é m_bSetÀº TRUE·Î
	m_bSet=TRUE;
	//ÃßÀû¹Ú½ºÀÇ ¸ÞÀÎ ¹Ú½º¸¦ ±×¸°´Ù.
	CPen pen,*oldPen;
	CBrush brush,*oldBrush;
	pen.CreatePen(PS_DOT,m_nLindTick,RGB(125,125,125));
	oldPen=pDC->SelectObject(&pen);
	pDC->MoveTo(m_rect.left,m_rect.top);
	pDC->LineTo(m_rect.right,m_rect.top);
	pDC->LineTo(m_rect.right,m_rect.bottom);
	pDC->LineTo(m_rect.left,m_rect.bottom);
	pDC->LineTo(m_rect.left,m_rect.top);
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	brush.CreateSolidBrush(RGB(0,0,0));
	oldBrush=pDC->SelectObject(&brush);
	//8°³ ¹æÇâÀÇ ¹Ú½º¸¦ ±×¸°´Ù.
	for(int i=0;i<8;i++)
		pDC->Rectangle(m_rectDirect[i]);

	pDC->SelectObject(oldBrush);
	brush.DeleteObject();

}

//ÃßÀû¹Ú½ºÀÇ 8°³ ¹æÇâ ¿µ¿ª°ú ÀÌµ¿¿µ¿ª¿¡
//¸¶¿ì½º Æ÷ÀÎÅÍ°¡ ÀÖ´Â°¡¸¦ Ã£´Â ÇÔ¼ö
int CMyTracker::PtInRect(CPoint point)
{
	//12°³ÀÇ ¹æÇâÀ» m_rect¸¦ ÂüÁ¶ÇÏ¿© ¸¸µç´Ù.
	MakeDirect();
	//ÀÌ 12°³ÀÇ ¹æÇâÀ§¿¡ ÀÖÀ¸¸é ÇØ´ç ¹øÈ£¸¦
	//¸®ÅÏÇÑ´Ù.
	for(int i=0;i<12;i++)
		if(m_rectDirect[i].PtInRect(point))
		{
			return i;
		}
	//ÃßÀû ¹Ú½º ¿µ¿ªÀ§¿¡ ¾øÀ¸¸é -1¸¦ ¸®ÅÏ
	return -1;
}
//ÇØ´ç ¹æÇâ¿¡ µû¶ó ¸¶¿ì½º Ä¿¼­¸¦ ¼³Á¤ÇÑ´Ù.
void CMyTracker::SetMouseCursor()
{
	switch(m_nChoNum)
	{
		case 0:
		case 2:
				SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNWSE));
				break;
		case 1:
		case 3:
				SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNESW));
				break;
		case 4:
		case 5:
				SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKNS));
				break;
		case 6:
		case 7:
				SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACKWE));
				break;
		case 8:
		case 9:
		case 10:
		case 11:
				SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)AfxGetApp()->LoadCursor(AFX_IDC_TRACK4WAY));
				break;
		default:
			SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)LoadCursor(NULL,IDC_ARROW));
				break;
	}
}
//¸¶¿ì½º ÀÌµ¿½Ã
void CMyTracker::OnMouseMove(UINT nFlags, CPoint point)
{

	if(m_bDown && m_nProcess!=0)
	{
		m_rectTrack.right=point.x;
		m_rectTrack.bottom=point.y;
		CDC *pDC=m_pWnd->GetDC();
		CPen pen,*oldPen;
		pDC->SetROP2(R2_XORPEN);
		pen.CreatePen(PS_DOT,2,RGB(125,125,125));
		oldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(m_rectOld.left,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.top);

		pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
		pDC->LineTo(m_rectTrack.right,m_rectTrack.top);
		pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
		pDC->LineTo(m_rectTrack.left,m_rectTrack.bottom);
		pDC->LineTo(m_rectTrack.left,m_rectTrack.top);
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
		m_rectOld=m_rectTrack;
		m_pWnd->ReleaseDC(pDC);
	}
	if(m_nProcess==0 && !m_bDown && m_bSet)
	{
		m_nChoNum=PtInRect(point);
		SetMouseCursor();
	}
	else if(m_nProcess==0 && m_bSet)
	{
		//ÃßÀû¹Ú½º ¿µ¿ªÀÌ ¾Æ´Ï¸é ¸®ÅÏ
		if(m_nChoNum<0)
			return;
		//ÃßÀû¹Ú½º ¿µ¿ªÀÏ°æ¿ì
		switch(m_nChoNum)
		{
			case 0://ÁÂÃø »ó´Ü ¼±ÅÃ½Ã
					m_rectTrack.left=point.x;
					m_rectTrack.top=point.y;
					break;
			case 1://¿ìÃø »ó´Ü ¼±ÅÃ½Ã 
					m_rectTrack.right=point.x;
					m_rectTrack.top=point.y;
					break;
			case 2://¿ìÃø ÇÏ´Ü ¼±ÅÃ½Ã
					m_rectTrack.right=point.x;
					m_rectTrack.bottom=point.y;
					break;
			case 3://ÁÂÃø ÇÏ´Ü ¼±ÅÃ½Ã
					m_rectTrack.left=point.x;
					m_rectTrack.bottom=point.y;
					break;
			case 4://À§ÂÊÀ¸·Î 
					m_rectTrack.top=point.y;
					break;
			case 5://¾Æ·¡·Î
					m_rectTrack.bottom=point.y;
					break;
			case 6://ÁÂÃøÀ¸·Î
					m_rectTrack.left=point.x;
					break;
			case 7://¿ìÃøÀ¸·Î
					m_rectTrack.right=point.x;
					break;
					//ÀÌµ¿ÀÏ°æ¿ì
			case 8:case 9:case 10:case 11:
				{
					CPoint def;
					def=point-m_OldPoint;
					m_rectTrack.top+=def.y;
					m_rectTrack.bottom+=def.y;
					m_rectTrack.left+=def.x;
					m_rectTrack.right+=def.x;
					m_OldPoint=point;
				}
					break;
		}
		CDC *pDC=m_pWnd->GetDC();
		CPen pen,*oldPen;
		pDC->SetROP2(R2_XORPEN);
		//ÀÌÀü ±×·ÁÁø ¿µ¿ª Áö¿ì°í
		pen.CreatePen(PS_DOT,1,RGB(125,125,125));
		oldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(m_rectOld.left,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.top);
		pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
		pDC->LineTo(m_rectOld.left,m_rectOld.top);
		//»õ·Î¿î ¿µ¿ªÀ» ±×¸°´Ù.
		pDC->MoveTo(m_rectTrack.left,m_rectTrack.top);
		pDC->LineTo(m_rectTrack.right,m_rectTrack.top);
		pDC->LineTo(m_rectTrack.right,m_rectTrack.bottom);
		pDC->LineTo(m_rectTrack.left,m_rectTrack.bottom);
		pDC->LineTo(m_rectTrack.left,m_rectTrack.top);
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
		m_rectOld=m_rectTrack;
		m_pWnd->ReleaseDC(pDC);
	}


}
//¸¶¿ì½º ¹öÆ° ´­·¶À»¶§
void CMyTracker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//¸¸¾à¿¡ ÇöÀç ¾Æ¹«°Íµµ ¼³Á¤µÇ¾î ÀÖÁö¾Ê°í
	//¸¶¿ì½º¹öÆ°À» ´©¸§
	if(m_nChoNum<0)
	{
		//È­»ìÇ¥ Ä¿¼­ ¼³Á¤
		SetClassLong(m_pWnd->m_hWnd,GCL_HCURSOR,(long)LoadCursor(NULL,IDC_ARROW));
		//m_rectTrack¿Í m_rectOld¸¦ ÇöÀç pointÀÇ ÇÑÁ¡À¸·Î ¼³Á¤
		m_rectTrack.left=point.x;
		m_rectTrack.right=point.x;
		m_rectTrack.top=point.y;
		m_rectTrack.bottom=point.y;
		m_rectOld=m_rectTrack;
	}
	//m_OldPoint¿¡ point°ª ¼³Á¤
	m_OldPoint=point;
	m_bDown=TRUE;
}
//¸¶¿ì½º ¹öÆ° ³õÀ½
void CMyTracker::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDC *pDC=m_pWnd->GetDC();
	CPen pen,*oldPen;
	pDC->SetROP2(R2_XORPEN);
	pen.CreatePen(PS_DOT,10,RGB(125,125,125));
	oldPen=pDC->SelectObject(&pen);
	pDC->MoveTo(m_rectOld.left,m_rectOld.top);
	pDC->LineTo(m_rectOld.right,m_rectOld.top);
	pDC->LineTo(m_rectOld.right,m_rectOld.bottom);
	pDC->LineTo(m_rectOld.left,m_rectOld.bottom);
	pDC->LineTo(m_rectOld.left,m_rectOld.top);

	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	m_pWnd->ReleaseDC(pDC);
	m_rectOld=CRect(0,0,0,0);
	m_bDown=FALSE;
}
//ÇÁ·Î¼¼¼­ °ª ¼³Á¤
void CMyTracker::SetProcess(int num)
{
	m_nProcess=num;
}
//ÇöÀç ¼³Á¤µÈ ÇÁ·Î¼¼¼­ °ª ¾òÀ½
int CMyTracker::GetProcess()
{
	return m_nProcess;
}
//12°³ÀÇ ¹æÇâÀ» ¸¸µé¾î ÁØ´Ù.
void CMyTracker::MakeDirect()
{
	m_rect=CRect(min(m_rect.left,m_rect.right),
				 min(m_rect.top,m_rect.bottom),
				 max(m_rect.left,m_rect.right),
				 max(m_rect.top,m_rect.bottom));
	m_rectTrack=m_rect;

	int pos;
	m_rectDirect[0]=CRect(m_rect.left-m_nLineDot,m_rect.top-m_nLineDot,
		m_rect.left+m_nLineDot,m_rect.top+m_nLineDot);
	m_rectDirect[1]=CRect(m_rect.right-m_nLineDot,m_rect.top-m_nLineDot,
			m_rect.right+m_nLineDot,m_rect.top+m_nLineDot);

	m_rectDirect[2]=CRect(m_rect.right-m_nLineDot,m_rect.bottom-m_nLineDot,
		m_rect.right+m_nLineDot,m_rect.bottom+m_nLineDot);

	m_rectDirect[3]=CRect(m_rect.left-m_nLineDot,m_rect.bottom-m_nLineDot,
		m_rect.left+m_nLineDot,m_rect.bottom+m_nLineDot);

	pos=(m_rect.Width()/2)+m_rect.TopLeft().x; 
	m_rectDirect[4]=CRect(pos-m_nLineDot,m_rect.top-m_nLineDot,
		pos+m_nLineDot,m_rect.top+m_nLineDot);
	pos=(m_rect.Width()/2)+m_rect.TopLeft().x; 
	m_rectDirect[5]=CRect(pos-m_nLineDot,m_rect.bottom-m_nLineDot,
		pos+m_nLineDot,m_rect.bottom+m_nLineDot);
	pos=(m_rect.Height()/2)+m_rect.TopLeft().y; 
	m_rectDirect[6]=CRect(m_rect.left-m_nLineDot,pos-m_nLineDot,
		m_rect.left+m_nLineDot,pos+m_nLineDot);
	m_rectDirect[7]=CRect(m_rect.right-m_nLineDot,pos-m_nLineDot,
		m_rect.right+m_nLineDot,pos+m_nLineDot);

	m_rectDirect[8]=CRect(m_rect.left,m_rect.top-m_nLineDot,
		m_rect.right,m_rect.top+m_nLineDot);
	m_rectDirect[9]=CRect(m_rect.left-m_nLineDot,m_rect.top,
		m_rect.left+m_nLineDot,m_rect.bottom);
	m_rectDirect[10]=CRect(m_rect.right-m_nLineDot,m_rect.top,
		m_rect.right+m_nLineDot,m_rect.bottom);
	m_rectDirect[11]=CRect(m_rect.left,m_rect.bottom-m_nLineDot,
		m_rect.right,m_rect.bottom+m_nLineDot);

}
//ÃßÀû¹ÚÀ¸ ¿µ¿ªÀ» ¼³Á¤ÇÑ´Ù.
void CMyTracker::SetRect(CRect rect)
{
	m_rect=rect;
	m_rectTrack=rect;
}
