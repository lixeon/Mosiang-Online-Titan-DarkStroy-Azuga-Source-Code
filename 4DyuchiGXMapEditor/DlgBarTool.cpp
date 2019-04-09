// DlgBarTool.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DlgBarTool.h"
#include "LightPosDlg.h"
#include "DialogDirectionalLightProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tool.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBarTool dialog


CDlgBarTool::CDlgBarTool()
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDlgBarTool)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgBarTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBarTool)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBarTool, CDialogBar)
	//{{AFX_MSG_MAP(CDlgBarTool)
	ON_BN_CLICKED(IDC_RADIO_XAXIS, OnRadioXaxis)
	ON_BN_CLICKED(IDC_RADIO_YAXIS, OnRadioYaxis)
	ON_BN_CLICKED(IDC_RADIO_ZAXIS, OnRadioZaxis)
	ON_BN_CLICKED(IDC_RADIO_AXIS_FREE, OnRadioAxisFree)
	ON_BN_CLICKED(IDC_RADIO_CREATE, OnRadioCreate)
	ON_BN_CLICKED(IDC_RADIO_MOVE,OnRadioMove)
	ON_BN_CLICKED(IDC_RADIO_ROT, OnRadioRot)
	ON_BN_CLICKED(IDC_RADIO_SCALE, OnRadioScale)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TR, OnButtonResetTr)
	ON_BN_CLICKED(IDC_RADIO_EDIT_TILE, OnRadioEditTile)
	ON_BN_CLICKED(IDC_RADIO_EDIT_VERTEX, OnRadioEditVertex)
	ON_BN_CLICKED(IDC_RADIO_EDIT_GXO, OnRadioEditGxo)
	ON_BN_CLICKED(IDC_RADIO_EDIT_BRUSH, OnRadioEditBrush)
	ON_BN_CLICKED(IDC_RADIO_LIGHT_MODE, OnRadioLightMode)
	ON_BN_CLICKED(IDC_RADIO_TRIGGER_MODE, OnRadioTriggerMode)
	ON_BN_CLICKED(IDC_RADIO_GET_POS_TILE, OnRadioGetPosTile)
	ON_BN_CLICKED(IDC_RADIO_RMODE_SOLID, OnRadioRmodeSolid)
	ON_BN_CLICKED(IDC_RADIO_RMODE_WIRE, OnRadioRmodeWire)
	ON_BN_CLICKED(IDC_BUTTON_LPOS, OnButtonLightPos)
	ON_BN_CLICKED(IDC_RADIO_GET_HFOBJ, OnRadioGetHfobj)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LPOS,OnUpdateCmdUIAllLightMove)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DIRECTIONAL_LIGHT,OnUpdateCmdUIDirectionalLightButton)
	ON_BN_CLICKED(IDC_BUTTON_DIRECTIONAL_LIGHT, OnButtonDirectionalLight)
	ON_BN_CLICKED(IDC_RADIO_DRAW_LIGHTMAP, OnRadioDrawLightmap)
	ON_BN_CLICKED(IDC_RADIO_DRAW_HFIELD_VCOLOR, OnRadioDrawHFieldVertexColor)	
	ON_BN_CLICKED(IDC_RADIO_DRAW_ALPHA_TEXEL, OnRadioDrawAlphaTexel)
	ON_BN_CLICKED(IDC_RADIO_TS, OnRadioTs)
	ON_BN_CLICKED(IDC_RADIO_RMODE_BOTH, OnRadioRmodeBoth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBarTool message handlers


void CDlgBarTool::OnRadioZaxis() 
{
	// TODO: Add your control notification handler code here
	g_pTool->LockZAxis();	
	
	HWND	hButton;
	GetDlgItem(IDC_RADIO_ZAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_CHECKED,0);

	
	GetDlgItem(IDC_RADIO_XAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

	
	GetDlgItem(IDC_RADIO_YAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_AXIS_FREE,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

}

void CDlgBarTool::OnRadioYaxis() 
{
	// TODO: Add your control notification handler code here
	g_pTool->LockYAxis();
	
	HWND	hButton;
	GetDlgItem(IDC_RADIO_YAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_CHECKED,0);

	
	GetDlgItem(IDC_RADIO_XAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

	
	GetDlgItem(IDC_RADIO_ZAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_AXIS_FREE,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

}

void CDlgBarTool::OnRadioXaxis() 
{
	// TODO: Add your control notification handler code here
	
	g_pTool->LockXAxis();
	
	HWND	hButton;
	GetDlgItem(IDC_RADIO_XAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_CHECKED,0);


	GetDlgItem(IDC_RADIO_YAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);


	GetDlgItem(IDC_RADIO_ZAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_AXIS_FREE,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);
}
void CDlgBarTool::OnRadioAxisFree() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetFreeAxis();
		
	HWND	hButton;

	GetDlgItem(IDC_RADIO_AXIS_FREE,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_CHECKED,0);

	GetDlgItem(IDC_RADIO_XAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);


	GetDlgItem(IDC_RADIO_YAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);


	GetDlgItem(IDC_RADIO_ZAXIS,&hButton);
	::SendMessage(hButton,BM_SETCHECK,BST_UNCHECKED,0);
}
void CDlgBarTool::OnRadioCreate() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetEditType(EDIT_TYPE_CREATE);

}
void CDlgBarTool::OnRadioMove() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetEditType(EDIT_TYPE_MOVE);
}

void CDlgBarTool::OnRadioRot() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetEditType(EDIT_TYPE_ROT);
}

void CDlgBarTool::OnRadioScale() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetEditType(EDIT_TYPE_SCALE);
}
void CDlgBarTool::OnButtonResetTr() 
{
	// TODO: Add your control notification handler code here
	g_pTool->ResetTransformBrush();
	
}


