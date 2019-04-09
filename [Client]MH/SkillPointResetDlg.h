// SkillPointResetDlg.h: interface for the CSkillPointResetDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILL_POINT_RESET_DLG)
#define AFX_SKILL_POINT_RESET_DLG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "IndexGenerator.h"

class cButton;
class cPushupButton;
class cIconGridDialog;
class CAbilityIcon;
class CAbilityInfo;

enum
{
	eSR_Tab_War=0,
	eSR_Tab_KyungGong,
	eSR_Tab_Character,
	eSR_Tab_Max,
};

class CSkillPointResetDlg : public cDialog
{
	cButton*			m_ResetBtn;
	cButton*			m_OkBtn;
	cPushupButton*		m_GridButton[eSR_Tab_Max];
	cIconGridDialog*	m_IconGrid[eSR_Tab_Max];
	CIndexGenerator		m_IconIndexCreator;

	WORD				m_wShopItemPos;
	DWORD				m_dwCurTabNum;

public:
	CSkillPointResetDlg();
	virtual ~CSkillPointResetDlg();

	virtual void SetActive( BOOL val );
	void Linking();
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);

	void RefreshAbilityIcons();
	CAbilityIcon* MakeNewAbilityIcon( CAbilityInfo* pInfo );
	void SetAbilityToolTip(CAbilityIcon* pIcon);
	void SetTabNumber( DWORD dwTab );
	DWORD GetTabNumber()		{	return m_dwCurTabNum;	}
	void SetAbilityResetSyn();
	void SetShopItemPos(WORD wPos)	{	m_wShopItemPos = wPos;	}
	char* GetCurAbilityName();
};


#endif