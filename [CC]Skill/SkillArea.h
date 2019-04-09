// SkillArea.h: interface for the CSkillArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLAREA_H__FEAE2DB1_5B80_4F4B_A210_52C8BBD27E1E__INCLUDED_)
#define AFX_SKILLAREA_H__FEAE2DB1_5B80_4F4B_A210_52C8BBD27E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillAreaData.h"

class CSkillArea  
{
	VECTOR3 m_CenterPos;
	CSkillAreaData* m_pSkillAreaData;

public:
	CSkillArea();
	CSkillArea(CSkillAreaData* pAreaData);
	virtual ~CSkillArea();
	
	void AddTileAttrByAreaData(CObject* pObject);
	void RemoveTileAttrByAreaData(CObject* pObject);

	void SetCenterPos(VECTOR3* pCPos)
	{
		m_CenterPos = *pCPos;
	}
	void GetCenterPos(VECTOR3* pCPos) { *pCPos = m_CenterPos; }
	BOOL IsInArea(VECTOR3* pPos);
	BOOL IsInDamageTile(VECTOR3* pPos);

#ifdef _MHCLIENT_
	void StartEffectOnDamageArea(WORD EffectNum,CObject* pOperator);
#endif

};

#endif // !defined(AFX_SKILLAREA_H__FEAE2DB1_5B80_4F4B_A210_52C8BBD27E1E__INCLUDED_)
