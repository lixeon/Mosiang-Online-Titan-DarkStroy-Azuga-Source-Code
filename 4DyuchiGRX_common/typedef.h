#pragma once

#include "stdafx.h"
#include "math.inl"
#include "mesh_flag.h"


#define VECTOR3_SIZE	12
#define VECTOR4_SIZE	16

#define MIN_UNIT						10.0f
#define	ONE_CM							1.0f
#define MAX_NAME_LEN					128

#define DEFAULT_LIGHT_RADIUS			100.0f
#define DEFAULT_RENDER_ZORDER_UNIT		-1000.0f

#define WORLDMAP_DEFAULT_TOP			400.0f
#define WORLDMAP_DEFAULT_BOTTOM			0.0f

#define FOG_DISTANCE_START				2000.0f
#define FOG_DISTANCE_END				8000.0f

// EXECUTIVE CONST VALUE
#define	DEFAULT_DECAL_TRI_NUM			64

#define	MAX_MODEL_NUM_PER_GXOBJECT		8
#define MAX_RENDER_OBJECT_NUM			8192
#define MAX_ATTATCH_OBJECTS_NUM			8
#define	MAX_PICK_OBJECT_NUM				256
#define ZORDER_LATEST_RENDER			1100


// GEOMETRY CONST VALUE
#define PID_MODEL_LOD					"_LOD"
#define PID_MODEL_lod					"_lod"
#define PID_LEN							4


//trustpak
//#define DEFAULT_MODEL_LOD_UNIT			500.0
#define DEFAULT_MODEL_LOD_UNIT			1000.0
//
#define MAX_LOD_MODEL_NUM				3
#define MAX_HFINDEX_BUFER_NUM			128
#define MAX_OBJECTS_NUM_PER_MODEL		8192
#define	MAX_STATIC_RTLIGHT_NUM_PER_SCENE	8
#define MAX_DYNAMIC_RTLIGHT_NUM_PER_SCENE	8

//#define MAX_SPOT_LIGHT_NUM_PER_SCENE		16
#define MAX_SHADOW_LIGHT_NUM_PER_SCENE		64
#define MAX_PRJIMAGE_LIGHT_NUM_PER_SCENE	64
#define MAX_SPOT_LIGHT_NUM_PER_SCENE	(MAX_SHADOW_LIGHT_NUM_PER_SCENE + MAX_PRJIMAGE_LIGHT_NUM_PER_SCENE)
#define MAX_REALTIME_LIGHT_NUM			(MAX_STATIC_RTLIGHT_NUM_PER_SCENE + MAX_DYNAMIC_RTLIGHT_NUM_PER_SCENE)


#define STATIC_RTLIGHT_START_INDEX		MAX_DYNAMIC_RTLIGHT_NUM_PER_SCENE
#define MAX_DIRECTIONAL_LIGHT_NUM		4
#define MAX_CAMERA_NUM_PER_SCENE		128
#define MAX_MOTION_NUM					1024
#define	DEFAULT_AMBIENT_COLOR			0xff202020
#define MAX_RENDER_MODEL_NUM			1024
#define MAX_RENDER_STATICMODEL_OBJ_NUM	4096
#define MAX_RENDER_HFIELD_OBJ_NUM		1024
#define	DEFAULT_NEAR					100.0f
#define DEFAULT_FAR						80000.0f
#define MAX_FILEITEM_NUM				8192
#define MAX_FILEBUCKET_NUM				256
#define MAX_VIEWPORT_NUM				256
#define MAX_MATRIX_NUM_IN_POOL			4096
#define MAX_LIGHT_INDEX_NUM_IN_POOL		8192
#define MAX_MODEL_REF_INDEX_NUM			8192
#define DEFAULT_RESOURCE_SCHDULE_DISTANCE	16.0f
#define	DEFAULT_OVERLAP_TILES_NUM		4

// RENDERER CONST VALUE
#define	DEFAULT_ALPHA_REF_VALUE			200
#define DEFAULT_RENDER_TEXTURE_NUM		4
#define MAX_RENDER_TEXTURE_NUM			8
#define DEFAULT_RENDER_TEXTURE_SIZE		256
#define MAX_RENDER_TEXTURE_SIZE			1024
#define MAX_SPRITE_ZORDER_NUM			256
#define MAX_HFIELD_DETAIL_NUM			8
#define MAX_TILE_TEXTURE_NUM			65536				
#define MAX_SPRITE_FRAME_NUM			1024
#define	MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM			256
#define	MAX_TEXTURE_NUM					25600
#define MAX_TEXBUCKET_NUM				256
#define MAX_RENDER_MESHOBJ_NUM			4096
#define MAX_RENDER_TEXTBUFFER_SIZE		16384
#define MAX_RENDER_TRIBUFFER_SIZE		8192
#define MAX_RENDER_SPRITE_NUM			1024
#define DEFULAT_CIRCLE_PIECES_NUM		32
#define MAX_CIRCLE_PIECES_NUM			64
#define MAX_MIPMAP_LEVEL_NUM			12
#define MAX_MIPMAP_SIZE					1
#define MAX_MATERIAL_NUM				8192
#define MAX_MATERIAL_SET_NUM			2048
#define	MAX_D3DRESOURCE_NUM				65536
#define	DEFAULT_D3DRESOURCE_NUM			128
#define DEFAULT_VBCACHE_NUM						1024
#define DEFAULT_PHYSIQUE_OBJECT_NUM_PER_SCENE	32
#define DEFAULT_MOST_PHYSIQUE_VERTEX_NUM		800
#define MAX_PHYSIQUE_VERTEX_NUM					32768
#define MAX_PRJMESH_INDICES_NUM					16384
#define MAXSHINESTR_TO_SS3D_VAL					20.0f
#define DEFAULT_FREE_VBCACHE_RATE				0.084f	
#define MAX_EFFECT_SHADER_NUM					1024	
#define DEFAULT_LIMITED_VERTEXTBUFFER_INDICES	65536
#define MAX_RESOURCE_POOL_NUM					32

enum FONT_TYPE
{
	SS3D_FONT	=	0,
	D3DX_FONT	=	1
};

enum RENDER_MODE
{
	RENDER_MODE_SOLID		=	0,
	RENDER_MODE_POINT		=	1,
	RENDER_MODE_WIREFRAME	=	2
};

enum APPLY_PHYSIQUE_TYPE
{
	APPLY_PHYSIQUE_RECALC_NORMAL				= 0x00000001,
	APPLY_PHYSIQUE_WRITE_UV						= 0x00000002
};

enum DEBUG_DRAW_FLAG
{
	DEBUG_DRAW_MODEL_COL_MESH		 = 0x00000001,
	DEBUG_DRAW_STATIC_MODEL_COL_MESH = 0x00000002,
	DEBUG_DRAW_BONE_COL_MESH		 = 0x00000004
};

enum LIGHT_FLAG
{
	DISABLE_LIGHT = 0,
	ENABLE_LIGHT = 1
};

enum SHADOW_FLAG
{
	DISABLE_SHADOW	= 0,
	ENABLE_SHADOW	= 1
};


