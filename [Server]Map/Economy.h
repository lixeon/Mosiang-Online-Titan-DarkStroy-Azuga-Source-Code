// Economy.h: interface for the CEconomy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECONOMY_H__E8A731F4_A668_4CAC_8520_3EC0B5E9DDAD__INCLUDED_)
#define AFX_ECONOMY_H__E8A731F4_A668_4CAC_8520_3EC0B5E9DDAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct ITEM_INFO;

#define ECONOMY_OBJ CEconomy::GetInstance()

class CEconomy  
{
private:
	REGIST_BASEECONOMY* m_RegEconomy;
	ITEM_INFO * m_SpacialItemBase;
	
public:
	GETINSTANCE(CEconomy)

	CEconomy();
	virtual ~CEconomy();
	void SetRegistEconomy(REGIST_BASEECONOMY* RegEconomy);
	void SetBaseValue(ITEM_INFO* ListValue);
	REGIST_BASEECONOMY* GetRegistEconomy();
	void CalculBase(ITEM_INFO* ItemValue);
};

#endif // !defined(AFX_ECONOMY_H__E8A731F4_A668_4CAC_8520_3EC0B5E9DDAD__INCLUDED_)
