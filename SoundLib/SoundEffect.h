// SoundEffect.h: interface for the CSoundEffect class.
//
//////////////////////////////////////////////////////////////////////
/*
	사운드 이펙트.
	사실상의 인터페이스 역할만 하고 대부분의 일은 MilesSoundLib에서 다 하려고 노력한다.
*/
#if !defined(AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_)
#define AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"
#include "SoundEffectFile.h"
#include "mss/mss.h"

class	CMilesSoundLib;

class CSoundEffect  :	public ISoundEffect
{
private:
	VECTOR3			m_vPosition;				// 소리가 나는 위치.
//	VECTOR3			m_vVelocity;
	float			m_fVolume;					// 볼륨 0.0f ~ 1.0f
	HPROVIDER		m_h3DDriver;				// 3D 드라이버.
	H3DSAMPLE		m_hSoundSample;				// CMilesSoundLib에서 샘플을 할당 받았을때의 핸들.
	DWORD			m_dwSampleIndex;			// 할당 받은 샘플의 인덱스.

	CSoundEffectFile*	m_pFile;				// 소스 파일.

	float			m_fMaxDistance;				// 최대 거리.
	float			m_fMinDistance;				// 최소 거리.

	BOOL			m_bLoop;

	DWORD			m_dwRefCount;				// 레퍼런스 카운트

	CMilesSoundLib*		m_pSoundLib;
	DWORD				m_dwSoundLibIndex;				// CMilesSoundLib 에서의 인덱스.

	BOOL AllocSample();
	void ReleaseSample();

public:
	BOOL	Create( CSoundEffectFile* pFile, HPROVIDER h3DDriver, CMilesSoundLib* pSoundLib, DWORD dwEffectIndex);	// 생성자.
	void	Delete();																// 소멸자.

	H3DSAMPLE		GetSample(void){ return	m_hSoundSample;}
	DWORD			GetIndex(void){ return m_dwSoundLibIndex;}

	int				Procedure(void);			// 프로시저. soundlib상의 run에 붙어 실행된다.

// 외부에서 보이는 인터페이스들.
	void	_stdcall	Play( BOOL bLoop);		// 재생.
	void	_stdcall	Stop(void);		// 정지.

	void	_stdcall	SetMaxMinDistance( float fMaxDist, float fMinDist);

	void	_stdcall	SetPosition( VECTOR3* pPosition);	// 위치 설정한다.
	void	_stdcall	SetRelativePosition( VECTOR3* pPosition);		// 상대 위치 설정.
	void	_stdcall	SetVolume( float fVolume);
	float	_stdcall	GetVolume(void);

	BOOL	_stdcall	IsAlive(void);		// 아직 살아있는가? 외부에서 이놈을 날릴 시점을 알아낼 때 쓰인다.
	BOOL	_stdcall	IsLoop(void);		// 루프 플랙이 셋 되어있는가.?

	DWORD	_stdcall	AddRef();
	DWORD	_stdcall	Release();


//	CSoundEffect();
//	virtual ~CSoundEffect();

};

#endif // !defined(AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_)
