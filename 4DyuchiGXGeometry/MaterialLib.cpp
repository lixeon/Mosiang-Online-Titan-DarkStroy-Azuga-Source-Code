// MaterialLib.cpp: implementation of the CMaterialLib class.
//
//////////////////////////////////////////////////////////////////////

#include "MaterialLib.h"
#include "CoGeometry.h"
#include "geometry_global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaterialLib::CMaterialLib()
{
	memset((char*)this+4,0,sizeof(CMaterialLib)-4);
}

STDMETHODIMP_(ULONG) CMaterialLib::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CMaterialLib::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
	{
		delete this;
	}
	return 0;
}

STDMETHODIMP CMaterialLib::QueryInterface(REFIID refiid, PPVOID ppv)
{

	return E_NOINTERFACE;
	
}
BOOL CMaterialLib::ReadFile(char* szFileName)
{
	BOOL	bResult = FALSE;
	
	m_dwMtlTableNum = CreateMaterialTableFromFile(&m_pMtlTableList,szFileName);
	if (!m_dwMtlTableNum)
		goto lb_return;


	lstrcpy(m_szFileName,szFileName);
	bResult = TRUE;
	

lb_return:
	return bResult;
}
BOOL CMaterialLib::Initialize(I4DyuchiGXRenderer* pRenderer)
{
	BOOL	bResult = FALSE;

	if (!m_dwMtlTableNum)
		goto lb_return;

	if (pRenderer)
		m_dwMaterialHandle = pRenderer->CreateMaterialSet(m_pMtlTableList,m_dwMtlTableNum);

	Cleanup();
	bResult = TRUE;

lb_return:
	return bResult;

}
void CMaterialLib::Cleanup()
{
	ReleaseMaterialTableFromFile(m_pMtlTableList);
	m_pMtlTableList = NULL;
	m_dwMtlTableNum = 0;
}

BOOL CMaterialLib::Unload()
{
	BOOL	bResult = FALSE;
	if (!m_pGeometry)
		goto lb_return;
	
	if (!m_dwIndexInGeometry)
		goto lb_return;

	
	bResult = m_pGeometry->UnloadPreLoadedItem(m_dwIndexInGeometry);
lb_return:
	return bResult;
	
}

CMaterialLib::~CMaterialLib()
{
	Cleanup();
	if (m_dwIndexInGeometry)
		m_pGeometry->DeleteFileItem(m_dwIndexInGeometry);

}
