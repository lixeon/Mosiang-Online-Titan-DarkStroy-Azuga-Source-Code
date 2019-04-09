#pragma once

#include "stdafx.h"
#include "IAudio_GUID.h"
#include "IAudio.h"
#include "../4DyuchiGRX_Common/typedef.h"

interface I4DyuchiAudio : public IUnknown
{
	virtual BOOL			__stdcall			Initialize(HWND hWnd,DWORD dwMaxItemNum) = 0;
	virtual	GXSOUND_HANDLE	__stdcall			CreateSoundObject(char* szFileName,GXSoundPROC pProc,void* pData,DWORD dwFlag) = 0;
	virtual void			__stdcall			DeleteSoundObject(GXSOUND_HANDLE gxhSound) = 0;

	virtual BOOL			__stdcall			Play(GXSOUND_HANDLE gxhSound,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall			Stop(GXSOUND_HANDLE gxhSound) = 0;
	virtual void			__stdcall			SetVolume(GXSOUND_HANDLE gxhSound,long lVolume,DWORD dwDuration) = 0;	// -9600 - 0

	virtual void			__stdcall			SetSoundPosition(GXSOUND_HANDLE gxhSound,VECTOR3* pv3Pos) = 0;
	virtual void			__stdcall			SetListenerPosition(GXSOUND_HANDLE gxhSound,VECTOR3* pv3Pos) = 0;
	virtual	void			__stdcall			Process() = 0;
};
