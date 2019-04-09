// PartyBtnDlg.h: interface for the CPartyBtnDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYBTNDLG_H__262EBF94_64D0_4F59_A512_6C9D9F8B7A7F__INCLUDED_)
#define AFX_PARTYBTNDLG_H__262EBF94_64D0_4F59_A512_6C9D9F8B7A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cStatic;

class CPartyBtnDlg  : public cDialog
{
	cStatic* m_pBackGround;
	cButton* m_pSecedeBtn;
	cButton* m_pTransferBtn;
	cButton* m_pForcedSecedeBtn;
	cButton* m_pAddMemberBtn;
	cButton* m_pBreakUpBtn;
	cButton* m_pWarSuggestBtn;
	cButton* m_pOptionBtn;
	cButton* m_pMemberBtn;
	
	BOOL	m_bOption;

public:
	CPartyBtnDlg();
	virtual ~CPartyBtnDlg();

	void RefreshDlg();
	void Linking();
	void ShowNonPartyDlg();
	void ShowPartyMasterDlg();
	void ShowPartyMemberDlg();
	void ShowOption( BOOL bOption );

	virtual void Render();
};

#endif // !defined(AFX_PARTYBTNDLG_H__262EBF94_64D0_4F59_A512_6C9D9F8B7A7F__INCLUDED_)
