// Mouse.h: interface for the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSE_H__6F329A10_CF6A_4820_88D7_E4DAA766BCED__INCLUDED_)
#define AFX_MOUSE_H__6F329A10_CF6A_4820_88D7_E4DAA766BCED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CMouse;
extern CMouse g_mouse;

class CMouse  
{
	int	m_MouseX;
	int	m_MouseY;
	int	m_MouseAxisX,	m_MouseAxisY;
	int	m_LMouse,			m_RMouse,			m_MMouse;
	int	m_LMouseOld,		m_RMouseOld,		m_MMouseOld;
	int	m_LMouseDown,		m_RMouseDown,		m_MMouseDown;
	int	m_LMouseUp,			m_RMouseUp,			m_MMouseUp;
	int	m_LMousePressed,	m_RMousePressed,	m_MMousePressed;
	int	m_LMouseCheck,		m_RMouseCheck,		m_MMouseCheck;
	int	m_LMouseDblPressed,	m_RMouseDblPressed,	m_MMouseDblPressed;
	int	m_LMouseDblClick,	m_RMouseDblClick,	m_MMouseDblClick;
	int	m_LMouseDrag,		m_RMouseDrag,		m_MMouseDrag;
	int m_LMouseDblClickOld,m_RMouseDblClickOld,m_MMouseDblClickOld;
	int	m_MouseOldX,m_MouseOldY;

	int m_WheelMsg;
	int m_Wheel;
	HWND m_hWnd;
public:
	CMouse();
	virtual ~CMouse();

	void GetWindowMessage(HWND hWnd, DWORD wmsg,WPARAM wParam,LPARAM lParam);
	void MouseStateCheck();

	int  GetMouseX();
	int  GetMouseY();
	int  GetMouseAxisX();
	int  GetMouseAxisY();
	
	BOOL LButtonDown();
	BOOL RButtonDown();
	BOOL MButtonDown();

	BOOL LButtonUp();
	BOOL RButtonUp();
	BOOL MButtonUp();

	BOOL LButtonClick();
	BOOL RButtonClick();
	BOOL MButtonClick();

	BOOL LButtonDrag();
	BOOL RButtonDrag();
	BOOL MButtonDrag();

	BOOL LButtonPressed();
	BOOL RButtonPressed();
	BOOL MButtonPressed();

	BOOL LButtonDoubleClick();
	BOOL RButtonDoubleClick();
	BOOL MButtonDoubleClick();

	int GetWheel();
};

#endif // !defined(AFX_MOUSE_H__6F329A10_CF6A_4820_88D7_E4DAA766BCED__INCLUDED_)
