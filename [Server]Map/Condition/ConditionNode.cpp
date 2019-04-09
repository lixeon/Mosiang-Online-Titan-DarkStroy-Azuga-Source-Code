// ConditionNode.cpp: implementation of the CConditionNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionNode.h"

#include "SITUATION.h"
#include "RESOURCE.h"
#include "DATA.h"
#include "ACTION.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionNode::CConditionNode()
{
	m_wNodeSearchType		= SEARCH_BASIC;
	m_dwExpireTime			= 0;

	//m_Volatile				= 0;
	m_dwConditionID			= 0;
	bEndProcess				= FALSE;
	m_bCondition = RETURN_FALSE;

	pDATA					= NULL;
	pSITUATION				= NULL;
	pRESOURCE				= NULL;
	
	
	for( int i = 0 ; i < MAX_RETURN ; ++i )
	{
		pSubNode[i]		= NULL;
		pACTION[i]		= NULL;
	}
	m_bExpireTime = FALSE;

}

CConditionNode::~CConditionNode()
{

}

CRETURN CConditionNode::Execute()
{
	
	switch( m_wNodeSearchType )
	{
	case SEARCH_BASIC:
		break;
	case SEARCH_STOP:
		return RETURN_STOP;
	case SEARCH_JUMP_TRUE:		// 조건을 뛰어 넘음
		return RETURN_TRUE;
	case SEARCH_JUMP_FALSE:		// 조건을 뛰어 넘음
		return RETURN_FALSE;	
	default:
		ASSERT(0);
	}
	
	// DATA
	if( pDATA )
	{
		m_bCondition = pDATA->Execute();
		if(m_bCondition == RETURN_FALSE)
			return m_bCondition;
	}

	// SITUATION
	if( pSITUATION )
	{
		m_bCondition = pSITUATION->Execute();
		if(m_bCondition == RETURN_FALSE)
			return m_bCondition;
	}

	// RESOURCE
	if( pRESOURCE )
	{
		m_bCondition = pRESOURCE->Execute();
		if(m_bCondition == RETURN_FALSE)
			return m_bCondition;
	}
	
	// TIME
	if( m_bExpireTime )
	{
		if( m_dwStartTime + m_dwExpireTime < gCurTime )
		{
			m_bCondition = RETURN_TRUE;
		}
		else
		{
			m_bCondition = RETURN_FALSE;
			return m_bCondition;
		}
	}

	return m_bCondition;
}

void CConditionNode::Action(CObject * pObj)//CRETURN type)
{
	if(pACTION[m_bCondition])
		pACTION[m_bCondition]->Execute(pObj);
}

CConditionNode * CConditionNode::NextNode()// CBOOL type )
{
	return pSubNode[m_bCondition];
}

void CConditionNode::SetSubNode( CConditionNode* pNode, CRETURN bCondition )
{
	if( bCondition >= MAX_RETURN ) return;

	pSubNode[bCondition] = pNode;
}

void CConditionNode::SetAction( CACTION* pAction, CRETURN bCondition )
{
	if( bCondition >= MAX_RETURN ) return;

	pACTION[bCondition] = pAction;
}

