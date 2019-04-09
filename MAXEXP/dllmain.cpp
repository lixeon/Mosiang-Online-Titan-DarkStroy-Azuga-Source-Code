#include "../max_common/stdafx.h"
#include "resource.h"
#include "export.h"

#include "../max_common/helper.h"

HINSTANCE hInstance = NULL;


BOOL	g_fdwControlInit = FALSE;
#pragma comment(lib, "..\\4DyuchiGXGFunc\\SS3DGFuncN.lib")

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	hInstance = hinstDLL;

	// Initialize the custom controls. This should be done only once.
	if (!g_fdwControlInit)
	{
		g_fdwControlInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}
	return TRUE;
}


__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return GetString(IDS_LIBDESCRIPTION);
}

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS 
__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}


__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) {
	case 0: return GetMaxExpDesc();
	default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}
