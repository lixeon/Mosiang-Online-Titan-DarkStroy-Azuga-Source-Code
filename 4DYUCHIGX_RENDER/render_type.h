#ifndef RENDER_TYPE_H
#define RENDER_TYPE_H

#include "../4DyuchiGRX_Common/IRenderer.h"

struct RENDER_MESHOBJECT_DESC
{
	IDIMeshObject*		pMeshObject;

	DWORD				dwRefIndex;
	float				fDistance;				// 카메라 eye로부터의 거리.
	
	DWORD				dwAlpha;
	LIGHT_INDEX_DESC*	pDynamicLightIndexList;
	DWORD				dwLightNum;
	LIGHT_INDEX_DESC*	pSpotLightIndexList;
	DWORD				dwSpotLightNum;
	DWORD				dwMtlSetIndex;
	DWORD				dwEffectIndex;
	DWORD				dwFlag;

};

// 여기서부터 렌더링 순서..
#define		RENDER_PRIORITY_FACTOR_HFIELD			0.0f

#define		RENDER_PRIORITY_FACTOR_LMOBJECT			-100000.0f;
#define		RENDER_PRIORITY_FACTOR_VLSOBJECT		-100000.0f;
#define		RENDER_PRIORITY_FACTOR_VLOBJECT			-300000.0f;
#define		RENDER_PRIORITY_FACTOR_BILLBOARDOBJECT	-300060.0f;
#define		RENDER_PRIORITY_FACTOR_IMMMESHOBJECT	-900000.0f;
//#define		RENDER_PRIORITY_FACTOR_BILLBOARDOBJECT	-300000.0f;

#endif
