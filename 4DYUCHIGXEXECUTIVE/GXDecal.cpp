// GXDecal.cpp: implementation of the CGXDecal class.
//
//////////////////////////////////////////////////////////////////////

#include "GXDecal.h"
#include "../4DYuchiGRX_myself97/TriangleLib.h"
#include "../4DYuchiGRX_myself97/CheckClock.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DYuchiGXGFunc/global.h"
#include "Executive.h"
#include "GXDefault.h"


VECTOR3		g_pBuff1[DECAL_TRI_MAX];
VECTOR3		g_pBuff2[DECAL_TRI_MAX];
int			g_piDet[DECAL_TRI_MAX];
IVERTEX		g_pTri[3*DECAL_TRI_MAX];




DWORD CGXDecal::ClipOneTri( VECTOR3* pTri)
{
	if( m_dwTriCount == DECAL_TRI_MAX)
	{ 
#ifdef	_DEBUG
//		_asm int 3;			// 최대치를 넘는 경우.
#endif
		return	0;
		
	}
	VECTOR3*	pSrc	=	g_pBuff1;
	VECTOR3*	pDest	=	g_pBuff2;
	VECTOR3*	pTemp;
	memcpy( pSrc, pTri, sizeof(VECTOR3)*3);
	DWORD		dwSrc	=	3;
	DWORD		dwDest	=	0;
	DWORD	i;
	for( i = 0; i < 6; i++)
	{
		dwDest	=	ClipPolygonWithPlane( pDest, pSrc, dwSrc, &(m_pBoxPlane[i]));
		if( dwDest == 0)
			return 0;
		if( dwDest <= 2)
		{
			_asm nop;
			return	0;
		}
		if( dwDest >= DECAL_TRI_MAX)			// 최대치를 넘으면... 그냥 버린다.
		{
			_asm int 3;
			return	0;
		}

		// 소스와 데스트 바꾸고,
		pTemp	=	pSrc;
		pSrc	=	pDest;
		pDest	=	pTemp;
		dwSrc	=	dwDest;
	}

	if( dwSrc == 0)
		return	0;

	if( dwSrc <= 2)
	{
		_asm int 3;		// 결과가 직선으로 나온 경우 삼각형이 성립 안되므로 띵~
		return	0;
	}
	
	DWORD	j;
	// 폴리곤을 삼각형으로 풀어서 IVERTEX에 넣는다. 나중엔 삼각형 팬으로 바로 저장.
	for( i = 1; i < dwSrc-1; i++)
	{
		g_pTri[m_dwTriCount*3+0].x	=	pSrc[0].x;
		g_pTri[m_dwTriCount*3+0].y	=	pSrc[0].y;
		g_pTri[m_dwTriCount*3+0].z	=	pSrc[0].z;

		for( j = 0; j < 2; j++)
		{
			g_pTri[m_dwTriCount*3+1+j].x	=	pSrc[i+j].x;
			g_pTri[m_dwTriCount*3+1+j].y	=	pSrc[i+j].y;
			g_pTri[m_dwTriCount*3+1+j].z	=	pSrc[i+j].z;
		}

		m_dwTriCount++;
		if( m_dwTriCount >= DECAL_TRI_MAX)
			break;
	}

	_asm nop;
}

