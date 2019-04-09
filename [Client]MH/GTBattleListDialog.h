// GTBattleListDialog.h: interface for the CGTBattleListDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTBATTLELISTDIALOG_H__3555EFDE_1A79_4C6A_9FD6_8E0925E156EF__INCLUDED_)
#define AFX_GTBATTLELISTDIALOG_H__3555EFDE_1A79_4C6A_9FD6_8E0925E156EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


class cListCtrl;
class cPrtList;



class CGTBattleListDialog : public cDialog
{
	cListCtrl*		m_pBattleListCtrl;
	cPtrList		m_BattleList;
	DWORD			m_BattleCount;

	BOOL			m_bPlayOff;
	int				m_nPreSelectedIndex;

public:
	CGTBattleListDialog();
	virtual ~CGTBattleListDialog();
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void Linking();
	void SetActive( BOOL val );
	void DeleteAddBattleInfo();

	void RefreshBattleList();
	void AddBattleInfo( GTBATTLE_INFO* pBattleInfo );
	void DeleteBattleInfo( DWORD BattleID );
	void DeleteAllBattleInfo();
	
	void SetPlayOff( BOOL val )				{	m_bPlayOff = val;	}

	int GetSelectedBattle()					{	return m_nPreSelectedIndex;		}
	BOOL EnterBattleonObserverSyn();
};

#endif // !defined(AFX_GTBATTLELISTDIALOG_H__3555EFDE_1A79_4C6A_9FD6_8E0925E156EF__INCLUDED_)
