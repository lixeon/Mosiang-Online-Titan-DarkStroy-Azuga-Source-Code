// QuestLimitKind_Time.h: interface for the QuestLimitKind_Time class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTLIMITKIND_TIME_H__A0F76E77_B0F6_4CD8_BD91_A56EA3F4A0F4__INCLUDED_)
#define AFX_QUESTLIMITKIND_TIME_H__A0F76E77_B0F6_4CD8_BD91_A56EA3F4A0F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuestLimitKind.h"

class QuestLimitKind_Time : public CQuestLimitKind
{
public:
	QuestLimitKind_Time( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx );
	virtual ~QuestLimitKind_Time();

};

#endif // !defined(AFX_QUESTLIMITKIND_TIME_H__A0F76E77_B0F6_4CD8_BD91_A56EA3F4A0F4__INCLUDED_)
