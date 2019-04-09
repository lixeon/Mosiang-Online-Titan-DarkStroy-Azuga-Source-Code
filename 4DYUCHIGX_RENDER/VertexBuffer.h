#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include <d3d8.h>
#include "renderer_typedef.h"
#include "VBManager.h"

class CD3DResourceManager;
class CVBPool;
class CVBHeap;
class CIBHeap;


class CVertexBuffer
{
public:
	DWORD						m_dwSize;
	LPDIRECT3DVERTEXBUFFER8		m_pVB;
	DWORD						m_dwStartIndex;
	DWORD						m_dwVerticesNum;
	DWORD						m_dwVertexShader;
	CD3DResourceManager*		m_pResourceManager;
	
	
	
	// for vertexbuffer pool & heap
	IVBManager*					m_pVBManager;
	void*						m_pVBHandle;

public:
	BOOL						m_bEnable;
	
	friend class				CVBHeap;
	friend class				CStaticVBPool;
	DWORD						GetStartIndex()	{return m_dwStartIndex;}
	LPDIRECT3DVERTEXBUFFER8		GetVertexBuffer() {return m_pVB;}
	DWORD						GetVertexSize() {return m_dwSize;}
	DWORD						GetVerticesNum() {return m_dwVerticesNum;}

	BOOL						CreateVertexBuffer(CD3DResourceManager* pResourceManager,DWORD dwSize,DWORD dwUsage,DWORD FVF,D3DPOOL pool,DWORD dwNum);
	
	BOOL						SetVertexPosList(VECTOR3* pv3,DWORD dwNum);
	BOOL						SetVertexPosList(VECTOR4* pv4,DWORD dwNum);
	BOOL						SetVertexPosList(WORD* pIndex,VECTOR3* pv3,DWORD dwNum);
	BOOL						SetVertexPosList(DWORD dwDestPitch,DWORD dwSrcPosX,DWORD dwSrcPosY,HFIELD_DESC* pHFDesc,DWORD dwSampleStep);
	BOOL						SetVertexPosListForScreenFiltier(DWORD dwDestPitch,float left,float right,float top,float bottom,DWORD dwFacesNumX,DWORD dwFacesNumY);


	

	

	BOOL						SetVertexColor(DWORD dwColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum);
	BOOL						SetVertexColor(DWORD* pVertexColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum);
	
	BOOL						SetVertexPosListWithManualLock(BYTE* pDest,DWORD dwDestPitch,DWORD dwSrcPosX,DWORD dwSrcPosY,HFIELD_DESC* pHFDesc,DWORD dwSampleStep);
	BOOL						SetVertexColorWithManualLock(BYTE* pDest,DWORD* pVertexColor,DIFFUSE_COORD_OFFSET offset,DWORD dwNum);
	BOOL						SetVertexPosListWithManualLockForScreenFiltier(BYTE* pDest,DWORD dwDestPitch,float left,float right,float top,float bottom,DWORD dwFacesNumX,DWORD dwFacesNumY);
	


	BOOL						SetTexCoord(TVERTEX* ptv,TEX_COORD_OFFSET offset,DWORD dwNum);
	BOOL						SetTexCoord(TVERTEX* ptv,WORD* pIndex,TEX_COORD_OFFSET offset,DWORD dwNum);
	
	BOOL						SetTLVERTEXList(VECTOR4* pv4Rect,DWORD dwColor,DWORD dwNum);
	BOOL						SetVertexAndUVList(VECTOR3* pv3,TVERTEX* ptv,TEX_COORD_OFFSET offset,DWORD dwNum);

	HRESULT						LockVertexPtr(BYTE** ppVPtr,DWORD dwFlag);
	void						UnlockVertexPtr();
	DWORD						GetVertexShader()	{return m_dwVertexShader;}
	BOOL						SetNormal(VECTOR3* pnv,DWORD dwNum);
	BOOL						SetTangentU(VECTOR3* pnv,DWORD dwNum);
	void						Cleanup();
	void						SetTimeStamp();
	
	CVertexBuffer();
	~CVertexBuffer();

};
