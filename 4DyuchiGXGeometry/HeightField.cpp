#include "../4DyuchiGXGFunc/global.h"
#include "HeightField.h"
#include "HFieldObject.h"
#include "geometry_global.h"
#include "CoGeometry.h"
#include "MeshQuadTree.h"
#include "global_variable.h"
#include "HFieldTools.h"

#define DEFAULT_LV0_INDEX_BUFFER_NUM	8

STDMETHODIMP CoHeightField::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_HeightField == refiid)
		*ppv = (IHeightField*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoHeightField::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoHeightField::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CoHeightField::CoHeightField()
{
	memset((char*)this+4,0,sizeof(CoHeightField)-4);
}


BOOL CoHeightField::Initialize(CoGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer)
{
	m_pGeometry = pGeometry;
	m_pRenderer = pRenderer;
	return TRUE;
}

BOOL __stdcall CoHeightField::Create(HFIELD_CREATE_DESC* pDesc,DWORD dwFlag)
{
	float		objsize;

	memcpy(&m_hfDesc,pDesc,sizeof(HFIELD_CREATE_DESC));

	m_hfDesc.pTexTable = new TEXTURE_TABLE[pDesc->dwTileTextureNum];
	memcpy(m_hfDesc.pTexTable,pDesc->pTexTable,sizeof(TEXTURE_TABLE)*pDesc->dwTileTextureNum);

	objsize = pDesc->fFaceSize * (float)pDesc->dwFacesNumPerObjAxis;
	
	m_hfDesc.dwObjNumX = pDesc->dwObjNumX;
	m_hfDesc.dwObjNumZ = pDesc->dwObjNumZ;
	
	if (!m_hfDesc.dwObjNumX || !m_hfDesc.dwObjNumZ)
		return FALSE;

	m_hfDesc.dwFacesNumX = pDesc->dwFacesNumPerObjAxis*m_hfDesc.dwObjNumX;
	m_hfDesc.dwFacesNumZ = pDesc->dwFacesNumPerObjAxis*m_hfDesc.dwObjNumZ;
		
	m_hfDesc.dwYFNumX = m_hfDesc.dwFacesNumX+1;
	m_hfDesc.dwYFNumZ = m_hfDesc.dwFacesNumZ+1;

	m_hfDesc.width = m_hfDesc.dwFacesNumX*pDesc->fFaceSize;
	m_hfDesc.height = m_hfDesc.dwFacesNumZ*pDesc->fFaceSize;

	m_hfDesc.right  = m_hfDesc.left + m_hfDesc.width;
	m_hfDesc.bottom = m_hfDesc.top + m_hfDesc.height;

	CreateYFList();
	CreateHFObjectList();
	
	
	BuildHField(NULL,dwFlag);
		
	SetVertexColor(0xffffffff);

	if (!(HFIELD_MODEL_LOAD_ENABLE_SHADE & dwFlag))
	{
		Optimize();
	}
	

	return TRUE;
}
BOOL __stdcall CoHeightField::LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum)
{
	BOOL		bResult = FALSE;
	if (!m_pIDIHeightField)
		goto lb_return;

	if (m_hfDesc.pTexTable)
		delete [] m_hfDesc.pTexTable;


	m_hfDesc.dwTileTextureNum = dwTileTextureNum;
	m_hfDesc.pTexTable = new TEXTURE_TABLE[dwTileTextureNum];
	memcpy(m_hfDesc.pTexTable,pTexTable,sizeof(TEXTURE_TABLE)*dwTileTextureNum);

	bResult = m_pIDIHeightField->LoadTilePalette(pTexTable,dwTileTextureNum);

lb_return:
	return bResult;

}

BOOL __stdcall CoHeightField::ReplaceTile(char* szFileName,DWORD dwTexIndex)
{
	BOOL	bResult = FALSE;
	if (dwTexIndex >= m_hfDesc.dwTileTextureNum)
		goto lb_return;

	if (!m_hfDesc.pTexTable)
		goto lb_return;

	memset(m_hfDesc.pTexTable[dwTexIndex].szTextureName,0,MAX_NAME_LEN);
	GetNameRemovePath(m_hfDesc.pTexTable[dwTexIndex].szTextureName,szFileName);
//	lstrcpy(m_hfDesc.pTexTable[dwTexIndex].szTextureName,szFileName);
	return m_pIDIHeightField->ReplaceTile(szFileName,dwTexIndex);
	

lb_return:
	return bResult;
}

BOOL CoHeightField::BuildHField(LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag)
{

	
	LOAD_CALLBACK_FUNC	pFunc = NULL;
	if (pCallbackDesc)
	{
		pFunc = pCallbackDesc->pFunc;
	}

	m_hfDesc.dwTriNumPerObj = m_hfDesc.dwFacesNumPerObjAxis*m_hfDesc.dwFacesNumPerObjAxis*2;
	m_hfDesc.dwVerticesNumPerObj = (m_hfDesc.dwFacesNumPerObjAxis+1)*(m_hfDesc.dwFacesNumPerObjAxis+1);
	
	m_fHFieldDetailRSPerObj = m_hfDesc.fFaceSize*(float)m_hfDesc.dwFacesNumPerObjAxis / 2.0f;

lb_settilenum:
	DWORD	div_const = 1;
	for (DWORD i=0; i<m_hfDesc.bDetailLevelNum-1; i++)
		div_const *= 2;

	m_hfDesc.dwTileNumPerObjAxis = m_hfDesc.dwFacesNumPerObjAxis / div_const;
	if (!m_hfDesc.dwTileNumPerObjAxis)
	{
		m_hfDesc.bDetailLevelNum--;
		goto lb_settilenum;
	}

	m_hfDesc.dwFacesNumPerTileAxis = m_hfDesc.dwFacesNumPerObjAxis / m_hfDesc.dwTileNumPerObjAxis;
		
	m_hfDesc.dwTileNumX = m_hfDesc.dwTileNumPerObjAxis * m_hfDesc.dwObjNumX;
	m_hfDesc.dwTileNumZ = m_hfDesc.dwTileNumPerObjAxis * m_hfDesc.dwObjNumZ;
	
	m_hfDesc.fTileSize = m_hfDesc.fFaceSize * (float)m_hfDesc.dwFacesNumPerTileAxis;
	
	if (!m_hfDesc.pwTileTable)
	{
		m_hfDesc.pwTileTable = new WORD[m_hfDesc.dwTileNumX*m_hfDesc.dwTileNumZ];

		for (DWORD i=0; i<m_hfDesc.dwTileNumX*m_hfDesc.dwTileNumZ; i++)
		{
		//	m_hfDesc.pwTileTable[i] = i % m_hfDesc.dwTileTextureNum;
			m_hfDesc.pwTileTable[i] = 0;
			
		}
	}	

	///////////////////////////////////////////////////////////////////////////////////////////////////



	m_pColMeshObjDescList = new COLLISION_MESH_OBJECT_DESC[m_dwObjNum];
	memset(m_pColMeshObjDescList,0,sizeof(COLLISION_MESH_OBJECT_DESC)*m_dwObjNum);

	// 렌더링 불가 옵션 처리
	if (!(dwFlag & HFIELD_MODEL_LOAD_NOT_RENDER))
	{
		m_pIDIHeightField = m_pRenderer->CreateHeightField(0);
		m_pIDIHeightField->StartInitialize(&m_hfDesc);
	}


	if (dwFlag & HFIELD_MODEL_LOAD_ENABLE_DRAW_ALPHAMAP)
	{
		m_pHFieldTools = new CHFieldTools;
		m_pHFieldTools->Initialize(&m_hfDesc,TRUE,m_dwTotalAlphaMapNum,&m_dwAlphaMapWidthHeight);
		m_hfDesc.bBlendEnable = 1;
		if (m_pIDIHeightField)
			m_pIDIHeightField->SetHFieldTileBlend(TRUE);
	}
	else
	{
		if (m_bEnableAlphaMap)
		{
			if (m_hfDesc.bBlendEnable == 1)
			{
				// 파일을 로드하는 경우만 해당된다.
				m_pHFieldTools = new CHFieldTools;
				m_pHFieldTools->Initialize(&m_hfDesc,FALSE,m_dwTotalAlphaMapNum,&m_dwAlphaMapWidthHeight);
			}
		}
	}

	HFIELD_OBJECT_DESC	hfdesc;
	memset(&hfdesc,0,sizeof(HFIELD_OBJECT_DESC));

#ifdef _DEBUG
	DWORD	dwPrvTick = GetTickCount();
	DWORD	dwElapsedTick;
#endif

	for (DWORD z=0; z<m_hfDesc.dwObjNumZ; z++)
	{
		for (DWORD x=0; x<m_hfDesc.dwObjNumX; x++)
		{

			if (pFunc)
			{
				pFunc(*pCallbackDesc->pdwCurCount,pCallbackDesc->dwTotalCount,NULL);
				(*pCallbackDesc->pdwCurCount)++;
			}


			hfdesc.dwPosX = x*m_hfDesc.dwFacesNumPerObjAxis;
			hfdesc.dwPosZ = z*m_hfDesc.dwFacesNumPerObjAxis;
			hfdesc.dwObjPosX = x;
			hfdesc.dwObjPosZ = z;

			m_pHFieldObjectList[m_hfDesc.dwObjNumX*z+x].Initialize(this,&hfdesc,dwFlag);


			m_pHFieldObjectList[m_hfDesc.dwObjNumX*z+x].SetColMeshDesc(
				&m_pColMeshObjDescList[m_hfDesc.dwObjNumX*z+x]
				);

		}
	}
#ifdef _DEBUG
	dwElapsedTick = GetTickCount()-dwPrvTick;
	dwPrvTick = GetTickCount();
#endif
	ResetCollisionMesh();

#ifdef _DEBUG
	dwElapsedTick = GetTickCount()-dwPrvTick;
	dwPrvTick = GetTickCount();
#endif
	
	// 렌더링 불가 필드라면 인덱스버퍼를 만들 필요도 없다.
	if (m_pIDIHeightField)
	{
		CreateIndexBuffer(m_hfDesc.dwFacesNumPerObjAxis,m_hfDesc.bDetailLevelNum,m_hfDesc.dwFacesNumPerTileAxis);
		m_pIDIHeightField->EndInitialize();
	}
#ifdef _DEBUG
	dwElapsedTick = GetTickCount()-dwPrvTick;
	dwPrvTick = GetTickCount();
#endif
	SetHeightFieldDetail(0);
	return TRUE;
}
void __stdcall CoHeightField::SetHeightFieldDetail(DWORD dwDetailLevel)
{
	float	fRs = m_pHFieldObjectList[0].GetColMeshDesc()->boundingSphere.fRs;

	for (DWORD i=0; i<m_hfDesc.bDetailLevelNum; i++)
	{
		m_fDetailRS[i] = fRs * (float)(i+1) + (float)dwDetailLevel*fRs;
	}
/*
	m_fDetailRS[0] = fRs + (float)dwDetailLevel*fRs;

	
	m_fDetailRS[1] = m_fDetailRS[0]*2.0f;
	m_fDetailRS[2] = m_fDetailRS[0]*3.0f;
	m_fDetailRS[3] = m_fDetailRS[0]*4.0f;*/
}
BOOL CoHeightField::CreateIndexBuffer(DWORD dwFacesNumPerObj,DWORD dwDetailLevelNum,DWORD dwSkipStep)
{
	DWORD		dwPitch = dwFacesNumPerObj+1;
	DWORD		dwStep = 1;
	DWORD		dwBufferNum = m_hfDesc.dwIndexBufferNumLV0;
	
	DWORD		dwMemSize = 0;

	for (DWORD i=0; i<dwDetailLevelNum; i++)
	{
		DWORD		dwIndicesNum = dwFacesNumPerObj*dwFacesNumPerObj*2*3;
		
		WORD*		pIndex;
		for (DWORD j=0; j<16; j++)
		{
		//	pIndex = m_IndexTable[i][j].pIndex = new WORD[dwIndicesNum];
		//	m_IndexTable[i][j].dwIndicesNum = dwIndicesNum;



			m_pIDIHeightField->CreateIndexBuffer(dwIndicesNum,i,j,dwBufferNum);

			m_pIDIHeightField->LockIndexBufferPtr(&pIndex,i,j);
			BuildIndexBuffer((WORD*)pIndex,dwFacesNumPerObj,dwPitch,dwStep,dwIndicesNum);
			
			if (j & 0x1)	// 0001
				SetBoundaryUp((WORD*)pIndex,dwFacesNumPerObj,dwPitch,dwStep);	

			if (j & 2)		// 0010
				SetBoundaryLeft((WORD*)pIndex,dwFacesNumPerObj,dwPitch,dwStep);	

			if (j & 4)		// 0100
				SetBoundaryDown((WORD*)pIndex,dwFacesNumPerObj,dwPitch,dwStep);	

			if (j & 8)		// 1000
				SetBoundaryRight((WORD*)pIndex,dwFacesNumPerObj,dwPitch,dwStep);	

			DivIndexBuffer((WORD*)pIndex,dwFacesNumPerObj,dwSkipStep,dwStep,dwIndicesNum);

			m_pIDIHeightField->UnlcokIndexBufferPtr(i,j);		
		}
		if (i == dwDetailLevelNum-1)
		{
			dwBufferNum = (DWORD)( (float)m_dwObjNum * (1.5f/4.0f) );
			if (dwBufferNum > MAX_HFINDEX_BUFER_NUM)
				dwBufferNum = MAX_HFINDEX_BUFER_NUM;

		}

		dwMemSize += (dwIndicesNum*sizeof(WORD)*dwBufferNum);
		m_pIDIHeightField->InitiallizeIndexBufferPool(i,dwIndicesNum,dwBufferNum);

		//dwBufferNum *= 3;
		dwBufferNum *= 2;
		

		dwFacesNumPerObj /= 2;
		dwStep *= 2;
		dwSkipStep /= 2;
		
	}
	return TRUE;
}

