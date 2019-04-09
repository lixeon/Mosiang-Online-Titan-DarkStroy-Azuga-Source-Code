#ifndef _NEWBIE_GUID_
#define _NEWBIE_GUID_

#include "CQuestBase.h"

//------------------------------
// ³ªÁß¿¡ ½ºÅ©¸³Æ®È­ ½ÃÅ³°Í. (LBS 04.01.06)
// ÇöÀç ÇÏµåÄÚµù ÀÛ¾÷/ Test.
//------------------------------

class CNewbieGuid : public CQuestBase {
protected:

	// ½ÇÇà ÇÔ¼ö (ÀÓ½Ã ÄÚµù)
	void LevelUpCheck(DWORD level);
	void MapChangeCheck(DWORD map);

public:
	CNewbieGuid();
	virtual ~CNewbieGuid();

	virtual void CheckCondition(GAMEEVENT* event);
};


#endif //_NEWBIE_GUID_