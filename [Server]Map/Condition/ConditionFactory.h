// ConditionFactory.h: interface for the CConditionFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITIONFACTORY_H__FC40A453_F7C3_4F3E_BA6D_35F19F06A471__INCLUDED_)
#define AFX_CONDITIONFACTORY_H__FC40A453_F7C3_4F3E_BA6D_35F19F06A471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class SITUATION;
class RESOURCE;
class DATA;
class CACTION;
class CConditionNode;

#define CONDITIONFACTORY	CConditionFactory::GetInstance()


class CConditionFactory  
{
	CMemoryPoolTempl<DATA>*					m_pDATAPool;
	CMemoryPoolTempl<RESOURCE>*				m_pRESOURCEPool;
	CMemoryPoolTempl<SITUATION>*			m_pSITUATIONPool;
	CMemoryPoolTempl<CACTION>*				m_pCACTIONPool;
	CMemoryPoolTempl<CConditionNode>*		m_pCConditionNodePool;
public:
	CConditionFactory();
	virtual ~CConditionFactory();
	GETINSTANCE(CConditionFactory);


	void Init();
	void Release();

	DATA *				NewDATA();
	RESOURCE *			NewRESOURCE();
	SITUATION *			NewSITUATION();
	CACTION *			NewACTION();
	CConditionNode *	NewConditionNode();

	void			FreeDATA(DATA * obj);
	void			FreeRESOURCE(RESOURCE * obj);
	void			FreeSITUATION(SITUATION * obj);
	void			FreeACTION(CACTION * obj);
	void			FreeConditionNode(CConditionNode * obj);
};

#endif // !defined(AFX_CONDITIONFACTORY_H__FC40A453_F7C3_4F3E_BA6D_35F19F06A471__INCLUDED_)
