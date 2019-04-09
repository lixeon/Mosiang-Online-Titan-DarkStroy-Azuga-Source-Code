// AIParam.h: interface for the CAIParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIPARAM_H__FF49E282_44D5_4CDB_AD3A_AE8E4119A9A6__INCLUDED_)
#define AFX_AIPARAM_H__FF49E282_44D5_4CDB_AD3A_AE8E4119A9A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Header\GameResourceStruct.h"
#include "StateParam.h"

class AIPARAM
{
public:
	AIPARAM():
	AttackStartTime				(0),
	CurAttackKind				(0),				// ¼±ÅÃÇÑ °ø°Ý Á¾·ù(·£´ý È¤Àº ÆÐÅÏ)
	CurAttackPatternNum			(0),
	CurAttackPatternIndex		(0),
	SearchLastTime				(0),
	CollSearchLastTime			(0),
	RunawayType					(eRunawayType_None),
	pTarget						(NULL),
	pHelperMonster				(NULL),
	PursuitForgiveStartTime		(0),
	prePursuitForgiveTime		(0)
	{}

	DWORD	AttackStartTime;
	DWORD	SearchLastTime;
	DWORD	CollSearchLastTime;
	
	DWORD	CurAttackKind;		// ¼±ÅÃÇÑ °ø°Ý Á¾·ù(0,1,2~~~)

	WORD	CurAttackPatternNum;
	WORD	CurAttackPatternIndex;

	DWORD	PursuitForgiveStartTime;
	
	//runaway
	DWORD		prePursuitForgiveTime;
	WORD		RunawayType;
	CMonster *  pHelperMonster;
	CPlayer *	pTarget;


};

class CAIParam  : public AIPARAM
{
public:
	CAIParam();
	virtual ~CAIParam();
	
	void Init();

	DWORD GetCurAttackKind();
	//DWORD RandCurAttackKind(CMonster * pMonster);
	void SetCurrentAttackPattern(WORD w)
	{
		CurAttackPatternNum = w;
		CurAttackPatternIndex = 0;
	}
};

#endif // !defined(AFX_AIPARAM_H__FF49E282_44D5_4CDB_AD3A_AE8E4119A9A6__INCLUDED_)
