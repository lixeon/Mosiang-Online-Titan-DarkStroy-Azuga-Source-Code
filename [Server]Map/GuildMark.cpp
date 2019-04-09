// GuildMark.cpp: implementation of the CGuildMark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMark.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMark::CGuildMark()
{

}

CGuildMark::~CGuildMark()
{

}

void CGuildMark::Init(MARKNAMETYPE MarkName,  char* ImgData)
{
	m_MarkIdx = MarkName;
	memcpy(m_ImgData, ImgData, GUILDMARK_BUFSIZE);
}

char* CGuildMark::GetImgData()
{
	return m_ImgData;
}



CGuildUnionMark::CGuildUnionMark()
{
	m_dwMarkIdx = 0;
	memset( m_ImgData, 0, GUILDUNIONMARK_BUFSIZE );
}

CGuildUnionMark::~CGuildUnionMark()
{
}

void CGuildUnionMark::Init( DWORD dwMarkIdx, char* pImgData )
{
	m_dwMarkIdx = dwMarkIdx;
	memcpy( m_ImgData, pImgData, GUILDUNIONMARK_BUFSIZE );
}