// pOutPoly, 결과물 버퍼.
// 리턴값은 잘린후의 폴리곤 버텍스 갯수.
// pInPoly, 입력 버퍼.
// dwInPolyCount, 들어가는 폴리곤의 버텍스 갯수.
// pClipPlane, 자를 평면.
// 평면의 뒷 폴리곤은 버린다.
DWORD CGXDecal::ClipPolygonWithPlane( VECTOR3* pOutPoly, VECTOR3* pInPoly, DWORD dwInPolyCount, PLANE* pClipPlane)
{

#ifdef	_DEBUG
	if( dwInPolyCount < 3)
	{
		_asm int 3;				// 최소한의 삼각형도 안된다. 평면에 잘릴 자격도 없지 않은가.?
		return	0;
	}
#endif

	DWORD	dwOutPolyCount	=	0;

//	int		piDet[DECAL_TRI_MAX];			// 각 점이 평면의 어디에 있는가.? +1, 0, -1
	int*	piDet	=	g_piDet;
	// 위치 갯수. 이중에 쓰이는건 네가티브만인가.?
//	DWORD	dwPositive	=	0;
	DWORD	dwNegative	=	0;
//	DWORD	dwZero		=	0;
	
	float	t;						// 임시 변수 시점값 t
	VECTOR3		v;

	DWORD	i;

	// 부호 검사하고,
	for( i = 0; i < dwInPolyCount; i++)
	{
		float	fDet	=	DotProduct( &(pInPoly[i]), &(pClipPlane->v3Up))	+	pClipPlane->D;
		if( fDet > 0.0001f)
		{
			piDet[i]	=	1;
//			dwPositive++;
		}
		else if( fDet < -0.0001f)
		{
			piDet[i]	=	-1;
			dwNegative++;
		}
		else
		{
			piDet[i]	=	0;
//			dwZero++;
		}
	}

	// 혹시 모든 점이 평면 뒷쪽에 존재하는가.?
	if( dwInPolyCount == dwNegative)
	{
		return	0;		// 그러면 아무것도 자를것이 없으니 리턴.
	}

	// 혹시 모든점이 평면의 앞 혹은 평면위에 있는가.?
	if( dwNegative == 0)
	{
		// 그대로 카피해서 내보낸다.
		memcpy( pOutPoly, pInPoly, sizeof(VECTOR3)*dwInPolyCount);
		return	dwInPolyCount;
	}

	// 엣지 갯수.루프를 돌면서, 평면의 앞에 있다 판단되면 아웃풋 버퍼에 추가한다. 단순하게 생각하자.
	for( i = 0; i < dwInPolyCount; i++)
	{
		// 끝점 처리를 위한 인덱스.
		DWORD	dwCurrent	=	i;				// 지금점 인덱스.
		DWORD	dwNext;					// 다음점 인덱스.
		if( i == dwInPolyCount-1)
			dwNext	=	0;
		else	dwNext	=	i+1;

		if( piDet[dwCurrent] != -1)		// '+', '0'
		{
			// 공통 코드, 현재점 추가.
			pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent];
			dwOutPolyCount++;
			if( dwOutPolyCount >= DECAL_TRI_MAX)
				_asm int 3;

			if( piDet[dwNext] == -1)	// 다음점 '-' 인 경우
			{
				// 잘린점 추가.	
//				v	=	pInPoly[dwNext] - pInPoly[dwCurrent];
				VECTOR3_SUB_VECTOR3( &v, &(pInPoly[dwNext]), &(pInPoly[dwCurrent]));
				t	=	-1.0f * ( pClipPlane->D + DotProduct( &(pClipPlane->v3Up), &(pInPoly[dwCurrent]))) / ( DotProduct(&(pClipPlane->v3Up), &v));
//				pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent]	+	v * t;
				VECTOR3_MUL_FLOAT( &(pOutPoly[dwOutPolyCount]), &v, t);
				VECTOR3_ADDEQU_VECTOR3( &(pOutPoly[dwOutPolyCount]), &(pInPoly[dwCurrent]));
				dwOutPolyCount++;
				if( dwOutPolyCount >= DECAL_TRI_MAX)
					_asm int 3;
			}
		}
		else // piDet[dwCurrent] == -1	// '-'
		{
			if( piDet[dwNext] == 1)			// '+' -> '+' 엣지 통째로 add
			{
				// 잘린점 추가.
//				v	=	pInPoly[dwNext] - pInPoly[dwCurrent];
				VECTOR3_SUB_VECTOR3( &v, &(pInPoly[dwNext]), &(pInPoly[dwCurrent]));
				t	=	-1.0f * ( pClipPlane->D + DotProduct( &(pClipPlane->v3Up), &(pInPoly[dwCurrent]))) / ( DotProduct(&(pClipPlane->v3Up), &v));
//				pOutPoly[dwOutPolyCount]	=	pInPoly[dwCurrent]	+	v * t;
				VECTOR3_MUL_FLOAT( &(pOutPoly[dwOutPolyCount]), &v, t);
				VECTOR3_ADDEQU_VECTOR3( &(pOutPoly[dwOutPolyCount]), &(pInPoly[dwCurrent]));
				dwOutPolyCount++;
				if( dwOutPolyCount >= DECAL_TRI_MAX)
				{
					_asm int 3;
					break;
				}
			}
			else		// 그외엔 아무것도 없다.
			{
				_asm nop;
			}
		}
	}

	return	dwOutPolyCount;
}


