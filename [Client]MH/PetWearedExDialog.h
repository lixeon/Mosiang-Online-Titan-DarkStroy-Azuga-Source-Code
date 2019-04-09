#pragma once
//#include "d:\darkstory\main_dev\[client]mh\interface\cicondialog.h"
#include "./Interface/cIconDialog.h"

class CPetWearedExDialog : public cIconDialog
{
public:
	CPetWearedExDialog(void);
	virtual ~CPetWearedExDialog(void);

	BOOL AddItem(WORD relPos, cIcon * InIcon);
	BOOL DeleteItem(WORD relPos, cIcon ** outIcon);

	BOOL GetBlankPositionRestrictRef(WORD & absPos);

	BOOL CheckDuplication(DWORD ItemIdx);
};
