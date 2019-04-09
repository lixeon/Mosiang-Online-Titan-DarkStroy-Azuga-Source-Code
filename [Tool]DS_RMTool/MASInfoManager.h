// MASInfoManager.h: interface for the CMASInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASINFOMANAGER_H__8ADF0484_BD0B_4161_863E_D0555C2AA0FC__INCLUDED_)
#define AFX_MASINFOMANAGER_H__8ADF0484_BD0B_4161_863E_D0555C2AA0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include ".\common\Commondefine.h"

#define MASINFOMGR USINGTON(CMASInfoManager)

class CMASInfoManager  
{
protected:
	MASINFO		m_MASInfo[10];
	int			m_nCurMasInfo;
	int			m_nMaxMasInfo;

public:
	CMASInfoManager();
	virtual ~CMASInfoManager();

	BOOL	ReadMasInfo( char* filename );

	int			GetMaxMas()					{ return m_nMaxMasInfo; }
	MASINFO*	GetMasInfo( int index )		{ return &m_MASInfo[index]; }
	MASINFO*	GetCurMasInfo()				{ return &m_MASInfo[m_nCurMasInfo]; }
	void		SetCurMasInfo( int index )	{ m_nCurMasInfo = index; }
};

EXTERNGLOBALTON(CMASInfoManager)

#endif // !defined(AFX_MASINFOMANAGER_H__8ADF0484_BD0B_4161_863E_D0555C2AA0FC__INCLUDED_)
