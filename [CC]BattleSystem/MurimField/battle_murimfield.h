#ifndef _BATTLE_MURIMFIELD_H
#define _BATTLE_MURIMFIELD_H

#include "battle.h"

#ifndef _MAPSERVER_
#include "../ImageNumber.h"
#endif

class CBattleTeam_MurimField;

class CBattle_MurimField : public CBattle
{
protected:
	MURIMFIELD_BATTLE_INFO	m_MurimBattleInfo;

protected:
	CBattleTeam_MurimField* GetTeam( int Team )
	{
		ASSERT(Team < eBattleTeam_Max);
		return (CBattleTeam_MurimField*)m_TeamArray[Team];
	}
	int GetOtherTeamNum( int Team )
	{
		ASSERT(Team < eBattleTeam_Max);
		return Team == eBattleTeam1 ? eBattleTeam2 : eBattleTeam1;
	}

public:
	CBattle_MurimField();
	virtual ~CBattle_MurimField();

public:	// virtual func
	// Àû,¾Æ±º ±¸º°
	virtual BOOL	IsEnemy( CObject* pOperator, CObject* pTarget );
	virtual BOOL	IsFriend( CObject* pOperator, CObject* pTarget );
	
	// event func
	virtual void	OnCreate( BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2 );
	virtual void	OnFightStart();
	virtual void	OnDestroy();
	virtual void	OnTeamMemberAdd( int Team, DWORD MemberID, char* Name );
	virtual BOOL	OnTeamMemberDelete( int Team, DWORD MemberID, char* Name );
	virtual BOOL	OnTeamMemberDie( int Team, DWORD VictimMemberID, DWORD KillerID );
	virtual void	OnTeamMemberRevive( int Team, CObject* pReviver );
	virtual void	OnTeamObjectCreate( int Team, CBattleObject* pBattleObject );
	virtual void	OnTeamObjectDestroy( int Team, CBattleObject* pBattleObject );

	virtual void	OnTick();

	// ½ÂÆÐ ÆÇÁ¤
	virtual BOOL	Judge();
	virtual void	Victory( int WinnerTeamNum, int LoserTeamNum );
	virtual void	Draw();
	void			Result();

public:	// °ÔÀÓ ¹æ½Ä¿¡ µû¸¥ ½ÂÆÐ ÆÇ´Ü ¹× º¸»ó Ã³¸®( ¶Ç ´Ù¸¥ Å¬·¡½º·Î ¸¸µé¾î¾ß ÇÒ±î? )
	BOOL	Judge_MaxKill();
	BOOL	Judge_TimeLimit();

	BOOL	Result_Money();	

public:
#ifdef _MAPSERVER_	// Battle Á¤º¸ °ü·Ã ---------------------
	virtual void	GetBattleInfo( char* pInfo, WORD* size );
#endif

#ifdef _MHCLIENT_	// Render	(Å¬¶óÀÌ¾ðÆ®¸¸ »ç¿ëÇÔ) -------

protected:
	CImageNumber	m_ImageNumber;		// ½Ã°£ Ç¥½Ã
	cImage			m_ImageReady;
	cImage			m_ImageFight;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;

	VECTOR2			m_vImagePos;
	VECTOR2			m_vImageScale;

public:
	virtual void	Render();

#endif
	
};


#endif