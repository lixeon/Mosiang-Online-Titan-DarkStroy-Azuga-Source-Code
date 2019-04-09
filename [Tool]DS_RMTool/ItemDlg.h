#if !defined(AFX_ITEMDLG_H__77D974BF_7501_4A87_A50D_EF7ACA6779E1__INCLUDED_)
#define AFX_ITEMDLG_H__77D974BF_7501_4A87_A50D_EF7ACA6779E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDlg.h : header file
//

#include "PropertyList.h"

/////////////////////////////////////////////////////////////////////////////
// CItemDlg dialog

enum emItem_Basic
{
	emBasic_DBIdx = 0, emBasic_Idx, emBasic_Position, emBasic_Durability, emBasic_Quick, emBasic_Max,
};

enum emItem_Ex
{
	emBasicEx_OptionIdx = 0, emBasicEx_ItemDBIdx, emBasicEx_GenGol, emBasicEx_MinChub, emBasicEx_CheRyuk,
	emBasicEx_SimMek, emBasicEx_Life, emBasicEx_NaeRyuk, emBasicEx_Shield, emBasicEx_PhyAttack, 
	emBasicEx_CriticalPercent, emBasicEx_PhyDefense, emBasicEx_Max,
};

enum emItem_Attr
{
	emAttr_AFireElement = 0, emAttr_AWaterElement, emAttr_ATreeElement, emAttr_AGoldElement,
	emAttr_AEarthElement, emAttr_ANoneElement,
	emAttr_DFireElement, emAttr_DWaterElement, emAttr_DTreeElement, emAttr_DGoldElement,
	emAttr_DEarthElement, emAttr_DNoneElement, emAttr_Max,
};

class CItemDlg : public CDialog
{
protected:
	ITEMBASE			m_Item;
	ITEM_OPTION_INFO	m_ItemOption;
	WORD				m_wPos;
	INFOFIELD			m_Field;

	// 장착창의 경우에만 사용하는 변수
	WORD	m_wWearType;

	CPropertyItem* m_BasicList[emBasic_Max];
	CPropertyItem* m_BasicExList[emBasicEx_Max];
	CPropertyItem* m_AttrList[emAttr_Max];

	int m_nCheckValue;
	BOOL m_bOption;
	WORD m_wLevel;

///////////// 분류
void SetItemTypeControl();
void SetItemExTypeCombo(WORD wDef = 0);
void EnableOptionControl(BOOL bEnable = TRUE);
//////////////

////////////// 실제 내용
void SetItemCombo(DWORD type);
//////////////

void EnableRadio(BOOL bEnable = TRUE);

BOOL IsValiedOption();
	
// Construction
public:
	CItemDlg(CWnd* pParent = NULL);   // standard constructor
	
	void Init( INFOFIELD efield, const ITEMBASE* pitem, const ITEM_OPTION_INFO* pOption, WORD pos, WORD wWeartype = 0 );

	ITEMBASE* GetItemBase() { return &m_Item; }
	ITEM_OPTION_INFO* GetItemOption() { return &m_ItemOption; }

	void SetLevel(WORD level) { m_wLevel = level; }

// Dialog Data
	//{{AFX_DATA(CItemDlg)
	enum { IDD = IDD_DLG_ITEM };
	CButton	m_rdWeapon;
	CPropertyList	m_listExBasic;
	CPropertyList	m_listExAttr;
	CPropertyList	m_listBasic;
	CComboBox	m_cbExType;
	CButton	m_chExData;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEdit();
	afx_msg void OnRadioAccessory();
	afx_msg void OnRadioDeffen();
	afx_msg void OnRadioEtc();
	afx_msg void OnRadioWeapon();
	afx_msg void OnSelchangeComboExtype();
	afx_msg void OnCheckExdata();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDLG_H__77D974BF_7501_4A87_A50D_EF7ACA6779E1__INCLUDED_)
