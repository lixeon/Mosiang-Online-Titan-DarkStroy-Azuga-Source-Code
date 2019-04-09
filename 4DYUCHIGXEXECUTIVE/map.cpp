#include "map.h"
#include "gxdefault.h"
#include "GXObject.h"
#include "GXEventTriggerObject.h"
#include "executive.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_myself97/MClipper.h"
#include "global_variable.h"
#include "ShadeManager.h"
#include "../4DyuchiGRX_myself97/myself97common.h"

CGXMap::CGXMap()
{
	memset(this,0,sizeof(CGXMap));

}
BOOL CGXMap::Initialize(CoExecutive* pExecutive,GXMapSchedulePROC pProc,DWORD dwMaxObjectNum)
{
	if (m_pClipper)
		__asm int 3

	m_pExecutive = pExecutive;

	m_dwStaticMaxGXMapObjectNum = dwMaxObjectNum;
	m_ppStaticGXMapObjList = new CGXMapObject*[m_dwStaticMaxGXMapObjectNum];

	memset(m_ppStaticGXMapObjList,0,sizeof(CGXMapObject*)*m_dwStaticMaxGXMapObjectNum);


	m_pClipper = new MClipper;
	m_pClipper->Init(m_pExecutive, m_dwStaticMaxGXMapObjectNum);

	return TRUE;
}


BOOL CGXMap::BeginBuildMap(DWORD dwFlag)
{
	m_dwBuildTreeFlag	=	dwFlag;
	return TRUE;
}

BOOL CGXMap::EndBuildMap(float fTop,float fBottom)
{
	DWORD	i;
	CGXMapObject*	pStaticObject;
	DWORD	dwClipperIndex;
	// static gx오브젝트를 클리퍼에 밀어넣는다.
	for (i=0; i<m_dwStaticGXMapObjectNum; i++)
	{
		
		pStaticObject = m_ppStaticGXMapObjList[i];
		if (pStaticObject->GetClipperIndex() == 0xffffffff)
		{
			dwClipperIndex = m_pClipper->AddStaticObject(pStaticObject);
			pStaticObject->SetClipperIndex(dwClipperIndex);
		}
	}

	DWORD	dwStaticTreeBuildClock	=	0;
	if( m_dwBuildTreeFlag & BUILD_TREE_FLAG_BUILD_STATIC_OBJECT) 
	{
		// static object 트리 빌드. 시간 볼려고 따로 빼다.
//		dwStaticTreeBuildClock	=	GetLowClock();
		for (i=0; i<m_dwSTMGXMapObjNum; i++)
		{
			m_pSTMGXMapObjList[i].BuildBSPTree();
		}
//		dwStaticTreeBuildClock	=	GetLowClock()	-	dwStaticTreeBuildClock;
	}
	
	MAABB	WorldBoundBox;

	GetWorldBoundingBox(&WorldBoundBox);

	if (WorldBoundBox.Max.y < fTop)
		WorldBoundBox.Max.y = fTop;

	if (WorldBoundBox.Min.y > fBottom)
		WorldBoundBox.Min.y = fBottom;

	if (m_dwStaticGXMapObjectNum)
		m_pClipper->BuildTree(0, WorldBoundBox, 6);			// 범용적으로 가장 좋은값은.?

	return TRUE;
}

