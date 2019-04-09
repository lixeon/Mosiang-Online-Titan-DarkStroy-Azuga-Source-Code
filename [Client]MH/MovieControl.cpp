#include "StdAfx.h"
#include "MovieControl.h"

MovieControl::MovieControl()
{
	m_hWnd = NULL;
	m_bMovieOver = false;
	m_lsFile = NULL;
	m_pGraph = NULL;
	m_pVidWin = NULL;
	m_pMediaControl = NULL;
	m_pEvent = NULL;
}

MovieControl::~MovieControl()
{
	Destroy();
}

bool MovieControl::SetWindowHandle(HWND hWnd)
{
	if(hWnd == NULL)
	{		
		return false;
	}
	
	m_hWnd = hWnd;

	return true;
}

HWND MovieControl::GetWindowHandle()
{
	return m_hWnd;
}

bool MovieControl::ResisterFile(LPCWSTR sFile)
{
	m_lsFile = sFile;

	if(m_lsFile == NULL)
	{
		return false;
	}

	//DirectShow 절차
	//1. Filter Graph Manager의 인스턴스를 생성한다.
	//2. 필터 그래프를 구성하기 위해 필터 그래프 매니저를 사용한다.
	//3. 데이터가 필터로 통과하면서 그래프를 실행한다.

	HWND hWnd = NULL;
	hWnd = MovieControl::GetWindowHandle();

	if(hWnd == NULL)
	{
		return false;
	}

	RECT rc;
	CoInitialize(NULL); //COM관련 라이브러리를 사용하려면 CoInitialize(NUUL)를 먼저 호출해서 초기화 함

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder,	reinterpret_cast <void **>(&m_pGraph));

	HRESULT hr;
	hr = m_pGraph->RenderFile(m_lsFile, NULL); 
	if(FAILED(hr))
	{
		return false;
	}

	//QueryInterface()
	//첫번째 인자는 그 COM 객체를 사용하는 코드에서 COM객체에게 뭐 좀 달라고 요청하기 위한 인자입니다.
	//요청에 대해 두번째 인자로 받게 됩니다.
	m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVidWin);  
	m_pVidWin->HideCursor(OATRUE);	
	//IVideoWindow 인터페이스에 윈도우 해상도 설정
	m_pVidWin->put_Owner((OAHWND)hWnd); 

	m_pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);	
	GetClientRect(hWnd,&rc);
	m_pVidWin->SetWindowPosition(rc.left, rc.top, rc.right, rc.bottom);

	m_pGraph->QueryInterface(IID_IMediaEventEx, (void **)&m_pEvent); 
	m_pEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0); 

	m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl); 

	return true;
}

void MovieControl::PlayMovie()
{
	m_pMediaControl->Run(); 
	long ev;
	m_pEvent->WaitForCompletion(INFINITE, &ev);
}

void MovieControl::MovieStop()
{
	m_pMediaControl->Stop();
}

void MovieControl::Destroy()
{		
	if(m_bMovieOver)
	{
		m_pMediaControl->Stop();
	}

	if(m_pMediaControl)
		m_pMediaControl->Release(); 
		
	if(m_pVidWin)
	{
		m_pVidWin->put_Visible(OAFALSE);
		m_pVidWin->put_Owner(NULL);
		m_pVidWin->Release();
	}
	
	if(m_pEvent)
		m_pEvent->Release();	
	
	if(m_pGraph)
		m_pGraph->Release(); 	
	
	CoUninitialize();
}

bool MovieControl::HandleEvent() 
{ 
	long evCode, param1, param2; 
	if(m_pEvent != NULL)
	{
		HRESULT hr; 
		while (hr = m_pEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(hr)) 
		{ 
			hr = m_pEvent->FreeEventParams(evCode, param1, param2); 
			if ((EC_COMPLETE == evCode) || (EC_USERABORT == evCode)) 
			{ 			
				m_bMovieOver = true;
				break; 
			} 
		} 
	}	

	if(m_bMovieOver)
	{
		return true;
	}
	else
	{
		return false;
	}
} 