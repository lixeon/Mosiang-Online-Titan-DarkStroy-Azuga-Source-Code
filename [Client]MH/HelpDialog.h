#ifndef _HELPDIALOG_
#define _HELPDIALOG_

#include "cListDialogEx.h"
#include "cPage.h"


class cHelpDialog : public cDialog
{
	cListDialogEx*			m_pListDlg;
	DWORD					m_dwCurPageId;

	HYPER					m_sHyper[MAX_REGIST_HYPERLINK];
	int						m_nHyperCount;

public:
	cHelpDialog();
	virtual ~cHelpDialog();
	
	virtual void SetActive( BOOL val );

	void Linking();
	
	BOOL OpenDialog();
	BOOL OpenLinkPage( DWORD dwPageId );
	void EndDialog();

	HYPER* GetHyperInfo( DWORD dwIdx );

	void HyperLinkParser( DWORD dwIdx );
};

#endif //_HELPDIALOG_
