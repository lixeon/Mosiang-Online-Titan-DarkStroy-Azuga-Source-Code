// MilesSoundLib.cpp: implementation of the CMilesSoundLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MilesSoundLib.h"
#include "SoundLibErrorString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// vtable 카피용 더미.
CSoundEffect	g_DummyEffect;

/*
	에러 스트링.
	SoundLib.h 의 에러코드와 내용이 일치하도록 유지하자.
*/
char*	g_pszErrorString[]	=	{
	"SOUND_ERROR_NOERROR",
	"SOUND_ERROR_ALREADY_INITIALIZED",
	"SOUND_ERROR_INVALID_ARGUMENT",
	"SOUND_ERROR_MILES_STARTUP_FAILED",
	"SOUND_ERROR_BGMDRIVER_STARTUP_FAILED",
	"SOUND_ERROR_3DDRIVER_STARTUP_FAILED",
	"SOUND_ERROR_BGMDRIVER_NOT_LOADED",
	"SOUND_ERROR_UNABLE_OPEN_FILE",
	"SOUND_ERROR_OUT_OF_MAX_FILE_COUNT",
	"SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT"
};

CMilesSoundLib*		g_pSoundLib	=	0;

  
CMilesSoundLib::CMilesSoundLib()
{
	// miles가 올라갔는가? 앞으로 이 플랙을 체크해서 예외상황에 대비.
	m_bMilesStartUp				=	FALSE;

	// miles driver. 일단 이놈을 만들어야지 3d 드라이버가 생성된다.
	m_BGMDriver					=	NULL;
	m_BGMHandle					=	NULL;
	m_fBGMVolume				=	0.5f;
	
	// miles 3d driver
	m_3DDriver					=	NULL;
	m_dw3DDriverType			=	NULL;

	// sound effect file
	m_dwMaxSoundFileCount		=	0;
	m_dwSoundFileCount			=	0;
	m_ppSoundFileBuffer			=	NULL;
	m_hSoundFileIndexCreator	=	0;
	m_hSoundFileHash			=	0;

	//memorypool
	m_hSoundFileMemoryPool		=	0;

	// sound effect
	m_dwSoundEffectCount		=	0;				// 현재 사운드 갯수.
	m_dwMaxSoundEffectCount		=	0;				// 최대 사운드 갯수.
	m_ppSoundEffectBuffer		=	0;				// 디버깅용 CSoundEffect 포인터 버퍼.
	m_hSoundEffectIndexCreator	=	0;				// 포인터버퍼용 인덱스 크리에이터.

	//memorypool
	m_hSoundEffectMemoryPool	=	0;

	g_pSoundLib			=	this;
}

CMilesSoundLib::~CMilesSoundLib()
{

}

// 사운드 메니저 초기화.
SOUND_ERROR_CODE _stdcall CMilesSoundLib::Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware)
{
	BOOL	bIndexCreator;

//	bTryEAX	=	TRUE;
//	bForceSoftware	=	TRUE;
#ifdef	_DEBUG
	char	szErrorString[1024];
	wsprintf( szErrorString, "CMilesSoundLib::Init( dwMaxSoundFileCount = %d, dwMaxSoundEffectCount = %d, szMilesPass = %s, bTryEAX = %d", dwMaxSoundFileCount, dwMaxSoundEffectCount, szMilesPass, bTryEAX);
	OutputDebugString( szErrorString);
#endif

	// 이미 로드되어있나?
	if( m_bMilesStartUp == TRUE)
	{
		return	SOUND_ERROR_ALREADY_INITIALIZED;
	}

	// 인자가 이상한가?
	if( dwMaxSoundFileCount == 0 || dwMaxSoundEffectCount == 0 || szMilesPass == 0)
	{
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}

	// 마일즈 초기화.
	char*	szStartUpError	=	0;
	char*	szReturn		=	AIL_set_redist_directory(szMilesPass);
	S32		mssStartUp		=	AIL_startup();
	if( mssStartUp == 0)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_MILES_STARTUP_FAILED;
	}
	m_bMilesStartUp		=	TRUE;

	// 배경음악 초기화.
	BOOL	bStartupBGMDriver	=	StartupBGMDriver();	
	if( bStartupBGMDriver == FALSE)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_BGMDRIVER_STARTUP_FAILED;	// 사운드 카드가 없을 경우 일단 실패한다.
	}

	// 3DDriver 초기화.
	BOOL	bStartup3DDriver	=	Startup3DDriver( bTryEAX, bForceSoftware);
	if( bStartup3DDriver == FALSE)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_3DDRIVER_STARTUP_FAILED;	// 이게 실패하는 경우가 있을까.?
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// sound file 초기화.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	m_dwMaxSoundFileCount		=	dwMaxSoundFileCount;
	m_ppSoundFileBuffer			=	new	CSoundEffectFile*[m_dwMaxSoundFileCount];
	memset( m_ppSoundFileBuffer, 0, sizeof(CSoundEffectFile*) * m_dwMaxSoundFileCount);
	m_dwSoundFileCount			=	0;
	// 인덱스 크리에이터.
	m_hSoundFileIndexCreator		=	ICCreate();
	bIndexCreator	=	ICInitialize( m_hSoundFileIndexCreator, m_dwMaxSoundFileCount);
	if( bIndexCreator == FALSE)
	{
		_asm int 3;			// critical
	}
	// 해쉬.
	m_hSoundFileHash	=	VBHCreate();
	BOOL	bHash	=	VBHInitialize( m_hSoundFileHash, m_dwMaxSoundFileCount, MAX_PATH, m_dwMaxSoundFileCount);
