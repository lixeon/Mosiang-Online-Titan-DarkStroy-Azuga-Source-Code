// QuestMapMgr.h: interface for the CQuestMapMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTMAPMGR_H__18C47361_9AB2_4891_86DC_3018AB3C6188__INCLUDED_)
#define AFX_QUESTMAPMGR_H__18C47361_9AB2_4891_86DC_3018AB3C6188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		QUESTMAPNUM1		73
#define		QUESTMAPNUM2		37
#define		QUESTMAPNUM3		95

#define QUESTMAPMGR	USINGTON(CQuestMapMgr)

class CPlayer;

class CQuestMapMgr  
{
	BOOL	m_bQuestMap;
	
public:
	CQuestMapMgr();
	virtual ~CQuestMapMgr();

	BOOL	Init( WORD wMapNum );
	void	RemovePlayer( CPlayer* pPlayer );
	void	DiePlayer( CPlayer* pPlayer );

	BOOL	IsQuestMap()	{ return m_bQuestMap; }
};

EXTERNGLOBALTON(CQuestMapMgr);

#endif // !defined(AFX_QUESTMAPMGR_H__18C47361_9AB2_4891_86DC_3018AB3C6188__INCLUDED_)
