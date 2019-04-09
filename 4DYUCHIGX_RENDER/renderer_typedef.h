#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include <d3d8.h>

//#define _GEFORCE3_

enum DISPLAY_DEVICE_TYPE
{
	
};

enum RESORUCE_POOL_TYPE
{
	
	RESORUCE_POOL_TYPE_VBSTATICPOOL		= 0,
	RESORUCE_POOL_TYPE_IBSTATICPOOL		= 1,
	RESORUCE_POOL_TYPE_VBHEAP			= 2,
	RESORUCE_POOL_TYPE_IBHEAP			= 3
};

enum SHADER_TYPE
{
	SHADER_TYPE_VERTEX_SHADER	= 0,
	SHADER_TYPE_PIXEL_SHADER	= 1
};

/*
#define ALPHA_TEX_OPASITY		0x10000000

#define	ALPHA_TEX_TRANSP		0x01000000
#define	ALPHA_TEX_TRANSP_FILTER	0x01000000
#define	ALPHA_TEX_TRANSP_SUB	0x03000000
#define	ALPHA_TEX_TRANSP_ADD	0x05000000
#define	ALPHA_TEX_TRANSP_MASK	0x0f000000
*/
enum TEX_FLAG
{

	TEX_FLAG_ALPHA_OPASITY			=	0x10000000,
	TEX_FLAG_ALPHA_TRANSP			=	0x01000000,
	TEX_FLAG_ALPHA_TRANSP_FILTER	=	0x02000000,
	TEX_FLAG_ALPHA_TRANSP_SUB		=	0x04000000,
	TEX_FLAG_ALPHA_TRANSP_ADD		=	0x08000000
	
};

#define	ALPHA_TEX_TRANSP_MASK	0x0f000000
#define	ALPHA_TEX_PROPERTY_MASK	0xff000000

#define	TEX_FORMAT_565			0
#define	TEX_FORMAT_4444			1


enum CREATE_FACEGROUP_TYPE
{
	CREATE_FACEGROUP_TYPE_STATIC = 0x00000001
};

enum TEX_COORD_OFFSET
{
	TEX1_COORD_OFFSET_ZERO = 0,
	TEX1_COORD_OFFSET_NODIFFUSE = 12,
	TEX2_COORD_OFFSET_NODIFFUSE = 20,
	TEX1_COORD_OFFSET_RHW_DIFFUSE = 20,
	
	TEX1_COORD_OFFSET_LMMESH = 12,
	TEX2_COORD_OFFSET_LMMESH = 20,
	
	
	TEX1_COORD_OFFSET_VLMESH = 24,
	TEX1_COORD_OFFSET_VLSMESH = 16,
	


	TEX1_COORD_OFFSET_BBOARD = 12

};

enum DIFFUSE_COORD_OFFSET
{
	DIFFUSE_COORD_OFFSET_ZERO = 0,
	DIFFUSE_COORD_OFFSET_HFIELD = 12,
	DIFFUSE_COORD_OFFSET_XYZ = 24,
	DIFFUSE_COORD_OFFSET_XYZW = 16,
	DIFFUSE_COORD_VLS_OFFSET = 12

};
enum NORMAL_COORD_OFFSET
{
	NORMAL_COORD_OFFSET_VLMESH = 12
};

#define	D3DFVF_VLVERTEX	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1  )
#define D3DFVF_PLVERTEX  ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_XYZ )
#define D3DFVF_BBOARDVERTEX ( D3DFVF_XYZ | D3DFVF_TEX1)


#define D3DFVF_VLSVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define D3DFVF_HFIELDVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define D3DFVF_IVERTEX	(D3DFVF_XYZ | D3DFVF_TEX1)




#define D3DFVF_D3DLVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )


#define D3DFVF_D3DDUALVERTEX ( D3DFVF_XYZ | D3DFVF_TEX2)


#define D3DFVF_4DVERTEX	( D3DFVF_XYZRHW )


#define D3DFVF_D3DTLVERTEX		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
//#define D3DFVF_D3DTRIPLEVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX3 )
/*
struct D3DVLVERTEX 
{
	float		x;           
    float		y;
    float		z;

	float		nx;
	float		ny;
	float		nz;

	float		tu;          
    float		tv;
};
*/
struct D3DPLVERTEX 
{
    float		x;           
    float		y;
    float		z;