#ifdef	_DEBUG
	if( bHash == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	이펙트 파일용 해쉬 생성 실패.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create Effect file hash.\n");
		_asm int 3;
	}
#endif

	// 파일용 메모리 풀.
	//memorypool
	m_hSoundFileMemoryPool	=	CreateStaticMemoryPool();
	BOOL	bFilePool		=	InitializeStaticMemoryPool( m_hSoundFileMemoryPool, sizeof( CSoundEffectFile), 0, m_dwMaxSoundFileCount);
#ifdef	_DEBUG
	if( bFilePool == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	이펙트 파일용 스테이틱 메모리 풀 생성 실패.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create static memory pool for effect file.\n");
		_asm int 3;
	}
#endif
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// sound effect 초기화.
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// 디버깅을 위해 만들어진 사운드이펙트도 리스트를 유지한다.
	m_dwMaxSoundEffectCount		=	dwMaxSoundEffectCount;
	m_ppSoundEffectBuffer	=	new CSoundEffect*[m_dwMaxSoundEffectCount];
	memset( m_ppSoundEffectBuffer, 0, sizeof( CSoundEffect*)* m_dwMaxSoundEffectCount);
	m_dwSoundEffectCount			=	0;
	m_hSoundEffectIndexCreator	=	ICCreate();
	bIndexCreator	=	ICInitialize( m_hSoundEffectIndexCreator, m_dwMaxSoundEffectCount);
#ifdef	_DEBUG
	if( bIndexCreator == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	이펙트용 인덱스 크리에이터 생성 실패.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create index creator for effect.\n");
		_asm int 3;
	}
#endif

	m_hSoundEffectMemoryPool	=	CreateStaticMemoryPool();
	BOOL	bEffectPool			=	InitializeStaticMemoryPool( m_hSoundEffectMemoryPool, sizeof(CSoundEffect), 0, m_dwMaxSoundEffectCount);
#ifdef	_DEBUG
	if( bEffectPool == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	이펙트용 스테이틱 메모리 풀 생성 실패.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create static memory pool for effect.\n");
		_asm int 3;
	}
#endif

	
	this->AllocSamplePool();


	return	SOUND_ERROR_NOERROR;
}
/*
DWORD _stdcall CMilesSoundLib::GetAllocatedEffectCount(void)
{
//	

	return	0xffffffff;
}
*/

SOUND_3D_DRIVER _stdcall CMilesSoundLib::Get3DDriver(void)
{
	return	(SOUND_3D_DRIVER)m_dw3DDriverType;
}

void _stdcall CMilesSoundLib::SetListener( VECTOR3* pPosition, VECTOR3* pAngle)
{
#ifdef	_DEBUG
	if( pPosition == 0 || pAngle == 0)
		_asm int 3;
#endif

	m_vListenerPos	=	*pPosition;
	m_vListenerAng	=	*pAngle;


	// 리스너의 역행열을 바로 만든다.

	// 역방향으로 간 다음,
	VECTOR3		PosRev	=	m_vListenerPos;
	VECTOR3_MULEQU_FLOAT( &PosRev, -1.0f);
	MATRIX4		MoveRev;
	TranslateMatrix( &MoveRev, &PosRev);
//	TranslateMatrix( &m_mListener, &PosRev);
//	MatrixMultiply2( &m_mListener , MATRIX4* mat1, MATRIX4* mat2);

	// 역방향으로 돌린다.


	// z, x, y의 역방향인 -y, -x, -z로 돌린다.
	MATRIX4		Rotate, RotateY, RotateX, RotateZ;
	SetRotationYMatrix( &RotateY, -1.0f * m_vListenerAng.y);
	SetRotationXMatrix( &RotateX, -1.0f * m_vListenerAng.x);
	SetRotationZMatrix( &RotateZ, -1.0f * m_vListenerAng.z);

	MatrixMultiply3( &Rotate, &RotateY, &RotateX, &RotateZ);

	MatrixMultiply2( &m_mListenerRev, &MoveRev, &Rotate);



//	SetRotationXMatrix( &m_mListenerRev, -1.0f * m_vListenerAng.y);
	

_asm nop



}

SOUND_SPEAKER_TYPE	_stdcall CMilesSoundLib::Get3DSpeakerType(void)
{
//	Check();

	S32	SpeakerType	=	AIL_3D_speaker_type( m_3DDriver );
	switch( SpeakerType)
	{
	case	AIL_3D_2_SPEAKER:
		return	SOUND_SPEAKER_TYPE_STEREO;			// 일반적인 2체널.
	case	AIL_3D_HEADPHONE:
		return	SOUND_SPEAKER_TYPE_HEADPHONE;		// 헤드폰, 사운드카드의 종류에 따라 헤드폰 엠프등이 작동한다.
	case	AIL_3D_4_SPEAKER:
		return	SOUND_SPEAKER_TYPE_4CH;				// 4체널 포지셔닝.
	case	AIL_3D_51_SPEAKER:
		return	SOUND_SPEAKER_TYPE_6CH;				// 5.1체널 포지셔닝.
	case	AIL_3D_71_SPEAKER:
		return	SOUND_SPEAKER_TYPE_8CH;				// 7.1체널 포지셔닝. 프로디지7.1 등에서 지원된다.
	}
	return	SOUND_SPEAKER_TYPE_UNKNOWN;

}

