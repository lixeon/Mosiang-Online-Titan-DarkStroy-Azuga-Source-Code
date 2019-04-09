#if !defined(AFX_LOGOPERDOC_H__D90BF5E2_BCF6_424C_909D_A67B05CE220B__INCLUDED_)
#define AFX_LOGOPERDOC_H__D90BF5E2_BCF6_424C_909D_A67B05CE220B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOperDoc.h : header file
//

#include "basedoc.h"

/////////////////////////////////////////////////////////////////////////////
// CLogOperDoc document

class CLogOperDoc : public CBaseDoc
{
protected:
	CLogOperDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogOperDoc)

// Attributes
public:
	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogOperDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogOperDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogOperDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOPERDOC_H__D90BF5E2_BCF6_424C_909D_A67B05CE220B__INCLUDED_)
