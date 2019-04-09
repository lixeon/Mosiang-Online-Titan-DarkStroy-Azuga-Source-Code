#ifndef RENDER_TYPE_H
#define RENDER_TYPE_H

#include "../4DyuchiGRX_Common/IGeometry.h"

class CoModel;
class CoStaticModel;
class CoHeightField;
class CoBrush;

struct RENDER_MODEL_DESC
{
	CoModel*			pModel;
	DWORD				dwRefIndex;
	MATRIX4*			pMatParent;
	MATRIX4*			pMatTransform;
	DWORD				dwFrame;
	DWORD				dwMotionIndex;
	DWORD				dwAlpha;
	DWORD				dwMtlSetIndex;
	DWORD				dwEffectIndex;
	DWORD				dwFlag;
};

struct RENDER_STATIC_MODEL_DESC
{
	CoStaticModel*		pStaticModel;
	DWORD				dwObjIndex;
	DWORD				dwAlpha;
	DWORD				dwMtlSetIndex;
	DWORD				dwFlag;
};

struct RENDER_HFIELD_DESC
{
	CoHeightField*		pHField;
	DWORD				dwObjIndex;
	DWORD				dwAlpha;
	DWORD				dwFlag;
};



#define		TYPEID_RENDER_HFIELD		0x00000000
#define		TYPEID_RENDER_STATIC_MODEL	0x00000001
#define		TYPEID_RENDER_MODEL			0x00000002


#endif
