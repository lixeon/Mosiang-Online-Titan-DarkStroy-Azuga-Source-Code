// ServerToolDoc.h : interface of the CServerToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERTOOLDOC_H__64836DA0_9FA1_44A8_A3A9_E8AD66641FDD__INCLUDED_)
#define AFX_SERVERTOOLDOC_H__64836DA0_9FA1_44A8_A3A9_E8AD66641FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CServerToolDoc : public CDocument
{
protected: // create from serialization only
	CServerToolDoc();
	DECLARE_DYNCREATE(CServerToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerToolDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CServerToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CServerToolDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERTOOLDOC_H__64836DA0_9FA1_44A8_A3A9_E8AD66641FDD__INCLUDED_)
