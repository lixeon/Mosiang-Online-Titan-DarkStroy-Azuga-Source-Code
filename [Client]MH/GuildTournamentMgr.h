// GuildTournamentMgr.h: interface for the CGuildTournamentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDTOURNAMENTMGR_H__A01D75E9_DD8E_4792_A7F6_95B0E9A9AE16__INCLUDED_)
#define AFX_GUILDTOURNAMENTMGR_H__A01D75E9_DD8E_4792_A7F6_95B0E9A9AE16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define GTMGR	CGuildTournamentMgr::GetInstance()
#define GTMAPNUM	28


class CPlayer;



class CGuildTournamentMgr  
{

public:
	MAKESINGLETON(CGuildTournamentMgr);
	CGuildTournamentMgr();
	virtual ~CGuildTournamentMgr();

	void AddPlayer( CPlayer* pPlayer );
	void NetworkMsgParse( BYTE Protocol,void* pMsg );
};

#endif // !defined(AFX_GUILDTOURNAMENTMGR_H__A01D75E9_DD8E_4792_A7F6_95B0E9A9AE16__INCLUDED_)
