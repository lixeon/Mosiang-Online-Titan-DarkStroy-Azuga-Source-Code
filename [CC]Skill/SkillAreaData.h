// SkillAreaData.h: interface for the CSkillAreaData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SkillAreaDATA_H__E4BEA2D7_204C_4676_AC10_5375FDC96118__INCLUDED_)
#define AFX_SkillAreaDATA_H__E4BEA2D7_204C_4676_AC10_5375FDC96118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MHFile.h"

class CSkillAreaData  
{
	int m_AreaRadius;
	int m_AreaSize;

	int m_MaxAreaRadius;	// È¸Àü½ÃÄ×À»¶§ °¡Àå Å« ¹ÝÁö¸§

	AREATILE* m_pAreaTileInfo;

	void CalcMaxAreaRadius();

	void SetAreaAttrib(int index,AREATILE Attr);

	void InitAreaData(int AreaRadius);
	void ClearAreaData();
	
public:
	CSkillAreaData();
	virtual ~CSkillAreaData();

	void LoadAreaData(CMHFile* pFile);	
	void GetRotatedAreaData(CSkillAreaData* pRtArea,DIRINDEX DirIdx);
	
	AREATILE GetAreaAttrib(int index);
	inline int GetAreaIndex(int x,int z)	// x,z [-m_AreaRadius , m_AreaRadius]
	{
		int rx = x + m_AreaRadius;
		int rz = z + m_AreaRadius;
		return rz*m_AreaSize + rx;
	}
	inline int GetAreaIndex(float fLocalX,float fLocalZ)
	{
		int nx,nz;
		nx = int(fLocalX / fTILE_SIZE);
		if( (fLocalX - nx*fTILE_SIZE) > (fTILE_SIZE * 0.5f) )			nx += 1;
		if( (fLocalX - nx*fTILE_SIZE) < -(fTILE_SIZE * 0.5f) )			nx -= 1;

		nz = int(fLocalZ / fTILE_SIZE);
		if( (fLocalZ - nz*fTILE_SIZE) > (fTILE_SIZE * 0.5f) )			nz += 1;
		if( (fLocalZ - nz*fTILE_SIZE) < -(fTILE_SIZE * 0.5f) )			nz -= 1;

		return GetAreaIndex(nx,nz);
	}
	int GetAreaRadius()		{	return m_AreaRadius;	}

};

#endif // !defined(AFX_SkillAreaDATA_H__E4BEA2D7_204C_4676_AC10_5375FDC96118__INCLUDED_)

