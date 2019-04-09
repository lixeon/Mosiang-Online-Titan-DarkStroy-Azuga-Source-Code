// Keyboard.cpp: implementation of the CKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Keyboard.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CKeyboard g_keyboard;
extern HWND _g_hWnd;

CKeyboard::CKeyboard()
{
	m_pdidKey = NULL;
}

CKeyboard::~CKeyboard()
{
}

void CKeyboard::Init( LPDIRECTINPUT8 pdi )
{
	HRESULT hr;
	DWORD dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY;

    hr = pdi->CreateDevice( GUID_SysKeyboard, &m_pdidKey, NULL );
    hr |= m_pdidKey->SetDataFormat( &c_dfDIKeyboard );
    hr |= m_pdidKey->SetCooperativeLevel( _g_hWnd, dwCoopFlags );


// ¹öÆÛ¸ðµå
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = KEYBOARD_BUFFER;

	hr = m_pdidKey->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

	m_pdidKey->Acquire();

	ClearKeyState();

}


void CKeyboard::Release()
{
	if( m_pdidKey )
		m_pdidKey->Unacquire();

	SAFE_RELEASE( m_pdidKey );
}


void CKeyboard::UpdateKeyboardState()
{
	HRESULT hr;

	m_dwBufferedKeys = KEYBOARD_BUFFER;
	
	hr = m_pdidKey->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
								   m_didodKey, &m_dwBufferedKeys, 0 );
	if( FAILED( hr ) ) {

		do
		{
			hr = m_pdidKey->Acquire();
		}
		while( hr == DIERR_INPUTLOST );
		
		ClearKeyState();
	}
	else
	{
		m_dwBufferPos = m_dwBufferedKeys;	//¸¶Áö¸·Å°¸¦ ¹ÞÀ» ¼ö ÀÖµµ·Ï
	}
}


void CKeyboard::StartGetKeyState()
{
	m_dwBufferPos = 0;
}


BOOL CKeyboard::GetNextKeyState()
{
	++m_dwBufferPos;

	if( m_dwBufferPos > m_dwBufferedKeys )
		return FALSE;

//PressKey
	if( m_didodKey[m_dwBufferPos-1].dwData & 0x80 )
	{
		m_KeyPress[m_didodKey[m_dwBufferPos-1].dwOfs] = TRUE;
	}
	else
	{
		m_KeyPress[m_didodKey[m_dwBufferPos-1].dwOfs] = FALSE;
	}

	return TRUE;
}


BOOL CKeyboard::GetKeyDown( DWORD dwKey )
{
	if( m_dwBufferedKeys == 0 ) return FALSE;
	if( m_dwBufferPos > m_dwBufferedKeys || m_dwBufferPos < 1 )
		return FALSE;

	if(( m_didodKey[m_dwBufferPos-1].dwOfs == dwKey ) && ( m_didodKey[m_dwBufferPos-1].dwData & 0x80 ) )
		return TRUE;

	return FALSE;
}


BOOL CKeyboard::GetKeyUp( DWORD dwKey )
{
	if( m_dwBufferedKeys == 0 ) return FALSE;
	if( m_dwBufferPos > m_dwBufferedKeys || m_dwBufferPos < 1 )
		return FALSE;

	ASSERT( m_dwBufferPos <= m_dwBufferedKeys && m_dwBufferPos > 0 );

	if(( m_didodKey[m_dwBufferPos-1].dwOfs == dwKey ) && !( m_didodKey[m_dwBufferPos-1].dwData & 0x80 ) )
		return TRUE;

	return FALSE;
}


BOOL CKeyboard::GetKeyPressed( DWORD dwKey )
{
	return m_KeyPress[dwKey];
}



void CKeyboard::ClearKeyState()
{
	m_dwBufferedKeys	= 0;
	m_dwBufferPos		= 1;

	ZeroMemory( m_KeyPress, sizeof( m_KeyPress ) );
}


BOOL CKeyboard::IsEmptyKeyboardEvent()
{
	if( m_dwBufferPos > m_dwBufferedKeys )
		return TRUE;

	return FALSE;
}