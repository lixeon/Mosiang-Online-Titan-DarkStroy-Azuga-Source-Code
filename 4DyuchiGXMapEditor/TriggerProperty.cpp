// TriggerProperty.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "TriggerProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerProperty dialog


CTriggerProperty::CTriggerProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTriggerProperty)
	m_dwID = 0;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_bB = 0;
	m_bG = 0;
	m_bR = 0;
	//}}AFX_DATA_INIT
}


void CTriggerProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriggerProperty)
	DDX_Text(pDX, IDC_EDIT_UID, m_dwID);
	DDX_Text(pDX, IDC_EDIT_X, m_fX);
	DDV_MinMaxFloat(pDX, m_fX, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Y, m_fY);
	DDV_MinMaxFloat(pDX, m_fY, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Z, m_fZ);
	DDV_MinMaxFloat(pDX, m_fZ, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_B, m_bB);
	DDV_MinMaxByte(pDX, m_bB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_G, m_bG);
	DDV_MinMaxByte(pDX, m_bG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_R, m_bR);
	DDV_MinMaxByte(pDX, m_bR, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTriggerProperty, CDialog)
	//{{AFX_MSG_MAP(CTriggerProperty)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerProperty message handlers

BOOL CTriggerProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_bR = (m_pProperty->evDesc.dwColor & 0x00ff0000) >> 16;
	m_bG = (m_pProperty->evDesc.dwColor & 0x0000ff00) >> 8;
	m_bB = m_pProperty->evDesc.dwColor & 0x000000ff;

	m_fX = 	m_pProperty->evDesc.v3Pos.x;
	m_fY = 	m_pProperty->evDesc.v3Pos.y;
	m_fZ = 	m_pProperty->evDesc.v3Pos.z;

	m_dwID = m_pProperty->dwID;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTriggerProperty::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_pProperty->evDesc.dwColor = 0xff000000 | (m_bR<<16) | (m_bG<<8) |  m_bB;

	m_pProperty->evDesc.v3Pos.x = m_fX;
	m_pProperty->evDesc.v3Pos.y = m_fY;
	m_pProperty->evDesc.v3Pos.z = m_fZ;

	m_pProperty->dwID = m_dwID;
	EndDialog(0);

}

void CTriggerProperty::OnButtonDelete() 
{
	EndDialog(0xffffffff);
	// TODO: Add your control notification handler code here
	
}
