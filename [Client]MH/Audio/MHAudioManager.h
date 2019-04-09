// MHAudioManager.h: interface for the CMHAudioManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHAUDIOMANAGER_H__1B6E7288_11D4_43DC_BD78_91228E0D1D11__INCLUDED_)
#define AFX_MHAUDIOMANAGER_H__1B6E7288_11D4_43DC_BD78_91228E0D1D11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"

class CSound;
class CSoundItem;


#define AUDIOMGR	USINGTON(CMHAudioManager)



class CMHAudioManager  
{
	CYHHashTable<CSound> m_SoundTable;
	void			LoadList();

	float	m_MasterVolume;
	float	m_BGMVolume;
	float	m_SoundVolume;

	BOOL	m_bIsBGMPlaying;

//---KES : OPTION
	BOOL	m_bBGMSoundOn;
	BOOL	m_bEffectSoundOn;
	SNDIDX	m_idxLastBGM;
//---

	void ApplyVolumeChange();
//KES
	void ApplyBGMVolumeChange();
public:
	CMHAudioManager();
	virtual ~CMHAudioManager();
//	MAKESINGLETON(CMHAudioManager);

	BOOL			Initialize( HWND hWnd );
	void			Terminate();

	void			PlayBGM( SNDIDX idx );
	void			StopBGM();

	CSound*			Play( SNDIDX idx , CObject* pObject );
	void			Stop( CSound* pSound );
	void			StopAll();
	void			SetVolume( CSound* pSound, float val );	// 0~1 ªÁ¿Ã

	char *			GetFileName(SNDIDX idx);

	void			SetListenerPosition(VECTOR3* pPos,float fAngleRad);

	void Process();

	void SetMasterVolume(float fVal);
	void SetBGMVolume(float fVal);
	void SetSoundVolume(float fVal);

	float GetBGMVolume();
	float GetSoundVolume(float fEachSoundVolume);

//---KES : OPTION
	void SetBGMSoundOn( BOOL bOn )		{ m_bBGMSoundOn = bOn; }
	void SetEffectSoundOn( BOOL bOn )	{ m_bEffectSoundOn = bOn; }
	void PlayLastBGM();
//---

	
protected:	
	__int32			m_nMaxSound;
	CSoundItem	*	m_pSoudItemList;
};

EXTERNGLOBALTON(CMHAudioManager);
#endif // !defined(AFX_MHAUDIOMANAGER_H__1B6E7288_11D4_43DC_BD78_91228E0D1D11__INCLUDED_)
