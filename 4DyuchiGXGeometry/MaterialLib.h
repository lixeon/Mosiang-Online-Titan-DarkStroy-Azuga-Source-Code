// MaterialLib.h: interface for the CMaterialLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALLIB_H__95AF7809_1F59_4A6D_B5B6_3D10EFD0D81D__INCLUDED_)
#define AFX_MATERIALLIB_H__95AF7809_1F59_4A6D_B5B6_3D10EFD0D81D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_Common/typedef.h"
#include "../4DyuchiGRX_Common/IRenderer.h"

class CoGeometry;

class CMaterialLib  : public IUnknown
{
	DWORD					m_dwRefCount;
	CoGeometry*				m_pGeometry;
	DWORD					m_dwIndexInGeometry;

	MATERIAL_TABLE*			m_pMtlTableList;
	DWORD					m_dwMtlTableNum;
	DWORD					m_dwMaterialHandle;
	char					m_szFileName[MAX_NAME_LEN];

	void					Cleanup();
public:

	STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
	DWORD					GetMaterialHandle() {return m_dwMaterialHandle;}


	void					SetGeometry(CoGeometry* pGeometry) {m_pGeometry = pGeometry;}
	void					SetIndexInGeometry(DWORD dwIndex) {m_dwIndexInGeometry = dwIndex;}
	DWORD					GetFileIndex() {return m_dwIndexInGeometry;}

	
	BOOL					ReadFile(char* szFileName);
	BOOL					Initialize(I4DyuchiGXRenderer* pRenderer);
	char*					GetMtlFileName() {return m_szFileName;}
	BOOL					Unload();

	CMaterialLib();
	~CMaterialLib();

};

#endif // !defined(AFX_MATERIALLIB_H__95AF7809_1F59_4A6D_B5B6_3D10EFD0D81D__INCLUDED_)
