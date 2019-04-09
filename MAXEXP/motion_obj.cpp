#include "motion_obj.h"

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
	FILE_MOTION_OBJECT_HEADER*	pHeader = (FILE_MOTION_OBJECT_HEADER*)&this->m_dwIndex;
	DWORD	oldPos = ftell(fp);

	fwrite(pHeader,sizeof(FILE_MOTION_OBJECT_HEADER),1,fp);
	if (m_dwPosKeyNum)
		fwrite(m_pPosKey,sizeof(POS_KEY),m_dwPosKeyNum,fp);

	if (m_dwRotKeyNum)
		fwrite(m_pRotKey,sizeof(ROT_KEY),m_dwRotKeyNum,fp);

	if (m_dwScaleKeyNum)
		fwrite(m_pScaleKey,sizeof(SCALE_KEY),m_dwScaleKeyNum,fp);
	
	
	return (ftell(fp)-oldPos);

}
DWORD CMotionObject::ReadFile(FILE* fp)
{
	FILE_MOTION_OBJECT_HEADER*	pHeader = (FILE_MOTION_OBJECT_HEADER*)&this->m_dwIndex;
	DWORD	oldPos = ftell(fp);
	

	fread(pHeader,sizeof(FILE_MOTION_OBJECT_HEADER),1,fp);
	if (m_dwPosKeyNum)
	{
		CreatePosKeyList(m_dwPosKeyNum);
		fread(m_pPosKey,sizeof(POS_KEY),m_dwPosKeyNum,fp);
	}
	if (m_dwRotKeyNum)
	{
		CreateRotKeyList(m_dwRotKeyNum);
		fwrite(m_pRotKey,sizeof(ROT_KEY),m_dwRotKeyNum,fp);
	}

	if (m_dwScaleKeyNum)
	{
		CreateScaleKeyList(m_dwScaleKeyNum);
		fwrite(m_pScaleKey,sizeof(SCALE_KEY),m_dwScaleKeyNum,fp);
	}
	
	return (ftell(fp)-oldPos);
}
CMotionObject::~CMotionObject()
{
	ReleaseScaleKey();
	ReleasePosKey();
	ReleaseRotKey();
}