void _stdcall CMilesSoundLib::Set3DSpeakerType( SOUND_SPEAKER_TYPE Type)
{
//	Check();

	switch( Type)
	{
	case	SOUND_SPEAKER_TYPE_STEREO:			// 일반적인 2체널.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_2_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_HEADPHONE:		// 헤드폰, 사운드카드의 종류에 따라 헤드폰 엠프등이 작동한다.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_HEADPHONE);
		break;
	case	SOUND_SPEAKER_TYPE_4CH:				// 4체널 포지셔닝.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_4_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_6CH:				// 5.1체널 포지셔닝.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_51_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_8CH:				// 7.1체널 포지셔닝. 프로디지7.1 등에서 지원된다.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_71_SPEAKER);
		break;
	}

}
DWORD _stdcall CMilesSoundLib::ReleaseAllSoundEffect(void)
{
//	Check();

	// 버퍼를 돌면서 사운드 이펙트들을 지운다.
	if( m_dwSoundEffectCount == 0)
		return	0;


//	this->Run();

	DWORD	i;
	DWORD	dwCount = 0;
	DWORD	dwRemainedCount	=	m_dwSoundEffectCount;
	for( i = 0; i < m_dwMaxSoundEffectCount; i++)
	{
		if( m_ppSoundEffectBuffer[i] )
		{
			while( m_ppSoundEffectBuffer[i] )
			{
				m_ppSoundEffectBuffer[i]->Release();
			}
			dwCount++;
			dwRemainedCount--;
		}
	}

	if( dwRemainedCount != 0)
		_asm int 3;

	return	dwCount;
}

DWORD _stdcall CMilesSoundLib::ReleaseAllSoundFile(void)
{
//	Check();

	if( m_dwSoundFileCount == 0)
		return	0;

//	Check();

	DWORD	i;
	DWORD	dwCount	=	0;
	DWORD	dwRemainedCount	=	m_dwSoundFileCount;
	for( i = m_dwMaxSoundFileCount - 1; i < m_dwMaxSoundFileCount; i--)
	{

//		Check();

		if( m_ppSoundFileBuffer[i])
		{
//		Check();

			while( m_ppSoundFileBuffer[i])
			{
//		Check();
	
				this->ReleaseSoundEffectFile( m_ppSoundFileBuffer[i]);
			}
			dwCount++;
			dwRemainedCount--;

			if( dwRemainedCount != m_dwSoundFileCount)
				_asm int 3;
		}
		else
			_asm nop;
	}

	if( dwRemainedCount != 0)
		_asm int 3;

	return	dwCount;
}

// 사운드 메니저 말기화.
void _stdcall CMilesSoundLib::Delete()
{
//		Check();

	if( m_ppSoundFileBuffer)
	{
		// 사운드 이펙트가 남은게 있는가 체크한다.
		if( m_dwSoundEffectCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::SoundEffect를 다 해제하지 않았다.\n");
			OutputDebugString( "IMilesSoundLib::SoundEffect resource did not fully released.\n");
			_asm int 3;
		}

		// 사운드 이펙트 디버깅용 버퍼.
		delete	m_ppSoundEffectBuffer;
		m_ppSoundEffectBuffer	=	0;

		// 사운드 이펙트 인덱스크리에이터에 뭐가 남았나?
		DWORD	dwRemainedSoundEffectCount	=	ICGetIndexNum( m_hSoundEffectIndexCreator);
		if( dwRemainedSoundEffectCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundEffectIndexCreator 에서 갖고 있던 인덱스들이 다 해제가 안됬다. 사람을 부르시오");
			OutputDebugString( "IMilesSoundLib::m_hSoundEffectIndexCreator Index did not fully released. internal error. call me. -_-;.\n");
			_asm int 3;
		}
		// 사운드 이펙트 디버깅용 인덱스크리에이터 지운다.
		ICRelease( m_hSoundEffectIndexCreator);
		m_hSoundEffectIndexCreator	=	0;
		m_dwMaxSoundEffectCount		=	0;
		m_dwSoundEffectCount		=	0;
/*
		// eos콜백용 해쉬 지운다.
		QBHRelease( m_hSoundEffectHash);
		m_hSoundEffectHash	=	0;
*/
		// 사운드 이펙트용 스테이틱 메모리 풀을 날린다.
		ReleaseStaticMemoryPool( m_hSoundEffectMemoryPool);
		m_hSoundEffectMemoryPool	=	0;

		// 사운드 파일 남은게 있는가 체크한다.
		if( m_dwSoundFileCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::Sound Effect File을 모두 해제하지 않았다.");
			OutputDebugString( "IMilesSoundLib::Sound Effect File did not fully released.\n");
			_asm int 3;
		}
		// 사운드 파일 포인터 버퍼를 지운다.
		delete	m_ppSoundFileBuffer;
		m_ppSoundFileBuffer				=	0;
		if( m_hSoundFileIndexCreator == 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator이 널이다. 사람을 부르시오.\n");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator is NULL. call me -_-;\n");
			_asm int 3;// 어쩌라고오. =_=;
		}
		// 사운드 파일용 인덱스 메니져에서 파일이 다 날라갔나 검사.
		DWORD dwRemainedSoundFileCount	=	ICGetIndexNum( m_hSoundFileIndexCreator);
		if( dwRemainedSoundFileCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator 에서 갖고 있던 인덱스들이 다 해제가 안됬다. 사람을 부르시오");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator index did not fully released. call me -_-;\n");
			_asm int 3;
		}
		// 사운드 파일용 인덱스 메니져를 날린다.
		ICRelease( m_hSoundFileIndexCreator);
		m_hSoundFileIndexCreator	=	0;

		if( m_hSoundFileHash == 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash 이 널이다. 사람을 부르시오.");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash is NULL. call me -_-;\n");
			_asm int 3;
		}
		// 사운드 파일용 해쉬에 남은것 검사.
		DWORD*	pdwItemList	=	new	DWORD[m_dwMaxSoundFileCount];
		DWORD	dwRemainedSoundFileHashCount	=	VBHGetAllItem( m_hSoundFileHash, pdwItemList, m_dwMaxSoundFileCount);
		if( dwRemainedSoundFileHashCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash 에서 갖고 있던 아이템이 다 안날아갔다. 사람을 부르시오.");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash did not fully released. call me -_-;\n");
			_asm int 3;
		}
		delete[]	pdwItemList;
		// 사운드 파일용 해쉬를 날린다.
		VBHRelease( m_hSoundFileHash);
		m_hSoundFileHash	=	0;

		// 사운드 파일용 스테이틱 메모리 풀을 날린다.
		//memorypool
		ReleaseStaticMemoryPool( m_hSoundFileMemoryPool);
		m_hSoundFileMemoryPool		=	0;
	}

	ReleaseSamplePool();

	if( m_3DDriver)
	{
		Shutdown3DDriver();
		m_3DDriver	=	0;
	}

	if( m_BGMDriver)
	{
		ShutdownBGMDriver();
		m_BGMDriver	=	0;
	}

	if(m_bMilesStartUp)
	{
		AIL_shutdown();
		m_bMilesStartUp	=	FALSE;
	}

