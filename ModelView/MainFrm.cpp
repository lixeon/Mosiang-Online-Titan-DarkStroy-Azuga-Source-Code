// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ModelView.h"
#include "MainFrm.h"
#include "EngineRelation.h"
#include "ToolInfo.h"
#include "SetFrameDlg.h"
#include "LightDlg.h"
#include "SpecularDlg.h"
#include "IllusionAttachDlg.h"
#include "BackgroundColorDlg.h"
#include "FrontOfCameraDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//// Constants //////////////////////////////////////////////////////////////

const char* const c_szFileNew			= "현재 기능을 실행하면 기존의 자료가 모두 사라집니다.\n계속하시겠습니까?";
const char* const c_szAreYouSure		= "정말 제거 하시겠습니까?";


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()	
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnModComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnAnmComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnMtlComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnWavComboChange)
	ON_COMMAND(ID_OPEN_MOD, OnOpenMod)
	ON_COMMAND(ID_OPEN_ANM, OnOpenAnm)
	ON_COMMAND(ID_CLOSE_ANM, OnCloseAnm)
	ON_COMMAND(ID_CLOSE_MOD, OnCloseMod)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_OEPN_CH_FILE, OnOepnChFile)
	ON_COMMAND(IDC_PLAY_BUTTON, OnPlayButton)
	ON_COMMAND(IDC_PLAY_SOUND_CHECK, OnPlaySoundCheck)
	ON_COMMAND(IDC_STOP_BUTTON, OnStopButton)
	ON_COMMAND(IDC_PLAY_SOUND_CHECK, OnPlaySoundButton)
	ON_COMMAND(ID_EXPORT_CHR, OnExportChr)	
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(IDC_RESET_CAMERA, OnResetCamera)
	ON_WM_DROPFILES()
	ON_COMMAND(IDC_ALL_SELECT, OnAllSelect)
	ON_COMMAND(IDC_SET_FRAME, OnSetFrame)
	ON_COMMAND(IDC_LIGHT_DIALOG, OnLightDialog)
	ON_COMMAND(ID_OPEN_MTL, OnOpenMtl)
	ON_COMMAND(ID_CLOSE_MTL, OnCloseMtl)
	ON_WM_QUERYENDSESSION()
	ON_WM_HSCROLL()
	ON_UPDATE_COMMAND_UI(IDC_GRID_DISPLAY, OnUpdateGridDisplay)	
	ON_COMMAND(IDC_GRID_DISPLAY, OnGridDisplay)
	ON_UPDATE_COMMAND_UI(IDC_BOUNDING_DRAW, OnUpdateBoundingDraw)
	ON_COMMAND(IDC_BOUNDING_DRAW, OnBoundingDraw)
	ON_COMMAND(IDC_BONE_BOUNDING_DRAW, OnBoneBoundingDraw)
	ON_UPDATE_COMMAND_UI(IDC_BONE_BOUNDING_DRAW, OnUpdateBoneBoundingDraw)
	ON_UPDATE_COMMAND_UI(IDM_WIREMODE , OnUpdateDrawWireFrame)
	ON_COMMAND(IDM_WIREMODE, OnWiremode)
	ON_COMMAND(ID_RENDER_CAMERA_INFO, OnRenderCameraInfo)
	ON_UPDATE_COMMAND_UI(ID_RENDER_CAMERA_INFO, OnUpdateRenderCameraInfo)
	ON_COMMAND(ID_OPEN_WAV, OnOpenWav)
	ON_COMMAND(ID_CLOSE_WAV, OnCloseWav)
	ON_COMMAND(ID_WAV_LIST_CLEAR, OnWavListClear)
	ON_COMMAND(ID_SPECULAR_SETTING, OnSpecularSetting)
	ON_COMMAND(IDM_ILLUMINATION_LOAD, OnIlluminationLoad)
	ON_COMMAND(IDM_ILLUMINATION_SETUP, OnIlluminationSetup)
	ON_COMMAND(IDM_ILLUMINATION_UNLOAD, OnIlluminationUnload)
	ON_COMMAND(IDM_SET_BACKGROUND_COLOR, OnSetBackgroundColor)
	ON_COMMAND(ID_VIEW_EFFECT_PALETTE, OnViewEffectPalette)
	ON_COMMAND(IDC_FRONT_OF_CAMERA, OnFrontOfCamera)	
	ON_COMMAND(IDM_BOTHMODE, OnBothmode)
	ON_UPDATE_COMMAND_UI(IDM_BOTHMODE, OnUpdateBothmode)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_FRONT_OF_CAMERA_DESTROY, OnFrontOfCameraDlgDestroy)
	ON_MESSAGE(WM_USER_LIGHT_DIALOG_DESTROY, OnLightDialogDestroy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create tool bar	

	m_wndToolBar.CreatNewToolBar(this);
	m_wndToolBar.UpdateToolBar();

	m_wndDialogBar.CreatNewDialogBar(this);
	m_wndDialogBar.UpdateDialogBar();
	g_pDlgBar = &m_wndDialogBar;

	DragAcceptFiles(TRUE);

	m_pWndIllusionSetupDlg	= NULL;
	m_pFrontOfCameradlg		= NULL;
	m_pLightDlg				= NULL;
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{	
	OnIlluminationUnload();

	CFrameWnd::OnClose();
}

void CMainFrame::OnModComboChange(void)
{	
	g_ToolInfo.SetCurrentAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());
	g_ToolInfo.SetCurrentModFile(m_wndToolBar.m_wndCombo1.GetCurSel());	
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}


