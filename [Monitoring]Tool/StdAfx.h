// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__74ACF483_F8C6_4E6F_A7D9_A4335A96C6B9__INCLUDED_)
#define AFX_STDAFX_H__74ACF483_F8C6_4E6F_A7D9_A4335A96C6B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning (disable:4786)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "mmsystem.h"

#define _CRYPTCHECK_


#include <yhlibrary.h>
//#include "CommonHeader.h"

extern DWORD gCurTime;

#include "..\[CC]Header\vector.h"
#include "..\[CC]Header\protocol.h"
#include "..\[CC]Header\CommonDefine.h"
#include "..\[CC]Header\CommonGameDefine.h"
#include "..\[CC]Header\CommonGameStruct.h"
#include "..\[CC]Header\CommonStruct.h"
#include "..\[CC]Header\CommonGameFunc.h"
#include "..\[CC]Header\ServerGameDefine.h"
#include "..\[CC]Ability\AbilityCommonHeader.h"
#include "..\[CC]Header\ServerGameStruct.h"
#include "..\[CC]Header\CommonStructMS.h"


#define	_FILE_BIN_

#include "MHFile.h"
#include "FileBin.h"
#include "ServerToolCommon.h"

#include "Resource.h"


char*	GetDirectory();
char*	GetDataDirectory();
char*	GetUserCountDirectory();

void	LOG( char* msg, ... );

// ¹öÀü
#define		SERVERTOOLVERSION	"OPEN20050104"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__74ACF483_F8C6_4E6F_A7D9_A4335A96C6B9__INCLUDED_)
