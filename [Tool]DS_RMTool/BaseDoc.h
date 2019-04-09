#if !defined(AFX_BASEDOC_H__EA855F20_9826_4E69_A629_C6D0C0FF586B__INCLUDED_)
#define AFX_BASEDOC_H__EA855F20_9826_4E69_A629_C6D0C0FF586B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc document

#include "ChangeManager.h"


class CBaseDoc : public CDocument
{
protected:
	CBaseDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBaseDoc)

// Attributes
protected:
	CChangeManager			m_ChangeMgr;
	BOOL					m_bReadOnly;

	DWORD					m_dwDocumentIdx;
	DWORD					m_dwLoadingFlag;
	DWORD					m_dwSaveFlag;

// Operations
public:
	virtual void		InitData() {}										// Data Init
	virtual void		OpenData() {}										// Data Open(memcpy)
	virtual void		UpdateData(DWORD dwParam1=0, DWORD dwParam2=0) {}	// Control Update and DocData Reset
	virtual void		SaveData() {}										// Send Data Packet To Server
	virtual void		ProcessChangeManager();
	virtual void		NetworkMsgParse(BYTE Protocol, void* pMsg) {}

// Service
public:
	CChangeManager*		GetChangeManager() { return &m_ChangeMgr; }

	virtual DWORD		GetDocumentIdx() { return m_dwDocumentIdx; }
	virtual void		SetDocumentIdx(DWORD dwIdx) { m_dwDocumentIdx = dwIdx; }

	virtual CView*		GetView(WORD wIdx);

	virtual void		SetLoadingFlag(DWORD flag);
	virtual BOOL		IsLoadingAll() { return TRUE; }

	virtual void		SetSaveFlag(DWORD flag) { m_dwSaveFlag |= flag; }
	virtual BOOL		IsSetSaveFlag(DWORD flag) { return (m_dwSaveFlag & flag)? TRUE:FALSE;}

	BOOL				IsReadOnly() { return m_bReadOnly; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDOC_H__EA855F20_9826_4E69_A629_C6D0C0FF586B__INCLUDED_)
