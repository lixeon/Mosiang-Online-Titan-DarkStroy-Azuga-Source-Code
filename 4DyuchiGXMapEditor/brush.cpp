#include "brush.h"
#include "../4DyuchiGXGFunc/global.h"


C3DBrush::C3DBrush()
{
	memset(this,0,sizeof(C3DBrush));
	m_v3BrushScale.x = 1.0f;
	m_v3BrushScale.y = 1.0f;
	m_v3BrushScale.z = 1.0f;

}
	

BOOL C3DBrush::LoadModel(I4DyuchiGXGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer,char* szFileName)
{
	if (0xffffffff == pGeometry->LoadModel(&m_pBrushModel,szFileName,0))
		return FALSE;

	m_pBrushModel->Initialize(pRenderer,MODEL_INITIALIZE_FLAG_NOT_OPTIMIZE);
	m_dwModelRefIndex = m_pBrushModel->Reference();
	GetNameRemovePath(m_szName,szFileName);
	return TRUE;
}
void C3DBrush::Render(I4DyuchiGXGeometry* pGeometry)
{
	
	MATRIX4	matPos,matRotX,matRotY,matRotZ,matRot,matScale;
	
	SetScaleMatrix(&matScale,&m_v3BrushScale);

	SetRotationXMatrix(&matRotX,m_v3BrushRot.x);
	SetRotationYMatrix(&matRotY,m_v3BrushRot.y);
	SetRotationZMatrix(&matRotZ,m_v3BrushRot.z);
		
	MatrixMultiply2(&matRot,&matRotX,&matRotY);
	MatrixMultiply2(&matRot,&matRot,&matRotZ);

	TranslateMatrix(&matPos,&m_v3BrushPos);
	
	MatrixMultiply2(&m_matWorld,&matRot,&matPos);
	MatrixMultiply2(&m_matWorld,&matScale,&m_matWorld);

	pGeometry->RenderModel(m_pBrushModel,m_dwModelRefIndex,NULL,&m_matWorld,0,0,128,0,0,0);
}
DWORD C3DBrush::IsCollision(float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwPosX,DWORD dwPosY,DWORD dwFlag)
{
	POINT pt;
	pt.x = dwPosX;
	pt.y = dwPosY;

	VECTOR3		v3IntersectPoint;
	return m_pBrushModel->GetMeshObjectWithScreenCoord(&v3IntersectPoint,pfDist,pdwFaceGroupIndex,m_dwModelRefIndex,NULL,&m_matWorld,0,0,&pt,0,dwFlag);
}

C3DBrush::~C3DBrush()
{
	if (m_pBrushModel)
	{
		m_pBrushModel->UnReference(m_dwModelRefIndex);
		m_pBrushModel->Release();
		m_pBrushModel = NULL;
	}

}