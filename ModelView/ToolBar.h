//
//	Custom Tool bar inherited CToolBar (MFC)
//
//				2002.12.12
//				Trust Pak


#include "StdAfx.h"
#include "ToolInfo.h"


#ifndef _CUSTOM_TOOL_BAR_BY_TRUSTPAK_
#define _CUSTOM_TOOL_BAR_BY_TRUSTPAK_


//----------------------------------------------------------------------------
class CNewToolBar : public CToolBar
{
public:
	CNewToolBar();
	~CNewToolBar();

	BOOL CreatNewToolBar(CWnd* pWnd);

	BOOL ClearAnimationInfo(VOID);	
	BOOL UpdateToolBar(VOID);	

	CComboBox	m_wndCombo1;	// Mod files
	CComboBox	m_wndCombo2;	// Anm files	
	CComboBox	m_wndCombo3;	// Mtl files
	CComboBox	m_wndCombo4;	// Wav files
	CButton		m_wndResetCameraButton;
	CButton		m_wndAllSelectButton;
	CButton		m_wndPlaySoundButton;
	CButton		m_wndLightButton;

	CButton		m_wndFrontOfCamera;
	CStatic		m_wndFrontOfCameraStatic;
};

//创建一个新的工具栏
//----------------------------------------------------------------------------


#endif