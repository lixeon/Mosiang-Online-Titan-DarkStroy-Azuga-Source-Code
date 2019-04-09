// RegenPrototype.cpp: implementation of the CRegenPrototype class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegenPrototype.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CRegenObject::InitPrototype(CRegenPrototype * pPrototype)
{
	m_pPrototype = pPrototype;
}
void CRegenObject::InitHelpType()
{
	m_CurHelpType = m_pPrototype->InitHelpType;
}

DWORD CRegenObject::GetGroupID()
{
	return m_dwGroupID;
}