void CoHeightField::BuildIndexBuffer(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep,DWORD dwIndicesNum)
{
	DWORD		dwFaceCount = 0;
	DWORD		z,x;
	for (z=0; z<dwFacesNumPerObj; z++)
	{
		for (x=0; x<dwFacesNumPerObj; x++)
		{

			pIndex[dwFaceCount*6+0] = (WORD)((x + z*dwPitch) * dwStep);			// 왼쪽 위
			pIndex[dwFaceCount*6+1] = (WORD)((x + (z+1)*dwPitch) * dwStep);		// 왼쪽 아래
			pIndex[dwFaceCount*6+2] = (WORD)(((x+1) + z*dwPitch) * dwStep);		// 오른쪽 위

			pIndex[dwFaceCount*6+3] = (WORD)(((x+1) + z*dwPitch) * dwStep);		// 오른쪽 위
			pIndex[dwFaceCount*6+4] = (WORD)((x + (z+1)*dwPitch) * dwStep);		// 왼쪽 아래 
			pIndex[dwFaceCount*6+5] = (WORD)(((x+1) + (z+1)*dwPitch) * dwStep);	// 오른쪽 아래 

			dwFaceCount++;
		}
	}
}
void CoHeightField::DivIndexBuffer(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwSkipFacesNum,DWORD dwStep,DWORD dwIndicesNum)
{
	
	// 인덱스 버퍼를 격자모양으로 쪼개서 저장한다.
	WORD*	pTemp = new WORD[dwIndicesNum];

	DWORD		x_block,z_block,x,z;
	DWORD		dwBlockNumPerObj = dwFacesNumPerObj / dwSkipFacesNum;
	DWORD		dwIndexCount = 0;
	DWORD		dwIndicesNumPerObj = dwFacesNumPerObj*3*2;


	for (z_block = 0; z_block < dwBlockNumPerObj; z_block++)
	{
		for (x_block = 0; x_block < dwBlockNumPerObj; x_block++)
		{
			WORD* pEntry = pIndex + x_block*dwSkipFacesNum*6 + z_block*dwSkipFacesNum*dwIndicesNumPerObj;
			for (z=0; z<dwSkipFacesNum; z++)
			{
				for (x=0; x<dwSkipFacesNum; x++)
				{
					for (DWORD i=0; i<6; i++)
					{
						pTemp[dwIndexCount] = pEntry[x*6+i];
						dwIndexCount++;
					}
				}
				pEntry += dwIndicesNumPerObj;
			}
		}
	}
	memcpy(pIndex,pTemp,sizeof(WORD)*dwIndicesNum);
	
	delete [] pTemp;
}
void CoHeightField::SetBoundaryRight(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep)
{
	DWORD xpos = (dwFacesNumPerObj-1)*6;
	DWORD xpitch = dwFacesNumPerObj*3*2;

	DWORD	znum = dwFacesNumPerObj/2;
	for (DWORD z=0; z<znum; z++)
	{
		pIndex[xpos + (z*2)*xpitch + 5] = (WORD)(pIndex[xpos + (z*2)*xpitch + 5] + dwPitch*dwStep);
		pIndex[xpos + (z*2+1)*xpitch + 2] = (WORD)(pIndex[xpos + (z*2+1)*xpitch + 2] + dwPitch*dwStep);
		pIndex[xpos + (z*2+1)*xpitch + 3] = (WORD)(pIndex[xpos + (z*2+1)*xpitch + 3] + dwPitch*dwStep);
	}
}

void CoHeightField::SetBoundaryLeft(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep)
{
	DWORD xpos = 0;
	DWORD xpitch = dwFacesNumPerObj*3*2;

	DWORD	znum = dwFacesNumPerObj/2;
	for (DWORD z=0; z<znum; z++)
	{
		pIndex[xpos + (z*2)*xpitch + 1] = (WORD)(pIndex[xpos + (z*2)*xpitch + 1] - dwPitch*dwStep);
		pIndex[xpos + (z*2)*xpitch + 4] = (WORD)(pIndex[xpos + (z*2)*xpitch + 4] - dwPitch*dwStep);
		pIndex[xpos + (z*2+1)*xpitch + 0] = (WORD)(pIndex[xpos + (z*2+1)*xpitch + 0] - dwPitch*dwStep);
		
	}
}
void CoHeightField::SetBoundaryUp(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep)
{
	DWORD	xnum = dwFacesNumPerObj/2;
	for (DWORD x=0; x<xnum; x++)
	{
		pIndex[x*2*6 + 2] = (WORD)(pIndex[x*2*6 + 2] + dwPitch*dwStep);
		pIndex[x*2*6 + 3] = (WORD)(pIndex[x*2*6 + 3] + dwPitch*dwStep);
		pIndex[(x*2+1)*6 + 0] = (WORD)(pIndex[(x*2+1)*6 + 0] - dwStep);
	}
}
void CoHeightField::SetBoundaryDown(WORD* pIndex,DWORD dwFacesNumPerObj,DWORD dwPitch,DWORD dwStep)
{
	DWORD xpitch = dwFacesNumPerObj*3*2;
	DWORD zpos = dwFacesNumPerObj-1;
	DWORD	xnum = dwFacesNumPerObj/2;
	
	for (DWORD x=0; x<xnum; x++)
	{
		pIndex[x*2*6 + zpos*xpitch + 5] = (WORD)(pIndex[x*2*6 + zpos*xpitch + 5] + dwStep);
		pIndex[(x*2+1)*6 + zpos*xpitch + 1] = (WORD)(pIndex[(x*2+1)*6 + zpos*xpitch + 1] - dwPitch*dwStep);
		pIndex[(x*2+1)*6 + zpos*xpitch + 4] = (WORD)(pIndex[(x*2+1)*6 + zpos*xpitch + 4] - dwPitch*dwStep);
	}
}
void __stdcall CoHeightField::CalcY(float* py,DWORD dwPosX,DWORD dwPosZ)
{
	*py = m_hfDesc.pyfList[dwPosX + dwPosZ*m_hfDesc.dwYFNumX];
}
void __stdcall CoHeightField::CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosZ)
{
	::CalcXZ(
		px,
		pz,
		dwPosX,
		dwPosZ,
		m_hfDesc.top,
		m_hfDesc.left,
		m_hfDesc.bottom,
		m_hfDesc.right,
		m_hfDesc.width,
		m_hfDesc.height,
		m_hfDesc.dwFacesNumX,
		m_hfDesc.dwFacesNumZ);
}

void CoHeightField::CreateYFList()
{
	m_dwYFNum = m_hfDesc.dwYFNumX*m_hfDesc.dwYFNumZ;
	m_hfDesc.pyfList = new float[m_dwYFNum];
	memset(m_hfDesc.pyfList,0,sizeof(float)*m_dwYFNum);
}
void CoHeightField::CreateHFObjectList()
{
	m_dwObjNum = m_hfDesc.dwObjNumX*m_hfDesc.dwObjNumZ;
	m_pHFieldObjectList = new CHFieldObject[m_dwObjNum];

	CHFieldObject*	pSideObj;
	// 가운데 알맹이들
	for (int z=0; z<(int)m_hfDesc.dwObjNumZ; z++)
	{
		for (int x=0; x<(int)m_hfDesc.dwObjNumX; x++)
		{
			if (z>0)
			{

				pSideObj = &m_pHFieldObjectList[x + (z-1)*m_hfDesc.dwObjNumX];
				m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].SetSideObject(pSideObj,0);			// UP
			}

			if (x>0)
			{
				pSideObj = &m_pHFieldObjectList[(x-1) + z*m_hfDesc.dwObjNumX];
				m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].SetSideObject(pSideObj,1);			// LEFT
			}

			if (z < (int)(m_hfDesc.dwObjNumZ-1))
			{

				pSideObj = &m_pHFieldObjectList[x + (z+1)*m_hfDesc.dwObjNumX];
				m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].SetSideObject(pSideObj,2);			// DOWN
			}
			if (x < (int)(m_hfDesc.dwObjNumX-1))
			{
				pSideObj = &m_pHFieldObjectList[(x+1) + z*m_hfDesc.dwObjNumX];
				m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].SetSideObject(pSideObj,3);			// RIGHT
			}
		}
	}
}

BOOL __stdcall CoHeightField::ReadFile(char* szFileName,DWORD dwIndexBufferNumLV0,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	char	szRCFileName[MAX_NAME_LEN];

	void*	pFPVCL;
	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		goto lb_return;
	}

	DWORD		dwVersion;
	g_pFileStorage->FSRead(pFP,&dwVersion,sizeof(DWORD));
#ifdef _DEBUG
	if (dwVersion > 0x00000010 || !dwVersion)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CoModel::ReadFile(), if (dwVersion > 0x00000010 || !dwVersion, File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	ReadHFieldDesc(pFP);

	if (dwIndexBufferNumLV0 > 0 && dwIndexBufferNumLV0 < MAX_HFINDEX_BUFER_NUM)
	{
		m_hfDesc.dwIndexBufferNumLV0 = dwIndexBufferNumLV0;
	}
	ReadTextureTable(pFP);
	ReadTileTable(pFP);
	ReadAlphaMapHeader(pFP);

	BuildHField(pCallbackDesc,dwFlag);

	memset(szRCFileName,0,MAX_NAME_LEN);
	RemoveExt(szRCFileName,szFileName);
	lstrcat(szRCFileName,".am2");

	ReadAlphaMapBody(szRCFileName);

	memset(szRCFileName,0,MAX_NAME_LEN);
	RemoveExt(szRCFileName,szFileName);
	lstrcat(szRCFileName,".vch");

	pFPVCL = g_pFileStorage->FSOpenFile(szRCFileName,FSFILE_ACCESSMODE_BINARY);
	if (pFPVCL)
	{
		for (DWORD i=0; i<m_dwObjNum; i++)
		{
			m_pHFieldObjectList[i].ReadVertexColor(pFPVCL);
		}
		g_pFileStorage->FSCloseFile(pFPVCL);
	}
	else
		SetVertexColor(0xffffffff);

	bResult = TRUE;

	if (!(HFIELD_MODEL_LOAD_ENABLE_SHADE & dwFlag))
	{
		Optimize();
	}

	lstrcpy(m_szFileName,szFileName);

	
lb_closefile:
	g_pFileStorage->FSCloseFile(pFP);

lb_return:
	return bResult;
}

void __stdcall CoHeightField::SetVertexColor(DWORD dwColor)
{
	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].SetVertexColor(dwColor);
	}
}
BOOL __stdcall CoHeightField::WriteFile(char* szFileName,DWORD dwFlag)
{
	BOOL		bResult = FALSE;

	char		szNewVCLFileName[MAX_NAME_LEN];

	HFIELD_DESC hfDesc;
	memcpy(&hfDesc,&m_hfDesc,sizeof(HFIELD_DESC));

	float*		pyfList = hfDesc.pyfList;

	FILE* fp = fopen(szFileName,"wb");
	if (!fp)
		goto lb_return;

	// 버젼정보..
	DWORD		dwVersion;
	dwVersion = HEIGHT_FIELD_VERSION;
	fwrite(&dwVersion,sizeof(DWORD),1,fp);

	hfDesc.pyfList = NULL;
	hfDesc.pTexTable = NULL;
	hfDesc.pwTileTable = NULL;

	if (m_pHFieldTools)
	{
		if (m_pHFieldTools->GetAllocatedAlphaMapNum() && m_bEnableDrawAlphaMap)
		{
			hfDesc.bBlendEnable = 1;
		}
	}

	fwrite(&hfDesc,sizeof(HFIELD_DESC),1,fp);
	fwrite(pyfList,sizeof(float),hfDesc.dwYFNumX*hfDesc.dwYFNumZ,fp);


	WriteTextureTable(fp);
	WriteTileTable(fp);
	WriteAlphaMapHeader(fp);

	memset(szNewVCLFileName,0,MAX_NAME_LEN);
	RemoveExt(szNewVCLFileName,szFileName);
	lstrcat(szNewVCLFileName,".am2");

	WriteAlphaMapBody(szNewVCLFileName);

	bResult = TRUE;

	fclose(fp);

	EndShadeLightMapObject();


	memset(szNewVCLFileName,0,MAX_NAME_LEN);
	RemoveExt(szNewVCLFileName,szFileName);
	lstrcat(szNewVCLFileName,".vch");
	remove(szNewVCLFileName);
	rename(m_szTempVCLFileName,szNewVCLFileName);

lb_return:

	return TRUE;
}




DWORD CoHeightField::WriteTileTable(FILE* fp)
{
	DWORD	oldPos = ftell(fp);

	fwrite(&m_hfDesc.dwTileNumX,sizeof(DWORD),1,fp);
	fwrite(&m_hfDesc.dwTileNumZ,sizeof(DWORD),1,fp);
	fwrite(m_hfDesc.pwTileTable,sizeof(WORD),m_hfDesc.dwTileNumX*m_hfDesc.dwTileNumZ,fp);

	return (ftell(fp)-oldPos);

}
DWORD CoHeightField::WriteTextureTable(FILE* fp)
{
	DWORD	oldPos = ftell(fp);

	fwrite(&m_hfDesc.dwTileTextureNum,sizeof(DWORD),1,fp);
	fwrite(m_hfDesc.pTexTable,sizeof(TEXTURE_TABLE),m_hfDesc.dwTileTextureNum,fp);
	

	return (ftell(fp)-oldPos);
}


