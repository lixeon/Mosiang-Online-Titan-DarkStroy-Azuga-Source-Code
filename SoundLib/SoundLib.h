
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOUNDLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOUNDLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
/*
	왼손 좌표계.
	x는 오른쪽, y는 위, z는 앞이라 가정한다.
*/
#ifndef	_SoundLib_h_
#define	_SoundLib_h_


#ifdef SOUNDLIB_EXPORTS
#define SOUNDLIB_API extern "C" __declspec(dllexport)
#else
#define SOUNDLIB_API extern "C" __declspec(dllimport)
#endif

#include <windows.h>
#include "../4DyuchiGRX_common/typedef.h"


enum	SOUND_3D_DRIVER
{
	SOUND_DRIVER_TYPE_NONE,
	SOUND_DRIVER_TYPE_EAX3,
	SOUND_DRIVER_TYPE_EAX2,
	SOUND_DRIVER_TYPE_EAX1,
	SOUND_DRIVER_TYPE_DS3DHW,
	SOUND_DRIVER_TYPE_DS3DSW,
	SOUND_DRIVER_TYPE_MILES,
	SOUND_DRIVER_TYPE_DOLBY
};

enum	SOUND_SPEAKER_TYPE
{
	SOUND_SPEAKER_TYPE_STEREO,			// 일반적인 2체널.
	SOUND_SPEAKER_TYPE_HEADPHONE,		// 헤드폰, 사운드카드의 종류에 따라 헤드폰 엠프등이 작동한다.
	SOUND_SPEAKER_TYPE_4CH,				// 4체널 포지셔닝.
	SOUND_SPEAKER_TYPE_6CH,				// 5.1체널 포지셔닝.
	SOUND_SPEAKER_TYPE_8CH,				// 7.1체널 포지셔닝. 프로디지7.1 등에서 지원된다.
	SOUND_SPEAKER_TYPE_UNKNOWN			// miles에서 서라운드형이란걸 지원하는데 뭔지 모름. 
};

typedef		void*	SOUND_FILE_HANDLE;

// IMilesSoundLib::GetLastError 로 리턴되는 에러값.
enum SOUND_ERROR_CODE
{
	SOUND_ERROR_NOERROR,
	SOUND_ERROR_ALREADY_INITIALIZED,
	SOUND_ERROR_INVALID_ARGUMENT,
	SOUND_ERROR_MILES_STARTUP_FAILED,
	SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,			// Init중 사운드 카드가 없을 경우 이 메시지를 내고 튀어나온다.
	SOUND_ERROR_3DDRIVER_STARTUP_FAILED,			// Init중 3D 드라이버를 못찾으면 에러를 낸다. 이럴 경우가 있을까?
	SOUND_ERROR_BGMDRIVER_NOT_LOADED,
	SOUND_ERROR_UNABLE_OPEN_FILE,
	SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
	SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
};

struct	ISoundEffect
{
	virtual	void	_stdcall	Play( BOOL bLoop)=0;
	/*
		사운드 재생.
		
		BOOL	bLoop     이것이 TRUE면 루프를 돈다.

		설정된 위치, 설정된 볼륨, 설정된 max,min을 바탕으로 소리를 재생한다.
		내부적으로는 Play 하는 순간 Miles의 샘플과 파일을 연결시키고 start 한다.
	*/
	virtual void	_stdcall	Stop(void)=0;
	/*
		소리를 정지시킨다.
		내부적으로는 Stop 하는 순간 소리를 end 시키고 Miles의 샘플을 릴리즈 한다.
	*/

	virtual	void	_stdcall	SetMaxMinDistance( float fMaxDist, float fMinDist)=0;
	/*
		소리가 들리는 최대 거리, 최소 거리를 넣는다.

		float fMaxDist			// 최대 거리. 
		float fMinDist			// 최소 거리. 거리 0부터 이 거리까지는 볼륨이 줄어들지 않는다.

		fMinDist와 fMaxDist는 공부를 좀 하거나 시행착오를 겪어가면서 세팅 해야할 듯.
	*/

	virtual	void	_stdcall	SetPosition( VECTOR3* pPosition)=0;
	/*
		이펙트의 위치를 설정한다.

		VECTOR3* pPosition			월드에서의 이펙트의 좌표를 설정한다.

		SetPosition을 할 경우, IMilesSoundLib::SetListener 에서 설정한 카메라를 참고해서 카메라에 상대적인 위치를 세팅 한다.
	*/

	virtual	void	_stdcall	SetRelativePosition( VECTOR3* pPosition)=0;
	/*
		이펙트의 상대적 위치를 설정한다.

		VECTOR3* pPosition			카메라에서의 상대 위치.

		SetPosition()과 다르게 이것은 카메라를 0,0,0에 위치시킨 상태로 가정하고 그 절대값을 세팅하는 효과를 준다.
	*/

