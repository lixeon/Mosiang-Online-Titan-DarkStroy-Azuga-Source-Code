// IBHeap.h: interface for the CIBHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBHEAP_H__9E49BEF8_FFB7_4C88_8F19_5099A94E39B7__INCLUDED_)
#define AFX_IBHEAP_H__9E49BEF8_FFB7_4C88_8F19_5099A94E39B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D3DResourceBufferHeap.h"
#include "IBManager.h"

class CIndexBuffer;
class CoD3DDevice;
class CD3DResourceManager;


class CIBHeap  : public IIBManager
{
	CD3DResourceBufferHeap		m_D3DRCBufferHeap;
	CoD3DDevice*				m_pRenderer;
	CD3DResourceManager*		m_pResourceManager;
	RESORUCE_POOL_TYPE			m_type;


	DWORD						m_dwUsage;
	D3DPOOL						m_D3DPOOL;
	DWORD						m_dwItemNum;

	void						ClearMember();
public:
	DWORD						GetIndexBufferNum() {return m_D3DRCBufferHeap.GetD3DResourceBufferNum();}
	BOOL						Initialize(CD3DResourceManager* pResourceManager,DWORD dwUsage,D3DPOOL pool,DWORD dwMaxIndicesNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,DWORD dwFlag);

	void	__stdcall			OnLostDevice();
	void	__stdcall			Reset();
	void	__stdcall			Release();
	DWORD	__stdcall			GetResourceType();

	BOOL	__stdcall			AllocIB(DWORD* pdwErrorCode,CIndexBuffer* pIndexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController);
	void	__stdcall			ReleaseIB(void* pHandle);
	void	__stdcall			ReleaseBufferAll();
	void	__stdcall			RemoveD3DRCBuffer();


	


	void						Cleanup();

	CIBHeap();
	~CIBHeap();

};

#endif // !defined(AFX_IBHEAP_H__9E49BEF8_FFB7_4C88_8F19_5099A94E39B7__INCLUDED_)
