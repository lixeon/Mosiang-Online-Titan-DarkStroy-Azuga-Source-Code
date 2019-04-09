#ifndef _CHARMAKEDLG_
#define _CHARMAKEDLG_

#include "./interface/cDialog.h"
#include "CharMakeManager.h"

//class cComboBoxEx;
//class cEditBox;

class cCharMakeDlg : public cDialog
{
//	cComboBoxEx*	m_pMHair;
//	cComboBoxEx*	m_pWMHair;
//	cComboBoxEx*	m_pMFace;
//	cComboBoxEx*	m_pWMFace;
	cStatic*		m_pMHair;
	cStatic*		m_pWMHair;
	cStatic*		m_pMFace;
	cStatic*		m_pWMFace;

public:
	cCharMakeDlg();
	virtual ~cCharMakeDlg();

	void Linking();
	
	void InitComboStatus();
	void ChangeComboStatus( WORD wSex );

	void AddComboList( CHAR_M idx );

	void OnActionEvent(LONG lId, void* p, DWORD we);
};

#endif //_CHARMAKEDLG_