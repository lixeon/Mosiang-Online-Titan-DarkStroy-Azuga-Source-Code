// GuildUnionMarkMgr.h: interface for the CGuildUnionMarkMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDUNIONMARKMGR_H__8B6D20A4_8FC5_442D_8C64_E01BAFBBBE57__INCLUDED_)
#define AFX_GUILDUNIONMARKMGR_H__8B6D20A4_8FC5_442D_8C64_E01BAFBBBE57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cImageSelf.h"

class CGuildUnionMark
{
protected:
	cImageSelf	m_Img;
	BOOL		m_bLoaded;
public:
	CGuildUnionMark();
	virtual ~CGuildUnionMark();

	BOOL	LoadNewGuildUnionMark( char* pName );
	void	UpdateGuildUnionMark( char* pName );
	void	Render( VECTOR2* pos, DWORD dwColor = 0xffffffff );
};


#define GUILDUNIONMARKMGR USINGTON(CGuildUnionMarkMgr)

class CGuildUnionMarkMgr  
{
protected:
	CYHHashTable<CGuildUnionMark>	m_MarkTable;

	CGuildUnionMark*	LoadNewGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx );

public:
	CGuildUnionMarkMgr();
	virtual ~CGuildUnionMarkMgr();

	CGuildUnionMark*	GetGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx );
	BOOL	RegistGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx, char* pBmpName );
	void	SaveGuildUnionMark( int nServerSetNum, DWORD dwGuildUnionIdx, DWORD dwMarkIdx, char* pImgData );

};

EXTERNGLOBALTON(CGuildUnionMarkMgr)

#endif // !defined(AFX_GUILDUNIONMARKMGR_H__8B6D20A4_8FC5_442D_8C64_E01BAFBBBE57__INCLUDED_)