enum BEGIN_RENDER_FLAG
{
	BEGIN_RENDER_FLAG_CLEAR_ZBUFFER				=	0x00000000,
	BEGIN_RENDER_FLAG_CLEAR_FRAMEBUFFER			=	0x00000000,
	BEGIN_RENDER_FLAG_DONOT_CLEAR_ZBUFFER		=	0x00000001,
	BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER	=	0x00000002,
	BEGIN_RENDER_FLAG_USE_RENDER_TEXTURE		=	0x00000004

};
enum MODEL_INITIALIZE_FLAG
{
	MODEL_INITIALIZE_FLAG_NOT_OPTIMIZE				= 0x00000001

};


// 맵을 로딩할때 사용할 수 있는 플래그들.or로 조합해서 사용한다.
enum MAP_LOAD_FLAG
{
	STATIC_MODEL_LOAD_OPTIMIZE				= 0x00000001,
	STATIC_MODEL_LOAD_DEFFER_COMMIT_DEVICE	= 0x00000002,
	STATIC_MODEL_LOAD_ENABLE_SHADE			= 0x00000004,
	HFIELD_MODEL_LOAD_ENABLE_SHADE			= 0x00000004,

	HFIELD_MODEL_LOAD_OPTIMIZE				= 0x00000008,
	HFIELD_MODEL_LOAD_NOT_RENDER			= 0x00000010,
	HFIELD_MODEL_LOAD_ENABLE_DRAW_ALPHAMAP	= 0x00000020
};
enum LOAD_MAP_FLAG
{
	LOAD_MAP_FLAG_DEFAULT_PROC_AUTOANIMATION	= 0x00000100
};

/////////////////////////////////////////////////////////////////

enum EXECUTIVE_RENDER_MODE
{
	RENDER_MODE_DEFAULT		=	0,
	RENDER_MODE_TOOL		=	1
};

//trustpak 2005/06/09
enum EXECUTIVE_PICKING_MODE
{
	PICKING_MODE_DEFAULT	= 0,
	PICKING_MODE_TOOL		= 1					// Picking 안되는 속성에 상관없이 picking되게 한다. 
};
///

//trustpak 2005/06/24

enum MODEL_LOD_USING_MODE
{
	MODEL_LOD_USING_MODE_DEFAULT	= 0,		// Model LOD 를 사용한다. 
	MODEL_LOD_USING_MODE_SET_LEVEL	= 1,		// 지정된 레벨만 사용되도록 한다. 
	MODEL_LOD_USING_MODE_NOT_USE	= 2,		// Model LOD 를 사용하지 않도록 한다. 
};

///


enum SYMBOL_TYPE
{
	SYMBOL_TYPE_LIGHT		=	0,
	SYMBOL_TYPE_TRIGGER		=	1
};
#define SYMBOL_TYPE_NUM	2

enum SCHEDULE_FLAG
{
	SCHEDULE_FLAG_NOT_SCHEDULE		=	0x00000001,
	SCHEDULE_FLAG_DISABLE_UNLOAD	=	0x00000002,
	SCHEDULE_FLAG_NOT_RENDER		=	0x10000000
};
/*
#define SCHEDULE_FLAG_RENDER_MASK			0xf0000000
#define SCHEDULE_FLAG_RENDER_MASK_INVERSE	0x0fffffff


#define SCHEDULE_FLAG_SCHEDULE_MASK			0x0000000f
#define SCHEDULE_FLAG_SCHEDULE_MASK_INVERSE	0xfffffff0
*/
enum UNLOAD_PRELOADED_RESOURCE_TYPE
{
	UNLOAD_PRELOADED_RESOURCE_TYPE_ONLY_UNLOAD_ENABLED	= 0x00000000,
	UNLOAD_PRELOADED_RESOURCE_TYPE_ALL_PRELOADED		= 0x00000001
};

enum CHAR_CODE_TYPE
{
	CHAR_CODE_TYPE_ASCII = 1,
	CHAR_CODE_TYPE_UNICODE = 2
};

enum STATIC_MODEL_SHADE_TYPE
{
	STATIC_MODEL_SHADE_TYPE_ENABLE_SHADOW	=	0x00000001
};

enum LIGHTMAP_FLAG
{
	LIGHTMAP_FLAG_DISABLE_LIGHTMAP		= 0x00000000,
	LIGHTMAP_FLAG_ENABLE				= 0x00000001,
	LIGHTMAP_FLAG_DISABLE_TEXTURE		= 0x00000002,
	LIGHTMAP_FLAG_DISABLE_MAGFILTER		= 0x00000004
};

enum OBJECT_TYPE
{
	OBJECT_TYPE_UNKNOWN = 0xf000000,
	OBJECT_TYPE_LIGHT = 0xf1000000,
	OBJECT_TYPE_CAMERA = 0xf2000000,
	OBJECT_TYPE_CAMERA_TARGET = 0xf3000000,
	OBJECT_TYPE_MESH = 0xf4000000,
	OBJECT_TYPE_BONE = 0xf5000000,
	OBJECT_TYPE_ILLUSION_MESH = 0xf6000000,
	OBJECT_TYPE_COLLISION_MESH = 0xf7000000,
	OBJECT_TYPE_MATERIAL = 0x00f00000,
	OBJECT_TYPE_MOTION = 0x0000f000
};

enum MODEL_READ_TYPE
{
	MODEL_READ_TYPE_AS_LOD_SUBMODEL	= 0x00000001
};
enum GX_MAP_OBJECT_TYPE
{
	GX_MAP_OBJECT_TYPE_OBJECT	=	0,
	GX_MAP_OBJECT_TYPE_STRUCT	=	1,
	GX_MAP_OBJECT_TYPE_HFIELD	=	2,
	GX_MAP_OBJECT_TYPE_DECAL	=	3,
	GX_MAP_OBJECT_TYPE_TRIGGER	=	4,
	GX_MAP_OBJECT_TYPE_LIGHT	=	5,

	GX_MAP_OBJECT_TYPE_INVALID	=	0xffffffff
};
#define GX_MAP_OBJECT_RENDER_START_INDEX	0
#define GX_MAP_OBJECT_RENDER_END_INDEX		3
#define GX_MAP_OBJECT_RENDER_START_INDEX_TOOL	4
#define GX_MAP_OBJECT_RENDER_END_INDEX_TOOL		5

#define GX_MAP_OBJECT_TYPE_NUM		6

enum GXLIGHT_ATTACH_TYPE
{
	ATTACH_TYPE_ATTACH = 0x00000000,
	ATTACH_TYPE_LINK = 0x00000001
};
#define GXLIGHT_ATTACH_TYPE_MASK_			0x0000000f
#define GXLIGHT_ATTACH_TYPE_MASK_INVERSE	0xfffffff0

enum MESH_CONTROL_TYPE
{
	RESULT_MATRIX_ALIGN_VIEW	= 0x00000001,
	UPDATE_VERTEX_NORMAL		= 0x00000002
};

enum MATERIAL_ILLUNUM_TYPE
{
	SELF_ILLUNUM			= 0x10000000
};

#define MATERIAL_ILLUNUM_TYPE_MASK	0x10000000
#define	MATERIAL_ILLUNUM_TYPE_MASK_INVERSE	0x8fffffff
enum MATERIAL_TRANSP_TYPE
{
	TRANSP_TYPE_FILTER		=	0x00000001,
	TRANSP_TYPE_SUBTRACTIVE	=	0x00000002,
	TRANSP_TYPE_ADDITIVE	=	0x00000004

};
#define MATERIAL_TRANSP_TYPE_MASK			0x00000003
#define MATERIAL_TRANSP_TYPE_MASK_INVERSE	0xfffffffc

