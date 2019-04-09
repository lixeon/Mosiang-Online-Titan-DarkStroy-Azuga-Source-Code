// AISystem.h: interface for the CAISystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AISYSTEM_H__AEB07393_72B9_40AA_B249_3867848DF59A__INCLUDED_)
#define AFX_AISYSTEM_H__AEB07393_72B9_40AA_B249_3867848DF59A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "cConstLinkedList.h"
#include "StateMachinen.h"
#include "AIGroupManager.h"


class CObject;
class CMsgRouter;

class CAISystem  
{
	CYHHashTable<CObject> m_AISubordinatedObject;
	CMsgRouter * m_pROUTER;
	CIndexGenerator m_MonsterIDGenerator;
public:
	CAISystem();
	virtual ~CAISystem();
	
	
	void Process();
	void ConstantProcess(CObject * obj);
	void PeriodicProcess(CObject * obj);

	void AddObject(CObject * obj);
	CObject * RemoveObject(DWORD dwID);
	
/*	void RecallObject(DWORD dwGroupID, WORD kind, VECTOR3 * pPos);*/

//	void SetState(CObject * obj, DWORD inputEvent);
	void SetTransition(CObject * obj, DWORD inputEvent);
	void SendMsg(WORD msgID, DWORD src, DWORD dest, DWORD delay, DWORD flag);
//	CIndexGenerator * GetIndexGen() { return &m_objectIdxGen; }
	DWORD GeneraterMonsterID();
	void ReleaseMonsterID(DWORD id);

	void LoadAIGroupList();
	void RemoveAllList();
};

extern CAISystem * g_pAISystem;

#endif // !defined(AFX_AISYSTEM_H__AEB07393_72B9_40AA_B249_3867848DF59A__INCLUDED_)
