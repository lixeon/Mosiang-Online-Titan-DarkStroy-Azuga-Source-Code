// VBHeap.h: interface for the CVBHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBHEAP_H__068F46A7_42EB_417D_A03D_EDECC90F2101__INCLUDED_)
#define AFX_VBHEAP_H__068F46A7_42EB_417D_A03D_EDECC90F2101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"
#include "d3d8.h"

#include "VBManager.h"
#include "D3DResourceBufferHeap.h"

class CVertexBuffer;
class CoD3DDevice;
class CD3DResourceManager;



class CVBHeap  : public IVBManager
{
	CD3DResourceBufferHeap		m_D3DRCBufferHeap;
	CoD3DDevice*				m_pRenderer;
	CD3DResourceManager*		m_pResourceManager;
	RESORUCE_POOL_TYPE			m_type;
	
	
	DWORD						m_dwFVF;
	DWORD						m_dwUsage;
	DWORD						m_dwVertexSize;
	D3DPOOL						m_D3DPOOL;
	
	

	char*						m_pMassMemory;
	DWORD						m_dwMassMemorySize;

	
	void					ClearMember();
public:
	
	DWORD					GetVertexBufferNum()	{return m_D3DRCBufferHeap.GetD3DResourceBufferNum();}
	BOOL					Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwVertexSize,DWORD dwMaxVerticesNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,DWORD dwFlag);

	void					__stdcall OnLostDevice();
	void					__stdcall Reset();
	void					__stdcall Release();
	DWORD					__stdcall GetResourceType();

	BOOL					__stdcall AllocVB(DWORD* pdwErrorCode,CVertexBuffer* pVertexBuffer,DWORD dwVerticesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController);
	void					__stdcall ReleaseVB(void* pHandle);
	void					__stdcall ReleaseBufferAll();
	void					__stdcall SetTimeStamp(void* pHandle);
	

	void					Cleanup();

	CVBHeap();
	~CVBHeap();

};

#endif // !defined(AFX_VBHEAP_H__068F46A7_42EB_417D_A03D_EDECC90F2101__INCLUDED_)
