#pragma once

#include "object.h"
#include "../4DyuchiGXGFunc/global.h"


#pragma pack(push,4)

struct	FILE_LIGHT_HEADER
{
	DWORD			m_dwColor;
	float			m_fAspect;
	float			m_fIntense;
	float			m_fAttentStart;
	float			m_fAttentEnd;
	CLightFlag		m_lightFlag;
};
#pragma pack(pop)

class CLightObject : public CBaseObject
{
	DWORD				m_dwColor;
	float				m_fAspect;
	float				m_fIntense;
	float				m_fAttentStart;
	float				m_fAttentEnd;
	CLightFlag			m_lightFlag;

public:
	CLightFlag			GetLightFlag() {return m_lightFlag;}
	void				SetColor(float r,float g,float b,float a) {m_dwColor = COLORtoDWORD(r,g,b,a);}
	void				SetAttent(float start,float end) {m_fAttentStart = start; m_fAttentEnd = end;}
	DWORD				GetColor()	{return m_dwColor;}
	float				GetAttentStart() {return m_fAttentStart;}
	float				GetAttentEnd() {return m_fAttentEnd;}
	DWORD				ReadFile(void* pFP,DWORD dwVersion);

	CLightObject();
	~CLightObject();


};


