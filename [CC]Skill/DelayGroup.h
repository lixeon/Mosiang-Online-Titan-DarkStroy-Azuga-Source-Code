// DelayGroup.h: interface for the CDelayGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
#define AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../[Lib]YHLibrary/YHHashTable.h"

class CDelayGroup  
{
	struct CDelay
	{
		WORD Kind;
		WORD Idx;
		DWORD StartTime;
		DWORD DelayTime;
	};
	static CMemoryPoolTempl<CDelay>* m_pDelayPool;
	static DWORD m_PoolRefCount;

	CYHHashTable<CDelay> m_DelayedTable;

	DWORD MakeKey(WORD Kind,WORD Idx);

	// 타이탄 물약 딜레이 관련 변수 - tamoo : 07.10.30
	CDelay* m_pTitanPotionDelay;

public:
	enum eDelayKind
	{
		eDK_Skill = 1,
		eDK_Item = 2,
	};

	CDelayGroup();
	virtual ~CDelayGroup();

	void Init();
	void Release();

	void AddDelay(WORD Kind,WORD Idx,DWORD DelayTime,DWORD ElapsedTime=0);

	// 남은 딜레이 시간을 0~1로 리턴한다. 0 리턴하면 딜레이 없다는 뜻
	float CheckDelay(WORD Kind,WORD Idx,DWORD* pRemainTime = NULL );

	// 타이탄 물약 딜레이 관련 - - tamoo : 07.10.30
	void AddTitanPotionDelay( DWORD DelayTime,DWORD ElapsedTime=0 );
	float CheckTitanPotionDelay( DWORD* pRemainTime = NULL );

};

#endif // !defined(AFX_DELAYGROUP_H__9E742F39_D6EB_4FA4_BB74_50B937A618A0__INCLUDED_)
