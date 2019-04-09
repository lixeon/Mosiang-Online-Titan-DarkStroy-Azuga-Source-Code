#pragma once
#include "d:\darkstory\main_dev\[client]mh\interface\cdialog.h"

class CGuildInvitationKindSelectionDialog :	public cDialog
{
public:
	CGuildInvitationKindSelectionDialog(void);
	virtual ~CGuildInvitationKindSelectionDialog(void);

	void Linking();
	void OnActionEvent(LONG lId, void* p, DWORD we);

};
