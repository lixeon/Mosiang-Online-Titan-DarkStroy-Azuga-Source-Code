// CheckRoutine.h: interface for the CCheckRoutine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_)
#define AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer;
enum eCheckBits{ CB_EXIST = 1, CB_ICONIDX=2, CB_QABSPOS=4, CB_DURA=8, CB_ALL=15, CB_ENOUGHDURA=16, };
#define CHKRT CCheckRoutine::GetInstance()
class CCheckRoutine  
{
public:
	CCheckRoutine();
	virtual ~CCheckRoutine();
	GETINSTANCE(CCheckRoutine)

	BOOL StateOf(CPlayer * pPlayer, DWORD state);
	BOOL MoneyOf(CPlayer * pPlayer, eITEMTABLE tabIdx, MONEYTYPE moneyAmount);
	BOOL LevelOf(CPlayer * pPlayer, LEVELTYPE level);
	BOOL DistanceOf(CPlayer * pPlayer, VECTOR3 *vTPos, DWORD dist);

	// 아이템 기능수행 어빌리티 수련 여부
	BOOL AbilityOf(CPlayer * pPlayer, eABILITY_USE_JOB eKind);
	// 아이템 동일성 체크(위치,존재,아이템정보)
	BOOL ItemOf(CPlayer * pPlayer, POSTYPE AtPos, WORD wItemIdx, DURTYPE Dura, POSTYPE QuickPos, WORD flag = CB_ALL);
	
	BOOL DearlerItemOf(WORD wDealerIdx, WORD wBuyItemIdx);
	// 이미 같은 종류의 무공 등록여부
	// 등록되어 있으면 TRUE
	// 아니면 FALSE
	BOOL MugongOf(CPlayer * pPlayer, WORD wMugongIdx);

	BOOL ItemStageOf(CPlayer* pPlayer, BYTE bItemStage );
	
#ifdef _JAPAN_LOCAL_
	BOOL ItemAttrOf( CPlayer* pPlayer, WORD	wItemAttr );
	BOOL ItemAquireSkillOf( CPlayer* pPlayer, WORD wSkillIdx1, WORD wSkillIdx2 );
#endif
};

#endif // !defined(AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_)
