// MHCamera.h: interface for the CMHCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_)
#define AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Angle.h"
#include ".\Engine\EngineCamera.h"

class CObject;
class CEngineObject;


struct MHCAMERASHAKE;


#define MAX_CAMERA_DESC	8
#define CAMERA	USINGTON(CMHCamera)

enum eCAMERAMODE
{
	eCM_Free = 0,
	eCM_QuarterView,
	eCM_FollowView,
	eCM_LOGIN,
	eCM_EyeView,
};

enum eCAMERASTATE
{
	eCS_Normal,
	eCS_Sit,
	eCS_Die,
};

enum
{
	eCP_CHARMAKE = 0,
	eCP_CHARSEL,
	eCP_INTERPOLATE,

	eCP_MAX,
};

struct MHCAMERAPOS
{
	float m_AngleX;
	float m_AngleY;
	float m_Dist;

	VECTOR3 m_Pivot;
};


struct MHCAMERADESC
{
	CAngle m_AngleX;
	CAngle m_AngleY;

	float m_fDistance;

	VECTOR3 m_CameraPos;
	VECTOR3 m_CameraDir;
	
	VECTOR3 m_CameraDownVector;
	VECTOR3 m_CameraDownPositon;

	BOOL m_bCameraAngleMovingX;
	DWORD m_CameraAngleMoveStartX;
	DWORD m_CameraAngleMoveDurationX;
	
	BOOL m_bCameraAngleMovingY;
	DWORD m_CameraAngleMoveStartY;
	DWORD m_CameraAngleMoveDurationY;

	BOOL m_bCameraDistanceMoving;
	DWORD m_CameraDistanceMoveStart;
	DWORD m_CameraDistanceMoveDuration;
	float m_fDistanceStart;
	float m_fDistanceChange;

	float m_CharHeight;			//운기중에는 카메라가 서서히 내려갔으면.
	float m_StartCharHeight;
	float m_TargetCharHeight;
	BOOL  m_bHeightChanging;
	DWORD m_dwHeightStartTime;
	DWORD m_dwHeightChangeDuration;

	CObject* m_pPivotObject;	// 둘중에 하나는 꼭 있어야 한다.
	VECTOR3 m_PivotPos;

	BOOL m_bPivotObject;		// TRUE 면 PivotObject FALSE 면 PivotPos
	BOOL m_bTitanCam;			// 타이탄용 카메라인지 
};


class CMHCamera  
{
	//camerafilter
	CEngineObject* m_pCameraFilter;
	float m_MinAngle;
	float m_MaxAngle;
	
	VECTOR3 m_DownVector;
	DWORD m_CurCameraMode;
	CEngineCamera m_EngineCamera;

	MHCAMERADESC m_CameraDesc[MAX_CAMERA_DESC];
	int m_CurCamera;

	float m_fAddHeight;

//---sight distance option
	float m_fDistanceRate;	//50~100% 
	float m_fMaxSightDistance;
	float m_fFov;


	//////////////////////////////////////////////////////////////////////////
	// 진동 기능 구현을 위한 변수들
	VECTOR3 m_VibratePos;
	VECTOR3 m_VibrateAngle;
	VECTOR3 m_PushedVector;
	DWORD m_VibrateDuration;
	DWORD m_VibrateStartTime;
	BOOL m_bIsVibrating;


	void CalcVibration();
	//////////////////////////////////////////////////////////////////////////	

	
	//////////////////////////////////////////////////////////////////////////
	// 쉐이크 기능을 위한 변수들
	MHCAMERASHAKE** m_ppCameraShakeDesc;		// 쉐이킹 에 대한 Desc 들..
	VECTOR3 m_ShakingPos;
	BOOL m_bIsShaking;
	DWORD m_nCurShakingNum;						// 현재 하고 있는 쉐이킹 Desc Index (-1이면 하고 있지 않다)
	DWORD m_CurShakingPos;						// 현재 진행중인 쉐이킹 Desc내에서의 위치
	DWORD m_LastShakingPosChangedTime;			// 마지막으로 m_CurShakingPos를 바꾼 시간.
	float m_Amplitude;

