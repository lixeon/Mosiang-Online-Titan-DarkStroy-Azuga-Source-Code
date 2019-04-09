#ifndef _CHARSTATEDIALOG_H
#define _CHARSTATEDIALOG_H


#include "./interface/cDialog.h"


class cPushupButton;


class CCharStateDialog : public cDialog
{
protected:
	
	cPushupButton*	m_pBtnPK;
	cPushupButton*	m_pBtnMove;
	cPushupButton*	m_pBtnKyungGong;
	cPushupButton*	m_pBtnPeaceWar;
	cPushupButton*	m_pBtnUngi;
	
public:

	CCharStateDialog();
	virtual ~CCharStateDialog();
	
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void SetPKMode( BOOL bPKMode );
	void SetMoveMode( BOOL bRun );
	void SetKyungGongMode( BOOL bKyungGong );
	void SetPeaceWarMode( BOOL bPeace );
	void SetUngiMode( BOOL bUngi );

	void Refresh();
};





#endif