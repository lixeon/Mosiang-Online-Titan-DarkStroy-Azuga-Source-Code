#include "motion_obj.h"
#include <crtdbg.h>

CMotionObject::CMotionObject()
{
	memset(this,0,sizeof(CMotionObject));
}

BOOL CMotionObject::CreateRotKeyList(DWORD dwNum)
{
	m_pRotKey = new ROT_KEY[dwNum];
	memset(m_pRotKey,0,sizeof(ROT_KEY)*dwNum);
	m_dwRotKeyNum = 0;

	return TRUE;
}
BOOL CMotionObject::CreatePosKeyList(DWORD dwNum)
{
	m_pPosKey = new POS_KEY[dwNum];
	memset(m_pPosKey,0,sizeof(POS_KEY)*dwNum);
	m_dwPosKeyNum = 0;

	return TRUE;

}

BOOL CMotionObject::CreateScaleKeyList(DWORD dwNum)
{
	m_pScaleKey = new SCALE_KEY[dwNum];
	memset(m_pScaleKey,0,sizeof(SCALE_KEY)*dwNum);
	m_dwScaleKeyNum = 0;

	return TRUE;
}
BOOL CMotionObject::CreateAnimatedMeshKeyList(DWORD dwNum)
{
	m_pAnimatedMeshKey = new ANIMATED_MESH_KEY[dwNum];
	memset(m_pAnimatedMeshKey,0,sizeof(ANIMATED_MESH_KEY)*dwNum);
	m_dwAnimatedMeshKeyNum = 0;

	return TRUE;
}
BOOL CMotionObject::InsertRotKey(ROT_KEY* pKey)
{
	m_pRotKey[m_dwRotKeyNum] = *pKey;
	m_dwRotKeyNum++;
	return TRUE;
}
BOOL CMotionObject::InsertPosKey(POS_KEY* pKey)
{
	m_pPosKey[m_dwPosKeyNum] = *pKey;
	m_dwPosKeyNum++;
	return TRUE;
}
BOOL CMotionObject::InsertScaleKey(SCALE_KEY* pKey)
{
	m_pScaleKey[m_dwScaleKeyNum] = *pKey;
	m_dwScaleKeyNum++;
	return TRUE;
}

BOOL CMotionObject::InsertAnimatedMeshKey(ANIMATED_MESH_KEY* pKey)
{

	m_pAnimatedMeshKey[m_dwAnimatedMeshKeyNum] = *pKey;
	m_dwAnimatedMeshKeyNum++;
	return TRUE;
}

void CMotionObject::ReleasePosKey()
{
	if (m_pPosKey)
	{
		delete [] m_pPosKey;
		m_pPosKey = NULL;
	}
	m_dwPosKeyNum = 0;
}
void CMotionObject::ReleaseRotKey()
{
	if (m_pRotKey)
	{
		delete [] m_pRotKey;
		m_pRotKey = NULL;
	}
	m_dwRotKeyNum = 0;
}
void CMotionObject::ReleaseScaleKey()
{
	if (m_pScaleKey)
	{
		delete [] m_pScaleKey;
		m_pScaleKey = NULL;
	}
	m_dwScaleKeyNum = 0;
}

