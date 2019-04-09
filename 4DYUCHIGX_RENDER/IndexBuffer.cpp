#include "IndexBuffer.h"
#include "d3d_helper.h"

#include "D3DResourceManager.h"

CIndexBuffer::CIndexBuffer()
{
	m_pIB = NULL;
	m_dwStartIndex = 0;
	m_dwIndicesNum = 0;
	m_pResourceManager = NULL;

	
	m_pIBManager = NULL;
	m_pIBHandle = NULL;
	m_bEnable = FALSE;
}

BOOL CIndexBuffer::CreateIndexBuffer(CD3DResourceManager* pResourceManager,WORD* pIndex,DWORD dwUsage,D3DPOOL pool,DWORD dwNum)
{

	BOOL		bResult = FALSE;
	
	BYTE*			pDest;
	HRESULT			hr;


	hr = pResourceManager->CreateIndexBuffer(dwNum* sizeof(WORD),
                                      dwUsage,
                                      D3DFMT_INDEX16, pool,
                                      &m_pIB);
	if (D3D_OK != hr)
		goto lb_return;


	m_dwIndicesNum = dwNum;

	if (pIndex)
	{
		m_pIB->Lock( 0, 0, (BYTE**)&pDest, 0 );
		memcpy(pDest,pIndex,sizeof(WORD)*dwNum);
		m_pIB->Unlock();
	}
	bResult = TRUE;
	m_pResourceManager = pResourceManager;

lb_return:

	return bResult;

}
BOOL CIndexBuffer::SetIndexList(WORD* pIndex,DWORD dwNum)
{
	BOOL	bResult = FALSE;

	BYTE*			pDest;

	HRESULT	hr = m_pIB->Lock(m_dwStartIndex*sizeof(WORD),sizeof(WORD)*m_dwIndicesNum,(BYTE**)&pDest, 0 );

	if (hr != D3D_OK)
		goto lb_return;


	memcpy(pDest,pIndex,sizeof(WORD)*dwNum);

	m_pIB->Unlock();

	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL CIndexBuffer::LockPtr(WORD** ppWord)
{
	BOOL	bResult = FALSE;
	HRESULT	hr = m_pIB->Lock(m_dwStartIndex*sizeof(WORD),sizeof(WORD)*m_dwIndicesNum,(BYTE**)ppWord, 0 );

	if (hr != D3D_OK)
	{
		*ppWord = NULL;
		goto lb_return;
	}

	bResult = TRUE;
lb_return:
	return bResult;
}

void CIndexBuffer::UnlockPtr()
{
	m_pIB->Unlock();
}
void CIndexBuffer::Cleanup()
{
	if (m_pIBManager)
	{
		m_pIBManager->ReleaseIB(m_pIBHandle);


	}
	else
	{
		if (m_pIB)
		{
			m_pResourceManager->Release(m_pIB);
		}
	}
	m_pIB = NULL;	
	m_dwStartIndex = 0;
	m_dwIndicesNum = 0;
	m_pResourceManager = NULL;

	m_pIBManager = NULL;
	m_pIBHandle = NULL;
	
}
CIndexBuffer::~CIndexBuffer()
{
	Cleanup();
}
