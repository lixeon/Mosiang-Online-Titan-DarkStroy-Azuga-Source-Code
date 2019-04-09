// MainDialog.h: interface for the CMainDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDIALOG_H__FE03ACB8_DC10_4809_ABAB_845F21199918__INCLUDED_)
#define AFX_MAINDIALOG_H__FE03ACB8_DC10_4809_ABAB_845F21199918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class cPushupButton;

/*
enum
{
	ALLCLOSE_BTN = 0,
	CHAR_BTN,
	INVENTORY_BTN,
	MUGONG_BTN,
	PARTY_BTN,
	DEAL_BTN,		//°Å·¡
	ROADSHOP_BTN,	//³ëÁ¡
	MINIMAP_BTN,	//¹Ì´Ï¸Ê
	OPTION_BTN,		//¿É¼Ç¼³Á¤
	EXIT_BTN,		//³ª°¡±â

	TEMP_BTN,		//
	MAX_BTN
};
*/

enum
{
	CHAR_BTN = 0,
	INVENTORY_BTN,
	MUGONG_BTN,
	PARTY_BTN = 4,
	MAX_BTN
};

class CMainDialog  : public cDialog
{

public:
	CMainDialog();
	virtual ~CMainDialog();
	virtual void Add(cWindow * window);
	cPushupButton * GetPushupBtn(WORD idx) { return m_pBtn[idx]; }
private:

	cPushupButton * m_pBtn[MAX_BTN];
};

#endif // !defined(AFX_MAINDIALOG_H__FE03ACB8_DC10_4809_ABAB_845F21199918__INCLUDED_)