BOOL __stdcall CGXDecal::Render()
{
	DWORD	dwRenderClock	=	GetLowClock();

	I4DyuchiGXRenderer*	pRenderer	=	m_pExecutive->GetRenderer();

	pRenderer->RenderMeshObject(m_pRenderMeshObject,0,0,m_dwAlphaFlag,NULL,0,NULL,0,(DWORD)m_MaterialHandle,0,0);
	dwRenderClock	=	GetLowClock()	-	dwRenderClock;
	
	return	TRUE;
}




void CGXDecal::GenTexCoor()
{

	// 텍스쳐 젠 플랙에 따라 구형 텍스쳐 혹은 실린더형 텍스쳐좌표 생성.(예정)
	VECTOR3	Current;
	DWORD	i;
	float	fDist;
	for( i = 0; i < m_dwTriCount*3; i++)
	{
		Current.x	=	g_pTri[i].x;
		Current.y	=	g_pTri[i].y;
		Current.z	=	g_pTri[i].z;
		// u좌표(텍스쳐x는 오른쪽 평면부터의 거리)
		fDist	=	DotProduct( &Current, &(m_pBoxPlane[4].v3Up)) + m_pBoxPlane[4].D;
		g_pTri[i].u1	=	fDist / (m_DecalDesc.v3XYZScale.x*2);
		// v좌표(텍스쳐y는 아래 평면부터의 거리)
		fDist	=	DotProduct( &Current, &(m_pBoxPlane[5].v3Up)) + m_pBoxPlane[5].D;
		g_pTri[i].v1	=	fDist / (m_DecalDesc.v3XYZScale.y*2);
	}

}

void CGXDecal::CreateBoundingVolume()
{
#ifdef	_DEBUG
	if( m_dwTriCount == 0)			// 삼각형이 하나도 없는데 호출됬다.
	{
		_asm int 3;
		return	;
	}
#endif

	// 바운딩 박스.
	MAABB	Box;
	Box.Max.x	=	Box.Min.x	=	g_pTri[0].x;
	Box.Max.y	=	Box.Min.y	=	g_pTri[0].y;
	Box.Max.z	=	Box.Min.z	=	g_pTri[0].z;

	for( DWORD i = 1; i < m_dwTriCount; i++)
	{
		// x
		if( Box.Max.x < g_pTri[i].x)			// x max
			Box.Max.x	=	g_pTri[i].x;
		if( Box.Min.x > g_pTri[i].x)			// x min
			Box.Min.x	=	g_pTri[i].x;
		
		// y
		if( Box.Max.y < g_pTri[i].y)			// y max
			Box.Max.y	=	g_pTri[i].y;
		if( Box.Min.y > g_pTri[i].y)			// y min
			Box.Min.y	=	g_pTri[i].y;

		// z 
		if( Box.Max.z < g_pTri[i].z)			// z max
			Box.Max.z	=	g_pTri[i].z;
		if( Box.Min.z > g_pTri[i].z)			// z min
			Box.Min.z	=	g_pTri[i].z;
	}
	m_Box	=	Box;

	// 바운딩 스피어.
	BOUNDING_SPHERE		bs;
	bs.v3Point	=	(Box.Max + Box.Min) / 2.0f;
	bs.fRs		=	VECTOR3Length( &(Box.Max - bs.v3Point));

	m_BS		=	bs;


	return;
}

