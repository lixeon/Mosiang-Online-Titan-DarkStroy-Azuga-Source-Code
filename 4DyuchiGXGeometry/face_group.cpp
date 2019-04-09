#include "face_group.h"
#include "geometry_global.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

CFaceGroup::CFaceGroup()
{
	memset(this,0,sizeof(CFaceGroup));
	/*
	m_dwMtlIndex = 0;
	m_dwIndex = 0;
	m_dwFacesNum = 0;
	m_dwMaxFacesNum = 0;
	m_dwVertexIndexNum = 0;
	m_dwLightUVNum1 = 0;
	m_dwLightUVNum2 = 0;

	m_pFace = NULL;
	m_pMtl = NULL;		

	m_ptLightUV1 = NULL;	
	m_ptLightUV2 = NULL;	
	m_bIsDelsetMtl = FALSE;
*/
}


BOOL CFaceGroup::IsCollision(VECTOR3* pv3IntersectPoint,float* pfDist,WORD* pwIndex,VECTOR3* pv3RayPos,VECTOR3* pv3RayDir,VECTOR3* pv3Pos)
{
	float	u,v,fDist;
	VECTOR3	v3Tri[3];
	VECTOR3	v3IntersectPoint;

	BOOL				bResult = FALSE;
	float	fMinDist = 900000.0f;
	
	
	for (DWORD i=0; i<m_dwFacesNum; i++)
	{
		v3Tri[0] = pv3Pos[m_pFace[i*3+0]];
		v3Tri[1] = pv3Pos[m_pFace[i*3+1]];
		v3Tri[2] = pv3Pos[m_pFace[i*3+2]];

		if (IntersectTriangle(&v3IntersectPoint,pv3RayPos,pv3RayDir,v3Tri+0,v3Tri+1,v3Tri+2,&fDist,&u,&v,TRUE))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;

				*pfDist = fDist;
				*pwIndex = (WORD)i;
				*pv3IntersectPoint = v3IntersectPoint;
				bResult = TRUE;
			}
		}
	}
	return bResult;
}
WORD* CFaceGroup::CreateFaceList(DWORD dwFacesNum)
{
	m_pFace = new WORD[dwFacesNum*3];
	m_dwMaxFacesNum = dwFacesNum;

	
	
	
	return m_pFace;
}

BOOL CFaceGroup::SetFace(DWORD dwIndex,WORD a,WORD b,WORD c)
{
	BOOL	bResult = FALSE;
	
	if (dwIndex >= m_dwFacesNum)
		goto lb_return;

	m_pFace[dwIndex*3+0] = a;
	m_pFace[dwIndex*3+1] = b;
	m_pFace[dwIndex*3+2] = c;

lb_return:
	return bResult;

}

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	m_dwFacesNum++;


lb_return:
	return bResult;
}

void CFaceGroup::SetMaterial(DWORD* pdwVertexColor,CMaterial* pMtl)
{
	m_pMtl = pMtl;

	if (pdwVertexColor)
	{
		for (DWORD i=0; i<m_dwFacesNum*3; i++)
		{
//			DWORD diffuse = pMtl->GetDiffuse() | 0xff000000;
			MultipyColor(pdwVertexColor + m_pFace[i],pdwVertexColor[m_pFace[i]],pMtl->GetDiffuse() | 0xff000000);
	//		pdwVertexColor[m_pFace[i]] = 0xff000000 | pMtl->GetDiffuse();

		}
	}
}

DWORD CFaceGroup::CreateIVertexList(VECTOR3* pv3Point,TVERTEX* ptUV,IVERTEX** ppVertex)
{
	IVERTEX*	pv = *ppVertex = new IVERTEX[m_dwFacesNum*3];

	for (DWORD i=0; i<m_dwFacesNum*3; i++)
	{
//		pv[i].mtlIndex = m_dwMtlIndex;
			
		if (pv3Point)
			*(VECTOR3*)(&pv[i].x) = pv3Point[m_pFace[i]];
		
		if (ptUV)
			*(TVERTEX*)(&pv[i].u1) = ptUV[m_pFace[i]];
		/*
		이 부분은 사용안함.참고로 남겨둠.IVERTEX로 만든후에 라이트맵을새로 생성하므로 이 부분은 불필요.
		if (m_ptLightUV1)
			*(TVERTEX*)(&pv[i].u2) = m_ptLightUV1[i];

		if (m_ptLightUV2)
			*(TVERTEX*)(&pv[i].u2) = m_ptLightUV2[i];*/
	}
	return m_dwFacesNum*3;
}

