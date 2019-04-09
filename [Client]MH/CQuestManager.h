#ifndef _QUEST_MANAGER_
#define _QUEST_MANAGER_

// 1. µî·Ï List »ý¼º
// 2. Ãß°¡ ÀÛ¾÷

#define QUESTMGR1 USINGTON(CQuestManager1)

class CQuestBase;

class CQuestManager1 {
//	CMemoryPool<CQuestBase>*		m_QuestPool;
//	CYHHashTable<CQuestBase>*		m_QuestList;

	CYHHashTable<CQuestBase>*		m_QuestList;

public:
	CQuestManager1();
	~CQuestManager1();

	void Init();
	void Release();

	void AddQuest(CQuestBase* pQuest);
	void CheckState(GAMEEVENT* event);

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
};

EXTERNGLOBALTON(CQuestManager1)

#endif //_QUEST_MANAGER_