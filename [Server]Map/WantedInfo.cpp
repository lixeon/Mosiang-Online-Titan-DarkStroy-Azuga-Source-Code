// WantedInfo.cpp: implementation of the CWantedInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantedInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantedInfo::CWantedInfo()
{

}

CWantedInfo::~CWantedInfo()
{
}

void CWantedInfo::InitWantedInfo(WANTEDINFO * pInfo)
{
	m_WantedInfo.WantedChrIDX = pInfo->WantedChrIDX;
	SafeStrCpy(m_WantedInfo.WantedChrName, pInfo->WantedChrName, MAX_NAME_LENGTH+1);
	m_WantedInfo.TotalPrize = pInfo->TotalPrize;
	m_WantedInfo.WantedIDX = pInfo->WantedIDX;
	m_WantedInfo.RegistChrIDX = pInfo->RegistChrIDX;
	m_WantedInfo.TotalVolunteerNum = pInfo->TotalVolunteerNum;
}

void CWantedInfo::SetTotalPrize(MONEYTYPE TotalPrize)
{
	m_WantedInfo.TotalPrize = TotalPrize;
}

int CWantedInfo::GetTotalVolunteerNum()
{
	return m_WantedInfo.TotalVolunteerNum;
}

void CWantedInfo::IncreVolunteerNum()
{
	m_WantedInfo.TotalVolunteerNum++;
}

void CWantedInfo::DecreVolunteerNum()
{
	m_WantedInfo.TotalVolunteerNum--;
}