//	DeleteCriticalSection( &m_SampleCriticalSection);

	delete	this;

	return	;
}

void _stdcall CMilesSoundLib::GetErrorString( char** pszErrorString, SOUND_ERROR_CODE code)
{
	*pszErrorString	=	(g_pszErrorString[ code]);
}

BOOL CMilesSoundLib::StartupBGMDriver()
{
	if( m_BGMDriver != 0)
	{
		// 두번 호출 됬다.
		return	FALSE;
	}

	// 멀쩡하지 않은 사운드카드에선 실패할 수도 있으려나.?
	// 일단 사운드 드라이버가 없다면 실패 한다.
	HDIGDRIVER	bgmDriver	=	AIL_open_digital_driver( 48000, 16, 32, 0);
	if( bgmDriver	==	0)
	{
		return	FALSE;
	}

	m_BGMDriver	=	bgmDriver;


//lbFinishFindDriver:
	return	TRUE;

}

void CMilesSoundLib::ShutdownBGMDriver()
{
	if( m_BGMDriver == 0)
	{
		_asm int 3;
	}
	AIL_close_digital_driver(	m_BGMDriver);
	
}

SOUND_ERROR_CODE _stdcall CMilesSoundLib::PlayBGM( char *szBGMFileName)
{

//	Check();
	char* szLastError	=	0;
	if( m_BGMDriver == 0)
	{
		return	SOUND_ERROR_BGMDRIVER_NOT_LOADED;
	}

	if( szBGMFileName == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}
	
	size_t	fileNameLen	=	strlen( szBGMFileName);
	if( fileNameLen == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}


	if( m_BGMHandle)
	{
		StopBGM();
	}

	m_BGMHandle	=	AIL_open_stream( m_BGMDriver, szBGMFileName, 0 );
	if( m_BGMHandle == NULL)
	{
		szLastError	=	AIL_last_error();
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}

	AIL_set_stream_loop_count( m_BGMHandle, 0);

	this->SetBGMVolume( m_fBGMVolume);

	AIL_start_stream( m_BGMHandle);


	return	SOUND_ERROR_NOERROR;
}

void _stdcall CMilesSoundLib::StopBGM(void)
{
//		Check();
	if( m_BGMDriver == 0)
	{
		return;
	}
	AIL_close_stream( m_BGMHandle);
	m_BGMHandle	=	0;
}

void _stdcall CMilesSoundLib::SetBGMVolume( float fVolume)
{
//		Check();
	if( fVolume > 1.0f || fVolume < -1.0f)
	{
//		OutputDebugString("CMilesSoundLib::SetBGMVolume() 볼륨이 허용치 범위를 넘어섰다.");
		OutputDebugString("CMilesSoundLib::SetBGMVolume() over hit the max volume.\n");
		_asm int 3;
	}

	if( !m_BGMHandle)
	{
//		OutputDebugString("BGM이 플레이되고 있지 않은 상황에서 CMilesSoundLib::SetBGMVolume() 이 호출되었다.");
//		_asm int 3;
		m_fBGMVolume	=	fVolume;
	}
	else
	{
		m_fBGMVolume	=	fVolume;
		AIL_set_stream_volume_pan( m_BGMHandle, fVolume, 0.5f );	// 핸들, 볼륨, 팬 위치(지금 중앙)
	}

}

