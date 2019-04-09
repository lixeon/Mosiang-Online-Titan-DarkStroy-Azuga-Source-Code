// GTRegistDialog.h: interface for the CGTRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTREGISTDIALOG_H__B909D599_D33B_4282_9602_CE14E937EB09__INCLUDED_)
#define AFX_GTREGISTDIALOG_H__B909D599_D33B_4282_9602_CE14E937EB09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


class cStatic;
class cButton;


class CGTRegistDialog : public cDialog
{
	cStatic*		m_pRegistGuild;
	cStatic*		m_pRegistableGuild;

	cButton*		m_pRegistBtn;

public:
	CGTRegistDialog();
	virtual ~CGTRegistDialog();

	void Linking();
	void SetActive( BOOL val );

	DWORD TournamentRegistSyn();
	void SetRegistGuildCount( DWORD count );

};

#endif // !defined(AFX_GTREGISTDIALOG_H__B909D599_D33B_4282_9602_CE14E937EB09__INCLUDED_)
