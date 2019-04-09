#pragma once

#include "TConsole_Interface.h"


#define CUSTOM_MENU_ID		41001

/*
<아직 해야 할 일>
#######타이틀 아이콘 삭제
#######Visual한 틀 제작
3. 메모리 사용 줄이기
4. 에러 처리
5. 리스트 메모리 풀 사용
6. 메시지 콜백 처리
*/

class CTList;

class MHTCONSOLE_DESC_IN
{
public:
	MHTCONSOLE_DESC_IN()
	{
		pCustomMenu = NULL;
		dwDrawLastTime = 0;
		bDisableDraw = FALSE;
	}
	~MHTCONSOLE_DESC_IN()
	{
		if(pCustomMenu)
		{
			delete [] pCustomMenu;
			pCustomMenu = NULL;
		}
	}
	char	szConsoleName[CONSOLENAME_SIZE];
	DWORD	dwRefreshRate;

	WORD	wLogFileType;
	char	szLogFileName[LOG_FILENAME_SIZE];
	DWORD	dwFlushFileBufferSize;
	//list
	WORD	wMaxLineNum;
	DWORD	dwListStyle;
	DWORD	dwDrawLastTime;
	DWORD	dwDrawTimeOut;
	BOOL	bDisableDraw;

	int		Width;
	int		Height;

	int		nCustomMunuNum;
	MENU_CUSTOM_INFO * pCustomMenu;
	cbRetrnFunc cbReturnFunc;
//	CallWndProc cbWndProc;
};


class CTConsole : public IMHTConsole
{
public:

	CTConsole();
	virtual ~CTConsole();

	virtual HRESULT __stdcall QueryInterface(const IID& iid, void ** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();


	virtual eTCONSOLE_ERROR __stdcall CreateConsole(MHTCONSOLE_DESC * pDesc, HWND * hWndOut);
	virtual void __stdcall OutputDisplay(char * szString, int strLen);
	virtual void __stdcall OutputDisplayEx(char * szString, int strLen, DWORD color);
	virtual void __stdcall OutputFile(char * szString, int strLen);


	virtual void __stdcall SetDispRefreshRate(DWORD dwTickRate);
	virtual DWORD __stdcall GetDispRefreshRate();
	
	virtual void __stdcall ForceRefreshDisplay();
	virtual void __stdcall ForceFlushFile();
	virtual void __stdcall FlushFileAll();

	virtual void __stdcall MessageLoop();

	virtual LRESULT __stdcall CallDefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	void CreateList(LOGFONT * pFont);
	void CreateEdit();
	void DestroyList();
	void AddCustomMenu(MENU_CUSTOM_INFO * pMenu, int num);
	void ExecuteMenu(int menuID);
	BOOL IsStyle(DWORD dwStyle);

	static CTConsole * GetInstance();
	CTList * GetTList();
	HWND GetEditHWnd();
	void CallBackCommand(char * command);
	void DrawTimeoutUpdate();
	BOOL IsDrawTimeOut();
	void DrawTimeoutCheck();
private:
	BOOL InitInstance(HINSTANCE hInstance);
	ATOM ConsoleRegisterClass(HINSTANCE hInstance);
	
	MHTCONSOLE_DESC_IN m_DESC;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	LONG m_cRef;
	HANDLE m_hDrawEvent;
	HWND m_hEdit;
	CTList * m_pTList;
	static CTConsole * m_pTConsole;

	char * m_FileBuffer;
	DWORD m_dwFileBufferCP;
};
