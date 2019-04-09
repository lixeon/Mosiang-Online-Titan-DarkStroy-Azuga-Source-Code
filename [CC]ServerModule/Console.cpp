// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Console.h"
#include <stdio.H>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CConsole g_Console;

CConsole::CConsole()
{
	m_pIConsole = NULL;
//	CoInitialize(NULL);
}

CConsole::~CConsole()
{
//	CoUninitialize();
}

/*
BOOL CConsole::Init(int MaxButton,CUSTOM_BUTTON* pButtonInfo, void (*CommandFunc)(char* szCommand))
{	

	// ¿ì¼± I4DyuchiCOSOLEÄÄÆ÷³ÍÆ® »ý¼º
	HRESULT hr;
    
	hr = CoCreateInstance(
           CLSID_4DyuchiCONSOLE,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiCONSOLE,
           (void**)&m_pIConsole);

	
	if(FAILED(hr))
		return FALSE;

	// ÄÄÆ÷³ÍÆ®ÀÇ ÄÜ¼ÖÀ» »ý¼º.»ý¼ºÇÏ±â À§ÇØ DESC_CONSOLE±¸Á¶Ã¼¸¦ Ã¤¿ö¼­ ±× Æ÷ÀÎÅÍ¸¦ ³Ö¾î¾ßÇÑ´Ù.
	// DESC_CONSOLE±¸Á¶Ã¼´Â ÆÄÀÏ·Î±ëÀ» À§ÇÑ FILE_WRITE_INFO¿Í È­¸é·Î±ëÀ» À§ÇÑ DISPLAY_INFO
	// ¸¦ Æ÷ÇÔÇÑ´Ù. FILE_WRITE_INFOÀº ÆÄÀÏ·Î±ëÀ» ÇÏÁö ¾ÊÀ» °æ¿ì NULL·Î ³ÖÀ¸¸é µÈ´Ù.±×·¯³ª 
	// DISPLAY_INFO´Â ¹Ýµå½Ã Ã¤¿ö³Ö¾î¾ß ÇÑ´Ù.

	// ¾Æ·¡ÀÇ ¼öÄ¡´Â ±ÇÀå¼öÄ¡ÀÌ´Ù.´ë·« ¾Æ·¡ Á¤µµÀÇ °ªÀ¸·Î ¼¼ÆÃÇÏ¸é ¹«¸®°¡ ¾ø´Ù.
	DISPLAY_INFO	display;
	display.dwMaxAccessibleThreadNum = 5;		// ÀÌ ÄÜ¼Ö ÄÄÆ÷³ÍÆ®ÀÇ µð½ºÇÃ·¹ÀÌ ¸Å´ÏÁ®¸¦ µ¿½Ã¿¡ ¾ï¼¼½ºÇÒ ½º·¹µå ÃÖ´ë°¹¼ö
	display.dwMaxStringBufferNum = 10000;		// ÃÖ´ë 10000¶óÀÎÀÇ ¹öÆÛ¸¦ °¡Áø´Ù.
	display.dwMinMaintainBufferNum = 5000;		// È­¸é¿¡ º¸¿©Áö´Â ÃÖ´ë ¶óÀÎ¼ö´Â 10000-5000À¸·Î 5000¶óÀÎÀ» º¼¼ö ÀÖ´Ù.
	display.dwDisplayRefreshRate = 1000;		// °»½ÅÁÖ±â´Â 500msÀÌ´Ù.
	display.dwFilteredLevel = LOG_DEBUG;		// ·Î±×¸Þ½ÃÁö ¿ì¼±¼øÀ§°¡ LOG_DEBUGº¸´Ù ³·À¸¸é Ç¥½ÃÇÏÁö ¾Ê´Â´Ù.
	
	
	WRITE_FILE_INFO	file;
	file.dwMaxAccessibleThreadNum = 5;			// ÀÌ ÄÜ¼Ö ÄÄÆ÷³ÍÆ®ÀÇ ÆÄÀÏ ·Î±× ¸Å´ÏÁ®¸¦ ¾ï¼¼½ºÇÒ ½º·¹µå ÃÖ´ë°¹¼ö
	file.dwLimitedBufferSizeToWriteFile = 8192;	// ·Î±× ¸Þ½ÃÁö°¡ 8192¹ÙÀÌÆ® ½×ÀÏ¶§¸¶´Ù ½ÇÁ¦·Î ÆÄÀÏ¿¡ WRITEÇÑ´Ù.
	file.dwFilteredLevel = LOG_DEBUG;			// ·Î±×¸Þ½ÃÁö ¿ì¼±¼øÀ§°¡ LOG_DEBUGº¸´Ù ³·À¸¸é ÆÄÀÏ¿¡ ±â·ÏÇÏÁö ¾Ê´Â´Ù.

	DESC_CONSOLE	desc;

	desc.OnCommandFunc = CommandFunc;				// Å°º¸µå ÄÄ¸Çµå ÇÁ·ÒÇÁÆ®¿¡¼­ ÄÄ¸Çµå¸¦ ³Ö¾úÀ»¶§ parsingÇÒ ÄÝ¹éÇÔ¼ö ¼¼ÆÃ
	desc.dwCustomButtonNum = MaxButton;			// »ç¿ëÀÚ Á¤ÀÇ ¹öÆ° 2°³ »ç¿ëÇÔ.
	desc.pCustomButton = pButtonInfo;			// Ä¿½ºÅÒ ÀÌº¥Æ® Æ÷ÀÎÅÍ ¼¼ÆÃ
	desc.szFileName = "log.txt";				// writeÇÒ ÆÄÀÏ¸í.ÆÄÀÏ·Î±ëÇÏÁö ¾ÊÀ» °æ¿ì NULL
	desc.pDisplayInfo = &display;				// È­¸é µð½ºÇÃ·¹ÀÌ¸¦ À§ÇÑ DISPLAY_INFO ±¸Á¶Ã¼ Æ÷ÀÎÅÍ.¹«Á¶°Ç ¼¼ÆÃÇØ¾ßÇÑ´Ù.
	desc.pWriteFileInfo = &file;				// ÆÄÀÏ·Î±ëÀ» À§ÇÑ WRITE_FILE_INFO ±¸Á¶Ã¼ Æ÷ÀÎÅÍ.»ç¿ëÇÏÁö ¾ÊÀ¸¸é NULL

	m_pIConsole->CreateConsole(&desc);			// ÄÜ¼Ö »ý¼º

	return true;
}
*/

