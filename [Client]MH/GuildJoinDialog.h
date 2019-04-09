#pragma once
#include "interface\cdialog.h"

//문파초대선택창. 문파원 or 문하생
class CGuildJoinDialog :	public cDialog
{
public:
	CGuildJoinDialog(void);
	virtual ~CGuildJoinDialog(void);

	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);

};
