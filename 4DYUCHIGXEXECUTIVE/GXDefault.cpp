#include "GXDefault.h"
#include "executive.h"

DWORD __stdcall GXDefaultSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData)
{
	
	DWORD	dwMotionIndex = pExecutive->GXOGetCurrentMotionIndex(gxh);
	DWORD	dwFrame = pExecutive->GXOGetCurrentFrame(gxh);
	MOTION_DESC motionDesc;
	BOOL bHasMotion = pExecutive->GXOGetMotionDesc(gxh,&motionDesc,dwMotionIndex,0);

	float	frad = PI/16;

//	RENDER_TYPE_ENABLE_LIGHT_ANIMATION		=	0x00000001,
//	RENDER_TYPE_ENABLE_CAMERA_ANIMATION		=	0x00000100,
//	RENDER_TYPE_UPDATE_SHADING				=	0x00010000,
//	RENDER_TYPE_NOT_DRAW					=	0x00100000

//	VECTOR3*	pAxis;
	
//	Sleep(50);
//	pExcutive->GetDirection(gxh,&pAxis,&frad);
//	pExcutive->SetDirection(gxh,pAxis,frad + PI/4);
	
	//	RENDER_TYPE_ENABLE_CAMERA_ANIMATION);
	//	RENDER_TYPE_ENABLE_LIGHT_ANIMATION | 
	//	RENDER_TYPE_UPDATE_SHADING | 
	//	RENDER_TYPE_FILTER_LIGHT_RANGE );
	

	if (bHasMotion)
	{
		pExecutive->GXOSetCurrentFrame(gxh,dwFrame+arg1);
	
		if (motionDesc.dwLastFrame <= dwFrame)
			pExecutive->GXOSetCurrentFrame(gxh,0);
	}

	return 0;
}

DWORD __stdcall GXDefaultLightSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXLIGHT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData)
{
//	VECTOR3		v3mov;
//	v3mov.x = 0.0f;
//	v3mov.y = -0.001f;
//	v3mov.z = 0.0f;
//	VECTOR3*	pv3Pos;
//	pExcutive->GXLMovePosition(gxh,&v3mov);
//	pExcutive->GXLGetPosition(gxh,&pv3Pos);
	

	return 0;
}

DWORD __stdcall GXDefaultDecalSchedulePROC(I4DyuchiGXExecutive* pExecutive,GXDECAL_HANDLE gxd,DWORD msg,int arg1,int arg2,void* pData)
{
	DWORD	dwAlpha = pExecutive->GetAlphaFlag(gxd);
	if (dwAlpha >= 255)
		dwAlpha = 0;
	else 
		dwAlpha++;
	
	
	pExecutive->SetAlphaFlag(gxd,dwAlpha);


	return 0;
}
DWORD __stdcall GXDefaultMapSchedulePROC(DWORD msg,int frame_ince,int arg2,void* pData)
{
	return 0;
}