BOOL CMilesSoundLib::Startup3DDriver(BOOL bTryEAX, BOOL bForceSoftware)
{
	HPROVIDER	driver	=	0;
	BOOL		bLoad	=	FALSE;

	if( bForceSoftware)
		goto	lbSoftware;

	if( bTryEAX == FALSE)
		goto	lbTryDSHardware;


	// EAX 3
	driver	=	Find3DDriver("Creative Labs EAX 3 (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX3;
			goto	lbFinish;
		}
	}

	// EAX 2
	driver	=	Find3DDriver("Creative Labs EAX 2 (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX2;
			goto	lbFinish;
		}
	}

	// EAX 1
	driver	=	Find3DDriver("Creative Labs EAX (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX1;
			goto	lbFinish;
		}
	}

lbTryDSHardware:
	// DS3D hardware
	driver	=	Find3DDriver( "DirectSound3D Hardware Support");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DS3DHW;
			goto	lbFinish;
		}
	}

	// 이 위까지 포지셔닝이 지원된다.
	// 이 아래부터는 2스피커 전용.
lbSoftware:
	// MilesFast2D
	driver	=	Find3DDriver( "Miles Fast 2D Positional Audio");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_MILES;
			goto	lbFinish;
		}
	}

	// DS3D software
	driver	=	Find3DDriver( "DirectSound3D Software Emulation");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DS3DSW;
			goto	lbFinish;
		}
	}

	// Dolby
	driver	=	Find3DDriver( "Dolby Surround");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DOLBY;
			goto	lbFinish;
		}
	}


	// 여기까지 왔으면 드라이버를 찾는데 실패했다고 본다.
	// 사운드 카드가 없는 경우일 듯.
	m_dw3DDriverType	=	SOUND_DRIVER_TYPE_NONE;

	return	FALSE;




lbFinish:
 

	// Miles에서 요구하는 3D리스너를 만든다.



	_asm nop;
	return	TRUE;
	
}

SOUND_ERROR_CODE _stdcall CMilesSoundLib::CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char *szFileName)
{
//		Check();


	DWORD	dwResult	=	0xffffffff;
	BOOL	bResult;
#ifdef		_DEBUG
	if( m_BGMDriver	==	0)
	{
		*pOutHandle	=	0;
		return	SOUND_ERROR_BGMDRIVER_NOT_LOADED;
	}

	// 혹시 널인가? -_-;
	if( szFileName == 0 || pOutHandle == 0)
	{
		_asm int 3;
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}
#endif

	// 파일 이름 길이.
	int		iStrLen		=	lstrlen( szFileName);
#ifdef		_DEBUG
	if( iStrLen > MAX_PATH)
	{
		_asm int 3;
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}
#endif

	*pOutHandle	=	0;

	CSoundEffectFile*	pFile	=	0;
	DWORD				dwNewIndex	=	0xffffffff;

	// 해쉬에서 찾는다.
	DWORD		dwHashIndex;
	DWORD		dwHashReturn	=	VBHSelect( m_hSoundFileHash, &dwHashIndex, 1, szFileName, iStrLen);
#ifdef		_DEBUG
	if( dwHashReturn >= 2)
		_asm int 3;
#endif

	if ( dwHashReturn)
	{
		// 올라왔던 파일이다.
		// 해당 파일의 레퍼런스 카운터 하나 올리고,
		m_ppSoundFileBuffer[dwHashIndex]->AddRef();

		// 출력.
		*pOutHandle	=	(SOUND_FILE_HANDLE)m_ppSoundFileBuffer[dwHashIndex];
	}
	else
	{
		// 새로 올라오는 파일일 경우.
		// 최대 갯수를 초과하지 않았나?
		if( m_dwMaxSoundFileCount <= m_dwSoundFileCount)
		{
			return	SOUND_ERROR_OUT_OF_MAX_FILE_COUNT;
		}

		// 인덱스 할당 받고,
		dwNewIndex		=	ICAllocIndex( m_hSoundFileIndexCreator) - 1;		// 0base
		// 해쉬에 넣고,
		void*	hHashHandle	=	VBHInsert( m_hSoundFileHash, dwNewIndex, szFileName, iStrLen);
		// 메모리에서 할당 받고.
		//memorypool
		pFile		=	(CSoundEffectFile*)LALAlloc( m_hSoundFileMemoryPool);
		// 생성.
		dwResult	=	pFile->Create( szFileName, hHashHandle, dwNewIndex, this);
#ifdef			_DEBUG
		if( dwResult == SOUND_ERROR_INVALID_ARGUMENT)
			_asm int 3;				// 중대한 에러.
#endif
		if( dwResult == SOUND_ERROR_UNABLE_OPEN_FILE)
		{
			// 파일을 여는데 실패했다. 
			// 파일 객체 지우고,
			memset( pFile, 0, sizeof(CSoundEffectFile));
			LALFree( m_hSoundFileMemoryPool, pFile);
			pFile	=	0;
			// 해쉬에서 릴리즈.
			bResult	=	VBHDelete( m_hSoundFileHash, hHashHandle);
			if( bResult == FALSE)
			{
				_asm int 3;
			}
			// 인덱스에서 릴리즈.
			ICFreeIndex( m_hSoundFileIndexCreator, dwNewIndex+1);

			return	SOUND_ERROR_UNABLE_OPEN_FILE;
		}
		// 포인터 배열에 넣는다.
		m_ppSoundFileBuffer[dwNewIndex]	=	pFile;

		m_dwSoundFileCount++;

		// 출력.
		*pOutHandle		=	(SOUND_FILE_HANDLE)pFile;
	}

//	Check();

	return	SOUND_ERROR_NOERROR;
}

