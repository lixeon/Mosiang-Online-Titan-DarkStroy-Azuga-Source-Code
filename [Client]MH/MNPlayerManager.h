// MNPlayerManager.h: interface for the CMNPlayerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNPLAYERMANAGER_H__D3119CEB_8001_4D71_AEFC_B683AD465E43__INCLUDED_)
#define AFX_MNPLAYERMANAGER_H__D3119CEB_8001_4D71_AEFC_B683AD465E43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMNPlayer;


class CMNPlayerManager  
{
protected:

	CMemoryPoolTempl<CMNPlayer>*	m_mpPlayerPool;
	CYHHashTable<CMNPlayer>*		m_htPlayer;

	CMNPlayer*					m_pMNHero;
	DWORD						m_dwMNHeroId;

public:

	CMNPlayerManager();
	virtual ~CMNPlayerManager();

	void Init();
	void Release();

	void AddMNPlayer( MNPLAYER_BASEINFO* pInfo );
	void DeleteMNPlayer( DWORD dwObjectID );
	void DeleteMNPlayer( CMNPlayer* pMNPlayer );
	void DeleteAllMNPlayer();

	CMNPlayer* GetMNPlayer( DWORD dwObjectID );

	CMNPlayer*	GetMNHero()						{ return m_pMNHero; }
	DWORD		GetMNHeroID()					{ return m_dwMNHeroId; }
	void		SetMNHero( CMNPlayer* pHero );
};

#endif // !defined(AFX_MNPLAYERMANAGER_H__D3119CEB_8001_4D71_AEFC_B683AD465E43__INCLUDED_)
