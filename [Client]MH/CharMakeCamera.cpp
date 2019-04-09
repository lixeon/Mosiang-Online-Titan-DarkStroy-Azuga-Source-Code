//
//		Character Make - mode Camera
//
//				2005/03/29
//				trustpak

#include "StdAfx.h"
#include "../4DyuchiGRX_common/typedef.h"
#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/math.inl"
#include "../4DyuchiGXGFunc/global.h"

#include "CharMakeCamera.h"






//////////////////////////////////////////////////////////////////////////////
// Global Functions.
//
//----------------------------------------------------------------------------
// Name : MakeCameraMatrix
// Description :
//----------------------------------------------------------------------------
BOOL MakeCameraMatrix(VECTOR3* pv3Out,
					  VECTOR3* pv3Axis,
					  MATRIX4* pmatPitch,
					  MATRIX4* pmatYaw)
{
	VECTOR3 v3Result;
	VECTOR3 v3Temp;

	TransformVector3_VPTR2(&v3Result,
					       pv3Axis,
						   pmatPitch,
						   1);

	v3Temp.x = v3Result.x;
	v3Temp.y = v3Result.y;
	v3Temp.z = v3Result.z;

	TransformVector3_VPTR2(&v3Result,
						   &v3Temp,
						   pmatYaw,
						   1);	
	

	memcpy(pv3Out, &v3Result, sizeof(VECTOR3));	

	
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////////
// Class Name : CCharMakeCamera
// Description :
//
//----------------------------------------------------------------------------
// Name : CCharMakeCamera::CCharMakeCamera
// Description : Contructor.
//----------------------------------------------------------------------------
CCharMakeCamera::CCharMakeCamera()
{
	memset(this, 0, sizeof(CCharMakeCamera)); // 이런 식의 초기화에서는 절대 가상함수가 존재해서는 안됩니다.
											  // vtable을 덮어버리기 때문입니다. - 작성자 백 -
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::~CCharMakeCamera
// Description : Destructor.
//----------------------------------------------------------------------------
CCharMakeCamera::~CCharMakeCamera()
{
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::InitCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::InitCamera(I4DyuchiGXExecutive* pExecutive, VECTOR3* pv3CameraPos, VECTOR3* pv3CameraAngle, DWORD dwViewPortNo)
{
	memset(this, 0, sizeof(CCharMakeCamera));
	
	m_pExecutive	= pExecutive;
	m_dwViewPortNo	= dwViewPortNo;
	m_pExecutive	= pExecutive;

	m_v3CameraEye	= *pv3CameraPos;
	m_v3Angle		= *pv3CameraAngle;


	if (NULL != m_pExecutive)
	{
		m_pExecutive->GetGeometry()->SetCameraPos(&m_v3CameraEye, 0);
		m_pExecutive->GetGeometry()->SetCameraAngleRad(&m_v3Angle, 0);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::InitCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::MoveCamera(int iLastX, int iLastY, int iCurrentX, int iCurrentY)
{
	// Member variable handling.

	if (NULL == m_pExecutive)
	{
		return FALSE;
	}	

	float fVertMove = 0.0f;
	float fHoriMove = 0.0f;


	// Checking Horizontal movement.

	if (iLastX > iCurrentX)
	{		
		fHoriMove = float(iLastX - iCurrentX);
	}
	else if (iLastX == iCurrentX)
	{
		fHoriMove = 0.0f;
	}
	else
	{		
		fHoriMove = float(iLastX - iCurrentX);
	}

	// Checking Vertical movement.

	if (iLastY > iCurrentY)
	{
		fVertMove = float(iCurrentY -iLastY);
	}
	else if (iLastY == iCurrentY)
	{
		fVertMove = 0.0f;
	}
	else
	{
		fVertMove = float(iCurrentY -iLastY);
	}


	VECTOR3	v3Axis;
	MATRIX4 matrixYaw;
	MATRIX4 matrixPitch;

	VECTOR3 v3CameraAngle;
	m_pExecutive->GetGeometry()->GetCameraAngleRad(&v3CameraAngle, m_dwViewPortNo);
	
	v3CameraAngle.x	*=	-1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라. 

	SetRotationXMatrix(&matrixPitch, v3CameraAngle.x);
	SetRotationYMatrix(&matrixYaw, v3CameraAngle.y);	

	// First, Move X.

	v3Axis.x = 1.0f;
	v3Axis.y = 0.0f;
	v3Axis.z = 0.0f;
	
	VECTOR3 v3Result;

	MakeCameraMatrix(&v3Result,
					 &v3Axis,
					 &matrixPitch,
					 &matrixYaw);

	// Camera Move.

	VECTOR3 v3Move;	

	v3Move.x = v3Result.x * fHoriMove;
	v3Move.y = v3Result.y * fHoriMove;
	v3Move.z = v3Result.z * fHoriMove;
	

	m_pExecutive->GetGeometry()->MoveCamera(&v3Move, m_dwViewPortNo);	

	// Second Move Y

	v3Axis.x = 0.0f;
	v3Axis.y = 1.0f;
	v3Axis.z = 0.0f;


	MakeCameraMatrix(&v3Result,
					 &v3Axis,
					 &matrixPitch,
					 &matrixYaw);

	// Camera Move.	

	v3Move.x = v3Result.x * fVertMove;
	v3Move.y = v3Result.y * fVertMove;
	v3Move.z = v3Result.z * fVertMove;
	

	m_pExecutive->GetGeometry()->MoveCamera(&v3Move, m_dwViewPortNo);

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::ZoomInCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::ZoomInCamera(VOID)
{
	if (c_fZoomMax <= m_fZoom)
	{
		return FALSE;
	}

	// Member variable handling.

	if (NULL == m_pExecutive)
	{
		return FALSE;
	}

	const float c_fMoveDistance = 25.0f;
	
	VECTOR3	v3Axis;
	MATRIX4 matrixYaw;
	MATRIX4 matrixPitch;	

	VECTOR3 v3CameraAngle;
	m_pExecutive->GetGeometry()->GetCameraAngleRad(&v3CameraAngle, m_dwViewPortNo);

	v3CameraAngle.x *= -1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라. 

	SetRotationXMatrix(&matrixPitch, v3CameraAngle.x);
	SetRotationYMatrix(&matrixYaw, v3CameraAngle.y);

	v3Axis.x = 0.0f;
	v3Axis.y = 0.0f;
	v3Axis.z = 1.0f;	
	
	VECTOR3 v3Result;

	MakeCameraMatrix(&v3Result,
					 &v3Axis,
					 &matrixPitch,
					 &matrixYaw);

	// Camera Move.

	VECTOR3 v3Move;	

	v3Move.x = v3Result.x * c_fMoveDistance;
	v3Move.y = v3Result.y * c_fMoveDistance;
	v3Move.z = v3Result.z * c_fMoveDistance;

	m_pExecutive->GetGeometry()->MoveCamera(&v3Move, m_dwViewPortNo);


	m_fZoom += 1.0f;

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::ZoomOutCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::ZoomOutCamera(VOID)
{
	if (c_fZoomMin >= m_fZoom)
	{
		return FALSE;
	}

	// Member variable handling.

	if (NULL == m_pExecutive)
	{
		return FALSE;
	}

	const float c_fMoveDistance = -25.0f;
	
	VECTOR3	v3Axis;
	MATRIX4 matrixYaw;
	MATRIX4 matrixPitch;	


	VECTOR3 v3CameraAngle;
	m_pExecutive->GetGeometry()->GetCameraAngleRad(&v3CameraAngle, m_dwViewPortNo);

	v3CameraAngle.x *= -1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라.
	
	SetRotationXMatrix(&matrixPitch, v3CameraAngle.x);
	SetRotationYMatrix(&matrixYaw, v3CameraAngle.y);

	v3Axis.x = 0.0f;
	v3Axis.y = 0.0f;
	v3Axis.z = 1.0f;

	
	VECTOR3 v3Result;

	MakeCameraMatrix(&v3Result,
					 &v3Axis,
					 &matrixPitch,
					 &matrixYaw);

	// Camera Move.

	VECTOR3 v3Move;	

	v3Move.x = v3Result.x * c_fMoveDistance;
	v3Move.y = v3Result.y * c_fMoveDistance;
	v3Move.z = v3Result.z * c_fMoveDistance;

	m_pExecutive->GetGeometry()->MoveCamera(&v3Move, m_dwViewPortNo);


	m_fZoom -= 1.0f;

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::RotateCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::RotateCamera(GXOBJECT_HANDLE gxh, int iLastX, int iLastY, int iCurrentX, int iCurrentY)
{
	// Member variable handling.

	if (NULL == m_pExecutive)
	{
		return FALSE;
	}

	int iHoriMove = 0;
	int iVertMove = 0;	

	const float c_fRotateAngle = 0.04f;

	// Get Moved distance.

	iHoriMove = iCurrentX - iLastX;
	iVertMove = iCurrentY - iLastY;


	if (0 > iVertMove)
	{
		if (c_fRotPitchMax <= m_fPitchRot)
		{
			return FALSE;
		}
	}
	else if (0 < iVertMove)
	{
		if (c_fRotPitchMin >= m_fPitchRot)
		{
			return FALSE;
		}
	}

	float fYaw		= 0.0f;
	float fPitch	= 0.0f;

	if (0 > iVertMove)
	{
		fPitch = c_fRotateAngle;
	}
	else if (0 == iVertMove)
	{
		fPitch = 0.0f;
	}
	else
	{
		fPitch = -c_fRotateAngle;
	}


	if (0 > iHoriMove)
	{
		fYaw = -c_fRotateAngle;
	}
	else if (0 == iHoriMove)
	{
		fYaw = 0.0f;
	}
	else
	{
		fYaw = c_fRotateAngle;
	}

	CAMERA_DESC camera;
	VECTOR3		v3Result;
	VECTOR3		v3Pivot;
	VECTOR3		v3CameraAngle;
	
	memset(&v3Result, 0, sizeof(VECTOR3));
	memset(&v3Pivot, 0, sizeof(VECTOR3));
	memset(&v3CameraAngle,	0, sizeof(VECTOR3));

	m_pExecutive->GetGeometry()->GetCameraDesc(&camera, m_dwViewPortNo);
	m_pExecutive->GetGeometry()->GetCameraAngleRad(&v3CameraAngle, m_dwViewPortNo);
	
	
	COLLISION_MESH_OBJECT_DESC ColMeshDesc;

	m_pExecutive->GXOGetCollisionMesh(gxh, &ColMeshDesc);

	v3Pivot.x = ColMeshDesc.boundingSphere.v3Point.x;
	v3Pivot.y = ColMeshDesc.boundingSphere.v3Point.y;
	v3Pivot.z = ColMeshDesc.boundingSphere.v3Point.z;


	//SW 값 보정.
	if( v3CameraAngle.x < PI * -0.5f )
		v3CameraAngle.x += 2.f * PI;
	else if( v3CameraAngle.x > PI * 0.5f)
		v3CameraAngle.x -= 2.f * PI;

	RotatePositionWithPivot(&v3Result,
						    &v3Pivot,
							&camera.v3From,
							&v3CameraAngle,
							fYaw,
							fPitch);

	VECTOR3 v3CameraRotate;

	v3CameraRotate.x = fPitch;
	v3CameraRotate.y = fYaw;
	v3CameraRotate.z = 0.0f;

	m_pExecutive->GetGeometry()->SetCameraPos(&v3Result, m_dwViewPortNo);
	m_pExecutive->GetGeometry()->RotateCameraRad(&v3CameraRotate, m_dwViewPortNo);



	// for prevent too much rotated camera.	
	m_fPitchRot += fPitch;

	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : CCharMakeCamera::ResetCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CCharMakeCamera::ResetCamera(VOID)
{	
	VECTOR3 v3CameraEye;

	v3CameraEye.x = c_fCameraEyeX;
	v3CameraEye.y = c_fCameraEyeY;
	v3CameraEye.z = c_fCameraEyeZ;	

	m_pExecutive->GetGeometry()->ResetCamera(&v3CameraEye, 
											 DEFAULT_NEAR/10.0f,
											 DEFAULT_FAR/10.0f,
											 DEFAULT_FOV,
											 0);

	return TRUE;
}

//----------------------------------------------------------------------------
