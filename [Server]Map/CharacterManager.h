// CharacterManager.h: interface for the CCharacterManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERMANAGER_H__8C3E07DA_7CDF_4332_821E_30AF676DF754__INCLUDED_)
#define AFX_CHARACTERMANAGER_H__8C3E07DA_7CDF_4332_821E_30AF676DF754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHARMGR	USINGTON(CCharacterManager)

class CCharacterManager  
{
public:
	CCharacterManager();
	virtual ~CCharacterManager();

	BOOL ArrangeCharLevelPoint(CPlayer * pPlayer, WORD type);
	BOOL ArrangeCharMinusPoint(CPlayer * pPlayer, WORD type);	
};
EXTERNGLOBALTON(CCharacterManager);
#endif // !defined(AFX_CHARACTERMANAGER_H__8C3E07DA_7CDF_4332_821E_30AF676DF754__INCLUDED_)
