#include "motion_obj.h"
#include "../4DyuchiGXGFunc/global.h"
#include "global_variable.h"
#include "../4DyuchiGRX_Common/IFileStorage.h"

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
BOOL CMotionObject::SetMorphFrame(VECTOR3** pv3Local,DWORD dwVertexNum,TVERTEX** ptvUV,DWORD dwTexVertexNum,DWORD dwFrame)
{

	
	ANIMATED_MESH_KEY*		pPrvAnimatedMeshKey;
	ANIMATED_MESH_KEY*		pCurAnimatedMeshKey;
	

	int				iRet;
	BOOL			bResult = FALSE;

	*pv3Local = NULL;
	*ptvUV = NULL;

	if (!m_dwAnimatedMeshKeyNum)
		goto lb_return;

/*
#ifdef _DEBUG
	if (dwTexVertexNum)
	{

		if (m_pAnimatedMeshKey[0].dwTexVertexNum != dwTexVertexNum)
		{
			char txt[512];
			wsprintf(txt,"매쉬데이타와 애니메이션 데이타의 텍스쳐 버텍스 수가 일치하지 않습니다.\n");
			OutputDebugString(txt);
			__asm int 3
		}
	}

	if (dwVertexNum)
	{

		if (m_pAnimatedMeshKey[0].dwVertexNum != dwVertexNum)
		{
			char txt[512];
			wsprintf(txt,"매쉬데이타와 애니메이션 데이타의 버텍스 수가 일치하지 않습니다.\n");
			OutputDebugString(txt);
			__asm int 3
		}
	}
#endif*/

	if (!(m_pAnimatedMeshKey[0].dwVertexNum | m_pAnimatedMeshKey[0].dwTexVertexNum))
		goto lb_return;
		
	
	if (dwFrame < m_pAnimatedMeshKey[0].dwFrame)
		dwFrame = m_pAnimatedMeshKey[0].dwFrame;

	if (dwFrame > m_pAnimatedMeshKey[m_dwAnimatedMeshKeyNum-1].dwFrame)
		dwFrame = m_pAnimatedMeshKey[m_dwAnimatedMeshKeyNum-1].dwFrame;

	iRet = m_AnimatedMeshKeyTree.SearchItem((void**)&pPrvAnimatedMeshKey,(void**)&pCurAnimatedMeshKey,dwFrame);
	
	if (pPrvAnimatedMeshKey)
	{
		*pv3Local = pCurAnimatedMeshKey->pv3Pos;
		*ptvUV = pCurAnimatedMeshKey->ptUV;
	}
	else
	{
		*pv3Local = pCurAnimatedMeshKey->pv3Pos;
		*ptvUV = pCurAnimatedMeshKey->ptUV;

		// 아직 보간처리 안되어있음...
	//	float	fAlpha;
	//	fAlpha = (float)(dwFrame - pPrvPosKey->dwFrame) / (float)(pCurPosKey->dwFrame - pPrvPosKey->dwFrame);
	//	for (DWORD i=0; i<pCurAnimatedMeshKey->dwVertexNum; i++)
	//	{
	//		v3Pos = pPrvAnimatedMeshKey->v3Pos + (pCurAnimatedMeshKey->v3Pos - pPrvAnimatedMeshKey->v3Pos)* fAlpha;
	//	}

	}
	if (dwVertexNum > m_pAnimatedMeshKey[0].dwVertexNum)
		*pv3Local = NULL;
		
	if (dwTexVertexNum > m_pAnimatedMeshKey[0].dwTexVertexNum)
		*ptvUV = NULL;

	bResult = TRUE;
 

lb_return:
	return bResult;
}
BOOL CMotionObject::SetRotMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame)
{

	ROT_KEY*		pPrvRotKey;
	ROT_KEY*		pCurRotKey;
	QUARTERNION		q;

	int				iRet;

	BOOL			bResult = FALSE;
	float			fAlpha;

	if (!m_dwRotKeyNum)
		goto lb_return;
	
	if (dwFrame < m_pRotKey[0].dwFrame)
		dwFrame = m_pRotKey[0].dwFrame;

	if (dwFrame > m_pRotKey[m_dwRotKeyNum-1].dwFrame)
		dwFrame = m_pRotKey[m_dwRotKeyNum-1].dwFrame;

	iRet = m_RotKeyTree.SearchItem((void**)&pPrvRotKey,(void**)&pCurRotKey,dwFrame);
	

	if (pPrvRotKey)
	{
		// 보간해야한다.  
		fAlpha = (float)(dwFrame - pPrvRotKey->dwFrame) / (float)(pCurRotKey->dwFrame - pPrvRotKey->dwFrame);
		QuaternionSlerp(&q, &pPrvRotKey->Q, &pCurRotKey->Q, fAlpha);
		MatrixFromQuaternion(pMat, &q);
	}
	else
	{
		// 보간할 필요없다.
		MatrixFromQuaternion(pMat,&pCurRotKey->Q);
	}
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CMotionObject::SetPosMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame)
{
	
	POS_KEY*		pPrvPosKey;
	POS_KEY*		pCurPosKey;
	
	int				iRet;
	BOOL			bResult = FALSE;
	float			fAlpha;
	
	VECTOR3			v3Pos;

	if (!m_dwPosKeyNum)
		goto lb_return;
	
	if (dwFrame < m_pPosKey[0].dwFrame)
		dwFrame = m_pPosKey[0].dwFrame;

	if (dwFrame > m_pPosKey[m_dwPosKeyNum-1].dwFrame)
		dwFrame = m_pPosKey[m_dwPosKeyNum-1].dwFrame;

	iRet = m_PosKeyTree.SearchItem((void**)&pPrvPosKey,(void**)&pCurPosKey,dwFrame);

	if (pPrvPosKey)
	{
		fAlpha = (float)(dwFrame - pPrvPosKey->dwFrame) / (float)(pCurPosKey->dwFrame - pPrvPosKey->dwFrame);

		VECTOR3_SUB_VECTOR3(&v3Pos,&pCurPosKey->v3Pos,&pPrvPosKey->v3Pos);
		VECTOR3_MUL_FLOAT(&v3Pos,&v3Pos,fAlpha);
		VECTOR3_ADD_VECTOR3(&v3Pos,&pPrvPosKey->v3Pos,&v3Pos);
		TranslateMatrix(pMat,&v3Pos);
	}
	else
	{	
		TranslateMatrix(pMat,&pCurPosKey->v3Pos);
	} 
	bResult = TRUE;

lb_return:
	return bResult;
	
}
BOOL CMotionObject::SetScaleMatrix(MATRIX4* pMat,NODE_TM* pBaseTM,DWORD dwFrame)
{
	SCALE_KEY*		pPrvScaleKey;
	SCALE_KEY*		pCurScaleKey;
	
	int				iRet;

	BOOL			bResult = FALSE;
	float			fAlpha;
	VECTOR3			v3Scale;

	
	if (!m_dwScaleKeyNum)
		goto lb_return;
	
	if (dwFrame < m_pScaleKey[0].dwFrame)
		dwFrame = m_pScaleKey[0].dwFrame;

	if (dwFrame > m_pScaleKey[m_dwScaleKeyNum-1].dwFrame)
		dwFrame = m_pScaleKey[m_dwScaleKeyNum-1].dwFrame;

	iRet = m_ScaleKeyTree.SearchItem((void**)&pPrvScaleKey,(void**)&pCurScaleKey,dwFrame);


	if (pPrvScaleKey)
	{
		fAlpha = (float)(dwFrame - pPrvScaleKey->dwFrame) / (float)(pCurScaleKey->dwFrame - pPrvScaleKey->dwFrame);
	//	v3Pos = pPrvPosKey->v3Pos + (pCurPosKey->v3Pos - pPrvPosKey->v3Pos)* fAlpha;
		VECTOR3_SUB_VECTOR3(&v3Scale,&pCurScaleKey->v3Scale,&pPrvScaleKey->v3Scale);
		VECTOR3_MUL_FLOAT(&v3Scale,&v3Scale,fAlpha);
		VECTOR3_ADD_VECTOR3(&v3Scale,&pPrvScaleKey->v3Scale,&v3Scale);
		::SetScaleMatrix(pMat,&v3Scale);
	}
	else
	{
		::SetScaleMatrix(pMat,&pCurScaleKey->v3Scale);
	}
	bResult = TRUE;
 

lb_return:
	return bResult;
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
void CMotionObject::Initialize()
{
	int i,iMiddle;
/*
	CBinTree				m_PosKeyTree;
	CBinTree				m_ScaleKeyTree;
	CBinTree				m_AnimatedKeyTree;*/
	if (m_dwRotKeyNum)
	{
		iMiddle = (int)m_dwRotKeyNum / 2;
		m_RotKeyTree.Initialize(m_dwRotKeyNum);
		for (i=iMiddle; i>=0; i--)
		{
			m_RotKeyTree.InsertNode(m_pRotKey[i].dwFrame,m_pRotKey+i);
		}
		for (i=(int)(m_dwRotKeyNum-1); i > iMiddle; i--)
		{
			m_RotKeyTree.InsertNode(m_pRotKey[i].dwFrame,m_pRotKey+i);
		}
	}
	if (m_dwPosKeyNum)
	{
		iMiddle = (int)m_dwPosKeyNum / 2;
		m_PosKeyTree.Initialize(m_dwPosKeyNum);
		for (i=iMiddle; i>=0; i--)
		{
			m_PosKeyTree.InsertNode(m_pPosKey[i].dwFrame,m_pPosKey+i);
			
		}
		for (i=(int)(m_dwPosKeyNum-1); i > iMiddle; i--)
		{
			m_PosKeyTree.InsertNode(m_pPosKey[i].dwFrame,m_pPosKey+i);
		}
	}
	
	if (m_dwScaleKeyNum)
	{
		iMiddle = (int)m_dwScaleKeyNum / 2;
		m_ScaleKeyTree.Initialize(m_dwScaleKeyNum);
		for (i=iMiddle; i>=0; i--)
		{
			m_ScaleKeyTree.InsertNode(m_pScaleKey[i].dwFrame,m_pScaleKey+i);
		}
		for (i=(int)(m_dwScaleKeyNum-1); i > iMiddle; i--)
		{
			m_ScaleKeyTree.InsertNode(m_pScaleKey[i].dwFrame,m_pScaleKey+i);
		}
	}
	if (m_dwAnimatedMeshKeyNum)
	{
		iMiddle = (int)m_dwAnimatedMeshKeyNum / 2;
		m_AnimatedMeshKeyTree.Initialize(m_dwAnimatedMeshKeyNum);
		for (i=iMiddle; i>=0; i--)
		{
			m_AnimatedMeshKeyTree.InsertNode(m_pAnimatedMeshKey[i].dwFrame,m_pAnimatedMeshKey+i);
		}
		for (i=(int)(m_dwAnimatedMeshKeyNum-1); i > iMiddle; i--)
		{
			m_AnimatedMeshKeyTree.InsertNode(m_pAnimatedMeshKey[i].dwFrame,m_pAnimatedMeshKey+i);
		}
	}

	// 0번 키의 프레임이 0프레임이 되도록 리셋.
/*
	for (DWORD i=0; i<m_dwRotKeyNum; i++)
	{
		m_pRotKey[i].dwFrame -= 1;

	}
	for (i=0; i<m_dwPosKeyNum; i++)
	{
		m_pPosKey[i].dwFrame -= 1;

	}
	for (i=0; i<m_dwScaleKeyNum; i++)
	{
		m_pScaleKey[i].dwFrame -= 1;

	}
	for (i=0; i<m_dwAnimatedMeshKeyNum; i++)
	{
		m_pAnimatedMeshKey[i].dwFrame -= 1;
	}
*/
}
DWORD CMotionObject::ReadFile(void* pFP)
{
	FILE_MOTION_OBJECT_HEADER	header;
	
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(FILE_MOTION_OBJECT_HEADER));

	if (header.dwPosKeyNum)
	{
		CreatePosKeyList(header.dwPosKeyNum);
		m_dwPosKeyNum = header.dwPosKeyNum;
		dwLen += g_pFileStorage->FSRead(pFP,m_pPosKey,sizeof(POS_KEY)*m_dwPosKeyNum);

		
	}

	if (header.dwRotKeyNum)
	{
		CreateRotKeyList(header.dwRotKeyNum);
		m_dwRotKeyNum = header.dwRotKeyNum;
		dwLen += g_pFileStorage->FSRead(pFP,m_pRotKey,sizeof(ROT_KEY)*m_dwRotKeyNum);
		
	}

	if (header.dwScaleKeyNum)
	{
		CreateScaleKeyList(header.dwScaleKeyNum);
		m_dwScaleKeyNum = header.dwScaleKeyNum;
		dwLen += g_pFileStorage->FSRead(pFP,m_pScaleKey,sizeof(SCALE_KEY)*m_dwScaleKeyNum);

		
	}
	
	if (header.dwAnimatedMeshKeyNum)
	{
		m_dwAnimatedMeshKeyNum = header.dwAnimatedMeshKeyNum;
		m_pAnimatedMeshKey = new ANIMATED_MESH_KEY[m_dwAnimatedMeshKeyNum];
		for (DWORD j=0; j<m_dwAnimatedMeshKeyNum; j++)
		{
			dwLen += g_pFileStorage->FSRead(pFP,&m_pAnimatedMeshKey[j],sizeof(ANIMATED_MESH_KEY));

			if (m_pAnimatedMeshKey[j].dwVertexNum)
			{
				m_pAnimatedMeshKey[j].pv3Pos = new VECTOR3[m_pAnimatedMeshKey[j].dwVertexNum];
				dwLen += g_pFileStorage->FSRead(pFP,m_pAnimatedMeshKey[j].pv3Pos,sizeof(VECTOR3)*m_pAnimatedMeshKey[j].dwVertexNum);
//				memset(m_pAnimatedMeshKey[j].pv3Pos,0,sizeof(VECTOR3)*m_pAnimatedMeshKey[j].dwVertexNum);
				
			}
			if (m_pAnimatedMeshKey[j].dwTexVertexNum)
			{
				m_pAnimatedMeshKey[j].ptUV = new TVERTEX[m_pAnimatedMeshKey[j].dwTexVertexNum];
				dwLen += g_pFileStorage->FSRead(pFP,m_pAnimatedMeshKey[j].ptUV,sizeof(TVERTEX)*m_pAnimatedMeshKey[j].dwTexVertexNum);

	//			memset(m_pAnimatedMeshKey[j].ptUV,0,sizeof(TVERTEX)*m_pAnimatedMeshKey[j].dwTexVertexNum);
			}
		}
	}
	Initialize();
	m_dwNameLen = lstrlen(header.szObjectName);
	if (m_dwNameLen)
	{
		memcpy(m_szObjectName,header.szObjectName,m_dwNameLen);	
	}
	return dwLen;
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
				delete [] m_pAnimatedMeshKey[i].pv3Pos;
			if (m_pAnimatedMeshKey[i].ptUV)
				delete [] m_pAnimatedMeshKey[i].ptUV;
		}
		delete [] m_pAnimatedMeshKey;
		m_pAnimatedMeshKey = NULL;
		m_dwAnimatedMeshKeyNum = 0;

	}
}

