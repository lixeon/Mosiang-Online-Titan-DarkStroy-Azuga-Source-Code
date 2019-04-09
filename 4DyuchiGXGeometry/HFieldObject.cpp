#include "HFieldObject.h"
#include "HeightField.h"
#include "geometry_global.h"
#include "../4DyuchiGXGFunc/global.h"
#include "CoGeometry.h"
#include "global_variable.h"

CHFieldObject::CHFieldObject()
{
	memset((char*)this+4,0,sizeof(CHFieldObject)-4);
//	memset(m_pSideObj,0xcccccccc,sizeof(m_pSideObj));
	m_fDistanceFromViewPoint = 900000.0f;
	m_bRenderEnable = TRUE;

}

BOOL __stdcall CHFieldObject::IsIncludeViewVolume()
{
	return ClipRenderObjectWithViewVolume(
		m_pHField->GetGeometry()->INL_GetViewvolume(),
		m_pColMeshObjDesc,
		0);
}
BOOL __stdcall CHFieldObject::OnOut()
{
//	m_TileObject.Cleanup();
	return TRUE;
}
BOOL __stdcall CHFieldObject::OnFault()
{
	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);

/*	if (!m_TileObject.GetTilePos())
	{
		DWORD dwTilePosX = m_dwPosX / hfDesc.dwFacesNumPerTileAxis;
		DWORD dwTilePosZ = m_dwPosZ / hfDesc.dwFacesNumPerTileAxis;
		m_TileObject.Initialize(dwTilePosX,dwTilePosZ,&hfDesc);
	}*/
	m_pHFieldControlObject->ReBuildMesh(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc , m_pdwVertexColor);


//	m_pHFieldControlObject->SetYFactor(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc);
//	m_pHFieldControlObject->SetVertexColor(m_pdwVertexColor,hfDesc.dwVerticesNumPerObj);
	return TRUE;
}

BOOL CHFieldObject::Initialize(CoHeightField* pHField,HFIELD_OBJECT_DESC* pHFObjDesc,DWORD dwFlag)
{
	

	HFIELD_DESC	hfDesc;
	pHField->GetHFieldDesc(&hfDesc);

	m_pHField = pHField;

	m_HFObjDesc = *pHFObjDesc;
	
	m_dwPosX = pHFObjDesc->dwPosX;
	m_dwPosZ = pHFObjDesc->dwPosZ;



	// 네 귀퉁이의 버텍스 포지션을 구한다.어차피 x와 z만 사용할 것이므로 갱신은 필요없다.
	DWORD		dwPitch = hfDesc.dwFacesNumPerObjAxis;
	m_pHField->CalcXZ(&m_v3Rect[0].x,&m_v3Rect[0].z,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ);
	m_pHField->CalcY(&m_v3Rect[0].y,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ);
	
	m_pHField->CalcXZ(&m_v3Rect[1].x,&m_v3Rect[1].z,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ + dwPitch);
	m_pHField->CalcY(&m_v3Rect[1].y,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ);

	m_pHField->CalcXZ(&m_v3Rect[2].x,&m_v3Rect[2].z,pHFObjDesc->dwPosX + dwPitch,pHFObjDesc->dwPosZ + dwPitch);
	m_pHField->CalcY(&m_v3Rect[2].y,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ);

	m_pHField->CalcXZ(&m_v3Rect[3].x,&m_v3Rect[3].z,pHFObjDesc->dwPosX + dwPitch,pHFObjDesc->dwPosZ);
	m_pHField->CalcY(&m_v3Rect[3].y,pHFObjDesc->dwPosX,pHFObjDesc->dwPosZ);


//	DWORD dwTilePosX = m_dwPosX / hfDesc.dwFacesNumPerTileAxis;
//	DWORD dwTilePosZ = m_dwPosZ / hfDesc.dwFacesNumPerTileAxis;
//	m_TileObject.Initialize(dwTilePosX,dwTilePosZ,&hfDesc);

	


	m_pdwVertexColor = new DWORD[hfDesc.dwVerticesNumPerObj];
	memset(m_pdwVertexColor,0xff,sizeof(DWORD)*hfDesc.dwVerticesNumPerObj);
	
//	m_pHFieldControlObject->Create(this,m_dwPosX,m_dwPosZ,pHField->GetDetailLevelNum(),pHField->GetFacesNumPerObjAxis(),pHField->GetFacesNumPerObjAxis(),m_v3Rect,&hfDesc);
//	m_pHFieldControlObject->SetYFactor(pHField->GetFacesNumPerObjAxis()+1,&hfDesc);

	if (dwFlag & HFIELD_MODEL_LOAD_NOT_RENDER)
		m_bRenderEnable = FALSE;
	
	BuildAllToIDIMeshObject();
	SetDetailLevel(pHField->GetDetailLevelNum()-1);


	return TRUE;
}

