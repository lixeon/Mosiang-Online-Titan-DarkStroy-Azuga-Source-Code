// PropertyList.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyList.h"
//#include "GameResourceManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPropertyItem::CPropertyItem(CString propName, CString curValue, int nItemType, CString cmbItems, BOOL bNumeric, BYTE bType, DWORD MaxNum)
{
	m_propName = propName;
	m_curValue = curValue;
	m_nItemType = nItemType;
	m_cmbItems = cmbItems;
	m_bType = bType;
	m_etMax = MaxNum;
	m_bNumeric = bNumeric;
		
	ClearItemsEx();
}

/////////////////////////////////////////////////////////////////////////////
// CPropertyList

CPropertyList::CPropertyList()
{
}

CPropertyList::~CPropertyList()
{
}


BEGIN_MESSAGE_MAP(CPropertyList, CListBox)
	//{{AFX_MSG_MAP(CPropertyList)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(IDC_PROPCMBBOX, OnKillfocusCmbBox)
	ON_CBN_SELCHANGE(IDC_PROPCMBBOX, OnSelchangeCmbBox)
	ON_EN_KILLFOCUS(IDC_PROPEDITBOX, OnKillfocusEditBox)
	ON_EN_CHANGE(IDC_PROPEDITBOX, OnChangeEditBox)
	ON_BN_CLICKED(IDC_PROPBTNCTRL, OnButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyList message handlers

BOOL CPropertyList::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CListBox::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style |= LBS_OWNERDRAWFIXED;

	m_bTracking = FALSE;
	m_nDivider = 0;
	m_bDivIsSet = FALSE;

	return TRUE;
}

void CPropertyList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 20; //pixels
}


void CPropertyList::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rectFull = lpDIS->rcItem;
	CRect rect = rectFull;
	if (m_nDivider==0)
		m_nDivider = rect.Width() / 2;
	rect.left = m_nDivider;
	CRect rect2 = rectFull;
	rect2.right = rect.left - 1;
	UINT nIndex = lpDIS->itemID;

	if (nIndex != (UINT) -1)
	{
		//draw two rectangles, one for each row column
		dc.FillSolidRect(rect2,RGB(192,192,192));
		dc.DrawEdge(rect2,EDGE_SUNKEN,BF_BOTTOMRIGHT);
		dc.DrawEdge(rect,EDGE_SUNKEN,BF_BOTTOM);

		//get the CPropertyItem for the current row
		CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(nIndex);

		dc.SetBkMode(TRANSPARENT);
		
		//write the property name in the first rectangle
		dc.DrawText(pItem->m_propName,CRect(rect2.left+3,rect2.top+3,
												rect2.right-3,rect2.bottom+3),
												DT_LEFT | DT_SINGLELINE);
		
		//write the initial property value in the second rectangle
		dc.DrawText(pItem->m_curValue,CRect(rect.left+3,rect.top+3,
												rect.right+3,rect.bottom+3),
												DT_LEFT | DT_SINGLELINE);
	}
	dc.Detach();
}

int CPropertyList::AddItem(CString txt)
{
	int nIndex = AddString(txt);
	return nIndex;
}

int CPropertyList::AddPropItem(CPropertyItem* pItem)
{
	int nIndex = AddString(_T(""));
	SetItemDataPtr(nIndex,pItem);
	return nIndex;
}

int CPropertyList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bDivIsSet = FALSE;
	m_nDivider = 0;
	m_bTracking = FALSE;

	m_hCursorSize = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

//	m_SSerif8Font.CreatePointFont(100,_T("System"));

	return 0;
}

