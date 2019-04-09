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


// ¸®½ºÆ®ÀÇ ¸ÖÆ¼ ¶óÀÎ
// Æ¯Á¤ ¶óÀÎ ÀÐÀ» ¼ö ÀÖ°Ô
// ¸®ÇÁ·¹½¬
// ¸®½ºÆ® ¹®ÀÚ¸¦ ÆÄÀÏ·Î ±â·Ï
// 