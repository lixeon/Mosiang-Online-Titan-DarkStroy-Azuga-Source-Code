#if !defined(AFX_LOGDOC_H__D28F8BD2_E541_445D_B34C_5EE0E35138DE__INCLUDED_)
#define AFX_LOGDOC_H__D28F8BD2_E541_445D_B34C_5EE0E35138DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDoc.h : header file
//

#include "basedoc.h"
#include "ProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CLogDoc document

class CLogDoc : public CBaseDoc
{
protected:
	CLogDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogDoc)

// Attributes
public:
	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	DWORD	m_dwPage;
	
	void	StartDlg();
	void	EndDlg();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDOC_H__D28F8BD2_E541_445D_B34C_5EE0E35138DE__INCLUDED_)
