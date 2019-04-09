#ifndef _GAMEEVENT_MANAGER_
#define _GAMEEVENT_MANAGER_

////////////////////////////////////////////////////////////////
// 1. ÀÌº¥Æ®¿¡ µ¥ÀÌÅÍ´Â µ¿ÀûÀ¸·Î ¸¸µé°Í.
// 2. ÇÁ·Î¼¼½ºÀÇ ÃÖÀûÈ­¸¦ °í·Á. (ºÎÈ­ÀÇ ÃÖ¼ÒÈ­)
// 3. ¼­¹ö¿ÍÀÇ ¿¬µ¿ °í·Á.
// 4. Ã³¸® ·çÆ¾ÀÇ ½ºÅ©¸³Æ®È­.
// 5. File·ÎÀÇ ÀúÀå°ú DB·ÎÀÇ ÀúÀåÀ» µ¿½Ã¿¡ »ý°¢.
// 6. 1Â÷´Â ¼­¹ö·ÎÀÇ ´Ü¹æÇâ ¸Þ¼¼Áö Àü´Þ. (NotifyÇü½Ä)
// 7. ÃßÈÄ ¼­¹ö¿¡¼­ CheckÇØ¾ßÇÒ ÀÌº¥Æ®¿¡ ´ëÇØ¼­ Á¤¸®.
////////////////////////////////////////////////////////////////

#define MAX_EVENTCOUNT	100

#define GAMEEVENTMGR USINGTON(CGameEventManager)

class CGameEventManager {

	GAMEEVENT	m_EventPool[MAX_EVENTCOUNT];
	WORD		m_wEventCount;

public:
	CGameEventManager();
	virtual ~CGameEventManager();

	void Process();
	void AddEvent( DWORD event, DWORD dwData = 0);
};	

EXTERNGLOBALTON(CGameEventManager)


#endif //_GAMEEVENT_MANAGER_