enum HFIELD_ADJUST_TYPE
{
	HFIELD_ADJUST_TYPE_SET	= 0x00000001,
	HFIELD_ADJUST_TYPE_ADD	= 0x00000002,
	HFIELD_ADJUST_TYPE_SUB	= 0x00000004
};
enum VERTEX_TYPE
{
	VERTEX_TYPE_TEXTURE = 0x00000001,
	VERTEX_TYPE_PHYSIQUE = 0x01000000,
	VERTEX_TYPE_NORMAL = 0x00010000
};
enum TEXTURE_TYPE
{
	TEXTURE_TYPE_DEFAULT			= 0x00000000,
	TEXTURE_TYPE_DETAIL				= 0x00000001
};

enum RENDER_TEXTURE_TYPE
{
	RENDER_TEXTURE_TYPE_SHADOW		= 0,
	RENDER_TEXTURE_TYPE_REFLECT		= 1
};


enum CREATE_MATERIAL_TYPE
{
	// 최상위 8비트는 보더컬러 
	CREATE_MATERIAL_TYPE_TEXBORDER	= 0x00000001

};
enum TEXTURE_MAP_TYPE
{
	TEXTURE_MAP_TYPE_AMBIENT		= 0,
	TEXTURE_MAP_TYPE_DIFFUSE		= 1,
	TEXTURE_MAP_TYPE_SPECULAR		= 2,
	TEXTURE_MAP_TYPE_SHINE			= 3,
	TEXTURE_MAP_TYPE_SHINESTRENGTH	= 4,
	TEXTURE_MAP_TYPE_SELFILLUM		= 5,
	TEXTURE_MAP_TYPE_OPACITY		= 6,
	TEXTURE_MAP_TYPE_FILTERCOLOR	= 7,
	TEXTURE_MAP_TYPE_BUMP			= 8,
	TEXTURE_MAP_TYPE_REFLECT		= 9
};
#define TEXTURE_MAP_TYPE_MAX_INDEX	9
#define	TEXTURE_MAP_TYPE_NUM	10

enum REDNER_TYPE
{
	RENDER_TYPE_ENABLE_LIGHT_ANIMATION		=	0x00000001,
	RENDER_TYPE_ENABLE_CAMERA_ANIMATION		=	0x00000002,
	RENDER_TYPE_ENABLE_MODEL_LIGHT			=	0x00000004,
	RENDER_TYPE_ENABLE_CLIP_PER_OBJECT		=	0x00000008,
	RENDER_TYPE_AS_LOD_SUBMODEL				=	0x00000010,
	RENDER_TYPE_UPDATE_SHADING				=	0x00000020,
	RENDER_TYPE_NOT_DRAW					=	0x00000040,
	RENDER_TYPE_SELF_ILLUMIN				=	0x00000080,
	RENDER_TYPE_IGNORE_VIEWVOLUME_CLIP		=	0x00000100,
	RENDER_TYPE_SEND_SHADOW					=	0x00000200,
	RENDER_TYPE_RECV_SHADOW					=	0x00000400,
	RENDER_TYPE_DISABLE_ZCLIP				=	0x00000800,
	RENDER_TYPE_DISABLE_TEX_FILTERING		=	0x00001000,
	RENDER_TYPE_USE_PROJECTIONLIGHT			=	0x00002000,
	RENDER_TYPE_ENABLE_ILLUSION				=	0x00004000,
	RENDER_TYPE_SPRITE_ADD					=	0x00008000,
	RENDER_TYPE_SPRITE_MUL					=	0x00010000,
	RENDER_TYPE_SPRITE_OPASITY				=	0x00020000,
	RENDER_TYPE_USE_EFFECT					=	0x00040000,

	// setframe에서 사용할 플래그.최상위 4비트만 사용한다
	RENDER_TYPE_UPDATE_COLLISION_BONEMESH_DESC		= 0x10000000,
	RENDER_TYPE_UPDATE_ILLUSION_FRAME				= 0x20000000,
	RENDER_TYPE_UPDATE_ALWAYS						= 0X40000000

};
#define RENDER_TYPE_UPDATE_MASK	0xf0000000

//#define RENDER_TYPE_UPDATE_SHADING_MASK_INVERSE	0xfffffeff
//#define RENDER_TYPE_SEND_SHADOW_MASK_INVERSE	0xffffbfff
//#define RENDER_TYPE_SELF_ILLUMIN_MASK_INVERSE	0xfffff7ff

enum SHADOW_TYPE
{
	ENABLE_PROJECTION_SHADOW		=	0x00000001,
	ENABLE_PROJECTION_TEXMAP		=	0x00000002
};
enum PICK_TYPE
{
	PICK_TYPE_DEFAULT				= 0x00000000,
	PICK_TYPE_PER_COLLISION_MESH	= 0x00000001,
	PICK_TYPE_PER_FACE				= 0x00000002,
	PICK_TYPE_PER_BONE_OBJECT		= 0x00000004,

	PICK_TYPE_SORT					= 0x00000010
};

#define PICK_TYPE_PURE_MASK			= 0x0000000f

enum LIGHT_TEXTURE_CREATE_TYPE
{
	LIGHT_TEXTURE_CREATE_BORDER				= 0x00000001,
	LIGHT_TEXTURE_CREATE_PACK_FILE			= 0x00000010
};

// GXMapObject 속성 플래그 ///////////////////////////////////////////////////////////
// 최상위 8비트는 GXMAP_OBJECT_COMMON_FLAG 영역이므로 사용하지 않는다.
enum GXMAP_OBJECT_COMMON_FLAG
{
	GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE		= 0x01000000,
	GXMAP_OBJECT_COMMON_TYPE_NOT_USE_CLIPPER	= 0x02000000,
	GXMAP_OBJECT_COMMON_TYPE_DISABLE_UNLOAD		= 0x04000000,
	GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM		= 0x08000000


};
#define GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE_INVERSE		0xfeffffff
#define GXMAP_OBJECT_COMMON_TYPE_NOT_USE_CLIPPER_INVERSE	0xfdffffff

