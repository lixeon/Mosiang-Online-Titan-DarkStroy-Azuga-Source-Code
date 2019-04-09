// GTScoreInfoDailog.h: interface for the CGTScoreInfoDailog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTSCOREINFODAILOG_H__49E37D19_391B_480A_B515_AE1DEA41C09F__INCLUDED_)
#define AFX_GTSCOREINFODAILOG_H__49E37D19_391B_480A_B515_AE1DEA41C09F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"
#include "../ImageNumber.h"


class cStatic;
class cButton;



class CGTScoreInfoDialog : public cDialog
{
	CImageNumber	m_ScoreImage[2];
	CImageNumber	m_EntranceTimeImage;

	cStatic*		m_pGuildName[2];
	cStatic*		m_pGuildScore[2];
	cStatic*		m_pTimeStatic;
	cButton*		m_pOutBtn;
	int				m_Score[2];
	char			m_GuildName[2][MAX_GUILD_NAME+1];
	DWORD			m_FightTime;
	DWORD			m_EntranceTime;
	BOOL			m_bStart;

public:
	CGTScoreInfoDialog();
	virtual ~CGTScoreInfoDialog();

	void Linking();
	void Render();
	void Process();
	
	void SetBattleInfo( SEND_BATTLESCORE_INFO* pInfo );
	void StartBattle()			{	m_bStart = TRUE;	}
	void EndBattle()			{	m_bStart = FALSE;	m_FightTime=0;	}
	
	void SetTeamScore( DWORD Team, DWORD Count )		{	m_Score[Team] = Count;	}
	void ShowOutBtn( BOOL bShow );
};

#endif // !defined(AFX_GTSCOREINFODAILOG_H__49E37D19_391B_480A_B515_AE1DEA41C09F__INCLUDED_)