DWORD CoHeightField::ReadTileTable(void* pFP)
{
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&m_hfDesc.dwTileNumX,sizeof(DWORD));
	dwLen += g_pFileStorage->FSRead(pFP,&m_hfDesc.dwTileNumZ,sizeof(DWORD));

	m_hfDesc.pwTileTable = new WORD[m_hfDesc.dwTileNumX*m_hfDesc.dwTileNumZ];

	dwLen += g_pFileStorage->FSRead(pFP,m_hfDesc.pwTileTable,sizeof(WORD)*m_hfDesc.dwTileNumX*m_hfDesc.dwTileNumZ);


	return dwLen;
}
DWORD CoHeightField::ReadTextureTable(void* pFP)
{
	DWORD	dwLen = 0;

	DWORD	dwTableNum;
	dwLen += g_pFileStorage->FSRead(pFP,&dwTableNum,sizeof(DWORD));

	m_hfDesc.pTexTable = new TEXTURE_TABLE[dwTableNum];
	dwLen += g_pFileStorage->FSRead(pFP,m_hfDesc.pTexTable,sizeof(TEXTURE_TABLE)*dwTableNum);

	return dwLen;

}
DWORD CoHeightField::ReadHFieldDesc(void* pFP)
{
	DWORD	dwLen = 0;

	dwLen += g_pFileStorage->FSRead(pFP,&m_hfDesc,sizeof(HFIELD_DESC));

	CreateYFList();

	dwLen += g_pFileStorage->FSRead(pFP,m_hfDesc.pyfList,sizeof(float)*m_dwYFNum);

	CreateHFObjectList();

	return dwLen;
}


BOOL __stdcall CoHeightField::ResetCollisionMesh()
{
	DWORD		i;
		
	for (i=0;i<m_dwObjNum; i++)
		m_pHFieldObjectList[i].ResetCollisionMesh();
	
	DWORD		dwWorldBoundVertexNum = m_hfDesc.dwObjNumX*m_hfDesc.dwObjNumZ*8;
	VECTOR3*	pv3WorldBound = new VECTOR3[dwWorldBoundVertexNum];
	VECTOR3*	pv3BoundEntry = pv3WorldBound;


	for (i=0;i<m_dwObjNum; i++)
	{
		memcpy(pv3BoundEntry,m_pHFieldObjectList[i].GetColMeshDesc()->boundingBox.v3Oct,sizeof(VECTOR3)*8);
		pv3BoundEntry += 8;
	}
	CalcBoundingMesh(pv3WorldBound,dwWorldBoundVertexNum,&m_colWolrdBoundingMesh);
	delete [] pv3WorldBound;

	return TRUE;
}
BOOL __stdcall CoHeightField::GetIntersectPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex)
{
	BOOL		bResult = FALSE;
	
    VECTOR3		v3PickRayDir;
    VECTOR3		v3PickRayOrig;
	MATRIX4	matProj,matView;

	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);

	VIEWPORT*	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);

	POINT p;
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	CalcRay(&v3PickRayOrig,&v3PickRayDir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);
	Normalize(&v3PickRayDir,&v3PickRayDir);
	VECTOR3_MUL_FLOAT(&v3PickRayDir,&v3PickRayDir,pVP->ViewVolume.fFar);
    

	DWORD		dwPosX,dwPosZ;
	if (!GetIntersectPointWithRay(pv3IntersectPoint,pfDist,&dwPosX,&dwPosZ,&v3PickRayOrig,&v3PickRayDir))
		goto lb_return;

	*pfDist = CalcDistance(&v3PickRayOrig,pv3IntersectPoint);
	bResult = TRUE;
lb_return:

	return bResult;
}


void __stdcall CoHeightField::GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc)
{
//	ResetCollisionMesh();
	*pColDesc = m_colWolrdBoundingMesh;
}
BOOL __stdcall CoHeightField::SetYFacorPosList(float* pyfList,DWORD dwYFNumX,DWORD dwYFNumZ)
{
	BOOL	bResult = FALSE;
	if (dwYFNumX != m_hfDesc.dwYFNumX || dwYFNumZ != m_hfDesc.dwYFNumZ)
		goto lb_return;

	memcpy(m_hfDesc.pyfList,pyfList,sizeof(float)*dwYFNumX*dwYFNumZ);
	
	DWORD i;
	for (i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].UpdateVertexPos();
		
	}
	ResetCollisionMesh();
	bResult = TRUE;

lb_return:
	return bResult;
}

void __stdcall CoHeightField::EnableRender(DWORD dwObjIndex)
{
	if (dwObjIndex < m_dwObjNum)
	{	
		m_pHFieldObjectList[dwObjIndex].EnableRender();
	}
}	
void __stdcall CoHeightField::DisableRender(DWORD dwObjIndex)
{
	if (dwObjIndex < m_dwObjNum)
	{	
		m_pHFieldObjectList[dwObjIndex].DisableRender();
	}
}

void __stdcall CoHeightField::FreezeDetailLevel(DWORD dwObjIndex,DWORD dwDetailLevel)
{
	if (dwObjIndex < m_dwObjNum)
	{	
		m_pHFieldObjectList[dwObjIndex].UnFreezeDetail();
		m_pHFieldObjectList[dwObjIndex].SetDetailLevel(dwDetailLevel);
		m_pHFieldObjectList[dwObjIndex].FreezeDetail();
		
	}
}
void __stdcall CoHeightField::UnFreezeDetailLevel(DWORD dwObjIndex)
{
	if (dwObjIndex < m_dwObjNum)
	{
		m_pHFieldObjectList[dwObjIndex].UnFreezeDetail();
	}
}


BOOL __stdcall CoHeightField::RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag)
{
	// 이 함수는 몽땅 렌더링될 하이트필드가 몽땅 큐에 들어간 다음 나중에 일괄적으로 호출된다.
	// CoGeometry::RenderHField()에서 이미 클립핑이 된 상태이다.
		

	BOOL			bResult = FALSE;

	float		fDistance;
	

	if (m_pHFieldObjectList[dwObjIndex].IsInvisible())
		goto lb_return;

	fDistance = 0;					// 가장 먼저 찍을것이므로..일단 0


	COLLISION_MESH_OBJECT_DESC*		pColMeshDesc = NULL;
	LIGHT_INDEX_DESC*				pAppliedLightIndex = NULL;
	DWORD							dwAppliedLightNum = 0;
	LIGHT_INDEX_DESC*				pAppliedSpotLightIndex = NULL;
	DWORD							dwAppliedSpotLightNum = 0;

	dwAppliedLightNum = 0;
	dwAppliedSpotLightNum = 0;


	dwAppliedLightNum = m_pGeometry->GetDynamicRTLightNum();
	if (dwAppliedLightNum)
	{
		pAppliedLightIndex = m_pGeometry->AllocLightIndex(dwAppliedLightNum);
		if (!pAppliedLightIndex)
			dwAppliedLightNum = 0;
	}
	
	// 라이트 범위가 오브젝트에 닿지 않는 경우는 클립	
	pColMeshDesc = m_pHFieldObjectList[dwObjIndex].GetColMeshDesc();
	dwAppliedLightNum = ClipLightWithRS(pAppliedLightIndex,pColMeshDesc,m_pGeometry->GetDynamicRTLightDescBuffer(),dwAppliedLightNum,0);
	

	// 스폿 라이트가 있는 경우..
	if (m_pGeometry->GetSpotLightNum())
	{
		pAppliedSpotLightIndex = m_pGeometry->AllocLightIndex(m_pGeometry->GetSpotLightNum());
		if (!pAppliedSpotLightIndex)
		{
			dwAppliedSpotLightNum = 0;
			goto lb_render_mesh;
		}

		for (DWORD i=0; i<m_pGeometry->GetSpotLightNum(); i++)
		{
			if (ClipRenderObjectWithViewVolume(m_pGeometry->GetSpotLightViewVolume(i),pColMeshDesc,0))
			{
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].bLightIndex = (BYTE)i;
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].pMtlHandle = m_pGeometry->GetSpotLightDesc(i)->pMtlHandle;
				pAppliedSpotLightIndex[dwAppliedSpotLightNum].bTexOP = (BYTE)( m_pGeometry->GetSpotLightDesc(i)->dwProjTexIndex & 0x000000ff );

				dwAppliedSpotLightNum++;
			}
		}
	}
lb_render_mesh:

	// 상하좌우의 오브젝트를 체크해서 이음매처리 방법을 결정한다.
	m_pHFieldObjectList[dwObjIndex].SetPositionMask();
	
	fDistance = DotProduct(&m_pGeometry->INL_GetCameraEyePlane()->v3Up,&pColMeshDesc->boundingSphere.v3Point) 
				+ m_pGeometry->INL_GetCameraEyePlane()->D;
	
	m_pRenderer->RenderMeshObject(m_pHFieldObjectList[dwObjIndex].GetDeviceObject(),
		0,
		fDistance,				// 카메라 eye평면으로부터의 거리 
		dwAlpha,
		pAppliedLightIndex,dwAppliedLightNum,
		pAppliedSpotLightIndex,dwAppliedSpotLightNum,					// 스폿라이트 인덱스 포인터와 갯수.
		0,
		0,
		dwFlag);

	if (m_pGeometry->INL_GetDrawDebugFlag())
	{
		m_pRenderer->RenderBox(pColMeshDesc->boundingBox.v3Oct,0xff00ff00);
//		m_pRenderer->RenderSphere(&pColMeshDesc->boundingSphere.v3Point,pColMeshDesc->boundingSphere.fRs,0xffffffff);
	}	
	bResult = TRUE;

lb_return:
	return bResult;
}



void CoHeightField::CalcDetail(DWORD dwObjIndex,VECTOR3* pv3ViewPoint)
{
	COLLISION_MESH_OBJECT_DESC*		pColMeshDesc = m_pHFieldObjectList[dwObjIndex].GetColMeshDesc();
	float fDistance = CalcDistance(pv3ViewPoint,&pColMeshDesc->boundingSphere.v3Point);

	DWORD		dwDetailLevel = m_hfDesc.bDetailLevelNum-1;
	for (DWORD i=0; i<m_hfDesc.bDetailLevelNum-1; i++)
	{
		if (fDistance < (pColMeshDesc->boundingSphere.fRs + m_fDetailRS[i]))
		{
			dwDetailLevel = i;
			goto lb_set;

		}
	}
/*
	dwDetailLevel = (DWORD)(fDistance / m_fHFieldDetailRSPerObj);
	if (dwDetailLevel > m_hfDesc.dwDetailLevelNum-1)
		dwDetailLevel = m_hfDesc.dwDetailLevelNum-1;

	if (fDistance < pColMeshDesc->boundingSphere.fRs)
		dwDetailLevel = 0;

	
*/

/*
	for (i=1; i<m_dwDetailLevelNum+1; i++)
	{
		float	fRs = m_fHFieldDetailRSPerObj* (float)i;
	//	if (fDistance <= m_fHFieldDetailRSPerObj + fRs)
		if (fDistance <= fRs)
		{
			dwDetailLevel = i-1;
			break;
		}
	}
		
	/*for (i=0; i<m_dwDetailLevelNum; i++)
	{
		if (fDistance < m_fDetailDistance[i] + m_fHFieldDetailRSPerObj)
		{
			dwDetailLevel = i;
			break;
		}
	}*/
lb_set:
	m_pHFieldObjectList[dwObjIndex].SetDistanceFromViewPoint(fDistance);
	m_pHFieldObjectList[dwObjIndex].SetDetailLevel(dwDetailLevel);
}


void CoHeightField::SetDetailLevel(DWORD dwObjIndex,DWORD dwDetailLevel)
{
	m_pHFieldObjectList[dwObjIndex].SetDetailLevel(dwDetailLevel);
}


void CoHeightField::MakeHeightFieldHigh(DWORD dwX, DWORD dwZ, float fHeight, float fRadius,DWORD dwActionType)
{
	int		xnum,ynum;
	RECT	rect;		// 검사할 범위.

	VECTOR3 Fact;
	VECTOR3	PickFact;
	
	PickFact.y = m_hfDesc.pyfList[dwX+dwZ*m_hfDesc.dwYFNumX];
	CalcXZ(&PickFact.x,&PickFact.z,dwX,dwZ);

	ynum = xnum = (int)( (fRadius / m_hfDesc.fFaceSize) + 0.5f );
	rect.left = (int)dwX - xnum - 1;
	rect.right = (int)dwX + xnum + 1;
	rect.top = (int)dwZ - ynum - 1;
	rect.bottom = (int)dwZ + ynum + 1;

	if (rect.left < 0)
		rect.left = 0;
	
	if (rect.top < 0)
		rect.top = 0;
	
	if (rect.right > (int)m_hfDesc.dwYFNumX)
		rect.right = (int)m_hfDesc.dwYFNumX;

	if (rect.bottom > (int)m_hfDesc.dwYFNumZ)
		rect.bottom = (int)m_hfDesc.dwYFNumZ;

	for(int j = rect.top; j < rect.bottom; j++)
	{
		for(int i = rect.left; i < rect.right; i++)
		{
			Fact.y = m_hfDesc.pyfList[i + j*m_hfDesc.dwYFNumX];
			CalcXZ(&Fact.x,&Fact.z,i,j);
			// 거리 체크.
//			float	fTemp	=	(Fact.x-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].x)*(Fact.x-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].x) + (Fact.y-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].y)*(Fact.y-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].y) + (Fact.z-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].z)*(Fact.z-hfDesc.pzfList[dwX+dwZ*hfDesc.dwYFNumX].z);
			float	fTemp2	=	(Fact.x-PickFact.x)*(Fact.x-PickFact.x) + (Fact.z-PickFact.z)*(Fact.z-PickFact.z);
			
			if( fTemp2 > fRadius*fRadius)
				continue;

			// 실제로 지형을 올린다.
			float	fDist = (float)sqrt(fTemp2);
			Fact.y = Fact.y + (cosf( 3.1415926f * fDist / fRadius)+1.0f)*fHeight/2;
			m_hfDesc.pyfList[i + j*m_hfDesc.dwYFNumX]	=	Fact.y;
		}
	}

//	Update(HEIGHT_FIELD_UPDATE_TYPE_VERTEX_POS);
	
	// 갱신할 오브젝트만 갱신한다.
	DWORD	xstart = rect.left / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	xend = rect.right / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	zstart = rect.top / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	zend = rect.bottom / m_hfDesc.dwFacesNumPerObjAxis;

	for (DWORD i=xstart; i<=xend; i++)
	{
		for (DWORD j=zstart; j<=zend; j++)
		{
			m_pHFieldObjectList[i + j*m_hfDesc.dwObjNumX].UpdateVertexPos();
		}
	}
}

