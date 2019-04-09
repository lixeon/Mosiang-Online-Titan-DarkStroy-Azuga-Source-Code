// DialogDirectionalLightProperty.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogDirectionalLightProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tool.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectionalLightProperty dialog


CDialogDirectionalLightProperty::CDialogDirectionalLightProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDirectionalLightProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDirectionalLightProperty)
	m_bAmbientB = 0;
	m_bAmbientG = 0;
	m_bAmbientR = 0;
	m_bDiffuseB = 255;
	m_bDiffuseG = 255;
	m_bDiffuseR = 255;
	m_bSpecularB = 0;
	m_bSpecularG = 0;
	m_bSpecularR = 0;
	m_fDirZ = 0.0f;
	m_fDirY = 0.0f;
	m_fDirX = 0.0f;
	m_bEnableLight = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogDirectionalLightProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDirectionalLightProperty)
	DDX_Text(pDX, IDC_EDIT_AMBIENT_B, m_bAmbientB);
	DDV_MinMaxByte(pDX, m_bAmbientB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_AMBIENT_G, m_bAmbientG);
	DDV_MinMaxByte(pDX, m_bAmbientG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_AMBIENT_R, m_bAmbientR);
	DDV_MinMaxByte(pDX, m_bAmbientR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_B, m_bDiffuseB);
	DDV_MinMaxByte(pDX, m_bDiffuseB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_G, m_bDiffuseG);
	DDV_MinMaxByte(pDX, m_bDiffuseG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_R, m_bDiffuseR);
	DDV_MinMaxByte(pDX, m_bDiffuseR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_B, m_bSpecularB);
	DDV_MinMaxByte(pDX, m_bSpecularB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_G, m_bSpecularG);
	DDV_MinMaxByte(pDX, m_bSpecularG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_R, m_bSpecularR);
	DDV_MinMaxByte(pDX, m_bSpecularR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DIR_Z, m_fDirZ);
	DDX_Text(pDX, IDC_EDIT_DIR_Y, m_fDirY);
	DDX_Text(pDX, IDC_EDIT_DIR_X, m_fDirX);
	DDX_Check(pDX, IDC_CHECK_DIRECTIONAL_LIGHT, m_bEnableLight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDirectionalLightProperty, CDialog)
	//{{AFX_MSG_MAP(CDialogDirectionalLightProperty)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDirectionalLightProperty message handlers


void CDialogDirectionalLightProperty::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_pLightDesc->dwAmbient = 0xff000000 | (m_bAmbientR<<16) | (m_bAmbientG<<8) |  m_bAmbientB;
	m_pLightDesc->dwDiffuse = 0xff000000 | (m_bDiffuseR<<16) | (m_bDiffuseG<<8) |  m_bDiffuseB;
	m_pLightDesc->dwSpecular = 0xff000000 | (m_bSpecularR<<16) | (m_bSpecularG<<8) |  m_bSpecularB;
	m_pLightDesc->v3Dir.x = m_fDirX;
	m_pLightDesc->v3Dir.y = m_fDirY;
	m_pLightDesc->v3Dir.z = m_fDirZ;
	m_pLightDesc->bEnable = m_bEnableLight;


	
	EndDialog(0);
}

BOOL CDialogDirectionalLightProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bDiffuseR = (m_pLightDesc->dwDiffuse & 0x00ff0000) >> 16;
	m_bDiffuseG = (m_pLightDesc->dwDiffuse & 0x0000ff00) >> 8;
	m_bDiffuseB = m_pLightDesc->dwDiffuse & 0x000000ff;
	
	m_bAmbientR = (m_pLightDesc->dwAmbient & 0x00ff0000) >> 16;
	m_bAmbientG = (m_pLightDesc->dwAmbient & 0x0000ff00) >> 8;
	m_bAmbientB = m_pLightDesc->dwAmbient & 0x000000ff;
	
	m_bSpecularR = (m_pLightDesc->dwSpecular & 0x00ff0000) >> 16;
	m_bSpecularG = (m_pLightDesc->dwSpecular & 0x0000ff00) >> 8;
	m_bSpecularB = m_pLightDesc->dwSpecular & 0x000000ff;

	m_fDirX = m_pLightDesc->v3Dir.x;
	m_fDirY = m_pLightDesc->v3Dir.y;
	m_fDirZ = m_pLightDesc->v3Dir.z;

	m_bEnableLight = m_pLightDesc->bEnable;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
