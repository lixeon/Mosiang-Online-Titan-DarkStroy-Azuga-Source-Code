// GuildMark.h: interface for the CGuildMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMARK_H__452B0AB3_F6FD_4A8C_BBA5_6663675C5FFD__INCLUDED_)
#define AFX_GUILDMARK_H__452B0AB3_F6FD_4A8C_BBA5_6663675C5FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuildMarkImg;
class CGuildMark  
{
	CGuildMarkImg* m_pMarkImg;


public:
	CGuildMark();
	virtual ~CGuildMark();
	
	void Clear();

	void SetGuildMarkImg(CGuildMarkImg* pMarkImg);
	void Render(VECTOR2* pos, DWORD dwColor = 0xffffffff);
};

#endif // !defined(AFX_GUILDMARK_H__452B0AB3_F6FD_4A8C_BBA5_6663675C5FFD__INCLUDED_)