void CPropertyList::OnSelchange() 
{
	CRect rect;
	CString lBoxSelText;

	//m_curSel = GetCurSel();

	GetItemRect(m_curSel,rect);
	rect.left = m_nDivider;

	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	if (m_btnCtrl)
		m_btnCtrl.ShowWindow(SW_HIDE);

	if (pItem->m_nItemType==PIT_COMBO)
	{
		//display the combo box.  If the combo box has already been
		//created then simply move it to the new location, else create it
		m_nLastBox = 0;
		if (m_cmbBox)
			m_cmbBox.MoveWindow(rect);
		else
		{	
			rect.bottom += 100;
			m_cmbBox.Create(WS_VSCROLL | CBS_DROPDOWN | CBS_NOINTEGRALHEIGHT | WS_VISIBLE | WS_CHILD | WS_BORDER,
							rect,this,IDC_PROPCMBBOX);
//			m_cmbBox.SetFont(&m_SSerif8Font);
		}

		CString cmbItems = pItem->m_cmbItems;
		lBoxSelText = pItem->m_curValue;
		
		m_cmbBox.ResetContent();

		//add the choices for this particular property
		if( pItem->m_bType == CBT_ITEM )
		{
			ITEM_INFO * pItemInfo = NULL;
			int i, nIndex;
			i = 0;

			while( nIndex = pItem->m_cmbItemsEx[i] )
			{
				pItemInfo = GAMERESRCMNGR->GetItemInfo(nIndex);
				if( pItemInfo )
				{
					int id = m_cmbBox.AddString(pItemInfo->ItemName);
					m_cmbBox.SetItemData(id, pItemInfo->ItemIdx);
				}

				++i;
			}
		}
		else if( pItem->m_bType == CBT_MUGONG )
		{
			SKILLINFO* pSkillInfo = NULL;
			int i, nIndex;
			i = 0;

			while( nIndex = pItem->m_cmbItemsEx[i] )
			{
				pSkillInfo = GAMERESRCMNGR->GetSkillInfo(nIndex);
				if( pSkillInfo )
				{
					int id = m_cmbBox.AddString(pSkillInfo->SkillName);
					m_cmbBox.SetItemData(id, pSkillInfo->SkillIdx);
				}

				++i;
			}
		}
		else
		{
//			m_cmbBox.AddString("");		
			int i,i2;
			i=0;
			while ((i2=cmbItems.Find('|',i)) != -1)
			{
				m_cmbBox.AddString(cmbItems.Mid(i,i2-i));
				i=i2+1;
			}
		}


		m_cmbBox.ShowWindow(SW_SHOW);
		m_cmbBox.SetFocus();

		//jump to the property's current value in the combo box
		int j = m_cmbBox.FindStringExact(0,lBoxSelText);
		if (j != CB_ERR)
			m_cmbBox.SetCurSel(j);
		else
			m_cmbBox.SetCurSel(0);
	}
	else if (pItem->m_nItemType==PIT_EDIT)
	{
		//display edit box
		m_nLastBox = 1;
		m_prevSel = m_curSel;
		rect.bottom -= 3;
		if (m_editBox)
			m_editBox.MoveWindow(rect);
		else
		{	
			DWORD style = ES_LEFT | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD | WS_BORDER;
			if( pItem->m_bNumeric )
				style |= ES_NUMBER;

			m_editBox.Create(style, rect,this,IDC_PROPEDITBOX);
//			m_editBox.SetFont(&m_SSerif8Font);

			if( pItem->m_bNumeric )
				m_editBox.SetLimitText(9);
		}

		lBoxSelText = pItem->m_curValue;

		m_editBox.ShowWindow(SW_SHOW);
		m_editBox.SetFocus();
		//set the text in the edit box to the property's current value
		m_editBox.SetWindowText(lBoxSelText);
	}
	else if (pItem->m_nItemType==PIT_NONE)
	{
		//display edit box
		m_nLastBox = 1;
		m_prevSel = m_curSel;
		rect.bottom -= 3;

		lBoxSelText = pItem->m_curValue;
	}
	else
		DisplayButton(rect);
}

