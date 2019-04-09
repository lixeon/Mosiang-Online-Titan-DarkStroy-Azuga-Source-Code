//
//	Model View Camera Module
//
//			2003/01/17 ~ 2003/01/18
//			Seong-hee Park


#include "StdAfx.h"
#include "Camera.h"
#include "ToolInfo.h"





//////////////////////////////////////////////////////////////////////////////
// Class Name : CModelViewCamera
// Description :
//
//----------------------------------------------------------------------------
// Name : CModelViewCamera::CModelViewCamera
// Description : Contructor.
//----------------------------------------------------------------------------
CModelViewCamera::CModelViewCamera()
{
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::~CModelViewCamera
// Description : Destructor.
//----------------------------------------------------------------------------
CModelViewCamera::~CModelViewCamera()
{
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::InitCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::InitCamera(I4DyuchiGXExecutive* pExecutive, DWORD dwViewPortNo)
{
	memset(this, 0, sizeof(CModelViewCamera));
	m_pExecutive = pExecutive;
	m_dwViewPortNo = dwViewPortNo;

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::InitCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::MoveCamera(int iLastX, int iLastY, int iCurrentX, int iCurrentY)
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
	
	v3CameraAngle.x	*=	-1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라. 2003/01/20

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
// Name : CModelViewCamera::ZoomInCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::ZoomInCamera(VOID)
{
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

	v3CameraAngle.x *= -1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라. 2003/01/20

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

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::ZoomOutCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::ZoomOutCamera(VOID)
{
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

	v3CameraAngle.x *= -1.0f; // 현재 xrotate가 반대로 나온다. 언제 고쳐질지 몰라. 2003/01/20
	
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

	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::RotateCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::RotateCamera(GXOBJECT_HANDLE gxh, int iLastX, int iLastY, int iCurrentX, int iCurrentY)
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

	g_pExecutive->GetGeometry()->GetCameraDesc(&camera, m_dwViewPortNo);
	g_pExecutive->GetGeometry()->GetCameraAngleRad(&v3CameraAngle, m_dwViewPortNo);
	
	
	if (NULL == gxh)
	{

		// rotate around Grid.

		if (TRUE == g_ToolInfo.m_bIsGrid)
		{
			COLLISION_MESH_OBJECT_DESC ColMeshDesc;

			g_pExecutive->GXOGetCollisionMesh(g_ToolInfo.m_gxhGrid, &ColMeshDesc);

			v3Pivot.x = ColMeshDesc.boundingSphere.v3Point.x;
			v3Pivot.y = ColMeshDesc.boundingSphere.v3Point.y;
			v3Pivot.z = ColMeshDesc.boundingSphere.v3Point.z;
		}
		else
		{
			v3Pivot.x		= camera.v3To.x;
			v3Pivot.y		= camera.v3To.y;
			v3Pivot.z		= camera.v3To.z;
		}
	}
	else
	{		

		COLLISION_MESH_OBJECT_DESC ColMeshDesc;

		g_pExecutive->GXOGetCollisionMesh(gxh, &ColMeshDesc);

		v3Pivot.x = ColMeshDesc.boundingSphere.v3Point.x;
		v3Pivot.y = ColMeshDesc.boundingSphere.v3Point.y;
		v3Pivot.z = ColMeshDesc.boundingSphere.v3Point.z;

	}

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

	g_pExecutive->GetGeometry()->SetCameraPos(&v3Result, m_dwViewPortNo);
	g_pExecutive->GetGeometry()->RotateCameraRad(&v3CameraRotate, m_dwViewPortNo);

	return TRUE;	
}

//----------------------------------------------------------------------------
// Name : CModelViewCamera::ResetCamera
// Description : 
//----------------------------------------------------------------------------
BOOL CModelViewCamera::ResetCamera(VOID)
{	
	VECTOR3 v3CameraEye;

	v3CameraEye.x = c_fCameraEyeX;
	v3CameraEye.y = c_fCameraEyeY;
	v3CameraEye.z = c_fCameraEyeZ;	

	g_pExecutive->GetGeometry()->ResetCamera(&v3CameraEye, 
											 DEFAULT_NEAR/10.0f,
											 DEFAULT_FAR/10.0f,
											 DEFAULT_FOV,
											 0);

	return TRUE;
}

//----------------------------------------------------------------------------


