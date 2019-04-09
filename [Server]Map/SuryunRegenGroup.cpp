// SuryunRegenGroup.cpp: implementation of the CSuryunRegenGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuryunRegenGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunRegenGroup::CSuryunRegenGroup()
{
	m_bExecute = FALSE;
	m_AliveMonsterCount = 0;
}

CSuryunRegenGroup::~CSuryunRegenGroup()
{

}

int CSuryunRegenGroup::DecreaseAliveMonsterNum()
{ 
	m_AliveMonsterCount--;
	return m_AliveMonsterCount;
}