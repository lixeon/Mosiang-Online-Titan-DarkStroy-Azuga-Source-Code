// MHAudioManager.cpp: implementation of the CMHAudioManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHAudioManager.h"
#include "SoundItem.h"
#include "Sound.h"
#include "SoundFileManager.h"

extern BOOL g_bActiveApp;

IMilesSoundLib*		g_pSoundLib;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMHAudioManager);
CMHAudioManager::CMHAudioManager()
{
	g_pSoundLib		=	NULL;
	m_pSoudItemList = NULL;
	
	m_SoundTable.Initialize(32);

	m_MasterVolume = 1;
	m_BGMVolume = 1;
	m_SoundVolume = 1;

	m_idxLastBGM = 0;

	m_bIsBGMPlaying = FALSE;
}

CMHAudioManager::~CMHAudioManager()
{
	
}


BOOL CMHAudioManager::Initialize( HWND hWnd )
{
	m_bIsBGMPlaying = FALSE;

	// 사운드 라이브러리 생성.
	BOOL	bCreated	=	CreateSoundLib( &g_pSoundLib);
	if( bCreated == FALSE)
	{
//		_asm nop;
		return FALSE;
	}
	
	// 초기화. 파일 최대 갯수 100개, 소리 객체 200개, eax 시도.
	SOUND_ERROR_CODE	sResult	=	g_pSoundLib->Init( 500, 500, "./Sound/miles", FALSE, TRUE);
	
	g_pSoundLib->Set3DSpeakerType(SOUND_SPEAKER_TYPE_STEREO);

//	JSD - 일단 두가지 경우만 체크함
//	마일즈 초기화에 실패하면 SOUND_ERROR_MILES_STARTUP_FAILED,
//	BGM드라이버를 찾지 못할 경우, 사운드카드가 없다면 SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,
	if( sResult == SOUND_ERROR_BGMDRIVER_STARTUP_FAILED ||	
		sResult == SOUND_ERROR_MILES_STARTUP_FAILED )
	{
		Terminate();
		return FALSE;
	}
//
	if( sResult != SOUND_ERROR_NOERROR )
	{
//		_asm nop;
		Terminate();
		return FALSE;
	}
	
	LoadList();

	return TRUE;
}

void CMHAudioManager::LoadList()
{
	CSoundFileManager sfilemgr;
	sfilemgr.Init( &m_pSoudItemList, m_nMaxSound );
}

void CMHAudioManager::Terminate()
{
	m_bIsBGMPlaying = FALSE;

	CSound* pSound;
	m_SoundTable.SetPositionHead();
	while(pSound = m_SoundTable.GetData())
	{
		pSound->Stop();
		delete pSound;
	}
	m_SoundTable.RemoveAll();

	if( m_pSoudItemList )
	{
		delete [] m_pSoudItemList;
		m_pSoudItemList = NULL;
	}
	
	if( g_pSoundLib )
	{
		g_pSoundLib->Delete();
		g_pSoundLib = NULL;
	}
}

CSound*	CMHAudioManager::Play( SNDIDX idx ,CObject* pObject )
{
	if( !g_bActiveApp )		return NULL;
	if( !m_bEffectSoundOn )	return NULL;
	if( !g_pSoundLib )		return NULL;

	ASSERT(idx < m_nMaxSound);
	if(idx >= m_nMaxSound)
		return NULL;

	if( m_pSoudItemList == NULL ) return NULL;

	CSound* pSound = m_pSoudItemList[idx].CreateSound(pObject);
//	ASSERT(pSound);
	if(pSound == NULL)
		return NULL;
	
	pSound->Play();
	pSound->SetVolume(GetSoundVolume(pSound->GetEachSoundVolume()));

	m_SoundTable.Add(pSound,(DWORD)pSound);
	return pSound;
}
void	CMHAudioManager::Stop( CSound* pSound )
{
	if(g_pSoundLib == NULL)
		return;
	
	ASSERT(pSound);
	if( pSound == NULL ) return;

	pSound->Stop();
	m_SoundTable.Remove((DWORD)pSound);
}
void	CMHAudioManager::SetVolume( CSound* pSound, float val )
{
	if(g_pSoundLib == NULL)
		return;

	ASSERT(pSound);
	if( pSound )
		pSound->SetVolume(val);
}

