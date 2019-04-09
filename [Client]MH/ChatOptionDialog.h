/*
#ifndef _CHATOPTIONDIALOG_H
#define _CHATOPTIONDIALOG_H


#include "./Interface/cDialog.h"

class cCheckBox;

class CChatOptionDialog : public cDialog
{
protected:
	cCheckBox*		m_pBtnOption[12];
	BOOL			m_bFirst;
	BOOL			m_bChanged;
public:

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID);
	virtual void SetActive(BOOL val);

	void OnActionEvent(LONG lId, void * p, DWORD we);
	void Linking();

};



#endif
*/