DWORD CMilesSoundLib::GetAllocatedSoundEffectCount()
{
//		Check();
	return	m_dwSoundEffectCount;
}

DWORD CMilesSoundLib::GetAllocatedSoundFileCount()
{
//		Check();
	return	m_dwSoundFileCount;
}



void CMilesSoundLib::ReleaseSoundEffectFile(SOUND_FILE_HANDLE hFile)
{
		
	BOOL	bResult;
#ifdef	_DEBUG
		if( hFile == 0 || (DWORD)hFile == 0xcccccccc || (DWORD)hFile == 0xdddddddd || (DWORD)hFile == 0xcdcdcdcd)
		{
			_asm int 3;
			return	;
		}
		if( m_dwSoundFileCount == 0)
		{
			_asm int 3;
			return	;
		}
		bResult		=	IsValidFileHandle( hFile);
		if( bResult == FALSE)
			_asm int 3;

#endif

	

//	DWORD	dwIndex;

	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hFile;

	

	// dwRelease값이 0이면 메모리상에서 날리므로 그에따른 처리를 한다.
	DWORD	dwRelease		=	pFile->Release();

	return	;

}

/*
	Miles의 하나의 sample은 하나의 ds 세컨드버퍼를 의미하는 것이라서 soundeffect 하나 만들때마다
	샘플핸들 하나 발급 받는건 말이 안되는 일이 되었다. -_-;
	이게 호출 되면 soundeffect에 파일의 버퍼만 넘기고,
	CSoundEffect::Play 가 되는 시점에 샘플을 발급받고 플레이를 한다.
*/
SOUND_ERROR_CODE _stdcall CMilesSoundLib::CreateSoundEffect(ISoundEffect **ppSoundEffect, SOUND_FILE_HANDLE hSound)
{
		
	BOOL	bResult;

#ifdef	_DEBUG
		if( hSound == 0 || (DWORD)hSound == 0xcccccccc || (DWORD)hSound == 0xdddddddd || (DWORD)hSound == 0xcdcdcdcd)
		{
			return	SOUND_ERROR_INVALID_ARGUMENT;
		}

		bResult		=	IsValidFileHandle( hSound);
		if( bResult == FALSE)
			return	SOUND_ERROR_INVALID_ARGUMENT;
#endif
	if( m_dwMaxSoundEffectCount <= m_dwSoundEffectCount)
	{
		// 찰 데까지 다 찼다.
		*ppSoundEffect	=	0;
		return	SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT;
	}

	// 파일 핸들
	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hSound;

	// 디버깅용 버퍼의 인덱스 하나 할당 한다.
	DWORD	dwIndex	=	ICAllocIndex( m_hSoundEffectIndexCreator) - 1;

	// 사운드 오브젝트 생성. 메모리 메니져에서 생성하는 식으로 해야할 듯.
	CSoundEffect*	pSound;
	pSound	=	(CSoundEffect*)LALAlloc( m_hSoundEffectMemoryPool);
	*((DWORD*)pSound)	=	*((DWORD*)(&g_DummyEffect));				// vtable 카피.
	pSound->Create( pFile,  m_3DDriver, this, dwIndex);
	// 디버깅용 버퍼에다 넣고,
	m_ppSoundEffectBuffer[dwIndex]		=	pSound;

	m_dwSoundEffectCount++;

	*ppSoundEffect	=	pSound;

	return	SOUND_ERROR_NOERROR;
}

HPROVIDER CMilesSoundLib::Find3DDriver(const char *szDriverName)
{
	int	enumDriver;
	HPROENUM	next	=	HPROENUM_FIRST ;
	HPROVIDER	dest	=	0;
	char *name;
	int	iStrCmp;

lbStartFindDriver:

	enumDriver	=	AIL_enumerate_3D_providers( &next, &dest, &name);
	if( enumDriver == 0)
	{
		_asm nop;
		// 더이상 남은 드라이버가 없다. 찾기 실패.
		return	0;
	}

	iStrCmp	=	strcmp( name, szDriverName);
	if( iStrCmp == 0)
	{
		return	dest;
		
		

	}
	goto	lbStartFindDriver;


}

BOOL CMilesSoundLib::Load3DDriver( HPROVIDER driver)
{
	char* szLastError	=	0;
	// 라이브러리 올린다. 일단 다이렉트사운드 소프트웨어 드라이버.
	U32	uiResult	=	AIL_open_3D_provider( driver);
	if( uiResult != 0)
	{
		szLastError	=	AIL_last_error();
		return	FALSE;
	}

	m_3DDriver	=	driver;

	return	TRUE;

}

void CMilesSoundLib::Shutdown3DDriver()
{
	if( m_3DDriver)
	{
		AIL_close_3D_provider( m_3DDriver);
		m_3DDriver	=	0;
		m_dw3DDriverType	=	SOUND_DRIVER_TYPE_NONE;
	}

}