DWORD __stdcall CoHeightField::GetXZFrom3DCoord(HFIELD_POS* phfPos,DWORD dwMaxHFPosNum,VECTOR3* pv3IntersectPoint,float fRs)
{
	DWORD	dwResult = 0;
	DWORD	dwCenterX,dwCenterZ;
	

	if (pv3IntersectPoint->x < m_hfDesc.left)
		goto lb_return;

	if (pv3IntersectPoint->x > m_hfDesc.right)
		goto lb_return;

	if (pv3IntersectPoint->z < m_hfDesc.top)
		goto lb_return;

	if (pv3IntersectPoint->z > m_hfDesc.bottom)
		goto lb_return;

	dwCenterX = (DWORD)((pv3IntersectPoint->x - m_hfDesc.left) / m_hfDesc.fFaceSize);
	dwCenterZ = (DWORD)((pv3IntersectPoint->z - m_hfDesc.top) / m_hfDesc.fFaceSize);


	
	DWORD_RECT	rect;
	GetVertexPosRect(&rect,dwCenterX,dwCenterZ,fRs);


	float	fMinDistance;
	float	fDist;
	VECTOR3	v3Point;

	DWORD	x,z;
	DWORD	dwSelectedX,dwSelectedZ;
	dwSelectedX = 0xffffffff;
	dwSelectedZ = 0xffffffff;

	fMinDistance = 900000.0f;
	for (x=rect.left; x<=rect.right; x++)
	{
		for (z=rect.top; z<=rect.bottom; z++)
		{
			CalcY(&v3Point.y,x,z);
			CalcXZ(&v3Point.x,&v3Point.z,x,z);

			fDist = CalcDistance(&v3Point,pv3IntersectPoint);
			if (fDist < fRs)
			{
				if (dwResult >= dwMaxHFPosNum)
					goto lb_return;

				phfPos[dwResult].dwX = x;
				phfPos[dwResult].dwZ = z;
				dwResult++;
			}
		}
	}
lb_return:
	return dwResult;
}
void CoHeightField::GetVertexPosRect(DWORD_RECT* pRect,DWORD dwCenterVertexX,DWORD dwCenterVertexZ,float fRadius)
{
	int		left,right,top,bottom,xnum,ynum;
	
	ynum = xnum = (int)( (fRadius / m_hfDesc.fFaceSize) + 0.5f );
	left = (int)dwCenterVertexX - xnum - 1;
	right = (int)dwCenterVertexX + xnum + 1;
	top  = (int)dwCenterVertexZ - ynum - 1;
	bottom = (int)dwCenterVertexZ + ynum + 1;

	if (left < 0)
		left = 0;
	
	if (top < 0)
		top = 0;
	
	if (right > (int)m_hfDesc.dwYFNumX)
		right = (int)m_hfDesc.dwYFNumX;

	if (bottom > (int)m_hfDesc.dwYFNumZ)
		bottom = (int)m_hfDesc.dwYFNumZ;

	pRect->left = left;
	pRect->top = top;
	pRect->right = right;
	pRect->bottom = bottom;
}
BOOL __stdcall CoHeightField::InterpolateField(DWORD dwX, DWORD dwZ, float fBias, float fRadius)
{
	VECTOR3 v3Point;
	VECTOR3	v3PickedPoint;

	v3PickedPoint.y = m_hfDesc.pyfList[dwX+dwZ*m_hfDesc.dwYFNumX];
	CalcXZ(&v3PickedPoint.x,&v3PickedPoint.z,dwX,dwZ);

	DWORD_RECT	rect;
	GetVertexPosRect(&rect,dwX,dwZ,fRadius);

	float	min_h,max_h,avg_h;
	min_h = 900000.0f;
	max_h = -900000.0f;

	DWORD	x,z;
	for(z = rect.top; z < rect.bottom; z++)
	{
		for(x = rect.left; x < rect.right; x++)
		{
			v3Point.y = m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX];
			if (min_h > v3Point.y)
				min_h = v3Point.y;

			if (max_h < v3Point.y)
				max_h = v3Point.y;
		}
	}
	avg_h = (min_h + max_h) / 2.0f;

	float distdist,rsrs,T;
	rsrs = fRadius*fRadius;
		
	for(z = rect.top; z < rect.bottom; z++)
	{
		for(x = rect.left; x < rect.right; x++)
		{


			v3Point.y = m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX];
			CalcXZ(&v3Point.x,&v3Point.z,x,z);

			// 거리 체크.
			distdist = (v3PickedPoint.x-v3Point.x)*(v3PickedPoint.x-v3Point.x) + (v3PickedPoint.z-v3Point.z)*(v3PickedPoint.z-v3Point.z);
			if (distdist > rsrs)
				continue;

			T = 1.0f + (distdist / rsrs) * fBias;
			v3Point.y = v3Point.y - ( (v3Point.y - avg_h) / T );
			m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX] = v3Point.y;
		}
	}

//	Update(HEIGHT_FIELD_UPDATE_TYPE_VERTEX_POS);
	
	// 갱신할 오브젝트만 갱신한다.
	DWORD	xstart = rect.left / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	xend = rect.right / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	zstart = rect.top / m_hfDesc.dwFacesNumPerObjAxis;
	DWORD	zend = rect.bottom / m_hfDesc.dwFacesNumPerObjAxis;

	if (xend >= m_hfDesc.dwObjNumX)
		xend = m_hfDesc.dwObjNumX-1;

	if (zend >= m_hfDesc.dwObjNumZ)
		zend = m_hfDesc.dwObjNumZ-1;
	

	for (z=zstart; z<=zend; z++)
	{
		for (x=xstart; x<=xend; x++)
		{
		
			m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].UpdateVertexPos();
		}
	}
	return TRUE;
}
DWORD __stdcall CoHeightField::GetObjectNum()
{
	return m_dwObjNum;
}

void __stdcall CoHeightField::ResetHeight(float h)
{
	for (DWORD i=0; i<m_hfDesc.dwYFNumX*m_hfDesc.dwYFNumZ; i++)
	{
		m_hfDesc.pyfList[i] = h;
	}
	for (i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].UpdateVertexPos();
	}
}

BOOL __stdcall CoHeightField::SetYFactorFromModel(I3DModel* pModel,DWORD dwRefIndex,float fGridLength,float fMinY,float fMaxY)
{
	BOOL			bResult = FALSE;
	
	DWORD			i,x,z;
	CMeshQuadTree	Tree;
	IVERTEX*		pVertices;
	DWORD			dwFacesNum;
	VECTOR3			v3Pos;
	


	dwFacesNum = pModel->CreateIVertexList(&pVertices,dwRefIndex,NULL,NULL,0,0);
	if (!dwFacesNum)
		goto lb_return;

	
	Tree.Initialize(pVertices,dwFacesNum,fMinY,fMaxY);

	for (z=0; z<m_hfDesc.dwYFNumZ; z++)
	{
		for (x=0; x<m_hfDesc.dwYFNumX; x++)
		{
			
			CalcXZ(&v3Pos.x,&v3Pos.z,x,z);
			m_hfDesc.pyfList[x + m_hfDesc.dwYFNumX*z] = Tree.GetYFactor(v3Pos.x,v3Pos.z);
		}
	}
	

	pModel->ReleaseIVertexList(pVertices);
	for (i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].UpdateVertexPos();
	}
	bResult = TRUE;
lb_return:
	return bResult;

}
DWORD __stdcall	CoHeightField::GetObjectIndex(DWORD dwSeqIndex)
{
	DWORD dwIndex = 0xffffffff;

	if (dwSeqIndex >= m_dwObjNum)
		goto lb_return;

	// 하이트필드 오브젝트의 배열순서와 동일하므로 그냥 리턴..
	dwIndex = dwSeqIndex;

lb_return:
	return dwIndex;
}

void __stdcall CoHeightField::Optimize()
{
	for (DWORD i=0; i<m_hfDesc.dwObjNumX*m_hfDesc.dwObjNumX; i++)
	{
		m_pHFieldObjectList[i].Optimize();
	}
}

COLLISION_MESH_OBJECT_DESC* __stdcall CoHeightField::GetCollisionMeshObjectDesc(DWORD dwObjIndex)
{
	return m_pHFieldObjectList[dwObjIndex].GetColMeshDesc();
}

BOOL __stdcall CoHeightField::GetHFieldDesc(HFIELD_DESC* pDesc)
{
	*pDesc = m_hfDesc;
	return TRUE;
}
BOOL __stdcall CoHeightField::BeignShadeLightMapObject(DWORD dwColor)
{
/*	m_dwAmbientColor = dwColor;
	m_pbIsShadedTable = new BYTE[m_dwObjNum];
	memset(m_pbIsShadedTable,0,sizeof(BYTE)*m_dwObjNum);
*/	
	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].BeginShadeVertexColor(dwColor);
	}
	return TRUE;
}

BOOL __stdcall CoHeightField::ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc)
{
	BOOL	bResult = FALSE;
	if (dwObjIndex >= m_dwObjNum)
		goto lb_return;

	m_pHFieldObjectList[dwObjIndex].ShadeVertexColor(pv3,dwFacesNum,pDesc,0,pFunc);
	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CoHeightField::EndShadeLightMapObject()
{
		
	memset(m_szTempVCLFileName,0,MAX_NAME_LEN);
	lstrcpy(m_szTempVCLFileName,"hfvertexcolor");
	lstrcat(m_szTempVCLFileName,".tmp");
	
	FILE*	fpVCL = fopen(m_szTempVCLFileName,"wb");

	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].EndShadeVertexColor(fpVCL);
	}

	if (fpVCL)
		fclose(fpVCL);
	
}
DWORD __stdcall	CoHeightField::GetObjectPosWithScreenCoord(VECTOR3* pv3IntersectPoint,POINT* ptCursor,DWORD dwViewportIndex)
{

	DWORD		dwIndex = 0xffffffff;

    VECTOR3		v3PickRayDir;
    VECTOR3		v3PickRayOrig;
	float		x,z;
	MATRIX4	matProj,matView;

	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	
	VIEWPORT*	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);
	
	POINT p;
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	CalcRay(&v3PickRayOrig,&v3PickRayDir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);
	Normalize(&v3PickRayDir,&v3PickRayDir);
	VECTOR3_MUL_FLOAT(&v3PickRayDir,&v3PickRayDir,pVP->ViewVolume.fFar);
    
    

	DWORD		dwPosX,dwPosZ;
	VECTOR3		v3IntersectPoint;
	float		fDist;
	if (!GetIntersectPointWithRay(&v3IntersectPoint,&fDist,&dwPosX,&dwPosZ,&v3PickRayOrig,&v3PickRayDir))
		goto lb_return;

	x = (v3IntersectPoint.x - m_hfDesc.left) / m_hfDesc.width;
	z = (v3IntersectPoint.z - m_hfDesc.top) / m_hfDesc.height;

	// 오브젝트의 x,z 좌표를 구한다.
	dwPosX = (DWORD)(x*(float)m_hfDesc.dwObjNumX);
	dwPosZ = (DWORD)(z*(float)m_hfDesc.dwObjNumZ);
	dwIndex = dwPosX + dwPosZ*m_hfDesc.dwObjNumX;

	*pv3IntersectPoint = v3IntersectPoint;
	

lb_return:
	return dwIndex;
}


CoHeightField::~CoHeightField()
{


	if (m_hfDesc.pyfList)
	{
		delete [] m_hfDesc.pyfList;
		m_hfDesc.pyfList = NULL;
	}

	if (m_hfDesc.pTexTable)
	{
		delete [] m_hfDesc.pTexTable;
		m_hfDesc.pTexTable = NULL;
	}

	if (m_hfDesc.pwTileTable)
	{
		delete [] m_hfDesc.pwTileTable;
		m_hfDesc.pwTileTable = NULL;
	}


	if (m_pColMeshObjDescList)
	{
		delete [] m_pColMeshObjDescList;
		m_pColMeshObjDescList = NULL;
	}


	if (m_pHFieldObjectList)
	{
		delete [] m_pHFieldObjectList;
		m_pHFieldObjectList = NULL;
	}
	if (m_pIDIHeightField)
	{
		m_pIDIHeightField->Release();
		m_pIDIHeightField = NULL;
	}
	

	if (m_pHFieldTools)
	{
		delete m_pHFieldTools;
		m_pHFieldTools = NULL;
	}

	if (m_pGeometry)
	{
		if (m_dwIndexInGeometry)
			m_pGeometry->DeleteFileItem(m_dwIndexInGeometry);
	}
	
}



