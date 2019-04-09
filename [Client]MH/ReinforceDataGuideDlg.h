#pragma once
#include "INTERFACE\cDialog.h"

enum eRFDG_ITEM_KIND
{
	eRFDG_ITEM_WEAPON,
	eRFDG_ITEM_CAP,
	eRFDG_ITEM_CLOTHES,
	eRFDG_ITEM_BOOTS,
	eRFDG_ITEM_CLOVE,
	eRFDG_ITEM_CLOAK,
	eRFDG_ITEM_BLET,
	eRFDG_ITEM_AMULET,
	eRFDG_ITEM_RING,
};

class cPushupButton;
class cDialog;

class CReinforceDataGuideDlg :
	public cDialog
{
	cPushupButton*	m_pItemKindButton[9];
	cDialog*		m_pDataDlg[9];
	WORD			m_wCurData;

public:
	CReinforceDataGuideDlg(void);
	virtual ~CReinforceDataGuideDlg(void);

	void Linking();

	void Show();
	void Close();

	void SelectData(WORD index) { m_wCurData = index; }

	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionEventWindow(CMouse * mouseInfo);
};
