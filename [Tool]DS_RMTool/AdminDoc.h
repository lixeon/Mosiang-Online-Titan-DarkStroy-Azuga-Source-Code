#if !defined(AFX_ADMINDOC_H__7966E789_9F01_4980_A364_F521B0709667__INCLUDED_)
#define AFX_ADMINDOC_H__7966E789_9F01_4980_A364_F521B0709667__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminDoc.h : header file
//

#include "basedoc.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc document

class CAdminDoc : public CBaseDoc
{
protected:
	CAdminDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdminDoc)

// Attributes
public:
	OPERATOR_TOTALINFO	m_sOperator;

	void	SetOperatorTotalInfo(WORD count, OPERATOR* pOper, BOOL bEnd);
	void	SetIpTotalInfo(WORD count, IPINFO* pIP, BOOL bEnd);
	BOOL	IsLoadingAll() { return (m_dwLoadingFlag==eAL_LoadingAll)? TRUE:FALSE; }

// Operations
public:
	virtual void	InitData();
	virtual void	OpenData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdminDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdminDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
	void NetworkMsgParse(BYTE Protocol, void* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINDOC_H__7966E789_9F01_4980_A364_F521B0709667__INCLUDED_)
