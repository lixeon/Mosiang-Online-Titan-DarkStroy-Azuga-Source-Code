// Keyboard.h: interface for the CKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARD_H__66F7202D_E3A3_4262_8EAD_804E65D6653F__INCLUDED_)
#define AFX_KEYBOARD_H__66F7202D_E3A3_4262_8EAD_804E65D6653F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
// Å° ÄÚµå ( Virtual-Key ÄÚµå¿Í °°´Ù. )
#define KEY_LBUTTON     0x0001            // Left mouse button
#define KEY_RBUTTON     0x0002            // Right mouse button
#define KEY_CANCEL      0x0003            // Used for control-break processing
#define KEY_MBUTTON     0x0004            // Middle mouse button (3-button mouse)
#define KEY_BACK        0x0008            // BACKSPACE key
#define KEY_TAB         0x0009            // TAB key
#define KEY_CLEAR       0x000C            // CLEAR key                                                                
#define KEY_RETURN      0x000D            // RETURN key
#define KEY_SHIFT       0x0010            // SHIFT key
#define KEY_CONTROL     0x0011            // CONTROL key
#define KEY_MENU        0x0012            // MENU key
#define KEY_PAUSE       0x0013            // PAUSE key
#define KEY_CAPITAL     0x0014            // CAPITAL key
#define KEY_ESCAPE      0x001B            // ESCAPE key
#define KEY_SPACE       0x0020            // SPACEBAR
#define KEY_PGUP        0x0021            // PAGE UP key
#define KEY_PGDN        0x0022            // PAGE DOWN key
#define KEY_END         0x0023            // END key
#define KEY_HOME        0x0024            // HOME key
#define KEY_LEFT        0x0025            // LEFT ARROW key
#define KEY_UP          0x0026            // UP ARROW key
#define KEY_RIGHT       0x0027            // RIGHT ARROW key
#define KEY_DOWN        0x0028            // DOWN ARROW key
#define KEY_SELECT      0x0029            // SELECT key
#define KEY_EXCUTE      0x002B            // EXECUTE key
#define KEY_SNAPSHOT    0x002C            // PRINTSCREEN key for Windows version 3.0 and later
#define KEY_INSERT      0x002D            // INSERT key
#define KEY_DELETE      0x002E            // DELETE key
#define KEY_HELP        0x002F            // HELP key
#define KEY_0           0x0030            // 0 key
#define KEY_1           0x0031            // 1 key
#define KEY_2           0x0032            // 2 key
#define KEY_3           0x0033            // 3 key
#define KEY_4           0x0034            // 4 key
#define KEY_5           0x0035            // 5 key
#define KEY_6           0x0036            // 6 key
#define KEY_7           0x0037            // 7 key
#define KEY_8           0x0038            // 8 key
#define KEY_9           0x0039            // 9 key
#define KEY_A           0x0041            // A key
#define KEY_B           0x0042            // B key
#define KEY_C           0x0043            // C key
#define KEY_D           0x0044            // D key
#define KEY_E           0x0045            // E key
#define KEY_F           0x0046            // F key
#define KEY_G           0x0047            // G key
#define KEY_H           0x0048            // H key
#define KEY_I           0x0049            // I key
#define KEY_J           0x004A            // J key
#define KEY_K           0x004B            // K key
#define KEY_L           0x004C            // L key
#define KEY_M           0x004D            // M key
#define KEY_N           0x004E            // N key
#define KEY_O           0x004F            // O key
#define KEY_P           0x0050            // P key
#define KEY_Q           0x0051            // Q key
#define KEY_R           0x0052            // R key
#define KEY_S           0x0053            // S key
#define KEY_T           0x0054            // T key
#define KEY_U           0x0055            // U key
#define KEY_V           0x0056            // V key
#define KEY_W           0x0057            // W key
#define KEY_X           0x0058            // X key
#define KEY_Y           0x0059            // Y key
#define KEY_Z           0x005A            // Z key
#define KEY_PAD0        0x0060            // Numeric key pad 0 key
#define KEY_PAD1        0x0061            // Numeric key pad 1 key
#define KEY_PAD2        0x0062            // Numeric key pad 2 key
#define KEY_PAD3        0x0063            // Numeric key pad 3 key
#define KEY_PAD4        0x0064            // Numeric key pad 4 key
#define KEY_PAD5        0x0065            // Numeric key pad 5 key
#define KEY_PAD6        0x0066            // Numeric key pad 6 key
#define KEY_PAD7        0x0067            // Numeric key pad 7 key
#define KEY_PAD8        0x0068            // Numeric key pad 8 key
#define KEY_PAD9        0x0069            // Numeric key pad 9 key
#define KEY_MULTIPLY    0x006A            // Multiply key
#define KEY_ADD         0x006B            // Add key
#define KEY_SEPARATER   0x006C            // Separater key
#define KEY_SUBTRACT    0x006D            // Subtract key
#define KEY_DECIMAL     0x006E            // Decimal key
#define KEY_DIVIDE      0x006F            // Divide key
#define KEY_F1          0x0070            // F1 key
#define KEY_F2          0x0071            // F2 key
#define KEY_F3          0x0072            // F3 key
#define KEY_F4          0x0073            // F4 key
#define KEY_F5          0x0074            // F5 key
#define KEY_F6          0x0075            // F6 key
#define KEY_F7          0x0076            // F7 key
#define KEY_F8          0x0077            // F8 key
#define KEY_F9          0x0078            // F9 key
#define KEY_F10         0x0079            // F10 key
#define KEY_F11         0x007A            // F11 key
#define KEY_F12         0x007B            // F12 key
#define KEY_F13         0x007C            // F13 key
#define KEY_F14         0x007D            // F14 key
#define KEY_F15         0x007E            // F15 key
#define KEY_F16         0x007F            // F16 key
#define KEY_NUMLOCK     0x0090            // NUM LOCK key
#define KEY_OEM_SCROLL  0x0091            // SCROLL LOCK key
#define KEY_OEM_1       0x00BA            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_PLUS    0x00BB            // Plus (+) key
#define KEY_OEM_COMMA   0x00BC            // Comma (,) key
#define KEY_OEM_MINUS   0x00BD            // Minus (-) key
#define KEY_OEM_PERIOD  0x00BE            // Period (.) key
#define KEY_OEM_2       0x00BF            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_3       0x00C0            // ( ~ ) Å°, Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_4       0x00DB            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_5       0x00DC            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_6       0x00DD            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_7       0x00DE            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_8       0x00DF            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_9       0x00E0            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_OEM_102     0x00E2            // <> or \| on enhanced, non-U.S. IBM-compatible 102-key keyboard
*/


