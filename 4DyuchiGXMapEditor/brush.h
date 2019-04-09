#pragma once

#include "../4DyuchiGRX_Common/IGeometry.h"

class C3DBrush
{
	I3DModel*					m_pBrushModel;
	DWORD						m_dwModelRefIndex;
	
	VECTOR3						m_v3BrushRot;
	VECTOR3						m_v3BrushPos;
	VECTOR3						m_v3BrushScale;
	MATRIX4						m_matWorld;
	
	char						m_szName[MAX_NAME_LEN];
public:
	DWORD						GetModelRefIndex() {return m_dwModelRefIndex;}
	MATRIX4*					GetTM()				{return &m_matWorld;}
	I3DModel*					GetModel()			{return m_pBrushModel;}

	BOOL						LoadModel(I4DyuchiGXGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer,char* szFileName);
	void						Render(I4DyuchiGXGeometry* pGeometry);
	
	char*						GetName() {return m_szName;}
	
	VECTOR3*					GetPos() {return &m_v3BrushPos;}
	VECTOR3*					GetScale() {return &m_v3BrushScale;}
	VECTOR3*					GetRot() {return &m_v3BrushRot;}
	DWORD						IsCollision(float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwPosX,DWORD dwPosY,DWORD dwFlag);

	

	C3DBrush();
	~C3DBrush();

	
	void						Render();

};
