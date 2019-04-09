// GuildMark.cpp: implementation of the CGuildMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMark.h"

#include "GuildMarkImg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMark::CGuildMark()
{
	m_pMarkImg = NULL;
}

CGuildMark::~CGuildMark()
{
}

void CGuildMark::Clear()
{
	if(m_pMarkImg)
	{
		if(m_pMarkImg->GetMarkName() != 0)
		{
			m_pMarkImg->Release();
			delete m_pMarkImg;
			m_pMarkImg = NULL;
		}
	}
}

void CGuildMark::SetGuildMarkImg(CGuildMarkImg* pMarkImg)
{
	Clear();

	m_pMarkImg = pMarkImg;
}

void CGuildMark::Render(VECTOR2* pos, DWORD dwColor)
{
	if(m_pMarkImg == NULL)
		return;
	
	m_pMarkImg->Render(pos,dwColor);
}
