// QuestUpdater.h: interface for the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUPDATER_H__E3AC5CA7_B411_4A0C_813D_E8F5F1F47628__INCLUDED_)
#define AFX_QUESTUPDATER_H__E3AC5CA7_B411_4A0C_813D_E8F5F1F47628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQuest;
class CPlayer;

class CQuestUpdater  
{
public:
	CQuestUpdater();
	virtual ~CQuestUpdater();
	
	void	StartQuest( CPlayer* player, CQuest* pQuest );
	void	EndQuest( CPlayer* player, CQuest* pQuest );
	void	DeleteQuest( CPlayer* player, CQuest* pQuest );
	void	UpdateQuest( CPlayer* player, DWORD dwFlag, DWORD dwParam, DWORD dwTime );

	void	StartSubQuest( CPlayer* player, CQuest* pQuest, DWORD sIndex );
	void	EndSubQuest( CPlayer* player, CQuest* pQuest, DWORD sIndex );
	void	UpdateSubQuest( CPlayer* player, CQuest* pQuest, DWORD sIndex );

	void	GiveQuestItem( CPlayer* pPlayer, DWORD dwItemIdx, DWORD dwItemNum );
	void	TakeQuestItem( CPlayer* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum );
	void	UpdateQuestItem( CPlayer* pPlayer, DWORD dwQuestIdx, DWORD dwItemIdx, DWORD dwItemNum );

	void	UpdateQuestCheckTime( CPlayer* player, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );
};


#endif // !defined(AFX_QUESTUPDATER_H__E3AC5CA7_B411_4A0C_813D_E8F5F1F47628__INCLUDED_)
