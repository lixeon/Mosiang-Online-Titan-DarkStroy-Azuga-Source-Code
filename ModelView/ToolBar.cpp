//
//	Custom Tool bar inherited CToolBar (MFC)
//
//				2002.12.12
//				Trust Pak


#include "StdAfx.h"
#include "Resource.h"
#include "ToolBar.h"
#include "ToolInfo.h"






//////////////////////////////////////////////////////////////////////////////
// Class Name : CNewToolBar
// Description :
//
//----------------------------------------------------------------------------
// Name : CNewToolBar::CNewToolBar()
// Description : Contructor.
//----------------------------------------------------------------------------
CNewToolBar::CNewToolBar()	
{
}

//----------------------------------------------------------------------------
// Name : CNewToolBar::CNewToolBar()
// Description : Disconstructor
//----------------------------------------------------------------------------
CNewToolBar::~CNewToolBar()
{
}

//----------------------------------------------------------------------------
// Name : CNewToolBar::CNewToolBar()
// Description : Disconstructor
//----------------------------------------------------------------------------
BOOL CNewToolBar::CreatNewToolBar(CWnd* pWnd)
{	
	// First, Create tool bar control.
	
	Create(pWnd, WS_CHILD | WS_VISIBLE | CBRS_TOP);		
	LoadToolBar(IDR_TOOLBAR);
	EnableDocking(CBRS_ALIGN_TOP);


	// Second, Create combo boxes.
	
	int iComboWidth = 140;
	int iScrollWidth = 80;
	CRect rect;

	// Mod files

	SetButtonInfo(0, IDC_COMBO1, TBBS_SEPARATOR, iComboWidth);
	GetItemRect(0, &rect);

	rect.bottom += 100;

	m_wndCombo1.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		               rect,
					   this,
					   IDC_COMBO1);

	
	// Anm files

	SetButtonInfo(1, IDC_COMBO2, TBBS_SEPARATOR, iComboWidth);
	GetItemRect(1, &rect);		

	rect.bottom += 100;
	
	m_wndCombo2.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
					   rect,
					   this,
					   IDC_COMBO2);


	// Mtl files

	SetButtonInfo(2, IDC_COMBO3, TBBS_SEPARATOR, iComboWidth);
	GetItemRect(2, &rect);

	rect.bottom += 100;

	m_wndCombo3.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
					   rect,
					   this,
					   IDC_COMBO3);	

	// Wav files.

	SetButtonInfo(3, IDC_COMBO4, TBBS_SEPARATOR, iComboWidth);
	GetItemRect(3, &rect);

	rect.bottom += 100;

	m_wndCombo4.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
					   rect,
					   this,
					   IDC_COMBO4);	



	// Make Camera button.

	GetItemRect(5, &rect);
	rect.right += 10;
	
	m_wndResetCameraButton.Create("Camera",
						   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						   rect,
						   this,
						   IDC_RESET_CAMERA);	


	// Check box for All model data animation.

	SetButtonInfo(6, 0, TBBS_SEPARATOR, 20);

	GetItemRect(7, &rect);
	rect.right += 20;
	m_wndAllSelectButton.Create("All Mod",
						   WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
						   rect,
						   this,
						   IDC_ALL_SELECT);	

	
	SetButtonInfo(8, 0, TBBS_SEPARATOR, 30);
	SetButtonInfo(9, IDC_LIGHT_DIALOG, TBBS_SEPARATOR, 25);

	// Button for Light Setting Dialog.

	GetItemRect(9, &rect);
	rect.right = rect.left + 25;
	m_wndLightButton.Create("L",
						    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						    rect,
						    this,
						    IDC_LIGHT_DIALOG);

	// Button for Front of Camera.

	SetButtonInfo(10, 0, TBBS_SEPARATOR, 5);
	SetButtonInfo(11, IDC_FRONT_OF_CAMERA, TBBS_SEPARATOR, 60);

	GetItemRect(11, &rect);
	rect.right = rect.left + 60;
	m_wndFrontOfCamera.Create("Front:",
							 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
							 rect,
							 this,
							 IDC_FRONT_OF_CAMERA);
	
	
	// Static control for Front of Camera.

	SetButtonInfo(12, IDC_FRONT_OF_CAMERA_STATIC, TBBS_SEPARATOR, 100);

	GetItemRect(12, &rect);
	rect.right = rect.left + 100;
	m_wndFrontOfCameraStatic.Create("None",
									WS_CHILD | WS_VISIBLE,
									rect,
									this,
									IDC_FRONT_OF_CAMERA_STATIC);
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CNewToolBar::SetAniInfoToToolBar
// Description :
//----------------------------------------------------------------------------
BOOL CNewToolBar::UpdateToolBar(VOID)
{
	// First, Model file combo box update.

	int i = 0;
	int iLastComboboxIndex = 0;

	iLastComboboxIndex = m_wndCombo1.GetCurSel();	
	

	// Clear all contents of combo box
	for (i = m_wndCombo1.GetCount(); i >= 0; i--)
	{
		m_wndCombo1.DeleteString(i);
	}
	
	char   szFileName[1024];		
	
	for (i = 0; i < g_ToolInfo.m_lstModFileName.GetCount(); i++)
	{
		POSITION pos = g_ToolInfo.m_lstModFileName.FindIndex(i);
		CString strModFileName = g_ToolInfo.m_lstModFileName.GetAt(pos);

		GetPureFileName(szFileName, (LPSTR)(LPCTSTR)strModFileName, 1024);
		m_wndCombo1.AddString(szFileName); 
	}

	if (iLastComboboxIndex < m_wndCombo1.GetCount() && 0 < iLastComboboxIndex)
	{		
		// Restore combo box's top index.

		m_wndCombo1.SetCurSel(g_ToolInfo.m_iCurrentModNo);
	}
	else
	{
		// setting combo box's top index.
		m_wndCombo1.SetCurSel(0);
	}
	

	// Second, Animation file combo box update.

	iLastComboboxIndex = m_wndCombo2.GetCurSel();


	// Clear all contents of combo box
	for (i = m_wndCombo2.GetCount(); i >= 0; i--)
	{
		m_wndCombo2.DeleteString(i);
	}

	
	for (i = 0; i < g_ToolInfo.m_lstAnmFileName.GetCount(); i++)
	{
		// Add string to the combo box.

		POSITION pos = g_ToolInfo.m_lstAnmFileName.FindIndex(i);
		CString strAnmFileName = g_ToolInfo.m_lstAnmFileName.GetAt(pos);

		GetPureFileName(szFileName, (LPSTR)(LPCTSTR)strAnmFileName, 1024);
		m_wndCombo2.AddString(szFileName); 
	}

	if (iLastComboboxIndex < m_wndCombo2.GetCount() && 0 < iLastComboboxIndex)
	{		
		// Restore combo box's top index.		
		m_wndCombo2.SetCurSel(g_ToolInfo.m_iCurrentAnmNo);
	}
	else
	{
		// Setting combo box's top index
		m_wndCombo2.SetCurSel(0);
	}


	// Third, Material combo box update

	iLastComboboxIndex = m_wndCombo3.GetCurSel();


	// Clear all contents of combo box
	for (i = m_wndCombo3.GetCount(); i >= 0; i--)
	{
		m_wndCombo3.DeleteString(i);
	}

	for (i = 0; i < g_ToolInfo.m_lstMtlFileName.GetCount(); i++)
	{
		// Add string to the combo box.

		POSITION pos = g_ToolInfo.m_lstMtlFileName.FindIndex(i);
		CString strMtlFileName = g_ToolInfo.m_lstMtlFileName.GetAt(pos);

		GetPureFileName(szFileName, (LPSTR)(LPCTSTR)strMtlFileName, 1024);
		m_wndCombo3.AddString(szFileName); 
	}

	if (iLastComboboxIndex < m_wndCombo3.GetCount() && 0 < iLastComboboxIndex)
	{		
		// Restore combo box's top index.		
		m_wndCombo3.SetCurSel(g_ToolInfo.m_iCurrentMtlNo);
	}
	else
	{
		// Setting combo box's top index
		m_wndCombo3.SetCurSel(0);
	}


	// Fourth, Wav combo box update.

	iLastComboboxIndex = m_wndCombo4.GetCurSel();

	// Clear all contents of combo box.

	for (i = m_wndCombo4.GetCount(); i >= 0; i--)
	{
		m_wndCombo4.DeleteString(i);	
	}

	for (i = 0; i < g_ToolInfo.m_lstWavFileName.GetCount(); i++)
	{
		// Add string to the combo box.

		POSITION pos = g_ToolInfo.m_lstWavFileName.FindIndex(i);
		CString strWavFileName = g_ToolInfo.m_lstWavFileName.GetAt(pos);

		GetPureFileName(szFileName, (LPSTR)(LPCTSTR)strWavFileName, 1024);
		m_wndCombo4.AddString(szFileName);
	}	

	if (iLastComboboxIndex < m_wndCombo4.GetCount() && 0 < iLastComboboxIndex)
	{
		// Restore combo box's topindex.

		m_wndCombo4.SetCurSel(g_ToolInfo.m_iCurrentWavNo);
	}
	else
	{
		// Setting combo box's top index.

		m_wndCombo4.SetCurSel(0);
	}
	
	return TRUE;
}


//----------------------------------------------------------------------------
// Name : CNewToolBar::ClearAnimationInfo
// Description :
//----------------------------------------------------------------------------
BOOL CNewToolBar::ClearAnimationInfo(VOID)
{
	// Clear all Information

	g_ToolInfo.ClearInformation();	
	UpdateToolBar();

	return TRUE;
}

//----------------------------------------------------------------------------

