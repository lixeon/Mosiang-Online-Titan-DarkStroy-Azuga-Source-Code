#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"
#include "./interface/cDialog.h"
#include "./interface/cGuagen.h"

class cStatic;
class cButton;
class cGuagen;

class CPetStateMiniDlg : public cDialog
{
	cStatic*		m_pNameDlg;		//Æê ÀÌ¸§
	cStatic*		m_pStateDlg;
	cStatic*		m_pFriend;
	cStatic*		m_pStamina;

	cGuagen*		m_pFriendGuage;
	cGuagen*		m_pStaminaGuage;

	cButton*		m_pPetUseRestBtn;	//Æê È°µ¿/ÈÞ½Ä ¹öÆ°
	cButton*		m_pPetInvenBtn;	//Æê ÀÎº¥Åä¸®

	cButton*		m_pDlgToggleBtn;

public:
	CPetStateMiniDlg(void);
	virtual ~CPetStateMiniDlg(void);

	void Linking();
	void OnActionEvent(LONG lId, void *p, DWORD we);

	cStatic* GetNameTextWin()		{	return m_pNameDlg;	}
	cStatic* GetUseRestTextWin()	{	return m_pStateDlg;	}

	cStatic* GetFriendShipTextWin()	{	return m_pFriend;	}
	cStatic* GetStaminaTextWin()	{	return m_pStamina;	}

	cGuagen* GetFriendShipGuage()	{	return m_pFriendGuage;	}
	cGuagen* GetStaminaGuage()		{	return m_pStaminaGuage;	}
};