void CMainFrame::OnAnmComboChange(void)
{
	g_ToolInfo.SetCurrentAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());	
	g_ToolInfo.SetCurrentModFile(m_wndToolBar.m_wndCombo1.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnMtlComboChange(void)
{
	g_ToolInfo.SetCurrentMtlFile(m_wndToolBar.m_wndCombo3.GetCurSel());
	g_ToolInfo.SetCurrentModFile(m_wndToolBar.m_wndCombo1.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnWavComboChange(void)
{
	g_ToolInfo.SetCurrentWavFile(m_wndToolBar.m_wndCombo4.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}


void CMainFrame::OnOpenMod() 
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}	


   // make file open dialog box.

	char szFilter[] = "Model Files(*.mod)|*.mod||";

	char* pFileName;
		
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

	// File name checking.

	CString strInputFileName = pFileName;

	if ("" == strInputFileName)
	{
		return;
	}

	// Second, To Add file information to the Tool information object.
	
	g_ToolInfo.AddModFile(pFileName);

	g_ToolInfo.SetCurrentAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());

	// Last, To Prepare object information for using Character viewer tool.

	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnOpenAnm()
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}


	// make file open dialog box.

	char szFilter[] = "Motion Files(*.anm)|*.anm||";

	char* pFileName;
		
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
		if ("" == strPathName)
		{
			return;
		}
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		return;
	}

	// Checking input file name.
	
	CString strInputFileName = pFileName;

	if ("" == strInputFileName)
	{
		return;
	}

	// Second Add file information to the Tool information object.	
	
	g_ToolInfo.AddAnmFile(pFileName);	

	// Last, To Prepare object information for using Character viewer tool.

	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnCloseAnm()
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}

	int iMessageBox = MessageBox(c_szAreYouSure, NULL, MB_YESNO);
	if (IDNO == iMessageBox)
	{
		return;
	}

	g_ToolInfo.RemoveAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();

}

