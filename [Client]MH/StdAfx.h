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
#define _FOR_MUKHYANG
#define _CLIENTVERSIONCHK_
//pjslocal
//#define TAIWAN_LOCAL

#define _FILE_BIN_

#define _MHCLIENT_
#define _CRYPTCHECK_

#ifndef _SEMIRELEASE
#define _CHEATENABLE_
#endif
#define _ASSERTENABLE_



#pragma warning(disable:4786)

#include <windows.h>

// Windows Header Files:
#include <ole2.h>
#include <initguid.h>

#include "yhlibrary.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"
#include <assert.h>

// TODO: reference additional headers your program requires here


#include "Motion.h"
#include "..\4DyuchiGXGFunc\global.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\ClientGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\ClientGameStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"


#ifdef _TL_LOCAL_
#include "ThaiBreaker.h"
#endif

#include "MHNetwork.h"

#include "ErrorMsg.h"

#include "DirectoryManager.h"


extern BOOL g_bUsingEnginePack;



//#define __USINGDIRECTINPUTMOUSECONTROLL__
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
