#pragma once

#include "../4DyuchiGRX_common/typedef.h"

inline DWORD COLORtoDWORD(float r,float g,float b,float a)
{
	DWORD R = (DWORD)(r*255);
	DWORD G = (DWORD)(g*255);
	DWORD B = (DWORD)(b*255);
	DWORD A = (DWORD)(a*255);

	return ( 
		( (R<<24) & 0xff000000 ) || 
		( (G<<16) & 0x00ff0000 ) ||
		( (B<<8) & 0x0000ff00 ) ||
		( A & 0x000000ff )
		);
}

class CMaterial
{
	OBJECT_TYPE		m_dwType;
	DWORD			m_dwTextureNum;
	DWORD			m_dwDiffuse;
	DWORD			m_dwAmbient;
	DWORD			m_dwSpecular;
	float			m_fTransparency;
	float			m_fShine;
	float			m_fShineStrength;
	TCHAR			m_uszTexmapFileName[TEXTURE_MAP_TYPE_NUM][MAX_NAME_LEN];
	DWORD			m_dwIndex;
	DWORD			m_dwFlag;

public:
	DWORD			GetFlag()	{return m_dwFlag;}
	TCHAR*			GetTexmapName(TEXTURE_MAP_TYPE type) {return &m_uszTexmapFileName[type][0];}
	void			SetDiffuse(float r,float g,float b,float a) {m_dwDiffuse = COLORtoDWORD(r,g,b,a);}
	void			SetAmbient(float r,float g,float b,float a) {m_dwAmbient = COLORtoDWORD(r,g,b,a);}
	void			SetSpecular(float r,float g,float b,float a) {m_dwSpecular = COLORtoDWORD(r,g,b,a);}
	void			SetTransparency(float fVal) {m_fTransparency = fVal;}
	void			SetShine(float fVal) {m_fShine = fVal;}
	void			SetShineStrength(float fVal) {m_fShineStrength = fVal;}
	
	DWORD			GetDiffuse() {return m_dwDiffuse;}
	DWORD			GetAmbient() {return m_dwAmbient;}
	DWORD			GetSpecular() {return m_dwSpecular;}

	float			GetTransparency() {return m_fTransparency;}
	float			GetShine() {return m_fShine;}
	float			GetShineStrength() {return m_fShineStrength;}
	CMaterial();
};