DWORD CMotionObject::WriteFile(FILE* fp)
{
	// 현재 애니메이션 상태는 1프레임부터 시작한다.이걸 0베이스로 바꿔준다.

	FILE_MOTION_OBJECT_HEADER*	pHeader = (FILE_MOTION_OBJECT_HEADER*)&this->m_dwIndex;
	DWORD	oldPos = ftell(fp);

	DWORD i;
	for (i=0; i<m_dwPosKeyNum; i++)
	{

		if (m_pPosKey[i].dwFrame > 0)
		{
			m_pPosKey[i].dwFrame--;
		}
		else
			MessageBox(NULL,"CMotionObject::WriteFile()-PosKey.frame <= 0","Error",MB_OK);
	}
	
	for (i=0; i<m_dwRotKeyNum; i++)
	{

		if (m_pRotKey[i].dwFrame > 0)
		{
			m_pRotKey[i].dwFrame--;
		}
		else
			MessageBox(NULL,"CMotionObject::WriteFile()-RotKey.frame <= 0","Error",MB_OK);
	}

	for (i=0; i<m_dwScaleKeyNum; i++)
	{

		if (m_pScaleKey[i].dwFrame > 0)
		{
			m_pScaleKey[i].dwFrame--;
		}
		else
			MessageBox(NULL,"CMotionObject::WriteFile()-ScaleKey.frame <= 0","Error",MB_OK);
	}
	
	for (i=0; i<m_dwAnimatedMeshKeyNum; i++)
	{

		if (m_pAnimatedMeshKey[i].dwFrame > 0)
		{
			m_pAnimatedMeshKey[i].dwFrame--;
		}
		else
			MessageBox(NULL,"CMotionObject::WriteFile()-AnimatedMeshKey.frame <= 0","Error",MB_OK);
	}

	fwrite(pHeader,sizeof(FILE_MOTION_OBJECT_HEADER),1,fp);
	if (m_dwPosKeyNum)
		fwrite(m_pPosKey,sizeof(POS_KEY),m_dwPosKeyNum,fp);

	if (m_dwRotKeyNum)
		fwrite(m_pRotKey,sizeof(ROT_KEY),m_dwRotKeyNum,fp);

	if (m_dwScaleKeyNum)
		fwrite(m_pScaleKey,sizeof(SCALE_KEY),m_dwScaleKeyNum,fp);
	
	if (m_dwAnimatedMeshKeyNum)
	{
		for (DWORD j=0; j<m_dwAnimatedMeshKeyNum; j++)
		{
			fwrite(&m_pAnimatedMeshKey[j],sizeof(ANIMATED_MESH_KEY),1,fp);
			fwrite(m_pAnimatedMeshKey[j].pv3Pos,sizeof(VECTOR3),m_pAnimatedMeshKey[j].dwVertexNum,fp);
			fwrite(m_pAnimatedMeshKey[j].ptUV,sizeof(TVERTEX),m_pAnimatedMeshKey[j].dwTexVertexNum,fp);
		}
	}
	
	return (ftell(fp)-oldPos);

}
DWORD CMotionObject::ReadFile(FILE* fp)
{
	FILE_MOTION_OBJECT_HEADER	header;
	
	DWORD	oldPos = ftell(fp);
	

	fread(&header,sizeof(FILE_MOTION_OBJECT_HEADER),1,fp);

	if (header.dwPosKeyNum)
	{
		CreatePosKeyList(header.dwPosKeyNum);
		m_dwPosKeyNum = header.dwPosKeyNum;
		fread(m_pPosKey,sizeof(POS_KEY),m_dwPosKeyNum,fp);
		
	}

	if (header.dwRotKeyNum)
	{
		CreateRotKeyList(header.dwRotKeyNum);
		m_dwRotKeyNum = header.dwRotKeyNum;
		fread(m_pRotKey,sizeof(ROT_KEY),m_dwRotKeyNum,fp);
		
	}

	if (header.dwScaleKeyNum)
	{
		CreateScaleKeyList(header.dwScaleKeyNum);
		m_dwScaleKeyNum = header.dwScaleKeyNum;
		fread(m_pScaleKey,sizeof(SCALE_KEY),m_dwScaleKeyNum,fp);
		
	}
	_tcscpy(m_uszObjectName,header.uszObjectName);	
	return (ftell(fp)-oldPos);
}
CMotionObject::~CMotionObject()
{
	
	ReleaseScaleKey();
	ReleasePosKey();
	ReleaseRotKey();
	
	if (m_pAnimatedMeshKey)
	{
		for (DWORD i=0; i<m_dwAnimatedMeshKeyNum; i++)
		{
			if (m_pAnimatedMeshKey[i].pv3Pos)
			{
				delete [] m_pAnimatedMeshKey[i].pv3Pos;
				m_pAnimatedMeshKey[i].pv3Pos = NULL;
			}
			
			if (m_pAnimatedMeshKey[i].ptUV)
			{
				delete [] m_pAnimatedMeshKey[i].ptUV;
				m_pAnimatedMeshKey[i].ptUV = NULL;
			}
			
		}
		delete [] m_pAnimatedMeshKey;

		m_pAnimatedMeshKey = NULL;
		m_dwAnimatedMeshKeyNum = 0;

	}
}