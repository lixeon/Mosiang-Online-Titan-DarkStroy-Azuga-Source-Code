// CharacterCalcManager.h: interface for the CCharacterCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERCALCMANAGER_H__BFD2AD23_C5F1_4A36_A09F_8DF40484B6CB__INCLUDED_)
#define AFX_CHARACTERCALCMANAGER_H__BFD2AD23_C5F1_4A36_A09F_8DF40484B6CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHARCALCMGR USINGTON(CCharacterCalcManager)

class CCharacterCalcManager  
{
	
public:	
//	//MAKESINGLETON(CCharacterCalcManager)
	CCharacterCalcManager();
	virtual ~CCharacterCalcManager();
	
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};
EXTERNGLOBALTON(CCharacterCalcManager)
#endif // !defined(AFX_CHARACTERCALCMANAGER_H__BFD2AD23_C5F1_4A36_A09F_8DF40484B6CB__INCLUDED_)
