#include "TConsole.h"
#include "TConsole_Guids.h"
#include "TList.h"
//#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NewCmdEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LONG OldCmdEditProc;
extern HMODULE g_hModule;
//CTList * m_pTList = NULL;
//HWND g_hEdit = NULL;
#define		EDITBOX_HEIGHT		30
CTConsole * CTConsole::m_pTConsole = NULL;
CTConsole::CTConsole():
m_cRef(1),
m_hEdit(NULL),
m_pTList(NULL),
m_dwFileBufferCP(0),
m_FileBuffer(NULL),
m_hDrawEvent(NULL),
m_hWnd(NULL),
m_hInst(NULL)
{
}
CTConsole::~CTConsole()
{
	if(m_hDrawEvent) KillTimer(m_hWnd, 1);

	if(m_DESC.wLogFileType & LFILE_DESTROYLOGFILEOUT)
		FlushFileAll();

	ForceFlushFile();

	DestroyList();
	if(m_FileBuffer)
	{
		delete [] m_FileBuffer;
		m_FileBuffer = NULL;
	}
}
CTConsole * CTConsole::GetInstance()
{
	return m_pTConsole;
}
HWND CTConsole::GetEditHWnd()
{
	return m_hEdit;
}
CTList * CTConsole::GetTList()
{
	return m_pTList;
}
BOOL CTConsole::IsStyle(DWORD dwStyle)
{
	return (dwStyle & m_DESC.dwListStyle);
}
void CTConsole::CreateList(LOGFONT * pFont)
{
	DestroyList();

	m_pTList = new CTList;
	m_pTList->CreateList(m_DESC.dwListStyle, 10, 10, (m_DESC.Width - 20), (m_DESC.Height - 70), m_DESC.wMaxLineNum, 100, m_hWnd, m_hInst);
	if(!(m_DESC.dwListStyle & TL_NAIVEFONT))
		m_pTList->CreateFont(pFont);
	m_pTList->SetLineHeightMargin(2);
	
	
}

void CTConsole::CreateEdit()
{
	if(m_DESC.dwListStyle & TLO_SIMPLEFRAME)
	{	
		m_hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER ,
											10, (m_DESC.Height - 55), (m_DESC.Width - 20), EDITBOX_HEIGHT, 
											m_hWnd, 0, m_hInst, NULL);
	}
	else
	{
		m_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER ,
											10, (m_DESC.Height - 55), (m_DESC.Width - 20), EDITBOX_HEIGHT, 
											m_hWnd, 0, m_hInst, NULL);
	}
	OldCmdEditProc = SetWindowLong(m_hEdit, GWL_WNDPROC, (LONG)NewCmdEditProc);
	SetFocus(m_hEdit);
}
void CTConsole::DestroyList()
{
	if(m_pTList)
	{
		delete m_pTList;
		m_pTList = NULL;
	}
}

void CTConsole::AddCustomMenu(MENU_CUSTOM_INFO * pMenu, int num)
{
	int StartMenuID = CUSTOM_MENU_ID;
	HMENU MenuBar = GetSubMenu(GetMenu(m_hWnd), 1);
	for(int i = 0 ; i < num ; ++i)
	{
		InsertMenu(MenuBar, i, MF_BYPOSITION, StartMenuID, pMenu[i].szMenuName);
		++StartMenuID;
	}
}
void CTConsole::ExecuteMenu(int menuID)
{
	if(menuID < m_DESC.nCustomMunuNum)
		m_DESC.pCustomMenu[menuID].cbMenuCommand();
}

void CTConsole::CallBackCommand(char * command)
{
	if(m_DESC.cbReturnFunc)
		m_DESC.cbReturnFunc(command);
}
eTCONSOLE_ERROR __stdcall CTConsole::CreateConsole(MHTCONSOLE_DESC * pDesc, HWND * hWndOut)
{
	//SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
	m_pTConsole = this;

	if(pDesc->szConsoleName)
	strncpy(m_DESC.szConsoleName, pDesc->szConsoleName, CONSOLENAME_SIZE);
	m_DESC.dwRefreshRate				= pDesc->dwRefreshRate;
	m_DESC.wLogFileType					= pDesc->wLogFileType;
	if(pDesc->szLogFileName)
	strncpy(m_DESC.szLogFileName, pDesc->szLogFileName, LOG_FILENAME_SIZE);
	m_DESC.dwFlushFileBufferSize		= pDesc->dwFlushFileBufferSize;
	m_DESC.wMaxLineNum					= pDesc->wMaxLineNum;
	m_DESC.dwListStyle					= pDesc->dwListStyle;
	m_DESC.dwDrawTimeOut				= pDesc->dwDrawTimeOut;
	m_DESC.Width						= pDesc->Width;
	m_DESC.Height						= pDesc->Height;
	m_DESC.cbReturnFunc					= pDesc->cbReturnFunc;
	m_DESC.nCustomMunuNum				= pDesc->nCustomMunuNum;
	if(m_DESC.nCustomMunuNum)
		m_DESC.pCustomMenu					= new MENU_CUSTOM_INFO[m_DESC.nCustomMunuNum];
	for(int i = 0 ; i < m_DESC.nCustomMunuNum ; ++i)
	{
		if(pDesc->pCustomMenu[i].szMenuName)
		strncpy(m_DESC.pCustomMenu[i].szMenuName, pDesc->pCustomMenu[i].szMenuName, MUNU_CUSTOMNAME_SIZE);
		m_DESC.pCustomMenu[i].cbMenuCommand = pDesc->pCustomMenu[i].cbMenuCommand;
	}

	m_hInst = g_hModule;

	ConsoleRegisterClass(m_hInst);

	if(!InitInstance(m_hInst)) 
	{
		return eTE_ERROR_NOTCREATE;
	}

	CreateList(pDesc->pFont);
	CreateEdit();
	SetDispRefreshRate(m_DESC.dwRefreshRate);

	AddCustomMenu(m_DESC.pCustomMenu, m_DESC.nCustomMunuNum);

	m_FileBuffer = new char[m_DESC.dwFlushFileBufferSize];

	hWndOut = &m_hWnd;
	return eTE_ERROR_SUCCESS;
}

