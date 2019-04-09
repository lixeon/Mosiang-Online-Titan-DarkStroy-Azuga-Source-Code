#pragma once


enum eTCONSOLE_ERROR
{
	eTE_ERROR_SUCCESS,
	eTE_ERROR_NOTCREATE,
};

enum eTCONSOLE_EVENT
{
	eTEVE_ENTERKEY,
};

enum 
{
	eWNDEV_DEFWND = 99,
	eWNDEV_WM_DESTROY = 100,
};

enum eTLSTYLEOUT
{
	TLO_NAIVEFONT				= 0x00000001,		/* ±âº» ÆùÆ® »ç¿ë */
	TLO_MULTILINE				= 0x00000002,		// not implemented
	TLO_DISABLENOSCROLL			= 0x00000004,		/* ÁÙ ¼ö°¡ ¸®½ºÅ©¸¦ ³Ñ¾î°¥¶§±îÁö ½ºÅ©·Ñ ÄÁÆ®·Ñ ¾Èº¸ÀÓ »ç¿ë*/
	TLO_LINENUMBER				= 0x00000008,		/* ¸®½ºÆ® ÁÙ ¹øÈ£ Ãâ·Â »ç¿ë */
	TLO_DISABLESCROLLTUMBTRACK	= 0x00000010,		/* ½ºÅ©·Ñ ¹öÆ° UP½Ã ½ºÅ©·Ñ º¯È­ »ç¿ë */
	TLO_SCROLLTUMBTRACKUPDATE	= 0x00000020,		/* ½ºÅ©·Ñ ¹öÆ° ÀÌµ¿½Ã ¹Ù·Î È­¸é ¾÷µ¥ÀÌÆ® »ç¿ë */
	TLO_SIMPLEFRAME				= 0x00000040,		/* Window Simple Frame »ç¿ë */
	TLO_NOTMESSAGECLOSE			= 0x00000080,		/* WM_CLOSE°¡ ¿ÔÀ» ¶§ ¹°¾î º¸Áö ¾Ê°í ¹Ù·Î Á¾·á */
};
		

enum eLOGFILETYPE
{
	LFILE_LOGOVERFLOWFILEOUT	= 0x00000001,				/* wMaxLineNum ³Ñ¾î°¡¸é File Output»ç¿ë */
	LFILE_DESTROYLOGFILEOUT		= 0x00000002,				/* ÄÜ¼Ö Á¾·á½Ã ¸®½ºÆ® ·Î±× ±â·Ï */
};

typedef void (*cbRetrnFunc) (char * szCommand);

//typedef LRESULT WINAPI (*CallWndProc) (UINT message, WPARAM wParam, LPARAM lParam);

#define CONSOLENAME_SIZE		64
#define LOG_FILENAME_SIZE		32
#define MUNU_CUSTOMNAME_SIZE	16

#define LINE_COLOR_GREEN		RGB(0,255,0)
#define LINE_COLOR_ORANGE		RGB(212, 208, 200)
#define LINE_COLOR_PURPLE		RGB(128, 128, 200)
#define LINE_COLOR_WHITE		RGB(255, 255, 255)

struct MENU_CUSTOM_INFO
{
	char szMenuName[MUNU_CUSTOMNAME_SIZE];
	void (*cbMenuCommand)();
};

struct MHTCONSOLE_DESC
{
	char	* szConsoleName;
	DWORD	dwRefreshRate;

	WORD	wLogFileType;
	char	* szLogFileName;
	DWORD	dwFlushFileBufferSize;
	//list
	WORD	wMaxLineNum;
	DWORD	dwListStyle;

	DWORD	dwDrawTimeOut;					// 0 : disable timeout, 0ÀÌ ¾Æ´Ñ °ª : timeout ms

	int		Width;
	int		Height;

	int		nCustomMunuNum;
	MENU_CUSTOM_INFO * pCustomMenu;

	LOGFONT * pFont;
	cbRetrnFunc cbReturnFunc;
//	CallWndProc cbWndProc;
};

