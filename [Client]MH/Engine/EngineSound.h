// EngineSound.h: interface for the CEngineSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINESOUND_H__61B1B594_52EE_4AEE_B5B9_6BB6B860FEB0__INCLUDED_)
#define AFX_ENGINESOUND_H__61B1B594_52EE_4AEE_B5B9_6BB6B860FEB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IAudio.h"

class CEngineSound  
{
	GXSOUND_HANDLE m_hSound;
	char m_SoundFileName[64];

public:
	CEngineSound();
	virtual ~CEngineSound();

	BOOL Init(char* SoundFileName);
	void Release();

	void Play();
	void Stop();
	
	void SetVolume(long Volume);

	friend DWORD MHSoundProc(I4DyuchiAudio* pAudio,GXSOUND_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
};

#endif // !defined(AFX_ENGINESOUND_H__61B1B594_52EE_4AEE_B5B9_6BB6B860FEB0__INCLUDED_)
