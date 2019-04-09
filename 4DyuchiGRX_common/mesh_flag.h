#pragma once

#include "stdafx.h"

// 0 bit - 7 bit /////////////////////////////////////////////
enum SHADE_TYPE
{
	SHADE_TYPE_VERTEX_LIGHT_IM		= 0x00000000,
	SHADE_TYPE_VERTEX_LIGHT_RT		= 0x00000001,
	SHADE_TYPE_LIGHT_MAP			= 0x00000003
};
#define	SHADE_TYPE_MASK				0x0000000f
#define	SHADE_TYPE_MASK_INVERSE		0xfffffff0

enum TRANSFORM_TYPE
{
	TRANSFORM_TYPE_SOLID = 0x0000000,
	TRANSFORM_TYPE_NOT_SOLID = 0x00000010,
	TRANSFORM_TYPE_ALIGN_VIEW = 0x00000030,
	TRANSFORM_TYPE_ILLUSION = 0x00000050
};
#define TRANSFORM_TYPE_MASK			0x000000f0
#define TRANSFORM_TYPE_MASK_INVERSE	0xffffff0f

// 0 bit - 7 bit /////////////////////////////////////////////

// 8 bit /////////////////////////////////////////////
enum RIGID_TYPE
{
	RIGID_TYPE_NOT_RIGID	= 0x00000000,
	RIGID_TYPE_RIGID		= 0x00000100
};
#define RIGID_TYPE_MASK			0x00000100
#define RIGID_TYPE_MASK_INVERSE	~(RIGID_TYPE_MASK)
// 8 bit /////////////////////////////////////////////

// 9 bit /////////////////////////////////////////////
enum PICK_ENABLE_TYPE
{
	PICK_ENABLE = 0x00000000,
	PICK_DISABLE = 0x00000200
};
#define PICK_ENABLE_TYPE_MASK			0x00000200
#define PICK_ENABLE_TYPE_MASK_INVERSE	~(PICK_ENABLE_TYPE_MASK)
// 9 bit /////////////////////////////////////////////


// 최상위 1비트는 z버퍼 쓰기 가능/불가능
// 그 다음 7비트는 렌더링 우선순위 128단계
#define RENDER_ZPRIORITY_DEFAULT	0
#define RENDER_ZPRIORITY_UNIT		-10.0f
#define RENDER_ZPRIORITY_MASK			0x7f000000
#define RENDER_ZPRIORITY_MASK_INVERSE	0x80ffffff

#define WRITE_ZBUFFER_MASK			0x80000000
#define WRITE_ZBUFFER_MASK_INVERSE	0x7fffffff

class CMeshFlag
{
	DWORD				m_dwFlag;
public:
	BOOL				IsDisableZBubfferWrite() { return (m_dwFlag & WRITE_ZBUFFER_MASK)>>31;}
	void				DisableZBufferWrite()	{ m_dwFlag |= WRITE_ZBUFFER_MASK;}
	void				EnableZBufferWrite()	{m_dwFlag &= WRITE_ZBUFFER_MASK_INVERSE;}

	int					GetRenderZPriorityValue() 
	{
		// 최상위 1비트는 z버퍼쓰기 가능/불가능, 그 다음 7비트는 렌더링 순서로 사용한다.
		// 7비트중 최상위비트는 싸인비트로 사용한다.
		int		iZOrder =  (m_dwFlag & RENDER_ZPRIORITY_MASK)>>24;
		int		sign_mask = ( 0 - ( (iZOrder & 0x00000040)>>6 ) ) & 0xffffffc0;
		iZOrder = iZOrder | sign_mask;
		return iZOrder;
	}
	void				SetRenderZPriorityValue(int iZOrder) 
	{
		iZOrder = iZOrder & 0x0000007f;
		m_dwFlag = (m_dwFlag & RENDER_ZPRIORITY_MASK_INVERSE) | (iZOrder<<24);
	}

	SHADE_TYPE			GetShadeType() {return (SHADE_TYPE)(m_dwFlag & SHADE_TYPE_MASK);}
	void				SetShadeType(SHADE_TYPE type) {m_dwFlag = (m_dwFlag & SHADE_TYPE_MASK_INVERSE) | type;}

	

	TRANSFORM_TYPE		GetTransformType() {return (TRANSFORM_TYPE)(m_dwFlag & TRANSFORM_TYPE_MASK);}
	void				SetTransformType(TRANSFORM_TYPE type) {m_dwFlag = (m_dwFlag & TRANSFORM_TYPE_MASK_INVERSE) | type;}

	RIGID_TYPE			GetRigidType()  {return (RIGID_TYPE)(m_dwFlag & RIGID_TYPE_MASK);}
	void				SetRigidType(RIGID_TYPE type) {m_dwFlag = (m_dwFlag & RIGID_TYPE_MASK_INVERSE) | type;}


	PICK_ENABLE_TYPE	GetPickEnable() {return (PICK_ENABLE_TYPE)(m_dwFlag & PICK_ENABLE_TYPE_MASK);}
	void				SetPickEnable(PICK_ENABLE_TYPE type) {m_dwFlag = (m_dwFlag & PICK_ENABLE_TYPE_MASK_INVERSE) | type;}

	
	CMeshFlag()			{m_dwFlag = 0;}
};
enum DYNAMIC_LIGHT_APPLY_TYPE
{
	DYNAMIC_LIGHT_APPLY_TYPE_DISABLE			=	0x00000000,
	DYNAMIC_LIGHT_APPLY_TYPE_CHARACTER_ENABLE	=	0x00000001,
	DYNAMIC_LIGHT_APPLY_TYPE_MAP_ENABLE			=	0x00000002,
	DYNAMIC_LIGHT_APPLY_TYPE_BOTH_ENABLE		=	0x00000003

};
#define DYNAMIC_LIGHT_APPLY_TYPE_MASK				0x0000000f
#define DYNAMIC_LIGHT_APPLY_TYPE_MASK_INVERSE		0xfffffff0

class CLightFlag
{
	DWORD			m_dwFlag;
public:
	DYNAMIC_LIGHT_APPLY_TYPE	GetDynamicLightType() {return (DYNAMIC_LIGHT_APPLY_TYPE)(m_dwFlag & DYNAMIC_LIGHT_APPLY_TYPE_MASK);}
	void						SetDynamicLightType(DYNAMIC_LIGHT_APPLY_TYPE type) {m_dwFlag = (m_dwFlag & DYNAMIC_LIGHT_APPLY_TYPE_MASK_INVERSE) | type;}
	CLightFlag()				{m_dwFlag = 0;}
};

class CCameraFlag
{
	DWORD			m_dwFlag;
	
};
