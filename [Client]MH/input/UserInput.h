// UserInput.h: interface for the CUserInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINPUT_H__EFE6028D_0705_4A49_8A6D_0248736A359E__INCLUDED_)
#define AFX_USERINPUT_H__EFE6028D_0705_4A49_8A6D_0248736A359E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dinput.h>


#include "Mouse.h"
#include "Keyboard.h"

#include "../MouseEventThrower.h"
#include "../HeroMove.h"


class CUserInput;
extern CUserInput g_UserInput;

#define KEYBOARD	g_UserInput.GetKeyboard()
#define MOUSE		g_UserInput.GetMouse()



class CUserInput  
{

protected:

	CHeroMove m_HeroMove;
	CMouseEventThrower m_MouseEventThrower;

/////
	LPDIRECTINPUT8	m_pdi;

	CKeyboard	m_Keyboard;
	CMouse		m_Mouse;
	
	BOOL		m_bFocus;

public:
//	MAKESINGLETON(CUserInput);

	CUserInput();
	virtual ~CUserInput();
	
	void Init();
	void Release();
	
	void Process();
	void StartGetKeyState();
	BOOL GetNextKeyState();
	
	CHeroMove*	GetHeroMove()	{ return &m_HeroMove; }
	
	CKeyboard*	GetKeyboard()	{ return &m_Keyboard; }
	CMouse*		GetMouse()		{ return &m_Mouse; }

	void SetInputFocus( BOOL bFocus ) { m_bFocus = bFocus; }
};



#endif // !defined(AFX_USERINPUT_H__EFE6028D_0705_4A49_8A6D_0248736A359E__INCLUDED_)
