// StatsCalcManager.h: interface for the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)
#define AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __MAPSERVER__
#define PLAYERTYPE CPlayer
class CPlayer;
#else
#define PLAYERTYPE CHero
class CHero;
#endif

#include "..\[CC]Header\GameResourceStruct.h"

#define STATSMGR	CStatsCalcManager::GetInstance()

class CStatsCalcManager  
{
	CStatsCalcManager();
public:	
	MAKESINGLETON(CStatsCalcManager);
	virtual ~CStatsCalcManager();

	/*
	void DoCalcBaseStats(PLAYERTYPE* pPlayer);
	void DoCalcItemStats(PLAYERTYPE* pPlayer);
	void DoCalcMugongStats(PLAYERTYPE* pPlayer);
	*/

	// È£Ãâ ¿ì¼±¼øÀ§ ¼ø!
	void CalcItemStats(PLAYERTYPE* pPlayer);	
	void CalcCharStats(PLAYERTYPE* pPlayer);

	void Clear(player_calc_stats * pStats);
		
	void CalcCharLife(PLAYERTYPE* pPlayer);	
	void CalcCharShield(PLAYERTYPE* pPlayer);	
	void CalcCharNaeruyk(PLAYERTYPE* pPlayer);	

	//SW070127 타이탄
	void CalcTitanItemStats(PLAYERTYPE* pPlayer);

	//2007. 6. 11. CBH - 세트아이탬 능력치 관련 함수
	void CalcSetItemStats(PLAYERTYPE* pPlayer);
	void SetSetItemStats(SET_ITEM_OPTION* pSetItemOption, SET_ITEM_OPTION* pSetItem_stats);

	// magi82 - UniqueItem(070626)
	void CalcUniqueItemStats(PLAYERTYPE* pPlayer);
};

#endif // !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)