BOOL CConsole::Init(int MaxButton,MENU_CUSTOM_INFO* pMenuInfo, cbRetrnFunc commandFunc)
{
	HRESULT hr;
	
	hr = CoCreateInstance(
           CLSID_ULTRA_TCONSOLE,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_ITConsole,
           (void**)&m_pIConsole);

	
	if(FAILED(hr))
		return FALSE;

	LOGFONT logFont;
	logFont.lfHeight		= 17; 
	logFont.lfWidth			= 0; 
	logFont.lfEscapement	= 0; 
	logFont.lfOrientation	= 0; 
	logFont.lfWeight		= FW_BOLD; 
	logFont.lfItalic		= 0; 
	logFont.lfUnderline		= 0; 
	logFont.lfStrikeOut		= 0; 
	logFont.lfCharSet		= HANGUL_CHARSET; 
	logFont.lfOutPrecision	= 0; 
	logFont.lfClipPrecision	= 0; 
	logFont.lfQuality		= 0; 
	logFont.lfPitchAndFamily	= 0; 
	strcpy(logFont.lfFaceName, "±¼¸²"); 

	HWND hWnd;
	MHTCONSOLE_DESC	desc;
	desc.szConsoleName = "Darkstory";
	desc.dwRefreshRate = 1000;
	desc.wLogFileType = LFILE_DESTROYLOGFILEOUT;//0;//LFILE_LOGOVERFLOWFILEOUT;
	desc.szLogFileName = "./Log/ConsoleLog.txt";
	desc.dwFlushFileBufferSize = 10000;
	desc.dwDrawTimeOut	= 1000*60*3;
	desc.wMaxLineNum = 1000;
	desc.dwListStyle = TLO_NOTMESSAGECLOSE|TLO_LINENUMBER|TLO_SCROLLTUMBTRACKUPDATE;
	desc.Width	= 800;
	desc.Height = 400;
	desc.pFont = &logFont;

	desc.nCustomMunuNum = MaxButton;
	desc.cbReturnFunc = commandFunc;
	desc.pCustomMenu = pMenuInfo;
	m_pIConsole->CreateConsole(&desc, &hWnd);

	return TRUE;
}

void CConsole::Release()
{
	if(m_pIConsole)
	{
		m_pIConsole->Release();
		m_pIConsole = NULL;
	}
	
}

void CConsole::Log(int LogType,int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];
	
	va_list vl;

	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);


	switch(LogType) 
	{
	case eLogDisplay:
		m_pIConsole->OutputFile(va_Temp_Buff,strlen(va_Temp_Buff));
		break;
	case eLogFile:
		{
			//m_pIConsole->WriteFile(va_Temp_Buff,strlen(va_Temp_Buff),MsgLevel);
		}
		break;
	default:
		MessageBox(NULL,"Not Defined LogType",0,0);
	}
}

void CConsole::LOG(int MsgLevel,char* LogMsg,...)
{
	static char   va_Temp_Buff[1024];
	
	va_list vl;

	va_start(vl, LogMsg);
	vsprintf(va_Temp_Buff, LogMsg, vl);
	va_end(vl);

	if(m_pIConsole)
	m_pIConsole->OutputDisplay(va_Temp_Buff,strlen(va_Temp_Buff));
}

void CConsole::WaitMessage()
{
	m_pIConsole->MessageLoop();
}
