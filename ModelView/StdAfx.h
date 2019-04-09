// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__59A9B73F_D189_4034_A540_C75EE97FC0A3__INCLUDED_)
#define AFX_STDAFX_H__59A9B73F_D189_4034_A540_C75EE97FC0A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


// Tool custom window messages.

#define WM_USER_FRONT_OF_CAMERA_DESTROY		(WM_USER + 0x100)
#define WM_USER_LIGHT_DIALOG_DESTROY		(WM_USER + 0x101)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__59A9B73F_D189_4034_A540_C75EE97FC0A3__INCLUDED_)