BOOL CTConsole::InitInstance(HINSTANCE hInstance)
{
	m_hInst = hInstance;
	
	int cx = m_DESC.Width+GetSystemMetrics(SM_CXSIZEFRAME)*2;
	int cy = m_DESC.Height+GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYMENU);
	
	m_hWnd = CreateWindow(m_DESC.szConsoleName, m_DESC.szConsoleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, cx, cy, NULL, NULL, m_hInst, NULL);
	
	if(!m_hWnd)
	{
		return FALSE;
	}
	
	ShowWindow(m_hWnd, 1);
	UpdateWindow(m_hWnd);
	
	return TRUE;
}

ATOM CTConsole::ConsoleRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TCONSOLEICON);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= (LPCSTR)IDR_MENU;
	wcex.lpszClassName	= m_DESC.szConsoleName;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


void __stdcall CTConsole::OutputDisplayEx(char * szString, int strLen, DWORD color)
{
	int count = m_pTList->AddString(szString, color);
	if(count > m_DESC.wMaxLineNum)
	{
		if(m_DESC.wLogFileType & LFILE_LOGOVERFLOWFILEOUT)
		{
			char szStr[256];
			m_pTList->DeleteString(szStr);
			OutputFile(szStr, strlen(szStr));
		}
		else
		{
			m_pTList->DeleteString(NULL);
		}
	}
}

void __stdcall CTConsole::OutputDisplay(char * szString, int strLen)
{
	int count = m_pTList->AddString(szString);
	if(count > m_DESC.wMaxLineNum)
	{
		if(m_DESC.wLogFileType & LFILE_LOGOVERFLOWFILEOUT)
		{
			char szStr[256];
			m_pTList->DeleteString(szStr);
			OutputFile(szStr, strlen(szStr));
		}
		else
		{
			m_pTList->DeleteString(NULL);
		}
	}
}

void __stdcall CTConsole::OutputFile(char * szString, int strLen)
{
	if(m_dwFileBufferCP + strLen >= m_DESC.dwFlushFileBufferSize)
	{
		ForceFlushFile();
	}
	strncpy(&m_FileBuffer[m_dwFileBufferCP], szString, strLen);
	m_FileBuffer[m_dwFileBufferCP + strLen] = 0;
	m_dwFileBufferCP += strLen;
}
void __stdcall CTConsole::FlushFileAll()
{
	char szStr[256];

	while(m_pTList->DeleteString(szStr))
		OutputFile(szStr, strlen(szStr));

	ForceFlushFile();
}

void __stdcall CTConsole::SetDispRefreshRate(DWORD dwTickRate)
{
	m_DESC.dwRefreshRate = dwTickRate;
	if(m_hDrawEvent) KillTimer(m_hWnd, 1);
	m_hDrawEvent = (HANDLE)SetTimer(m_hWnd, 1, dwTickRate, NULL);
}
DWORD __stdcall CTConsole::GetDispRefreshRate()
{
	return m_DESC.dwRefreshRate;
}

void __stdcall CTConsole::ForceRefreshDisplay()
{
	if(m_pTList)
		m_pTList->OnDraw(TRUE);
}

void __stdcall CTConsole::ForceFlushFile()
{
	/*
	FILE* fp = fopen(m_DESC.szLogFileName,"a+");
	if(fp)
	{
		fprintf(fp,m_FileBuffer);
		m_dwFileBufferCP = 0;
		m_FileBuffer[0] = 0;
		fclose(fp);
	}
	*/
	HANDLE hFile = CreateFile(m_DESC.szLogFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile)
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
		DWORD dwWritten = 0;
		WriteFile(hFile, m_FileBuffer, m_dwFileBufferCP, &dwWritten, NULL);
		m_dwFileBufferCP -= dwWritten;
		CloseHandle(hFile);
	}
	
}

