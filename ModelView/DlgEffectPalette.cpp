// DlgEffectPalette.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "DlgEffectPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "DlgCreateEffect.h"
#include "engineRelation.h"
#include "ToolInfo.h"
CDlgEffectPalette*	g_pDlgEffectPalette = NULL;

void DelEffectFunc(void* pVoid)
{
	delete (CUSTOM_EFFECT_DESC*)pVoid;
}


/////////////////////////////////////////////////////////////////////////////
// CDlgEffectPalette dialog


CDlgEffectPalette::CDlgEffectPalette(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEffectPalette::IDD, pParent)
{

	//{{AFX_DATA_INIT(CDlgEffectPalette)
	m_bEnableTexWave = FALSE;
	m_bDisableSrcTex = FALSE;
	m_strTexName = _T("");
	//}}AFX_DATA_INIT
	Create(CDlgEffectPalette::IDD,pParent);

}


void CDlgEffectPalette::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEffectPalette)
	DDX_Control(pDX, IDC_LIST_EFFECT, m_lbEffectList);
	DDX_Check(pDX, IDC_CHECK_TEXGEN_WAVE, m_bEnableTexWave);
	DDX_Check(pDX, IDC_CHECK_DISABLE_SRCTEX, m_bDisableSrcTex);
	DDX_Text(pDX, IDC_EDIT_TEX_NAME, m_strTexName);
	DDV_MaxChars(pDX, m_strTexName, 260);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEffectPalette, CDialog)
	//{{AFX_MSG_MAP(CDlgEffectPalette)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_CREATE, OnButtonEffectCreate)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_DELETE, OnButtonEffectDelete)
	ON_LBN_SELCHANGE(IDC_LIST_EFFECT, OnSelchangeListEffect)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_EFFECT_PALETTE, OnButtonUpdateEffectPalette)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_APPLY, OnButtonEffectApply)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TEXFILE, OnButtonBrowseTexfile)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_SAVE, OnButtonEffectSave)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_LOAD, OnButtonEffectLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEffectPalette message handlers

void CDlgEffectPalette::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);
	
}
void CDlgEffectPalette::DisableView()
{
	ShowWindow(SW_HIDE);
	
}

BOOL CDlgEffectPalette::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lbcEffectList.Initialize(MAX_EFFECT_NUM,128,&m_lbEffectList,DelEffectFunc);
//	BOOL CListBoxContainer::Initialize(DWORD dwMaxItemNum,DWORD dwMaxSize,CListBox* pListBox,DelFunc pDelFunc)

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEffectPalette::OnButtonEffectCreate() 
{
	// TODO: Add your control notification handler code here
	CDlgCreateEffect	dlgCreate;
	char			szEffectName[64];
	memset(szEffectName,0,sizeof(szEffectName));

	dlgCreate.SetEffectNameResulPtr(szEffectName);
	dlgCreate.DoModal();

	if (!lstrlen(szEffectName))
		return;

	CUSTOM_EFFECT_DESC*		pEffect = new CUSTOM_EFFECT_DESC;
	memset(pEffect,0,sizeof(CUSTOM_EFFECT_DESC));

	lstrcpy(pEffect->szEffectShaderName,szEffectName);
	m_lbcEffectList.InsertItem(szEffectName,pEffect);

	int	iIndex;
	CUSTOM_EFFECT_DESC*	pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetSelectedItem(&iIndex);
	m_iSelectedEffectIndex = iIndex;
	g_ToolInfo.UpdateEffectIndex();


}


void CDlgEffectPalette::OnButtonEffectDelete() 
{
	// TODO: Add your control notification handler code here
	m_lbcEffectList.DeleteSelectedItem();

	int	iIndex;
	CUSTOM_EFFECT_DESC*	pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetSelectedItem(&iIndex);
	m_iSelectedEffectIndex = iIndex;
	g_ToolInfo.UpdateEffectIndex();


}

void CDlgEffectPalette::OnSelchangeListEffect() 
{
	// TODO: Add your control notification handler code here
	int	iIndex;
	CUSTOM_EFFECT_DESC*	pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetSelectedItem(&iIndex);
	m_iSelectedEffectIndex = iIndex;

	if (!pEffectDesc)
		return;

	if (pEffectDesc->method == TEXGEN_METHOD_WAVE)
		m_bEnableTexWave = TRUE;
	else 
		m_bEnableTexWave = FALSE;

	m_bDisableSrcTex = pEffectDesc->bDisableSrcTex;
	m_strTexName = (LPCSTR)pEffectDesc->szTexName;
	
	UpdateData(FALSE);
	g_ToolInfo.UpdateEffectIndex();


}

void CDlgEffectPalette::UpdateEffectPalette()
{
	CUSTOM_EFFECT_DESC	effectDesc[MAX_EFFECT_NUM];
	memset(effectDesc,0,sizeof(effectDesc));

	CUSTOM_EFFECT_DESC*	pEffectDesc;
	DWORD	dwNum = m_lbcEffectList.GetItemNum();

	for (DWORD i=0; i<dwNum ; i++)
	{
		pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetItem(i);
		effectDesc[i] = *pEffectDesc;
	}
	g_pExecutive->GetRenderer()->CreateEffectShaderPalette(effectDesc,dwNum);
	g_ToolInfo.UpdateEffectIndex();

}

