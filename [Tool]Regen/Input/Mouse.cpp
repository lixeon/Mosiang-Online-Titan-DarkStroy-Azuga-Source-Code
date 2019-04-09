// Mouse.cpp: implementation of the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mouse.h"

CMouse g_mouse;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define _INPUTDEBUG_
#ifdef _INPUTDEBUG_
FILE* inputtestFile;
#endif

CMouse::CMouse()
{
	m_WheelMsg = 0;

#ifdef _INPUTDEBUG_
	inputtestFile = fopen("../inputtest.txt","w");
#endif
	
}

CMouse::~CMouse()
{

#ifdef _INPUTDEBUG_
	fclose(inputtestFile);
#endif

}

#ifdef _INPUTDEBUG_
#define INPUTLOG(a)		fprintf(inputtestFile,"%s  %d\n",a,gCurTime)
#else
#define INPUTLOG(a)
#endif

void CMouse::GetWindowMessage(HWND hWnd, DWORD wmsg,WPARAM wParam,LPARAM lParam)
{
	m_hWnd = hWnd;
	switch(wmsg)
	{	
	case WM_LBUTTONUP:			m_LMousePressed = 0;		INPUTLOG("L UP");		break;
	case WM_RBUTTONUP:			m_RMousePressed = 0;		INPUTLOG("R UP");		break;
	case WM_MBUTTONUP:			m_MMousePressed = 0;		INPUTLOG("M UP");		break;

	case WM_LBUTTONDOWN:		m_LMousePressed = 1;	    INPUTLOG("L DN");		break;
	case WM_RBUTTONDOWN:		m_RMousePressed = 1;		INPUTLOG("R DN");		break;
	case WM_MBUTTONDOWN:		m_MMousePressed = 1;		INPUTLOG("M DN");		break;

	case WM_LBUTTONDBLCLK:		m_LMouseDblPressed = 1;		INPUTLOG("L DB");		break;
	case WM_RBUTTONDBLCLK:		m_RMouseDblPressed = 1;		INPUTLOG("R DB");		break;
	case WM_MBUTTONDBLCLK:		m_MMouseDblPressed = 1;		INPUTLOG("M DB");		break;

	case 0x020A:	//WM_MOUSEWHEEL
		{
			int xPos = LOWORD(lParam); // horizontal position of pointer 
			int yPos = HIWORD(lParam); // vertical position of pointer 
			if(int(wParam) > 0)		//wheel up
				++m_WheelMsg;
			else								//wheel down
				--m_WheelMsg;
		}
		break;
	}
}