void CMainFrame::OnCloseMod() 
{
	this->OnIlluminationUnload();

	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}

	int iMessageBox = MessageBox(c_szAreYouSure, NULL, MB_YESNO);
	if (IDNO == iMessageBox)
	{
		return;
	}

	g_ToolInfo.RemoveModFile(m_wndToolBar.m_wndCombo1.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnFileNew() 
{
	OnIlluminationUnload();	
	m_wndToolBar.ClearAnimationInfo();
	
	g_ToolInfo.RemoveFrontOfCameraObject(); // Remove Front camera object.
	
	if (NULL != m_pFrontOfCameradlg)
	{
		m_pFrontOfCameradlg->UpdateDialog();
	}
}

void CMainFrame::OnOepnChFile() 
{	
	// Show file open dialog box.	

	char szFilter[] = "Character file(*.chr, *.chx)|*.chr;*.chx||";

	char* pFileName;
		
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
		if ("" == strPathName)
		{
			return;
		}

		strPathName.MakeUpper();

		if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
		{
			return;
		}

		// Clear existing animation.

		m_wndToolBar.ClearAnimationInfo();


		// Chekcing input file name.

		CString strInputFileName = pFileName;

		if ("" == strInputFileName)
		{
			return;
		}
		
		// Judgment File class and Load Chracter file.

		ENUM_CH_FILE_TYPE FileType;		

		FileType = QueryChFileType(pFileName);

		if (CHR_FILE == FileType)
		{
			g_ToolInfo.AddChrFile(pFileName);
		}

		if (CHX_FILE == FileType)
		{
			g_ToolInfo.AddChxFile(pFileName);
		}

		if (NOT_CH_FILE == FileType)
		{
			return;
		}

		// Last, To Prepare object information for using Character viewer tool.

		m_wndToolBar.UpdateToolBar();
		m_wndDialogBar.UpdateDialogBar();
	}		
}

void CMainFrame::OnPlayButton() 
{
	// Animation!

	g_ToolInfo.m_bNowPlayed = TRUE;
	SetTimer(1, 50, NULL);
}

void CMainFrame::OnStopButton()
{
	// Stop!

	g_ToolInfo.m_bNowPlayed = FALSE;	
	KillTimer(1);
}

void CMainFrame::OnPlaySoundButton()
{
	if (0 >= g_ToolInfo.m_lstGXObject.GetCount())
	{
		return; 
	}
}

void CMainFrame::OnExportChr() 
{
	// File dialog.

	char szFilter[] = "Chr Files(*.chr)|*.chr|Chx Files(*.chx)|*chx|All Files(*.*)|*.*||"; 
    CFileDialog fileDlg(FALSE,
					    NULL,
						"NoName.chr",
						OFN_OVERWRITEPROMPT,
						szFilter); 

	if(IDOK == fileDlg.DoModal()) 
	{
		CString strExt = fileDlg.GetFileExt();

		ENUM_CH_FILE_TYPE type;
		strExt.MakeUpper();

		if (strExt == "CHR")
		{
			type = CHR_FILE;
		}
		else if (strExt == "CHX")
		{
			type = CHX_FILE;
		}
		else
		{
			type = NOT_CH_FILE;
		}
		
		char szFileName[1024];
		memset(szFileName, 0, 1024);

		CString strFileName = fileDlg.GetPathName();		
		
		memcpy(szFileName, (LPSTR)(LPCTSTR)strFileName, strFileName.GetLength());

		g_ToolInfo.CreateChFile(szFileName, type);
	}

	
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if (1 == nIDEvent)
	{
		m_wndDialogBar.SetScrollBar(g_ToolInfo.m_iScrollMin,
									g_ToolInfo.m_iScrollMax,
									g_ToolInfo.m_iScrollPos);

		m_wndDialogBar.SetFrameStatic();
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{

	if (0 < zDelta)
	{
		g_ToolInfo.m_Camera.ZoomInCamera();		
	}
	else
	{
		g_ToolInfo.m_Camera.ZoomOutCamera();		
	}

	//
	// Notice.
	// If window is scrolled, then return value is not Zero
	// Another way window is not scrolled, then return Zero.
	//
	// - Trust pak -

	return FALSE;
}

void CMainFrame::OnResetCamera() 
{
	g_ToolInfo.m_Camera.ResetCamera();
}



void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	int iCount = 0;

	char szFileExtension[512];
	char szFileName[MAX_PATH];
	char szDirectory[MAX_PATH];
	
	iCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, szFileName, _MAX_PATH);

	for (int i = 0; i < iCount ; i++)
	{
		memset(szDirectory, 0, MAX_PATH);

		DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
		GetFileExtension(szFileExtension, szFileName, 512);

		CString strFileExtension = szFileExtension;
		
		strFileExtension.MakeUpper();

		if (strFileExtension == "CHR")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddChrFile(szFileName);
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}
		
		if (strFileExtension == "CHX")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddChxFile(szFileName);
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}

		if (strFileExtension == "MOD")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddModFile(szFileName);
			g_ToolInfo.SetCurrentAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());	
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}

		if (strFileExtension == "ANM")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddAnmFile(szFileName);
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}

		if (strFileExtension == "MTL")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddMtlFile(szFileName);
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}

		if (strFileExtension == "WAV")
		{
			QueryDirectoryString(szDirectory, szFileName);
			::SetCurrentDirectory(szDirectory);

			g_ToolInfo.AddWavFile(szFileName);
			m_wndToolBar.UpdateToolBar();
			m_wndDialogBar.UpdateDialogBar();
		}

	}

	DragFinish(hDropInfo);
}