void CDlgEffectPalette::OnButtonUpdateEffectPalette() 
{
	// TODO: Add your control notification handler code here
	UpdateEffectPalette();
	
}

void CDlgEffectPalette::OnButtonEffectApply() 
{
	// TODO: Add your control notification handler code here
	int	iIndex;
	CUSTOM_EFFECT_DESC*	pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetSelectedItem(&iIndex);
	if (!pEffectDesc)
		return;

	UpdateData(TRUE);

	pEffectDesc->bDisableSrcTex = m_bDisableSrcTex;
	if (m_bEnableTexWave)
		pEffectDesc->method = TEXGEN_METHOD_WAVE;
	else
		pEffectDesc->method = TEXGEN_METHOD_REFLECT_SPHEREMAP;

	LPCSTR	pszTexName = (LPCSTR)m_strTexName;

	lstrcpy(pEffectDesc->szTexName,pszTexName);
}

void CDlgEffectPalette::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_lbcEffectList.DeleteAll();
	
}


void CDlgEffectPalette::OnButtonBrowseTexfile() 
{
	
	char szFilters[] = "Targa Files(*.tga)|*.tga|Tiff Files(*.tif) |*.tif||";
	
	char* filename = NULL;
		
	CString pathname;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,szFilters, NULL); 
	
	
	dlg.DoModal();	
	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return;


	if (!filename)
		return;

	m_strTexName = (LPCSTR)filename;
	UpdateData(FALSE);



}

void CDlgEffectPalette::OnButtonEffectSave() 
{
	// TODO: Add your control notification handler code here
	
	char szFilters[] = "EffectShader Files (*.efs) | *.efs | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"efs","*.efs",NULL,szFilters,AfxGetMainWnd());


	
	if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			return;
	}

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return;

	if (!lstrlen(filename))
		return;

	
	FILE*	fp = fopen(filename,"wb");
	
	DWORD	dwVersion = 0x00000001;
	DWORD	dwEffectNum = m_lbcEffectList.GetItemNum();
	DWORD	dwTerminate = 0;


	CUSTOM_EFFECT_DESC*	pEffectTable = new 	CUSTOM_EFFECT_DESC[dwEffectNum];
	memset(pEffectTable,0,sizeof(CUSTOM_EFFECT_DESC)*dwEffectNum);

	CUSTOM_EFFECT_DESC*	pEffectDesc;
	for (DWORD i=0; i<dwEffectNum ; i++)
	{
		pEffectDesc = (CUSTOM_EFFECT_DESC*)m_lbcEffectList.GetItem(i);
		pEffectTable[i] = *pEffectDesc;
	}
	fwrite(&dwVersion,sizeof(DWORD),1,fp);
	fwrite(&dwEffectNum,sizeof(DWORD),1,fp);
	fwrite(pEffectTable,sizeof(CUSTOM_EFFECT_DESC),dwEffectNum,fp);
	fwrite(&dwTerminate,sizeof(DWORD),1,fp);


	fclose(fp);
	

	char	szHeaderFileName[_MAX_PATH];
	RemoveExt(szHeaderFileName,filename);
	lstrcat(szHeaderFileName,".h");

	fp = fopen(szHeaderFileName,"wt");

	
	for (i=0; i<dwEffectNum; i++)
	{
		fprintf(fp,"%s \t%s%s \t %d \n","#define","EFS_",pEffectTable[i].szEffectShaderName,i);
	}
	fclose(fp);

	if (pEffectTable)
		delete pEffectTable;

}



void CDlgEffectPalette::OnButtonEffectLoad() 
{
	// TODO: Add your control notification handler code here
	char szFilter[] = "Model Files(*.efs)|*.efs||";

	char* pFileName = NULL;
		
	CString strPathName;
	CFileDialog dlg(TRUE,
				    NULL,
					NULL,
					OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,
					szFilter,
					NULL);
	
	if (dlg.DoModal() == IDOK)
	{
		strPathName = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (strPathName == "")
		{
			return;
		}
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		return;
	}


	FILE*	fp = fopen(pFileName,"rb");
	if (!fp)
		return;

	DWORD	dwVersion;
	DWORD	dwEffectNum;

	fread(&dwVersion,sizeof(DWORD),1,fp);
	fread(&dwEffectNum,sizeof(DWORD),1,fp);

	if (dwEffectNum >= MAX_EFFECT_SHADER_NUM)
	{

		goto lb_close;
	}

	CUSTOM_EFFECT_DESC*	pEffectTable;
	pEffectTable = new CUSTOM_EFFECT_DESC[dwEffectNum];
	memset(pEffectTable,0,sizeof(CUSTOM_EFFECT_DESC)*dwEffectNum);

	fread(pEffectTable,sizeof(CUSTOM_EFFECT_DESC),dwEffectNum,fp);


	m_lbcEffectList.DeleteAll();

	CUSTOM_EFFECT_DESC*	pEffectDesc;
	
	DWORD	i;
	for (i=0; i<dwEffectNum; i++)
	{
		pEffectDesc = new CUSTOM_EFFECT_DESC;
		*pEffectDesc = pEffectTable[i];
		m_lbcEffectList.InsertItem(pEffectDesc->szEffectShaderName,pEffectDesc);
	}
	if (pEffectTable)
		delete pEffectTable;

	m_iSelectedEffectIndex = 0xffffffff;
	UpdateEffectPalette();
	
lb_close:
	fclose(fp);

}
