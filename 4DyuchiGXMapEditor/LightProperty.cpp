// LightProperty.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "LightProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightProperty dialog


CLightProperty::CLightProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CLightProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightProperty)
	m_R = 0;
	m_B = 0;
	m_G = 0;
	m_fRS = 0.0f;
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;
	m_bLightSwitch = TRUE;
	m_bShadowSwitch = TRUE;
	m_bEnabeDynamicLight = FALSE;
	//}}AFX_DATA_INIT
	m_pLightProperty = NULL;
}

void CLightProperty::SetGXLightProperty(GXLIGHT_PROPERTY* pLightProperty)
{
	m_pLightProperty = pLightProperty;

}

void CLightProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightProperty)
	DDX_Text(pDX, IDC_EDIT_R, m_R);
	DDV_MinMaxDWord(pDX, m_R, 0, 255);
	DDX_Text(pDX, IDC_EDIT_B, m_B);
	DDV_MinMaxDWord(pDX, m_B, 0, 255);
	DDX_Text(pDX, IDC_EDIT_G, m_G);
	DDV_MinMaxDWord(pDX, m_G, 0, 255);
	DDX_Text(pDX, IDC_EDIT_RS, m_fRS);
	DDV_MinMaxFloat(pDX, m_fRS, 0.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_X, m_fPosX);
	DDV_MinMaxFloat(pDX, m_fPosX, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Y, m_fPosY);
	DDV_MinMaxFloat(pDX, m_fPosY, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Z, m_fPosZ);
	DDV_MinMaxFloat(pDX, m_fPosZ, -900000.0f, 900000.0f);
	DDX_Check(pDX, IDC_CHECK_ONOFF, m_bLightSwitch);
	DDX_Check(pDX, IDC_CHECK_SHADOW_ONOFF, m_bShadowSwitch);
	DDX_Check(pDX, IDC_CHECK_ENABLE_DYNAMIC_LIGHT, m_bEnabeDynamicLight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightProperty, CDialog)
	//{{AFX_MSG_MAP(CLightProperty)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightProperty message handlers

void CLightProperty::OnButtonOk() 
{
	UpdateData(TRUE);

	m_pLightProperty->lightDesc.dwDiffuse = 0xff000000 | (m_R<<16) | (m_G<<8) |  m_B;
	m_pLightProperty->lightDesc.fRs = m_fRS;
	m_pLightProperty->lightDesc.v3Point.x = m_fPosX;
	m_pLightProperty->lightDesc.v3Point.y = m_fPosY;
	m_pLightProperty->lightDesc.v3Point.z = m_fPosZ;

	m_pLightProperty->bLightSwitch = m_bLightSwitch;
	m_pLightProperty->bShadowSwitch = m_bShadowSwitch;
	m_pLightProperty->bEnableDynamicLight = m_bEnabeDynamicLight;
	
	// TODO: Add your control notification handler code here
	EndDialog(0);

}



BOOL CLightProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_R = (m_pLightProperty->lightDesc.dwDiffuse & 0x00ff0000) >> 16;
	m_G = (m_pLightProperty->lightDesc.dwDiffuse & 0x0000ff00) >> 8;
	m_B = m_pLightProperty->lightDesc.dwDiffuse & 0x000000ff;
	
	m_fPosX = m_pLightProperty->lightDesc.v3Point.x;
	m_fPosY	= m_pLightProperty->lightDesc.v3Point.y;
	m_fPosZ	= m_pLightProperty->lightDesc.v3Point.z;

	m_bLightSwitch = m_pLightProperty->bLightSwitch;
	m_bShadowSwitch = m_pLightProperty->bShadowSwitch;
	m_bEnabeDynamicLight = m_pLightProperty->bEnableDynamicLight;

	m_fRS = m_pLightProperty->lightDesc.fRs;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightProperty::OnButtonDelete() 
{
	EndDialog(0xffffffff);

	// TODO: Add your control notification handler code here
	
}
