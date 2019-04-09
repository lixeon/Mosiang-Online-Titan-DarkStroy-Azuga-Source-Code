#ifndef _CLASS_QUEST_BASE_
#define _CLASS_QUEST_BASE_

// Äù½ºÆ®¿¡ °üÇÑ ±âº»Å¬·¡½º

// ¼­¹ö·Î ºÎÅÍ »óÅÂ°ªÀ» ¹Þ´Â´Ù.
// Manager·Î ºÎÅÍ ÀÐ¾îµéÀÎ ±âº» ·çÆ¾À» º¹»çÇÑ´Ù.

class CQuestBase {
protected:
	DWORD		m_dwQuestIdx;	
	QFLAG		m_State;		// ÁøÇà »óÅÂ°ª 

	int			m_nValidNum;	// »ç¿ë bit °¹¼ö  (¿Ï·á¸¦ ÆÇ´ÜÇÒ¶§ »ç¿ë)

public:
	CQuestBase();
	virtual ~CQuestBase();

	void Init(DWORD idx, QSTATETYPE state);
	void SetValidbitNum(int num) { m_nValidNum = num; }

	virtual void CheckCondition(GAMEEVENT* event) { return; }

	DWORD GetQuestIdx() { return m_dwQuestIdx; }

	virtual void SetValue(QSTATETYPE value);
	virtual void SetState(BYTE field);
	
	BOOL IsComplete() { return m_State.IsSet(1); }
};

#endif // _CLASS_QUEST_BASE_