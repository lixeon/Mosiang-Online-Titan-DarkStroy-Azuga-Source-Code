// SkillAreaData.cpp: implementation of the CSkillAreaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillAreaData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillAreaData::CSkillAreaData()
{
	m_AreaRadius = 0;
	m_AreaSize = 0;
	m_pAreaTileInfo = NULL;
}

CSkillAreaData::~CSkillAreaData()
{
	SAFE_DELETE_ARRAY(m_pAreaTileInfo);
}

void CSkillAreaData::InitAreaData(int AreaRadius)
{	
	m_AreaRadius = AreaRadius;
	m_AreaSize = m_AreaRadius*2 + 1;
	m_pAreaTileInfo = new AREATILE[m_AreaSize*m_AreaSize];
}

void CSkillAreaData::ClearAreaData()
{
	memset(m_pAreaTileInfo,0,sizeof(AREATILE)*m_AreaSize*m_AreaSize);
}

void CSkillAreaData::SetAreaAttrib(int index,AREATILE Attr)
{
	m_pAreaTileInfo[index] = Attr;
}

AREATILE CSkillAreaData::GetAreaAttrib(int index)
{
	return m_pAreaTileInfo[index];
}

void CSkillAreaData::LoadAreaData(CMHFile* pFile)
{
	InitAreaData(pFile->GetByte());

	for(int n =0;n<m_AreaSize*m_AreaSize;++n)
	{
		SetAreaAttrib(n,pFile->GetAreaTile());
	}

	CalcMaxAreaRadius();
}

void CSkillAreaData::CalcMaxAreaRadius()
{
	int ndist;
	int maxdist = m_AreaRadius;
	float dist;
	VECTOR3 ss;
	AREATILE tile;
	for(int n=0;n<m_AreaSize*m_AreaSize;++n)
	{
		tile = GetAreaAttrib(n);
		if(tile == 0)
			continue;

		ss.x = (float)(n % m_AreaSize - m_AreaRadius) * fTILE_SIZE;
		ss.z = (float)(n / m_AreaSize - m_AreaRadius) * fTILE_SIZE;

		dist = sqrtf(ss.x*ss.x + ss.z*ss.z);
		ndist = int(dist / fTILE_SIZE);		
		if( (dist - ndist*fTILE_SIZE) > (fTILE_SIZE * 0.5f) )			ndist += 1;

		if(maxdist < ndist)
			maxdist = ndist;
	}

	m_MaxAreaRadius = maxdist;
}

void CSkillAreaData::GetRotatedAreaData(CSkillAreaData* pRtAreaData,DIRINDEX DirIdx)
{
	float AngleRad = DIRTORAD(DirIdx);

	pRtAreaData->InitAreaData(m_MaxAreaRadius);
	pRtAreaData->ClearAreaData();
	
	VECTOR3 ss;
	VECTOR3 rt;
	int newidx;
	AREATILE tile;
	for(int n=0;n<m_AreaSize*m_AreaSize;++n)
	{
		tile = GetAreaAttrib(n);
		if(tile == 0)
			continue;

		ss.x = (float)(n % m_AreaSize - m_AreaRadius) * fTILE_SIZE;
		ss.z = (float)(n / m_AreaSize - m_AreaRadius) * fTILE_SIZE;

		RotateVectorAxisY(&rt,&ss,AngleRad);

		newidx = pRtAreaData->GetAreaIndex(rt.x,rt.z);
		pRtAreaData->SetAreaAttrib(newidx,tile);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// for test
	/*
	{
		FILE* fp;
		char fn[256];
		sprintf(fn,"d%03d.txt",DirIdx);
		fp = fopen(fn,"w");
		
		fprintf(fp,"%d",DirIdx);
		fprintf(fp,"\n");

		for(int n=0;n<m_AreaSize*m_AreaSize;++n)
		{
			if(n%m_AreaSize == 0)
				fprintf(fp,"\n");

			fprintf(fp,"%d ",GetAreaAttrib(n));
		}

		fprintf(fp,"\n");
		fprintf(fp,"\n");
		
		for(n=0;n<pRtAreaData->m_AreaSize*pRtAreaData->m_AreaSize;++n)
		{
			if(n%pRtAreaData->m_AreaSize == 0)
				fprintf(fp,"\n");
			fprintf(fp,"%d ",pRtAreaData->GetAreaAttrib(n));
		}
		fclose(fp);
	}
	*/
}
