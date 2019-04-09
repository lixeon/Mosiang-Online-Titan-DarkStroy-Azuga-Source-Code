// ConditionFactory.cpp: implementation of the CConditionFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionFactory.h"
#include "SITUATION.h"
#include "RESOURCE.h"
#include "DATA.h"
#include "ACTION.h"
#include "ConditionNode.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionFactory::CConditionFactory()
{
	m_pDATAPool				= NULL;
	m_pRESOURCEPool			= NULL;
	m_pSITUATIONPool		= NULL;
	m_pCACTIONPool			= NULL;
	m_pCConditionNodePool	= NULL;
}

CConditionFactory::~CConditionFactory()
{

}

void CConditionFactory::Init()
{
	m_pDATAPool				= new CMemoryPoolTempl<DATA>;
	m_pRESOURCEPool			= new CMemoryPoolTempl<RESOURCE>;
	m_pSITUATIONPool		= new CMemoryPoolTempl<SITUATION>;
	m_pCACTIONPool			= new CMemoryPoolTempl<CACTION>;
	m_pCConditionNodePool	= new CMemoryPoolTempl<CConditionNode>;
	m_pDATAPool->Init(80, 20, "DATAPool");
	m_pRESOURCEPool->Init(80, 20, "RESOURCEPool");
	m_pSITUATIONPool->Init(80, 20, "SITUATIONPool");
	m_pCACTIONPool->Init(80, 20, "CACTIONPool");
	m_pCConditionNodePool->Init(80, 20, "CConditionNodePool" );

}
void CConditionFactory::Release()
{
	SAFE_DELETE(m_pDATAPool);
	SAFE_DELETE(m_pRESOURCEPool);
	SAFE_DELETE(m_pSITUATIONPool);
	SAFE_DELETE(m_pCACTIONPool);
	SAFE_DELETE(m_pCConditionNodePool);
}

DATA * CConditionFactory::NewDATA()
{
	return m_pDATAPool->Alloc();
}
RESOURCE * CConditionFactory::NewRESOURCE()
{
	return m_pRESOURCEPool->Alloc();
}
SITUATION *	CConditionFactory::NewSITUATION()
{
	return m_pSITUATIONPool->Alloc();
}
CACTION * CConditionFactory::NewACTION()
{
	return m_pCACTIONPool->Alloc();
}
CConditionNode * CConditionFactory::NewConditionNode()
{
	return m_pCConditionNodePool->Alloc();
}
void		CConditionFactory::FreeDATA(DATA * obj)
{
	m_pDATAPool->Free(obj);
}
void		CConditionFactory::FreeRESOURCE(RESOURCE * obj)
{
	m_pRESOURCEPool->Free(obj);
}
void		CConditionFactory::FreeSITUATION(SITUATION * obj)
{
	m_pSITUATIONPool->Free(obj);
}
void		CConditionFactory::FreeACTION(CACTION * obj)
{
	m_pCACTIONPool->Free(obj);
}
void		CConditionFactory::FreeConditionNode(CConditionNode * obj)
{
	m_pCConditionNodePool->Free(obj);
}