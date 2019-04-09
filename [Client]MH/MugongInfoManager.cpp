// MugongInfoManager.cpp: implementation of the CMugongInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongInfoManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongInfoManager::CMugongInfoManager()
{
	m_MugongInfoTable.Initialize(256);

}

CMugongInfoManager::~CMugongInfoManager()
{

}

BOOL CMugongInfoManager::AddMugongInfo(WORD MugongIdx,WORD MugongKind)
{
	if(m_MugongInfoTable.GetData(MugongIdx))
	{
		ASSERTMSG(0,"¹«°ø ÀÎµ¦½º°¡ Áßº¹ µÇ¾ú½À´Ï´Ù. Conflict Skill Index.");
		return FALSE;
	}
	return TRUE;		
}