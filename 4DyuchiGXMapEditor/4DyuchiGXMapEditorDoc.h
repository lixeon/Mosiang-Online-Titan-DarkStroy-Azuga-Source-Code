// 4DyuchiGXMapEditorDoc.h : interface of the CMy4DyuchiGXMapEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_4DYUCHIGXMAPEDITORDOC_H__0D15261B_BE7C_4DBB_B7F9_C8FA51DD6589__INCLUDED_)
#define AFX_4DYUCHIGXMAPEDITORDOC_H__0D15261B_BE7C_4DBB_B7F9_C8FA51DD6589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "tool.h"

class CMy4DyuchiGXMapEditorDoc : public CDocument
{
protected: // create from serialization only
	CMy4DyuchiGXMapEditorDoc();
	DECLARE_DYNCREATE(CMy4DyuchiGXMapEditorDoc)

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4DyuchiGXMapEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMy4DyuchiGXMapEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMy4DyuchiGXMapEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4DYUCHIGXMAPEDITORDOC_H__0D15261B_BE7C_4DBB_B7F9_C8FA51DD6589__INCLUDED_)
