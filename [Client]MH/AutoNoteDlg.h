#pragma once


#include "./Interface/cDialog.h"

class cTextArea;
class cButton;
class cListDialog;

class CAutoNoteDlg : public cDialog
{
protected:
	cTextArea*		m_pTextAreaManual;
	cButton*		m_pBtnAsk;
	cListDialog*	m_pListAuto;

public:
	CAutoNoteDlg(void);
	~CAutoNoteDlg(void);

	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void AddAutoList( char* strName, char* strDate );

	void SetActiveTestClient();
};