// SuryunDialog.h: interface for the CSuryunDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SURYUNDIALOG_H__055D7AB6_A44C_44C5_91AF_97D530258CCA__INCLUDED_)
#define AFX_SURYUNDIALOG_H__055D7AB6_A44C_44C5_91AF_97D530258CCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include "AbilityInfo.h"
#include "IndexGenerator.h"

#include "AbilityIcon.h"

class cStatic;
class cButton;

class CSuryunDialog  : public cTabDialog
{
	POSTYPE m_CurPosition;
	CIndexGenerator m_IconIndexCreator;
	cStatic* m_pSuryunExpStc;
	cStatic* m_pNeedSuryunExpStc;
	DWORD m_SelectedAbilityIDX;
	cButton* m_pSkRedistBtn;

public:
	CSuryunDialog();
	virtual ~CSuryunDialog();
	
	virtual void SelectTab(BYTE idx);

	virtual void SetActive( BOOL val );
	virtual void Add(cWindow * window);
	void Linking();
	
	CAbilityIcon* GetAbilityIcon(POSTYPE pos);
	CAbilityIcon* GetAbilityIconAbs(POSTYPE AbsPos);
	
	void LearnAbilitySyn();
	
	CAbilityIcon* AddAbility(int State, BYTE Level, CAbilityInfo* pInfo);
	void UpdateAbility(int State,BYTE Level,CAbilityInfo* pInfo);
	CAbilityIcon* MakeNewAbility(CAbilityInfo* pInfo,int State,BYTE Level);
	cImage * GetIconImage(WORD Ability2DNum, cImage * pImageGet);
	void SetAbilityToolTip(CAbilityIcon* pIcon);

	void SetAbilityExp(DWORD Exp);
	
	void SetSelectAbilityIDX(DWORD id) { m_SelectedAbilityIDX = id;	}
	DWORD GetSelectAbility()			{ return m_SelectedAbilityIDX;	}

	virtual void OnActionEvent(LONG lId, void * p, DWORD we);
	
	void UpdateAbility(CAbilityInfo* pInfo);
	
	void RedistBtnActive();

};

#endif // !defined(AFX_SURYUNDIALOG_H__055D7AB6_A44C_44C5_91AF_97D530258CCA__INCLUDED_)