enum GXOBJECT_CREATE_TYPE
{
	// 최상위 8비트는 GXMAP_OBJECT_COMMON_FLAG 영역이므로 사용하지 않는다.
	GXOBJECT_CREATE_TYPE_OPTIMIZE		= 0x00000001,
	GXOBJECT_CREATE_TYPE_NOT_OPTIMIZE	= 0x00000000,
	GXOBJECT_CREATE_TYPE_EFFECT			= 0x00000002,
	GXOBJECT_CREATE_TYPE_APPLY_HFIELD	= 0x00000004,
	GXOBJECT_CREATE_TYPE_DEFAULT_PROC	= 0x00000008,
	GXOBJECT_CREATE_TYPE_NOT_USE_MODEL	= 0x00000010
};
#define GXOBJECT_CREATE_TYPE_APPLY_HFIELD_INVERSE	0xfffffffb 
#define GXOBJECT_CREATE_TYPE_EFFECT_INVERSE			0xfffffffd
enum GXLIGHT_TYPE
{
	// 최상위 8비트는 GXMAP_OBJECT_COMMON_FLAG 영역이므로 사용하지 않는다.
	GXLIGHT_TYPE_TEX_PROJECTION						= 0x00000000,	// 이 경우 최하위 8비트가 라이트 인덱스 
	GXLIGHT_TYPE_ENABLE_SHADOW						= 0x00000200,
	GXLIGHT_TYPE_ENABLE_SPOT						= 0x00000400,
	GXLIGHT_TYPE_DISABLE_LIGHT_COLOR				= 0x00000800,
	GXLIGHT_TYPE_STATIC								= 0x00001000,
	GXLIGHT_TYPE_STATIC_SHADOW_DISABLE				= 0x00002000,
	GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT				= 0x00004000,
	GXLIGHT_TYPE_ENABLE_IMAGE_PROJECTION			= 0x00008000,
	GXLIGHT_TYPE_DISABLE_NOT_RENDER_MODEL_IN_TOOL	= 0x00010000,
	GXLIGHT_TYPE_ONLY_USE_TOOL						= 0x00020000
};
#define GXLIGHT_TYPE_STATIC_SHADOW_DISABLE_INVERSE	0xffffdfff
#define GXLIGHT_TYPE_ENABLE_DYNAMIC_LIGHT_INVERSE	0xffffbfff

enum GXTRIGGER_TYPE
{
	// 최상위 8비트는 GXMAP_OBJECT_COMMON_FLAG 영역이므로 사용하지 않는다.
	GXTRIGGER_TYPE_MOVABLE				= 0x00000001

};
#define GX_LIGHT_TYPE_TEX_INDEX_0		= 0x00000000
//////////////////////////////////////////////////////////////////////////////////////////////

enum SPRITE_CREATE_FLAG
{
	SPRITE_CREATE_DEFAULT = 0x00000000,
	SPRITE_CREATE_IMAGE = 0x00000001
};
enum TEXTURE_CREATE_FLAG
{
	TEXTURE_CREATE_RENDER_DEFAULT	= 0x00000000,
	TEXTURE_CREATE_RENDER_TARGET	= 0x00000001,
	TEXTURE_CREATE_SYSTEM_MAMANGED	= 0x00000000,
	TEXTURE_CREATE_SYSTEM_MEMEORY	= 0x00000100,
	TEXTURE_CREATE_SYSTEM_VIDEO		= 0x00000200,
};


enum MATERIAL_TYPE
{
	MATERIAL_TYPE_2SIDE		= 0x00000100

};
enum HEIGHT_FIELD_UPDATE_TYPE
{
	HEIGHT_FIELD_UPDATE_TYPE_TEXTURE = 0x00000000,
	HEIGHT_FIELD_UPDATE_TYPE_VERTEX_POS = 0x00000001
};

enum EDIT_ALPHA_TEXEL_TYPE
{
	EDIT_ALPHA_TEXEL_OP_ADD		=	0x01000000,
	EDIT_ALPHA_TEXEL_OP_SUB		=	0x02000000,
	EDIT_ALPHA_TEXEL_BRIGHT_0	=	0x00000000,
	EDIT_ALPHA_TEXEL_BRIGHT_1	=	0x00000001,
	EDIT_ALPHA_TEXEL_BRIGHT_2	=	0x00000002,
	EDIT_ALPHA_TEXEL_BRIGHT_3	=	0x00000004
};
#define EDIT_ALPHA_TEXEL_OP_MASK		0xff000000
#define EDIT_ALPHA_TEXEL_BRIGHT_MASK	0x00000003

enum POSITION_STATE
{
	UP		= 0,
	LEFT	= 1,
	DOWN	= 2,
	RIGHT	= 3
};
#define	MATERIAL_TYPE_2SIDE_MASK	0x0000000f

#ifndef _MAX_EXP
#define PI			3.14159265358979323846f // Pi
#define PI_MUL_2	6.28318530717958623200f // 2 * Pi
#define PI_DIV_2	1.57079632679489655800f // Pi / 2
#define PI_DIV_4	0.78539816339744827900f // Pi / 4
#define INV_PI		0.31830988618379069122f // 1 / Pi
#define EPSILON		1.0e-5f;                 // Tolerance for FLOATs
#define	DEFAULT_FOV					PI/4.0f
#endif

#define R_565_MASK	0x0000F800
#define G_565_MASK	0x000007E0
#define B_565_MASK	0x0000001F

struct DWORD_RECT
{
	DWORD		left;
	DWORD		top;
	DWORD		right;
	DWORD		bottom;
};
struct FLOAT_RECT
{
	float	fLeft;
	float	fTop;
	float	fRight;
	float	fBottom;
};



struct COLORVALUE
{
	float	r;
	float	g;
	float	b;
	float	a;
};
struct TVERTEX
{
	float u;
	float v;
};

struct TPVERTEX
{
	float	x;
	float	y;
	float	z;
	float	tu;
	float	tv;
};

struct NODE_TM
{
	float		fRotAng;
	float		fPosX,fPosY,fPosZ;
	float		fRotAxisX,fRotAxisY,fRotAxisZ;
	float		fScaleX,fScaleY,fScaleZ;
	float		fScaleAxisX,fScaleAxisY,fScaleAxisZ;
	float		fScaleAngle;
	MATRIX4		mat4;
	MATRIX4		mat4Inverse;
};

enum SPOT_LIGHT_TYPE
{
	SPOT_LIGHT_TYPE_PRJIMAGE	= 0,
	SPOT_LIGHT_TYPE_SHADOW		= 1,
	SPOT_LIGHT_TYPE_MIRROR		= 2
};

struct LIGHT_DESC
{
	DWORD			dwAmbient;
	DWORD			dwDiffuse;
	DWORD			dwSpecular;

	VECTOR3			v3Point;
	float			fRs;

	VECTOR3			v3To;
	VECTOR3			v3Up;
	DWORD			dwProjTexIndex;
	float			fFov;

	float			fNear;
	float			fFar;
	float			fWidth;
	float			fHeight;
	void*			pMtlHandle;
	DWORD			dwFlag;
	SPOT_LIGHT_TYPE	type;
};
//#define LIGHT_DESC_SIZE	(4 + VECTOR3_SIZE + 4 + VECTOR3_SIZE + VECTOR3_SIZE + 4 + 4 + 4 + 4);
//#define LIGHT_DESC_RS_OFFSET	4+VECTOR3_SIZE
struct MIRROR_DESC
{
	VECTOR3			v3Point;
	float			fRs;

	VECTOR3			v3To;
	VECTOR3			v3Up;
	float			fFov;

	float			fNear;
	float			fFar;
	float			fWidth;
	float			fHeight;

	DWORD			dwFlag;
};


struct DIRECTIONAL_LIGHT_DESC
{
	DWORD			dwAmbient;
	DWORD			dwDiffuse;
	DWORD			dwSpecular;
	VECTOR3			v3Dir;
	BOOL			bEnable;
};

struct LIGHT_INDEX_DESC
{

	void*		pMtlHandle;
	BYTE		bLightIndex;
	BYTE		bTexOP;
	BYTE		bReserved2;
	BYTE		bReserved3;

};
struct INDEX_BUFFER_DESC
{
	DWORD		dwIndicesNum;
	WORD*		pIndex;
};
struct INDEX_POS
{
	DWORD		dwX;
	DWORD		dwY;
};