void CPropertyList::DisplayButton(CRect region)
{
	//displays a button if the property is a file/color/font chooser
	m_nLastBox = 2;
	m_prevSel = m_curSel;

	if (region.Width() > 25)
		region.left = region.right - 25;
	region.bottom -= 3;

	if (m_btnCtrl)
		m_btnCtrl.MoveWindow(region);
	else
	{	
		m_btnCtrl.Create("...",BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
						region,this,IDC_PROPBTNCTRL);
//		m_btnCtrl.SetFont(&m_SSerif8Font);
	}

	m_btnCtrl.ShowWindow(SW_SHOW);
	m_btnCtrl.SetFocus();
}

void CPropertyList::OnKillFocus(CWnd* pNewWnd) 
{
	//m_btnCtrl.ShowWindow(SW_HIDE);

	CListBox::OnKillFocus(pNewWnd);
}

void CPropertyList::OnKillfocusCmbBox() 
{
	m_cmbBox.ShowWindow(SW_HIDE);

	Invalidate();
}

void CPropertyList::OnKillfocusEditBox()
{
	CString newStr;
	m_editBox.ShowWindow(SW_HIDE);

	Invalidate();
}

void CPropertyList::OnSelchangeCmbBox()
{
	CString selStr;
	if (m_cmbBox)
	{
		m_cmbBox.GetLBText(m_cmbBox.GetCurSel(),selStr);
		CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);
		pItem->m_curValue = selStr;

		GetParent()->SendMessage(WM_CHANGEDATA, (WPARAM)pItem, (LPARAM)atoi(selStr.GetBuffer(0)));
	}
}

void CPropertyList::OnChangeEditBox()
{
	CString newStr;
	m_editBox.GetWindowText(newStr);
	
	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	if( pItem->m_bNumeric )
	{
		if( pItem->m_etMax < (DWORD)atoi(newStr) )
			itoa(pItem->m_etMax, newStr.GetBuffer(0), 10);
	}

	pItem->m_curValue = newStr;

	GetParent()->SendMessage(WM_CHANGEDATA, (WPARAM)pItem, (LPARAM)atoi(newStr.GetBuffer(0)));
}

void CPropertyList::OnButton()
{
	CPropertyItem* pItem = (CPropertyItem*) GetItemDataPtr(m_curSel);

	//display the appropriate common dialog depending on what type
	//of chooser is associated with the property
	if (pItem->m_nItemType == PIT_COLOR)
	{
		COLORREF initClr;
		CString currClr = pItem->m_curValue;
		//parse the property's current color value
		if (currClr.Find("RGB") > -1)
		{
			int j = currClr.Find(',',3);
			CString bufr = currClr.Mid(4,j-4);
			int RVal = atoi(bufr);
			int j2 = currClr.Find(',',j+1);
			bufr = currClr.Mid(j+1,j2-(j+1));
			int GVal = atoi(bufr);
			int j3 = currClr.Find(')',j2+1);
			bufr = currClr.Mid(j2+1,j3-(j2+1));
			int BVal = atoi(bufr);
			initClr = RGB(RVal,GVal,BVal);
		}
		else
			initClr = 0;
		
		CColorDialog ClrDlg(initClr);
		
		if (IDOK == ClrDlg.DoModal())
		{
			COLORREF selClr = ClrDlg.GetColor();
			CString clrStr;
			clrStr.Format("RGB(%d,%d,%d)",GetRValue(selClr),
						GetGValue(selClr),GetBValue(selClr));
			m_btnCtrl.ShowWindow(SW_HIDE);

			pItem->m_curValue = clrStr;
			Invalidate();
		}
	}
	else if (pItem->m_nItemType == PIT_FILE)
	{
		CString SelectedFile; 
		CString Filter("Gif Files (*.gif)|*.gif||");
	
		CFileDialog FileDlg(TRUE, NULL, NULL, NULL,
			Filter);
		
		CString currPath = pItem->m_curValue;
		FileDlg.m_ofn.lpstrTitle = "Select file";
		if (currPath.GetLength() > 0)
			FileDlg.m_ofn.lpstrInitialDir = currPath.Left(
				currPath.GetLength() - currPath.ReverseFind('\\'));

		if(IDOK == FileDlg.DoModal())
		{
			SelectedFile = FileDlg.GetPathName();
			
			m_btnCtrl.ShowWindow(SW_HIDE);
			
			pItem->m_curValue = SelectedFile;
			Invalidate();
		}
	}
	else if (pItem->m_nItemType == PIT_FONT)
	{	
		CFontDialog FontDlg(NULL,CF_EFFECTS | CF_SCREENFONTS,NULL,this);
		
		if(IDOK == FontDlg.DoModal())
		{
			CString faceName = FontDlg.GetFaceName();
			
			m_btnCtrl.ShowWindow(SW_HIDE);
			
			pItem->m_curValue = faceName;
			Invalidate();
		}
	}
}

