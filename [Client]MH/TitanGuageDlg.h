// TitanGuageDlg.h: interface for the CTitanGuageDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TITANGUAGEDLG_H)
#define TITANGUAGEDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "ObjectGuagen.h"

class cStatic;

enum eTitanGuage
{
	eTITAN_GuageHp,
	eTITAN_GuageMp,
	eTITAN_GuadgeMax,
};

class CTitanGuageDlg : public cDialog
{
    //CObjectGuagen* m_TitanGuage[eTITAN_GuadgeMax];
	//SW070912 ±‚»π∫Ø∞Ê
	CObjectGuagen* m_TitanGuage;
	cStatic*	m_pHpPercent;
	//cStatic*	m_pMpPercent;	

public:
	CTitanGuageDlg();
	virtual ~CTitanGuageDlg();

	void Linking();
	virtual void SetActive( BOOL val );
	static BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	void SetLife(DWORD dwLife);
	void SetNaeRyuk(DWORD dwNaeRyuk);
};

#endif // !defined(TITANGUAGEDLG_H)
