// DelayGroup.cpp: implementation of the CDelayGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryPoolTempl<CDelayGroup::CDelay>* CDelayGroup::m_pDelayPool = NULL;
DWORD CDelayGroup::m_PoolRefCount = 0;

DWORD CDelayGroup::MakeKey(WORD Kind,WORD Idx)
{
	return (Kind << 16) | Idx;
}

CDelayGroup::CDelayGroup()
{
	if(m_pDelayPool == NULL)
	{
		m_pDelayPool = new CMemoryPoolTempl<CDelay>;
#ifdef _MAPSERVER_
		m_pDelayPool->Init(1024,128,"CDelayGroup");
#else
		m_pDelayPool->Init(16,16,"CDelayGroup");
#endif
	}

	++m_PoolRefCount;

	m_DelayedTable.Initialize(16);

	m_pTitanPotionDelay = NULL;
}

CDelayGroup::~CDelayGroup()
{
	Release();

	if(m_PoolRefCount == 0)
	{
		static BOOL bMsgBox = TRUE;
		if(bMsgBox)
			MessageBox(0,"DelayGroup 이 이상하다!!",0,0);

		return;
	}

	--m_PoolRefCount;
	if(m_PoolRefCount == 0)
	{
		m_pDelayPool->Release();
		delete m_pDelayPool;
		m_pDelayPool = NULL;
	}
}

void CDelayGroup::Init()
{
	m_pTitanPotionDelay = NULL;
	Release();
}

void CDelayGroup::Release()
{
	m_DelayedTable.SetPositionHead();
	while(CDelay* pDelay = m_DelayedTable.GetData())
	{
		m_pDelayPool->Free(pDelay);
	}
	m_DelayedTable.RemoveAll();
}

float CDelayGroup::CheckDelay(WORD Kind,WORD Idx, DWORD* pRemainTime)
{
	// magi82 - Titan(070910) 타이탄 무공업데이트
	//if(Idx >= SKILLNUM_TO_TITAN)
	//	Idx -= SKILLNUM_TO_TITAN;

	DWORD key = MakeKey(Kind,Idx);
	CDelay* pDelay = m_DelayedTable.GetData(key);
	if(pDelay == NULL)
		return 0;

	DWORD Elapsed = gCurTime - pDelay->StartTime;
#ifdef _MAPSERVER_
	//2007. 11. 15. CBH - 무공 딜레이 버그 관련 수정
	//클라이언트에서 딜레이가 풀리고 바로 무공을 쓰면 서버에서는 이전 무공의 딜레이가 끝나지 않아서
	//무공이 먹히는 버그가 있어서 0.3초의 오차를 주기위해 300을 더해서 보정.
	float rate = 1 - ( (Elapsed+300)/(float)pDelay->DelayTime);
#else
	float rate = 1 - (Elapsed/(float)pDelay->DelayTime);
#endif
	
	if( pRemainTime )
	{
		if( pDelay->DelayTime > Elapsed )
			*pRemainTime = pDelay->DelayTime - Elapsed;
		else
			*pRemainTime = 0;
	}

	// 딜레이가 끝났으면 딜레이를 없엔다.
	if(rate <= 0)
	{
		if( pRemainTime )
			*pRemainTime = 0;
		m_pDelayPool->Free(pDelay);
		m_DelayedTable.Remove(key);
		return 0;
	}

	return rate;
}
void CDelayGroup::AddDelay(WORD Kind,WORD Idx,DWORD DelayTime,DWORD ElapsedTime )
{
	DWORD key = MakeKey(Kind,Idx);
	CDelay* pDelay = m_DelayedTable.GetData(key);

	// 기존 딜레이가 있으면 갱신한후 리턴한다.
	if(pDelay)
	{
		pDelay->StartTime = gCurTime;
		pDelay->DelayTime = DelayTime;
		return;
	}

	// 없었으면 새로 추가한다.
	pDelay = m_pDelayPool->Alloc();
	pDelay->Kind = Kind;
	pDelay->Idx = Idx;
	
	DWORD dwStartTime = gCurTime-ElapsedTime;
	if( gCurTime < ElapsedTime )
	{
		dwStartTime = gCurTime;

		if( DelayTime > ElapsedTime )
			DelayTime -= ElapsedTime;
		else
			DelayTime = 0;
	}
		
	pDelay->StartTime = dwStartTime;
	pDelay->DelayTime = DelayTime;

	m_DelayedTable.Add(pDelay,key);
}

float CDelayGroup::CheckTitanPotionDelay( DWORD* pRemainTime )
{
	if( m_pTitanPotionDelay == NULL )
		return 0;

	DWORD Elapsed = gCurTime - m_pTitanPotionDelay->StartTime;
	float rate = 1 - (Elapsed/(float)m_pTitanPotionDelay->DelayTime);

	if( pRemainTime )
	{
		if( m_pTitanPotionDelay->DelayTime > Elapsed )
			*pRemainTime = m_pTitanPotionDelay->DelayTime - Elapsed;
		else
			*pRemainTime = 0;
	}

	// 딜레이가 끝났으면 딜레이를 없엔다.
	if(rate <= 0)
	{
		if( pRemainTime )
			*pRemainTime = 0;
		m_pDelayPool->Free(m_pTitanPotionDelay);
		m_pTitanPotionDelay = NULL;
		return 0;
	}

	return rate;
}
void CDelayGroup::AddTitanPotionDelay( DWORD DelayTime, DWORD ElapsedTime )
{
	// 기존 딜레이가 있으면 갱신한후 리턴한다.
	if( m_pTitanPotionDelay )
	{
		m_pTitanPotionDelay->StartTime = gCurTime;
		m_pTitanPotionDelay->DelayTime = DelayTime;
		return;
	}

	// 없었으면 새로 추가한다.
	m_pTitanPotionDelay = m_pDelayPool->Alloc();

	DWORD dwStartTime = gCurTime-ElapsedTime;
	if( gCurTime < ElapsedTime )
	{
		dwStartTime = gCurTime;

		if( DelayTime > ElapsedTime )
			DelayTime -= ElapsedTime;
		else
			DelayTime = 0;
	}

	m_pTitanPotionDelay->StartTime = dwStartTime;
	m_pTitanPotionDelay->DelayTime = DelayTime;
}
