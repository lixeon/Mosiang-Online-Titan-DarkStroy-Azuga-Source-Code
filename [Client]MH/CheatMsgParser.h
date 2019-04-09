// CheatMsgParser.h: interface for the CCheatMsgParser class.
//
//////////////////////////////////////////////////////////////////////


//#ifdef _CHEATENABLE_ //KES CHEAT!!!

#if !defined(AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_)
#define AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
struct EVENTITEM
{
	char	strName[20];
};
*/

#include "Object.h"

#define CHEATMGR USINGTON(CCheatMsgParser)
class CCheatMsgParser  
{
	BOOL m_bCheatEnable;
	CObject* m_pTargetObj;
//	EVENTITEM	m_eventitem[10][20];
//	WORD		m_itemCount[10];
//	BOOL m_bEvent;

	//SW051011 ����/��ȭ ġƮ
	//BOOL	m_bItCantBeRare;
public:
	enum{ eKindOption_None=0, eKindOption_Reinforce=1, eKindOption_Rare=2, };
	WORD	m_wItemOptionKind;
	WORD	m_wTargetItemIdx;
	POSTYPE	m_ItemAbsPos;
	WORD	m_OptionCount;
	ITEM_OPTION_INFO	m_OptionInfo;
	BOOL	m_bReadyToOptionCheat;

//	//MAKESINGLETON(CCheatMsgParser);

	CCheatMsgParser();
	virtual ~CCheatMsgParser();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,MSGBASE* pMsg);

	BOOL IsCheatEnable() { return m_bCheatEnable; }
	void SetCheatEnable( BOOL bCheat ) { m_bCheatEnable = bCheat; }

	void SetCheatTargetObject(CObject* pObj)	{	m_pTargetObj = pObj;	}
	CObject* GetCheatTargetObject()	{ return m_pTargetObj;	}
	// ?Ìº¥Æ®·Î ÇÔ¼ö Ãß±â
//	void LoadItem();
//	void EventMonsterRegen(WORD type, WORD count = 30);
//	WORD ExtractItem(WORD idx);
//	VECTOR3 GetRegenPos();
};

#ifdef _CHEATENABLE_
BOOL CheatFunc(char* cheat);
#endif

EXTERNGLOBALTON(CCheatMsgParser)

#endif // !defined(AFX_CHEATMSGPARSER_H__F7D74F56_32EC_4F80_B767_348E65ACC54C__INCLUDED_)
//#endif //_CHEATENABLE_	//KES CHEAT!!!