void CDlgBarTool::OnRadioEditTile() 
{
	// TODO: Add your control notification handler code here
	if (g_pTool)
		g_pTool->SetPickTileMode();
}

void CDlgBarTool::OnRadioEditVertex() 
{
	// TODO: Add your control notification handler code here
	if (g_pTool)
		g_pTool->SetPickVertexMode();
}

void CDlgBarTool::OnRadioEditGxo() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetGXObjectMode();
}

void CDlgBarTool::OnRadioEditBrush() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetBrushMode();
	
}

void CDlgBarTool::OnRadioGetPosTile() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetTileInfoMode();

}

void CDlgBarTool::OnRadioLightMode() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetLightMode();

}

void CDlgBarTool::OnRadioTriggerMode() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetTriggerMode();
	
	
}

void CDlgBarTool::OnRadioRmodeSolid() 
{
	// TODO: Add your control notification handler code here
	if (g_pTool)
	{
		g_pTool->SetRenderMode(0);
		g_pTool->Render();
	}

	HWND hWnd;
	
	GetDlgItem(IDC_RADIO_RMODE_WIRE,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_RMODE_BOTH,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);
	
}

void CDlgBarTool::OnRadioRmodeWire() 
{
	// TODO: Add your control notification handler code here
	if (g_pTool)
	{
		g_pTool->SetRenderMode(2);
		g_pTool->Render();
	}


	HWND hWnd;
	
	GetDlgItem(IDC_RADIO_RMODE_SOLID,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_RMODE_BOTH,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);
}


void CDlgBarTool::OnRadioRmodeBoth() 
{
	// TODO: Add your control notification handler code here
	if (g_pTool)
	{
		g_pTool->EnableRenderBothMode();
		g_pTool->Render();
	}

	HWND hWnd;
	
	GetDlgItem(IDC_RADIO_RMODE_WIRE,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);

	GetDlgItem(IDC_RADIO_RMODE_SOLID,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);
	
}




void CDlgBarTool::OnButtonLightPos()
{
	CLightPosDlg Dlg;

	if( Dlg.DoModal() == IDOK )
	{
		VECTOR3	v3AddLightPos;
		v3AddLightPos.x	= Dlg.GetPosX();
		v3AddLightPos.y	= Dlg.GetPosY();
		v3AddLightPos.z	= Dlg.GetPosZ();

		g_pTool->SetLightPositionAll( v3AddLightPos );
	}
}


void CDlgBarTool::OnRadioGetHfobj() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetHFieldObjectMode();
	
}
void CDlgBarTool::OnUpdateCmdUIAllLightMove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDlgBarTool::OnUpdateCmdUIDirectionalLightButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}





void CDlgBarTool::OnButtonDirectionalLight() 
{
	// TODO: Add your control notification handler code here
	DIRECTIONAL_LIGHT_DESC	lightDesc;
	g_pTool->GetDirectionalLightDesc(&lightDesc);

	CDialogDirectionalLightProperty	lightProperty;
	lightProperty.SetLightDescPtr(&lightDesc);
	lightProperty.DoModal();
	if (lightDesc.bEnable)
		g_pTool->EnableDirectionalLight(&lightDesc);
	else 
		g_pTool->DisableDirectionalLight();



}

void CDlgBarTool::OnRadioDrawLightmap() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetDrawLightMapMode();

	
}

void CDlgBarTool::OnRadioDrawHFieldVertexColor() 
{
	// TODO: Add your control notification handler code here
	g_pTool->SetDrawHFieldVertexColor();

	
}



void CDlgBarTool::OnRadioDrawAlphaTexel() 
{
	// TODO: Add your control notification handler code here
	
	g_pTool->SetDrawHFieldaAlphaMap();

}

//yh
void CDlgBarTool::OnRadioTs() 
{	
	// TODO: Add your control notification handler code here
	if (g_pTool)
		g_pTool->SetPickTileSetMode();	
}
//




//trustpak

VOID CDlgBarTool::ToggleTileEditButton(VOID)
{
	OnRadioEditTile();
	
	HWND hWnd;
	GetDlgItem(IDC_RADIO_EDIT_TILE,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_CHECKED,0);
	
	GetDlgItem(IDC_RADIO_TS,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);
}

VOID CDlgBarTool::ToggleTileSetEditButton(VOID)
{
	OnRadioTs();

	HWND hWnd;
	GetDlgItem(IDC_RADIO_TS,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_CHECKED,0);

	GetDlgItem(IDC_RADIO_EDIT_TILE,&hWnd);
	::SendMessage(hWnd, BM_SETCHECK, BST_UNCHECKED,0);
	
}

