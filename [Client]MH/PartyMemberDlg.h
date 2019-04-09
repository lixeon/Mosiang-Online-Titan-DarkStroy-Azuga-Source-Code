// PartyMemberDlg.h: interface for the CPartyMemberDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)
#define AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CObjectGuagen;
class cPushupButton;
class cStatic;
class CPartyBtnDlg;

class CPartyMemberDlg  : public cDialog
{
	cPushupButton * m_pName;
	CObjectGuagen * m_pLife;
	CObjectGuagen * m_pNaeryuk;
	cStatic		  * m_pLevel;

	CPartyBtnDlg*	m_pPartyBtnDlg;

	BOOL			m_bRealActive;
	int				m_nIndex;
	BOOL			m_bOption;
	BOOL			m_bMember;

public:

	DWORD m_MemberID;
	
	CPartyMemberDlg();
	virtual ~CPartyMemberDlg();
	
	void SetActive(BOOL val);
	void Linking(int i);
	void SetMemberData(PARTY_MEMBER* pInfo);
	void SetNameBtnPushUp(BOOL val);
	void SetPartyBtnDlg( CPartyBtnDlg* pDlg )	{ m_pPartyBtnDlg = pDlg; }
	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();

	void	ShowOption( BOOL bOption )			{ m_bOption = bOption; }	
	void	ShowMember( BOOL bMember )			{ m_bMember = bMember; }
};

#endif // !defined(AFX_PARTYMEMBERDLG_H__7426A94E_644C_44BE_80FE_877AE4A137B7__INCLUDED_)
