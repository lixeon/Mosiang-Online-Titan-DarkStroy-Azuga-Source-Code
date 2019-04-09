#pragma once
#include ".\interface\cicondialog.h"

class CMugongBase;
class CItem;
class cStatic;

class CSkillOptionChangeDlg :
	public cIconDialog
{
	cIconDialog * m_pMugongIconDlg;
	cIconDialog * m_pItemIconDlg;

	cStatic* m_pOption[5];

public:
	CSkillOptionChangeDlg(void);
	~CSkillOptionChangeDlg(void);

	void Linking();
	BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);

	void OnActionEvnet(LONG lId, void * p, DWORD we);

	CMugongBase* GetMugong() { return (CMugongBase*)(m_pMugongIconDlg->GetIconForIdx(0)); }
	CItem* GetItem() { return (CItem*)(m_pItemIconDlg->GetIconForIdx(0)); }

	void SetActive(BOOL val);
	void SetOptionToolTip();
};
