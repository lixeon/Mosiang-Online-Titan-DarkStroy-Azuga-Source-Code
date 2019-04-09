#include "face_group.h"

CFaceGroup::CFaceGroup()
{
	memset(this,0,sizeof(CFaceGroup));

}
WORD* CFaceGroup::CreateFaceList(DWORD dwFacesNum)
{
	m_pFace = new WORD[dwFacesNum*3];
	m_ptLightUV0 = new TVERTEX[dwFacesNum*3];

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

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	
	// 라이트맵 매쉬인 경우
	memcpy(m_ptLightUV0 + m_dwFacesNum*3,ptv,sizeof(TVERTEX)*3);
	m_dwLightUVNum0 += 3;
	
	m_dwFacesNum++;


lb_return:
	return bResult;
}

BOOL CFaceGroup::InsertFace(WORD a,WORD b,WORD c,TVERTEX* ptv1,TVERTEX* ptv2)
{
	BOOL	bResult = FALSE;
	
	if (m_dwFacesNum >= m_dwMaxFacesNum)
		goto lb_return;

	m_pFace[m_dwFacesNum*3+0] = a;
	m_pFace[m_dwFacesNum*3+1] = b;
	m_pFace[m_dwFacesNum*3+2] = c;
	
	// 라이트맵 매쉬인 경우
	memcpy(m_ptLightUV0 + m_dwFacesNum*3,ptv1,sizeof(TVERTEX)*3);
	m_dwLightUVNum0 += 3;

	
	m_dwFacesNum++;


lb_return:
	return bResult;
}


DWORD CFaceGroup::WriteFile(FILE* fp)
{
	FILE_FACE_GROUP_HEADER* pHeader = (FILE_FACE_GROUP_HEADER*)&(this->m_dwMtlIndex);

	DWORD	oldPos = ftell(fp);
	
	fwrite(pHeader,sizeof(FILE_FACE_GROUP_HEADER),1,fp);
	
	if (m_dwFacesNum)
		fwrite(m_pFace,sizeof(WORD),m_dwFacesNum*3,fp);
		
	if (m_dwLightUVNum0)
		fwrite(m_ptLightUV0,sizeof(TVERTEX),m_dwLightUVNum0,fp);

	
	return (ftell(fp)-oldPos);
}
CFaceGroup::~CFaceGroup()
{
	if (m_pFace)
	{
		delete [] m_pFace;
		m_pFace = NULL;
	}
	if (m_ptLightUV0)
	{
		delete [] m_ptLightUV0;
		m_ptLightUV0 = NULL;
	}

}