BOOL CHFieldObject::BuildAllToIDIMeshObject()
{
	BOOL	bResult = FALSE;

	if (!m_bRenderEnable)
		goto lb_return;

	if (m_pDeviceObject)
		m_pDeviceObject->Release();
	
	if (m_pHFieldControlObject)
		m_pHFieldControlObject->Release();
	
	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);

	m_pDeviceObject = m_pHField->GetIDIHFieldManager()->CreateHeightFieldObject(&m_HFObjDesc);
	
	DWORD dwTilePosX,dwTilePosZ;

	if (!m_TileObject.GetTilePos())
	{
		dwTilePosX = m_dwPosX / hfDesc.dwFacesNumPerTileAxis;
		dwTilePosZ = m_dwPosZ / hfDesc.dwFacesNumPerTileAxis;
		m_TileObject.Initialize(dwTilePosX,dwTilePosZ,&hfDesc);
	}

	m_pDeviceObject->StartInitialize(NULL,NULL,this);
	m_pDeviceObject->EndInitialize();

	m_pDeviceObject->QueryInterface(IID_HFIELD_OBJECT,(void**)&m_pHFieldControlObject);
	

	m_pHFieldControlObject->Create(		m_dwPosX,m_dwPosZ,
										m_pHField->GetDetailLevelNum(),
										m_pHField->GetFacesNumPerObjAxis(),
										m_pHField->GetFacesNumPerObjAxis(),
										m_v3Rect,&hfDesc
										);
//	m_pHFieldControlObject->SetYFactor(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc);
//	m_pHFieldControlObject->SetVertexColor(m_pdwVertexColor,hfDesc.dwVerticesNumPerObj);
	m_pHFieldControlObject->ReBuildMesh(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc , m_pdwVertexColor);
	

	bResult = TRUE;
lb_return:
	return bResult;
}

BOOL CHFieldObject::UpdateTile()
{
	BOOL	bResult = FALSE;

	if (!m_bRenderEnable)
		goto lb_return;

	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);

//	m_pHFieldControlObject->CleanupAlphaMap();
	m_TileObject.UpdateTileInfo(&hfDesc);
	m_pHFieldControlObject->SetMustUpdate();
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CHFieldObject::UpdateIDIMeshObject()
{
	BOOL	bResult = FALSE;

	if (!m_bRenderEnable)
		goto lb_return;

	m_pHFieldControlObject->SetMustUpdate();
	bResult = TRUE;

lb_return:
	return bResult;
}
	

BOOL CHFieldObject::UpdateVertexPos()
{
	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);
	
	m_pHFieldControlObject->SetYFactor(m_pHField->GetFacesNumPerObjAxis()+1,&hfDesc);
	return TRUE;
}
void CHFieldObject::SetDetailLevel(DWORD dwDetailLevel)
{
	if (!m_bRenderEnable)
		return;
	
	if (m_dwPropertyFlag & HFOBJ_RENDER_TYPE_DETAIL_FIX)
		return;

	if (dwDetailLevel >= m_pHField->GetDetailLevelNum())
		dwDetailLevel = m_pHField->GetDetailLevelNum()-1;

	m_dwDetailLevel = dwDetailLevel;
	m_pHFieldControlObject->SetDetailLevel(dwDetailLevel);
}
void CHFieldObject::SetDistanceFromViewPoint(float fDistance)
{
	m_fDistanceFromViewPoint = fDistance;
	
	if (m_bRenderEnable)
		m_pHFieldControlObject->SetDistanceFromViewPoint(fDistance);

}
void CHFieldObject::SetPositionMask()
{

	DWORD		dwPosMask = 0;
	DWORD		dwMask[4] = {1,2,4,8};
	for (DWORD i=0; i<4; i++)
	{
		if (m_pSideObj[i])
		{
			if (m_pSideObj[i]->GetDetailLevel() > m_dwDetailLevel)
				dwPosMask |= dwMask[i];
		}
	}
	if (m_bRenderEnable)
		m_pHFieldControlObject->SetPositionMask(dwPosMask);

}

