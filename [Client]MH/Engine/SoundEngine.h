// SoundEngine.h: interface for the CSoundEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDENGINE_H__5764A51D_446C_4BCE_A5FD_295FCCD9CFDE__INCLUDED_)
#define AFX_SOUNDENGINE_H__5764A51D_446C_4BCE_A5FD_295FCCD9CFDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IAudio.h"

class CSoundEngine  
{
	I4DyuchiAudio* m_pAudio;
	BOOL CreateAudio(HWND hWnd,int MaxAudioObject);
public:
	CSoundEngine(HWND hWnd,int MaxAudioObject);
	virtual ~CSoundEngine();
};


extern I4DyuchiAudio* g_pAudio;


#endif // !defined(AFX_SOUNDENGINE_H__5764A51D_446C_4BCE_A5FD_295FCCD9CFDE__INCLUDED_)
