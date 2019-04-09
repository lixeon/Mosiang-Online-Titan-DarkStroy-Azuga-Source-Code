// ConditionObjectManager.cpp: implementation of the CConditionObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConditionObjectManager.h"
#include "OPDATA.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _SWTHS(a)			switch(a)	{	case -1:	{
#define _SWTHE				}	return FALSE;		}
#define CASEOF(a)				}	return FALSE;		case (a):	{



CConditionObjectManager::CConditionObjectManager()
{
	m_ppOPDATAList = NULL;
	m_wMaxOPDATANum = 0;
}

CConditionObjectManager::~CConditionObjectManager()
{
	DeleteAllOPDATA();
}


void CConditionObjectManager::LoadOPDATA()
{
	/*
	CMHFile file;
		file.Init("OPDataList.txt", "r");
	
		m_wMaxOPDATANum = file.GetWord();
		m_ppOPDATAList = void*[m_wMaxOPDATANum];
		for( int i = 0 ; i < m_wMaxOPDATANum ; ++i )
		{
			m_ppOPDATAList[i] = NULL;
		}*/
}
void CConditionObjectManager::DeleteAllOPDATA()
{
	delete [] m_ppOPDATAList;
	m_ppOPDATAList = NULL;
}
BOOL CConditionObjectManager::AddOPDATA( void * pDataObj, DWORD opCode )
{
	if(m_ppOPDATAList[opCode])
		return FALSE;
	m_ppOPDATAList[opCode] = pDataObj;
	return TRUE;
}
void * CConditionObjectManager::GetOPDATA( DWORD opCode )
{
	return m_ppOPDATAList[opCode];
}

CONDITION_FUNC	CConditionObjectManager::GetSITUATIONFunc(SITUATIONIDX idx)
{
	return globalSITUATION[idx];
}


BOOL CConditionObjectManager::Evaluate(DWORD opCode1, DWORD oprator, DWORD opCode2 )
{
	OPBASE * b1 = static_cast<OPBASE *>(GetOPDATA(opCode1));
	
	_SWTHS( b1->GetOPType() )
	CASEOF(OP_DW)
		COPDATA<DWORD> * cb1 = static_cast<COPDATA<DWORD> *>(b1);
		OPBASE * b2 = static_cast<OPBASE *>(GetOPDATA(opCode2));
		_SWTHS( b2->GetOPType() )
		CASEOF(OP_DW)
			COPDATA<DWORD> * cb2 = static_cast<COPDATA<DWORD> *>(b2);
			_SWTHS( oprator )
			CASEOF(LESSTHAN_TYPE)
				if( cb1->GetOPData() < cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MORETHAN_TYPE)
				if( cb1->GetOPData() > cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(LTEQUAL_TYPE)
				if( cb1->GetOPData() <= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MTEQUAL_TYPE)
				if( cb1->GetOPData() >= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(EQUAL_TYPE)
				if( cb1->GetOPData() == cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			_SWTHE
		_SWTHE
	CASEOF(OP_IN)
		COPDATA<int> * cb1 = static_cast<COPDATA<int> *>(b1);
		OPBASE * b2 = static_cast<OPBASE *>(CONDITIONOBJMGR->GetOPDATA(opCode2));
		_SWTHS( b2->GetOPType() )
		CASEOF(OP_DW)
			COPDATA<int> * cb2 = static_cast<COPDATA<int> *>(b2);
			_SWTHS( oprator )
			CASEOF(LESSTHAN_TYPE)
				if( cb1->GetOPData() < cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MORETHAN_TYPE)
				if( cb1->GetOPData() > cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(LTEQUAL_TYPE)
				if( cb1->GetOPData() <= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MTEQUAL_TYPE)
				if( cb1->GetOPData() >= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(EQUAL_TYPE)
				if( cb1->GetOPData() == cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			_SWTHE
		_SWTHE
	CASEOF(OP_LG)
		COPDATA<LONG> * cb1 = static_cast<COPDATA<LONG> *>(b1);
		OPBASE * b2 = static_cast<OPBASE *>(CONDITIONOBJMGR->GetOPDATA(opCode2));
		_SWTHS( b2->GetOPType() )
		CASEOF(OP_DW)
			COPDATA<LONG> * cb2 = static_cast<COPDATA<LONG> *>(b2);
			_SWTHS( oprator )
			CASEOF(LESSTHAN_TYPE)
				if( cb1->GetOPData() < cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MORETHAN_TYPE)
				if( cb1->GetOPData() > cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(LTEQUAL_TYPE)
				if( cb1->GetOPData() <= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(MTEQUAL_TYPE)
				if( cb1->GetOPData() >= cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			CASEOF(EQUAL_TYPE)
				if( cb1->GetOPData() == cb2->GetOPData() )
					return TRUE;
				else
					return FALSE;
			_SWTHE
		_SWTHE
	_SWTHE	

	ASSERT(0);
	return TRUE;
}