	void CalcShaking();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 이동보간을 위한 변수들
	MHCAMERAPOS m_MovePos[eCP_MAX];
	int m_CurPosition;
	int m_GotoPosition;
	
	float m_MoveTime;
	float m_MoveStartTime;
	float m_MoveSpeed;
	
	BOOL m_bMoved;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 카메라자유이동을 위한 변수들
	DWORD m_LastMoveNoticeTime;			// 캐릭터가 이동했다고 서버에 알려준 시간



	//////////////////////////////////////////////////////////////////////////
	
	
	void CalcCameraPosAngle();
	void CalcFreeCamera();
	void ProcessCameraMode();
	void ProcessCameraMoving();

	void RestrictCameraAngleDistance();
	
	void FollowCamera();

public:
	CMHCamera();
//	//MAKESINGLETON(CMHCamera);
	~CMHCamera();

	void Init(CObject* pObject,float angleX,float angleY,float fDistance);
	void InitCamera(int CameraNum,float angleX,float angleY,float fDistance,CObject* pObject, BOOL bTitanCam=FALSE);
	void InitCamera(int CameraNum,float angleX,float angleY,float fDistance,VECTOR3* pPivotPos);

	void SetToEngineCamera( BOOL bForce=FALSE );


	void SetCameraFilter(CEngineObject* pEngineObj,float minAngle = 0,float maxAngle = gPHI/4);


	void MouseRotate(int x,int y);

	void Process();

	void SetCurCamera(int Camera);
	void SetCameraPivotObject(int Camera,CObject* pObject);
	void SetCameraPivotPos(int Camera,VECTOR3* pPos);

	void SetTargetAngleX(int Camera,float fAngleX,DWORD Duration);
	void SetTargetAngleY(int Camera,float fAngleY,DWORD Duration);
	void SetDistance(int Camera,float fDistance,DWORD Duration);
	float GetDistance(int Camera) { return m_CameraDesc[Camera].m_fDistance; }

	void Rotate(int Camera,float RotateAngle,DWORD Duration);
	void ZoomDistance(int Camera,float fDistanceChange,DWORD Duration);

	void ChangeCameraSmooth(int Camera,DWORD Duration);	// 현재 카메라에서 다른 카메라로 부드럽게 이동한다.
	void ChangeToDefaultCamera();	// 현재 카메라의 설정값을 그대로 0번 카메라에 셋팅한다.
	void StopAllMoving(int Camera);	// 회전이나 줌등을 멈춘다.

	void SetMaxSightDistance( float fMaxDistance )		{ m_fMaxSightDistance = fMaxDistance; m_fDistanceRate = 0.0f;  }
	void SetCameraFov( float fFov )						{ m_fFov = fFov; }

	void SetSightDistanceRate( float fDistanceRate );


	//////////////////////////////////////////////////////////////////////////
	// 진동기능을 위한 함수들
	void SetPushedVibration(VECTOR3* PushedVector,DWORD Duration);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// 쉐이킹 기능을 위한 함수들
	void SetShaking(int ShakeNum, float amp);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 이동보간을 위한 함수들
	void SetMovePos(int Index, MHCAMERAPOS* pCameraPos);
	void MoveStart(int GotoIndex);
	void Interpolation();
	BOOL IsMoved()				{	return m_bMoved;	}
	int GetGotoPosition()		{	return m_GotoPosition;	}
	void ResetInterPolation();
	//////////////////////////////////////////////////////////////////////////


	int GetCameraMode() { return m_CurCameraMode; }
	void SetCameraMode(int CameraMode);	
	void ToggleCameraViewMode();
	
	void SetCharState( int eCharState, DWORD dwDuration );
//	BOOL CanChangeCameraAngleTo( float xRad, float yRad );
	void Fly( float fFly );
	
	// 2005 크리스마스 이벤트
	// 현재 카메라 정보를 가져오는 함수
	MHCAMERADESC* GetCameraDesc() { return &m_CameraDesc[m_CurCamera]; }
};
EXTERNGLOBALTON(CMHCamera)
#endif // !defined(AFX_MHCAMERA_H__3EE48679_085F_4E12_AF97_C06D8AB29A00__INCLUDED_)