struct PLANE
{
	VECTOR3		v3Up;
	float		D;
};
#define PLANE_D_OFFSET	VECTOR3_SIZE


struct PATCH;
struct TEXTURE_PLANE
{
	VECTOR3			v3Normal;
	VECTOR3			v3Rect[4];
	DWORD			dwEntryPosX;
	DWORD			dwEntryPosY;
	PLANE			plane;
	DWORD			dwReserved;
	DWORD			dwPatchNum;
	PATCH*			pPatch;
	DWORD			dwTexelSize;
};
class CBaseObject;
class CMeshObject;

#pragma pack(push,1)

struct TGA_HEADER
{
	char idLength;
	char ColorMapType;
	char ImageType;
	WORD ColorMapFirst;
	WORD ColorMapLast;
	char ColorMapBits;
	WORD FirstX;
	WORD FirstY;
	WORD width;
	WORD height;
	char Bits;
	char Descriptor;
};
#pragma pack(pop)


#ifdef _MAX_EXP
#define	MAXEXP_MAX_MATERIAL_NUM		65536		
struct FACE
{
	WORD		a;
	WORD		b;
	WORD		c;
	WORD		mtlid;
	VECTOR3		v3Normal[3];
};
struct TFACE
{
	WORD		a;
	WORD		b;
	WORD		c;
	WORD		flag;
};

#define	MAX_PHYSIQUE_OBJECT_NUM		512

struct POINT3D_REF_TVERTEX
{
	VECTOR3*				pv3Point;
	TVERTEX*				ptv;
	POINT3D_REF_TVERTEX*	pNext;
	WORD					wIndex;

};
struct PATCH
{
	VECTOR3			v3Point;
	VECTOR3			v3Normal;
	float			fDistance;
	DWORD			dwColor;
	DWORD			sx;
	DWORD			sy;
	TEXTURE_PLANE*	pTexPlane;
};


#else

struct PATCH
{
	DWORD		dwColor;
	VECTOR3		v3Point;
	WORD		sx;
	WORD		sy;
	DWORD		dwLightColor[4];
};

#define		PATCH_SX_OFFSET				4+VECTOR3_SIZE
#define		PATCH_SY_OFFSET				4+VECTOR3_SIZE+2
#define		PATCH_LIGHT_COLOR_OFFSET	4+VECTOR3_SIZE+2+2
#define		PATCH_SIZE					36				


#endif

#pragma pack(push,1)


struct BONE_OLD
{
	void*			pBone;
	float			fWeight;
	VECTOR3			v3Offset;
	VECTOR3			v3NormalOffset;
};

struct BONE
{
	void*			pBone;
	float			fWeight;
	VECTOR3			v3Offset;
	VECTOR3			v3NormalOffset;
	VECTOR3			v3TangentOffset;
};




struct PHYSIQUE_VERTEX
{
	BYTE			bBonesNum;
	BONE*			pBoneList;
};
#pragma pack(pop)


#define BONE_OLD_SIZE			32
#define BONE_SIZE				32+12
#define	PHYSIQUE_VERTEX_SIZE	5


#define POS_OFFSET_IN_BONE		8
#define NORMAL_OFFSET_IN_BONE	20
#define TANGENT_OFFSET_IN_BONE	32



struct COLLISION_FACE
{
	VECTOR3		v3Point[3];
	VECTOR3		v3Up;
	float		D;

};
struct MATERIAL 
{	

	DWORD			dwTextureNum;
	DWORD			dwDiffuse;
	DWORD			dwAmbient;
	DWORD			dwSpecular;
	float			fTransparency;
	float			fShine;
	float			fShineStrength;
	char			szDiffuseTexmapFileName[MAX_NAME_LEN];	
	char			szReflectTexmapFileName[MAX_NAME_LEN];	
	char			szBumpTexmapFileName[MAX_NAME_LEN];

	DWORD			dwFlag;

	DWORD			GetFlag()	{return dwFlag;}
	char*			GetDiffuseTexmapName() {return szDiffuseTexmapFileName;}
	char*			GetReflectTexmapName() {return szReflectTexmapFileName;}
	char*			GetBumpTexmapName() {return szBumpTexmapFileName;}

	DWORD			GetDiffuse() {return dwDiffuse;}
	DWORD			GetAmbient() {return dwAmbient;}
	DWORD			GetSpecular() {return dwSpecular;}
	float			GetTransparency() {return fTransparency;}
	float			GetShine() {return fShine;}
	float			GetShineStrength() {return fShineStrength;}
};

struct MATERIAL_TABLE
{
	MATERIAL*		pMtl;
	DWORD			dwMtlIndex;
};

struct LIGHT_TEXTURE
{
	TEXTURE_PLANE*		pTexPlane;
	DWORD				dwTexPlaneNum;
	DWORD				dwSurfaceWidth;
	DWORD				dwSurfaceHeight;
	DWORD				dwBPS;
	char*				pBits;

};

struct VIEW_VOLUME
{
	VECTOR3		From;		// 카메라의 위치.
	PLANE		Plane[4];	// 순서대로 왼쪽, 오른쪽, 아래, 위.

	float		fFar;		// 시점으로부터 가장 먼곳의 거리.
	BOOL		bIsOrtho;

	float		fWidth;

	//YH
	VECTOR3		Points[4];
	// 이아래부턴 없어도 될려나.?
	//	VECTOR3		To;			// 이건 필요있는지 모르겠군.
	//	float		fFov;		// 카메라 좌우의 fov.
	//	float		fHVRatio;	// 가로, 세로 비율, 일반적으로 1.3333정도겠지.

	//	VECTOR3		v3SenderPosition;		// 프로젝션 쉐도우에서 수신자 위치.
};

struct MESH_DESC
{
	DWORD					dwVertexNum;
	VECTOR3*				pv3WorldList;
	VECTOR3*				pv3LocalList;
	DWORD					dwTexVertexNum;
	TVERTEX*				ptvTexCoordList;

	MATRIX4*				pMatrixWorldInverse;

	DWORD					dwFaceGroupNum;

	LIGHT_TEXTURE			LightTexture;
	DWORD*					pVertexColor;
	VECTOR3*				pv3NormalLocal;
	VECTOR3*				pv3TangentULocal;
	CMeshFlag				meshFlag;
};

struct CAMERA_DESC
{
	VECTOR3			v3From;
	VECTOR3			v3To;
	VECTOR3			v3Up;			// 카메라의 up.
	VECTOR3			v3EyeDir;
	float			fXRot;
	float			fYRot;
	float			fZRot;
	float			fFovX;
	float			fFovY;
	float			fAspect;
	float			fNear;
	float			fFar;

};


class CMaterial;

struct FACE_DESC
{
	WORD*			pIndex;				// face index 배열 포인터 
	DWORD			dwFacesNum;			// face갯수
	DWORD			dwMtlIndex;			// 매터리얼 인덱스
	MATERIAL*		pMaterial;			// material 
	TVERTEX*		ptUVLight1;			// 라이트 텍스쳐용 uv좌표.면순서와 1:1 대응 
	TVERTEX*		ptUVLight2;			// 라이트 텍스쳐용 uv좌표.면순서와 1:1 대응.스태틱 라이트맵.
	DWORD*			pdwMtlIndex;		// 매터리얼 인덱스 
};