void CFaceGroup::ReleaseIVertexList(IVERTEX* pv)
{
	delete [] pv;

}
DWORD CFaceGroup::ReadFile(void* pFP)
{
	FILE_FACE_GROUP_HEADER* pHeader = (FILE_FACE_GROUP_HEADER*)&(this->m_dwMtlIndex);

	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,pHeader,sizeof(FILE_FACE_GROUP_HEADER));

	if (m_dwFacesNum)
	{
		m_pFace = new WORD[m_dwFacesNum*3];
		dwLen += g_pFileStorage->FSRead(pFP,m_pFace,sizeof(WORD)*m_dwFacesNum*3);
	}
	if (m_dwLightUVNum1)
	{
		m_ptLightUV1 = new TVERTEX[m_dwLightUVNum1];
		dwLen += g_pFileStorage->FSRead(pFP,m_ptLightUV1,sizeof(TVERTEX)*m_dwLightUVNum1);
	}

	if (m_dwLightUVNum2)
	{
		m_ptLightUV2 = new TVERTEX[m_dwLightUVNum2];
		dwLen += g_pFileStorage->FSRead(pFP,m_ptLightUV2,sizeof(TVERTEX)*m_dwLightUVNum2);
	}
	return dwLen;
}
DWORD CFaceGroup::WriteFile(FILE* fp)
{
	FILE_FACE_GROUP_HEADER* pHeader = (FILE_FACE_GROUP_HEADER*)&(this->m_dwMtlIndex);

	DWORD	oldPos = ftell(fp);
	
	fwrite(pHeader,sizeof(FILE_FACE_GROUP_HEADER),1,fp);
	
	if (m_dwFacesNum)
		fwrite(m_pFace,sizeof(WORD),m_dwFacesNum*3,fp);
		
	if (m_dwLightUVNum1)
		fwrite(m_ptLightUV1,sizeof(TVERTEX),m_dwLightUVNum1,fp);

	if (m_dwLightUVNum2)
		fwrite(m_ptLightUV2,sizeof(TVERTEX),m_dwLightUVNum2,fp);

	return (ftell(fp)-oldPos);
}
BOOL CFaceGroup::Duplicate(CFaceGroup* pFaceGroup)
{
	memcpy(pFaceGroup,this,sizeof(FILE_FACE_GROUP_HEADER));

	pFaceGroup->m_pMtl = m_pMtl;
	pFaceGroup->m_pFace = new WORD[m_dwFacesNum*3];
	memcpy(pFaceGroup->m_pFace,m_pFace,sizeof(WORD)*m_dwFacesNum*3);

	if (m_ptLightUV1)
	{
		pFaceGroup->m_ptLightUV1 = new TVERTEX[m_dwFacesNum*3];
		memcpy(pFaceGroup->m_ptLightUV1,m_ptLightUV1,sizeof(TVERTEX)*m_dwFacesNum*3);
	}
	if (m_ptLightUV2)
	{
		pFaceGroup->m_ptLightUV2 = new TVERTEX[m_dwFacesNum*3];
		memcpy(pFaceGroup->m_ptLightUV2,m_ptLightUV2,sizeof(TVERTEX)*m_dwFacesNum*3);
	}
	return TRUE;

}
void CFaceGroup::ReleaseFaceList()
{
	if (m_pFace)
	{
		delete [] m_pFace;
		m_pFace = NULL;
	}
	if (m_ptLightUV1)
	{
		delete [] m_ptLightUV1;
		m_ptLightUV1 = NULL;
	}
	if (m_ptLightUV2)
	{
		delete [] m_ptLightUV2;
		m_ptLightUV2 = NULL;
	}
}
CFaceGroup::~CFaceGroup()
{
	ReleaseFaceList();

}