	virtual	void	_stdcall	SetVolume( float fVolume)=0;
	/*
		볼륨을 설정한다.

		float fVolume			0.0f 부터 1.0f 까지 볼륨을 설정할 수 있다.
	*/
	virtual	float	_stdcall	GetVolume(void)=0;
	/*
		볼륨을 얻는다.
	*/

	virtual	BOOL	_stdcall	IsAlive(void)=0;
	/*
		사운드가 재생되고 sample을 잡고 있는가를 리턴한다.
	*/

	virtual	BOOL	_stdcall	IsLoop(void)=0;
	/*
		루프 플랙이 쎗 되어있는가.?
	*/

	virtual	DWORD	_stdcall	AddRef()=0;
	virtual	DWORD	_stdcall	Release()=0;
	/*
		레퍼런스 카운터를 뺀다.
		일반적으로 Release() 하는 순간 레퍼런스카운터가 0이 되면서 메모리에서 사라진다.
	*/
};

struct	IMilesSoundLib
{
	virtual	SOUND_ERROR_CODE _stdcall Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware) = 0;// 사운드 메니저 초기화.
	/*
		IMilesSoundLib을 초기화 한다.

		DWORD	dwMaxSoundFileCount는 최대한 올려 놓을 수 있는 wav, mp3파일의 갯수,
		DWORD	dwMaxSoundEffectCount는 게임 객체가 소리를 낼 소리 객체 최대 갯수.
		char*	szMilesPass는 마일즈가 깔려있는 디렉토리 기본적으로 "redist"이지만 "miles"로 옮기길..
		BOOL	bTryEAX는 3D 드라이버를 잡을 때 eax를 시도 하는가 여부를 알린다.
		BOOL	bForceSoftware는 소프트웨어드라이버를 강제로 시도한다. 대부분의 경우 성공할 것이고, 이때 bTryEAX는 무시된다.
		리턴값은 별 문제 없이 성공하면 SOUND_ERROR_NOERROR,
		Init가 두번째 되는 경우 SOUND_ERROR_ALREADY_INITIALIZED,
		들어온 인자가 이상할 경우 SOUND_ERROR_INVALID_ARGUMENT,
		마일즈 초기화에 실패하면 SOUND_ERROR_MILES_STARTUP_FAILED,
		BGM드라이버를 찾지 못할 경우, 사운드카드가 없다면 SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,
		3D 사운드 드라이버를 올리지 못하면 SOUND_ERROR_3DDRIVER_STARTUP_FAILED
		를 벹어낸다.
		이중 사운드카드가 없을경우 를 대비해 SOUND_ERROR_BGMDRIVER_STARTUP_FAILED는 처리해주고,
		나머지 에러는 프로그래머를 찾도록. -_-;
	*/

	virtual	SOUND_3D_DRIVER		_stdcall	Get3DDriver(void)=0;
	/*
		현재 잡혀있는 3D 드라이버를 리턴한다.
	*/

	virtual	void				_stdcall	SetListener( VECTOR3* pPosition, VECTOR3* pAngle)=0;
	/*
		청자의 위치와 보는 각을 결정한다.
		VECTOR3* pPosition		 청자의 위치.
		VECTOR3* pAngle			청자가 보는 각도, radius 단위이다.

		한 게임 프레임 내에서 다른 ISoundEffect에 SetPosition하기 전에 먼저 세팅 되는 것이 멀쩡하다.
	*/

	virtual	SOUND_SPEAKER_TYPE	_stdcall Get3DSpeakerType(void)=0;
	/*
		잡혀있는 3D driver가 사용하는 스피커 배치의 형태를 알아낸다.
		이는 제어판/사운드 의 고급정보에 있는 스피커 배치와 같을듯.
	*/

	virtual	void				_stdcall Set3DSpeakerType( SOUND_SPEAKER_TYPE Type)=0;
	/*
		3D driver가 사용하는 스피커 타입을 강제로 설정한다.
	*/

	// BGM
	virtual	SOUND_ERROR_CODE _stdcall PlayBGM( char* szBGMFileName)=0;
	/*
		szBGMFileName으로 몇시된 파일을 백그라운드로 플레이 한다.
		
		char* szBGMFileName		에는 BGM파일 이름을 넣는다. .wav, .mp3등이 가능하다.

		성공할 경우 SOUND_ERROR_NOERROR,
		Init가 성공하지 못한 경우 SOUND_ERROR_BGMDRIVER_NOT_LOADED 을 리턴하고 아무짓도 안한다.
		파일 이름이 잘못됬을 경우 SOUND_ERROR_UNABLE_OPEN_FILE
		을 리턴한다.

	*/

	virtual	void			 _stdcall StopBGM(void)=0;
	/*
		플레이 되고 있는 BGM을 정지한다.
		Init가 성공하지 안은 상태에서 호출되도 상관 없다. 
	*/

	virtual void			_stdcall SetBGMVolume( float fVolume)=0;
	/*
	 *	BGM의 볼륨을 설정한다. 범위는 1.0f 에서 0.0f까지.
	 */

	// Sound Effect File
	virtual SOUND_ERROR_CODE _stdcall CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char* szFileName)=0;
	/*
		사운드 파일에 대한 핸들을 하나 만든다. 생성시의 속도를 위해 사운드 이펙트는 파일 이름으로 바로 생성되지 안으므로
		파일 핸들을 하나 만들고 이 핸들로 사운드 이펙트를 만든다.

		SOUND_FILE_HANDLE* pOutHandle		사운드 파일 핸들을 받을 포인터.
		char* szFileName					할당할 파일 이름.

		성공할 경우 SOUND_ERROR_NOERROR,
		BGM드라이버가 올라가지 못한 경우(사운드카드가 없는 경우 등..) SOUND_ERROR_BGMDRIVER_NOT_LOADED,
		주어진 인자에 문제가 있을 경우 SOUND_ERROR_INVALID_ARGUMENT,
		최대 파일 개체 갯수를 초과할 경우 SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
		주어진 파일을 열지 못했을 경우 SOUND_ERROR_UNABLE_OPEN_FILE
		을 리턴한다.

		내부적으로 리소스 공유가 되므로 같은 파일이름에 대해 Creat를 하면 미리 발급 되었던 핸들을 리턴하고,
		레퍼런스 카운터가 1 올라간다.
	*/

	virtual	void			 _stdcall ReleaseSoundEffectFile( SOUND_FILE_HANDLE hFile)=0;
	/*
		hFile로 주어진 파일을 릴리즈 한다.
		
		하나의 파일에 대해 핸들이 여러개 발급될 것이므로, 내부적으로 hFile의 레퍼런스 카운터를 하나 떨어뜨린다.
		레퍼런스 카운터가 0이 되는 순간 실제 메모리에서 없어진다.
	*/

	virtual	SOUND_ERROR_CODE _stdcall CreateSoundEffect( ISoundEffect** ppSoundEffect, SOUND_FILE_HANDLE hSound)=0;
	/*
		hSound 파일 핸들을 플레이 하는 사운드 객체를 만든다.
		만드는 순간 플레이가 되는 것은 아니고 만든 후 받은 인터페이스로 Play 해줘야 한다.

		ISoundEffect** ppSoundEffect		인터페이스 포인터.
		SOUND_FILE_HANDLE hSound			만들려는 사운드의 파일 핸들.

		성공하면 SOUND_ERROR_NOERROR,
		인자로 주어진 hSound가 엄한 값이면 SOUND_ERROR_INVALID_ARGUMENT,
		Init때 주어진 최대 이펙트 갯수를 초과하면 SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
		를 리턴한다.

		받은 ISoundEffect 객체는 지울때 ISoundEffect->Release()를 사용한다.
	*/

	virtual	void			_stdcall	Run()=0;

	virtual	DWORD			_stdcall	GetAllocatedSoundEffectCount()=0;
	/*
		현재 잡혀있는 사운드 이팩트의 갯수를 리턴한다.

		디버그용.
	*/
	virtual	DWORD			_stdcall	GetAllocatedSoundFileCount()=0;
	/*
		현재 잡혀있는 사운드 파일의 갯수를 리턴한다.

		디버그용.
	*/

	
	virtual	DWORD			_stdcall	ReleaseAllSoundEffect(void)=0;
	/*
		잡혀있던 모든 Effect를 지운다.

		이시점까지 릴리즈 되지 않은 Sound Effect의 갯수가 리턴된다.

		웬만하면 이 함수 쓰지 말고 그때그때 하나하나 직접 지워줄 것을 추천한다.
	*/

	virtual	DWORD			_stdcall	ReleaseAllSoundFile(void)=0;
	/*
		잡혀있던 모든 File을 지운다.

		이시점까지 릴리즈 되지 않은 Sound Effect File의 갯수가 리턴된다.

		웬만하면 이 함수 쓰지 말고 그때그때 하나하나 직접 지워줄 것을 추천한다.
	*/

	virtual	void _stdcall Delete() = 0;
	/*
		ISoundLib의 소멸자.

		리소스를 제대로 해제하지 않은상태에서 호출되면 무시무시한 인터럽트들이 걸릴 것이리.
	*/

	virtual	void _stdcall GetErrorString( char** pszErrorString, SOUND_ERROR_CODE)=0;
	/*
		SOUND_ERROR_NOERROR 외의 에러값을 받았을때 그것들을 문자열로 치환해준다.
		디버깅용 함수.
	*/

	virtual	void	_stdcall	SetMaxVoiceCount( DWORD dwMaxVoice)=0;
	virtual	DWORD	_stdcall	GetMaxVoiceCount()=0;

};


SOUNDLIB_API BOOL CreateSoundLib( IMilesSoundLib** ppISoundLib);



#endif