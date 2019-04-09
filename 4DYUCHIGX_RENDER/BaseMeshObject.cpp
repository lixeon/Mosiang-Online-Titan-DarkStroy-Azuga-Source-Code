#include "BaseMeshObject.h"

CBaseMeshObject::CBaseMeshObject()
{
	memset((char*)this+4,0,sizeof(CBaseMeshObject)-4);
}


STDMETHODIMP CBaseMeshObject::QueryInterface(REFIID refiid, PPVOID ppv)
{
	*ppv = NULL;
	return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CBaseMeshObject::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CBaseMeshObject::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
BOOL __stdcall CBaseMeshObject::StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic)
{
	return FALSE;
}
void __stdcall CBaseMeshObject::EndInitialize()
{
}
BOOL __stdcall CBaseMeshObject::InsertFaceGroup(FACE_DESC* pDesc)
{
	return FALSE;
}
	
BOOL __stdcall CBaseMeshObject::RenderProjection(
		DWORD		dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD		dwAlpha,
		BYTE*		pSpotLightIndex,
		DWORD		dwViewNum,
		DWORD		dwFlag
		)
{
	return FALSE;
}



BOOL __stdcall CBaseMeshObject::Render(DWORD dwAlpha,DWORD dwFlag)
{
	return FALSE;
}
void __stdcall CBaseMeshObject::DisableUpdate()
{
	m_bCanUpdate = FALSE;
}
CBaseMeshObject::~CBaseMeshObject()
{

}