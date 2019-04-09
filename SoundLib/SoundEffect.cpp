// SoundEffect.cpp: implementation of the CSoundEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MilesSoundLib.h"
#include "SoundEffect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CSoundEffect::CSoundEffect()
{

}
CSoundEffect::~CSoundEffect()
{

}
*/
/*
	Create 되는 시점에서 바로 핸들을 받아서 얼록 받는건 아니고 파일의 버퍼 포인터만 가지고 있다가,
	Play()가 호출되는 시점에서 CMilesSoundLib에서 출력 버퍼를 하나 할당 받은 뒤 플레이 한다.
*/
BOOL CSoundEffect::Create( CSoundEffectFile* pFile, HPROVIDER h3DDriver, CMilesSoundLib* pSoundLib, DWORD dwEffectIndex)
{
#ifdef	_DEBUG
	if( pFile == 0 || h3DDriver == 0 || pSoundLib == 0 )
	{
		_asm int 3;
	}
	if( pFile->GetWaveBuffer() == 0)
	{
		_asm int 3;
	}
#endif

	m_pFile			=	pFile;
	pFile->AddRef();

	m_pSoundLib		=	pSoundLib;			// 메니저 클래스의 포인터.
	m_vPosition.x	=	m_vPosition.y	=	m_vPosition.z	=	0.0f;
	m_fVolume		=	1.0f;				// 초기 볼륨. 0.0f~1.0f까지.
	m_h3DDriver		=	h3DDriver;				// 드라이버 핸들.
	m_hSoundSample	=	0;
	m_dwSampleIndex	=	0xffffffff;
	m_dwRefCount	=	1;					// 레퍼런스 카운터

	m_dwSoundLibIndex	=	dwEffectIndex;	// 메니저 클래스에서의 this의 인덱스.

	m_fMaxDistance	=	100.0f;
	m_fMinDistance	=	10.0f; 

	return	TRUE;
}

void CSoundEffect::Delete()
{
#ifdef		_DEBUG
	if( m_dwRefCount != 0)
	{
		_asm int 3;
	}
#endif

	m_pSoundLib->DeletingSoundEffect( m_dwSoundLibIndex, this);

	m_h3DDriver	=	0;
	if( m_hSoundSample)
	{
		this->Stop();
	}
	DWORD	dwRef	=	m_pFile->Release();
#ifdef			_DEBUG
	if( dwRef == 0)
		_asm nop			// 여기서 걸린다는 것은 이놈이 갖고 있는 사운드 파일 핸들이 먼저 지워지고 이놈이 나중에 지워짐을 뜻한다.
#endif
	m_pFile		=	0;
	m_dwRefCount	=	0;
	m_pSoundLib		=	0;
	m_dwSampleIndex	=	0xffffffff;
	m_dwSoundLibIndex	=	0xffffffff;

}

// 아직 살아있는가? 외부에서 이놈을 날릴 시점을 알아낼 때 쓰인다.
BOOL CSoundEffect::IsAlive(void)
{
	if( m_hSoundSample == 0)
	{
		return	FALSE;
	}
	
	U32	status	=	AIL_3D_sample_status( m_hSoundSample);
	switch( status)
	{
		case SMP_DONE:
			return	FALSE;
		case	SMP_PLAYING:
			return	TRUE;
		case	SMP_STOPPED:
			_asm nop;
			break;
		case	SMP_FREE:
			_asm nop;
			break;
		case	SMP_PLAYINGBUTRELEASED:
			_asm nop;
			break;
		default:
			_asm int 3;
	}




	// 왜날뵑!
	return	FALSE;

}

// 루프 플랙이 셋 되어있는가.?
BOOL _stdcall CSoundEffect::IsLoop(void)
{
	return	m_bLoop;
}

// 재생.
/*
	샘플 할당받기를 시도한다.
	해서 되면 되고, 안되면 안된다. -_-;
	이미 플레이중일 경우 다시 플레이 한다.
*/
void _stdcall CSoundEffect::Play( BOOL bLoop)
{

	BOOL		bAllocSample	=	FALSE;

	if( !m_hSoundSample)
	{
		bAllocSample	=	AllocSample();
		if( bAllocSample == FALSE)
			goto	lbFailed;
 		_asm nop;
	}
	else
	{
		// 만약 할당을 받아놓고 계속 play, stop, play stop 할 요랑이라면 이럴 수 있다.
		_asm nop;
	}

	AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	this->SetMaxMinDistance( m_fMaxDistance, m_fMinDistance);
	this->SetVolume( m_fVolume);

	m_bLoop	=	bLoop;
	if( m_bLoop)
		AIL_set_3D_sample_loop_count(  m_hSoundSample, 0);
	else
		AIL_set_3D_sample_loop_count( m_hSoundSample, 1);

	AIL_start_3D_sample(  m_hSoundSample);


	return	;

lbFailed:
	return	;
}

