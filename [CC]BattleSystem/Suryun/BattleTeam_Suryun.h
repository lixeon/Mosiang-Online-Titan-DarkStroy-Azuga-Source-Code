// BattleTeam_Suryun.h: interface for the CBattleTeam_Suryun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETEAM_SURYUN_H__91CDE3A2_3517_4218_96E8_6B979D49DE3C__INCLUDED_)
#define AFX_BATTLETEAM_SURYUN_H__91CDE3A2_3517_4218_96E8_6B979D49DE3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\BattleTeam.h"

class CBattleTeam_Suryun_Player : public CBattleTeam  
{
public:
	CBattleTeam_Suryun_Player();
	virtual ~CBattleTeam_Suryun_Player();

	virtual BOOL IsAddableTeamMember(CObject* pObject);
};

class CBattleTeam_Suryun_Monster : public CBattleTeam  
{
public:
	CBattleTeam_Suryun_Monster();
	virtual ~CBattleTeam_Suryun_Monster();

	virtual BOOL IsAddableTeamMember(CObject* pObject);
};

#endif // !defined(AFX_BATTLETEAM_SURYUN_H__91CDE3A2_3517_4218_96E8_6B979D49DE3C__INCLUDED_)
