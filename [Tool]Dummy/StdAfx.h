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
#define _WIN32_WINNT	0x0500 
//#define _CRYPTCHECK_

//#define TAIWAN_LOCAL //pjslocal

#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <initguid.h>
#include <stdio.h>
#include <assert.h>
// TODO: reference additional headers your program requires here

#include "Console.h"
 
#include <yhlibrary.h>
#pragma warning(disable : 4786)
#include <map>
//#include <queue>
//#include <list>
//#include <vector>
//#include <stack>
//#include <algorithm>
using namespace std;

extern DWORD gCurTime;

#include "../[CC]Header/Vector.h"
#include "../[CC]Header/protocol.h"
#include "../[CC]Header/CommonDefine.h"
#include "../[CC]Header/CommonGameDefine.h"
#include "../[CC]Header/ClientGameDefine.h"
#include "../[CC]Header/CommonGameStruct.h"
#include "../[CC]Header/CommonStruct.h"
#include "../[CC]Header/ClientGameStruct.h"
#include "../[CC]Header/CommonGameFunc.h"

#include ".\ServerSystem.h"
#include "MHFile.h"

#define _NETWORKTEST

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