// SounEffect의 Release가 호출 되었을때 SoundLib에 통보하기 위해 쓰인다.
void CMilesSoundLib::DeletingSoundEffect( DWORD dwEffectIndex, CSoundEffect* pEffect)
{
		
	// 일단 비교.
#ifdef	_DEBUG
	if( dwEffectIndex	>=	m_dwMaxSoundEffectCount)
	{
		_asm int 3;
	}
	if( m_dwSoundEffectCount == 0)
	{
		_asm int 3;
	}
	if( pEffect == 0)
	{
		_asm int 3;
	}
	if( m_ppSoundEffectBuffer[dwEffectIndex] != pEffect)
	{
		_asm int 3;
	}
#endif

		LALFree( m_hSoundEffectMemoryPool, pEffect);

	// 최종적으로 버퍼에서 지운다.
	m_ppSoundEffectBuffer[dwEffectIndex]	=	0;
	m_dwSoundEffectCount--;
	// 인덱스에서 없앤다.
	ICFreeIndex( m_hSoundEffectIndexCreator, dwEffectIndex+1);

}

/*
	사운드 파일의 핸들이 존재하는지 루프를 돌면서 체크하는 디버깅용 함수.
*/
BOOL CMilesSoundLib::IsValidFileHandle(SOUND_FILE_HANDLE hSound)
{
		
	if( hSound == 0)
	{
		return	FALSE;
	}
	DWORD	i;
	BOOL	bFound	=	FALSE;
	for( i = 0; i < m_dwMaxSoundFileCount; i++)
	{
		if( m_ppSoundFileBuffer[i] == hSound)
		{
			return	TRUE;
		}
	}

	return	FALSE;
	
}


void CMilesSoundLib::SoundEffectSetPosition(VECTOR3 *pOutPos, CSoundEffect *pEffect, VECTOR3 *pPos)
{
		
	// 카메라의 역행열에 넣은 값을 pOutPos에 리턴.
	TransformVector3_VPTR2( pOutPos, pPos, &m_mListenerRev, 1);

}


void _stdcall CMilesSoundLib::Run()
{
		_asm nop;			// 아직 아무것도...
//	m_dwMaxVoiceCount	=	30;

}

/*
void CMilesSoundLib::Check(void)
{

	DWORD	dwRef;
	DWORD	dwCount	=	0;
	int	i;
	for( i = 0; i < m_dwMaxSoundEffectCount; i++)
	{
		if( m_ppSoundEffectBuffer[i])
		{
			dwCount++;
		}
	}
	if( dwCount != m_dwSoundEffectCount)
		_asm int 3;

	dwCount	=	0;
	for( i = 0; i < m_dwMaxSoundFileCount; i++)
	{
		if( m_ppSoundFileBuffer[i])
		{
			dwRef	=	m_ppSoundFileBuffer[i]->GetRef();
			if( dwRef == 0)
				_asm nop;
			dwCount++;
		}
	}
	if( dwCount != m_dwSoundFileCount)
		_asm int 3;


}
*/

void CMilesSoundLib::DeleteSoundEffectFile(SOUND_FILE_HANDLE hFile)
{
		
	BOOL	bResult;
#ifdef	_DEBUG
		if( hFile == 0 || (DWORD)hFile == 0xcccccccc || (DWORD)hFile == 0xdddddddd || (DWORD)hFile == 0xcdcdcdcd)
		{
			_asm int 3;
			return	;
		}
		bResult		=	IsValidFileHandle( hFile);
		if( bResult == FALSE)
			_asm int 3;
#endif
	
	DWORD	dwIndex;

	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hFile;

	// dwRelease값이 0이면 메모리상에서 날리므로 그에따른 처리를 한다.
	DWORD	dwRelease		=	pFile->GetRef();
#ifdef		_DEBUG
	if( dwRelease != 0)
	{
		OutputDebugString("만든 사람을 부르시오. -_-;");
		_asm int 3;
		return	;
	}
#endif
	
	// 인덱스 없애고, 
	dwIndex			=	pFile->GetSoundLibIndex();
	ICFreeIndex( m_hSoundFileIndexCreator, dwIndex+1);			// 0base
	m_ppSoundFileBuffer[dwIndex]	=	0;

	// 해쉬에서 없애고,
	void*	hHashIndex		=	pFile->GetHashIndex();
	VBHDelete( m_hSoundFileHash, hHashIndex);

	pFile->Delete();
		//memorypool
		memset( pFile, 0, sizeof(CSoundEffectFile));
		LALFree( m_hSoundFileMemoryPool, pFile);
	pFile	=	0;

	m_dwSoundFileCount--;



		
	_asm nop;
}