void __stdcall CTConsole::MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (msg.message == WM_QUIT) 
			return;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void CTConsole::DrawTimeoutCheck()
{
	if(m_DESC.dwDrawTimeOut != 0 
		&& !m_DESC.bDisableDraw && (GetTickCount() - m_DESC.dwDrawLastTime > m_DESC.dwDrawTimeOut) )
		m_DESC.bDisableDraw = TRUE;
}
void CTConsole::DrawTimeoutUpdate()
{
	m_DESC.dwDrawLastTime = GetTickCount();
	if(m_DESC.bDisableDraw)
	{
		m_pTList->Draw();
		m_DESC.bDisableDraw = FALSE;
	}
	
}
BOOL CTConsole::IsDrawTimeOut()
{
	return m_DESC.bDisableDraw;
}
LRESULT __stdcall CTConsole::CallDefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return eWNDEV_DEFWND;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
/*
	switch(m_DESC.cbWndProc(message, wParam, lParam))
	{
	case eWNDEV_DEFWND:
		{
			return DefWindowProc(m_hWnd, message, wParam, lParam);
		}
		break;
	case eWNDEV_WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}*/

	//CTConsole * pTConsole = (CTConsole *)::GetWindowLong(hWnd, GWL_USERDATA);
	CTConsole * pTConsole = CTConsole::GetInstance();
	CTList * pTList = NULL;
	if(pTConsole)
	{
		pTList = pTConsole->GetTList();
		if(pTList)
			pTList->MessageProc(message, wParam, lParam);
	}
		
	

	RECT crt;
	PAINTSTRUCT ps;
	HDC hdc;
	WORD wmId, wmEvent;
	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		if(wmId >= CUSTOM_MENU_ID && wmId <= CUSTOM_MENU_ID+100)
		{
			if(pTConsole)
			{
				pTConsole->ExecuteMenu(wmId - CUSTOM_MENU_ID);
			}
			return 0;
		}

		switch (wmId)
		{
		case IDM_FLUSHFILE:
			{
				if(pTConsole)
					pTConsole->ForceFlushFile();
			}
			break;
		case IDM_ABOUT:
			DialogBox(g_hModule, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			if(pTConsole && !pTConsole->IsStyle(TLO_NOTMESSAGECLOSE))
			{
				if(IDYES == MessageBox(hWnd, "Do you want to close this console?", "Warning - MHConsole", MB_YESNO))
					DestroyWindow(hWnd);
			}
			else
				DestroyWindow(hWnd);
			//PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &crt);
			int Width = crt.right - crt.left;
			int Height = crt.bottom - crt.top;
			if(pTConsole)
			{
				if(pTConsole->GetEditHWnd())
					MoveWindow(pTConsole->GetEditHWnd(), 10, (Height - 55), (Width - 20), EDITBOX_HEIGHT, TRUE);	

				if(pTList)
					pTList->SetSize((Width - 20), (Height - 70));
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(pTConsole)
		{
			if(pTList)
				pTList->Draw();
		}
		EndPaint(hWnd, &ps);
		
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_KEYDOWN:
	case WM_MOUSEMOVE:
		if(pTConsole)
		{
			pTConsole->DrawTimeoutUpdate();
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_TIMER:
		if(pTConsole)
		{
			pTConsole->DrawTimeoutCheck();
			if(pTList && !pTConsole->IsDrawTimeOut())
				pTList->OnDraw();
		}
		break;
	case WM_CLOSE:
		if(pTConsole && !pTConsole->IsStyle(TLO_NOTMESSAGECLOSE))
		{
			if(IDYES == MessageBox(hWnd, "Do you want to close this console?", "Warning - MHConsole", MB_YESNO))
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:	
			//MessageBox(hWnd, "Do", NULL, NULL);
			PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK NewCmdEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
	case WM_CHAR: 
        switch (wParam) 
        { 
		case 0x0D:
			{
				
				//CTConsole * pTConsole = (CTConsole *)::GetWindowLong(hWnd, GWL_USERDATA);
				CTConsole * pTConsole = CTConsole::GetInstance();
				if(!pTConsole) break;

				pTConsole->DrawTimeoutUpdate();
				static char buff[256];
				SendMessage(hWnd, WM_GETTEXT, (WPARAM)256, (LPARAM)buff);
				if(strcmp(buff, "") != 0)
				{
					pTConsole->CallBackCommand(buff);
					pTConsole->OutputDisplay(buff, strlen(buff));	
					SendMessage(hWnd, WM_SETTEXT, 0,0);
				}
			}
			break;
		}
		break;
	}
	return CallWindowProc((WNDPROC)OldCmdEditProc,hWnd,message,wParam,lParam);
}

HRESULT __stdcall CTConsole::QueryInterface(const IID& iid, void ** ppv)
{
	if(iid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if(iid == IID_ITConsole)
	{
		*ppv = static_cast<IMHTConsole*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}
ULONG __stdcall CTConsole::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}
ULONG __stdcall CTConsole::Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}







// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}