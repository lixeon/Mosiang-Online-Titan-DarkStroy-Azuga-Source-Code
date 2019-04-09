// DlgCreateEffect.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "DlgCreateEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgCreateEffect dialog


CDlgCreateEffect::CDlgCreateEffect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateEffect::IDD, pParent)
{

	m_pszEffectName = NULL;
	//{{AFX_DATA_INIT(CDlgCreateEffect)
	m_strEffectName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCreateEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateEffect)
	DDX_Text(pDX, IDC_EDIT_EFFECT_NAME, m_strEffectName);
	DDV_MaxChars(pDX, m_strEffectName, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCreateEffect, CDialog)
	//{{AFX_MSG_MAP(CDlgCreateEffect)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_NAME_OK, OnButtonEffectNameOk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateEffect message handlers

void CDlgCreateEffect::OnButtonEffectNameOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	LPCSTR	pszName = (LPCSTR)m_strEffectName;
	lstrcpy(m_pszEffectName,pszName);
	EndDialog(0);
}

void CDlgCreateEffect::OnDestroy() 
{
	UpdateData(TRUE);

	LPCSTR	pszName = (LPCSTR)m_strEffectName;
	lstrcpy(m_pszEffectName,pszName);
	
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	OnButtonEffectNameOk();
}
