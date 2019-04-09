// SoundLib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SoundLib.h"
#include "MilesSoundLib.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



SOUNDLIB_API BOOL CreateSoundLib( IMilesSoundLib** ppISoundLib)
{
	CMilesSoundLib*	pSoundLib	=	new	CMilesSoundLib;
	if( pSoundLib == 0)
	{
		*ppISoundLib	=	NULL;
		return	FALSE;
	}

	*ppISoundLib	=	pSoundLib;

	return	TRUE;
}
