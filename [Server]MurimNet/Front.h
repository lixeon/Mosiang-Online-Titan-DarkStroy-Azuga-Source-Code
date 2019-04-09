// Front.h: interface for the CFront class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRONT_H__DD20E09D_2897_45EB_89DF_EDA9B79D252F__INCLUDED_)
#define AFX_FRONT_H__DD20E09D_2897_45EB_89DF_EDA9B79D252F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <HashTable.h>


class CMNPlayer;


class CFront  
{
protected:

	CYHHashTable<CMNPlayer>		m_htPlayer;

	int		m_nTotalPlayer;
	DWORD	m_dwMaxPlayer;

public:
	CFront();
	virtual ~CFront();

	void Init( DWORD dwMaxPlayer );

	void AddPlayer( CMNPlayer* pPlayer, DWORD dwKey );
	void DeletePlayer( DWORD dwKey );
	void DeleteAllPlayers();
};

#endif // !defined(AFX_FRONT_H__DD20E09D_2897_45EB_89DF_EDA9B79D252F__INCLUDED_)
