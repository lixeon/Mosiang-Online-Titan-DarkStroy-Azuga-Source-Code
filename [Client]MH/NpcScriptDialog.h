#ifndef __NPCSCRIPT_DIALOG__
#define __NPCSCRIPT_DIALOG__

#include "cListDialogEx.h"
#include "cPage.h"


class cStatic;


class cNpcScriptDialog  : public cDialog
{
protected:
	DWORD			m_dwCurNpc;
	DWORD			m_dwCurPageId;

	HYPER			m_sHyper[MAX_REGIST_HYPERLINK];
	int				m_nHyperCount;

	cListDialogEx*	m_pListDlg;
	cStatic*		m_pTitleText;

public:
	cNpcScriptDialog();
	virtual ~cNpcScriptDialog();

	void Linking();
	virtual void SetActive( BOOL val );
	void DisActiveEx();

	// ¿ÜºÎ¿¡¼­ È£ÃâÇÏ´Â ÇÔ¼ö....
	BOOL OpenDialog( DWORD dwNpcId );
	BOOL OpenLinkPage( DWORD dwPageId );
	void EndDialog();

	void HyperLinkParser( DWORD dwIdx );

public:
	void SetCurSelectedNpcId( DWORD dwId ) { m_dwCurNpc = dwId-1; }
	void SetDlgTitleText( char* strTitle );

	HYPER* GetHyperInfo( DWORD dwIdx );

	void OpenQuestLinkPage( DWORD dwPageId );

	DWORD	m_dwQuestIdx;
};

#endif