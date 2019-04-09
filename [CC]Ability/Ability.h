// Ability.h: interface for the CAbility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITY_H__802B397D_E65E_4AC7_8917_1679564529B4__INCLUDED_)
#define AFX_ABILITY_H__802B397D_E65E_4AC7_8917_1679564529B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAbilityInfo;
class CAbilityUse;

class CAbility  
{
	CAbilityInfo* m_pAbilityInfo;
	CAbilityUse* m_pAbilityUse;

	BYTE m_Level;
	BOOL m_bActivating;
	POSTYPE m_QuickPosition;

	inline void SetQuickPosition(POSTYPE quickPos) {	m_QuickPosition = quickPos;	}

	friend class CAbilityManager;
public:
	WORD GetIndex();
	BYTE GetLevel();
	WORD GetKind();

	void Init(CAbilityInfo* pInfo, BYTE Level,POSTYPE QuickPos);
	void IncreaseLevel();
	
	CAbilityInfo* GetAbilityInfo()	{	return m_pAbilityInfo;	}
	BOOL IsActivating()		{	return m_bActivating;	}


	inline POSTYPE GetQuickPosition()	{	return m_QuickPosition;	}
		
	void Use();
	
	void AddLevel( int Level )			{	m_Level += Level;	}
	void DecreaseLevel();
};

#endif // !defined(AFX_ABILITY_H__802B397D_E65E_4AC7_8917_1679564529B4__INCLUDED_)
