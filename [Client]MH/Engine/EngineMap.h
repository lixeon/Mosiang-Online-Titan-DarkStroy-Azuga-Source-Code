// EngineMap.h: interface for the EngineMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINEMAP_H__F77B65F3_874B_42AB_BFAE_5A409891E6B7__INCLUDED_)
#define AFX_ENGINEMAP_H__F77B65F3_874B_42AB_BFAE_5A409891E6B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"

class CEngineMap  
{
	//////////////////////////////////////////////////////////////////////////
	I4DyuchiGXExecutive*		m_pExecutive;
	
	BOOL						m_bMapBuild;

	GXMAP_HANDLE				m_ghMap;
	MAABB						m_WorldBox;
	
	IHeightField*				m_pHField;
	HFIELD_DESC					m_hfDesc;
	char						m_szHFieldName[_MAX_PATH];

	I3DStaticModel*				m_pStaticModel;

	cPtrList					m_GXObjectList;	
	cPtrList					m_GXLightList;	


	BOOL						ImportHeightField(char* szFileName,DWORD dwIndexBufferNum);
	BOOL						ImportStaticModel(char* szFileName);
	BOOL						BuildMap( MAABB* pWorldBox);
	void						CleanHeightField();
	void						CleanStaticModel();
	
	//////////////////////////////////////////////////////////////////////////
	
public:
	CEngineMap();
	virtual ~CEngineMap();

	BOOL Init(char* szMapFile);
	void Release();

	void SetAmbientColor(DWORD Color);
};

#endif // !defined(AFX_ENGINEMAP_H__F77B65F3_874B_42AB_BFAE_5A409891E6B7__INCLUDED_)