void CMainFrame::OnAllSelect() 
{
	if (0 >= g_ToolInfo.m_lstGXObject.GetCount())
	{
		return; 
	}

	if (FALSE == g_ToolInfo.m_bIsAllSelect)
	{
		m_wndToolBar.m_wndAllSelectButton.SetCheck(BST_CHECKED);
		g_ToolInfo.m_bIsAllSelect = TRUE;
	}
	else
	{
		m_wndToolBar.m_wndAllSelectButton.SetCheck(BST_UNCHECKED);
		g_ToolInfo.m_bIsAllSelect = FALSE;
	}

	g_ToolInfo.SetCurrentModFile(g_ToolInfo.m_iCurrentModNo);	
	g_ToolInfo.SetCurrentAnmFile(m_wndToolBar.m_wndCombo2.GetCurSel());
	g_ToolInfo.SetCurrentMtlFile(m_wndToolBar.m_wndCombo3.GetCurSel());
}

void CMainFrame::OnSetFrame() 
{
	CSetFrameDlg dlg;

	dlg.m_iFps = g_ToolInfo.m_dwFps;

	if (IDOK == dlg.DoModal())
	{
		g_ToolInfo.m_dwFps = dlg.m_iFps;		
		SetFramePerSecond(g_ToolInfo.m_dwFps);
	}
}

void CMainFrame::OnLightDialog() 
{
	if (NULL != m_pLightDlg)
	{
		m_pLightDlg->SetFocus();
	}
	else
	{
		m_pLightDlg = new CLightDlg;
		m_pLightDlg->Create(IDD_LIGHT_DIALOG);

		m_pLightDlg->m_byAmbientR = g_ToolInfo.m_abyAmbient[0];
		m_pLightDlg->m_byAmbientG = g_ToolInfo.m_abyAmbient[1];
		m_pLightDlg->m_byAmbientB = g_ToolInfo.m_abyAmbient[2];

		m_pLightDlg->m_byDiffuseR = g_ToolInfo.m_abyDiffuse[0];
		m_pLightDlg->m_byDiffuseG = g_ToolInfo.m_abyDiffuse[1];
		m_pLightDlg->m_byDiffuseB = g_ToolInfo.m_abyDiffuse[2];

		m_pLightDlg->m_fDirectionX = g_ToolInfo.m_afDirection[0];
		m_pLightDlg->m_fDirectionY = g_ToolInfo.m_afDirection[1];
		m_pLightDlg->m_fDirectionZ = g_ToolInfo.m_afDirection[2];

		m_pLightDlg->m_bLightOn = g_ToolInfo.m_bOnLight;			

		m_pLightDlg->UpdateData(FALSE);
		m_pLightDlg->ShowWindow(TRUE);
	}

	/*
	CLightDlg dlg;
	
	dlg.m_byAmbientR = g_ToolInfo.m_abyAmbient[0];
	dlg.m_byAmbientG = g_ToolInfo.m_abyAmbient[1];
	dlg.m_byAmbientB = g_ToolInfo.m_abyAmbient[2];

	dlg.m_byDiffuseR = g_ToolInfo.m_abyDiffuse[0];
	dlg.m_byDiffuseG = g_ToolInfo.m_abyDiffuse[1];
	dlg.m_byDiffuseB = g_ToolInfo.m_abyDiffuse[2];

	dlg.m_fDirectionX = g_ToolInfo.m_afDirection[0];
	dlg.m_fDirectionY = g_ToolInfo.m_afDirection[1];
	dlg.m_fDirectionZ = g_ToolInfo.m_afDirection[2];

	dlg.m_bLightOn = g_ToolInfo.m_bOnLight;	

	if (IDOK == dlg.DoModal())
	{
		g_ToolInfo.m_abyAmbient[0] = dlg.m_byAmbientR;
		g_ToolInfo.m_abyAmbient[1] = dlg.m_byAmbientG;
		g_ToolInfo.m_abyAmbient[2] = dlg.m_byAmbientB;

		g_ToolInfo.m_abyDiffuse[0] = dlg.m_byDiffuseR;
		g_ToolInfo.m_abyDiffuse[1] = dlg.m_byDiffuseG;
		g_ToolInfo.m_abyDiffuse[2] = dlg.m_byDiffuseB;

		g_ToolInfo.m_afDirection[0] = dlg.m_fDirectionX;
		g_ToolInfo.m_afDirection[1] = dlg.m_fDirectionY;
		g_ToolInfo.m_afDirection[2] = dlg.m_fDirectionZ;

		g_ToolInfo.m_bOnLight = dlg.m_bLightOn;

		
		UpdateLightInformation(g_ToolInfo.m_Camera.GetViewPortNo());
	}
	*/
}

