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

#pragma warning (disable:4786)

#include <windows.h>


// TODO: reference additional headers your program requires here


#include <windows.h>
#include <winsock2.h>
#include <ole2.h>
#include <initguid.h>
#include <stdio.h>
#include <assert.h>


#include "DataBase.h"
#include "Console.h"
 
#include <yhlibrary.h>
//#include "CommonHeader.h"
//---MurimNet
#include "MNDefines.h"

#include "../[CC]Header/vector.h"
#include "../[CC]Header/protocol.h"
#include "../[CC]Header/CommonDefine.h"
#include "../[CC]Header/CommonGameDefine.h"
#include "../[CC]Header/ServerGameDefine.h"
#include "../[CC]Header/CommonGameStruct.h"
#include "../[CC]Header/CommonStruct.h"
#include "../[CC]Header/ServerGameStruct.h"
#include "../[CC]Header/CommonGameFunc.h"


#include "./ServerSystem.h"


#include "ShareDefines.h"
#include "ShareStruct.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
