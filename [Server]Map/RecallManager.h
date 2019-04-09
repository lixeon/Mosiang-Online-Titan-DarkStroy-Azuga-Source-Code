// RecallManager.h: interface for the CRecallManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECALLMANAGER_H__917D8CAE_02EC_42B5_BE06_00B1EC0CE825__INCLUDED_)
#define AFX_RECALLMANAGER_H__917D8CAE_02EC_42B5_BE06_00B1EC0CE825__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RECALLMGR	CRecallManager::GetInstance()

#include "..\[CC]Header\GameResourceStruct.h"

class CObject;
class CMonster;
class CRecallManager  
{
	enum
	{
		RECALL_NONE						= 0,
		RECALL_GROUP					= 1,
		RECALL_RECALLERRANGE			= 2,
		RECALL_RECALLERPOSITION			= 3,
		RECALL_TARGETRANGE				= 4,
	};

public:
	GETINSTANCE(CRecallManager)
	CRecallManager();
	virtual ~CRecallManager();

	//es
	void EventRecall(CObject * pRecaller, WORD MonsterKind, BYTE cbMonsterCount, BYTE cbChannel, VECTOR3* pPos, WORD wRadius, WORD DropItemID, DWORD dwDropRatio );
	void DeleteEventRecallMonster();
	//yh
//	void Recall(CObject * pRecaller, DWORD MonsterKind, WORD DropItemID, VECTOR3* pPos, BOOL bBoss);

	void RecallObject(CMonster * pRecaller, RECALLINFO * pRecallInfo);

	int RecallRange(CObject* pRecaller, DWORD dwGridID, VECTOR3 * vPos,  WORD wRecallMonsterKind, DWORD wRecallNum, DWORD dwRange, int GroupID,BOOL bRandomPos = TRUE, BOOL bEventRegen = FALSE, WORD ObjectKind = 32 /*eObjectKind_Monster */);

	// quest
	void	QuestRecall( WORD wMonsterKind, BYTE bMonsterCount, DWORD dwChannel, VECTOR3* pMonsterPos, WORD wRadius );
	void	DeleteQuestRecallMonster( DWORD dwChannel );
};

#endif // !defined(AFX_RECALLMANAGER_H__917D8CAE_02EC_42B5_BE06_00B1EC0CE825__INCLUDED_)
