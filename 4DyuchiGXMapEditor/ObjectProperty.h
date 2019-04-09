#if !defined(AFX_OBJECTPROPERY_H__4B27C2CD_9314_48A2_907F_3C4E21E96056__INCLUDED_)
#define AFX_OBJECTPROPERY_H__4B27C2CD_9314_48A2_907F_3C4E21E96056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectPropery.h : header file
//
#include "../4DyuchiGRX_Common/typedef.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectProperty dialog

class CObjectProperty : public CDialog
{

	GXOBJECT_PROPERTY*		m_pObjectProperty;
// Construction
public:
	CObjectProperty(CWnd* pParent = NULL);   // standard constructor
	void					SetGXObjectProperty(GXOBJECT_PROPERTY* pProperty);

// Dialog Data
	//{{AFX_DATA(CObjectProperty)
	enum { IDD = IDD_DIALOG_OBJECT_PROPERTY };
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	DWORD	m_dwID;
	float	m_fRad;
	float	m_fAxisX;
	float	m_fAxisY;
	float	m_fAxisZ;
	BOOL	m_bApplyHField;
	BOOL	m_bAsEffect;

	
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;

	BOOL m_bLock;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectProperty)
	afx_msg void OnButtonOk();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTPROPERY_H__4B27C2CD_9314_48A2_907F_3C4E21E96056__INCLUDED_)
