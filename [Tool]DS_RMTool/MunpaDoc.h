#if !defined(AFX_MUNPADOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)
#define AFX_MUNPADOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaDoc.h : header file
//

#include "basedoc.h"
#include "IndexGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CMunpaDoc document

class CMunpaDoc : public CBaseDoc
{
protected:
	CMunpaDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMunpaDoc)

// Attributes
protected:

public:
	// ¿øº» (DB¿Í Ç×»ó °°´Ù.)
	MUNPABASEINFO			m_sBaseInfo_org;
	MUNPAWAREHOUSEINFO		m_sWareHouse_org;

	CYHHashTable<ITEM_OPTION_INFO>	m_OptionInfo_org;

	// º¯°æº» (controlµéÀÇ µ¥ÀÌÅÍ¸¦ Á¦¾îÇÒ¶§ »ç¿ë.)
	MUNPABASEINFO*			m_pBaseInfo;
	MUNPAWAREHOUSEINFO*		m_pWareHouse;

	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionInfo;

	// ¿É¼Ç ÀÓ½Ã Key°ª »ý¼º..
	CIndexGenerator m_OptionKeyGenerator;

// Operations
public:
	BOOL				IsLoadingAll() { return (m_dwLoadingFlag==eML_LoadingAll)? TRUE:FALSE; }

	virtual void		InitData();
	virtual void		OpenData();
	virtual void		UpdateData(DWORD dwParam1=0, DWORD dwParam2=0);
	virtual void		SaveData();

	
	ITEM_OPTION_INFO* GetItemOptionInfo(DWORD wOptionIdx);
	ITEM_OPTION_INFO* GetItemOptionInfoOrigin(DWORD wOptionIdx);

	WORD AddLocalOptionInfo( ITEM_OPTION_INFO* pLocalOption, BOOL bNew = TRUE );
	void DelLocalOptionInfo( WORD Index );

	WORD CreateLocalOptionIndex();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMunpaDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMunpaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMunpaDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void NetworkMsgParse(BYTE Protocol, void* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPADOC_H__B85A4AAC_B586_4CB3_BA59_064EC6811B9E__INCLUDED_)
