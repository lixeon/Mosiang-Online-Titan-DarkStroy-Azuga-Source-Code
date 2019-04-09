// DlgBarHField.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "MainFrm.h"
#include "tool.h"
#include "DlgBarHField.h"

#include "DialogHFieldProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDlgBarHField*	g_pDlgBarHField = NULL;



CDlgBarHField::CDlgBarHField()
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDlgBarHField)
	m_dwBrushSize = 1;
	m_fHeightPitch = 20.0f;
	m_fBias = 16.0f;
	m_fRadius = 600.0f;
	m_dwHFieldDetail = 0;
	m_dwFreezeDetailLevel = 0;
	m_bG = 0;
	m_bR = 0;
	m_bB = 0;
	m_fHFieldColorBrushSize = 200.0f;
	m_bEnableHFieldBlend = FALSE;
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CDlgBarHField, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarHField)
	ON_EN_CHANGE(IDC_EDIT_BRUSH_SIZE, OnChangeEditBrushSize)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_PROPERTY, OnButtonViewProperty)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_VIEW_PROPERTY,OnUpdateCmdUIPropertyButton)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SELECT,OnUpdateCmdUISelectButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_REFRESH_HFIELD,OnUpdateCmdUIRefreshButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HFOBJ_DISABLE,OnUpdateCmdUIHFObjDisableButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HFOBJ_ENABLE,OnUpdateCmdUIHFObjEnableButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_RESET_Z,OnUpdateCmdUIResetZButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HFOBJ_FREEZE_DETAIL,OnUpdateCmdUIHFObjFreezeDetailButton)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HFOBJ_UNFREEZE_DETAIL,OnUpdateCmdUIHFObjUnfreezeDetailButton)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_BRUSH_SIZE,OnUpdateCmdUIEditBrush)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_FREEZE_DETAIL,OnUpdateCmdUIEditFreezeDetail)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_HFBRUSH_SIZE,OnCmdUIUpdateEditHfbrushSize)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_LM_R,OnCmdUIUpdateEditLM_R)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_LM_G,OnCmdUIUpdateEditLM_G)
	ON_UPDATE_COMMAND_UI(IDC_EDIT_LM_B,OnCmdUIUpdateEditLM_B)
	ON_BN_CLICKED(IDC_CHECK_SELECT, OnCheckSelect)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT_PITCH, OnChangeEditHeightPitch)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, OnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_INTERPOLATION_BIAS, OnChangeEditInterpolationBias)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_HFIELD, OnButtonRefreshHfield)
	ON_EN_CHANGE(IDC_EDIT_HFIELD_DETAIL, OnChangeEditHfieldDetail)
	ON_BN_CLICKED(IDC_BUTTON_HFOBJ_DISABLE, OnButtonHfobjDisable)
	ON_BN_CLICKED(IDC_BUTTON_HFOBJ_ENABLE, OnButtonHfobjEnable)
	ON_BN_CLICKED(IDC_BUTTON_RESET_Z, OnButtonResetZ)
	ON_BN_CLICKED(IDC_BUTTON_HFOBJ_FREEZE_DETAIL, OnButtonHfobjFreezeDetail)
	ON_BN_CLICKED(IDC_BUTTON_HFOBJ_UNFREEZE_DETAIL, OnButtonHfobjUnfreezeDetail)
	ON_EN_CHANGE(IDC_EDIT_FREEZE_DETAIL, OnChangeEditFreezeDetail)
	ON_EN_CHANGE(IDC_EDIT_LM_B, OnChangeEditLmB)
	ON_EN_CHANGE(IDC_EDIT_LM_G, OnChangeEditLmG)
	ON_EN_CHANGE(IDC_EDIT_LM_R, OnChangeEditLmR)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CHECK_PICK_COLOR, OnCheckPickColor)
	ON_EN_CHANGE(IDC_EDIT_HFBRUSH_SIZE, OnChangeEditHfbrushSize)
	ON_BN_CLICKED(IDC_CHECK_HFIELD_BLEND, OnCheckHfieldBlend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgBarHField dialog






void CDlgBarHField::DoDataExchange(CDataExchange* pDX)
{
	

	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarHField)
	DDX_Control(pDX, IDC_CHECK_PICK_COLOR, m_btPickColor);
	DDX_Control(pDX, IDC_CHECK_SELECT, m_btSelect);
	DDX_Text(pDX, IDC_EDIT_BRUSH_SIZE, m_dwBrushSize);
	DDV_MinMaxDWord(pDX, m_dwBrushSize, 1, 128);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_PITCH, m_fHeightPitch);
	DDV_MinMaxFloat(pDX, m_fHeightPitch, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT_INTERPOLATION_BIAS, m_fBias);
	DDV_MinMaxFloat(pDX, m_fBias, 1.f, 64.f);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_fRadius);
	DDV_MinMaxFloat(pDX, m_fRadius, 1.f, 9000.f);
	DDX_Text(pDX, IDC_EDIT_HFIELD_DETAIL, m_dwHFieldDetail);
	DDV_MinMaxDWord(pDX, m_dwHFieldDetail, 0, 8);
	DDX_Text(pDX, IDC_EDIT_FREEZE_DETAIL, m_dwFreezeDetailLevel);
	DDV_MinMaxDWord(pDX, m_dwFreezeDetailLevel, 0, 8);
	DDX_Text(pDX, IDC_EDIT_LM_G, m_bG);
	DDV_MinMaxByte(pDX, m_bG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LM_R, m_bR);
	DDV_MinMaxByte(pDX, m_bR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_LM_B, m_bB);
	DDV_MinMaxByte(pDX, m_bB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_HFBRUSH_SIZE, m_fHFieldColorBrushSize);
	DDX_Check(pDX, IDC_CHECK_HFIELD_BLEND, m_bEnableHFieldBlend);
	//}}AFX_DATA_MAP
	UpdateColor();

	

	

}