// 샘플 풀을 얼록 하고, 관련 자료구조 만든다.
void CMilesSoundLib::AllocSamplePool()
{
	// 최대 샘플 갯수. 씨댕 이제 찾았네. -_-;
	U32		max_3D_samples;
	AIL_3D_provider_attribute( m_3DDriver, "Maximum supported samples", &max_3D_samples );	

	if( max_3D_samples == 0)
		_asm int 3;

	// 최대 갯수에서 하나 빼야하나.? 혹시 음악이 나오면 하나 빼줘야 하는거 아닌가 모르겠네. -_-;
	max_3D_samples--;
	max_3D_samples--;
	max_3D_samples--;
	
	// 최대 갯수.
	m_dwMaxSamplePoolCount			=	max_3D_samples;

	// 테스트.
//	m_dwMaxSamplePoolCount	=	10;

	// 샘플 풀 버퍼.
	m_pSamplePool					=	new H3DSAMPLE[m_dwMaxSamplePoolCount];
	m_dwSamplePoolCount				=	0;
	H3DSAMPLE		hSample;
	DWORD		i;
	for( i = 0; i < m_dwMaxSamplePoolCount;i++)
	{
		hSample	=	AIL_allocate_3D_sample_handle(m_3DDriver);
		if( hSample)
		{
			m_pSamplePool[i]	=	hSample;
		}
		else
		{
			_asm int 3;  // 이기 뭐꼬? 주어진 샘플 갯수보다 더 생성이 안되네?
			m_dwMaxSamplePoolCount	=	i;
			break;
		}
	}

#ifdef			_DEBUG
	if( i == 0)
	{
		// 한개도 만들지 못했을 경우.
		_asm int 3;
	}
#endif
	

	// 인덱스 크리에이터.
	m_hSamplePoolIndexCreator		=	ICCreate();
	BOOL	bIndexCreator	=	ICInitialize( m_hSamplePoolIndexCreator, m_dwMaxSamplePoolCount);
#ifdef			_DEBUG
	if( bIndexCreator == FALSE)
	{
		_asm int 3;			// critical
	}
#endif

	// 최대 갯수 제한.
	m_dwMaxVoiceCount			=	m_dwMaxSamplePoolCount;	// 풀 얼록 갯수를 제한하는게 아니라 이펙트에 핸들 할당하는 갯수를 제한한다.
	// 대충 끝?

//	m_dwMaxVoiceCount	=	1;
}

void CMilesSoundLib::ReleaseSamplePool()
{
	// 일단 ISoundEffect가 샘플을 다 반환했는가 체크 하고,
	// 해제되지 않고 남은 샘플이 있는 경우 걸림
	assert( ! m_dwSamplePoolCount );

	// 인덱스 크리에이터 해제.
	if( m_hSamplePoolIndexCreator)
	{
		DWORD dwRemainedSample	=	ICGetIndexNum( m_hSamplePoolIndexCreator);
		if( dwRemainedSample != 0)
			_asm int 3;			// 모든 인덱스가 해제된게 아니다.

		ICRelease( m_hSamplePoolIndexCreator);
		m_hSamplePoolIndexCreator	=	0;
	}
	
	// 샘플 풀 해제.
	if( m_pSamplePool )
	{
		for( DWORD i = 0; i < m_dwMaxSamplePoolCount; i++)
		{
			if( m_pSamplePool[i] == 0)
				_asm int 3;					// 디버그용 혹시 널이면???

			AIL_release_3D_sample_handle( m_pSamplePool[i]);
			m_pSamplePool[i]		=	0;
		}
		delete[]	m_pSamplePool;
		m_pSamplePool = 0;
	}
	
	m_dwMaxSamplePoolCount	=	0;
	// 리턴.
}

/*
 *	해당 사운드 이펙트에게 샘플을 발급한다.
 */
BOOL CMilesSoundLib::AllocSampleToSoundEffect(H3DSAMPLE *phOutSample, DWORD *pdwOutIndex)
{
	// 현재 발급해준 인덱스가 다 바닥났나.?
	if( m_dwSamplePoolCount == m_dwMaxSamplePoolCount)
	{
		// 단순히 한계 갯수를 초과했을 뿐이다.
		return	FALSE;
	}

	if( m_dwSamplePoolCount >= m_dwMaxVoiceCount)
	{
		// 최대 동시발급 갯수가 찼다. 과연 효과가 있을까?
		return	FALSE;
	}
	
	// 인덱스 크리에이터에서 빈 인덱스를 하나 발급 받는다.
	DWORD		dwNewIndex	=	ICAllocIndex( m_hSamplePoolIndexCreator) - 1;	// 1베이스.
#ifdef			_DEBUG
	if( dwNewIndex == 0xffffffff)
		_asm int 3;
#endif

	*pdwOutIndex	=	dwNewIndex;
	*phOutSample	=	m_pSamplePool[dwNewIndex];
	
	m_dwSamplePoolCount++;

	return	TRUE;
}

/*
 *	사운드 이펙트가 다 쓴 샘플을 반납한다.
 */
BOOL CMilesSoundLib::ReleaseSampleFromSoundEffect(DWORD dwSampleIndex)
{
	// 해당 인덱스가 쓰이고 있는가?
	// 몰라.
	ICFreeIndex( m_hSamplePoolIndexCreator, dwSampleIndex+1);
	
	m_dwSamplePoolCount--;

	return	TRUE;
}

void CMilesSoundLib::SetMaxVoiceCount( DWORD dwMaxVoice)
{
	if( m_dwMaxSamplePoolCount < dwMaxVoice)
	{
		m_dwMaxVoiceCount	=	m_dwMaxSamplePoolCount;
	}
	else
		m_dwMaxVoiceCount	=	dwMaxVoice;
}

DWORD CMilesSoundLib::GetMaxVoiceCount()
{
	return	m_dwMaxVoiceCount;
}
