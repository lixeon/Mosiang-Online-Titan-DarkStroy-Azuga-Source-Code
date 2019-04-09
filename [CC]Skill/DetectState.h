#pragma once
#include "..\[cc]skill\specialstate.h"
#include "Object.h"

class CObject;

class CDetectState :
	public CSpecialState
{
public:
	CDetectState(CObject* pParent);
	~CDetectState(void);

	virtual void StartState(DWORD Time,WORD wParam1,WORD wParam2,float fParam3,WORD EffectNum,WORD StateIcon,BOOL bHeroOper = FALSE);
	virtual void EndState();

};
