// CDialogHFieldProperty.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogHFieldProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tool.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogHFieldProperty dialog


CDialogHFieldProperty::CDialogHFieldProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHFieldProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogHFieldProperty)
	m_fBottom = 0.0f;
	m_fFaceSize = 0.0f;
	m_dwFacesNumPerObjAxis = 0;
	m_strName = _T("");
	m_fLeft = 0.0f;
	m_fRight = 0.0f;
	m_fTop = 0.0f;
	m_dwObjNumX = 0;
	m_dwObjNumZ = 0;
	m_dwDetailLevelNum = 0;
	m_dwIndexBufferNum = 0;
	m_fMinTileLength = 0.0f;
	//}}AFX_DATA_INIT
//	Create(CDialogHFieldProperty::IDD,pParent);

}

void CDialogHFieldProperty::Present(HFIELD_CREATE_DESC* phfCreate,BOOL bShow)
{
	
	m_fLeft = phfCreate->left;
	m_fTop = phfCreate->top;
	
	m_fFaceSize = phfCreate->fFaceSize;
	m_dwFacesNumPerObjAxis = phfCreate->dwFacesNumPerObjAxis;
	m_dwObjNumX = phfCreate->dwObjNumX;
	m_dwObjNumZ = phfCreate->dwObjNumZ;
	m_dwDetailLevelNum = phfCreate->dwDetailLevelNum;
	m_dwIndexBufferNum = phfCreate->dwIndexBufferNumLV0;
	Calc();
	
	if (bShow)
	{
		UpdateData(FALSE);
	}
}

void CDialogHFieldProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogHFieldProperty)
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_fBottom);
	DDX_Text(pDX, IDC_EDIT_FACE_SIZE, m_fFaceSize);
	DDX_Text(pDX, IDC_EDIT_FACES_NUM_PER_OBJ, m_dwFacesNumPerObjAxis);
	DDV_MinMaxDWord(pDX, m_dwFacesNumPerObjAxis, 0, 1024);
	DDX_Text(pDX, IDC_EDIT_HFIELD_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_fLeft);
	DDV_MinMaxFloat(pDX, m_fLeft, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_fRight);
	DDX_Text(pDX, IDC_EDIT_TOP, m_fTop);
	DDV_MinMaxFloat(pDX, m_fTop, -900000.0f, 900000.0f);
	DDX_Text(pDX, IDC_EDIT_OBNNUM_X, m_dwObjNumX);
	DDV_MinMaxDWord(pDX, m_dwObjNumX, 0, 400);
	DDX_Text(pDX, IDC_EDIT_OBJNUM_Z, m_dwObjNumZ);
	DDV_MinMaxDWord(pDX, m_dwObjNumZ, 0, 400);
	DDX_Text(pDX, IDC_EDIT_LOD_NUM, m_dwDetailLevelNum);
	DDV_MinMaxDWord(pDX, m_dwDetailLevelNum, 0, 8);
	DDX_Text(pDX, IDC_EDIT_INDEX_BUFFER_NUM, m_dwIndexBufferNum);
	DDV_MinMaxDWord(pDX, m_dwIndexBufferNum, 4, 128);
	DDX_Text(pDX, IDC_EDIT_MIN_TILE_LENGTH, m_fMinTileLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogHFieldProperty, CDialog)
	//{{AFX_MSG_MAP(CDialogHFieldProperty)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_PROPERTY_OK, OnButtonHfieldPropertyOk)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_EX1, OnButtonHfieldEx1)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_EX2, OnButtonHfieldEx2)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_EX3, OnButtonHfieldEx3)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_EX4, OnButtonHfieldEx4)
	ON_EN_CHANGE(IDC_EDIT_OBNNUM_X, OnChangeEditObnnumX)
	ON_EN_CHANGE(IDC_EDIT_OBJNUM_Z, OnChangeEditObjnumZ)
	ON_EN_CHANGE(IDC_EDIT_FACE_SIZE, OnChangeEditFaceSize)
	ON_EN_CHANGE(IDC_EDIT_FACES_NUM_PER_OBJ, OnChangeEditFacesNumPerObj)
	ON_EN_CHANGE(IDC_EDIT_LEFT, OnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_TOP, OnChangeEditTop)
	ON_EN_CHANGE(IDC_EDIT_LOD_NUM, OnChangeEditLodNum)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_EX5, OnButtonHfieldEx5)
	ON_BN_CLICKED(IDC_BUTTON_HFIELD_TTB, OnButtonHfieldTtb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogHFieldProperty message handlers

void CDialogHFieldProperty::OnButtonHfieldPropertyOk() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left = m_fLeft;
	hfCreate.right = m_fRight;
	hfCreate.top = m_fTop;
	hfCreate.bottom = m_fBottom;
		
	hfCreate.fFaceSize = m_fFaceSize;
	
	hfCreate.dwFacesNumPerObjAxis = m_dwFacesNumPerObjAxis;
	hfCreate.dwObjNumX = m_dwObjNumX;
	hfCreate.dwObjNumZ = m_dwObjNumZ;
	hfCreate.dwDetailLevelNum = m_dwDetailLevelNum;
	hfCreate.dwIndexBufferNumLV0 = m_dwIndexBufferNum;
	g_pTool->SetHFieldCreateDesc(&hfCreate);
	EndDialog(0);
}
/*

	hfcreate.left = 
	hfcreate.right = 
	hfcreate.top = 
	hfcreate.bottom = 
	
	hfcreate.fFaceSize = 
	hfcreate.dwTileNumPerObjAxis = 
	hfcreate.dwFacesNumPerObjAxis = 
	hfcreate.dwTileSize = 
	hfcreate.dwLowDetailTextureSize = 
	hfcreate.dwLightTexelSize = 
	hfcreate.szHFieldName = "test";*/