struct IVERTEX
{
	float			x;
	float			y;
	float			z;

	float			u1;
	float			v1;
};


struct MOTION_DESC
{
	DWORD					dwTicksPerFrame;
	DWORD					dwFirstFrame;
	DWORD					dwLastFrame;
	DWORD					dwFrameSpeed;
	DWORD					dwKeyFrameStep;
	char					szMotionName[MAX_NAME_LEN];
};


struct MODEL_STATUS
{
	DWORD		dwFrame;
	DWORD		dwMotionIndex;
	DWORD		dwLODLevel;
	float		fDistanceFromView;
	float		fDistanceFactor;
	void*		pMotionObject;
	BOOL		bAxisAlignOK;
};
struct MOTION_STATUS
{
	DWORD		dwFrame;
	DWORD		dwMotionIndex;
	void*		pMotionUID;
};

struct BOUNDING_BOX
{
	VECTOR3			v3Oct[8];
};

struct BOUNDING_SPHERE
{
	VECTOR3			v3Point;
	float			fRs;
};

struct BOUNDING_CYLINDER
{
	VECTOR3			v3Point;
	float			fRs;
	float			fAy;
};

struct BOUNDING_CAPSULE
{
	VECTOR3			v3From;
	VECTOR3			v3To;
	float			fRadius;
};

struct COLLISION_MESH_OBJECT_DESC_SAVELOAD
{
	BOUNDING_BOX		boundingBox;
	BOUNDING_SPHERE		boundingSphere;
	BOUNDING_CYLINDER	boundingCylinder;
	DWORD				dwObjIndex;
	char				szObjName[MAX_NAME_LEN];
};

struct COLLISION_MESH_OBJECT_DESC
{
	BOUNDING_BOX		boundingBox;
	BOUNDING_SPHERE		boundingSphere;
	BOUNDING_CYLINDER	boundingCylinder;
	DWORD				dwObjIndex;
};


struct COLLISION_MODEL_DESC
{
	COLLISION_MESH_OBJECT_DESC	colMeshModel;
	DWORD						dwColMeshObjectDescNum;
	COLLISION_MESH_OBJECT_DESC	colMeshObjectDesc[1];
};
struct OBJ_REF_DESC
{
	COLLISION_MESH_OBJECT_DESC	colMeshDescLocal;
	COLLISION_MESH_OBJECT_DESC	colMeshDescWorld;
	MATRIX4						matResult;
	MODEL_STATUS				modelStatus;
};

struct IMAGE_HEADER
{
	DWORD				dwWidth;
	DWORD				dwHeight;
	DWORD				dwPitch;
	DWORD				dwBPS;
};
struct IMAGE_DESC
{
	IMAGE_HEADER	imgHeader;
	char*			pBits;
};

struct TEXTURE_TABLE
{
	WORD		wIndex;
	char		szTextureName[MAX_NAME_LEN];

};
struct PACKFILE_NAME_TABLE
{
	char	szFileName[_MAX_PATH];
	DWORD	dwFlag;
};
struct HFIELD_CREATE_DESC
{
	float			left;
	float			top;
	float			right;
	float			bottom;
	float			fFaceSize;
	DWORD			dwFacesNumPerObjAxis;
	DWORD			dwObjNumX;
	DWORD			dwObjNumZ;
	DWORD			dwDetailLevelNum;
	DWORD			dwIndexBufferNumLV0;
	TEXTURE_TABLE*	pTexTable;
	DWORD			dwTileTextureNum;


};

struct HFIELD_DESC
{
	float			left;
	float			top;
	float			right;
	float			bottom;
	float			fFaceSize;

	DWORD			dwFacesNumPerObjAxis;
	DWORD			dwObjNumX;
	DWORD			dwObjNumZ;
	BYTE			bDetailLevelNum;
	BYTE			bReserved0;
	BYTE			bReserved1;
	BYTE			bBlendEnable;
	DWORD			dwIndexBufferNumLV0;

	TEXTURE_TABLE*	pTexTable;
	DWORD			dwTileTextureNum;


	float*			pyfList;
	DWORD			dwYFNumX;
	DWORD			dwYFNumZ;
	float			width;
	float			height;
	DWORD			dwFacesNumX;
	DWORD			dwFacesNumZ;
	DWORD			dwTriNumPerObj;
	DWORD			dwVerticesNumPerObj;

	WORD*			pwTileTable;
	DWORD			dwFacesNumPerTileAxis;
	DWORD			dwTileNumPerObjAxis;
	DWORD			dwTileNumX;
	DWORD			dwTileNumZ;
	float			fTileSize;
};

struct HFIELD_OBJECT_DESC
{
	DWORD			dwPosX;
	DWORD			dwPosZ;

	DWORD			dwObjPosX;
	DWORD			dwObjPosZ;


	//	IMAGE_DESC*		pTexImage;
	//	LIGHT_TEXTURE	LightTexture;

};
struct SYSTEM_STATUS
{
	DWORD		dwAvaliableTexMem;
	DWORD		dwTotalTexMem;
	char		szDeviceType[MAX_NAME_LEN];
};

struct TILE_ENTRY_DESC
{
	// 4가지 경우에 대한 인덱스리스트의 엔트리.
	WORD		wTilePosEntry;	
	WORD		wTilePosNum;
};

struct INDEX_ENTRY_DESC
{
	DWORD				dwStartIndex;
	DWORD				dwTriNum;	
};

struct ALPHAMAP_DESC
{
	char*				pAlphaMapBits;
	void*				pVoidExt;
	WORD				wWidthHeight;
	BYTE				bReserved0;
	BYTE				bReserved1;
};

struct TILE_BUFFER_DESC
{
private:
	WORD				wTileIndexIntegrated;
	BYTE				bReserved1;
	BYTE				bReserved2;
public:

	INDEX_ENTRY_DESC	indexEntryDesc[4];

	// 이 종류의 타일디스크립터에 속한 인덱스들의 집합.
	DWORD				dwTilePosNumPri;
	DWORD				dwTilePosNumExt;

	ALPHAMAP_DESC		alphaMapDesc;	

	INDEX_POS*			pTilePosPri;
	TILE_ENTRY_DESC		tileEntryDescPri[4];

	INDEX_POS*			pTilePosExt;
	TILE_ENTRY_DESC		tileEntryDescExt[4];

	WORD				GetIntegratedTileIndex() {return wTileIndexIntegrated;}
	WORD				GetTileIndex() {return wTileIndexIntegrated & 0x3fff;}
	DWORD				GetTileDir() {return (wTileIndexIntegrated & 0xc000)>>14;}
	void				SetTileIndex(WORD wTileIndex) 
	{
		wTileIndexIntegrated = wTileIndex;
	}
	WORD				IsEqual(WORD wTileIndex)
	{
		return (wTileIndex & 0x3fff) - (wTileIndexIntegrated & 0x3fff);

	}




};



enum DISPLAY_TYPE
{
	WINDOW_WITH_BLT			= 0x00000000,
	FULLSCREEN_WITH_BLT		= 0x00000001,
	FULLSCREEN_WITH_FLIP	= 0x10000001
};

