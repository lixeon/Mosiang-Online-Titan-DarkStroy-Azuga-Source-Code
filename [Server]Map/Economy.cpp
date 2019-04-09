// Economy.cpp: implementation of the CEconomy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Economy.h"
#include "..\[CC]Header\GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEconomy::CEconomy()
{
	m_RegEconomy = new REGIST_BASEECONOMY;
	m_SpacialItemBase = new ITEM_INFO;
}

CEconomy::~CEconomy()
{
	delete m_RegEconomy;
	delete m_SpacialItemBase;

}

void CEconomy::SetRegistEconomy(REGIST_BASEECONOMY* RegEconomy)
{
	if(RegEconomy)
		memcpy(m_RegEconomy, RegEconomy, sizeof(REGIST_BASEECONOMY));
}

REGIST_BASEECONOMY* CEconomy::GetRegistEconomy()
{
	return m_RegEconomy;
}

void CEconomy::SetBaseValue(ITEM_INFO* ListValue)
{
	if(ListValue)
		memcpy(m_SpacialItemBase, ListValue, sizeof(ITEM_INFO));
}

void CEconomy::CalculBase(ITEM_INFO* ItemValue)
{
//	m_SpacialItemBase[m_RegEconomy->OriginNum].BasePrice = m_RegEconomy->OriginPrice;
//	m_SpacialItemBase[m_RegEconomy-]
//	m_SpacialItemBase[m_RegEconomy->OriginNum].BuyPrice = 
}