void CDialogHFieldProperty::OnButtonHfieldEx1() 
{
	// TODO: Add your control notification handler code here
	
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left = -100000.0f;
	hfCreate.top = -100000.0f;
	
	hfCreate.fFaceSize = 500.0f;
	hfCreate.dwFacesNumPerObjAxis = 32;
	hfCreate.dwObjNumX = 20;
	hfCreate.dwObjNumZ = 20;
	hfCreate.dwDetailLevelNum = 3;
	hfCreate.dwIndexBufferNumLV0 = 8;


	Present(&hfCreate,TRUE);


}

void CDialogHFieldProperty::OnButtonHfieldEx2() 
{
	// TODO: Add your control notification handler code here
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left = -128000.0f;
	hfCreate.top = -128000.0f;
	
	hfCreate.fFaceSize = 100.0f;
	hfCreate.dwFacesNumPerObjAxis = 64;
	hfCreate.dwObjNumX = 40;
	hfCreate.dwObjNumZ = 40;
	hfCreate.dwDetailLevelNum = 4;
	hfCreate.dwIndexBufferNumLV0 = 8;

	Present(&hfCreate,TRUE);
}

void CDialogHFieldProperty::OnButtonHfieldEx3() 
{
	// TODO: Add your control notification handler code here
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left	= -128000.0f;
	hfCreate.top	= -128000.0f;
	

	hfCreate.fFaceSize = 100.0f;
	hfCreate.dwFacesNumPerObjAxis = 128;
	hfCreate.dwObjNumX = 20;
	hfCreate.dwObjNumZ = 20;
	hfCreate.dwDetailLevelNum = 4;
	hfCreate.dwIndexBufferNumLV0 = 8;
	
	Present(&hfCreate,TRUE);
}

void CDialogHFieldProperty::OnButtonHfieldEx4() 
{
	// TODO: Add your control notification handler code here
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left		= 0.0f;
	hfCreate.top		= 0.0f;


	hfCreate.fFaceSize	= 125.0f;
	hfCreate.dwFacesNumPerObjAxis = 32;
	hfCreate.dwObjNumX	= 6;
	hfCreate.dwObjNumZ	= 6;
	hfCreate.dwDetailLevelNum		= 1;
	hfCreate.dwIndexBufferNumLV0	= 16;

	Present(&hfCreate,TRUE);
}


void CDialogHFieldProperty::OnChangeEditObnnumX() 
{
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);

}

void CDialogHFieldProperty::OnChangeEditObjnumZ() 
{
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);
}


void CDialogHFieldProperty::OnChangeEditFaceSize() 
{
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);

}

void CDialogHFieldProperty::OnChangeEditFacesNumPerObj() 
{
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);
}

void CDialogHFieldProperty::Calc()
{
	

	m_fRight = m_fLeft + m_fFaceSize * (float)m_dwFacesNumPerObjAxis * (float)m_dwObjNumX;
	m_fBottom = m_fTop + m_fFaceSize * (float)m_dwFacesNumPerObjAxis * (float)m_dwObjNumZ;
		
	DWORD dwDetail = 1;
	for (DWORD i=1; i<m_dwDetailLevelNum; i++)
	{
		dwDetail *= 2;
	}
	m_fMinTileLength = m_fFaceSize * dwDetail;
	
}

void CDialogHFieldProperty::OnChangeEditLeft() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);
	
}


void CDialogHFieldProperty::OnChangeEditTop() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);
	
}

void CDialogHFieldProperty::OnChangeEditLodNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Calc();
	UpdateData(FALSE);
	
}


void CDialogHFieldProperty::OnButtonHfieldEx5() 
{
	// TODO: Add your control notification handler code here
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left		= 0.0f;
	hfCreate.top		= 0.0f;


	hfCreate.fFaceSize	= 100.0f;
	hfCreate.dwFacesNumPerObjAxis = 32;
	hfCreate.dwObjNumX	= 16;
	hfCreate.dwObjNumZ	= 16;
	hfCreate.dwDetailLevelNum		= 3;
	hfCreate.dwIndexBufferNumLV0	= 12;

	Present(&hfCreate,TRUE);
}

void CDialogHFieldProperty::OnButtonHfieldTtb() 
{
	HFIELD_CREATE_DESC	hfCreate;

	hfCreate.left		= 0.0f;
	hfCreate.top		= 0.0f;

	hfCreate.fFaceSize				= 50.0f;
	hfCreate.dwFacesNumPerObjAxis	= 32;
	hfCreate.dwObjNumX				= 32;
	hfCreate.dwObjNumZ				= 32;
	hfCreate.dwDetailLevelNum		= 1;
	hfCreate.dwIndexBufferNumLV0	= 12;

	Present(&hfCreate,TRUE);
}
