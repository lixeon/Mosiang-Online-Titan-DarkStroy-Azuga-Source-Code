// NewbieGuide.h: interface for the CNewbieGuide class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWBIEGUIDE_H__8B2BB818_492C_4894_A63F_008EB86C2D84__INCLUDED_)
#define AFX_NEWBIEGUIDE_H__8B2BB818_492C_4894_A63F_008EB86C2D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"
#include "..\[CC]Quest\QuestDefines.h"

class CMHFile;

struct NEWBIEDATA
{
	DWORD	m_dwKind;
	DWORD	m_dwParam;
	DWORD	m_dwPage;
	DWORD	m_dwBitFlag;
	NEWBIEDATA() : m_dwKind(0), m_dwParam(0), m_dwPage(0), m_dwBitFlag(0)	{}
};

class CNewbieGuide  
{
protected:
	CYHHashTable<NEWBIEDATA>	m_NewbieData;

	DWORD	m_dwFlag1;
	DWORD	m_dwFlag2;
	DWORD	m_dwFlag3;	

public:
	CNewbieGuide();
	virtual ~CNewbieGuide();

	void	LoadNewbieGuide( char* filename );
	NEWBIEDATA*	LoadNewbieGuideData( CMHFile* pfile, DWORD dwBitFlag );

	void	SetFlagData( DWORD dwFlag1, DWORD dwFlag2, DWORD dwFlag3 )	{ m_dwFlag1 = dwFlag1; m_dwFlag2 = dwFlag2; m_dwFlag3 = dwFlag3; }
	void	SetFlag( DWORD dwBit );
	BOOL	IsComplete( DWORD dwBit );

	void	CheckEvent( GAMEEVENT* pGE );
	void	CheckLevelUp( DWORD dwLevel );
	void	CheckMapChange( DWORD dwMapNum );
	void	CheckAbility( DWORD dwAbilityNum );
	void	CheckElse( DWORD dwEventKind );	
};

#endif // !defined(AFX_NEWBIEGUIDE_H__8B2BB818_492C_4894_A63F_008EB86C2D84__INCLUDED_)