/*
void	RotationFromQuaternion(VECTOR3* pv3Axis,float* pfRotAngle,QUARTERNION* pQ)
{
	//     fTheta = acosf(w) * 2.0f;
 //   v.x    = x / sinf( fTheta/2.0f );
 //   v.y    = y / sinf( fTheta/2.0f );
  //  v.z    = z / sinf( fTheta/2.0f );

    *pfRotAngle = acosf(pQ->w) * 2.0f;
    pv3Axis->x    = pQ->x / sinf( *pfRotAngle/2.0f );
    pv3Axis->y    = pQ->y / sinf( *pfRotAngle/2.0f );
    pv3Axis->z    = pQ->z / sinf( *pfRotAngle/2.0f );
}
*/
	/*
	// 애니메이션 0번 프레임에서의 키값과 모델데이타의 기본 키 값이 다르기 때문에 그 차이만큼을 뺀다.

	if (pBaseTM->fRotAng)
		SetRotationMatrix(&matBase,(VECTOR3*)&pBaseTM->fRotAxisX,(-1)*pBaseTM->fRotAng);
	else
		SetIdentityMatrix(&matBase);	
	
	MatrixFromQuaternion(&matFrame0,&m_pRotKey->Q);
	SetInverseMatrix(&matBaseInverse,&matBase);
	MatrixMultiply2(&matTrans,&matBaseInverse,&matFrame0);
	MatrixMultiply2(&matTemp,&matBase,&matTrans);
	MatrixMultiply2(pMat,pMat,&matTemp);
	*/