	float		nx;
	float		ny;
	float		nz;

	float		tu;          
    float		tv;
    
	float		tanu_x;
	float		tanu_y;
	float		tanu_z;
};

typedef D3DPLVERTEX D3DVLVERTEX;
struct D3DVLSVERTEX 
{
	float		x;           
    float		y;
    float		z;

	DWORD		color;

	float		tu;          
    float		tv;
};


struct D3DLVERTEX 
{
    float     x;             
    float     y;
    float     z;

    DWORD     color;         
};

struct D3DTLVERTEX
{
	float		x;
	float		y;
	float		z;
	float		w;

	DWORD		color;
	
	float		tu;
	float		tv;
};

struct D3D4DVERTEX
{
	float		x;
	float		y;
	float		z;
	float		w;
};



struct D3DDUALVERTEX
{
	float		x;
	float		y;
	float		z;
	
	float		tu1;
    float		tv1;    
	
	float		tu2;
    float		tv2;
};
/*
struct D3DTRIPLEVERTEX
{
	float		x;
	float		y;
	float		z;
	
	DWORD		color;
	
	float		tu1;
    float		tv1;    
	
	float		tu2;
    float		tv2;

	float		tu3;
	float		tv3;
};
*/
struct HFIELDVERTEX
{
	float		x;
	float		y;
	float		z;
	DWORD		dwColor;
};


struct D3DBBOARDVERTEX
{
	float		x;
	float		y;
	float		z;
	float		tu1;
	float		tv2;
};

struct ARGB
{
	BYTE		a;
	BYTE		r;
	BYTE		g;
	BYTE		b;
};

struct VS_CONSTANT_LIGHT
{
	VECTOR3		v3Pos;
	float		fRsPow;
};

enum DIMATERIAL_PROPERTY_TYPE
{
	DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE		=	0x00000001,
	DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN	=	0x00000002,
	DIMATERIAL_PROPERTY_TYPE_TEX_OPASITY	=	0x00000004,

	DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP			=	0x00000008,
	DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_FILTER	=	0x00000010,
	DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_SUB		=	0x00000020,
	DIMATERIAL_PROPERTY_TYPE_TEX_TRANSP_ADD		=	0x00000040,
};





struct INDEX_LIST_DESC
{
	DWORD		dwStartIndex;
	DWORD		dwIndicesNum;
};

enum DYNAMIC_LIGHT_TYPE
{
	DYNAMIC_LIGHT_TYPE_POINTLIGHT	 = 0,
	DYNAMIC_LIGHT_TYPE_IMAGE		 = 1
};

struct PRJLIGHT_DESC
{
	VECTOR3				v3From;
	VECTOR3				v3To;
	VECTOR3				v3Up;
	float				fFov;
	float				fNear;
	float				fFar;
	float				fWidth;
	BOOL				bOrtho;
	
	
	BOUNDING_SPHERE		sphereSenderShadow;

	MATRIX4		matPrjTex;
	MATRIX4		matPrjView;
	MATRIX4		matPrjProj;
	
};

class CRenderTexture;
struct DIMATERIAL;

struct SPOT_LIGHT_DESC
{
	SPOT_LIGHT_TYPE			type;	
	CRenderTexture*			pRenderTexture;
	BOOL					bUseRenderTexture;
	DIMATERIAL*				pMtl;
	DWORD					dwColorOP;

	PRJLIGHT_DESC			prjDesc;

};

interface IResourceController
{
	virtual BOOL				__stdcall	IsIncludeViewVolume() = 0;
	virtual BOOL				__stdcall	OnFault() = 0;
	virtual BOOL				__stdcall	OnOut() = 0;
};

struct SHADER_HANDLE
{
	DWORD		dwShader;
	DWORD*		pdwCodeMemAddr;
	DWORD		dwRefCount;
	SHADER_TYPE type;
public:
	SHADER_HANDLE()
	{
		dwShader = 0xffffffff;
		pdwCodeMemAddr = NULL;
		dwRefCount = 0;
	}

};