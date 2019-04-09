// DecalMeshObject.h: interface for the CImmMeshObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECALMESHOBJECT_H__FC9D7FA4_35AD_4AD6_9022_41594DF66346__INCLUDED_)
#define AFX_DECALMESHOBJECT_H__FC9D7FA4_35AD_4AD6_9022_41594DF66346__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"
#include "VertexBuffer.h"
#include "../4DyuchiGRX_Common/IRenderer.h"
#include "BaseMeshObject.h"

class CoD3DDevice;
struct DIMATERIAL;

class CImmMeshObject  : public CBaseMeshObject , public IDIImmMeshObject 
{
	CVertexBuffer	m_VB;
	BYTE*			m_pWorkMemory;
	DWORD			m_dwWorkMemorySize;
	DWORD			m_dwTriCount;
	DIMATERIAL*		m_pMtlHandle;
public:
	

	STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

//	BOOL		__stdcall	StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl);
//	void		__stdcall	EndInitialize();
//	BOOL		__stdcall	InsertFaceGroup(FACE_DESC* pDesc);
	

		
	BOOL		__stdcall	Render(
		DWORD dwRefIndex,DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag
		);

	
//	BOOL		__stdcall	RenderShadow(DWORD dwRefIndex,DWORD dwAlpha,BYTE* pSpotLightIndex,DWORD dwViewNum,DWORD dwFlag);
	BOOL		__stdcall	Update(DWORD dwFlag);
//	void		__stdcall	DisableUpdate();
	BOOL		__stdcall	GetTriBufferPtr(BYTE** ppDest,DWORD* pdwSize);
	
	void					Initialize(CoD3DDevice* pRenderer);
	void					Cleanup();
	BOOL					Build(IVERTEX* piv3Tri,DWORD dwTriCount,void* pMtlHandle,DWORD dwFlag);
	CImmMeshObject();

	
};

#endif // !defined(AFX_DECALMESHOBJECT_H__FC9D7FA4_35AD_4AD6_9022_41594DF66346__INCLUDED_)