DWORD __stdcall CoHeightField::GetVertexListWithScreenCoord(HFIELD_POS* pVertexList,DWORD dwMaxVertexNum,RECT* pRect,DWORD dwViewportIndex)
{
	DWORD dwSelectedVertexNum = GetVertexListWithScreenCoord(pVertexList,dwMaxVertexNum,pRect,1,dwViewportIndex);
	return dwSelectedVertexNum;
}
DWORD __stdcall CoHeightField::GetTileListWithScreenCoord(HFIELD_POS* pTileList,DWORD dwMaxTileNum,RECT* pRect,DWORD dwViewportIndex)
{
	BOOL		bResult	=	FALSE;
    
	VIEW_VOLUME		vv;	
	m_pGeometry->CalcViewVolume(&vv,pRect,dwViewportIndex);


	CHFieldObject* pHFieldObj[512];
	DWORD			dwSelectedObjNum;
	dwSelectedObjNum = 0;

	VECTOR3		v3Point[4];
	DWORD		i,j,k,x,z,dwSelectedTileNum;
	float		D;
	DWORD		dwTilePosX,dwTilePosZ;

	dwSelectedTileNum = 0;

	for (i=0; i<m_dwObjNum; i++)
	{
		if (ClipRenderObjectWithViewVolume(&vv,m_pHFieldObjectList[i].GetColMeshDesc(),0))
		{
			pHFieldObj[dwSelectedObjNum] = &m_pHFieldObjectList[i];
#ifdef _DEBUG
			if (dwSelectedObjNum>=512)
			{
				char	txt[512];
				memset(txt,0,512);
				wsprintf(txt,"CoHeightField::GetTileListWithScreenCoord(), dwSelectedObjNum>=512, File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
				
			}
#endif
			dwSelectedObjNum++;
	
			for (z=m_pHFieldObjectList[i].GetVertexPosZ(); z<m_pHFieldObjectList[i].GetVertexPosZ() + m_hfDesc.dwFacesNumPerObjAxis; z+=m_hfDesc.dwFacesNumPerTileAxis)
			{
				for (x=m_pHFieldObjectList[i].GetVertexPosX(); x<m_pHFieldObjectList[i].GetVertexPosX() + m_hfDesc.dwFacesNumPerObjAxis; x+=m_hfDesc.dwFacesNumPerTileAxis)
				{
					// 귀퉁이 네 점이 모두 들어가야 한다.
					v3Point[0].y = m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX];
					CalcXZ(&v3Point[0].x,&v3Point[0].z,x,z);
					
					v3Point[1].y = m_hfDesc.pyfList[x + (z+m_hfDesc.dwFacesNumPerTileAxis)*m_hfDesc.dwYFNumX];
					CalcXZ(&v3Point[1].x,&v3Point[1].z,x,z+m_hfDesc.dwFacesNumPerTileAxis);

					v3Point[2].y = m_hfDesc.pyfList[x+m_hfDesc.dwFacesNumPerTileAxis + (z+m_hfDesc.dwFacesNumPerTileAxis)*m_hfDesc.dwYFNumX];
					CalcXZ(&v3Point[2].x,&v3Point[2].z,x+m_hfDesc.dwFacesNumPerTileAxis,z+m_hfDesc.dwFacesNumPerTileAxis);

					v3Point[3].y = m_hfDesc.pyfList[x+m_hfDesc.dwFacesNumPerTileAxis + z*m_hfDesc.dwYFNumX];
					CalcXZ(&v3Point[3].x,&v3Point[3].z,x+m_hfDesc.dwFacesNumPerTileAxis,z);

					for(j=0; j < 4; j++)
					{
						for (k=0; k<4; k++)
						{
							D = DotProduct(&vv.Plane[j].v3Up,&v3Point[k]) + vv.Plane[j].D;
							if( D > 0)
								goto lb_skip;
						}

					}
					if (dwSelectedTileNum >= dwMaxTileNum)
						goto lb_return;

					dwTilePosX = x / m_hfDesc.dwFacesNumPerTileAxis;
					dwTilePosZ = z / m_hfDesc.dwFacesNumPerTileAxis;

					for (k=0; k<dwSelectedTileNum; k++)
					{
						if (pTileList[k].dwX == dwTilePosX && pTileList[k].dwZ == dwTilePosZ)
							goto lb_skip;
					}
			
					pTileList[dwSelectedTileNum].dwX = dwTilePosX;
					pTileList[dwSelectedTileNum].dwZ = dwTilePosZ;
			
					dwSelectedTileNum++;
lb_skip:
					__asm nop
				}
			}
		}

	}
lb_return:
	return dwSelectedTileNum;
/*
	// 버텍스 골라내는거랑 똑같이 한다.단지 버텍스 서치간격을 타일당 면 갯수만큼 증가시킨다.버텍스 갯수가 곧 타일갯수가 된다.
	DWORD dwSelectedVertexNum = GetVertexListWithScreenCoord(pTileList,dwMaxTileNum,pRect,m_hfDesc.dwFacesNumPerTileAxis,dwViewportIndex);

	for (DWORD i=0; i<dwSelectedVertexNum; i++)
	{
		pTileList[i].dwX = pTileList[i].dwX / m_hfDesc.dwFacesNumPerTileAxis;
		pTileList[i].dwZ = pTileList[i].dwZ / m_hfDesc.dwFacesNumPerTileAxis;
	}

	return dwSelectedVertexNum;
*/


}
DWORD CoHeightField::GetVertexListWithScreenCoord(HFIELD_POS* pVertexList,DWORD dwMaxVertexNum,RECT* pRect,DWORD dwVertexOffset,DWORD dwViewportIndex)
{
   
	VIEW_VOLUME		vv;	
	m_pGeometry->CalcViewVolume(&vv,pRect,dwViewportIndex);


	CHFieldObject* pHFieldObj[512];
	DWORD			dwSelectedObjNum;
	dwSelectedObjNum = 0;

	VECTOR3		v3Point;
	DWORD		x,z,k,dwSelectedVertexNum;
	float		D;
	dwSelectedVertexNum = 0;
	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		if (ClipRenderObjectWithViewVolume(&vv,m_pHFieldObjectList[i].GetColMeshDesc(),0))
		{
			pHFieldObj[dwSelectedObjNum] = &m_pHFieldObjectList[i];
#ifdef _DEBUG
			if (dwSelectedObjNum>=512)
			{
				char	txt[512];
				memset(txt,0,512);
				wsprintf(txt,"CoHeightField::GetVertexListWithScreenCoord(), dwSelectedObjNum>=512, File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif

			dwSelectedObjNum++;
	
			for (z=m_pHFieldObjectList[i].GetVertexPosZ(); z<=m_pHFieldObjectList[i].GetVertexPosZ() + m_hfDesc.dwFacesNumPerObjAxis; z+=dwVertexOffset)
			{
				for (x=m_pHFieldObjectList[i].GetVertexPosX(); x<=m_pHFieldObjectList[i].GetVertexPosX() + m_hfDesc.dwFacesNumPerObjAxis; x+=dwVertexOffset)
				{
					v3Point.y = m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX];
					CalcXZ(&v3Point.x,&v3Point.z,x,z);

					for(DWORD j = 0; j < 4; j++)
					{
						D = DotProduct(&vv.Plane[j].v3Up,&v3Point) + vv.Plane[j].D;

						if( D > 0)
							goto lb_skip;
					}
					if (dwSelectedVertexNum >= dwMaxVertexNum)
						goto lb_return;

					for (k=0; k<dwSelectedVertexNum; k++)
					{
						if (pVertexList[k].dwX == x && pVertexList[k].dwZ == z)
							goto lb_skip;
					}
					pVertexList[dwSelectedVertexNum].dwX = x;
					pVertexList[dwSelectedVertexNum].dwZ = z;
			//		m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX] = -100.0f;
					dwSelectedVertexNum++;
lb_skip:
					__asm nop
				}
			}
			

		}

	}

lb_return:
	return dwSelectedVertexNum;
}
BOOL __stdcall CoHeightField::AdjustHeight(HFIELD_POS* pVertexList,DWORD dwVertexNum,float fValue,DWORD dwActionType)
{
	BOOL		bResult = FALSE;

	CHFieldObject*	pHFieldObj[128];
	DWORD			dwUpdateObjNum;

	for (DWORD i=0; i<dwVertexNum; i++)
	{
		if (pVertexList[i].dwX >= m_hfDesc.dwYFNumX)
			goto lb_return;
		
		if (pVertexList[i].dwZ >= m_hfDesc.dwYFNumZ)
			goto lb_return;

		if (dwActionType == HFIELD_ADJUST_TYPE_SET)
			m_hfDesc.pyfList[pVertexList[i].dwX + pVertexList[i].dwZ * m_hfDesc.dwYFNumX] = fValue;
		else if (dwActionType == HFIELD_ADJUST_TYPE_ADD)
			m_hfDesc.pyfList[pVertexList[i].dwX + pVertexList[i].dwZ * m_hfDesc.dwYFNumX] += fValue;
		else if (dwActionType == HFIELD_ADJUST_TYPE_SUB)
			m_hfDesc.pyfList[pVertexList[i].dwX + pVertexList[i].dwZ * m_hfDesc.dwYFNumX] -= fValue;

//		m_hfDesc.pyfList[pVertexList[i].dwX + pVertexList[i].dwZ * m_hfDesc.dwYFNumX] += fPitch;
	}
	dwUpdateObjNum = GetIncludeVertexObjectList(pHFieldObj,128,pVertexList,dwVertexNum);

	for (i=0; i<dwUpdateObjNum; i++)
	{
		pHFieldObj[i]->UpdateVertexPos();
	}

	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL CoHeightField::IsValideObjIndex(DWORD dwObjX,DWORD dwObjZ)
{
	BOOL	bResult = FALSE;

	if (dwObjX >= m_hfDesc.dwObjNumX)
		goto lb_return;

	if (dwObjZ >= m_hfDesc.dwObjNumZ)
		goto lb_return;

	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD CoHeightField::GetIncludeVertexObjectList(CHFieldObject** ppHFieldObj,DWORD dwMaxObjNum,HFIELD_POS* pVertexList,DWORD dwVertexNum)
{
	// 점 리스트가 포함되는 하이트필드 오브젝트를 골라준다.주로 업데이트할때 쓴다.
	DWORD	dwObjX,dwObjZ;
	BOOL	bExtX,bExtZ;
	
	CHFieldObject*	pHFObjUpdate[1024];
	DWORD			dwUpdateObjNum = 0;

	CHFieldObject* pHFObjFind[4];		// 업데이트후보오브젝트 ..
	DWORD			dwFindObjNum;

	for (DWORD i=0; i<dwVertexNum; i++)
	{
		bExtX = FALSE;
		bExtZ = FALSE;

		dwFindObjNum = 0;					// 후보 오브젝트 갯수.
		
		dwObjX = pVertexList[i].dwX / m_hfDesc.dwFacesNumPerObjAxis;
		if ((pVertexList[i].dwX % m_hfDesc.dwFacesNumPerObjAxis) == 0)
		{
			if (dwObjX > 0)
				bExtX = TRUE;
		}
		
		dwObjZ = pVertexList[i].dwZ / m_hfDesc.dwFacesNumPerObjAxis;
		if ((pVertexList[i].dwZ % m_hfDesc.dwFacesNumPerObjAxis) == 0)
		{
			if (dwObjZ > 0)
				bExtZ = TRUE;
		}
		// 원래기본적으로 하나
		if (IsValideObjIndex(dwObjX,dwObjZ))
		{
			pHFObjFind[dwFindObjNum] = &m_pHFieldObjectList[dwObjX + dwObjZ*m_hfDesc.dwObjNumX];
			dwFindObjNum++;
		}

		// x축 오른쪽 경계에 걸치는 경우
		if (bExtX)
		{
			if (IsValideObjIndex(dwObjX-1,dwObjZ))
			{
				pHFObjFind[dwFindObjNum] = &m_pHFieldObjectList[dwObjX-1 + dwObjZ*m_hfDesc.dwObjNumX];
				dwFindObjNum++;
			}
		}

		// z축 아랫쪽 경계에 걸치는 경우
		if (bExtZ)
		{
			if (IsValideObjIndex(dwObjX,dwObjZ-1))
			{
				pHFObjFind[dwFindObjNum] = &m_pHFieldObjectList[dwObjX + (dwObjZ-1)*m_hfDesc.dwObjNumX];
				dwFindObjNum++;
			}
		}

		// x,z축 오른쪽 하단에 걸치는 경우 
		if (bExtX && bExtZ)
		{
			if (IsValideObjIndex(dwObjX-1,dwObjZ-1))
			{
				pHFObjFind[dwFindObjNum] = &m_pHFieldObjectList[dwObjX-1 + (dwObjZ-1)*m_hfDesc.dwObjNumX];
				dwFindObjNum++;
			}
		}

		for (DWORD j=0; j<dwFindObjNum; j++)
		{
			for (DWORD k=0; k<dwUpdateObjNum; k++)
			{
				if (pHFObjFind[j] == pHFObjUpdate[k])
					goto lb_skip;
			}
			pHFObjUpdate[dwUpdateObjNum] = pHFObjFind[j];
#ifdef _DEBUG
			if (dwUpdateObjNum >= 1024)
			{
				char	txt[512];
				memset(txt,0,512);
				wsprintf(txt,"CoHeightField::GetIncludeVertexObjectList(), dwUpdateObjNum>=1024, File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif


			dwUpdateObjNum++;
lb_skip:
			__asm nop
		}
	}
	if (dwUpdateObjNum > dwMaxObjNum)
		dwUpdateObjNum = dwMaxObjNum;

	if (dwUpdateObjNum)
		memcpy(ppHFieldObj,pHFObjUpdate,sizeof(CHFieldObject**)*dwUpdateObjNum);

	return dwUpdateObjNum;
}

BOOL __stdcall CoHeightField::RenderVertex(HFIELD_POS* pVertexList,DWORD dwVertexNum,DWORD dwColor)
{
	VECTOR3		v3Point;
	BOOL		bResult = FALSE;
	for (DWORD i=0; i<dwVertexNum; i++)
	{
		if (pVertexList[i].dwX >= m_hfDesc.dwYFNumX)
			goto lb_return;
		
		if (pVertexList[i].dwZ >= m_hfDesc.dwYFNumZ)
			goto lb_return;

		v3Point.y = m_hfDesc.pyfList[pVertexList[i].dwX + pVertexList[i].dwZ * m_hfDesc.dwYFNumX];
		CalcXZ(&v3Point.x,&v3Point.z,pVertexList[i].dwX,pVertexList[i].dwZ);
		m_pGeometry->RenderSphere(&v3Point,10.0f,0xffff00cc);
	}
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoHeightField::RenderTile(HFIELD_POS* phfStart,HFIELD_POS* pTileList,DWORD dwTileNum,WORD* pwTileIndexList,DWORD dwColor)
{
	VECTOR3		v3Point[4];
	DWORD		dwVertexPosX,dwVertexPosZ;
	BOOL		bResult = FALSE;
	DWORD		dwTilePosX,dwTilePosZ;

	for (DWORD i=0; i<dwTileNum; i++)
	{
		dwTilePosX = phfStart->dwX + pTileList[i].dwX;
		dwTilePosZ = phfStart->dwZ + pTileList[i].dwZ;

		if (dwTilePosX >= m_hfDesc.dwTileNumX)
			continue;
			
		
		if (dwTilePosZ >= m_hfDesc.dwTileNumZ)
			continue;


		dwVertexPosX = dwTilePosX * m_hfDesc.dwFacesNumPerTileAxis;
		dwVertexPosZ = dwTilePosZ * m_hfDesc.dwFacesNumPerTileAxis;

		v3Point[0].y = m_hfDesc.pyfList[dwVertexPosX + dwVertexPosZ*m_hfDesc.dwYFNumX];
		CalcXZ(&v3Point[0].x,&v3Point[0].z,dwVertexPosX,dwVertexPosZ);

		v3Point[1].y = m_hfDesc.pyfList[dwVertexPosX + (dwVertexPosZ+m_hfDesc.dwFacesNumPerTileAxis)*m_hfDesc.dwYFNumX];
		CalcXZ(&v3Point[1].x,&v3Point[1].z,dwVertexPosX,dwVertexPosZ+m_hfDesc.dwFacesNumPerTileAxis);

		v3Point[2].y = m_hfDesc.pyfList[dwVertexPosX+m_hfDesc.dwFacesNumPerTileAxis + (dwVertexPosZ+m_hfDesc.dwFacesNumPerTileAxis)*m_hfDesc.dwYFNumX];
		CalcXZ(&v3Point[2].x,&v3Point[2].z,dwVertexPosX+m_hfDesc.dwFacesNumPerTileAxis,dwVertexPosZ+m_hfDesc.dwFacesNumPerTileAxis);

		v3Point[3].y = m_hfDesc.pyfList[dwVertexPosX+m_hfDesc.dwFacesNumPerTileAxis + dwVertexPosZ*m_hfDesc.dwYFNumX];
		CalcXZ(&v3Point[3].x,&v3Point[3].z,dwVertexPosX+m_hfDesc.dwFacesNumPerTileAxis,dwVertexPosZ);

	//	m_pRenderer->RenderGrid(v3Point,dwColor);
		if (pwTileIndexList)
			m_pIDIHeightField->RenderGrid(v3Point,pwTileIndexList[i],(dwColor & 0xff000000)>>24);
		else
			m_pRenderer->RenderGrid(v3Point,dwColor);
		
	}
	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL __stdcall CoHeightField::SetTileList(HFIELD_POS* phfStart,HFIELD_POS* pTileList,WORD* pTexTileIndexList,DWORD dwTileNum,WORD wTileIndex)
{
	BOOL		bResult = FALSE;
	DWORD		dwTilePosX,dwTilePosZ;
	CHFieldObject*	pHFObjUpdate[128];
	DWORD			dwUpdateObjNum = 0;

	for (DWORD i=0; i<dwTileNum; i++)
	{
		dwTilePosX = phfStart->dwX + pTileList[i].dwX;
		dwTilePosZ = phfStart->dwZ + pTileList[i].dwZ;

		if (dwTilePosX >= m_hfDesc.dwTileNumX)
			continue;
			
		
		if (dwTilePosZ >= m_hfDesc.dwTileNumZ)
			continue;

		if (pTexTileIndexList)
			m_hfDesc.pwTileTable[dwTilePosX + dwTilePosZ*m_hfDesc.dwTileNumX] = pTexTileIndexList[i];
		else
			m_hfDesc.pwTileTable[dwTilePosX + dwTilePosZ*m_hfDesc.dwTileNumX] = wTileIndex;


		DWORD dwObjX = dwTilePosX / m_hfDesc.dwTileNumPerObjAxis;
		DWORD dwObjZ = dwTilePosZ / m_hfDesc.dwTileNumPerObjAxis;

		for (DWORD j=0; j<dwUpdateObjNum; j++)
		{
			if (pHFObjUpdate[j] == &m_pHFieldObjectList[dwObjX + dwObjZ*m_hfDesc.dwObjNumX])
				goto lb_skip;
		}
#ifdef _DEBUG
		if (dwUpdateObjNum >= 128)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"CoHeightField::SetTileList(), dwUpdateObjNum>=128, File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif

		pHFObjUpdate[dwUpdateObjNum] = &m_pHFieldObjectList[dwObjX + dwObjZ*m_hfDesc.dwObjNumX];
		dwUpdateObjNum++;
lb_skip:
		__asm nop
	}

	HFIELD_POS	hfPosStart;
	hfPosStart.dwX = phfStart->dwX + pTileList[0].dwX;
	hfPosStart.dwZ = phfStart->dwZ + pTileList[0].dwZ;


	for (i=0; i<dwUpdateObjNum; i++)
	{
		pHFObjUpdate[i]->UpdateTile();
	}
	bResult = TRUE;
lb_return:
	return bResult;
}



//BOOL __stdcall CoHeightField::RenderTileRect
// GetVertexPos2의 개정판, 커서에 집힌 점의 x,y,z를 얻을 수 있다.
BOOL __stdcall CoHeightField::GetVertexPosWithScreenCoord(DWORD* pdwPosX, DWORD* pdwPosZ, VECTOR3 OUT *pWhere, POINT* ptCursor, float fVertexRadius,DWORD dwViewportIndex)
{
	BOOL		bResult	=	FALSE;
	VECTOR3		From, To;

	float		fCurrentDist,fDist;
	VECTOR3		Fact;
	DWORD		i, j;
	DWORD		dwCurrentI, dwCurrentJ;

	VECTOR3		v3PickRayDir;
    VECTOR3		v3PickRayOrig;



	MATRIX4	matProj,matView;

	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	VIEWPORT*	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);
	
	POINT p;
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

	CalcRay(&v3PickRayOrig,&v3PickRayDir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);
    
    
	DWORD		dwPosX,dwPosZ;
	VECTOR3		v3IntersectPoint;

	Normalize(&v3PickRayDir,&v3PickRayDir);
	VECTOR3_MUL_FLOAT(&v3PickRayDir,&v3PickRayDir,pVP->ViewVolume.fFar);

	if (!GetIntersectPointWithRay(&v3IntersectPoint,&fDist,&dwPosX,&dwPosZ,&v3PickRayOrig,&v3PickRayDir))
		goto lb_return;

	From = v3PickRayOrig;
	To.x = From.x + v3PickRayDir.x;
	To.y = From.y + v3PickRayDir.y;
	To.z = From.z + v3PickRayDir.z;

	fCurrentDist = 900000.0f;
	fDist = 0;
	
	for( j = dwPosZ; j < dwPosZ+2; j++)	// 점들의 리스트를 선형으로 뒤진다.
	{
		for( i = dwPosX; i < dwPosX+2; i++)
		{
			Fact.y = m_hfDesc.pyfList[i + j*m_hfDesc.dwYFNumX];
			CalcXZ(
				&Fact.x,
				&Fact.z,
				i,
				j);

			fDist = CalcDistance(&v3IntersectPoint,&Fact);
			if (fDist <= fVertexRadius)
			{
				if( fDist < fCurrentDist)
				{	
					bResult = TRUE;
					dwCurrentI = i;
					dwCurrentJ = j;
					fCurrentDist = fDist;
				}
			}
		}		
	}

	// 걸리는게 없으면... -_-;
	if (!bResult)
		goto lb_return;

lb_true:
	// 걸리는게 있으면 인덱스를 출력.
	*pdwPosX = dwCurrentI;
	*pdwPosZ = dwCurrentJ;
	pWhere->y = m_hfDesc.pyfList[dwCurrentI + dwCurrentJ*m_hfDesc.dwYFNumX];
	CalcXZ(
		&pWhere->x,
		&pWhere->z,
		dwCurrentI,
		dwCurrentJ);

lb_return:
	return bResult;
}



void CoHeightField::GetYFactors(DWORD* pdwPosX,DWORD* pdwPosZ,DWORD* pdwWidthNum,DWORD* pdwHeightNum,FLOAT_RECT* pfRect)
{

	DWORD		xstart,zstart,xend,zend;

	xstart = (DWORD)((pfRect->fLeft-m_hfDesc.left)	/ m_hfDesc.fFaceSize);
	zstart = (DWORD)((pfRect->fTop-m_hfDesc.top)	/	m_hfDesc.fFaceSize);

	xend = (DWORD)((pfRect->fRight - m_hfDesc.left) / m_hfDesc.fFaceSize);
	zend = (DWORD)((pfRect->fBottom - m_hfDesc.top) / m_hfDesc.fFaceSize);

	*pdwPosX = xstart;
	*pdwPosZ = zstart;
	*pdwWidthNum = xend - xstart;
	*pdwHeightNum = zend - zstart;
}


BOOL __stdcall CoHeightField::SetTile(DWORD dwPosX,DWORD dwPosZ,WORD wTileIndex)
{
	BOOL		bResult = FALSE;
	DWORD		dwObjX,dwObjZ;
	
	if (dwPosX >= m_hfDesc.dwTileNumX || dwPosZ >= m_hfDesc.dwTileNumZ)
		goto lb_return;

	if (m_hfDesc.pwTileTable[dwPosX + dwPosZ*m_hfDesc.dwTileNumX] == wTileIndex)
		goto lb_return;

	m_hfDesc.pwTileTable[dwPosX + dwPosZ*m_hfDesc.dwTileNumX] = wTileIndex;
	
	dwObjX = dwPosX / m_hfDesc.dwTileNumPerObjAxis;
	dwObjZ = dwPosZ / m_hfDesc.dwTileNumPerObjAxis;

	m_pHFieldObjectList[dwObjX + dwObjZ*m_hfDesc.dwObjNumX].UpdateTile();
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL __stdcall CoHeightField::GetTile(WORD* pwTileIndex,DWORD dwPosX,DWORD dwPosZ)
{
	BOOL		bResult = FALSE;

	if (dwPosX >= m_hfDesc.dwTileNumX || dwPosZ >= m_hfDesc.dwTileNumZ)
		goto lb_return;

	*pwTileIndex = m_hfDesc.pwTileTable[dwPosX + dwPosZ*m_hfDesc.dwTileNumX];
	bResult = TRUE;

lb_return:
	return bResult;
}



BOOL __stdcall CoHeightField::SetLightVertexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwColor,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;

	HFIELD_POS	pos[1024];
//	float	fRs = m_hfDesc.fFaceSize;
	DWORD	dwHFPosNum = GetXZFrom3DCoord(pos,1024,pv3IntersectPoint,fRs);
	if (!dwHFPosNum)
		goto lb_return;

	DWORD	dwObjNum;
	CHFieldObject* pHFieldObj[64];
	dwObjNum = GetIncludeVertexObjectList(pHFieldObj,64,pos,dwHFPosNum);

	DWORD	i;
	for (i=0; i<dwObjNum; i++)
	{
		bResult = pHFieldObj[i]->SetLightVertexel(pos,dwHFPosNum,dwColor);
	}
lb_return:
	return bResult;

}
BOOL __stdcall CoHeightField::GetLightVertexel(DWORD* pdwColor,POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex)
{
	BOOL	bResult = FALSE;

	
	HFIELD_POS	pos;
	DWORD		dwHFPosNum;
	dwHFPosNum = GetXZFrom3DCoord(&pos,1,pv3IntersectPoint,m_hfDesc.fFaceSize);
	if (!dwHFPosNum)
		goto lb_return;

	DWORD	dwObjNum;
	CHFieldObject* pHFieldObj[4];
	dwObjNum = GetIncludeVertexObjectList(pHFieldObj,4,&pos,1);


	if (!dwObjNum)
		goto lb_return;

	bResult = pHFieldObj[0]->GetLightVertexel(pdwColor,&pos);
	
lb_return:
	return bResult;
	
	
}
BOOL __stdcall CoHeightField::SetTileRect(RECT* pRect,WORD wTileIndex)
{
	BOOL		bResult = FALSE;
	

	if (pRect->left < 0)
		pRect->left = 0;

	if (pRect->right >= (int)m_hfDesc.dwTileNumX)
		pRect->right = (int)m_hfDesc.dwTileNumX-1;

	if (pRect->top < 0)
		pRect->top = 0;

	if (pRect->bottom >= (int)m_hfDesc.dwTileNumZ)
		pRect->bottom = (int)m_hfDesc.dwTileNumZ-1;

	int			x,z;
	HFIELD_POS	hfPosStart;
	hfPosStart.dwX = pRect->left;
	hfPosStart.dwZ = pRect->top;

	for (z = pRect->top; z<=pRect->bottom; z++)
	{
		for (x = pRect->left; x<=pRect->right; x++)
		{
			m_hfDesc.pwTileTable[x + z*m_hfDesc.dwTileNumX] = wTileIndex;
		}
	}
		
	RECT		rcObj;

	rcObj.left = pRect->left / (int)m_hfDesc.dwTileNumPerObjAxis;
	rcObj.top = pRect->top / (int)m_hfDesc.dwTileNumPerObjAxis;
	
	rcObj.right = pRect->right / (int)m_hfDesc.dwTileNumPerObjAxis;
	rcObj.bottom = pRect->bottom / (int)m_hfDesc.dwTileNumPerObjAxis;

	for (z = rcObj.top; z<=rcObj.bottom; z++)
	{
		for (x = rcObj.left; x<=rcObj.right; x++)
		{
			m_pHFieldObjectList[x + z*m_hfDesc.dwObjNumX].UpdateTile();
		}
	}

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoHeightField::GetTileRectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,RECT* pRect,POINT* ptCursor,DWORD dwBrushSize,DWORD dwViewportIndex)
{
	BOOL		bResult = FALSE;
	
	if (!dwBrushSize)
		goto lb_return;


	MATRIX4	matProj,matView;
	
	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	
	VIEWPORT*	pVP;
	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);


	POINT p;
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

    VECTOR3		v3PickRayDir;
    VECTOR3		v3PickRayOrig;
	CalcRay(&v3PickRayOrig,&v3PickRayDir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);

	Normalize(&v3PickRayDir,&v3PickRayDir);
	VECTOR3_MUL_FLOAT(&v3PickRayDir,&v3PickRayDir,pVP->ViewVolume.fFar);

	DWORD		dwVertexPosX,dwVertexPosZ;
	if (!GetIntersectPointWithRay(pv3IntersectPoint,pfDist,&dwVertexPosX,&dwVertexPosZ,&v3PickRayOrig,&v3PickRayDir))
		goto lb_return;

	int		iPosX,iPosZ;

	iPosX = (int)((pv3IntersectPoint->x - m_hfDesc.left) / m_hfDesc.fTileSize);
	iPosZ = (int)((pv3IntersectPoint->z - m_hfDesc.top) / m_hfDesc.fTileSize);

	dwBrushSize = (dwBrushSize-1) / 2;
	

	pRect->left = iPosX - (int)dwBrushSize;
	pRect->right = iPosX + (int)dwBrushSize;
	pRect->top = iPosZ - (int)dwBrushSize;
	pRect->bottom = iPosZ + (int)dwBrushSize;
	
	if (pRect->left < 0)
		pRect->left = 0;

	if (pRect->right >= (int)m_hfDesc.dwTileNumX)
		pRect->right = (int)m_hfDesc.dwTileNumX-1;

	if (pRect->top < 0)
		pRect->top = 0;

	if (pRect->bottom >= (int)m_hfDesc.dwTileNumZ)
		pRect->bottom = (int)m_hfDesc.dwTileNumZ-1;


	bResult = TRUE;
	
lb_return:
	return bResult;
}
BOOL __stdcall CoHeightField::GetTilePosWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,POINT* ptCursor,DWORD dwViewportIndex)
{
	BOOL		bResult = FALSE;
	
	MATRIX4	matProj,matView;
	m_pGeometry->GetProjectionMatrix(&matProj,dwViewportIndex);
	m_pGeometry->GetViewMatrix(&matView,dwViewportIndex);
	
	VIEWPORT*	pVP = m_pGeometry->INL_GetViewport(dwViewportIndex);

	POINT p;
	if (ptCursor->x > pVP->rcClip.right || ptCursor->y > pVP->rcClip.bottom)
		goto lb_return;

	p.x = ptCursor->x - pVP->rcClip.left;
	p.y = ptCursor->y - pVP->rcClip.top;

	if (p.x < 0 || p.y < 0)
		goto lb_return;

    VECTOR3		v3PickRayDir;
    VECTOR3		v3PickRayOrig;
	CalcRay(&v3PickRayOrig,&v3PickRayDir,p.x,p.y,(DWORD)pVP->wClipWidth,(DWORD)pVP->wClipHeight,&matProj,&matView);

	Normalize(&v3PickRayDir,&v3PickRayDir);
	VECTOR3_MUL_FLOAT(&v3PickRayDir,&v3PickRayDir,pVP->ViewVolume.fFar);

	DWORD		dwVertexPosX,dwVertexPosZ;
	if (!GetIntersectPointWithRay(pv3IntersectPoint,pfDist,&dwVertexPosX,&dwVertexPosZ,&v3PickRayOrig,&v3PickRayDir))
		goto lb_return;

	*pdwPosX = (DWORD)((pv3IntersectPoint->x - m_hfDesc.left) / m_hfDesc.fTileSize);
	*pdwPosZ = (DWORD)((pv3IntersectPoint->z - m_hfDesc.top) / m_hfDesc.fTileSize);
	
	bResult = TRUE;
	
lb_return:
	return bResult;
}
BOOL __stdcall CoHeightField::GetY(float* pfHeight,float x,float z)
{
	BOOL		bResult = FALSE;

	if (x < m_hfDesc.left || 
		x>= m_hfDesc.right || 
		z < m_hfDesc.top || 
		z>= m_hfDesc.bottom)
		goto lb_return;

	DWORD		dwVertexPosX,dwVertexPosZ;

	dwVertexPosX = (DWORD)((x - m_hfDesc.left) / m_hfDesc.fFaceSize);
	dwVertexPosZ = (DWORD)((z - m_hfDesc.top) / m_hfDesc.fFaceSize);


	VECTOR3		v3Orig,v3Ray;
	v3Orig.x = x;
	v3Orig.z = z;
	v3Orig.y = 400000.0f;

	v3Ray.x = 0.0f;
	v3Ray.z = 0.0f;
	v3Ray.y = -400000.0f;

	VECTOR3		v3IntersectPoint;
	float		fDist;
	if (!IsIntersectPoint(&v3IntersectPoint,&fDist,&v3Orig,&v3Ray,dwVertexPosX,dwVertexPosZ))
		goto lb_return;

	*pfHeight = v3IntersectPoint.y;
	bResult = TRUE;
	
lb_return:
	return bResult;
}
void GetQuadRegion(FLOAT_RECT* pfRectQuad,FLOAT_RECT* pfRect)
{
	float	fWidth = pfRect->fRight - pfRect->fLeft;
	float	fHeight = pfRect->fBottom - pfRect->fTop;

	fWidth /= 2.0f;
	fHeight /= 2.0f;

	
	for (DWORD i=0; i<4; i++)
	{
		pfRectQuad[i] = *pfRect;

	}
	pfRectQuad[0].fRight -= fWidth;
	pfRectQuad[0].fBottom -= fHeight;

	pfRectQuad[1].fRight = pfRectQuad[0].fRight;
	pfRectQuad[1].fTop += fHeight;

	pfRectQuad[2].fLeft += fWidth;
	pfRectQuad[2].fTop = pfRectQuad[1].fTop;
	
	pfRectQuad[3].fLeft = pfRectQuad[2].fLeft;
	pfRectQuad[3].fBottom = pfRectQuad[0].fBottom;

}
struct HPOS_BLOCK
{
	DWORD	dwPosX;
	DWORD	dwPosZ;
	VECTOR3	v3IntersectPoint;
	float	fDist;
};

BOOL __stdcall CoHeightField::GetIntersectPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,VECTOR3* pv3Orig,VECTOR3* pv3Dir)
{

	HPOS_BLOCK		hfPosBlock[3];
	HPOS_BLOCK		hfPosResult;

	BOOL		bResult = FALSE;
	float		a,b,delta;
	
	VECTOR3	v3To;
	float	x,z;
	DWORD	k;


	VECTOR3_ADD_VECTOR3(&v3To,pv3Orig,pv3Dir);

	a = (v3To.z - pv3Orig->z) / (v3To.x - pv3Orig->x);
	b = -1.0f * a * pv3Orig->x + pv3Orig->z;
	
	DWORD dwPosX,dwPosZ,dwPrvPosX,dwPrvPosZ;
	dwPrvPosX = 0xffffffff;
	dwPrvPosZ = 0xffffffff;
	delta = m_hfDesc.fFaceSize / 2.0f;

	int	iCount = 0;
	if (fabs(a) < 1.0f)
	{
		delta *= ( (v3To.x - pv3Orig->x) / fabs(v3To.x - pv3Orig->x) );
		if (delta < 0)
		{
			// 역방향으로 간다.
			x = pv3Orig->x + delta;
		}
		else
		{
			// 정방향으로 간다.
			x = pv3Orig->x - delta;
		}
		iCount = fabs( (v3To.x - pv3Orig->x) / delta ) + 2;

		while (iCount)
		{
			//for (x = start; x<=end; x += delta)
			z = a*x + b;

			dwPosX = (DWORD)((x - m_hfDesc.left) / m_hfDesc.fFaceSize);
			dwPosZ = (DWORD)((z - m_hfDesc.top) / m_hfDesc.fFaceSize);

			if (dwPrvPosX == dwPosX && dwPrvPosZ == dwPosZ)
				goto lb_skip_x;
				
			dwPrvPosX = dwPosX;
			dwPrvPosZ = dwPosZ;

			
			hfPosBlock[0].dwPosX = dwPosX;
			hfPosBlock[0].dwPosZ = dwPosZ;

			hfPosBlock[1].dwPosX = dwPosX;
			hfPosBlock[1].dwPosZ = dwPosZ-1;

			hfPosBlock[2].dwPosX = dwPosX;
			hfPosBlock[2].dwPosZ = dwPosZ+1;

		
			hfPosResult.fDist = 900000.0f;
			hfPosResult.dwPosX = 0xffffffff;
			hfPosResult.dwPosZ = 0xffffffff;

			
			for (k=0; k<3; k++)
			{
				if (
					IsIntersectPoint(
					&hfPosBlock[k].v3IntersectPoint,
					&hfPosBlock[k].fDist,
					pv3Orig,pv3Dir,
					hfPosBlock[k].dwPosX,
					hfPosBlock[k].dwPosZ))
				{
					bResult = TRUE;
					if (hfPosBlock[k].fDist < hfPosResult.fDist)
					{
						hfPosResult = hfPosBlock[k];
					}
				}
			}
			if (bResult)
				goto lb_true;

lb_skip_x:
			x += delta;
			iCount--;
		}
	}
	else
	{
		delta *= ( (v3To.z - pv3Orig->z) / fabs(v3To.z - pv3Orig->z) );
		if (delta < 0)
		{
			// 역방향으로 간다.
			z = pv3Orig->z + delta;
		}
		else
		{
			// 정방향으로 간다.
			z = pv3Orig->z - delta;
		}
		iCount = fabs( (v3To.z - pv3Orig->z) / delta ) + 2;
		while (iCount)
		{
			//for (z = start; z<=end; z += delta)
			x = (z - b) / a;

			dwPosX = (DWORD)((x - m_hfDesc.left) / m_hfDesc.fFaceSize);
			dwPosZ = (DWORD)((z - m_hfDesc.top) / m_hfDesc.fFaceSize);

			if (dwPrvPosX == dwPosX && dwPrvPosZ == dwPosZ)
				goto lb_skip_z;
				
			dwPrvPosX = dwPosX;
			dwPrvPosZ = dwPosZ;

			hfPosBlock[0].dwPosX = dwPosX;
			hfPosBlock[0].dwPosZ = dwPosZ;

			hfPosBlock[1].dwPosX = dwPosX-1;
			hfPosBlock[1].dwPosZ = dwPosZ;

			hfPosBlock[2].dwPosX = dwPosX+1;
			hfPosBlock[2].dwPosZ = dwPosZ;

		
			hfPosResult.fDist = 900000.0f;
			hfPosResult.dwPosX = 0xffffffff;
			hfPosResult.dwPosZ = 0xffffffff;

			for (k=0; k<3; k++)
			{
				if (
					IsIntersectPoint(
					&hfPosBlock[k].v3IntersectPoint,
					&hfPosBlock[k].fDist,
					pv3Orig,pv3Dir,
					hfPosBlock[k].dwPosX,
					hfPosBlock[k].dwPosZ))
				{
					bResult = TRUE;
					if (hfPosBlock[k].fDist < hfPosResult.fDist)
					{
						hfPosResult = hfPosBlock[k];
					}
				}
			}
			if (bResult)
				goto lb_true;
lb_skip_z:
			z += delta;
			iCount--;

		}
	}
	// 실패...
	goto lb_return;

lb_true:
			
	*pdwPosX = (DWORD)((hfPosResult.v3IntersectPoint.x - m_hfDesc.left) / m_hfDesc.fFaceSize);
	*pdwPosZ = (DWORD)((hfPosResult.v3IntersectPoint.z - m_hfDesc.top) / m_hfDesc.fFaceSize);
	*pv3IntersectPoint = hfPosResult.v3IntersectPoint;
	*pfDist = hfPosResult.fDist;

lb_return:
	return bResult;
}

BOOL CoHeightField::IsIntersectPoint(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir,DWORD dwPosX,DWORD dwPosZ)
{
	BOOL		bResult = FALSE;
	VECTOR3		v3Face[6];

	if (dwPosX > m_hfDesc.dwYFNumX - 2)
		goto lb_return;

	if (dwPosZ > m_hfDesc.dwYFNumZ - 2)
		goto lb_return;

	v3Face[0].y = m_hfDesc.pyfList[dwPosX + dwPosZ*m_hfDesc.dwYFNumX];
	CalcXZ(&v3Face[0].x,&v3Face[0].z,dwPosX,dwPosZ);

	v3Face[1].y = m_hfDesc.pyfList[dwPosX +	(dwPosZ+1)*m_hfDesc.dwYFNumX];
	CalcXZ(&v3Face[1].x,&v3Face[1].z,dwPosX,dwPosZ + 1);

	v3Face[2].y = m_hfDesc.pyfList[dwPosX+1	+ dwPosZ*m_hfDesc.dwYFNumX];
	CalcXZ(&v3Face[2].x,&v3Face[2].z,dwPosX + 1,dwPosZ);

	v3Face[3] = v3Face[2];
	v3Face[4] = v3Face[1];
	
	v3Face[5].y = m_hfDesc.pyfList[dwPosX + 1	+	(dwPosZ + 1)*m_hfDesc.dwYFNumX];
	CalcXZ(&v3Face[5].x,&v3Face[5].z,dwPosX + 1,dwPosZ + 1);
	
	DWORD	k;
	float	fBary1,fBary2;
	for (k=0; k<2; k++)
	{
		if (IntersectTriangle(
			pv3IntersectPoint,
			pv3Orig,
			pv3Dir,
			&v3Face[k*3+0],
			&v3Face[k*3+1],
			&v3Face[k*3+2],
			pfDist,
			&fBary1,
			&fBary2,
			TRUE))
		{
			bResult = TRUE;
			goto lb_return;
		}
	}
lb_return:
	return bResult;
}

DWORD __stdcall CoHeightField::GetExtTileIndexTop(POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex)
{
	DWORD	dwTileIndex = 0xffffffff;
	
	if (!m_hfDesc.bBlendEnable)
		goto lb_return;

	VECTOR3		v3IntersectPoint;
	float		fDist;
	DWORD		dwTilePosX,dwTilePosZ;
	DWORD		dwObjIndex;

	dwObjIndex = GetObjectPosWithScreenCoord(&v3IntersectPoint,ptCursor,dwViewportIndex);
	if (dwObjIndex == 0xffffffff)
		goto lb_return;

	dwTilePosX = (DWORD)((v3IntersectPoint.x - m_hfDesc.left) / m_hfDesc.fTileSize);
	dwTilePosZ = (DWORD)((v3IntersectPoint.z - m_hfDesc.top) / m_hfDesc.fTileSize);

	dwTileIndex = m_pHFieldObjectList[dwObjIndex].GetExtTileTop(dwTilePosX,dwTilePosZ);

lb_return:
	return dwTileIndex;
}

BOOL __stdcall CoHeightField::SetAlphaTexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwTileIndex,DWORD dwActionFlag,DWORD dwViewportIndex)
{
	BOOL			bResult = FALSE;

	if (!m_hfDesc.bBlendEnable)
		goto lb_return;

	VECTOR3		v3IntersectPoint;
	DWORD		dwObjIndex;
	dwObjIndex = GetObjectPosWithScreenCoord(&v3IntersectPoint,ptCursor,dwViewportIndex);
	if (0xffffffff == dwObjIndex )
		goto lb_return;

	DWORD	dwAlphaTexWidthHeight;
	dwAlphaTexWidthHeight= m_hfDesc.dwTileNumPerObjAxis*4;
	
	bResult = m_pHFieldObjectList[dwObjIndex].SetAlphaTexel(&v3IntersectPoint,fRs,dwAlphaTexWidthHeight,dwTileIndex,dwActionFlag);
	if (bResult)
		m_pHFieldObjectList[dwObjIndex].GetHFieldControlObject()->SetMustUpdate();

lb_return:
	return bResult;
}

void __stdcall CoHeightField::EnableBlend(BOOL bEnable)
{
	if (bEnable)
	{
		if (!m_pHFieldTools)
		{
			m_pHFieldTools = new CHFieldTools;
			m_pHFieldTools->Initialize(&m_hfDesc,TRUE,m_dwTotalAlphaMapNum,&m_dwAlphaMapWidthHeight);
		}
		
	}
	m_bEnableAlphaMap = bEnable;
//	m_hfDesc.bBlendEnable = (BYTE)bEnable;
	m_pIDIHeightField->SetHFieldTileBlend(bEnable);
	UpdateAllIDIMeshObjects();
//	UpdateAllTiles();
}

void __stdcall CoHeightField::UpdateAllTiles()
{


	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].UpdateTile();
	}
}
void __stdcall CoHeightField::UpdateAllIDIMeshObjects()
{
	for (DWORD i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].UpdateIDIMeshObject();
	}
}



