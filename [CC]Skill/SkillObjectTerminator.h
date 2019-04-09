// SkillObjectTerminator.h: interface for the CSkillObjectTerminator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTERMINATOR_H__CF400D37_2167_4744_8AB6_341E589E5E9C__INCLUDED_)
#define AFX_SKILLOBJECTTERMINATOR_H__CF400D37_2167_4744_8AB6_341E589E5E9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillInfo;
class CSkillObjectTerminator  
{
public:
	CSkillObjectTerminator(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTerminator();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel) = 0;

	virtual BOOL CheckTerminate(SKILLOBJECT_INFO* pSkillObjectInfo) = 0;

};

#endif // !defined(AFX_SKILLOBJECTTERMINATOR_H__CF400D37_2167_4744_8AB6_341E589E5E9C__INCLUDED_)