void CGXMap::RebuildMap(float fTop,float fBottom)
{
	m_pClipper->Rebuild(fTop,fBottom);
}
DWORD CGXMap::AddDynamicObject( CGXMapObject* pObject)
{
	DWORD	dwClipperIndex = 0xffffffff;

	if (pObject->GetClipperIndex() != 0xffffffff)
		goto lb_return;

	if (pObject->GetPropertyFlag() & GXMAP_OBJECT_COMMON_TYPE_NOT_USE_CLIPPER)
		goto lb_return;


	dwClipperIndex = m_pClipper->AddDynamicObject(pObject);
	pObject->SetClipperIndex(dwClipperIndex);
	m_pClipper->UpdateDynamicObject(dwClipperIndex);

lb_return:
	return dwClipperIndex;
}
// HField,STM ,다이나믹하지 않은 개체는 다 들어간다..
DWORD CGXMap::InsertStaticGXMapObject(CGXMapObject* pObj)
{
	m_ppStaticGXMapObjList[m_dwStaticGXMapObjectNum] = pObj;
	m_dwStaticGXMapObjectNum++;
	return m_dwStaticGXMapObjectNum;
}
BOOL CGXMap::InsertSTM(I3DStaticModel* pStaticModel, DWORD BuildFlag)
{
	BOOL		bResult = FALSE;

#ifdef _DEBUG
	if (m_pSTMGXMapObjList)
		__asm int 3
#endif
	DWORD		i;
	if (!pStaticModel)
		goto lb_return;

	m_pBaseModel = pStaticModel;
	m_pBaseModel->AddRef();

	m_dwSTMGXMapObjNum = m_pBaseModel->GetObjectNum();
	m_pSTMGXMapObjList = new CGXStaticMapObject[m_dwSTMGXMapObjNum];
	DWORD	dwIndex;

	for (i=0; i<m_dwSTMGXMapObjNum; i++)
	{
		dwIndex = InsertStaticGXMapObject(m_pSTMGXMapObjList+i);
		m_pSTMGXMapObjList[i].Initialize(m_pExecutive,this,dwIndex,m_pBaseModel->GetObjectIndex(i));

	}

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CGXMap::InsertHField(IHeightField* pHField)
{
	
	BOOL		bResult = FALSE;

	DWORD		i;
	if (!pHField)
		goto lb_return;
#ifdef _DEBUG
	if (m_pGXHFieldMapObjList)
		__asm int 3
#endif
	m_pHField = pHField;
	m_pHField->AddRef();

	m_dwGXHFieldMapObjNum = m_pHField->GetObjectNum();
	m_pGXHFieldMapObjList = new CGXHFieldMapObject[m_dwGXHFieldMapObjNum];
	DWORD	dwIndex;

	for (i=0; i<m_dwGXHFieldMapObjNum; i++)
	{
		dwIndex = InsertStaticGXMapObject(m_pGXHFieldMapObjList+i);
		m_pGXHFieldMapObjList[i].Initialize(m_pExecutive,this,dwIndex,m_pHField->GetObjectIndex(i));
		
	}

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CGXMap::CreateInitialGXObjectList(DWORD dwNum)
{
	BOOL	bResult = FALSE;

	if (!dwNum)
		goto lb_return;
#ifdef _DEBUG
	if (m_ppInitialGXObjectList)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CGXMap::CreateGXObjectList(), Already GXObject Created, File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);

	}
#endif

	m_ppInitialGXObjectList = new CoGXObject*[dwNum];
	memset(m_ppInitialGXObjectList,0,sizeof(CoGXObject*)*dwNum);
	m_dwMaxInitialGXObectNum = dwNum;
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL CGXMap::InsertInitialGXObject(CoGXObject* pGXO)
{
	BOOL	bResult = FALSE;
	if (m_dwInitialGXObjectNum >= m_dwMaxInitialGXObectNum)
		goto lb_return;

	m_ppInitialGXObjectList[m_dwInitialGXObjectNum] = pGXO;
	pGXO->SetIndexInIntialTable(m_dwInitialGXObjectNum);
	m_dwInitialGXObjectNum++;
	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CGXMap::CreateInitialGXLightList(DWORD dwNum)
{
	BOOL	bResult = FALSE;

	if (!dwNum)
		goto lb_return;

#ifdef _DEBUG
	if (m_ppInitialGXLightList)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CGXMap::CreateGXLightList(), Already GXLight Created, File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	m_ppInitialGXLightList = new CoGXLight*[dwNum];
	memset(m_ppInitialGXLightList,0,sizeof(CoGXLight*)*dwNum);
	m_dwMaxInitialGXLightNum = dwNum;
	bResult = TRUE;
lb_return:
	return bResult;
	
}
BOOL CGXMap::InsertInitialGXLight(CoGXLight* pGXL)
{
	BOOL	bResult = FALSE;
	if (m_dwInitialGXLightNum >= m_dwMaxInitialGXLightNum)
		goto lb_return;

	m_ppInitialGXLightList[m_dwInitialGXLightNum] = pGXL;
	pGXL->SetIndexInIntialTable(m_dwInitialGXLightNum);
	m_dwInitialGXLightNum++;
	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL CGXMap::CreateInitialGXTriggerList(DWORD dwNum)
{
	BOOL	bResult = FALSE;
	
	if (!dwNum)
		goto lb_return;

#ifdef _DEBUG
	if (m_ppInitialGXTriggerList)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"CGXMap::CreateGXTriggerList(), Already GXTrigger Created, File:%s , Line:%d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	m_ppInitialGXTriggerList = new CoGXEventTrigger*[dwNum];
	memset(m_ppInitialGXTriggerList,0,sizeof(CoGXEventTrigger*)*dwNum);
	m_dwMaxInitialGXTriggerNum = dwNum;
	bResult = TRUE;

lb_return:
	return bResult;

}
BOOL CGXMap::InsertInitialGXTrigger(CoGXEventTrigger* pGXT)
{
	BOOL	bResult = FALSE;
	if (m_dwInitialGXTriggerNum >= m_dwMaxInitialGXTriggerNum)
		goto lb_return;

	m_ppInitialGXTriggerList[m_dwInitialGXTriggerNum] = pGXT;
	pGXT->SetIndexInIntialTable(m_dwInitialGXTriggerNum);
	m_dwInitialGXTriggerNum++;
	bResult = TRUE;

lb_return:
	return bResult;
}

void CGXMap::DeleteInitialGXObject(DWORD dwInitialiIndex)
{
	m_ppInitialGXObjectList[dwInitialiIndex] = NULL;
}
void CGXMap::DeleteInitialGXLight(DWORD dwInitialiIndex)
{
	m_ppInitialGXLightList[dwInitialiIndex] = NULL;
}
void CGXMap::DeleteInitialGXTrigger(DWORD dwInitialiIndex)
{
	m_ppInitialGXTriggerList[dwInitialiIndex] = NULL;
}
void CGXMap::GetWorldBoundingBox(MAABB* pAABB)
{
	COLLISION_MESH_OBJECT_DESC	col;
	VECTOR3	v3[8*2];
	VECTOR3*	pv3Entry = v3;
	DWORD		dwVertexNum = 0;

	if (m_pHField)
	{
		m_pHField->GetWorldBoundingMesh(&col);
		memcpy(pv3Entry,col.boundingBox.v3Oct,sizeof(VECTOR3)*8);
		pv3Entry += 8;
		dwVertexNum += 8;
	}
	if (m_pBaseModel)
	{
		m_pBaseModel->GetWorldBoundingMesh(&col);
		memcpy(pv3Entry,col.boundingBox.v3Oct,sizeof(VECTOR3)*8);
		dwVertexNum += 8;
	}
	CMeshFlag	flag;

	if (!dwVertexNum)
		memset(&col,0,sizeof(col));
	
	::CalcBoundingMesh(v3,dwVertexNum,&col);
	pAABB->Min = col.boundingBox.v3Oct[5];
	pAABB->Max = col.boundingBox.v3Oct[3];
	
}



struct IVERTEX_SET
{
	IVERTEX*		pVertex;
	DWORD			dwNum;
	CGXMapObject*	pMapObject;
};

BOOL CGXMap::ShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc)
{

	CGXMapObject*	pMapObjectShade[MAX_RENDER_OBJECT_NUM];
	DWORD			dwObjNumPerLight;
	DWORD			dwTotalFacesNum;
	DWORD			dwExecLightNum;
	
	IVERTEX_SET*	pIVSet;
	VECTOR3*		pv3 = NULL;
	VECTOR3*		pv3Offset = NULL;
	DWORD			i,j,k;
	BOOL			bResult = FALSE;
	BOOL			bNeedDel = FALSE;
	
	CoGXLight*		pGXL;

	
	dwExecLightNum = m_pExecutive->GetGXLightsNum();
	
	if (dwExecLightNum)
	{
		pLight = new LIGHT_DESC[dwExecLightNum];
		bNeedDel = TRUE;
		dwLightNum = 0;

		for (i=0; i<dwExecLightNum; i++)
		{
			pGXL = (CoGXLight*)m_pExecutive->GetGXLightWithSeqIndex(i);
			if (pGXL->IsRenderable())
			{
				if (pGXL->GetPropertyFlag() & GXLIGHT_TYPE_STATIC)
				{
					pGXL->GetLightDesc(pLight+dwLightNum);
					pLight[dwLightNum].dwFlag = pGXL->GetPropertyFlag();
					dwLightNum++;
				}
			}
		}
	}

	if (m_pHField)
		m_pHField->BeignShadeLightMapObject(dwAmbientColor);
	
	if (m_pBaseModel)
		m_pBaseModel->BeignShadeLightMapObject(dwAmbientColor);
	
	CShadeManager	shadeManager;
	
	for (i=0; i<dwLightNum; i++)
	{
		dwTotalFacesNum = 0;
		
		BOUNDING_SPHERE		BS;
		BS.v3Point	=	pLight[i].v3Point;
		BS.fRs		=	pLight[i].fRs;
		m_pClipper->FindNearObject( BS);
	
		dwObjNumPerLight = m_pClipper->GetMeshObjects(pMapObjectShade,8192);

		if (!dwObjNumPerLight)
			continue;

		pIVSet = new IVERTEX_SET[dwObjNumPerLight];
		memset(pIVSet,0,sizeof(IVERTEX_SET)*dwObjNumPerLight);

		for (j=0; j<dwObjNumPerLight; j++)
		{
			
			pIVSet[j].pMapObject = pMapObjectShade[j];

			if (!(pLight[i].dwFlag & GXLIGHT_TYPE_STATIC_SHADOW_DISABLE))
			{

				if (dwFlag & STATIC_MODEL_SHADE_TYPE_ENABLE_SHADOW)
				{
					pIVSet[j].dwNum = pIVSet[j].pMapObject->CreateIVertexList(&pIVSet[j].pVertex);
					dwTotalFacesNum += pIVSet[j].dwNum;
				}
			}
		}
		
		if (dwTotalFacesNum)
			pv3 = new VECTOR3[dwTotalFacesNum*3];
		else 
			pv3 = NULL;

		for (j=0; j<dwObjNumPerLight; j++)
		{
		
			DWORD		dwTriCount = 0;
			pv3Offset = pv3;

			VIEW_VOLUME	vv;
			BOOL		bVV = MakeViewVolumeIncludingSphere(
				&vv,
				&pLight[i].v3Point,
				&pIVSet[j].pMapObject->GetCollisionMeshObjectDesc()->boundingSphere,
				pLight[i].fRs
				);
			for (k=0; k<dwObjNumPerLight; k++)
			{
				
				if (bVV)
				{
					// 뷰볼륨이 만들어지는 경우는 라이트 뷰볼륨 안에 들어가는 오브젝트만 그림자용 충돌메쉬로 잡는다.
					if (!ClipRenderObjectWithViewVolume(&vv,pIVSet[k].pMapObject->GetCollisionMeshObjectDesc(),0))
						continue;
				}
				else
				{
					// 뷰볼륨을 만들수 없는 경우는 라이트를 받을 오브젝트의 바운딩 스피어과 겹치는
					// 오브젝트들만 그림자용 충돌 매쉬로 잡는다.
					if (!IsCollisionSpherAndSphere(
						&pIVSet[j].pMapObject->GetCollisionMeshObjectDesc()->boundingSphere.v3Point,
						pIVSet[j].pMapObject->GetCollisionMeshObjectDesc()->boundingSphere.fRs,
						&pIVSet[k].pMapObject->GetCollisionMeshObjectDesc()->boundingSphere.v3Point,
						pIVSet[k].pMapObject->GetCollisionMeshObjectDesc()->boundingSphere.fRs))
						continue;


				}
				dwTriCount += pIVSet[k].dwNum;
				for (DWORD l=0; l<pIVSet[k].dwNum*3; l++)
				{
					*(VECTOR3*)&pv3Offset->x = *(VECTOR3*)&pIVSet[k].pVertex[l].x;
					pv3Offset++;
				}
			}
//			pIVSet[j].pMapObject->ShadeLightMapObject(pIVSet[j].pv3ForShadow,dwTriCount,pLight+i);
			shadeManager.InsertShadeItem(pIVSet[j].pMapObject,pv3,dwTriCount,pLight+i,pFunc);
		
		}
		shadeManager.WaitShading();

		for (j=0; j<dwObjNumPerLight; j++)
		{
			if (pIVSet[j].dwNum)
			{
				pIVSet[j].pMapObject->ReleaseIVertexList(pIVSet[j].pVertex);
				pIVSet[j].pVertex = NULL;
			}
		}
		delete [] pIVSet;
		if (pv3)
		{
			delete [] pv3;
			pv3 = NULL;
		}


	}
	if (m_pBaseModel)
		m_pBaseModel->EndShadeLightMapObject();

	if (m_pHField)
		m_pHField->EndShadeLightMapObject();


	if (bNeedDel)
		delete [] pLight;

	bResult = TRUE;

lb_return:
	return bResult;
}
DWORD CGXMap::GetAllInitialGXObjects(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum)
{
	DWORD	dwCount = 0;

	if (!m_ppInitialGXObjectList)
		goto lb_return;

	DWORD	i;
	for (i=0; i<m_dwInitialGXObjectNum; i++)
	{
#ifdef	_DEBUG
		char	txt[512];
		if (!m_ppInitialGXObjectList[i])
		{
			memset(txt,0,512);
			wsprintf(txt,"CGXMap::GetAllInitialGXObjects(), if (!m_ppInitialGXObjectList[i]), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}

		if(m_pExecutive->IsValidHandle(m_ppInitialGXObjectList[i]) == GX_MAP_OBJECT_TYPE_INVALID)
		{
			memset(txt,0,512);
			wsprintf(txt,"CGXMap::GetAllInitialGXObjects(), if(IsValidHandle(m_ppInitialGXObjectList[i])==GX_MAP_OBJECT_TYPE_INVALID), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif
		if (dwCount >= dwMaxNum)
			goto lb_return;

		pObjArray[dwCount] = m_ppInitialGXObjectList[i];
		dwCount++;
	}
	
lb_return:
	return dwCount;
}
void CGXMap::DeleteAllInitialObjects()
{
	DWORD	i;

	if (m_ppInitialGXObjectList)
	{

		for (i=0; i<m_dwInitialGXObjectNum; i++)
		{
#ifdef	_DEBUG
			char	txt[512];
			if (!m_ppInitialGXObjectList[i])
			{
				memset(txt,0,512);
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if (!m_ppInitialGXObjectList[i]), File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}

			if(m_pExecutive->IsValidHandle(m_ppInitialGXObjectList[i]) == GX_MAP_OBJECT_TYPE_INVALID)
			{
				memset(txt,0,512);
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if(IsValidHandle(m_ppInitialGXObjectList[i])==GX_MAP_OBJECT_TYPE_INVALID), File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif
			m_pExecutive->ImmDeleteGXObject(m_ppInitialGXObjectList[i]);
		}
		delete [] m_ppInitialGXObjectList;
		m_ppInitialGXObjectList = NULL;
	}
	if (m_ppInitialGXLightList)
	{

		for (i=0; i<m_dwInitialGXLightNum; i++)
		{
#ifdef	_DEBUG
			char	txt[512];
			if (!m_ppInitialGXLightList[i])
			{
				memset(txt,0,512);
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if (!m_ppInitialGXLightList[i]), File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}

			if(m_pExecutive->IsValidHandle(m_ppInitialGXLightList[i]) == GX_MAP_OBJECT_TYPE_INVALID)
			{
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if(IsValidHandle(m_ppInitialGXLightList[i])==GX_MAP_OBJECT_TYPE_INVALID, File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif
			m_pExecutive->ImmDeleteGXLight(m_ppInitialGXLightList[i]);

		}
		delete [] m_ppInitialGXLightList;
		m_ppInitialGXLightList = NULL;

	}
	if (m_ppInitialGXTriggerList)
	{
		for (i=0; i<m_dwInitialGXTriggerNum; i++)
		{
#ifdef	_DEBUG
			char	txt[512];
			if (!m_ppInitialGXTriggerList[i])
			{
				memset(txt,0,512);
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if (!m_ppInitialGXTriggerList[i]), File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}

			if(m_pExecutive->IsValidHandle(m_ppInitialGXTriggerList[i]) == GX_MAP_OBJECT_TYPE_INVALID)
			{
				wsprintf(txt,"CGXMap::DeleteAllInitialObjects(), if (IsValidHandle(m_ppInitialGXTriggerList[i])==GX_MAP_OBJECT_TYPE_INVALID, File:%s , Line:%d \n",__FILE__,__LINE__);
				DWORD	dwAddr;
				GetEIP(&dwAddr);
				g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			}
#endif
			m_pExecutive->ImmDeleteGXEventTrigger(m_ppInitialGXTriggerList[i]);
		}
		delete [] m_ppInitialGXTriggerList;
		m_ppInitialGXTriggerList = NULL;
	}
}

void CGXMap::SetHFieldAlphaFlag(DWORD dwAlpha)
{
	dwAlpha &= 0x000000ff;
	for (DWORD i=0; i<m_dwGXHFieldMapObjNum; i++)
	{
		m_pGXHFieldMapObjList[i].SetAlphaFlag(dwAlpha);
	}
}
void CGXMap::SetSTMAlphaFlag(DWORD dwAlpha)
{
	dwAlpha &= 0x000000ff;
	for (DWORD i=0; i<m_dwSTMGXMapObjNum; i++)
	{
		m_pSTMGXMapObjList[i].SetAlphaFlag(dwAlpha);
	}
}
CGXMap::~CGXMap()
{
	m_pClipper->DeleteAll();
	delete m_pClipper;
	m_pClipper = NULL;
	
	if (m_ppStaticGXMapObjList)
	{
		delete [] m_ppStaticGXMapObjList;
		m_ppStaticGXMapObjList = NULL;
	}
	if (m_pSTMGXMapObjList)
	{
		delete [] m_pSTMGXMapObjList;
		m_pSTMGXMapObjList = NULL;
	}
	if (m_pGXHFieldMapObjList)
	{
		delete [] m_pGXHFieldMapObjList;
		m_pGXHFieldMapObjList = NULL;
	}	

	if (m_pBaseModel)
	{
		m_pBaseModel->Release();
		m_pBaseModel = NULL;
	}
	if (m_pHField)
	{
		m_pHField->Release();
		m_pHField = NULL;
	}
}
