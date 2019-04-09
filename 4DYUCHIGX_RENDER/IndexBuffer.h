#pragma once

#include "../4DyuchiGRX_common/typedef.h"
#include "IBManager.h"

#include <d3d8.h>

class CD3DResourceManager;
class CIndexBuffer
{
	LPDIRECT3DINDEXBUFFER8			m_pIB;
	DWORD							m_dwStartIndex;
	DWORD							m_dwIndicesNum;
	CD3DResourceManager*			m_pResourceManager;

	// for indexbuffer pool & HEAP
	IIBManager*						m_pIBManager;
	void*							m_pIBHandle;
	
	
public:
	BOOL							m_bEnable;
	void							CheckValid() 
	{
		if (!m_pIB) 
		__asm int 3 
	}


	friend	class	CIBHeap;
	friend	class	CStaticIBPool;
	DWORD							GetStartIndex() {return m_dwStartIndex;}
	LPDIRECT3DINDEXBUFFER8			GetIndexBuffer() {return m_pIB;}
	BOOL							CreateIndexBuffer(CD3DResourceManager* pResourceManager,WORD* pIndex,DWORD dwUsage,D3DPOOL pool,DWORD dwNum);
	BOOL							SetIndexList(WORD* pIndex,DWORD dwNum);
	BOOL							LockPtr(WORD** ppWord);
	void							UnlockPtr();
	void							Cleanup();
	DWORD							GetIndicesNum() {return m_dwIndicesNum;}

	CIndexBuffer();
	~CIndexBuffer();
};
