// AbilityIcon.h: interface for the CAbilityIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_)
#define AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseItem.h"
class CAbilityInfo;
class CAbility;

class CAbilityIcon : public CBaseItem  
{
	int m_State;
	int m_Level;
	// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
	// 최종레벨에 도달하면 完자를 찍어주어야 한다
	// 렌더할때마다 계산하는거보다 초기화시에 한번만 계산하자
	int m_MaxLevel;
	BOOL m_bAbilityActive;

	CAbilityInfo* m_pAbilityInfo;
	
	cImage m_ScrollImg[3];
	int m_CurScrollImage;
public:
	CAbilityIcon();
	virtual ~CAbilityIcon();
	
	CAbilityInfo* GetAbilityInfo()	{	return m_pAbilityInfo;	}

	void InitAbilityIcon(CAbilityInfo* pInfo);
	void UpdateState(int State,int Level);
	virtual void Render();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);

	virtual inline POSTYPE	GetQuickPosition();
	virtual inline POSTYPE	GetPosition();
	virtual inline void		SetQuickPosition(POSTYPE pos);
	virtual inline WORD		GetItemIdx();
	int GetState() { return m_State; }
	int GetLevel() { return m_Level; }

	void RenderInfo(VECTOR2* pPos);

};

#endif // !defined(AFX_ABILITYICON_H__704928D4_D243_4636_B070_C8ED0B758C66__INCLUDED_)