// 정지.
void _stdcall CSoundEffect::Stop(void)
{
	if( m_hSoundSample)
	{
		AIL_end_3D_sample(  m_hSoundSample);
		ReleaseSample();
	}
/*
	else
	{
		_asm int 3;			// Stop이 호출되었다는 것은 정상적으로 샘플을 할당받고 플레이가 되었다는것을 뜻한다.
							// 무한 루프가 아닌 한엔 호출될 일이 있을까?
	}
*/
}

void _stdcall CSoundEffect::SetMaxMinDistance( float fMaxDist, float fMinDist)
{
#ifdef			_DEBUG
	if( fMinDist < 0.0f || fMinDist > fMaxDist)
	{
		_asm int 3;
	}
#endif
	
	if( m_hSoundSample)
	{
		AIL_set_3D_sample_distances(m_hSoundSample, fMaxDist, fMinDist);
	}

	m_fMaxDistance	=	fMaxDist;
	m_fMinDistance	=	fMinDist;
}

// 위치 설정한다.
void _stdcall CSoundEffect::SetPosition( VECTOR3* pPosition)
{
#ifdef			_DEBUG
	if( pPosition == 0)
	{
		_asm int 3;
	}
#endif

	VECTOR3		OutPos;
	m_pSoundLib->SoundEffectSetPosition( &OutPos, this, pPosition);

	m_vPosition	=	OutPos;
	

	if( m_hSoundSample )
	{

		// 거리 테스트를 해서 m_fMaxDistance 보다 멀면 Stop 시킨다.
/*
		fDistance	=	VECTOR3Length( &m_vPosition);
		if( fDistance > m_fMaxDistance)
		{
			Stop();
		}
		else
*/
		AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}

}

// 상대 위치 설정.
void _stdcall CSoundEffect::SetRelativePosition( VECTOR3* pPosition)
{
#ifdef	_DEBUG
	if( pPosition == 0)
		_asm int 3;
#endif

	m_vPosition		=	*pPosition;
	if( m_hSoundSample)
	{
		AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}
}


void _stdcall CSoundEffect::SetVolume( float fVolume)
{
#ifdef			_DEBUG
	if( fVolume < 0.0f)
	{
		_asm int 3;
	}

	if( fVolume > 1.0f)
	{
		_asm int 3;
	}
#endif

	m_fVolume	=	fVolume;


	if( m_hSoundSample)
	{
		AIL_set_3D_sample_volume( m_hSoundSample, m_fVolume);
	}

}

float _stdcall CSoundEffect::GetVolume(void)
{
	return	m_fVolume;
}

DWORD _stdcall CSoundEffect::AddRef()
{
	m_dwRefCount++;
	return	m_dwRefCount;
}


DWORD _stdcall CSoundEffect::Release()
{
	m_dwRefCount--;
	if( m_dwRefCount == 0)
	{
		this->Delete();
	}
	return	0;
}

BOOL CSoundEffect::AllocSample()
{

	if( m_hSoundSample)
	{
		_asm int 3;			// 이미 할당 받아놨는데?
		return	FALSE;
	}

	// 사운드라이브러리에서 할당을 받는다.
	H3DSAMPLE	hSample;
	DWORD		dwIndex;
	BOOL	bAlloc	=	m_pSoundLib->AllocSampleToSoundEffect( &hSample, &dwIndex);
	if( bAlloc == FALSE)
	{
		// 최대 할당량을 넘었나? 어쨌든 리턴 폴스.
		return	FALSE;
	}

	// 사운드 파일이랑 바인드. 이놈 왜이렇게 느리지.
	S32			sResult	=	AIL_set_3D_sample_file( hSample, m_pFile->GetWaveBuffer());
	if( sResult == 0)
	{
		// 샘플 생성이 안되는 경우.		이런 일이 생기면 곤란한데... 왜그라노.. 
		// 보고 이런일 없으면 빼자.
		char*	szLastError	=	AIL_last_error();
		_asm nop;
		// 그냥 릴리즈 하고 
		BOOL	bRelease	=	m_pSoundLib->ReleaseSampleFromSoundEffect( dwIndex);
		if( bRelease == FALSE)
		{
			_asm int 3;
		}
		return	FALSE;
 	}

	// 이까지 오면 샘플을 받는데 성공 했다.
	m_hSoundSample	=	hSample;
	m_dwSampleIndex	=	dwIndex;


	return	TRUE;
}

void CSoundEffect::ReleaseSample()
{
	if( m_hSoundSample)
	{
		// 샘플을 잡고 있는, 정상적인 경우라면, 
		m_pSoundLib->ReleaseSampleFromSoundEffect( m_dwSampleIndex);
		m_hSoundSample	=	0;
		m_dwSampleIndex	=	0xffffffff;
	}
	else
		_asm nop;
}

// 프로시저. soundlib상의 run에 붙어 실행된다.
int CSoundEffect::Procedure(void)
{

	return	0;
}

