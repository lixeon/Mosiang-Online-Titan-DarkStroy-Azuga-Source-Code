// SkillObject_Mine.h: interface for the CSkillObject_Mine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECT_MINE_H__46659E88_6DB1_4C45_9BFB_2D0FA91044D8__INCLUDED_)
#define AFX_SKILLOBJECT_MINE_H__46659E88_6DB1_4C45_9BFB_2D0FA91044D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MAPSERVER_
#include "SkillObject_server.h"
#else
#include "SkillObject_client.h"
#endif

class CSkillObject_Mine : public CSkillObject  
{
	BOOL m_bOperate;
	BOOL CheckOperate(CObject* pObject);
public:
	CSkillObject_Mine(CSkillInfo* pSkillInfo,
				CSkillObjectTerminator* pTerminator,
				CSkillObjectTargetList* pTargetList,
				CSkillObjectFirstUnit* pFirstUnit);
	virtual ~CSkillObject_Mine();

	virtual void Init(SKILLOBJECT_INFO* pInfo,CTargetList* pTList);

	virtual void UpdateTargetList(CObject* pObject);
	virtual DWORD Update();

	virtual BOOL Operate(CObject* pRequestor,MAINTARGET* pMainTarget,CTargetList* pTList);
};

#endif // !defined(AFX_SKILLOBJECT_MINE_H__46659E88_6DB1_4C45_9BFB_2D0FA91044D8__INCLUDED_)
