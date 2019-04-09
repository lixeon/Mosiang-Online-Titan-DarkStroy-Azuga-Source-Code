#include <unknwn.h>
#include "TConsole_Def.h"

interface IMHTConsole : IUnknown
{
	virtual eTCONSOLE_ERROR __stdcall CreateConsole(MHTCONSOLE_DESC * pDesc, HWND * hWndOut) = 0;
	virtual void __stdcall OutputDisplay(char * szString, int strLen) = 0;
	virtual void __stdcall OutputFile(char * szString, int strLen) = 0;


	virtual void __stdcall SetDispRefreshRate(DWORD dwTickRate) = 0;
	virtual DWORD __stdcall GetDispRefreshRate() = 0;
	
	virtual void __stdcall ForceRefreshDisplay() = 0;
	virtual void __stdcall ForceFlushFile() = 0;
	virtual void __stdcall FlushFileAll() = 0;
	
	virtual void __stdcall MessageLoop() = 0;

	virtual LRESULT __stdcall CallDefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)=0;
};


// 리스트의 멀티 라인
// 특정 라인 읽을 수 있게
// 리프레쉬
// 리스트 문자를 파일로 기록
// 