BOOL CHFieldObject::ResetCollisionMesh()
{
	CMeshFlag	flag;
	
	HFIELD_DESC	hfDesc;
	m_pHField->GetHFieldDesc(&hfDesc);

	DWORD		dwVerticesNumPerObjAxis = hfDesc.dwFacesNumPerObjAxis+1;
	DWORD		dwVerticesNum = dwVerticesNumPerObjAxis*dwVerticesNumPerObjAxis;
	
		
	VECTOR3*	pv3Pos = new VECTOR3[dwVerticesNum];
	memset(pv3Pos,0,sizeof(VECTOR3)*dwVerticesNum);
		

	DWORD		dwCount = 0;
	
	for (DWORD j=0; j<dwVerticesNumPerObjAxis; j++)
	{
		for (DWORD k=0; k<dwVerticesNumPerObjAxis; k++)
		{
			DWORD xpos = m_dwPosX+k;
			DWORD zpos = m_dwPosZ+j;

			pv3Pos[dwCount].y = hfDesc.pyfList[hfDesc.dwYFNumX*zpos + xpos];
			m_pHField->CalcXZ(
				&pv3Pos[dwCount].x,
				&pv3Pos[dwCount].z,
				xpos,
				zpos);
			
			dwCount++;
		}
	}
	::CalcBoundingMesh(pv3Pos,dwVerticesNum,m_pColMeshObjDesc);
		
	
	delete [] pv3Pos;
	return TRUE;
}


BOOL CHFieldObject::BeginShadeVertexColor(DWORD dwColor)
{
	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);


	m_pv3VertexNormal = new VECTOR3[hfdesc.dwVerticesNumPerObj];
	m_pv3Point = new VECTOR3[hfdesc.dwVerticesNumPerObj];
	
	DWORD	dwCount = 0;

	for (DWORD z=0; z<hfdesc.dwFacesNumPerObjAxis+1; z++)
	{
		for (DWORD x=0; x<hfdesc.dwFacesNumPerObjAxis+1; x++)
		{
			m_pdwVertexColor[dwCount] = dwColor; 

			m_pv3VertexNormal[dwCount].x = 0.0f;
			m_pv3VertexNormal[dwCount].y = 1.0f;
			m_pv3VertexNormal[dwCount].z = 0.0f;
			
			m_pHField->CalcY(&m_pv3Point[dwCount].y,m_dwPosX+x,m_dwPosZ+z);
			m_pHField->CalcXZ(&m_pv3Point[dwCount].x,&m_pv3Point[dwCount].z,m_dwPosX+x,m_dwPosZ+z);
			dwCount++;
		}
	}
	return TRUE;
}
BOOL CHFieldObject::ShadeVertexColor(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,DWORD dwFlag,SHADE_FUNC pFunc)
{
	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);

	::ShadeVertexColor(m_pdwVertexColor,m_pv3Point,m_pv3VertexNormal,hfdesc.dwVerticesNumPerObj,pv3,dwFacesNum,pLight,pFunc);
	
	return FALSE;
}

void CHFieldObject::ReleaseVertexColor()
{
	if (m_pdwVertexColor)
	{
		delete [] m_pdwVertexColor;
		m_pdwVertexColor = NULL;
	}
}
void CHFieldObject::Optimize()
{
//	ReleaseVertexColor();
}
void CHFieldObject::EndShadeVertexColor(FILE* fp)
{
	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);

	if (m_bRenderEnable)
		m_pHFieldControlObject->SetVertexColor(m_pdwVertexColor,hfdesc.dwVerticesNumPerObj);

	if (fp)
	{
		fwrite(m_pdwVertexColor,sizeof(DWORD),hfdesc.dwVerticesNumPerObj,fp);
	}
	
	if (m_pv3VertexNormal)
	{
		delete [] m_pv3VertexNormal;
		m_pv3VertexNormal = NULL;
	}
	if (m_pv3Point)
	{
		delete [] m_pv3Point;
		m_pv3Point = NULL;
	}
}
void CHFieldObject::SetVertexColor(DWORD dwColor)
{
	if (m_bRenderEnable)
		m_pHFieldControlObject->SetVertexColorAll(dwColor);
}

