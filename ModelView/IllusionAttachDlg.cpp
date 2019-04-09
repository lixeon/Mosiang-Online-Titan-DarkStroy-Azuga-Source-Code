// IllusionAttachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "IllusionAttachDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIllusionAttachDlg dialog


CIllusionAttachDlg::CIllusionAttachDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIllusionAttachDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIllusionAttachDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CIllusionAttachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIllusionAttachDlg)
	DDX_Control(pDX, IDC_ILLUSION_OBJECT_LIST, m_ObjectListBox);
	DDX_Control(pDX, IDC_ILLUSION_MOD_LIST, m_ModListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIllusionAttachDlg, CDialog)
	//{{AFX_MSG_MAP(CIllusionAttachDlg)
	ON_LBN_SELCHANGE(IDC_ILLUSION_MOD_LIST, OnSelchangeIllusionModList)
	ON_LBN_SELCHANGE(IDC_ILLUSION_OBJECT_LIST, OnSelchangeIllusionObjectList)
	ON_BN_CLICKED(ID_ILLUMINATION_LOAD_OK, OnIlluminationLoadOk)
	ON_BN_CLICKED(ID_ILLUMINATION_DLG_CANCEL, OnIlluminationDlgCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIllusionAttachDlg message handlers

BOOL CIllusionAttachDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	int		iGXObjectCount;
	//	chx 모드인가 mod 모드인가.?
//	if( g_ToolInfo.m_bIsChFile)
	{
//		iGXObjectCount	=	g_ToolInfo.m_iTotalGxObject;
	}


	// 다이얼로그가 뜰 때 g_ToolInfo에서 mod 리스트를 받아온다.
	int		iModCount	=	g_ToolInfo.m_lstModFileName.GetCount();
	// chr이나 chx일 때도 이 modlist는 정상적이다.

	if( iModCount <= 0)
	{
		_asm int 3
	}

	CString		strFileName;
	char		szFileName[MAX_PATH];
	BOOL		bResult;

	// mod 리스트를 얻어와서 MOD리스트박스에 올린다.
	POSITION		pos	=	g_ToolInfo.m_lstModFileName.GetHeadPosition();

	while( pos)
	{
		strFileName = g_ToolInfo.m_lstModFileName.GetNext(pos);
		bResult	=	GetPureFileName( szFileName, (LPSTR)(LPCTSTR)strFileName, MAX_PATH);
		if( bResult == FALSE)
		{
			_asm int 3;
		}

		m_ModListBox.AddString( szFileName);

	}

	m_gxhAttachTarget	=	0;
	m_pModel			=	0;
	memset( m_szObjectName, 0, sizeof(char)*1024);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIllusionAttachDlg::OnSelchangeIllusionModList() 
{
	// TODO: Add your control notification handler code here
	int		iSelected	=	m_ModListBox.GetCurSel ();
	if( LB_ERR == iSelected)
	{
		// 이런 경우가 생길 수 있나.?
		_asm int 3;
	}
	m_iSelectedModelIndex = iSelected;

	// ObjList 리셋 하고,
	m_ObjectListBox.ResetContent();

	// iSelected 로 선택된 model을 받아온다.
	GXOBJECT_HANDLE	gxh;
	DWORD			dwModReffIndex;
	I3DModel*		pModel;
	DWORD			i;
	DWORD			dwObjCount;
	char*			szObjName;
	if( g_ToolInfo.m_bIsChFile)					// chr, chx파일일 경우 gxh는 하나밖에 없으므로, 헤드를 받는다.
	{
		gxh = g_ToolInfo.m_lstGXObject.GetHead();
		// mod 내의 오브젝트 리스트를 얻는다.
		g_pExecutive->GXOGetModel( gxh, &pModel, &dwModReffIndex, iSelected);
	}
	else		// mod가 열거된 형태일 경우, 그중 하나를 받는다.
	{
		POSITION	pos =	g_ToolInfo.m_lstGXObject.FindIndex(iSelected);
		gxh				=	g_ToolInfo.m_lstGXObject.GetAt(pos);
		g_pExecutive->GXOGetModel( gxh, &pModel, &dwModReffIndex, 0);
	}

	// gxobject
	m_gxhAttachTarget	=	gxh;

	// 얻어온 모델을 돌면서 이름을 리스트박스에 등록한다.
	dwObjCount	=	pModel->GetObjectNum();
	for( i = 0; i < dwObjCount; i++)
	{
		szObjName	=	pModel->GetObjectName( i);
		m_ObjectListBox.AddString( szObjName);
	}
	m_pModel	=	pModel;

	_asm nop;



/*	GXOBJECT_HANDLE		gxhSelected	=	g_ToolInfo.GetNThMod( iSelected);


		POSITION pos = m_lstGXObject.FindIndex(iCount);
		GXOBJECT_HANDLE gxh = m_lstGXObject.GetAt(pos);


	g_ToolInfo.m_lstGXObject.GetHead()

		GXOBJECT_HANDLE gxh = m_lstGXObject.GetHead();
*/

/*
	// 선택된 mod내의 오브젝트를 얻어서 object list box에다 넣는다.
	GXOBJECT_HANDLE		hSelected	=	this->m_pModList[iSelected];

	g_ToolInfo.m_
	virtual ULONG				__stdcall	GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex) = 0;// Release()필요


	m_ObjectListBox.ResetContent();
*/


	_asm nop
	
}

void CIllusionAttachDlg::OnSelchangeIllusionObjectList() 
{
	// TODO: Add your control notification handler code here
	// 여기선 별로 하는게 없다.
}

void CIllusionAttachDlg::OnIlluminationLoadOk() 
{
	// TODO: Add your control notification handler code here
	int		sel	=	m_ObjectListBox.GetCurSel();
	if( sel == LB_ERR)
	{
		// 만약 아무것도 안고른 상태에서 load가 들어온 경우,
		MessageBox("잔상용 MOD를 어태치할 오브젝트를 골라주세요.", "Object Select Error", MB_OK);
		return	;
	}

	CString		strObj;
	m_ObjectListBox.GetText( sel, strObj);


	// 이까지 오면 골라져있는 상태이므로 그 오브젝트의 이름을 가져온다.
	char* szObjName	=	m_pModel->GetObjectName( sel);
	lstrcpy( m_szObjectName, szObjName);


	// 오픈 다이얼로그를 띄운다.
	// 성희 코드에서 배껴옴.
	char szFilter[] = "Model Files(*.mod)|*.mod||";

	char* pFileName;
		
	CString strPathName;
	CFileDialog dlg(TRUE,
				    NULL,
					NULL,
					OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,
					szFilter,
					NULL);
	
	int		iReturn	=	dlg.DoModal();

	if (iReturn == IDOK)
	{
		strPathName = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (strPathName == "")
		{
			_asm int 3;
//			return;
			OnIlluminationDlgCancel();
			return	;
		}
	}
	else if( iReturn == IDCANCEL)
	{
		OnIlluminationDlgCancel();
		return	;
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		_asm int 3;
		OnIlluminationDlgCancel();
	}

	// 모델 포인터, 오브젝트 이름, 붙일 오브젝트 이름 다 나왔다.
	g_ToolInfo.OnLoadIllusionMesh( m_gxhAttachTarget,m_iSelectedModelIndex, szObjName, pFileName);


	OnIlluminationDlgCancel();

	EndDialog(1);
}

void CIllusionAttachDlg::OnIlluminationDlgCancel() 
{
	// TODO: Add your control notification handler code here
	if( m_pModel)
	{
		m_pModel->Release();
	}
	this->EndDialog(0);
}
