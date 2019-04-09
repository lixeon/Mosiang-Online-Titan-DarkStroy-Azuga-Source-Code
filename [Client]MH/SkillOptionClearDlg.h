#pragma once
#include ".\interface\cicondialog.h"

class CItem;

class CSkillOptionClearDlg :
	public cIconDialog
{
	cIconDialog * m_pMugongIconDlg;
	WORD m_ItemPos;
	WORD m_ClearOption;

public:
	CSkillOptionClearDlg(void);
	~CSkillOptionClearDlg(void);

	void Linking();
	BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	void SetActive(BOOL val);
	void SetItem(CItem* pItem);
	void OptionClearSyn();
};
