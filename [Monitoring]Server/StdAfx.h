// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>
#include <winsock2.h>
#include <ole2.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>


struct VECTOR3	{float x,y,z;};

struct VECTOR2	{float x,y;};

extern DWORD gCurTime;

#define _CRYPTCHECK_

// Local Header Files
#include "../[Lib]YhLibrary/YHLibrary.h"
#include "Protocol.h"
#include "CommonDefine.h"
#include "MSDefine.h"
#include "CommonGameDefine.h"
#include "CommonGameStruct.h"
#include "CommonStruct.h"
#include "ServerGameStruct.h"
#include "CommonStructMS.h"

extern void LogConsole(char * buff,...);
extern void WriteAssertMsg(char* pStrFileName,int Line,char* pMsg);
extern void LOG( char* msg, ... );

// TODO: reference additional headers your program requires here
#define		SERVERTOOLVERSION	"OPEN20050104"
#define		RMTOOLVERSION		"OPEN20041223"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
