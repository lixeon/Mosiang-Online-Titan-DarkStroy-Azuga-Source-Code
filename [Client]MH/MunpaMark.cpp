// MunpaMark.cpp: implementation of the CMunpaMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MunpaMark.h"
#include "./interface/cWindowManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMunpaMark::CMunpaMark()
{
}

CMunpaMark::~CMunpaMark()
{
	Release();
}

BOOL CMunpaMark::Init(char* strFileName)
{
	Release();	
	m_Image.LoadSprite(strFileName);
	return TRUE;
}

void CMunpaMark::Release()
{
	m_Image.Release();
}

void CMunpaMark::Render(VECTOR2* pPos) 
{
	m_Image.RenderSprite(NULL,NULL,0,pPos,0xffffffff);
}