BOOL CHFieldObject::SetAlphaTexel(VECTOR3* pv3IntersectPoint,float fRs,DWORD dwTexWidthHeight,DWORD dwTileIndex,DWORD dwActionFlag)
{
	BOOL	bResult = FALSE;
	DWORD	dwRS = 1;
	DWORD	dwCenterX,dwCenterZ;

	float	fWidth =   m_v3Rect[2].x - m_v3Rect[0].x;
	float	fHeight =   m_v3Rect[2].z - m_v3Rect[0].z;
	dwCenterX = (DWORD)(((pv3IntersectPoint->x - m_v3Rect[0].x) / fWidth) * (float)dwTexWidthHeight);
	dwCenterZ = (DWORD)(((pv3IntersectPoint->z - m_v3Rect[0].z) / fHeight) * (float)dwTexWidthHeight);


	TILE_BUFFER_DESC*	pTileBufferDesc;

	pTileBufferDesc = m_TileObject.SetAlphaTexel(m_pHField->GetHFieldTools(),this,dwTileIndex,dwActionFlag,dwCenterX,dwCenterZ,dwRS,dwTexWidthHeight);
	if (pTileBufferDesc)
	{
		m_pHFieldControlObject->UpdateAlphaMap(pTileBufferDesc);

	}

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL CHFieldObject::SetLightVertexel(HFIELD_POS* pPos,DWORD dwHFPosNum,DWORD dwColor)
{
	BOOL	bResult = FALSE;
	
	int		iPosX;
	int		iPosZ;
	DWORD	dwSelectedVertexIndex;
	
	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);

	if (!m_pdwVertexColor)
		goto lb_return;

	DWORD	i;
	for (i=0; i<dwHFPosNum; i++)
	{

		iPosX = (int)pPos[i].dwX - (int)m_dwPosX;
		iPosZ = (int)pPos[i].dwZ - (int)m_dwPosZ;
	
		if (iPosX < 0)
			iPosX = 0;

		if (iPosX >= hfdesc.dwFacesNumPerObjAxis)
			iPosX = hfdesc.dwFacesNumPerObjAxis;

		if (iPosZ < 0)
			iPosZ = 0;

		if (iPosZ >= hfdesc.dwFacesNumPerObjAxis)
			iPosZ = hfdesc.dwFacesNumPerObjAxis;
		

		dwSelectedVertexIndex = (hfdesc.dwFacesNumPerObjAxis+1)*iPosZ + iPosX;
		m_pdwVertexColor[dwSelectedVertexIndex] = dwColor;

		if (m_bRenderEnable)
			m_pHFieldControlObject->SetVertexColor(m_pdwVertexColor,hfdesc.dwVerticesNumPerObj);

	}
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CHFieldObject::GetLightVertexel(DWORD* pdwColor,HFIELD_POS* pPos)
{
	BOOL	bResult = FALSE;
	
	int		iPosX = (int)pPos->dwX - (int)m_dwPosX;
	int		iPosZ = (int)pPos->dwZ - (int)m_dwPosZ;
	DWORD	dwSelectedVertexIndex;

#ifdef _DEBUG

	if (iPosX < 0)
		__asm int 3

	if (iPosZ < 0)
		__asm int 3
#endif

	if (!m_pdwVertexColor)
		goto lb_return;

	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);
	dwSelectedVertexIndex = (hfdesc.dwFacesNumPerObjAxis+1)*iPosZ + iPosX;
	*pdwColor = m_pdwVertexColor[dwSelectedVertexIndex];

	bResult = TRUE;

lb_return:
	return bResult;
}


DWORD CHFieldObject::ReadVertexColor(void* pFP)
{
	DWORD	dwLen = 0;
	
	HFIELD_DESC	hfdesc;
	m_pHField->GetHFieldDesc(&hfdesc);
	dwLen += g_pFileStorage->FSRead(pFP,m_pdwVertexColor,sizeof(DWORD)*hfdesc.dwVerticesNumPerObj);

	if (m_bRenderEnable)
		m_pHFieldControlObject->SetVertexColor(m_pdwVertexColor,hfdesc.dwVerticesNumPerObj);
	

	return dwLen;
}

void __stdcall CHFieldObject::GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum)
{
	m_TileObject.GetTileBuffer(ppTileBufferDesc,pdwTileBufferDescNum);
}
DWORD __stdcall	CHFieldObject::GetTileBufferDescNum()
{
	return m_TileObject.GetTileBufferDescNum();
}
TILE_BUFFER_DESC* __stdcall	CHFieldObject::GetTileBufferDesc(DWORD dwIndex)
{
	return m_TileObject.GetTileBufferDesc(dwIndex);
}


CoHeightField* CHFieldObject::GetHField(CTileObject* pTileObject)
{
	/*
	CTileObject					m_TileObject;
	IDIMeshObject*				m_pDeviceObject;
	IDIHFieldObject*			m_pHFieldControlObject;
	COLLISION_MESH_OBJECT_DESC*	m_pColMeshObjDesc;
	CoHeightField*				m_pHField;
	*/

	CHFieldObject*	pHFieldObject = NULL;
	DWORD dwOffset = (DWORD)&pHFieldObject->m_pHField - (DWORD)&pHFieldObject->m_TileObject;
	CoHeightField* pHField = *(CoHeightField**)( (DWORD)pTileObject + dwOffset );
	return pHField;
}

CHFieldObject::~CHFieldObject()
{
	ReleaseVertexColor();

	if (m_pDeviceObject)
	{
		m_pDeviceObject->Release();
		m_pDeviceObject = NULL;
	}
	if (m_pHFieldControlObject)
	{
		m_pHFieldControlObject->Release();
		m_pHFieldControlObject = NULL;
	}
}