/////////////////////////////////////////////////////////////////////////////
// CDlgBarHField message handlers

void CDlgBarHField::OnChangeEditBrushSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}


void CDlgBarHField::OnCmdUIUpdateEditLM_R(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnCmdUIUpdateEditLM_G(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnCmdUIUpdateEditLM_B(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUIEditBrush(CCmdUI* pCmdUI)
{

//	UpdateData(FALSE);
	pCmdUI->Enable();

}
void CDlgBarHField::OnUpdateCmdUIEditFreezeDetail(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();

}
void CDlgBarHField::OnUpdateCmdUIPropertyButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
void CDlgBarHField::OnUpdateCmdUIRefreshButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUISelectButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUIHFObjFreezeDetailButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUIHFObjUnfreezeDetailButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}
void CDlgBarHField::OnUpdateCmdUIResetZButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUIHFObjDisableButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarHField::OnUpdateCmdUIHFObjEnableButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}


void CDlgBarHField::OnButtonViewProperty() 
{
	// TODO: Add your control notification handler code here
	IHeightField*	pHField = g_pTool->GetHField();
	if (!pHField)
		return;

	HFIELD_DESC		hfDesc;
	pHField->GetHFieldDesc(&hfDesc);
	CDialogHFieldProperty	property;
	property.Present((HFIELD_CREATE_DESC*)&hfDesc,FALSE);
	property.DoModal();
	if (hfDesc.dwIndexBufferNumLV0 != property.GetIndexBufferNum())
	{
		g_pTool->ChangeHFieldIndexBufferNum(property.GetIndexBufferNum());
		__asm nop
	}
}

void CDlgBarHField::OnButtonResetZ() 
{
	// TODO: Add your control notification handler code here
	HWND	hWndEdit;

	char	szZ[32];
	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->GetDlgBalHField()->GetDlgItem(IDC_EDIT_DEFAULT_Z,&hWndEdit);
	::GetWindowText(hWndEdit, szZ,32);
	
	float	h = (float)atof(szZ);
	g_pTool->ResetHeight(h);
}

void CDlgBarHField::OnCheckSelect() 
{
	// TODO: Add your control notification handler code here
	
	g_pTool->SetSelectMode(m_btSelect.GetCheck());
}

void CDlgBarHField::ToggleCheckSelect() 
{
	BOOL	bCheck = m_btSelect.GetCheck();

	m_btSelect.SetCheck((~bCheck) & 0x00000001);
	OnCheckSelect();
}

void CDlgBarHField::OnChangeEditHeightPitch() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	
}

DWORD CDlgBarHField::GetLightTexelColor()
{
	UpdateData(TRUE);
	DWORD dwColor;
	dwColor = 0xff000000 | (m_bR<<16) | (m_bG<<8) | m_bB;
	return dwColor;
}
void CDlgBarHField::SetLightTexelColor(DWORD dwColor)
{
	m_bR = (dwColor & 0x00ff0000) >> 16;
	m_bG = (dwColor & 0x0000ff00) >> 8;
	m_bB = (dwColor & 0x000000ff);

	UpdateData(FALSE);
	UpdateColor();

}
void CDlgBarHField::OnChangeEditRadius() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	
}

void CDlgBarHField::OnChangeEditInterpolationBias() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	
}