void	CMHAudioManager::PlayBGM( SNDIDX idx )
{
	if( m_bIsBGMPlaying == TRUE && m_idxLastBGM == idx )
		return;

	m_idxLastBGM = idx;

	if( idx == 0 )			return;
	if( !g_bActiveApp )		return;
	if( !m_bBGMSoundOn )	return;
	if( !g_pSoundLib )		return;	

	ASSERT(idx < m_nMaxSound);

	StopBGM();

	m_bIsBGMPlaying = TRUE;
	
	g_pSoundLib->PlayBGM(GetFileName(idx));
	g_pSoundLib->SetBGMVolume(GetBGMVolume());
}
void	CMHAudioManager::StopBGM()
{
	if(g_pSoundLib == NULL)
		return;

	m_bIsBGMPlaying = FALSE;
	g_pSoundLib->StopBGM();
}

char * CMHAudioManager::GetFileName(SNDIDX idx)
{
	if(g_pSoundLib == NULL || m_pSoudItemList == NULL )
		return NULL;
	return m_pSoudItemList[idx].GetFileName();
}

void CMHAudioManager::Process()
{
	if(g_pSoundLib == NULL)
		return;
	
	CSound* pSound;
	m_SoundTable.SetPositionHead();
	while(pSound = m_SoundTable.GetData())
	{
		if(pSound->Process() == TRUE)
		{
			pSound->Stop();
			delete pSound;
			m_SoundTable.Remove((DWORD)pSound);
		}
	}
}
void CMHAudioManager::StopAll()
{
	CSound* pSound;
	m_SoundTable.SetPositionHead();
	while(pSound = m_SoundTable.GetData())
	{
		pSound->Stop();
		delete pSound;
	}
	m_SoundTable.RemoveAll();
}
void CMHAudioManager::SetListenerPosition(VECTOR3* pPos,float fAngleRad)
{	
	if(g_pSoundLib == NULL)
		return;

	VECTOR3 odir,dir;
	odir.x = 0;
	odir.y = 0;
	odir.z = -1;
	TransToRelatedCoordinate(&dir,&odir,fAngleRad);//-gPHI/2);
	g_pSoundLib->SetListener(pPos,&dir);
	/*
	VECTOR3 odir,dir;
	odir.x = 0;
	odir.y = 0;
	odir.z = -1;
	TransToRelatedCoordinate(&dir,&odir,fAngleRad);
	g_pSoundLib->SetListener(pPos,&dir);
	*/
}

void CMHAudioManager::SetMasterVolume(float fVal)
{
	if(fVal < 0)
		m_MasterVolume = 0;
	else if(fVal > 1)
		m_MasterVolume = 1;
	else
		m_MasterVolume = fVal;

	if(g_pSoundLib == NULL)
		return;
	
	ApplyVolumeChange();
}
void CMHAudioManager::SetBGMVolume(float fVal)
{
	if(fVal < 0)
		m_BGMVolume = 0;
	else if(fVal > 1)
		m_BGMVolume = 1;
	else
		m_BGMVolume = fVal;

	if(g_pSoundLib == NULL)
		return;

	ApplyBGMVolumeChange();
}
void CMHAudioManager::SetSoundVolume(float fVal)
{
	if(fVal < 0)
		m_SoundVolume = 0;
	else if(fVal > 1)
		m_SoundVolume = 1;
	else
		m_SoundVolume = fVal;

	if(g_pSoundLib == NULL)
		return;	

	ApplyVolumeChange();
}

void CMHAudioManager::ApplyVolumeChange()
{
	if(g_pSoundLib)
	{
		CSound* pSound;
		m_SoundTable.SetPositionHead();
		while(pSound = m_SoundTable.GetData())
		{
			pSound->SetVolume(GetSoundVolume(pSound->GetEachSoundVolume()));
		}
	}
}


void CMHAudioManager::ApplyBGMVolumeChange()
{
	if(g_pSoundLib && m_bIsBGMPlaying)
	{		
		g_pSoundLib->SetBGMVolume(GetBGMVolume());
	}
}



float CMHAudioManager::GetBGMVolume()
{
	if(g_pSoundLib == NULL)
		return 0.0f;
	return m_MasterVolume * m_BGMVolume;	
}
float CMHAudioManager::GetSoundVolume(float fEachSoundVolume)
{
	if(g_pSoundLib == NULL)
		return 0.0f;
	return m_MasterVolume * m_SoundVolume * fEachSoundVolume;
}

//KES
void	CMHAudioManager::PlayLastBGM()
{
	if( !m_bBGMSoundOn )
		return;

	if(g_pSoundLib == NULL)
		return;

	ASSERT(m_idxLastBGM < m_nMaxSound);
	
	m_bIsBGMPlaying = TRUE;

	g_pSoundLib->PlayBGM(GetFileName(m_idxLastBGM));
	g_pSoundLib->SetBGMVolume(GetBGMVolume());
}