//	MatrixFromQuaternion(pMat,&m_pRotKey->Q);


//	MatrixMultiply2(pMat,&matTrans,pMat);
//	*pMat = matTrans;

/*	matFrame0._11 -= matBase._11;
	matFrame0._12 -= matBase._12;
	matFrame0._13 -= matBase._13;
	matFrame0._21 -= matBase._21;
	matFrame0._22 -= matBase._22;
	matFrame0._23 -= matBase._23;
	matFrame0._31 -= matBase._31;
	matFrame0._32 -= matBase._32;
	matFrame0._33 -= matBase._33;


	pMat->_11 += matFrame0._11;
	pMat->_12 += matFrame0._12;
	pMat->_13 += matFrame0._13;
	pMat->_21 += matFrame0._21;
	pMat->_22 += matFrame0._22;
	pMat->_23 += matFrame0._23;
	pMat->_31 += matFrame0._31;
	pMat->_32 += matFrame0._32;
	pMat->_33 += matFrame0._33;
*/

/*

	RotationFromQuaternion(&axis,&angle1,&m_pRotKey->Q);
	angle2 = angle1 - pBaseTM->fRotAng;
	
	RotationFromQuaternion(&axis,&angle1,&q);
	angle1 += angle2;
	SetRotationMatrix(pMat,&axis,angle1);
*/	