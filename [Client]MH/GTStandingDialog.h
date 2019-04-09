// GTStandingDialog.h: interface for the CGTStandingDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTSTANDINGDIALOG_H__FE986491_5B8E_40B8_BFE1_A7799EC00F9A__INCLUDED_)
#define AFX_GTSTANDINGDIALOG_H__FE986491_5B8E_40B8_BFE1_A7799EC00F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"


#define MAXGROUP			5
#define STATICCOUNT			29
#define GUILDINFOCOUNT		8
#define STATICCOUNT_SF		28
#define GUILDINFOCOUNT_SF	4
#define NOMARKGUILDIDX		9999999


enum
{
	eGTPAGEVIEW_SEMIFANAL=0,
	eGTPAGEVIEW_AGROUP,
	eGTPAGEVIEW_BGROUP,
	eGTPAGEVIEW_CGROUP,
	eGTPAGEVIEW_DGROUP,
};



class cPushupButton;
class cStatic;
class CGuildMark;


class CGTStandingDialog : public cDialog
{
	cStatic*			m_pMainBase[2];			// 0-본선, 1-예선
	cPushupButton*		m_pGroupBtn[MAXGROUP];
	cStatic*			m_pGuildMark[GUILDINFOCOUNT+1];
	cStatic*			m_pGuildName[GUILDINFOCOUNT];
	cStatic*			m_pLineStatic[STATICCOUNT];

	cStatic*			m_pSFGuildMark[GUILDINFOCOUNT_SF+1];
	cStatic*			m_pSFGuildName[GUILDINFOCOUNT_SF];

	REGISTEDGUILDINFO	m_GuildInfo[MAXGUILD_INTOURNAMENT];
	DWORD				m_dwCurPage;			// 0-본선, 1-A, 2-B, 3-C, 4-D
	DWORD				m_CurTournament;
	CGuildMark*			m_pGuildMarkImage[GUILDINFOCOUNT+1];
	CGuildMark*			m_pNonGuildMark;

	//
	void DrawRankingLine16( REGISTEDGUILDINFO GuildInfo );
	void DrawRankingLine8( REGISTEDGUILDINFO GuildInfo );
	void DrawRankingLine4( REGISTEDGUILDINFO GuildInfo );
	void DrawRankingLine2( REGISTEDGUILDINFO GuildInfo );
	void DrawRankingLine1( REGISTEDGUILDINFO GuildInfo );
	void AddGuildMarkName( REGISTEDGUILDINFO GuildInfo, BOOL bWin );
public:
	CGTStandingDialog();
	virtual ~CGTStandingDialog();

	void Linking();
	void SetActive( BOOL val );
	void Render();

	void RefreshGuildInfo();
	void SetCurTournament( DWORD Curtournament )			{	m_CurTournament = Curtournament;	}
	void AddGuildInfo( REGISTEDGUILDINFO* pGuildInfo );

	void SetCurPage( DWORD Page );
	void ResetActive();
	void ResetAll();
};

#endif // !defined(AFX_GTSTANDINGDIALOG_H__FE986491_5B8E_40B8_BFE1_A7799EC00F9A__INCLUDED_)
