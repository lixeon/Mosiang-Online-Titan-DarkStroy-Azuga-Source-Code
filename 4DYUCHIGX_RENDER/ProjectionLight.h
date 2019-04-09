#ifndef PROJECTION_LIGHT_H
#define PROJECTION_LIGHT_H

#include "CTexture.h"

class CoD3DDevice;


class CProjectionLight
{
	CTexture			m_LoadedTexture;
	LPDIRECT3DTEXTURE8	m_pGeneratedTexture;

	LPDIRECT3DTEXTURE8	m_pSelectedTexture;
	CoD3DDevice*		m_pRenderer;

	BOOL				GenerateLightMap(LPDIRECT3DDEVICE8 pDevice,DWORD dwRS,DWORD dwColor,DWORD dwFlag);
	void				CreateAttenuationMap(LPDIRECT3DTEXTURE8 pTexture,DWORD dwWidth,DWORD dwHeight,DWORD dwRS,DWORD dwColor);

public:
	LPDIRECT3DTEXTURE8	GetTexture() {return m_pSelectedTexture;}

	BOOL				Create(CoD3DDevice* pRenderer,DWORD dwRS,DWORD dwColor,char* szFileName,DWORD dwFlag);
	
	CProjectionLight();
	~CProjectionLight();
};

#endif
