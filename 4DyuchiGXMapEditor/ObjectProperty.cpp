// ObjectPropery.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "ObjectProperty.h"
#include ".\objectproperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty dialog


CObjectProperty::CObjectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectProperty::IDD, pParent)
	, m_bLock(FALSE)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
{
	//{{AFX_DATA_INIT(CObjectProperty)
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_dwID = 0;
	m_fRad = 0.0f;
	m_fAxisX = 0.0f;
	m_fAxisY = 0.0f;
	m_fAxisZ = 0.0f;
	m_bApplyHField = FALSE;
	m_bAsEffect = FALSE;
	//}}AFX_DATA_INIT
	m_pObjectProperty = NULL;
}

void CObjectProperty::SetGXObjectProperty(GXOBJECT_PROPERTY* pProperty)
{
	m_pObjectProperty = pProperty;
}

void CObjectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectProperty)
	DDX_Text(pDX, IDC_EDIT_X, m_fX);
	DDV_MinMaxFloat(pDX, m_fX, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Y, m_fY);
	DDV_MinMaxFloat(pDX, m_fY, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_Z, m_fZ);
	DDV_MinMaxFloat(pDX, m_fZ, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_UID, m_dwID);
	DDX_Text(pDX, IDC_EDIT_RAD, m_fRad);
	DDX_Text(pDX, IDC_EDIT_AXIS_X, m_fAxisX);
	DDV_MinMaxFloat(pDX, m_fAxisX, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y, m_fAxisY);
	DDV_MinMaxFloat(pDX, m_fAxisY, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_AXIS_Z, m_fAxisZ);
	DDV_MinMaxFloat(pDX, m_fAxisZ, -900000.0f, 900000.0f);
	DDX_Check(pDX, IDC_CHECK_APPLY_HFIELD, m_bApplyHField);
	DDX_Check(pDX, IDC_CHECK_AS_EFFECT, m_bAsEffect);
	//}}AFX_DATA_MAP

	DDX_Check(pDX, IDC_CHECK_TRANSFORM_LOCK, m_bLock);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, m_fScaleZ);
}


BEGIN_MESSAGE_MAP(CObjectProperty, CDialog)
	//{{AFX_MSG_MAP(CObjectProperty)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty message handlers

void CObjectProperty::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
		
	UpdateData(TRUE);

	m_pObjectProperty->v3Pos.x = m_fX;
	m_pObjectProperty->v3Pos.y = m_fY;
	m_pObjectProperty->v3Pos.z = m_fZ;

	m_pObjectProperty->v3Axis.x = m_fAxisX;
	m_pObjectProperty->v3Axis.y = m_fAxisY;
	m_pObjectProperty->v3Axis.z = m_fAxisZ;

	m_pObjectProperty->fRad = m_fRad;

	m_pObjectProperty->v3Scale.x = m_fScaleX;
	m_pObjectProperty->v3Scale.y = m_fScaleY;
	m_pObjectProperty->v3Scale.z = m_fScaleZ;

	m_pObjectProperty->dwID = m_dwID;
	m_pObjectProperty->bApplyHField = m_bApplyHField;
	m_pObjectProperty->bAsEffect = m_bAsEffect;
	m_pObjectProperty->bLock = m_bLock;
	
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CObjectProperty::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0xffffffff);
}

BOOL CObjectProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here
	m_fX = m_pObjectProperty->v3Pos.x;
	m_fY = m_pObjectProperty->v3Pos.y;
	m_fZ = m_pObjectProperty->v3Pos.z;

	m_fAxisX = m_pObjectProperty->v3Axis.x;
	m_fAxisY = m_pObjectProperty->v3Axis.y;
	m_fAxisZ = m_pObjectProperty->v3Axis.z;

	m_fScaleX = m_pObjectProperty->v3Scale.x;
	m_fScaleY = m_pObjectProperty->v3Scale.y;
	m_fScaleZ = m_pObjectProperty->v3Scale.z;


	m_fRad = m_pObjectProperty->fRad;
	m_dwID = m_pObjectProperty->dwID;
	m_bApplyHField = m_pObjectProperty->bApplyHField;
	m_bAsEffect = m_pObjectProperty->bAsEffect;
	m_bLock = m_pObjectProperty->bLock;
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