void CMouse::MouseStateCheck()
{
	//////////////////////////////////////////////////////////////////////////
	// 마우스 현재 상태 얻어오는 부분
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd,&pt);
	m_MouseX = pt.x;
	m_MouseY = pt.y;	
	//////////////////////////////////////////////////////////////////////////
	

	m_LMouse = m_LMousePressed;
	m_RMouse = m_RMousePressed;

    if ( m_LMouse  == m_LMouseOld )			m_LMouseCheck = 0;
	else									m_LMouseCheck = 1;
    if ( m_RMouse  == m_RMouseOld )			m_RMouseCheck = 0;
	else									m_RMouseCheck = 1;
    if ( m_MMouse  == m_MMouseOld )			m_MMouseCheck = 0;
	else									m_MMouseCheck = 1;

	if ( m_LMouse==1 && m_LMouseCheck==1 )  m_LMouseDown = 1;
    else									m_LMouseDown = 0;
	if ( m_RMouse==1 && m_RMouseCheck==1 )  m_RMouseDown = 1;
	else									m_RMouseDown = 0;
	if ( m_MMouse==1 && m_MMouseCheck==1 )  m_MMouseDown = 1;
	else									m_MMouseDown = 0;

	if ( m_LMouse==0 && m_LMouseCheck==1 )  m_LMouseUp = 1;
    else									m_LMouseUp = 0;
	if ( m_RMouse==0 && m_RMouseCheck==1 )  m_RMouseUp = 1;
	else									m_RMouseUp = 0;
	if ( m_MMouse==0 && m_MMouseCheck==1 )  m_MMouseUp = 1;
	else									m_MMouseUp = 0;

    if ( m_LMouse==1 && m_LMouseCheck==0 )	m_LMouseDrag = 1;
	else									m_LMouseDrag = 0;
    if ( m_RMouse==1 && m_RMouseCheck==0 )	m_RMouseDrag = 1;
	else									m_RMouseDrag = 0;
    if ( m_MMouse==1 && m_MMouseCheck==0 )	m_MMouseDrag = 1;
	else									m_MMouseDrag = 0;

	if( m_LMouseDblPressed == 0 && (m_LMouseDblClickOld != m_LMouseDblPressed) )
		m_LMouseDblClick = 1;
	else
		m_LMouseDblClick = 0;

	if( m_RMouseDblPressed == 0 && (m_RMouseDblClickOld != m_RMouseDblPressed) )
		m_RMouseDblClick = 1;
	else
		m_RMouseDblClick = 0;

	if( m_MMouseDblPressed == 0 && (m_MMouseDblClickOld != m_MMouseDblPressed) )
		m_MMouseDblClick = 1;
	else
		m_MMouseDblClick = 0;

	m_Wheel = m_WheelMsg;
	m_WheelMsg = 0;

	m_LMouseOld = m_LMouse;
	m_RMouseOld = m_RMouse;
	m_MMouseOld = m_MMouse;

	m_LMouseDblClickOld = m_LMouseDblPressed;
	m_RMouseDblClickOld = m_RMouseDblPressed;
	m_MMouseDblClickOld = m_MMouseDblPressed;

	m_LMouseDblPressed = 0;
	m_RMouseDblPressed = 0;
	m_MMouseDblPressed = 0;
	
	m_MouseAxisX = m_MouseOldX - m_MouseX;
	m_MouseAxisY = m_MouseOldY - m_MouseY;

	m_MouseOldX = m_MouseX;
	m_MouseOldY = m_MouseY;
	
}

int CMouse::GetMouseX()
{
	return m_MouseX;
}

int CMouse::GetMouseY()
{
	return m_MouseY;
}

int  CMouse::GetMouseAxisX()
{
	return m_MouseAxisX;
}

int  CMouse::GetMouseAxisY()
{
	return m_MouseAxisY;
}

BOOL CMouse::LButtonPressed()
{
	return m_LMousePressed;
}
BOOL CMouse::RButtonPressed()
{
	return m_RMousePressed;
}
BOOL CMouse::MButtonPressed()
{
	return m_MMousePressed;
}

BOOL CMouse::LButtonDown()
{
	return m_LMouseDown;
}
BOOL CMouse::RButtonDown()
{
	return m_RMouseDown;
}
BOOL CMouse::MButtonDown()
{
	return m_MMouseDown;
}

BOOL CMouse::LButtonUp()
{	
	return m_LMouseUp;
}
BOOL CMouse::RButtonUp()
{
	return m_RMouseUp;
}
BOOL CMouse::MButtonUp()
{
	return m_MMouseUp;
}

BOOL CMouse::LButtonClick()
{
	return m_LMouseUp;
}
BOOL CMouse::RButtonClick()
{
	return m_RMouseUp;
}
BOOL CMouse::MButtonClick()
{
	return m_MMouseUp;
}

BOOL CMouse::LButtonDrag()
{
	return m_LMouseDrag;
}
BOOL CMouse::RButtonDrag()
{
	return m_RMouseDrag;
}
BOOL CMouse::MButtonDrag()
{
	return m_MMouseDrag;
}



BOOL CMouse::LButtonDoubleClick()
{
	return m_LMouseDblClick;
}
BOOL CMouse::RButtonDoubleClick()
{
	return m_RMouseDblClick;
}
BOOL CMouse::MButtonDoubleClick()
{
	return m_MMouseDblClick;
}

int CMouse::GetWheel()
{
	return m_Wheel;
}
