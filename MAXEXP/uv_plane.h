#pragma once

#include "../max_common/stdafx.h"

struct VERTEX_LINK
{
	VECTOR3*		pv3In;	// 점3개짜리 포인터 
	VECTOR3			v3Out[3];
	TVERTEX*		ptv;
	VERTEX_LINK*	pNext;
};

class CUVPlane
{
	VERTEX_LINK*		m_pVLHead;
	VERTEX_LINK*		m_pVLCur;
	PLANE				m_plane;
	DWORD				m_dwVLNum;
	TEXTURE_PLANE		m_texPlane;
public:

	TEXTURE_PLANE*		GetTexturePlane() {return &m_texPlane;}
	PLANE*				GetPlane() {return &m_plane;}
	VERTEX_LINK*		GetVLHead() {return m_pVLHead;}

	void				SetPlane(PLANE* pPlane) {memcpy(&m_plane,pPlane,sizeof(PLANE));}
	DWORD				AddVertex(VECTOR3* ptv3,TVERTEX* ptv);

	CUVPlane();
	~CUVPlane();



};