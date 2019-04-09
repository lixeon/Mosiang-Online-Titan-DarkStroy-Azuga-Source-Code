// DS_RMToolDoc.h : interface of the CDS_RMToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DS_RMTOOLDOC_H__1606A91D_736A_4F26_8AEC_E271730C1995__INCLUDED_)
#define AFX_DS_RMTOOLDOC_H__1606A91D_736A_4F26_8AEC_E271730C1995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basedoc.h"

class CDS_RMToolDoc : public CBaseDoc//CDocument
{
protected: // create from serialization only
	CDS_RMToolDoc();
	DECLARE_DYNCREATE(CDS_RMToolDoc)
// Operations
public:
	////////////////////////////////////////////////////////////
	// °èÁ¤ Á¤º¸
	USERINFO		m_sUserInfoOrg;		// DB¿Í µ¿ÀÏ
	USERINFO*		m_sUserInfo;		// ÆíÁýÃ¢°ú µ¿ÀÏ
	////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDS_RMToolDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDS_RMToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CDS_RMToolDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetUserInfo(USERINFO* user);
	void ChangeUserLevel( DWORD dwUserLevel );

	void NetworkMsgParse(BYTE Protocol, void* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DS_RMTOOLDOC_H__1606A91D_736A_4F26_8AEC_E271730C1995__INCLUDED_)
