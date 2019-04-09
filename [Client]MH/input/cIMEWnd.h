// cIMEWnd.h: interface for the cIMEWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMEWND_H__73C6ABA6_B6AB_41DB_B801_31DFB4DF7A99__INCLUDED_)
#define AFX_CIMEWND_H__73C6ABA6_B6AB_41DB_B801_31DFB4DF7A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "imm32.lib")

#define CIMEWND	USINGTON(cIMEWnd)

class cIMEex;

class cIMEWnd  
{
public:
	cIMEWnd();
	virtual ~cIMEWnd();

	BOOL OnMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void SetDockingEx(cIMEex * box, BOOL bDock);
	BOOL IsDocking() { return m_pDockingEx ? TRUE : FALSE; }
	cIMEex* GetDockingEx() { return m_pDockingEx; }
	void SetCompositing( BOOL bCompositing );

	void SetCtrlPushed( BOOL bPushed ) { m_bCtrlPushed = bPushed; }
	BOOL IsCtrlPushed() { return m_bCtrlPushed; }

private:
	BOOL OnKey(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL OnChar(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	BOOL OnIMEComposition(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	cIMEex*	m_pDockingEx;

	BOOL m_bCompositing;

	BOOL m_bCtrlPushed;
};

EXTERNGLOBALTON(cIMEWnd);

#endif // !defined(AFX_CIMEWND_H__73C6ABA6_B6AB_41DB_B801_31DFB4DF7A99__INCLUDED_)
