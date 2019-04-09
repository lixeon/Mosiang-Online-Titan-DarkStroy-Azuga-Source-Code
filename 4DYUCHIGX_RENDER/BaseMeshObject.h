#ifndef BASE_MESH_OBJECT_H
#define BASE_MESH_OBJECT_H

#include "../4DyuchiGRX_common/IGeometry.h"
#include "d3d_helper.h"

class CoD3DDevice;

class CBaseMeshObject : public IDIMeshObject
{
protected:
	float					m_fRenderPriorityFactor;
	DWORD					m_dwRefCount;
	DWORD					m_dwTotalFacesNum;
	DWORD					m_dwMaxFaceGroupNum;
	DWORD					m_dwFaceGroupNum;
	DWORD					m_dwVertexNum;
	CoD3DDevice*			m_pRenderer;
	IGeometryController*		m_pMeshController;
	IGeometryControllerStatic*	m_pControllerStatic;
	CMeshFlag				m_meshFlag;
	BOOL					m_bCanUpdate;
	DWORD					m_dwObjCount;
	
public:
	STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL		__stdcall	StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic);
	void		__stdcall	EndInitialize();
	BOOL		__stdcall	InsertFaceGroup(FACE_DESC* pDesc);
	void	__stdcall		DisableUpdate();
	BOOL	__stdcall		RenderProjection(
		DWORD		dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD		dwAlpha,
		BYTE*		pSpotLightIndex,
		DWORD		dwViewNum,
		DWORD		dwFlag
		);
	BOOL		__stdcall	Render(DWORD dwAlpha,DWORD dwFlag);
	

	float					GetRenderPriorityFactor()		{return m_fRenderPriorityFactor;}
	DWORD					GetFacesNum()			{return m_dwTotalFacesNum;}
	DWORD					GetVertexNum()			{return m_dwVertexNum;}
	void					SetObjCount(DWORD dwCount) {m_dwObjCount = dwCount;}
	void					SetRenderer(CoD3DDevice* pRenderer) {m_pRenderer = pRenderer;}

	CBaseMeshObject();
	virtual					~CBaseMeshObject();

};
#endif
