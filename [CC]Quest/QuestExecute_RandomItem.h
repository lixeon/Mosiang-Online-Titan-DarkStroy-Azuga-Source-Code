// QuestExecute_RandomItem.h: interface for the CQuestExecute_RandomItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_)
#define AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestExecute.h"

struct RANDOMITEM
{
	WORD	wItemIdx;
	WORD	wItemNum;
	WORD	wPercent;
};

class CQuestExecute_RandomItem : public CQuestExecute   
{
protected:
	DWORD			m_dwMaxItemCount;
	DWORD			m_dwRandomItemCount;
	RANDOMITEM*		m_pRandomItem;
	
public:
	CQuestExecute_RandomItem( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~CQuestExecute_RandomItem();

	virtual BOOL	Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest );
	RANDOMITEM*		GetRandomItem();
};

#endif // !defined(AFX_QUESTEXECUTE_RANDOMITEM_H__E5FAF1F8_D9F2_4B21_9792_D7F488E99EF6__INCLUDED_)