DWORD CoHeightField::WriteAlphaMapHeader(FILE* fp)
{
	DWORD	oldPos = ftell(fp);


	if (!m_pHFieldTools)
		goto lb_return;

	ALPHA_MAP_HEADER	header;
	
	header.dwAlphaMapNum = m_pHFieldTools->GetAllocatedAlphaMapNum();
	header.dwWidthHeight = m_pHFieldTools->GetAlphaMapWidthHeight();

	header.dwReserved0 = 0;
	header.dwReserved1 = 0;
	header.dwReserved2 = 0;
	header.dwReserved3 = 0;

	if (!header.dwAlphaMapNum)
		goto lb_return;

	DWORD	dwBoundary;
	dwBoundary = m_dwObjNum;			// 필드 데이타의 경계를 세팅.0이면 이후에 아무것도 없는것.있으면 오브젝트 갯수로 세팅
	fwrite(&dwBoundary,sizeof(DWORD),1,fp);
	fwrite(&header,sizeof(ALPHA_MAP_HEADER),1,fp);
	dwBoundary = 0;
	fwrite(&dwBoundary,sizeof(DWORD),1,fp);


lb_return:
	return (ftell(fp)-oldPos);
}
DWORD CoHeightField::WriteAlphaMapBody(char* szFileName)
{	
	DWORD	dwLen = 0;

	if (!m_pHFieldTools)
		goto lb_return;

	dwLen = m_pHFieldTools->WriteAlphaMap(szFileName,m_pHFieldObjectList,m_dwObjNum);
lb_return:
	return dwLen;
}

