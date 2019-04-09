// Console.h: interface for the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLE_H__CBF4D949_750B_4299_B466_82A70331A49E__INCLUDED_)
#define AFX_CONSOLE_H__CBF4D949_750B_4299_B466_82A70331A49E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "iconsole.h"

#include "TConsole_Interface.h"
#include "TConsole_Guids.h"

enum
{
	eLogDisplay,
	eLogFile,
};


class CConsole  
{
	IMHTConsole* m_pIConsole;
public:
		
	CConsole();
	virtual ~CConsole();

	//BOOL Init(int MaxButton,CUSTOM_BUTTON* pButtonInfo,void (*CommandFunc)(char* szCommand));

	BOOL Init(int MaxButton,MENU_CUSTOM_INFO* pMenuInfo,void (*CommandFunc)(char* szCommand));
	void Release();

	void WaitMessage();

	void Log(int nLogType,int LogLevel,char* strMsg,...);
	void LOG(int LogLevel,char* strMsg,...);

};

extern CConsole g_Console;

#endif // !defined(AFX_CONSOLE_H__CBF4D949_750B_4299_B466_82A70331A49E__INCLUDED_)
