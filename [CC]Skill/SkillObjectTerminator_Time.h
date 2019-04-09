// SkillObjectTerminator_Time.h: interface for the CSkillObjectTerminator_Time class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTERMINATOR_TIME_H__21FCFF67_E429_46B1_9CC4_2005F1EDB367__INCLUDED_)
#define AFX_SKILLOBJECTTERMINATOR_TIME_H__21FCFF67_E429_46B1_9CC4_2005F1EDB367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTerminator.h"

class CSkillObjectTerminator_Time : public CSkillObjectTerminator  
{
	DWORD m_LimitTime;
	DWORD m_AddionalTime;
public:
	CSkillObjectTerminator_Time(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTerminator_Time();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);

	virtual BOOL CheckTerminate(SKILLOBJECT_INFO* pSkillObjectInfo);
};

#endif // !defined(AFX_SKILLOBJECTTERMINATOR_TIME_H__21FCFF67_E429_46B1_9CC4_2005F1EDB367__INCLUDED_)
