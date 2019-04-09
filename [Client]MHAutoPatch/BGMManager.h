// BGMManager.h: interface for the CBGMManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BGMMANAGER_H__D3D6B161_F5AC_4A59_99E6_CEAF4E1890F5__INCLUDED_)
#define AFX_BGMMANAGER_H__D3D6B161_F5AC_4A59_99E6_CEAF4E1890F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Audio/SoundLib.h"

class CBGMManager
{
public:
	CBGMManager();
	virtual ~CBGMManager();

	BOOL CreateSound( HWND hWnd );
	void ReleaseSound();

	void PlayBGM( char* pFileName );
	void StopBGM();

protected:
	IMilesSoundLib*		m_pSoundLib;

};

#endif // !defined(AFX_BGMMANAGER_H__D3D6B161_F5AC_4A59_99E6_CEAF4E1890F5__INCLUDED_)
