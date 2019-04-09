// Mouse.cpp: implementation of the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mouse.h"

extern HWND _g_hWnd;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouse::CMouse()
{
	m_pMouseInfo	= NULL;
	m_nWheelMsg		= 0;
	m_nQueSize		= MAX_QUESIZE;
}

CMouse::~CMouse()
{

}

//init들어오기전에 마우스 이벤트가 나올수 있다. 고치잣! confirm
void CMouse::Init()
{
//Init que	
	m_nQueSize		= MAX_QUESIZE;
	m_pMouseInfo	= new sMOUSEINFO[m_nQueSize];
	m_nHead = 0;
	m_nTail = 0;
//

//Init Mouse
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( _g_hWnd, &pt );
	m_nMouseOldX = m_nMouseX = pt.x;
	m_nMouseOldY = m_nMouseY = pt.y;
	m_nMouseEventX = 0;
	m_nMouseEventY = 0;

	m_nWheelMsg		= 0;
	m_nCombineKey	= 0;
//
}

void CMouse::Release()
{
	SAFE_DELETE( m_pMouseInfo );
}

void CMouse::GetWindowMessage( HWND hWnd, DWORD wmsg, WPARAM wParam, LPARAM lParam )
{
	switch(wmsg)
	{
	case WM_LBUTTONDOWN:	SetCapture( _g_hWnd ); RecordMouseEvent( eME_LDOWN, wParam, lParam );	break;
	case WM_RBUTTONDOWN:	SetCapture( _g_hWnd ); RecordMouseEvent( eME_RDOWN, wParam, lParam );	break;
	case WM_MBUTTONDOWN:	SetCapture( _g_hWnd ); RecordMouseEvent( eME_MDOWN, wParam, lParam );	break;
	case WM_LBUTTONUP:		ReleaseCapture(); RecordMouseEvent( eME_LUP, wParam, lParam );	break;
	case WM_RBUTTONUP:		ReleaseCapture(); RecordMouseEvent( eME_RUP, wParam, lParam );	break;
	case WM_MBUTTONUP:		ReleaseCapture(); RecordMouseEvent( eME_MUP, wParam, lParam );	break;
	case WM_LBUTTONDBLCLK:	//RecordMouseEvent( eME_LDOWN, wParam, lParam );
							RecordMouseEvent( eME_LDBL, wParam, lParam );	break;
	case WM_RBUTTONDBLCLK:	//RecordMouseEvent( eME_RDOWN, wParam, lParam );
							RecordMouseEvent( eME_RDBL, wParam, lParam );	break;
	case WM_MBUTTONDBLCLK:	//RecordMouseEvent( eME_MDOWN, wParam, lParam );
							RecordMouseEvent( eME_MDBL, wParam, lParam );	break;
	case 0x020A/*WM_MOUSEWHEEL*/:		m_nWheelMsg = int(wParam)>>16;	break;	//for wheel
	}	
}

void CMouse::RecordMouseEvent( int nEvent, WPARAM wParam, LPARAM lParam )
{
	sMOUSEINFO MouseInfo;

	MouseInfo.nEvent	= nEvent;
	MouseInfo.nX		= LOWORD(lParam);
	MouseInfo.nY		= HIWORD(lParam);
	MouseInfo.nKey		= wParam;

	PushMouseInfo( &MouseInfo );
}



void CMouse::GetNextMouseEvent()
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( _g_hWnd, &pt );
	m_nMouseX	= pt.x;
	m_nMouseY	= pt.y;

	m_nMouseAxisX	= m_nMouseOldX - m_nMouseX;
	m_nMouseAxisY	= m_nMouseOldY - m_nMouseY;

	m_nMouseOldX	= m_nMouseX;
	m_nMouseOldY	= m_nMouseY;

	m_nWheel		= m_nWheelMsg;
	m_nWheelMsg		= 0;

	for( int i = 0 ; i < eME_COUNT ; ++i )
		m_bMouseEvent[i] = FALSE;

	sMOUSEINFO MouseInfo;
	if( PopMouseInfo( &MouseInfo ) == TRUE )
	{
		ParseCurMouseEvent( &MouseInfo );
	}

	if( m_nMouseAxisX || m_nMouseAxisY )
	{
		if( LButtonPressed() )
			m_bLBtnDrag = TRUE;
		if( RButtonPressed() )	m_bRBtnDrag = TRUE;
		if( MButtonPressed() )	m_bMBtnDrag = TRUE;
	}
}

void CMouse::ParseCurMouseEvent( sMOUSEINFO* pMouseInfo )
{
	m_bMouseEvent[pMouseInfo->nEvent] = TRUE;
	
	switch( pMouseInfo->nEvent )
	{
	case eME_LDOWN:		m_bLBtnPress	= TRUE;		break;
	case eME_RDOWN:		m_bRBtnPress	= TRUE;		break;
	case eME_MDOWN:		m_bMBtnPress	= TRUE;		break;
	case eME_LUP:		m_bLBtnPress	= FALSE;	m_bLBtnDrag = FALSE;	break;
	case eME_RUP:		m_bRBtnPress	= FALSE;	m_bRBtnDrag = FALSE;	break;
	case eME_MUP:		m_bMBtnPress	= FALSE;	m_bMBtnDrag = FALSE;	break;
	case eME_LDBL:		break;
	case eME_RDBL:		break;
	case eME_MDBL:		break;
	}

	m_nMouseEventX	= pMouseInfo->nX;
	m_nMouseEventY	= pMouseInfo->nY;
	m_nCombineKey	= pMouseInfo->nKey;
}

BOOL CMouse::PushMouseInfo( sMOUSEINFO* pIn )
{
	int nTail = ( m_nTail + 1 ) % m_nQueSize;

	if( nTail == m_nHead ) return FALSE;

	m_pMouseInfo[m_nTail] = *pIn;
	m_nTail = nTail;

	return TRUE;
}

BOOL CMouse::PopMouseInfo( sMOUSEINFO* pOut )
{
	if( m_nTail == m_nHead ) return FALSE;

	*pOut = m_pMouseInfo[m_nHead];

	m_nHead = ( m_nHead + 1 ) % m_nQueSize;

	return TRUE;
}