BOOL CGXDecal::Initialize( CoExecutive* pExecutive, DECAL_DESC* pDesc,GXDecalSchedulePROC pProc)
{


#ifdef	_DEBUG
	if( pExecutive == 0)
	{
		_asm int 3;
	}
#endif

	memset(this,0,sizeof(CGXDecal));

	CGXMapObject::Cleanup();
	m_dwRenderFlag = 0xfffffff5;
	m_dwAlphaFlag = 0x000000ff;
	m_dwID = 0xfffffff0;

	m_pProc = pProc;
	if (!m_pProc)
		m_pProc = GXDefaultDecalSchedulePROC;


	m_pExecutive	=	pExecutive;
	m_DecalDesc		=	*pDesc;
	m_dwTriCount	=	0;




	// 클리핑용 평면을 만들기 위해 데칼 자체의 X,Y,Z를 구한다.
	CrossProduct( &m_X, &(m_DecalDesc.v3UpDirection), &(m_DecalDesc.v3FaceDirection));
	Normalize( &m_X, &m_X);
	VECTOR3_MULEQU_FLOAT( &m_X, m_DecalDesc.v3XYZScale.x);
	Normalize( &m_Y, &(m_DecalDesc.v3UpDirection));
	VECTOR3_MULEQU_FLOAT( &m_Y, m_DecalDesc.v3XYZScale.y);
	Normalize( &m_Z, &(m_DecalDesc.v3FaceDirection));
	VECTOR3_MULEQU_FLOAT( &m_Z, m_DecalDesc.v3XYZScale.z);

	// 평면 만들기.
	VECTOR3		PlaneTri[3];
	// 첫번째 평면.	앞.
	PlaneTri[0]		=	pDesc->v3Position - m_X + m_Y + m_Z;
	PlaneTri[1]		=	pDesc->v3Position + m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position + m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[0]), PlaneTri);
	// 두번째 평면.	뒤.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y - m_Z;			// 나중에 평면 뒤로 가는거 추가.
	PlaneTri[1]		=	pDesc->v3Position - m_X + m_Y - m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y - m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[1]), PlaneTri);
	// 세번째 평면.	왼쪽.
	PlaneTri[0]		=	pDesc->v3Position - m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position - m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y;
	CalcPlaneEquation( &(m_pBoxPlane[2]), PlaneTri);
	// 네번째 평면.	위.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position + m_X + m_Y + m_Z;
	PlaneTri[2]		=	pDesc->v3Position - m_X + m_Y;
	CalcPlaneEquation( &(m_pBoxPlane[3]), PlaneTri);
	// 다섯번째 평면.	오른쪽.
	PlaneTri[0]		=	pDesc->v3Position + m_X + m_Y;
	PlaneTri[1]		=	pDesc->v3Position + m_X - m_Y;
	PlaneTri[2]		=	pDesc->v3Position + m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[4]), PlaneTri);
	// 여섯번째 평면.	아래.
	PlaneTri[0]		=	pDesc->v3Position + m_X - m_Y;
	PlaneTri[1]		=	pDesc->v3Position - m_X - m_Y;
	PlaneTri[2]		=	pDesc->v3Position - m_X - m_Y + m_Z;
	CalcPlaneEquation( &(m_pBoxPlane[5]), PlaneTri);

	return	TRUE;
}

DWORD CGXDecal::AddTri( VECTOR3* pTri)
{
#ifdef	_DEBUG
	DWORD	dwClipTriangleClock;
	CheckVectorArray( pTri, 3);
#endif
	// 여기 나오는 삼각형들은 트리에서 걸러진놈들이므로 거의 그대로 믿으면 될듯 하다.

	// 볼륨을 이루는 평면들이 나왔으니 삼각형을 자른다.
#ifdef	_DEBUG
	dwClipTriangleClock	=	GetLowClock();
#endif
	DWORD	dwClippedTriCount	=	ClipOneTri( pTri);
#ifdef	_DEBUG
	dwClipTriangleClock	=	GetLowClock()	-	dwClipTriangleClock;
#endif
	// 삼각형 다 잘랐다.
	return	dwClippedTriCount;
}

