// GuildMark.h: interface for the CGuildMark class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMARK_H__08779C22_8CFD_42F3_B11B_1CCA4DE44632__INCLUDED_)
#define AFX_GUILDMARK_H__08779C22_8CFD_42F3_B11B_1CCA4DE44632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuildMark  
{
	DWORD m_MarkIdx;
	char m_ImgData[GUILDMARK_BUFSIZE];

public:
	CGuildMark();
	virtual ~CGuildMark();
	void Init(MARKNAMETYPE MarkName, char* ImgData);
	char* GetImgData();
};

class CGuildUnionMark
{
protected:
	DWORD	m_dwMarkIdx;
	char	m_ImgData[GUILDUNIONMARK_BUFSIZE];

public:
	CGuildUnionMark();
	virtual ~CGuildUnionMark();

	void	Init( DWORD dwMarkIdx, char* pImgData );
	char*	GetImgData()	{ return m_ImgData;	}
};

#endif // !defined(AFX_GUILDMARK_H__08779C22_8CFD_42F3_B11B_1CCA4DE44632__INCLUDED_)
