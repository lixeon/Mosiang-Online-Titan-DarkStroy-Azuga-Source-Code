// SkillObjectFactory.h: interface for the CSkillObjectFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTFACTORY_H__7B96A906_7E1E_4369_9406_83245E3446FB__INCLUDED_)
#define AFX_SKILLOBJECTFACTORY_H__7B96A906_7E1E_4369_9406_83245E3446FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillObject;
class CSkillInfo;

class CSkillObjectFactory  
{
public:
	CSkillObjectFactory();
	virtual ~CSkillObjectFactory();

	static CSkillObject* MakeNewSkillObject(CSkillInfo* pSkillInfo);
};

#endif // !defined(AFX_SKILLOBJECTFACTORY_H__7B96A906_7E1E_4369_9406_83245E3446FB__INCLUDED_)
