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
	TLO_NAIVEFONT				= 0x00000001,		/* 기본 폰트 사용 */
	TLO_MULTILINE				= 0x00000002,		// not implemented
	TLO_DISABLENOSCROLL			= 0x00000004,		/* 줄 수가 리스크를 넘어갈때까지 스크롤 컨트롤 안보임 사용*/
	TLO_LINENUMBER				= 0x00000008,		/* 리스트 줄 번호 출력 사용 */
	TLO_DISABLESCROLLTUMBTRACK	= 0x00000010,		/* 스크롤 버튼 UP시 스크롤 변화 사용 */
	TLO_SCROLLTUMBTRACKUPDATE	= 0x00000020,		/* 스크롤 버튼 이동시 바로 화면 업데이트 사용 */
	TLO_SIMPLEFRAME				= 0x00000040,		/* Window Simple Frame 사용 */
	TLO_NOTMESSAGECLOSE			= 0x00000080,		/* WM_CLOSE가 왔을 때 물어 보지 않고 바로 종료 */
};
		

enum eLOGFILETYPE
{
	LFILE_LOGOVERFLOWFILEOUT	= 0x00000001,				/* wMaxLineNum 넘어가면 File Output사용 */
	LFILE_DESTROYLOGFILEOUT		= 0x00000002,				/* 콘솔 종료시 리스트 로그 기록 */
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

	DWORD	dwDrawTimeOut;					// 0 : disable timeout, 0이 아닌 값 : timeout ms

	int		Width;
	int		Height;

	int		nCustomMunuNum;
	MENU_CUSTOM_INFO * pCustomMenu;

	LOGFONT * pFont;
	cbRetrnFunc cbReturnFunc;
//	CallWndProc cbWndProc;
};