DWORD CoHeightField::ReadAlphaMapHeader(void* pFP)
{
	DWORD	dwLen = 0;

//	dwLen += g_pFileStorage->FSRead(pFP,&m_hfDesc.dwTileNumX,sizeof(DWORD));
	ALPHA_MAP_HEADER	header;

	// 이전의 hfl파일의 끝이 4바이트여유만 있을것을 대비하여..4바이트부터 읽는다
	
	DWORD	dwBoundary;
	dwLen += g_pFileStorage->FSRead(pFP,&dwBoundary,sizeof(DWORD));
	if (!dwBoundary)
		goto lb_return;

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(header));
	if (!header.dwAlphaMapNum)
		goto lb_return;

	
	m_dwTotalAlphaMapNum = header.dwAlphaMapNum;
	m_dwAlphaMapWidthHeight = header.dwWidthHeight;
	
lb_return:
	return dwLen;

}
DWORD CoHeightField::ReadAlphaMapBody(char* szFileName)
{
	DWORD	dwLen = 0;

	if (!m_dwTotalAlphaMapNum)
		goto lb_return;

	if (!m_pHFieldTools)
		goto lb_return;

	dwLen = m_pHFieldTools->ReadAlphaMap(szFileName,m_pHFieldObjectList,m_dwObjNum);
lb_return:
	return dwLen;
}
void __stdcall CoHeightField::ClearAlphaMapAll()
{
	
	DWORD	i;
	for (i=0; i<m_dwObjNum; i++)
	{
		m_pHFieldObjectList[i].CleanupAlphaMap();
	}
}
void __stdcall CoHeightField::ClearAlphaMapWithObjectIndex(DWORD dwObjIndex)
{
	if (dwObjIndex >= m_dwObjNum)
		goto lb_return;

	m_pHFieldObjectList[dwObjIndex].CleanupAlphaMap();

lb_return:
	return;

}
void __stdcall CoHeightField::ClearAlphaMapWithTileIndex(DWORD dwObjIndex,DWORD dwTileIndex)
{
	if (dwObjIndex >= m_dwObjNum)
		goto lb_return;

	m_pHFieldObjectList[dwObjIndex].CleanupAlphaMapWithTileIndex(dwTileIndex);

lb_return:
	return;

}
void __stdcall CoHeightField::GetAlphaMapResourceStatus(DWORD* pdwMapNum,DWORD* pdwMemSize)
{
	*pdwMapNum = 0;
	*pdwMemSize = 0;
	if (m_pHFieldTools)
	{
		*pdwMapNum = m_pHFieldTools->GetAllocatedAlphaMapNum();
		*pdwMemSize = m_pHFieldTools->GetAllocatedAlphaMapMemSize();

	}
}

