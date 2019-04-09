// SummonMonster.h: interface for the CSummonMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUMMONMONSTER_H__DD1946BF_6867_4F82_86E3_B149D4058096__INCLUDED_)
#define AFX_SUMMONMONSTER_H__DD1946BF_6867_4F82_86E3_B149D4058096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"

class CSummonMonster : public CMonster  
{
	DWORD m_SummmonerID;
	DWORD m_RegenTime;
	DWORD m_DieTime;

public:
	CSummonMonster();
	virtual ~CSummonMonster();

};

#endif // !defined(AFX_SUMMONMONSTER_H__DD1946BF_6867_4F82_86E3_B149D4058096__INCLUDED_)