#define FLIP_MASK			0x10000000
#define FULLSCREEN_MASK		0x00000001

struct DISPLAY_INFO
{
	DISPLAY_TYPE	dispType;
	DWORD			dwWidth;
	DWORD			dwHeight;
	DWORD			dwRefreshRate;
	DWORD			dwBPS;
};

struct TILE_TABLE_DESC
{
	float		fTop;
	float		fLeft;
	float		fBottom;
	float		fRight;

	DWORD		dwTileNumWidth;
	DWORD		dwTileNumHeight;
	DWORD		dwBytesOfTile;
	float		fTileWidth;
	float		fTileHeight;
};

interface I4DyuchiGXExecutive;

typedef void* GXOBJECT_HANDLE;
typedef void* GXMAP_HANDLE;
typedef void* GXLIGHT_HANDLE;
typedef void* GXMAP_OBJECT_HANDLE;
typedef void* GXTRIGGER_HANDLE;
typedef void* GXDECAL_HANDLE;
typedef void* GXSOUND_HANDLE;





typedef DWORD (__stdcall *GXSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
typedef DWORD (__stdcall *GXLightSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXLIGHT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
typedef DWORD (__stdcall *GXMapObjectSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXMAP_OBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
typedef DWORD (__stdcall *GXMapSchedulePROC)(DWORD msg,int frame_ince,int arg2,void* pData);
typedef DWORD (__stdcall *GXDecalSchedulePROC)(I4DyuchiGXExecutive* pExecutive,GXDECAL_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);

enum ERROR_TYPE
{
	ERROR_TYPE_ENGINE_CODE			=	0,
	ERROR_TYPE_PARAMETER_INVALID	=	1,
	ERROR_TYPE_DEVICE_NOT_SUPPROT	=	2,
	ERROR_TYPE_D3D_ERROR			=	3,
	ERROR_TYPE_RESOURCE_LEAK		=	4,
	ERROR_TYPE_FILE_NOT_FOUND		=	5
};
typedef DWORD (__stdcall *ErrorHandleProc)(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr);


#include "GXSCHEDULE_PROC_MSG.h"


interface I4DyuchiAudio;

typedef DWORD (*GXSoundPROC)(I4DyuchiAudio* pAudio,GXSOUND_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);

// 여기서부터 추가. 2002/01/25

struct MAABB		// AABB 바운딩 박스.
{
	VECTOR3		Max;
	VECTOR3		Min;
};


//	충돌처리용 바운딩 볼륨.
enum	BOUNDING_VOLUME_TYPE
{
	BOUNDING_VOLUME_TYPE_NONE,
	//	BOUNDING_VOLUME_TYPE_PARTICLE,
	BOUNDING_VOLUME_TYPE_SPHERE,
	BOUNDING_VOLUME_TYPE_AAELLIPSOID,
	BOUNDING_VOLUME_TYPE_AAELLIPSOID2
};

// 2002/05/25 추가.
// BOUNDING_VOLUE에서 쓰일 충돌 대상 플랙.
enum COLLISION_TARGET_FLAG
{
	COLLISION_TARGET_FLAG_TEST_NONE					=	0x00000000,
	COLLISION_TARGET_FLAG_TEST_STATIC_OBJECT		=	0x00000001,
	COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT		=	0x00000002,
	COLLISION_TARGET_FLAG_TEST_HEIGHT_FIELD			=	0x00000004,
	COLLISION_TARGET_FLAG_TEST_EVENT_TRIGGER		=	0x00000008
};

//	I3DModel에서 가지게 된다.
//	아직 하나의 MODEL 파일에 하나씩의 충돌 그룹을 갖는다...
struct	BOUNDING_VOLUME
{
	DWORD				dwType;						// 바운딩 볼륨의 타입BOUNDING_VOLUME_TYPE. 
	VECTOR3				vPivot;						// 로컬 피봇. v3Axis+vPivot이 물체의 월드좌표이다.
	float				fRadius;					// 바운딩 스피어.
	float				fHeight;
	DWORD				dwCollisionTargetFlag;		// COLLISION_TARGET_FLAG타입의 플랙.
};

#define		fFloatMin					1.0f

// 2002/05/23
// 충돌처리 정보를 익스큐티브를 통해 exe로 넘겨줄때 사용할 구조체. 주로 클럭을 보고, 충돌처리 제대로 되면 없어질 놈.
struct	DBG_COLLISION_INFO		
{
	//	DWORD	dwCollisionCandidateTri;
	DWORD	dwTickFindOctree;
	//	DWORD	dwTickFindBSPTree;
	//	DWORD	dwTickTestOneTri;
	DWORD	dwTickTotalTestCollision;
	DWORD	dwMeetingDynamicObjectCount;
};

enum	BUILD_TREE_FLAG
{
	//	BUILD_TREE_FLAG_BUILD_OCTREE		=	0x00000001,
	BUILD_TREE_FLAG_BUILD_STATIC_OBJECT	=	0x00000002
};



struct HFPOINT
{
	DWORD	dwPosX;
	DWORD	dwPosZ;
};


struct EVENT_TRIGGER_DESC
{
	VECTOR3		v3Pos;
	VECTOR3		v3Scale;
	VECTOR3		v3Rot;
	DWORD		dwColor;
};

struct GXLIGHT_PROPERTY
{
	LIGHT_DESC	lightDesc;
	DWORD		dwID;
	BOOL		bLightSwitch;
	BOOL		bShadowSwitch;
	BOOL		bEnableDynamicLight;

};

struct GXOBJECT_PROPERTY
{
	VECTOR3		v3Pos;
	VECTOR3		v3Axis;
	VECTOR3		v3Scale;

	float		fRad;
	DWORD		dwID;
	BOOL		bApplyHField;
	BOOL		bAsEffect;
	BOOL		bLock;
};
struct GXTRIGGER_PROPERTY
{
	EVENT_TRIGGER_DESC	evDesc;
	DWORD				dwID;
};

#define PID_MOD_FILENAME	"*MOD_FILE_NAME"
#define PID_MOTION_NUM		"*MOTION_NUM"
#define PID_MATERIAL_NUM	"*MATERIAL_NUM"
#define PID_STATIC_MODEL	"STATIC_MODEL"
#define PID_HEIGHT_FIELD	"HEIGHT_FIELD"
#define PID_GX_MAP			"GX_MAP"
#define PID_GX_OBJECT		"GX_OBJECT"
#define PID_GX_LIGHT		"GX_LIGHT"
#define PID_GX_TRIGGER		"GX_TRIGGER"
#define	PID_GX_METADATA		"GX_METADATA"
#define	PID_MAX_HEIGHT		"MAX_HEIGHT"
#define	PID_MIN_HEIGHT		"MIN_HEIGHT"
#define	PID_BOUNDINGBOX_MAX "BOX_MAX"
#define	PID_BOUNDINGBOX_MIN	"BOX_MIN"

typedef DWORD (__stdcall *LOAD_CALLBACK_FUNC)(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);
typedef DWORD (__stdcall *GX_FUNC)();
typedef float (__stdcall *SHADE_FUNC)(float fLightRS,float fLightRsRs,float fDistance,float fDistDist,float cosang);


HRESULT typedef (__stdcall *CREATE_INSTANCE_FUNC)(void* ppv);

struct LOAD_CALLBACK_DESC
{
	DWORD				dwTotalCount;
	DWORD*				pdwCurCount;
	LOAD_CALLBACK_FUNC	pFunc;
};

struct TRI_FACE
{
	VECTOR3		v3Point;
	PLANE		plane;
	WORD		wIndex[4];

};
#define TRI_FACE_SIZE			12+16+8
#define TRI_FACE_NORMAL_OFFSET	12
#define TRI_FACE_D_OFFSET		24
#define TRI_FACE_INDEX_OFFSET	28

struct SHORT_RECT		
{
	short    left;
	short    top;
	short    right;
	short    bottom;
};

struct HFIELD_POS
{
	DWORD		dwX;
	DWORD		dwZ;

};

struct FOG_DESC
{
	float	fStart;
	float	fEnd;
	float	fDensity;
	DWORD	dwColor;
	BOOL	bEnable;
};

struct VIEWPORT
{
	CAMERA_DESC					cameraDesc;

	VIEW_VOLUME					ViewVolume;
	PLANE						planeCameraEye;
	MATRIX4						matViewInverse;
	MATRIX4						matView;
	MATRIX4						matProj;
	MATRIX4						matForBillBoard;
	DWORD						dwAmbientColor;
	DWORD						dwEmissiveColor;
	SHORT_RECT					rcClip;
	WORD						wClipWidth;
	WORD						wClipHeight;
	BOOL						bFullScreen;
	FOG_DESC					fogDesc;
	DIRECTIONAL_LIGHT_DESC		dirLightDesc;

};

interface I3DModel;
struct MODEL_HANDLE
{
	I3DModel*		pModel;
	DWORD			dwRefIndex;
};

struct CLOCK
{
	DWORD			dwLO;
	DWORD			dwHI;
};

struct PERFORMANCE_CONTEXT
{
	CLOCK			dwClock;			// 0
	CLOCK			dwUsagedClock;		// 8
	DWORD			dwPrvTick;			// 16
	DWORD			dwFrameCount;		// 20
	DWORD			dwAvgFrame;			// 24

};

struct DECAL_DESC
{
	VECTOR3			v3Position;					// 데칼이 생기는 시작점.
	VECTOR3			v3FaceDirection;			// 데칼이 보는 방향. 
	VECTOR3			v3UpDirection;				// 데칼의 '위'를 나타내는 벡터. 들어오는 있는 그대로 믿는다.
	VECTOR3			v3XYZScale;					// 각 성분의 길이. 왼손 좌표계 기준으로 데칼의 오른쪽이 X, 윗쪽이 Z, 화면방향이 Y.
	BOOL			bLookAtPivot;
	//	void*			MaterialHandle;
	char			szMaterialName[MAX_NAME_LEN];
	DWORD			dwTextureCoordGenMethod;			// 텍스쳐링 방법. 아직 무효. 실린더매핑밖에 안된다.
	DWORD			dwTTL;								// 타임 투 라이브. 프레임단위. 영원히 할려면 0xffffffff
};

struct MULTI_DWORD_KEY
{
	DWORD		dwNum;
	DWORD		dwKey[16];
};

struct SET_FRAME_ARGS
{
	MATRIX4*				pMatrixEntry;
	COLLISION_MODEL_DESC*	pColModelDesc;
	MATRIX4*				pParentMat;
	DWORD					dwFrame;
	DWORD					dwMotionIndex;
	MATRIX4					matBillboard;
	MATRIX4					matTransform[2]; // 0은 parent, 1은 transform
	MATRIX4					matWorldForPhysique;
	DWORD					dwFlag;
};

struct FONT_PROPERTY_DESC
{
	// 내용 고정 
	HFONT	hFont;
	int		iWidth;
	int		iHeight;


	// 내용이 바뀔 블럭.
	DWORD			dwColor;	
	char*			pszString;
	DWORD			dwStrLen;
	CHAR_CODE_TYPE	type;
};

struct PICK_GXOBJECT_DESC
{
	GXOBJECT_HANDLE		gxo;
	VECTOR3				v3IntersectPoint;
	float				fDist;
	DWORD				dwModelIndex;
	DWORD				dwObjIndex;
};

struct LOCKED_RECT
{
	INT                 Pitch;
	void*               pBits;
};

enum TEXTURE_FORMAT
{
	TEXTURE_FORMAT_A8R8G8B8,
	TEXTURE_FORMAT_A4R4G4B4,
	TEXTURE_FORMAT_R5G6B5,
	TEXTURE_FORMAT_A1R5G5B5
};

enum TEXGEN_METHOD
{
	TEXGEN_METHOD_REFLECT_SPHEREMAP	= 0,
	TEXGEN_METHOD_WAVE				= 1
};

struct CUSTOM_EFFECT_DESC
{
	char			szEffectShaderName[32];
	TEXGEN_METHOD	method;
	BOOL			bDisableSrcTex;
	char			szTexName[MAX_NAME_LEN];
	DWORD			dwFlag;
	DWORD			dwReserved1;
	DWORD			dwReserved2;
	DWORD			dwReserved3;
	DWORD			dwReserved4;
};

enum TRANSFORM_MATRIX_TYPE
{
	TRANSFORM_MATRIX_TYPE_VIEW	 = 0,
	TRANSFORM_MATRIX_TYPE_WORLD	 = 1,
	TRANSFORM_MATRIX_TYPE_PRJ	 = 2
};

struct	AFTER_INTERPOLATION_CALL_BACK_ARG
{
	DWORD		dwIncreasedTick;					// 최근 게임 프레임에서 증가된 틱 카운트.
	DWORD		dwTickPerFrame;						// 한 프레임당 배정된 틱.   dwIncreasedTick의 최대값이다.
};

typedef DWORD (__stdcall *AfterInterpolationCallBack)( AFTER_INTERPOLATION_CALL_BACK_ARG* pArg);
// 익스큐티브::Run()에서 위치보간과 랜더링 사이에 호출되는 콜백.
// 카메라를 캐릭터에 붙이거나할 때 쓰면 유용하다.

// 축방향 타원체. 충돌처리 등에 쓰인다.
struct	AAELLIPSOID
{
	VECTOR3		P;			// 피봇, 절대값.
	float		w;			// x, z쪽 지름.
	float		h;			// y축쪽 높이.
};

// 충돌처리 함수 내부에서 쓰이는 구조체.
struct	COLLISION_RESULT
{
	float		fMeetTime;					// 만날때의 시점.
	VECTOR3		vWhereMeet;					// 만나는 점.
	VECTOR3		vMeetPivot;					// 만날때의 오브젝트의 중점.
	PLANE		MeetPlane;					// 만나는 면.
	DWORD		dwComponentType;			// 삼각형의 어디에 만났나.?
};

// CoGXObject에서 호출하는 충돌처리 함수의 리턴벨류.
struct	COLLISION_TEST_RESULT
{
	VECTOR3		Candidate;
	VECTOR3		LastVelocity;
	//	VECTOR3		To;
	BOOL		bLand;				// 땅에 닿아 있는가.?
};

typedef DWORD (__stdcall *CollisionTestCallBackProcedure)(COLLISION_RESULT*	pResult);
// 지오메트리에서 충돌처리때 쓰일 콜백.