void CDlgBarHField::OnButtonRefreshHfield() 
{

	// TODO: Add your control notification handler code here
	if (g_pTool->GetHField())
	{
		g_pTool->GetHField()->ResetCollisionMesh();
		g_pTool->Render();
	}
	
}

void CDlgBarHField::OnChangeEditHfieldDetail() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_dwHFieldDetail >= 0 && m_dwHFieldDetail <= 8)
		g_pTool->SetHFieldDetail(m_dwHFieldDetail);
}
void CDlgBarHField::OnChangeEditFreezeDetail() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

}


void CDlgBarHField::OnButtonHfobjDisable() 
{
	// TODO: Add your control notification handler code here
	g_pTool->DisableRenderHFObj();
	
}

void CDlgBarHField::OnButtonHfobjEnable() 
{
	// TODO: Add your control notification handler code here
	g_pTool->EnableRenderHFObj();

	
}







void CDlgBarHField::OnButtonHfobjFreezeDetail() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_pTool->FreezeHFObjDetail(m_dwFreezeDetailLevel);
	
}

void CDlgBarHField::OnButtonHfobjUnfreezeDetail() 
{
	// TODO: Add your control notification handler code here
	g_pTool->UnfreezeHFObjDetail();
	
}


void CDlgBarHField::OnChangeEditLmB() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}

void CDlgBarHField::OnChangeEditLmG() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

}

void CDlgBarHField::OnChangeEditLmR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
}

void CDlgBarHField::UpdateColor()
{

	HWND hColorBox = ::GetDlgItem(m_hWnd,IDC_STATIC_CURRENT_COLOR);
	RECT	rect;
	::GetWindowRect(hColorBox,&rect);

	POINT	p;
	p.x = 0;
	p.y = 0;
	::ScreenToClient(m_hWnd,&p);

	rect.left += p.x;
	rect.right += p.x;
	rect.top += p.y;
	rect.bottom += p.y;

	HDC hDC = ::GetDC(m_hWnd);
	DWORD	dwColor = (m_bB<<16) | (m_bG<<8) | m_bR;
	
	HBRUSH	hBrush = CreateSolidBrush(dwColor);
	HBRUSH	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);

	FillRect(hDC,&rect,hBrush);

	
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);

	::ReleaseDC(m_hWnd,hDC);
	
}

void CDlgBarHField::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	HWND hColorBox = ::GetDlgItem(m_hWnd,IDC_STATIC_CURRENT_COLOR);
	RECT	rect;
	::GetWindowRect(hColorBox,&rect);

	POINT	p;
	p.x = 0;
	p.y = 0;
	::ScreenToClient(m_hWnd,&p);

	rect.left += p.x;
	rect.right += p.x;
	rect.top += p.y;
	rect.bottom += p.y;




	if (point.x < rect.left)
		return;

	if (point.x > rect.right)
		return;

	if (point.y < rect.top)
		return;

	if (point.y > rect.bottom)
		return;



//	DWORD	dwColor = (m_bR<<16) | (m_bG<<8) | m_bB;
	DWORD	dwColor = (m_bB<<16) | (m_bG<<8) | m_bR;

//	dwColor = 0x00ff0000;
	CColorDialog	colorDlg(dwColor,CC_FULLOPEN,NULL);
	colorDlg.DoModal();

	dwColor = colorDlg.GetColor();
	DWORD	r,g,b;
	r = dwColor & 0x000000ff;
	g = (dwColor & 0x0000ff00)>>8;
	b = (dwColor & 0x00ff0000)>>16;
	dwColor = (r<<16) | (g<<8) | b;

	SetLightTexelColor(dwColor);
	UpdateColor();



	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CDlgBarHField::OnCheckPickColor() 
{
	// TODO: Add your control notification handler code here
	
	BOOL	bCheck = m_btPickColor.GetCheck();
	g_pTool->SetPickColorMode(bCheck);

}
void CDlgBarHField::ToggleCheckPickColor() 
{
	BOOL	bCheck = m_btPickColor.GetCheck();

	m_btPickColor.SetCheck((~bCheck) & 0x00000001);
	OnCheckPickColor();
}





void CDlgBarHField::OnCmdUIUpdateEditHfbrushSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	
}




int CDlgBarHField::OnInitdialog()
{
	UpdateData(FALSE);
	UpdateColor();
	
	return 0;
}

void CDlgBarHField::OnChangeEditHfbrushSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if (m_fHFieldColorBrushSize >= 100.0f && m_fHFieldColorBrushSize <= 800.0f )
		g_pTool->SetHFieldVertexColorBrushSize(m_fHFieldColorBrushSize);
}

void CDlgBarHField::OnCheckHfieldBlend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_pTool->EnableHFieldBlend(m_bEnableHFieldBlend);
	g_pTool->Render();

}