void CMainFrame::OnOpenMtl() 
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}


	// make file open dialog box.

	char szFilter[] = "Material Files(*.mtl)|*.mtl||";

	char* pFileName;
		
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
		if ("" == strPathName)
		{
			return;
		}
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		return;
	}

	// Checking input file name.
	
	CString strInputFileName = pFileName;

	if ("" == strInputFileName)
	{
		return;
	}

	// Second Add file information to the Tool information object.		

	g_ToolInfo.AddMtlFile(pFileName);

	// Last, To Prepare object information for using Character viewer tool.

	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnCloseMtl() 
{
	if (TRUE == g_ToolInfo.m_bIsChFile)
	{
		if (IDNO == MessageBox(c_szFileNew, NULL, MB_YESNO))
		{
			return;
		}		

		m_wndToolBar.ClearAnimationInfo();
	}

	int iMessageBox = MessageBox(c_szAreYouSure, NULL, MB_YESNO);
	if (IDNO == iMessageBox)
	{
		return;
	}
	
	g_ToolInfo.RemoveMtlFile(m_wndToolBar.m_wndCombo3.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	// TODO: Add your specialized query end session code here	
	
	return TRUE;
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnUpdateGridDisplay(CCmdUI* pCmdUI) 
{
	if (FALSE == g_ToolInfo.m_bIsGrid)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (TRUE == g_ToolInfo.m_bDisplayGrid)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnGridDisplay() 
{
	if (TRUE == g_ToolInfo.m_bDisplayGrid)		// Checked.
	{
		g_ToolInfo.DisplayGrid(FALSE);
	}
	else
	{
		g_ToolInfo.DisplayGrid(TRUE);
	}
}

void CMainFrame::OnUpdateBoundingDraw(CCmdUI* pCmdUI) 
{
	if (FALSE == g_ToolInfo.m_bDisplayBoundingBox)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnBoundingDraw() 
{
	if (FALSE == g_ToolInfo.m_bDisplayBoundingBox)
	{
		g_ToolInfo.DisplayBoundingBox(TRUE);
	}
	else
	{
		g_ToolInfo.DisplayBoundingBox(FALSE);
	}
}


void CMainFrame::OnBoneBoundingDraw() 
{
	// TODO: Add your command handler code here
	if (FALSE == g_ToolInfo.m_bDisplayBoneBoundingBox)
	{
		g_ToolInfo.DisplayBoneBoundingBox(TRUE);
	}
	else
	{
		g_ToolInfo.DisplayBoneBoundingBox(FALSE);
	}
}

void CMainFrame::OnUpdateBoneBoundingDraw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (FALSE == g_ToolInfo.m_bDisplayBoneBoundingBox)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnUpdateDrawWireFrame(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_ToolInfo.m_bWireMode)
	{
		pCmdUI->SetCheck(1);		
	}
	else
	{
		pCmdUI->SetCheck(0);		
	}
}


void CMainFrame::OnWiremode() 
{
	// TODO: Add your command handler code here
	if (g_ToolInfo.m_bWireMode)
	{
		g_pExecutive->GetRenderer()->SetRenderMode(RENDER_MODE_SOLID);
		g_ToolInfo.m_bWireMode = FALSE;
	}
	else
	{	
		g_pExecutive->GetRenderer()->SetRenderMode(RENDER_MODE_WIREFRAME);		
		g_ToolInfo.m_bWireMode = TRUE;		
	}
	
}



void CMainFrame::OnUpdateBothmode(CCmdUI* pCmdUI) 
{
	BOOL bBothMode = g_pExecutive->GetRenderer()->GetRenderWireSolidBothMode();

	if (TRUE == bBothMode)
	{
		pCmdUI->SetCheck(1);		
	}
	else
	{
		pCmdUI->SetCheck(0);		
	}	
	
}



void CMainFrame::OnBothmode() 
{
	BOOL bBothMode = g_pExecutive->GetRenderer()->GetRenderWireSolidBothMode();

	if (TRUE == bBothMode)
	{		
		g_pExecutive->GetRenderer()->SetRenderWireSolidBothMode(FALSE);	
	}
	else
	{
		g_pExecutive->GetRenderer()->SetRenderWireSolidBothMode(TRUE);	
	}	
}



void CMainFrame::OnRenderCameraInfo() 
{
	if (TRUE == g_ToolInfo.m_bRenderCameraInfo)
	{
		g_ToolInfo.m_bRenderCameraInfo = FALSE;
	}
	else
	{
		g_ToolInfo.m_bRenderCameraInfo = TRUE;
	}
}

void CMainFrame::OnUpdateRenderCameraInfo(CCmdUI* pCmdUI) 
{
	if (FALSE == g_ToolInfo.m_bRenderCameraInfo)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}


void CMainFrame::OnOpenWav() 
{
	// make file open dialog box.

	char szFilter[] = "Wav Files(*.wav)|*.wav||";

	char* pFileName;
		
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
		if ("" == strPathName)
		{
			return;
		}
	}

	if (!(pFileName = (LPSTR)(LPCSTR)strPathName))
	{
		return;
	}

	// Checking input file name.
	
	CString strInputFileName = pFileName;

	if ("" == strInputFileName)
	{
		return;
	}

	// Second Add file information to the Tool information object.			

	g_ToolInfo.AddWavFile(pFileName);

	// Last, To Prepare object information for using Character viewer tool.

	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}


void CMainFrame::OnCloseWav() 
{
	int iMessageBox = MessageBox(c_szAreYouSure, NULL, MB_YESNO);
	if (IDNO == iMessageBox)
	{
		return;
	}
	
	g_ToolInfo.RemoveWavFile(m_wndToolBar.m_wndCombo4.GetCurSel());
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}



void CMainFrame::OnPlaySoundCheck()
{
	if (FALSE == g_ToolInfo.m_bPlaySound)
	{
		g_ToolInfo.m_bPlaySound = TRUE;
		m_wndDialogBar.SetPlaySoundButton(g_ToolInfo.m_bPlaySound);
	}
	else
	{
		g_ToolInfo.m_bPlaySound = FALSE;
		m_wndDialogBar.SetPlaySoundButton(g_ToolInfo.m_bPlaySound);
	}
}

void CMainFrame::OnWavListClear() 
{
	g_ToolInfo.ClearAudioInformation();	
	m_wndToolBar.UpdateToolBar();
	m_wndDialogBar.UpdateDialogBar();
}

void CMainFrame::OnSpecularSetting() 
{
	CSpecularDlg dlg;

	dlg.m_bSpecularEnable = g_ToolInfo.m_bSpecularEnable;
	dlg.m_fSpecularIntensity  = g_ToolInfo.m_fSpecularIntensity;

	if (IDOK == dlg.DoModal())
	{
		g_ToolInfo.m_bSpecularEnable = dlg.m_bSpecularEnable;
		g_ToolInfo.m_fSpecularIntensity = dlg.m_fSpecularIntensity;
		
		UpdateSpecularEnable(g_ToolInfo.m_bSpecularEnable, g_ToolInfo.m_fSpecularIntensity);
	}

}

// 잔상 메뉴 핸들러.
void CMainFrame::OnIlluminationLoad() 
{
	// TODO: Add your command handler code here
//	BOOL	bResult	=	g_ToolInfo.OnIlluminationObjectLoad();
//	char*	temp	=	new		char[32768];



	OnIlluminationUnload();			// 기존 것들 날리고.

	int		iGXObjectCount;
	//	chx 모드인가 mod 모드인가.?
	iGXObjectCount	=	g_ToolInfo.m_iTotalGxObject;

	
	if( g_ToolInfo.m_iTotalModNo <= 0 && g_ToolInfo.m_bIsChFile == FALSE)
	{
		// 띄울 다이얼로그는 미리 로드되어 있는 mod내의 오브젝트를 찾는 것을 목적으로 한다.
		// 미리 로드되어있는 것이 없으면 그저 리턴 할 뿐이다.
		MessageBox( "어태치 할 대상이 되는 mod가 없습니다. 미리 mod나 chr, chx를 로드 해 주세요.", "에러", MB_OK);
		return;
	}
	else
	{
		CIllusionAttachDlg	dlgAttach;
		int	iResult	=	dlgAttach.DoModal();

		if( iResult == 1)
		{
			m_pWndIllusionSetupDlg	=	new	CIllusionSetupDlg;
			m_pWndIllusionSetupDlg->Create( IDD_ILLUSION_SETUP, this);
			m_pWndIllusionSetupDlg->ShowWindow(SW_SHOW);
		}


	}

	
	
}

// 잔상 메뉴 핸들러.
void CMainFrame::OnIlluminationSetup() 
{
	// TODO: Add your command handler code here
//	g_ToolInfo.SetIllusionMeshStatus( 10, 0, TRUE);
	if( m_pWndIllusionSetupDlg)
	{
		m_pWndIllusionSetupDlg->ShowWindow( SW_SHOW);
		g_ToolInfo.ApplyIllusionMeshStatus( 15, 0, TRUE);
	}
}

// 잔상 메뉴 핸들러.
void CMainFrame::OnIlluminationUnload() 
{
	// TODO: Add your command handler code here
	// 잔상이 올라온 적이 없다고 가정하고 바로 리턴.
	if( m_pWndIllusionSetupDlg)
	{
		delete	m_pWndIllusionSetupDlg;
		m_pWndIllusionSetupDlg	=	0;
		BOOL	bResult	=	g_ToolInfo.OnUnloadIllusionMesh();
		if( bResult == FALSE)
			_asm int 3;
	}	
}

void CMainFrame::OnSetBackgroundColor() 
{
	// TODO: Add your command handler code here
	CBackgroundColorDlg		dlg;
	dlg.DoModal();

}

void CMainFrame::OnViewEffectPalette() 
{
	// TODO: Add your command handler code here
		// TODO: Add your command handler code here
	BOOL bResult = g_ToolInfo.ViewEffectPalette();
	
	CMenu*	pMenu = GetMenu();
	if (bResult)
		pMenu->CheckMenuItem(ID_VIEW_EFFECT_PALETTE,MF_CHECKED);
	else
		pMenu->CheckMenuItem(ID_VIEW_EFFECT_PALETTE,MF_UNCHECKED);

	
}

void CMainFrame::OnFrontOfCamera() 
{
	//
	// If the dialog box already exists, dispaly it.
	//

	if (NULL != m_pFrontOfCameradlg)
	{
		m_pFrontOfCameradlg->SetFocus();
	}
	else
	{
		//
		// Create Front of Camera Dialog box modeles.
		//

		m_pFrontOfCameradlg = new CFrontOfCameraDlg;
		m_pFrontOfCameradlg->Create(IDD_DIALOG_FRONT_OF_CAMERA);
		m_pFrontOfCameradlg->SetGXObjectFileName(g_ToolInfo.m_szFrontOfCameraName);
		m_pFrontOfCameradlg->UpdateDialog();

		m_pFrontOfCameradlg->ShowWindow(SW_SHOW);
	}
}


LRESULT CMainFrame::OnFrontOfCameraDlgDestroy(WPARAM,LPARAM)
{
	m_pFrontOfCameradlg = NULL;
	return 0;
}


LRESULT CMainFrame::OnLightDialogDestroy(WPARAM,LPARAM)
{
	m_pLightDlg = NULL;
	return 0;
}