DWORD __stdcall CoHeightField::QueryNeedTriNumWithAABB(DWORD* pdwStartX,DWORD* pdwStartZ,DWORD* pdwWidthNum,DWORD* pdwHeightNum,MAABB* pAABB)
{
	float	fHalfSize = m_hfDesc.fFaceSize / 2.0f;

	float	left_x,right_x,top_z,bottom_z;
	left_x = pAABB->Min.x;
	top_z = pAABB->Min.z;
	bottom_z = pAABB->Max.z + m_hfDesc.fFaceSize + m_hfDesc.fFaceSize;
	right_x = pAABB->Max.x + m_hfDesc.fFaceSize + m_hfDesc.fFaceSize;

	if (left_x < m_hfDesc.left)
		left_x = m_hfDesc.left;

	if (right_x > m_hfDesc.right)
		right_x = m_hfDesc.right;

	if (top_z < m_hfDesc.top)
		top_z = m_hfDesc.top;

	if (bottom_z > m_hfDesc.bottom)
		bottom_z = m_hfDesc.bottom;


//	*pdwWidthNum = (DWORD)((right_x - left_x) / m_hfDesc.fFaceSize) + 1;
//	*pdwHeightNum = (DWORD)((bottom_z - top_z) / m_hfDesc.fFaceSize) + 1;
	
	DWORD	dwEndX = (DWORD)((right_x - m_hfDesc.left) / m_hfDesc.fFaceSize);
	DWORD	dwEndZ = (DWORD)((bottom_z - m_hfDesc.top) / m_hfDesc.fFaceSize);

	DWORD	dwStartX = (DWORD)((left_x - m_hfDesc.left) / m_hfDesc.fFaceSize);
	DWORD	dwStartZ = (DWORD)((top_z - m_hfDesc.top) / m_hfDesc.fFaceSize);
	
	*pdwStartX = dwStartX;
	*pdwStartZ = dwStartZ;

	*pdwWidthNum = dwEndX - dwStartX;
	*pdwHeightNum = dwEndZ - dwStartZ;

	return (*pdwWidthNum) * (*pdwHeightNum) * 2;
}
DWORD __stdcall CoHeightField::GetTriListWithAABB(VECTOR3* pv3TriListOut, DWORD dwMaxTriCount, MAABB* pAABB)
{
	DWORD	dwWidthNum;
	DWORD	dwHeightNum;
	DWORD	dwStartX;
	DWORD	dwStartZ;

	DWORD dwNum = QueryNeedTriNumWithAABB(&dwStartX,&dwStartZ,&dwWidthNum,&dwHeightNum,pAABB);

	
	

	if (dwNum > dwMaxTriCount)
	{
		__asm int 3
/*		while (dwNum <= dwMaxTriCount)
		{
			dwStartX++;
			dwStartZ++;
			dwWidthNum -= 2;
			dwHeightNum -= 2;

			dwNum = dwWidthNum*dwHeightNum;
		}*/
	}

	// 삼각형단위로 추출해야하므로...1씩 줄인다.
	dwHeightNum--;
	dwWidthNum--;

	VECTOR3*		pv3Face2Tri = pv3TriListOut;
	

	DWORD		dwTriCount = 0;
	for (DWORD z=dwStartZ; z<dwHeightNum + dwStartZ; z++)
	{
		for (DWORD x=dwStartX; x<dwWidthNum + dwStartX; x++)
		{
#ifdef _DEBUG

			if (x > m_hfDesc.dwYFNumX - 2)
				__asm int 3

			if (z > m_hfDesc.dwYFNumZ - 2)
				__asm int 3
#endif
			pv3Face2Tri[0].y = m_hfDesc.pyfList[x + z*m_hfDesc.dwYFNumX];
			CalcXZ(&pv3Face2Tri[0].x,&pv3Face2Tri[0].z,x,z);

			pv3Face2Tri[1].y = m_hfDesc.pyfList[x +	(z+1)*m_hfDesc.dwYFNumX];
			CalcXZ(&pv3Face2Tri[1].x,&pv3Face2Tri[1].z,x,z + 1);

			pv3Face2Tri[2].y = m_hfDesc.pyfList[x+1	+ z*m_hfDesc.dwYFNumX];
			CalcXZ(&pv3Face2Tri[2].x,&pv3Face2Tri[2].z,x + 1,z);

			pv3Face2Tri[3] = pv3Face2Tri[2];
			pv3Face2Tri[4] = pv3Face2Tri[1];
		
			pv3Face2Tri[5].y = m_hfDesc.pyfList[x + 1	+	(z + 1)*m_hfDesc.dwYFNumX];
			CalcXZ(&pv3Face2Tri[5].x,&pv3Face2Tri[5].z,x + 1,z + 1);

			pv3Face2Tri += 6;
		}
	}
	return dwNum;
}

	