void CPropertyList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking)
	{
		//if columns were being resized then this indicates
		//that mouse is up so resizing is done.  Need to redraw
		//columns to reflect their new widths.
		
		m_bTracking = FALSE;
		//if mouse was captured then release it
		if (GetCapture()==this)
			::ReleaseCapture();

		::ClipCursor(NULL);

		CClientDC dc(this);
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		//set the divider position to the new value
		m_nDivider = point.x;

		//redraw
		Invalidate();
	}
	else
	{
		BOOL loc;
		int i = ItemFromPoint(point,loc);
		m_curSel = i;
		CListBox::OnLButtonUp(nFlags, point);
	}
}

void CPropertyList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((point.x>=m_nDivider-5) && (point.x<=m_nDivider+5))
	{
		//if mouse clicked on divider line, then start resizing

		::SetCursor(m_hCursorSize);

		CRect windowRect;
		GetWindowRect(windowRect);
		windowRect.left += 10; windowRect.right -= 10;
		//do not let mouse leave the list box boundary
		::ClipCursor(windowRect);
		
		if (m_cmbBox)
			m_cmbBox.ShowWindow(SW_HIDE);
		if (m_editBox)
			m_editBox.ShowWindow(SW_HIDE);

		CRect clientRect;
		GetClientRect(clientRect);

		m_bTracking = TRUE;
		m_nDivTop = clientRect.top;
		m_nDivBtm = clientRect.bottom;
		m_nOldDivX = point.x;

		CClientDC dc(this);
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));

		//capture the mouse
		SetCapture();
	}
	else
	{
		m_bTracking = FALSE;
		CListBox::OnLButtonDown(nFlags, point);
	}
}

void CPropertyList::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_bTracking)
	{
		//move divider line to the mouse pos. if columns are
		//currently being resized
		CClientDC dc(this);
		//remove old divider line
		InvertLine(&dc,CPoint(m_nOldDivX,m_nDivTop),CPoint(m_nOldDivX,m_nDivBtm));
		//draw new divider line
		InvertLine(&dc,CPoint(point.x,m_nDivTop),CPoint(point.x,m_nDivBtm));
		m_nOldDivX = point.x;
	}
	else if ((point.x >= m_nDivider-5) && (point.x <= m_nDivider+5))
		//set the cursor to a sizing cursor if the cursor is over the row divider
		::SetCursor(m_hCursorSize);
	else
		CListBox::OnMouseMove(nFlags, point);
}

void CPropertyList::InvertLine(CDC* pDC,CPoint ptFrom,CPoint ptTo)
{
	int nOldMode = pDC->SetROP2(R2_NOT);
	
	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo);

	pDC->SetROP2(nOldMode);
}

void CPropertyList::PreSubclassWindow() 
{
	m_bDivIsSet = FALSE;
	m_nDivider = 0;
	m_bTracking = FALSE;
	m_curSel = 1;

	m_hCursorSize = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

//	m_SSerif8Font.CreatePointFont(100,_T("System"));
}
