// MNPlayer.h: interface for the CMNPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNPLAYER_H__217F89FE_F8ED_4F12_B34A_436AC3525DDA__INCLUDED_)
#define AFX_MNPLAYER_H__217F89FE_F8ED_4F12_B34A_436AC3525DDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMNPlayer  
{

protected:
	
	MNPLAYER_BASEINFO	m_MNPlayerInfo;

	BOOL	m_bCaptain;
	
public:
	CMNPlayer();
	virtual ~CMNPlayer();
	void Init( MNPLAYER_BASEINFO* pInfo );
	void Release();
	
	void SetMNPlayerInfo( MNPLAYER_BASEINFO* pInfo )	{ m_MNPlayerInfo = *pInfo; }
	void SetTeam( BYTE cbTeam )						{ m_MNPlayerInfo.cbTeam = cbTeam; }

	MNPLAYER_BASEINFO* GetBaseInfo()	{ return &m_MNPlayerInfo; }
	DWORD		GetID()		{ return m_MNPlayerInfo.dwObjectID; }
	LEVELTYPE	GetLevel()	{ return m_MNPlayerInfo.Level; }
	char*		GetName()	{ return m_MNPlayerInfo.strPlayerName; }
	BYTE		GetTeam()	{ return m_MNPlayerInfo.cbTeam; }

	void		SetCaptain( BOOL bCaptain ) { m_bCaptain = bCaptain; }
	BOOL		IsCaptain()					{ return m_bCaptain; }
};

#endif // !defined(AFX_MNPLAYER_H__217F89FE_F8ED_4F12_B34A_436AC3525DDA__INCLUDED_)
