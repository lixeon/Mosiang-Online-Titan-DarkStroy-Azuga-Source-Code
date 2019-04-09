// AIUniqueGroup.cpp: implementation of the CAIUniqueGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AIUniqueGroup.h"
#include "ChannelSystem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
//this class is a one regen CAIGroup in total channel

CAIUniqueGroup::CAIUniqueGroup()
{

}

CAIUniqueGroup::~CAIUniqueGroup()
{

}

void CAIUniqueGroup::RegenCheck()
{
	CAIGroup::RegenCheck();
	
//	if( m_dwGridID == CHANNELSYSTEM->GetLastChannelID() )	return;
	
	DWORD GridID = CHANNELSYSTEM->GetRandomChannelID();
	SetRandomGridID(GridID);
}