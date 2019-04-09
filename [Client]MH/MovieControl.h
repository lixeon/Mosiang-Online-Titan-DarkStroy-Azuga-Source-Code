#ifndef _MOVIECONTROL_H
#define _MOVIECONTROL_H

#include <dshow.h>
#include <commctrl.h>
#include <commdlg.h>

#pragma comment(lib, "Strmiids")
#pragma comment(lib, "Quartz")

#define WM_GRAPHNOTIFY WM_APP+1

class MovieControl
{
public:
	MovieControl();
	~MovieControl();

	bool SetWindowHandle(HWND hWnd); //윈도우 핸들 설정
	HWND GetWindowHandle(); //윈도우 핸들 얻기
	bool ResisterFile(LPCWSTR sFile); //문자열 앞에 L을 붙이면 유니코드문자인 LPCWSTR로 인식

	virtual void Destroy(); 
	virtual void PlayMovie(); //동영상 시작	
	virtual void MovieStop(); // 동영상 스톱
	virtual bool HandleEvent(); //동영상 종료 확인

private:

	bool m_bMovieOver; //동영상 종료 유무 클래그 변수

	IGraphBuilder   *m_pGraph; 
	IVideoWindow    *m_pVidWin; //해당 인터페이스로 해상도, 풀스크린 여부 결정
	IMediaControl   *m_pMediaControl; 
	IMediaEventEx   *m_pEvent;

	LPCWSTR m_lsFile;
	//int m_nLeft, m_nTop, m_nRight, m_nBottom;

	HWND m_hWnd;
};


#endif _MOVIECONTROL_H