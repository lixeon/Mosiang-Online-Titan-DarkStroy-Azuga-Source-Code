// GuildMarkImg.cpp: implementation of the CGuildMarkImg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMarkImg.h"

#include "cImageSelf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMarkImg::CGuildMarkImg(MARKNAMETYPE markname,cImageSelf* pImg)
{
	m_MarkName = markname;
	m_pImg = pImg;
	cImageRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 16;
	rect.bottom = 12;

	if( m_pImg )
		m_pImg->SetImageSrcRect(&rect);
}

CGuildMarkImg::~CGuildMarkImg()
{

}

void CGuildMarkImg::Render(VECTOR2* pos, DWORD dwColor)
{
	VECTOR2 scale;
	scale.x = 1;
	scale.y = 0.75;
	m_pImg->RenderSprite(&scale,NULL,0,pos,dwColor);
}

void CGuildMarkImg::Release()
{

	SAFE_DELETE( m_pImg );
}