//
#define KEYBOARD_BUFFER			32
#include <dinput.h>
//


#define KEY_BACK        DIK_BACK          // BACKSPACE key
#define KEY_TAB         DIK_TAB           // TAB key
#define KEY_RETURN      DIK_RETURN        // RETURN key
#define KEY_PADENTER	DIK_NUMPADENTER		//NUMPAD ENTER key
#define KEY_SHIFT       DIK_LSHIFT        // SHIFT key
#define KEY_RSHIFT       DIK_RSHIFT        // SHIFT key
#define KEY_CONTROL     DIK_LCONTROL      // CONTROL key
#define KEY_MENU        DIK_LMENU            // MENU key
#define KEY_PAUSE       DIK_PAUSE            // PAUSE key
#define KEY_CAPITAL     DIK_CAPITAL            // CAPITAL key
#define KEY_ESCAPE      DIK_ESCAPE            // ESCAPE key
#define KEY_SPACE       DIK_SPACE            // SPACEBAR
#define KEY_PGUP        DIK_PGUP            // PAGE UP key
#define KEY_PGDN        DIK_PGDN            // PAGE DOWN key
#define KEY_END         DIK_END            // END key
#define KEY_HOME        DIK_HOME            // HOME key
#define KEY_LEFT        DIK_LEFT            // LEFT ARROW key
#define KEY_UP          DIK_UP            // UP ARROW key
#define KEY_RIGHT       DIK_RIGHT            // RIGHT ARROW key
#define KEY_DOWN        DIK_DOWN            // DOWN ARROW key
#define KEY_SNAPSHOT    DIK_SYSRQ            // PRINTSCREEN key for Windows version 3.0 and later
#define KEY_INSERT      DIK_INSERT            // INSERT key
#define KEY_DELETE      DIK_DELETE            // DELETE key
#define KEY_0           DIK_0            // 0 key
#define KEY_1           DIK_1            // 1 key
#define KEY_2           DIK_2            // 2 key
#define KEY_3           DIK_3            // 3 key
#define KEY_4           DIK_4            // 4 key
#define KEY_5           DIK_5            // 5 key
#define KEY_6           DIK_6            // 6 key
#define KEY_7           DIK_7            // 7 key
#define KEY_8           DIK_8            // 8 key
#define KEY_9           DIK_9            // 9 key
#define KEY_A           DIK_A            // A key
#define KEY_B           DIK_B            // B key
#define KEY_C           DIK_C            // C key
#define KEY_D           DIK_D            // D key
#define KEY_E           DIK_E            // E key
#define KEY_F           DIK_F            // F key
#define KEY_G           DIK_G            // G key
#define KEY_H           DIK_H            // H key
#define KEY_I           DIK_I            // I key
#define KEY_J           DIK_J            // J key
#define KEY_K           DIK_K            // K key
#define KEY_L           DIK_L            // L key
#define KEY_M           DIK_M            // M key
#define KEY_N           DIK_N            // N key
#define KEY_O           DIK_O            // O key
#define KEY_P           DIK_P            // P key
#define KEY_Q           DIK_Q            // Q key
#define KEY_R           DIK_R            // R key
#define KEY_S           DIK_S            // S key
#define KEY_T           DIK_T            // T key
#define KEY_U           DIK_U            // U key
#define KEY_V           DIK_V            // V key
#define KEY_W           DIK_W            // W key
#define KEY_X           DIK_X            // X key
#define KEY_Y           DIK_Y            // Y key
#define KEY_Z           DIK_Z            // Z key
#define KEY_PAD0        DIK_NUMPAD0            // Numeric key pad 0 key
#define KEY_PAD1        DIK_NUMPAD1            // Numeric key pad 1 key
#define KEY_PAD2        DIK_NUMPAD2            // Numeric key pad 2 key
#define KEY_PAD3        DIK_NUMPAD3            // Numeric key pad 3 key
#define KEY_PAD4        DIK_NUMPAD4            // Numeric key pad 4 key
#define KEY_PAD5        DIK_NUMPAD5            // Numeric key pad 5 key
#define KEY_PAD6        DIK_NUMPAD6            // Numeric key pad 6 key
#define KEY_PAD7        DIK_NUMPAD7            // Numeric key pad 7 key
#define KEY_PAD8        DIK_NUMPAD8            // Numeric key pad 8 key
#define KEY_PAD9        DIK_NUMPAD9            // Numeric key pad 9 key
#define KEY_MULTIPLY    DIK_MULTIPLY            // NumericMultiply key
#define KEY_ADD         DIK_ADD            // Numeric Add key
#define KEY_SUBTRACT    DIK_SUBTRACT            // Numeric Subtract key
#define KEY_DECIMAL     DIK_DECIMAL            // Numeric Decimal key
#define KEY_DIVIDE      DIK_DIVIDE            // Numeric Divide key
#define KEY_F1          DIK_F1            // F1 key
#define KEY_F2          DIK_F2            // F2 key
#define KEY_F3          DIK_F3            // F3 key
#define KEY_F4          DIK_F4            // F4 key
#define KEY_F5          DIK_F5            // F5 key
#define KEY_F6          DIK_F6            // F6 key
#define KEY_F7          DIK_F7            // F7 key
#define KEY_F8          DIK_F8            // F8 key
#define KEY_F9          DIK_F9            // F9 key
#define KEY_F10         DIK_F10            // F10 key
#define KEY_F11         DIK_F11            // F11 key
#define KEY_F12         DIK_F12            // F12 key
#define KEY_F13         DIK_F13            // F13 key
#define KEY_F14         DIK_F14            // F14 key
#define KEY_F15         DIK_F15            // F15 key
#define KEY_NUMLOCK     DIK_NUMLOCK            // NUM LOCK key
#define KEY_SCROLL		DIK_SCROLL            // SCROLL LOCK key
#define KEY_EQUALS		DIK_EQUALS            // Plus (+) key
#define KEY_COMMA		DIK_COMMA            // Comma (,) key
#define KEY_MINUS		DIK_MINUS            // Minus (-) key
#define KEY_PERIOD		DIK_PERIOD            // Period (.) key
#define KEY_SEMICOLON	DIK_SEMICOLON			// (:)
#define KEY_SLASH		DIK_SLASH				//(/)
#define KEY_OEM_3       DIK_GRAVE            // ( ~ ) Å°, Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_LBRACKET	DIK_LBRACKET            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_RBRACKET	DIK_RBRACKET            // Keyboard-specific punctuation key (may not appear on every keyboard)
#define KEY_BACKSLASH	DIK_BACKSLASH		//("\")
#define KEY_APOSTROPHE	DIK_APOSTROPHE         //(')
#define KEY_OEM_102     DIK_OEM_102            // <> or \| on enhanced, non-U.S. IBM-compatible 102-key keyboard





class CKeyboard  
{

protected:

	LPDIRECTINPUTDEVICE8    m_pdidKey;
	DIDEVICEOBJECTDATA		m_didodKey[KEYBOARD_BUFFER];

	DWORD	m_dwBufferedKeys;
	DWORD	m_dwBufferPos;

	BYTE	m_KeyPress[256];

	
public:
	
	CKeyboard();
	virtual ~CKeyboard();

	void Init( LPDIRECTINPUT8 pdi );
	void Release();
	void UpdateKeyboardState();

	void StartGetKeyState();
	BOOL GetNextKeyState();

	BOOL GetKeyDown( DWORD dwKey );
	BOOL GetKeyUp( DWORD dwKey );
	BOOL GetKeyPressed( DWORD dwKey );

	void ClearKeyState();
	
	BOOL IsEmptyKeyboardEvent();
	
	LPDIRECTINPUTDEVICE8 GetDirectInput() { return m_pdidKey; }
};


#endif // !defined(AFX_KEYBOARD_H__66F7202D_E3A3_4262_8EAD_804E65D6653F__INCLUDED_)
