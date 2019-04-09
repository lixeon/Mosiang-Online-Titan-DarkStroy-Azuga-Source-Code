//
//		Character Make - mode Camera
//
//				2005/03/29
//				trustpak




#ifndef _CHARMAKE_CAMERA_
#define _CHARMAKE_CAMERA_

/// Constants ////////////////////////////////////////////////////////////////

const float c_fCameraEyeX = 0.0f;
const float c_fCameraEyeY = 10.0f;
const float c_fCameraEyeZ = -1000.0f;
const float c_fCameraToX  = 0.0f;
const float c_fCameraToY  = 0.0f;
const float c_fCameraToZ  = 0.0f;
const float c_fCameraUpX  = 0.0f;
const float c_fCameraUpY  = 0.0f;
const float c_fCameraUpZ  = 0.0f;


const float c_fZoomMin		= -2;
/*const float c_fZoomMax		= 3;
const float c_fRotPitchMin	= -0.24f;
const float c_fRotPitchMax	= 0.20f;*/
//SW070725 캐릭터선택/생성창
const float c_fZoomMax		= 4;
const float c_fRotPitchMin	= -0.80f;
const float c_fRotPitchMax	= 0.f;

/// Type defines /////////////////////////////////////////////////////////////

class CCharMakeCamera
{
public:
	CCharMakeCamera();
	~CCharMakeCamera();

	BOOL InitCamera(I4DyuchiGXExecutive* pExecutive, VECTOR3* pv3CameraPos, VECTOR3* pv3CameraAngle, DWORD dwViewPortNo = 0);
	
	VOID SetViewPortNo(DWORD dwViewPortNo)	{ m_dwViewPortNo = dwViewPortNo; }
	INT  GetViewPortNo(VOID)				{ return m_dwViewPortNo; }
	
	
	BOOL ZoomInCamera(VOID);
	BOOL ZoomOutCamera(VOID);

	BOOL MoveCamera(int iLastX, int iLastY, int iCurrentX, int iCurrentY);
	BOOL RotateCamera(GXOBJECT_HANDLE gxh, int iLastX, int iLastY, int iCurrentX, int iCurrentY);	

	BOOL ResetCamera(VOID);


	// particular methods..
	VOID SetCameraScope(float fZoomIn, float fZoomOut, float fPitchRotMin, float fPitchRotMax);

private:

	INT			m_iCameraNo;	// -1 : No Set

	VECTOR3		m_v3Axis;
	VECTOR3		m_v3Angle;
	VECTOR3		m_v3CameraEye;
	VECTOR3		m_v3CameraTo;
	VECTOR3		m_v3CameraUp;

	DWORD		m_dwViewPortNo;
	I4DyuchiGXExecutive* m_pExecutive;


	// particular properties.
	
	float		m_fZoom;
	float		m_fPitchRot;	
	
};










#endif