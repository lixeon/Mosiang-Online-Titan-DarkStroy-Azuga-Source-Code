#pragma once
#include ".\interface\cdialog.h"
#include "cImageSelf.h"

class CKeySettingTipDlg :
	public cDialog
{
	cImageSelf		m_KeyImage[2];
	WORD			m_wMode;

public:
	CKeySettingTipDlg(void);
	~CKeySettingTipDlg(void);
	
	void Linking();
	void SetMode(WORD mode) { m_wMode = mode; }
	virtual void Render();
};