DWORD CGXDecal::Build()
{
	// 삼각형 다 잘른 상황에서 삼각형이 하나도 없는 경우 기본 도형을 만든다.
	if( m_dwTriCount != 0)
		goto	lbReturnTrue;

	VECTOR3		pTemp[6];
	pTemp[0]		=	m_DecalDesc.v3Position	+	m_X	+	m_Y;
	pTemp[1]		=	m_DecalDesc.v3Position	-	m_X	+	m_Y;
	pTemp[2]		=	m_DecalDesc.v3Position	-	m_X	-	m_Y;
	
	pTemp[3]		=	m_DecalDesc.v3Position	+	m_X	+	m_Y;
	pTemp[4]		=	m_DecalDesc.v3Position	-	m_X	-	m_Y;
	pTemp[5]		=	m_DecalDesc.v3Position	+	m_X	-	m_Y;

	DWORD	i;
	for( i = 0; i < 2*3; i++)
	{
		g_pTri[i].x		=	pTemp[i].x;
		g_pTri[i].y		=	pTemp[i].y;
		g_pTri[i].z		=	pTemp[i].z;
	}
	m_dwTriCount	=	2;

lbReturnTrue:
	CreateBoundingVolume();
	GenTexCoor();
	CreateMaterial( m_DecalDesc.szMaterialName);

	m_pRenderMeshObject = m_pExecutive->GetRenderer()->CreateImmMeshObject(g_pTri,m_dwTriCount,m_MaterialHandle,0);
	if (!m_pRenderMeshObject)
	{
		m_dwTriCount = 0;
		goto lb_return;
	}

	m_pRenderMeshObject->QueryInterface(IID_IMM_MESH_OBJECT,(void**)&m_pImmMeshObject);


lb_return:
	return	m_dwTriCount;
}

BOOL CGXDecal::ChangeMaterial( char* szFileName)
{
	DeleteMaterial();
	CreateMaterial( szFileName);
	
	return	FALSE;
}

void CGXDecal::CreateMaterial( char* szFileName)
{
	if( m_MaterialHandle)
	{
		_asm int 3;
		DeleteMaterial();
	}
	// 메터리얼.
	MATERIAL	Material;
	memset( &Material, 0, sizeof(Material));
	Material.dwDiffuse	=	0xff444444;
	Material.dwAmbient	=	0xff444444;
	Material.fTransparency = 1.0f;



	lstrcpy( Material.szDiffuseTexmapFileName, szFileName);
	GetNameRemovePath(Material.szDiffuseTexmapFileName,Material.szDiffuseTexmapFileName);
	
	CharToSmallASCIILen(Material.szDiffuseTexmapFileName,Material.szDiffuseTexmapFileName);
	



	void* MaterialHandle	=	m_pExecutive->GetRenderer()->CreateMaterial( &Material,NULL,NULL, 0);
	if( MaterialHandle == 0)
		_asm int 3;
	m_MaterialHandle	=	MaterialHandle;
}

void CGXDecal::DeleteMaterial()
{
	if( m_MaterialHandle)
		m_pExecutive->GetRenderer()->DeleteMaterial( m_MaterialHandle);
	m_MaterialHandle	=	0;
}

void CGXDecal::Delete()
{
	CGXMap*	pMap = m_pExecutive->GetGXMap();

	pMap->DeleteDecal(m_dwClipperIndex);
	if (m_pImmMeshObject)
	{
		m_pImmMeshObject->Release();
		m_pImmMeshObject = NULL;
	}
	if (m_pRenderMeshObject)
	{
		m_pRenderMeshObject->Release();
		m_pRenderMeshObject = NULL;
	}

	DeleteMaterial();
}

COLLISION_MESH_OBJECT_DESC*	__stdcall	CGXDecal::GetCollisionMeshObjectDesc()
{
	return NULL;
}
DWORD __stdcall	CGXDecal::OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2)
{
	return m_pProc(pExecutive,this,msg,arg1,arg2,m_pData);

}
DWORD __stdcall	CGXDecal::GetObjectIndexInModel()
{
	return 0;
}

BOOL __stdcall CGXDecal::IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	*pdwModelIndex = 0xffffffff;
	*pdwObjIndex = 0xffffffff;
	return FALSE;
}
DWORD __stdcall	CGXDecal::CreateIVertexList(IVERTEX** ppVertex)
{
	return 0;
}
void __stdcall CGXDecal::ReleaseIVertexList(IVERTEX*	pVertex)
{
}
BOOL __stdcall CGXDecal::ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc)
{
	return FALSE;
}
