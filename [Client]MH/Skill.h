// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILL_H__2838DA69_6FB2_4A87_9AC3_248866756652__INCLUDED_)
#define AFX_SKILL_H__2838DA69_6FB2_4A87_9AC3_248866756652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkill
{
protected:
	MUGONGBASE m_MugongBaseInfo;
	BOOL	m_bUse;
	char skillName[17];			// ½ºÅ³ÀÌ¸§
								// ´É·ÂÄ¡???
public:
	CSkill();
	virtual ~CSkill();
	void SetMugongBase(MUGONGBASE * mugong);
	MUGONGBASE * GetMugongBase() { return &m_MugongBaseInfo; }
	BOOL IsUse(){ return m_bUse; }
	void SetUse(BOOL val){ m_bUse = val; }
	void SetName(char * name){ strcpy(skillName, name); }
	
};

#endif // !defined(AFX_SKILL_H__2838DA69_6FB2_4A87_9AC3_248866756652__INCLUDED_)
