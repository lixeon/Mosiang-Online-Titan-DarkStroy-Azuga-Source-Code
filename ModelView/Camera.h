//
//	Model View Camera Module
//
//			2003/01/17 ~ 2003/01/18
//			Seong-hee Park



#include "EngineRelation.h"



#ifndef _MODEL_VIEW_CAMERA_
#define _MODEL_VIEW_CAMERA_



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


/// Type defines /////////////////////////////////////////////////////////////

class CModelViewCamera
{
public:
	CModelViewCamera();
	~CModelViewCamera();

	BOOL InitCamera(I4DyuchiGXExecutive* pExecutive, DWORD dwViewPortNo = 0);

	VOID SetViewPortNo(DWORD dwViewPortNo)	{ m_dwViewPortNo = dwViewPortNo; }
	INT  GetViewPortNo(VOID)				{ return m_dwViewPortNo; }
	
	
	BOOL ZoomInCamera(VOID);
	BOOL ZoomOutCamera(VOID);

	BOOL MoveCamera(int iLastX, int iLastY, int iCurrentX, int iCurrentY);
	BOOL RotateCamera(GXOBJECT_HANDLE gxh, int iLastX, int iLastY, int iCurrentX, int iCurrentY);	

	BOOL ResetCamera(VOID);

private:

	INT			m_iCameraNo;	// -1 : No Set

	VECTOR3		m_v3Axis;
	VECTOR3		m_v3Angle;
	VECTOR3		m_v3CameraEye;
	VECTOR3		m_v3CameraTo;
	VECTOR3		m_v3CameraUp;

	DWORD		m_dwViewPortNo;
	I4DyuchiGXExecutive* m_pExecutive;
	
};



#endif



