// ModelViewDoc.h : interface of the CModelViewDoc class
//
/////////////////////////////////////////////////////////////////////////////


#include "EngineRelation.h"

#if !defined(AFX_MODELVIEWDOC_H__954A929E_01AD_4D85_9A4F_3BEA100DA281__INCLUDED_)
#define AFX_MODELVIEWDOC_H__954A929E_01AD_4D85_9A4F_3BEA100DA281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CModelViewDoc : public CDocument
{
protected: // create from serialization only
	CModelViewDoc();
	DECLARE_DYNCREATE(CModelViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CModelViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELVIEWDOC_H__954A929E_01AD_4D85_9A4F_3BEA100DA281__INCLUDED_)
