// MClipper.cpp: implementation of the MClipper class.
//
//////////////////////////////////////////////////////////////////////

#include "MClipper.h"

#include "CheckClock.h"
#include "myself97common.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGXExecutive/GXObject.h"
#include "../4DyuchiGXExecutive/executive.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MClipper::MClipper( )
{
	memset( this, 0, sizeof(MClipper));

}

MClipper::~MClipper()
{
//	DeleteAll();
}


/****************************************************************************
	Init
	static object의 경우만 buildTree에서 하고 나머지 초기화의 대부분을 한다.
****************************************************************************/
DWORD MClipper::Init( CoExecutive* pExecutive, DWORD dwMaxDynamicObjectCount )
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::Init( dwMaxDynamicObjectCount = %d)\n", dwMaxDynamicObjectCount);
#endif
	mdwTempStaticObjectCount	=	0;
	BOOL	bResult;
	m_pExecutive	=	pExecutive;

	md.ICHandle	=	ICCreate();
	ICInitialize( md.ICHandle, dwMaxDynamicObjectCount);

	md.dwMaxCount					=	dwMaxDynamicObjectCount;
	md.dwCount						=	0;
	md.ppObj						=	new CGXMapObject*[md.dwMaxCount];
	md.pdwType						=	new	DWORD[md.dwMaxCount];
	md.pAABB						=	new MAABB[md.dwMaxCount];
	md.pBS							=	new BOUNDING_SPHERE[md.dwMaxCount];
	memset( md.ppObj, 0, sizeof(CGXMapObject*)*md.dwMaxCount);
	memset( md.pdwType, 0, sizeof(DWORD)*md.dwMaxCount);
	memset( md.pAABB, 0, sizeof(MAABB)*md.dwMaxCount);
	memset( md.pBS, 0, sizeof(BOUNDING_SPHERE)*md.dwMaxCount);

	mDObjCandidatePool	=	CreateStaticMemoryPool();
	bResult	=	InitializeStaticMemoryPool( mDObjCandidatePool, sizeof(MClipperObjLink)  , 100, md.dwMaxCount + 2);		// +2는 더미 링크.
	if( bResult	== FALSE)
	{
		_asm int 3;
	}

	// static한 임시 버퍼들 만들기.
	ms.ppObj				= new CGXMapObject*[50000];	// 스테이틱 오브젝트가 저장될 버퍼.
	ms.pAABB				=	0;									// 스테이틱 오브젝트의 aabb 버퍼.
	ms.dwCount				=	50000;				// 스테이틱 오브젝특의 갯수. 임시용 최대갯수.


	mdwMaxFindNodeDepth = 5;			// 노드를 탐색하는 중 이 이상은 안내려간다.
	mdwMinFindNodeObjectCount= 4;	// 노드를 탐색하는 중 노드의 오브젝트가 이만큼 이하로 있으면 안내려간다.

	mdwMaxRenderNodeDepth = 4;		// 랜더링때 노드를 탐색하는 중 이 이상은 안내려간다.

	mDynamicNodePool	=	CreateStaticMemoryPool();

	return 1;
}

/****************************************************************************
	AddStaticObject
	트리를 만들기 위해 static 오브젝트(map 오브젝트)를 넣는다.
	for를 써서 한번에 한개씩.
****************************************************************************/
DWORD MClipper::AddStaticObject(CGXMapObject *pObject)
{

	if( ms.dwCount <= mdwTempStaticObjectCount)	// 정해진 수치보다 오브젝트가 더 많이들어오는 것때문에 iTemp거시기를 쓴다.
	{
		_asm int 3
		return 0xffffffff;
	}
	// 버퍼를 채운다.
	ms.ppObj[mdwTempStaticObjectCount] = pObject;
	mdwTempStaticObjectCount++;

	return mdwTempStaticObjectCount-1;		// 인덱스는 0베이스.
}

/****************************************************************************
	BuildTree
	AddStaticObject로 받은 오브젝트를 이용해서 트리를 build 한다.
	이때 트리가 완전히 만들어지고, 고정된다.
	아직까지 iMode는 안쓴다.
****************************************************************************/
DWORD MClipper::BuildTree(DWORD dwMode, MAABB WorldBox, DWORD dwMaxLevel)
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::BuildTree(dwMode = %d, dwMaxLevel = %d)\n", dwMode, dwMaxLevel);
#endif

#ifdef	_DEBUG
	if( mpNode)
	{
		// 트리가 지워지지 않고 두번째 만들어지고 있다.
		PrintfDebugString("MClipper::BuildTree() Line = %d 트리가 지워지지 않고 두번째 만들어지고 있다.", __LINE__);
		_asm int 3;
		DeleteAll();
	}
#endif

	DWORD		dwBuildTick	=	GetTickCount();

	mdwMode	=	dwMode;

	if( dwMaxLevel == 0)
		dwMaxLevel	=	1;

	BOOL		bResult;

	mWorldBox				=	WorldBox;
	mdwMaxLevel				=	dwMaxLevel;

	DWORD i = 0, j = 0;

	// miStaticObjectCount, ppStaticObject.
	CGXMapObject**	pTemp = new CGXMapObject*[mdwTempStaticObjectCount];
	memcpy( pTemp, ms.ppObj, sizeof( CGXMapObject*) * mdwTempStaticObjectCount);
	delete[]	ms.ppObj;
	ms.ppObj			=	pTemp;
	ms.dwCount			=	mdwTempStaticObjectCount;

	ms.pdwType		=	new	DWORD[ms.dwCount];

	// 월드 박스 다시 만들기.
	COLLISION_MESH_OBJECT_DESC*	pTempDesc	=	ms.ppObj[0]->GetCollisionMeshObjectDesc();
	MAABB TempBox;
	TempBox.Max = TempBox.Min = pTempDesc->boundingBox.v3Oct[0];

	// 볼륨.
	ms.pAABB		=	new		MAABB[ms.dwCount];
	ms.pBS			=	new		BOUNDING_SPHERE[ms.dwCount];
	ms.pBB			=	new		BOUNDING_BOX[ms.dwCount];
	for( i = 0; i < ms.dwCount; i++)
	{
		ms.pdwType[i]	=	ms.ppObj[i]->GetObjectType();

		COLLISION_MESH_OBJECT_DESC*	pDesc;
		pDesc = ms.ppObj[i]->GetCollisionMeshObjectDesc();

		// BB
		ms.pBB[i]		=	pDesc->boundingBox;

		// AABB
		MAABB Box;
		Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
		for( j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// 최대.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// 최소.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;

			// 최대.
			if( TempBox.Max.x < vec3.x) TempBox.Max.x = vec3.x;
			if( TempBox.Max.y < vec3.y) TempBox.Max.y = vec3.y;
			if( TempBox.Max.z < vec3.z) TempBox.Max.z = vec3.z;
			// 최소.x
			if( TempBox.Min.x > vec3.x) TempBox.Min.x = vec3.x;
			if( TempBox.Min.y > vec3.y) TempBox.Min.y = vec3.y;
			if( TempBox.Min.z > vec3.z) TempBox.Min.z = vec3.z;
		}
		ms.pAABB[i] = Box;

		// 스펴.
		ms.pBS[i]		=	GetSphereFromAABB(Box);
	}

	// 스테이틱 오브젝트로 만든 TempBox와 월드박스를 비교, 큰쪽을 월드박스로 만든다.
	bResult	=	TestWorldBox( TempBox);
	if( bResult == FALSE)
	{
//		_asm int 3;
	}

	if( mWorldBox.Max.x == mWorldBox.Min.x)
		mWorldBox.Max.x	=	mWorldBox.Min.x + 10.0f;
	if( mWorldBox.Max.y == mWorldBox.Min.y)
		mWorldBox.Max.y	=	mWorldBox.Min.y + 10.0f;
	if( mWorldBox.Max.z == mWorldBox.Min.z)
		mWorldBox.Max.z	=	mWorldBox.Min.z + 10.0f;

	PrintfDebugString("MClipper::BuildTree() WorldBox.Max = ( %f, %f, %f), Min = ( %f, %f, %f)\n", mWorldBox.Max.x, mWorldBox.Max.y, mWorldBox.Max.z, mWorldBox.Min.x, mWorldBox.Min.y, mWorldBox.Min.z);
	


	
	// 트리가 가질 노드의 갯수. 풀 트리이므로 뻔하다.
	mdwNodeCount		=	(int(pow(4.0f,(int)( dwMaxLevel+1)) - 1) / (4 - 1));

	// 노드가 가질 dobj 링크 풀, 최대 갯수를 예측 했지만 이게 틀릴지 맞을지 모르겠다.
	mdwMaxDynamicNodePool		=	((dwMaxLevel+1) * 4 * md.dwMaxCount) + 	mdwNodeCount*2;
	bResult	=	InitializeStaticMemoryPool( mDynamicNodePool, sizeof(MClipperObjLink)  , 1000, mdwMaxDynamicNodePool);
	if( bResult	== FALSE)
	{
//		_asm int 3;
	}

	// 인자 트리를 만들면 되겠지.
	mdwNodeUsedCount	=	0;
	this->mpNode	=	new	MClipperNode[mdwNodeCount];
	memset( mpNode, 0, sizeof(MClipperNode)*mdwNodeCount);
	CreateNodeReculsive( 0xffffffff, 0);


	this->CreateCandidateBuffer( md.dwMaxCount, ms.dwCount);
	
	// 임시버퍼.
	mpTempStaticIndexBuffer	=	new BYTE[ms.dwCount];



	dwBuildTick	=	GetTickCount()	-	dwBuildTick;
#ifdef	_DEBUG
	PrintfDebugString("MClipper::BuildTree Time = %d Tick\n", dwBuildTick);
#endif
	
	return 1;
}

/****************************************************************************
	AddDynamicObject
	맵을 구성하는 오브젝트 이외의 것을 add한다. 
****************************************************************************/
DWORD MClipper::AddDynamicObject(CGXMapObject *pObject)
{
	
#ifdef	_DEBUG
	DWORD	dwTemp;
	// 혹시 같은 오브젝트가 두번째 들어오지 않았나.?
	for( dwTemp = 0; dwTemp < md.dwMaxCount; dwTemp++)
	{
		if( md.ppObj[dwTemp] == pObject)
		{
			PrintfDebugString("%s, %d 현재 들어온 오브젝느는 이미 클리퍼에 소속 되어 있다.", __FILE__, __LINE__);
			_asm int 3;
		}
	}
#endif

	// 빈 인덱스 하나 얻어서,
	DWORD	dwIndex	=	ICAllocIndex( md.ICHandle) - 1;
	if( dwIndex == 0xffffffff)
	{
		_asm int 3;			// 다이나믹 오브젝트의 갯수 초과.
		return	0xffffffff;
	}

	// 인덱스의 내용을 채우고,
	md.ppObj[dwIndex]	=	pObject;
	md.pdwType[dwIndex]		=	pObject->GetObjectType();

	// aabb 바운딩박스.
	COLLISION_MESH_OBJECT_DESC*	pDesc;
	pDesc = pObject->GetCollisionMeshObjectDesc();

	MAABB Box;
	Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
	if( pObject->GetObjectType() == GX_MAP_OBJECT_TYPE_LIGHT)
	{
		Box.Max.x	=	pDesc->boundingSphere.v3Point.x	+	pDesc->boundingSphere.fRs;
		Box.Max.y	=	pDesc->boundingSphere.v3Point.y	+	pDesc->boundingSphere.fRs;
		Box.Max.z	=	pDesc->boundingSphere.v3Point.z	+	pDesc->boundingSphere.fRs;
		Box.Min.x	=	pDesc->boundingSphere.v3Point.x	-	pDesc->boundingSphere.fRs;
		Box.Min.y	=	pDesc->boundingSphere.v3Point.y	-	pDesc->boundingSphere.fRs;
		Box.Min.z	=	pDesc->boundingSphere.v3Point.z	-	pDesc->boundingSphere.fRs;
	}
	else
	{
		for( int j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// 최대.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// 최소.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;
		}
	}

	md.pAABB[dwIndex] = Box;

	// 바운딩 스피어.
	md.pBS[dwIndex] = pDesc->boundingSphere;

	// 트리에 인서트.
	if( mpNode != 0)
	{
		AddDynamicObjectToTree( dwIndex);
	}

	md.dwCount++;
	/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Add-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif*/
	return dwIndex;
}

/****************************************************************************
	UpdateDynamicObject
	dynamicobject의 상태를 갱신한다, 위치같은것일 듯 한데, 충돌처리등에선 좀 더 생각해봐야한다.
****************************************************************************/
BOOL MClipper::UpdateDynamicObject(DWORD dwIndex)
{

	DWORD	dwDeleteClock;
	DWORD	dwAddClock;
	DWORD	dwUpdateClock;
#ifdef	_DEBUG
	// 그 인덱스가 사용되는가.?
	if( md.ppObj[dwIndex] == 0)	// 사용되고 있지 않으면.
	{
		OutputDebugString( "MClipper::UpdateDynamicObject()에 유효하지 않은 인덱스가 들어왔다.");
		_asm int 3;
		return FALSE;

	}
	if( dwIndex > md.dwMaxCount)
	{
		OutputDebugString( "MClipper::UpdateDynamicObject()최대값을 넘어선 인덱스가 들어왔다.");
		_asm int 3;
		return	FALSE;
	}
#endif
	
	dwDeleteClock	=	GetLowClock();
	// 트리에서 지운다.
	if( mpNode)
	{
		DeleteDynamicObjectFromTree( dwIndex);
	}
	dwDeleteClock	=	GetLowClock()	-	dwDeleteClock;

	dwUpdateClock	=	GetLowClock();
	COLLISION_MESH_OBJECT_DESC*	pDesc;
	pDesc = md.ppObj[dwIndex]->GetCollisionMeshObjectDesc();			// 클럭이 너무 높다.?
	MAABB Box;
	Box.Max = Box.Min = pDesc->boundingBox.v3Oct[0];
	if( md.ppObj[dwIndex]->GetObjectType() == GX_MAP_OBJECT_TYPE_LIGHT)
	{
		Box.Max.x	=	pDesc->boundingSphere.v3Point.x	+	pDesc->boundingSphere.fRs;
		Box.Max.y	=	pDesc->boundingSphere.v3Point.y	+	pDesc->boundingSphere.fRs;
		Box.Max.z	=	pDesc->boundingSphere.v3Point.z	+	pDesc->boundingSphere.fRs;
		Box.Min.x	=	pDesc->boundingSphere.v3Point.x	-	pDesc->boundingSphere.fRs;
		Box.Min.y	=	pDesc->boundingSphere.v3Point.y	-	pDesc->boundingSphere.fRs;
		Box.Min.z	=	pDesc->boundingSphere.v3Point.z	-	pDesc->boundingSphere.fRs;
	}
	else
	{
		for( int j = 1; j < 8; j++)
		{
			VECTOR3	vec3	=	pDesc->boundingBox.v3Oct[j];
			// 최대.
			if( Box.Max.x < vec3.x) Box.Max.x = vec3.x;
			if( Box.Max.y < vec3.y) Box.Max.y = vec3.y;
			if( Box.Max.z < vec3.z) Box.Max.z = vec3.z;
			// 최소.x
			if( Box.Min.x > vec3.x) Box.Min.x = vec3.x;
			if( Box.Min.y > vec3.y) Box.Min.y = vec3.y;
			if( Box.Min.z > vec3.z) Box.Min.z = vec3.z;
		}
	}
	md.pAABB[dwIndex] = Box;
	// 바운딩 스피어.
	md.pBS[dwIndex] = pDesc->boundingSphere;

	dwUpdateClock	=	GetLowClock()	-	dwUpdateClock;

	dwAddClock		=	GetLowClock();
	// 트리 갱신.
	if( mpNode)
	{
		AddDynamicObjectToTree(dwIndex);
	}
	dwAddClock		=	GetLowClock()	-	dwAddClock;
/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Update-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif*/
	return TRUE;

}

/****************************************************************************
	DeleteDynamicObject
	Add했던 오브젝트를 삭제한다.
****************************************************************************/
BOOL MClipper::DeleteDynamicObject(DWORD dwIndex)
{

	// 실제로 그 인덱스가 사용되는가.?
	if( md.ppObj[dwIndex] == NULL)		// 사용되고 있지 않으면.
	{
		_asm int 3;
#ifdef	_DEBUG
	PrintfDebugString("MClipper::DeleteDynamicObject(dwIndex = %d) Line = %d  그 인덱스는 사용되고 있지 않다. 외 _asm int 3을 주석처리 했는가.?\n", dwIndex, __LINE__);
#endif
		return FALSE;
	}

	// 트리에서 그 오브젝트를 지우고,
	if( mpNode )
	{
		DeleteDynamicObjectFromTree(dwIndex);
	}

	// 지워지기 전에 해당 gxmapobject의 클리퍼인덱스를 0xffffffff로 만든다.
	// 이는 gxmap이 지워져서 클리퍼가 없어졌다가 gxmap이 다시 생성되어 클리퍼 역시 다시 생성 된 상태에서,
	// 바깥에서 지워지지 않은 gxmapobject가 updatetogxmap을 호출하지 않은 상태에서 클리퍼를 다루지 못하게 방어 하기 위함이다.
	// 같은 내용의 코드가 DeleteDynamicObject()와 DeleteDecal()에 존재한다.
	md.ppObj[dwIndex]->SetClipperIndex( 0xffffffff);

	// 인덱스의 내용을 지우고,
	md.ppObj[dwIndex]	=	NULL;
	memset( &(md.pAABB[dwIndex]), 0, sizeof( MAABB));
	// 인덱스 반환.
	ICFreeIndex( md.ICHandle, dwIndex+1);
	md.dwCount--;

#ifdef	_DEBUG
	if( md.dwCount == 0xffffffff)
	{
		_asm int 3;		// 부호가 - 아래로 내려간게 되는가.?
	}
#endif
/*
#ifdef _DEBUG
	char	txt[128];
	wsprintf(txt,"Delete-node_count:%d,node_used_count:%d\n",mdwNodeCount,mdwNodeUsedCount);
	OutputDebugString(txt);
#endif
	*/
	return TRUE;
}

/****************************************************************************
	FindRenderCandidate
	vv로 주어진 볼륨을 가지고 랜더링 후보를 찾아낸다.
	disable 시키면 자기가 가진 버퍼를 그대로 내어놓는다.
****************************************************************************/
void MClipper::FindRenderCandidate(VIEW_VOLUME &vv)
{
	if( mpNode == 0)
	{
		return ;
	}

	BOOL	bIn		=	FALSE;

	DWORD	dwObjType;
	mdwDBG_TraveledNodeCount = 0;
	unsigned int iLowClock = 0;

	DWORD i = 0;

		memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);		// 임시 인덱스 버퍼. static

		memset( mRCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);
		memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

		// dobj 링크 버퍼.
		this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
		mdwDObjCandidateCount	=	0;
		mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

		iLowClock = GetLowClock();
		FindRenderCandidateReculsive( 0, vv);		// 재귀. 찾기.
		iLowClock = GetLowClock() - iLowClock;

		iLowClock = GetLowClock();
		// static object 버퍼 정리.
		for( i = 0; i < ms.dwCount; i++)
		{
			if( mpTempStaticIndexBuffer[i] != 0)
			{
				dwObjType		=	ms.pdwType[i];//mppStaticObject[i]->GetObjectType();

				// 뷰볼륨 클립.
//				bIn	=	ClipSphereWithViewVolume( &vv, &(ms.pBS[i]));
				bIn = ClipRenderObjectWithViewVolume(&vv,ms.ppObj[i]->GetCollisionMeshObjectDesc(),0);
				if( bIn)
				{
					mRCBuff.pppBuff[dwObjType][mRCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
					mRCBuff.pdwCount[dwObjType]++;
				}
			}
		}
		iLowClock = GetLowClock() - iLowClock;

		iLowClock = GetLowClock();
		// dynamic object 버퍼 정리.
		MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;

		DWORD dwIndex;
		while(1)
		{
			dwIndex		=	pLink->iIndex;
			if( dwIndex	==	INT_MAX)
				break;

			// 뷰볼륨 클립.
//			bIn	=	ClipSphereWithViewVolume( &vv, &(md.pBS[dwIndex]));
			bIn = ClipRenderObjectWithViewVolume(&vv,md.ppObj[dwIndex]->GetCollisionMeshObjectDesc(),0);
			if( bIn)
			{
				dwObjType		=	md.pdwType[dwIndex];//md.ppObj[iIndex]->GetObjectType();
				mRCBuff.pppBuff[dwObjType][mRCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
				mRCBuff.pdwCount[dwObjType]++;
			}

			pLink	=	pLink->pNext;
			


		}

		iLowClock = GetLowClock() - iLowClock;


	return ;
}

/****************************************************************************
****************************************************************************/
void MClipper::DeleteAll()
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::DeleteAll()\n");
#endif

	DWORD	dwClock;
	DWORD	i;

	// static object
	{
		if( ms.ppObj)				delete[]	ms.ppObj;			// 스테이틱 오브젝트가 저장될 버퍼.
		if( ms.pdwType)				delete[]	ms.pdwType;
		if( ms.pAABB)				delete[]	ms.pAABB;			// 스테이틱 오브젝트의 aabb 버퍼
		if( ms.pBS)					delete[]	ms.pBS;
		if( ms.pBB)					delete[]	ms.pBB;
	}

	DWORD	dwRemainedObject	=	0;

	// dynamic object
	{
#ifdef	_DEBUG
	PrintfDebugString("남아있는 Dynamic Object 갯수 = %d\n", md.dwCount);
	PrintfDebugString("남아있는 Dynamic Object 리스트\n");
#endif
		for( i = 0; i < md.dwMaxCount; i++)
		{
			if( md.ppObj[i])
			{
				dwRemainedObject++;
#ifdef	_DEBUG
	
	DWORD	dwHandleType	=	m_pExecutive->IsValidHandle( md.ppObj[i]);
	if( dwHandleType == GX_MAP_OBJECT_TYPE_INVALID)
	{
		PrintfDebugString("%d인덱스가 가리키는 핸들은 유효한 핸들이 아니다. 혹시 UpdateToGXMap이 두번 들어가지 않았나.?\n", i);
		_asm int 3;
		goto	lbHere;
	}

	switch( md.pdwType[i])
	{
	case	GX_MAP_OBJECT_TYPE_OBJECT:
		PrintfDebugString("%s\n", ((CoGXObject*)md.ppObj[i])->m_pModelFileItem->szFileName);
		break;
	case	GX_MAP_OBJECT_TYPE_LIGHT:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_LIGHT\n");
		break;
	case	GX_MAP_OBJECT_TYPE_TRIGGER:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_TRIGGER\n");
		break;
	case	GX_MAP_OBJECT_TYPE_DECAL:
		PrintfDebugString("GX_MAP_OBJECT_TYPE_DECAL\n");
		break;
	default:
		PrintfDebugString("default??\n");
	}

lbHere:
#endif
				md.ppObj[i]->SetClipperIndex( 0xffffffff);
			}
		}
#ifdef	_DEBUG
	PrintfDebugString("실제 지운 오브젝트 합계 = %d\n", dwRemainedObject);
	PrintfDebugString("끝.\n");
#endif

		if( md.ppObj)		delete[]	md.ppObj;			// 다이나믹 오브젝트가 저장될 버퍼.
		if( md.pdwType)			delete[]	md.pdwType;
		if(	md.pAABB)	delete[]	md.pAABB;		// 다이나믹 오브젝트의 aabb 버퍼.
		if( md.pBS)		delete[]	md.pBS;			// 다이나믹 오브젝트의 중점 버퍼.
	}

	// 트리 지우기.
	dwClock		=	GetLowClock();
	DeleteNodes( );
	dwClock		=	GetLowClock()	-	dwClock;

	if( mpTempStaticIndexBuffer)		delete[]	mpTempStaticIndexBuffer;

	// dobj 랜더 후보.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mpDObjCandidateLink	=	0;
	if( mDObjCandidatePool)
		ReleaseStaticMemoryPool(mDObjCandidatePool);
	mDObjCandidatePool	=	0;


	// 찾기함수 버퍼 지우기.
	DeleteCandidateBuffer();
//	miStaticCandidateCount		=	0;
//	if( mppStaticCandidateBuffer)		delete[]	mppStaticCandidateBuffer;
//	miDynamicCandidateCount	=	0;
//	if( mppDynamicCandidateBuffer)		delete[]	mppDynamicCandidateBuffer;

	if (mDynamicNodePool)
		ReleaseStaticMemoryPool( mDynamicNodePool);
	mDynamicNodePool	=	0;

//	md.IndexManager.Delete();
	ICRelease( md.ICHandle);

	memset(this,0,sizeof(MClipper));
}

/****************************************************************************
	private:	CreateNodeReculsive
	부모, 자식번호를 받아 노드를 하나 만들어 내놓는다.
	재귀.
****************************************************************************/
DWORD MClipper::CreateNodeReculsive(DWORD dwParent, DWORD dwChildNum)
{
	DWORD*	pTempIndex = NULL;
	MClipperNode*		pNode	=	0;
	DWORD				dwNode	=	0;
	DWORD i = 0;

	if( dwParent == 0xffffffff)	// top노드.
	{
		pNode	=	&(mpNode[0]);
		dwNode	=	0;

		pNode->dwParent		=	0xffffffff;							// 부모노드.
		pNode->dwChildNum	=	0;
		pNode->dwLevel		=	0;								// 이 노드의 층.
		pNode->Box			=	mWorldBox;					// 이 노드의 바운딩 박스.

		// static obj.
		pNode->dwStaticObjectCount	=	ms.dwCount;			// static object의 갯수.
		pNode->pStaticObjectIndex	=	new DWORD[ms.dwCount];	// static object의 인덱스 버퍼.
		for( i = 0; i < pNode->dwStaticObjectCount; i++)
		{
			pNode->pStaticObjectIndex[i] = i;
		}

		goto	lbCommonCode;

	}
	else		// 일반 노드.
	{
		// 일단 노드를 만든다. 다이나믹은 리프기반으로 돌아간다. 자식노드를 네개를 꽉 채워서.....
		pNode	=	&(mpNode[mdwNodeUsedCount]);
		dwNode	=	mdwNodeUsedCount;
		if( mdwNodeUsedCount == mdwNodeCount)
			_asm int 3;
		// 일단 자기 노드 채우고,
		pNode->dwParent = dwParent;
		pNode->dwLevel	=	mpNode[dwParent].dwLevel	+	1;
		pNode->dwChildNum	=	dwChildNum;
		pNode->Box		=	GetChildAABB( dwParent, dwChildNum);

		// 부모의 static object를 돌면서 자기한테 속한것을 끄집어낸다.
		pTempIndex = new DWORD[mpNode[pNode->dwParent].dwStaticObjectCount];
		pNode->dwStaticObjectCount	=	0;
		for( i = 0; i < mpNode[pNode->dwParent].dwStaticObjectCount; i++)
		{
			DWORD	dwObjIndex	=	mpNode[pNode->dwParent].pStaticObjectIndex[i];
			BOOL	bDidMeet	=	IsAABBMeetAABB( ms.pAABB[dwObjIndex], pNode->Box);
			if( bDidMeet)	// pTemp노드에 속하는 오브젝트가 있으면, 
			{
				pTempIndex[pNode->dwStaticObjectCount] = dwObjIndex;
				pNode->dwStaticObjectCount++;
			}
		}
		// 버퍼 정리.
		pNode->pStaticObjectIndex = new DWORD[pNode->dwStaticObjectCount];
		memcpy( pNode->pStaticObjectIndex, pTempIndex, sizeof(DWORD)*pNode->dwStaticObjectCount);
		delete[] pTempIndex;
	}
	
lbCommonCode:		// 루트와 자식 공용 코드.
	mdwNodeUsedCount++;		// 버퍼에서 사용된 노드의 갯수. 노드를 할당할때 이 값을 참고한다.

	pNode->Sphere	=	GetSphereFromAABB( pNode->Box);	

	// dynamic object를 위한 더미 링크.
	pNode->pDLink	=	CreateDummyLink(mDynamicNodePool);

	// 최대 레벨인가.?
	if(	mdwMaxLevel <= pNode->dwLevel)
	{
		return dwNode;
	}


	// 아니면 자식노드를 생성.
	for( i = 0; i < 4; i++)
	{
		pNode->pdwChild[i] = CreateNodeReculsive( dwNode, i);
	}
	return dwNode;
}

/****************************************************************************
	private:	GetChildAABB
	주어진 노드의 주어진 자식의 AABB를 만든다.
****************************************************************************/
MAABB MClipper::GetChildAABB(DWORD dwParent, DWORD dwChildNum)
{
	MAABB&		PBox = mpNode[dwParent].Box;
	VECTOR3		Middle;
	Middle.x = (PBox.Max.x + PBox.Min.x) / 2;
	Middle.z = (PBox.Max.z + PBox.Min.z) / 2;

	MAABB Box;	// 자식 박스.
	Box.Max.y	=	PBox.Max.y;
	Box.Min.y	=	PBox.Min.y;

	switch( dwChildNum)
	{
		case 0:		// 서북.
			Box.Max.x	=	Middle.x;
			Box.Min.x	=	PBox.Min.x;
			Box.Max.z	=	PBox.Max.z;
			Box.Min.z	=	Middle.z;
			break;
		case 1:		// 동북.
			Box.Max.x	=	PBox.Max.x;
			Box.Min.x	=	Middle.x;
			Box.Max.z	=	PBox.Max.z;
			Box.Min.z	=	Middle.z;
			break;
		case 2:		// 서남.
			Box.Max.x	=	Middle.x;
			Box.Min.x	=	PBox.Min.x;
			Box.Max.z	=	Middle.z;
			Box.Min.z	=	PBox.Min.z;
			break;
		case 3:		// 동남.
			Box.Max.x	=	PBox.Max.x;
			Box.Min.x	=	Middle.x;
			Box.Max.z	=	Middle.z;
			Box.Min.z	=	PBox.Min.z;
			break;
		default:	// 뭐지.?
			_asm int 3;
			break;
	}
	return Box;
}


/****************************************************************************
	private:	FindRenderCandidateReculsive
	FindRenderCandidate에서 호출되는 트리를 타는 재귀함수.
	다이나믹, 스테이틱 같이 찾는다.
****************************************************************************/
void MClipper::FindRenderCandidateReculsive(DWORD dwNode, VIEW_VOLUME &vv)
{
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;
	int iResult = 0;
	DWORD	i = 0, j = 0;

	// 현재의 노드가 vv에 들어가나.?

	DWORD	dwTemp	=	GetLowClock();
//	iResult =	WhereIsAABBFromViewVolume( pCurrentNode->Box, vv);
	iResult	=	WhereIsSphereFromViewVolume( pCurrentNode->Sphere, vv);
	dwTemp	=	GetLowClock() - dwTemp;
	// 완전히 안들어가면 리턴.
	if( iResult < 0)
	{
		goto Return;
	}

	// 완전히 들어가면 현재노드의 오브젝트를 임시 인덱스에 기록하고 리턴.
	// 완전히 들어가거나, 현재 노드가 uiFindMaxDepth이거나 보다 클경우 여기까지.
	// 현재 노드가 가진 static object 갯수가 대충 uiMinFindObjectCount 일경우 여기까지.
	if( pCurrentNode->dwLevel	==	mdwMaxLevel || iResult > 0 || pCurrentNode->dwLevel >= mdwMaxRenderNodeDepth)
	{
		// static object
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}


		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}


		goto Return;
	}
	// 자식이 있는 경우.
	else			// 재귀를 goto로 바꾼것의 핵심이 여기 있다. 그냥 pCurrent를 잘 바꿔준것.
	{
//		pCurrentNode = pCurrentNode->ppChild[0];
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[1];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[1])]);
		goto Start;
Child1Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[2];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[2])]);
		goto Start;
Child2Reculse:

//		pCurrentNode = pCurrentNode->pParent->ppChild[3];
		pCurrentNode = &(mpNode[(mpNode[pCurrentNode->dwParent].pdwChild[3])]);
		goto Start;
Child3Reculse:

//		pCurrentNode	=	pCurrentNode->pParent;
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

	goto Return;

Return:
	if( pCurrentNode->dwLevel == 0)		// 탑노드이면,
	{
		return ;
	}
	switch(pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// 뭔가 잘못됬다.
	}

	return;
}


/****************************************************************************
	private:	DeleteTreeReculsive
	트리 전체를 재귀적으로 지워나간다.
****************************************************************************/
void MClipper::DeleteNodes()
{
	if( mpNode)
	{
		DWORD	dwClock		=	GetLowClock();
		for( DWORD i = 0; i < mdwNodeCount; i++)
		{
			// static object
			delete[]	mpNode[i].pStaticObjectIndex;
			mpNode[i].dwStaticObjectCount	=	0;
			
// 디버깅할땐 푸는게 좋을텐데...
			// dynamic object
//			if( mpNode[i].pDLink->pNext->iIndex != INT_MAX)
//				_asm int 3;
			DeleteLink( mDynamicNodePool, mpNode[i].pDLink);
			
		}
		dwClock		=	GetLowClock()	-	dwClock;
		delete[]	mpNode;
		mpNode			=	0;
	}	
}


/****************************************************************************
****************************************************************************/
void MClipper::FindNearObject( BOUNDING_SPHERE& bs)
{
	mdwDBG_TraveledNodeCount = 0;
	int		iResult = 0;

	DWORD	dwObjType;

	memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);

	memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

	// dobj 링크 버퍼.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mdwDObjCandidateCount	=	0;
	mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

	unsigned int iClock = GetLowClock();
	// 트리타기.
	FindNearObjectReculsive( 0, bs);
	iClock = GetLowClock() - iClock;

	// SObj	버퍼 정리.
	iClock = GetLowClock();
	DWORD i = 0, j = 0;
	for( i = 0; i < ms.dwCount; i++)
	{
		// 버퍼 체크.
		if( mpTempStaticIndexBuffer[i] != 0)
		{
			iResult = WhereIsAABBFromBoundingSphere( ms.pAABB[i], bs);
			if( iResult >= 0)
			{
				dwObjType		=	ms.pdwType[i];
				mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
				mCBuff.pdwCount[dwObjType]++;
			}
		}
	}
	iClock = GetLowClock() - iClock;

	// 다이나믹 오브젝트.
	iClock = GetLowClock();
	// dynamic object 버퍼 정리.
	MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;
	while(1)
	{
		DWORD dwIndex		=	pLink->iIndex;
		if( dwIndex	== INT_MAX)
			break;

		iResult			=	WhereIsAABBFromBoundingSphere( md.pAABB[dwIndex], bs);
		if( iResult >= 0)
		{
			dwObjType		=	md.pdwType[dwIndex];
			mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
			mCBuff.pdwCount[dwObjType]++;
		}
		pLink	=	pLink->pNext;
	}
	iClock = GetLowClock() - iClock;


	return ;
}

/****************************************************************************
	private:	FindNearObjectReculsive
****************************************************************************/
void MClipper::FindNearObjectReculsive(DWORD dwNode, BOUNDING_SPHERE &bs)
{
	int	iResult = 0;
	DWORD	i = 0;//, j = 0;
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;

	// 현재 받은 노드가 바운딩 스피어 안에 들어가는가.?
	iResult	=	WhereIsAABBFromBoundingSphere( pCurrentNode->Box, bs);
	if( iResult == -1)	// 완전히 안들어간다.
	{
		goto Return ;
	}

	if( pCurrentNode->dwLevel == mdwMaxLevel || iResult == 1 || pCurrentNode->dwLevel >= mdwMaxFindNodeDepth || pCurrentNode->dwStaticObjectCount <= mdwMinFindNodeObjectCount)	// 완전히 들어가거나 최대깊이까지 들어갔을경우...
	{
		// 현제의 오브젝트들을 임시인덱스에 기록하고 리턴.
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}

		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}

		goto Return ;
	}
	else 
	{
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[1]]);
		goto Start;
Child1Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[2]]);
		goto Start;
Child2Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[3]]);
		goto Start;
Child3Reculse:
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

Return:
	if( pCurrentNode->dwLevel == 0)
	{
		return ;
	}
	switch( pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3; //여기까지 오면 안되지.. -_-;
	}

}

/****************************************************************************
****************************************************************************/
void MClipper::FindNearObjectWithCapsule( BOUNDING_CAPSULE& bc)
{
	mdwDBG_TraveledNodeCount = 0;
	int		iResult = 0;

	DWORD	dwObjType;

	memset( mpTempStaticIndexBuffer, 0, sizeof(BYTE)*ms.dwCount);

	memset( mCBuff.pdwCount, 0, sizeof(DWORD)*GX_MAP_OBJECT_TYPE_NUM);

	// dobj 링크 버퍼.
	this->DeleteLink( mDObjCandidatePool, mpDObjCandidateLink);
	mdwDObjCandidateCount	=	0;
	mpDObjCandidateLink	=	CreateDummyLink( mDObjCandidatePool);

	unsigned int iClock = GetLowClock();
	// 트리타기.
	FindNearObjectWithCapsuleReculsive( bc);
	iClock = GetLowClock() - iClock;

	// SObj	버퍼 정리.
	iClock = GetLowClock();
	DWORD i = 0, j = 0;
	for( i = 0; i < ms.dwCount; i++)
	{
		// 버퍼 체크.
		if( mpTempStaticIndexBuffer[i] != 0)
		{
//			iResult = WhereIsAABBFromBoundingSphere( ms.pAABB[i], bs);
			iResult	=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, ms.pBS[i]);
			if( iResult >= 0)
			{
				dwObjType		=	ms.pdwType[i];
				mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	ms.ppObj[i];
				mCBuff.pdwCount[dwObjType]++;
			}
		}
	}
	iClock = GetLowClock() - iClock;

	// 다이나믹 오브젝트.
	iClock = GetLowClock();
	// dynamic object 버퍼 정리.
	MClipperObjLink*	pLink		=	mpDObjCandidateLink->pNext;
	while(1)
	{
		DWORD	dwIndex		=	pLink->iIndex;
		if( dwIndex	==	INT_MAX)
			break;

		iResult			=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, md.pBS[dwIndex]);
		if( iResult >= 0)
		{
			dwObjType		=	md.pdwType[dwIndex];
			mCBuff.pppBuff[dwObjType][mCBuff.pdwCount[dwObjType]]	=	md.ppObj[dwIndex];
			mCBuff.pdwCount[dwObjType]++;
		}
		pLink	=	pLink->pNext;
	}
	iClock = GetLowClock() - iClock;

	return ;
}

void MClipper::FindNearObjectWithCapsuleReculsive( BOUNDING_CAPSULE& bc)
{
	int	iResult = 0;
	DWORD	i = 0;
	MClipperNode*	pCurrentNode	=	&(mpNode[0]);

Start:	
	mdwDBG_TraveledNodeCount++;

	// 현재 받은 노드가 바운딩 스피어 안에 들어가는가.?
//	iResult	=	WhereIsAABBFromBoundingSphere( pCurrentNode->Box, bs);
	iResult	=	BOUNDING_CAPSULE_Meet_BOUNDING_SPHERE( bc, pCurrentNode->Sphere);
	if( iResult == -1)	// 완전히 안들어간다.
	{
		goto Return ;
	}

	if( pCurrentNode->dwLevel == mdwMaxLevel || iResult == 1 || pCurrentNode->dwLevel >= mdwMaxFindNodeDepth || pCurrentNode->dwStaticObjectCount <= mdwMinFindNodeObjectCount)	// 완전히 들어가거나 최대깊이까지 들어갔을경우...
	{
		// 현제의 오브젝트들을 임시인덱스에 기록하고 리턴.
		for( i = 0; i < pCurrentNode->dwStaticObjectCount; i++)
		{
			mpTempStaticIndexBuffer[pCurrentNode->pStaticObjectIndex[i]] = 1;
		}

		// dynamic object
		MClipperObjLink*	pLink = pCurrentNode->pDLink->pNext;
		for( ; pLink->iIndex != INT_MAX; pLink = pLink->pNext)
		{
			AddIndexToLink( mDObjCandidatePool, mpDObjCandidateLink, pLink->iIndex);
		}

		goto Return ;
	}
	else 
	{
		pCurrentNode = &(mpNode[pCurrentNode->pdwChild[0]]);
		goto Start;
Child0Reculse:

		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[1]]);
		goto Start;
Child1Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[2]]);
		goto Start;
Child2Reculse:
		pCurrentNode	=	&(mpNode[mpNode[pCurrentNode->dwParent].pdwChild[3]]);
		goto Start;
Child3Reculse:
		pCurrentNode	=	&(mpNode[pCurrentNode->dwParent]);
	}

Return:
	if( pCurrentNode->dwLevel == 0)
	{
		return ;
	}
	switch( pCurrentNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3; //여기까지 오면 안되지.. -_-;
	}

}

/****************************************************************************
****************************************************************************/
void MClipper::SetMaxFindNodeDepth( DWORD dwMaxFindNodeDepth)
{
	mdwMaxFindNodeDepth = dwMaxFindNodeDepth;
}
/****************************************************************************
****************************************************************************/
void MClipper::SetMinFindNodeObjectCount( DWORD dwMinFindNodeObjectCount)
{
	mdwMinFindNodeObjectCount = dwMinFindNodeObjectCount;
}

/****************************************************************************
****************************************************************************/
void MClipper::AddIndexToLink(STMPOOL_HANDLE pool, MClipperObjLink *pLink, int iIndex)
{
#ifdef	_DEBUG
	if( iIndex <= -1 || iIndex >= INT_MAX)
	{
		PrintfDebugString( "%s, %d", __FILE__, __LINE__);
		_asm int 3;		// 한계 이상의 인덱스가 들어왔다.
	}
#endif

	MClipperObjLink*	pNew;

	while(1)
	{
		if( pLink->iIndex >= iIndex)
		{
			goto	lbReturn;
		}
		else if( pLink->pNext->iIndex > iIndex)		// 끼워넣을 조건.
		{
			pNew	=	(MClipperObjLink*)LALAlloc( pool);
			pNew->iIndex			=	iIndex;
			pNew->pPrev				=	pLink;
			pNew->pNext				=	pLink->pNext;
			pLink->pNext->pPrev		=	pNew;
			pLink->pNext			=	pNew;
			goto	lbReturn;
		}
		

		pLink	=	pLink->pNext;
		if( pLink->pNext == 0)
			goto	lbReturn;
	}

lbReturn:
	return	;
}

/****************************************************************************
****************************************************************************/
BOOL MClipper::DeleteIndexFromLink(STMPOOL_HANDLE pool, MClipperObjLink* pLink, int iIndex)
{
#ifdef	_DEBUG
	if( iIndex <= -1 || iIndex >= INT_MAX)
	{
		PrintfDebugString( "%s, %d", __FILE__, __LINE__);
		_asm int 3;		// 한계 이상의 인덱스가 들어왔다.
		return	FALSE;
	}
#endif

	while(1)
	{
		if( pLink->iIndex == iIndex)	// 지울 노드.
		{
			pLink->pPrev->pNext		=	pLink->pNext;
			pLink->pNext->pPrev		=	pLink->pPrev;

			LALFree( pool, pLink);
			return		TRUE;
		}

		if( pLink->iIndex > iIndex)
		{
			goto	lbReturnFalse;
		}

		pLink	=	pLink->pNext;
		if( pLink->pNext == 0)
			goto	lbReturnFalse;
	}

lbReturnFalse:
	return	FALSE;
}

/****************************************************************************
	// 트리의 노드에 다이나믹 오브젝트의 더미링크 두개를 단다.
****************************************************************************/
MClipperObjLink* MClipper::CreateDummyLink(STMPOOL_HANDLE pool)
{
	MClipperObjLink*	pDLink	=	(MClipperObjLink*)LALAlloc( pool);
	pDLink->pPrev			=	0;
	pDLink->iIndex			=	-1;
	pDLink->pNext			=	(MClipperObjLink*)LALAlloc( pool);
	pDLink->pNext->pPrev	=	pDLink;
	pDLink->pNext->iIndex	=	INT_MAX;
	pDLink->pNext->pNext		=	0;

	return	pDLink;
}

/****************************************************************************
	트리 내에 dobj 링크를 넣는다.
****************************************************************************/
void MClipper::AddDynamicObjectToTree(DWORD dwIndex)
{
	BOOL	bResult;
	MClipperNode*		pNode	=	&mpNode[0];
	int					iNode	=	0;

lbStart:
	// aabb간 충돌.
	bResult		=	IsAABBMeetAABB( pNode->Box, md.pAABB[dwIndex]);
	// 이 노드랑 만나면 노드에 추가.
	if( bResult == TRUE)
	{
		AddIndexToLink( mDynamicNodePool, pNode->pDLink, dwIndex);
	}

	// 노드와 만나지 않거나 이 노드가 리프라면 더이상 재귀 않고 리턴.
	if( bResult == FALSE || pNode->dwLevel == mdwMaxLevel)
	{
		goto	lbReturn;
	}
	// 아니면 자식 재귀.
	else		
	{
		pNode	=	&(mpNode[pNode->pdwChild[0]]);
		goto	lbStart;
		// 자식 재귀.
Child0Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[1])]);
		goto	lbStart;
Child1Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[2])]);
		goto	lbStart;
Child2Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[3])]);
		goto	lbStart;
Child3Reculse:

		pNode	=	&(mpNode[pNode->dwParent]);
	}

//	goto lbReturn;

lbReturn:
	if( pNode->dwLevel == 0)		// 탑노드이면,
	{
		return ;
	}
	switch(pNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// 뭔가 잘못됬다.
	}



	_asm int 3;
}

/****************************************************************************
	트리에서 dobj를 뺀다.
****************************************************************************/
void MClipper::DeleteDynamicObjectFromTree(DWORD dwIndex)
{

	BOOL		bResult;
	MClipperNode*		pNode	=	&mpNode[0];
	int					iNode	=	0;

//	return	;

lbStart:
	bResult	=	DeleteIndexFromLink( mDynamicNodePool, pNode->pDLink, dwIndex);
	if( bResult == FALSE || pNode->dwLevel == mdwMaxLevel)
	{
		goto	lbReturn;
	}
	// 아니면 자식 재귀.
	else		
	{
		pNode	=	&(mpNode[pNode->pdwChild[0]]);
		goto	lbStart;
		// 자식 재귀.
Child0Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[1])]);
		goto	lbStart;
Child1Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[2])]);
		goto	lbStart;
Child2Reculse:

		pNode	=	&(mpNode[(mpNode[pNode->dwParent].pdwChild[3])]);
		goto	lbStart;
Child3Reculse:

		pNode	=	&(mpNode[pNode->dwParent]);
	}

//	goto lbReturn;

lbReturn:
	if( pNode->dwLevel == 0)		// 탑노드이면,
	{
		return ;
	}
	switch(pNode->dwChildNum)
	{
		case 0:
			goto Child0Reculse;
			break;
		case 1:
			goto Child1Reculse;
			break;
		case 2:
			goto Child2Reculse;
			break;
		case 3:
			goto Child3Reculse;
			break;
		default:
			_asm int 3;		// 뭔가 잘못됬다.
	}

	_asm int 3;

}

/****************************************************************************
****************************************************************************/
void MClipper::DeleteLink(STMPOOL_HANDLE pool, MClipperObjLink *pLink)
{
	if( pLink == 0 || pool == 0)
		goto	lbReturn;

	MClipperObjLink*	pNext;

	while(1)
	{
		pNext	=	pLink->pNext;
		LALFree( pool, pLink);
		pLink	=	pNext;

		if( pLink == 0)
			goto	lbReturn;
	}

lbReturn:
	return	;
}

/****************************************************************************
****************************************************************************/
BOOL MClipper::TestWorldBox( MAABB& TempBox)
{
	float	fTemp;
	BOOL	bResult	=	TRUE;
	// 맥스값 비교.
	if( TempBox.Max.x > mWorldBox.Max.x)
	{
		bResult		=	FALSE;
		mWorldBox.Max.x	=	TempBox.Max.x;
	}
	if( TempBox.Max.y > mWorldBox.Max.y)
	{
		bResult		=	FALSE;
		mWorldBox.Max.y	=	TempBox.Max.y;
	}
	if( TempBox.Max.z > mWorldBox.Max.z)
	{
		bResult		=	FALSE;
		mWorldBox.Max.z	=	TempBox.Max.z;
	}

	// 민값 비교.
	if( TempBox.Min.x < mWorldBox.Min.x)
	{
		bResult		=	FALSE;
		mWorldBox.Min.x	=	TempBox.Min.x;
	}
	if( TempBox.Min.y < mWorldBox.Min.y)
	{
		bResult		=	FALSE;
		mWorldBox.Min.y	=	TempBox.Min.y;
	}
	if( TempBox.Min.z < mWorldBox.Min.z)
	{
		bResult		=	FALSE;
		mWorldBox.Min.z	=	TempBox.Min.z;
	}

	// 맥스,민 뒤집어진거 비교.
	if( mWorldBox.Max.x - mWorldBox.Min.x <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.x;
		mWorldBox.Max.x		=	mWorldBox.Min.x;
		mWorldBox.Min.x		=	fTemp;
		bResult		=	FALSE;
	}
	if( mWorldBox.Max.y - mWorldBox.Min.y <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.y;
		mWorldBox.Max.y		=	mWorldBox.Min.y;
		mWorldBox.Min.y		=	fTemp;
		bResult		=	FALSE;
	}
	if( mWorldBox.Max.z - mWorldBox.Min.z <= 0.0f)
	{
		fTemp				=	mWorldBox.Max.z;
		mWorldBox.Max.z		=	mWorldBox.Min.z;
		mWorldBox.Min.z		=	fTemp;
		bResult		=	FALSE;
	}


	return	bResult;
}

DWORD MClipper::GetRenderTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)
{
	*pppBuff	=	mRCBuff.pppBuff[dwID];
	return		mRCBuff.pdwCount[dwID];
}

DWORD MClipper::GetTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)
{
	*pppBuff	=	mCBuff.pppBuff[dwID];
	return		mCBuff.pdwCount[dwID];
}

DWORD MClipper::GetMeshObjects(CGXMapObject** ppBuffer,DWORD dwMaxNum)
{
	DWORD	dwResultNum	=	0;

	DWORD	i;
	
	DWORD	dwComponentCount;
	CGXMapObject**	ppBuff;

	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		dwComponentCount	=	this->GetTargetBuffer(&ppBuff, i);

		if( dwComponentCount + dwResultNum > dwMaxNum)
		{
			_asm int 3;				// 버퍼 크기보다 결과물이 많다.
			dwComponentCount	=	dwMaxNum	-	dwResultNum;
		}
		memcpy( ppBuffer, ppBuff, sizeof(CGXMapObject*)*dwComponentCount);
		ppBuffer	=	ppBuffer	+	dwComponentCount;		// 포인터 더하기.
		dwResultNum	=	dwResultNum		+	dwComponentCount;	// 카운트 더하기.
	}

	return dwResultNum;
}

// 노드만 살짝 지우고 다시 빌드한다.
void MClipper::Rebuild(float fTop,float fBottom)
{
#ifdef	_DEBUG
	PrintfDebugString("MClipper::Rebuild( fTop = %f, fBottom = %f)\n", fTop, fBottom);
#endif

	if( mpNode)
	{
		DeleteNodes();

		delete[]	ms.pAABB;
		ms.pAABB	=	0;
		delete[]	ms.pBS;
		ms.pBS		=	0;
		delete[]	ms.pBB;
		ms.pBB		=	0;

		DeleteCandidateBuffer();
		delete[]	mpTempStaticIndexBuffer;
		mpTempStaticIndexBuffer	=	0;
		delete[]	ms.pdwType;
		ms.pdwType		=	0;
		delete[]	mpTempStaticIndexBuffer;
		mpTempStaticIndexBuffer		=	0;

		mWorldBox.Max.y	=	fTop;
		mWorldBox.Min.y	=	fBottom;
		BuildTree( mdwMode, mWorldBox, mdwMaxLevel);

		RefreshDObjects();
	}
	else
	{
		_asm int 3;	// 트리가 빌드되지 안았는데 리빌드가 호출됬다.
	}

}

void MClipper::CreateCandidateBuffer(DWORD dwMaxDynamicObjectCount, DWORD dwStaticObjectCount)
{
	// 출력버퍼 0, 3, 4는 다이나믹, 1, 2는 스테이틱 오브젝트.
	mRCBuff.pppBuff[0]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_OBJECT
	mRCBuff.pppBuff[1]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_STRUCT
	mRCBuff.pppBuff[2]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_HFIELD
	mRCBuff.pppBuff[3]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_DECAL
	mRCBuff.pppBuff[4]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_TRIGGER
	mRCBuff.pppBuff[5]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_LIGHT

	mCBuff.pppBuff[0]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_OBJECT
	mCBuff.pppBuff[1]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_STRUCT
	mCBuff.pppBuff[2]	=	new	CGXMapObject*[ms.dwCount];		// GX_MAP_OBJECT_TYPE_HFIELD
	mCBuff.pppBuff[3]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_DECAL
	mCBuff.pppBuff[4]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_TRIGGER
	mCBuff.pppBuff[5]	=	new CGXMapObject*[md.dwMaxCount];	// GX_MAP_OBJECT_TYPE_LIGHT
}

void MClipper::DeleteCandidateBuffer()
{
	int	i;
	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		if( mRCBuff.pppBuff[i])
		{
			delete	mRCBuff.pppBuff[i];
			mRCBuff.pppBuff[i]		=	0;
		}
		mRCBuff.pdwCount[i]		=	0;
	}
	for( i = 0; i < GX_MAP_OBJECT_TYPE_NUM; i++)
	{
		if( mCBuff.pppBuff[i])
		{
			delete	mCBuff.pppBuff[i];
			mCBuff.pppBuff[i]		=	0;
		}
		mRCBuff.pdwCount[i]		=	0;
	}

}

void MClipper::RefreshDObjects()
{
	DWORD	i;
	for( i = 0; i < md.dwMaxCount; i++)
	{
		if( md.ppObj[i] != 0)
		{
			UpdateDynamicObject( i);
		}
	}
}

DWORD MClipper::AddDecal( CGXDecal* pDecal)
{
	// 빈 인덱스 하나 얻어서,
	DWORD	dwIndex	=	ICAllocIndex( md.ICHandle) - 1;

	// 빈 인덱스가 없으면 에러, 리턴.
	if( dwIndex == 0xffffffff)
	{
		_asm int 3;
		return	0xffffffff;
	}

	// 인덱스의 내용을 채우고,
	md.ppObj[dwIndex]	=	(CGXMapObject*)pDecal;
	md.pdwType[dwIndex]		=	pDecal->GetObjectType();

	// aabb 바운딩박스.

	pDecal->GetAABB( &(md.pAABB[dwIndex]));

	// 바운딩 스피어.
	pDecal->GetBoundingSphere( &(md.pBS[dwIndex]));

	// 트리에 인서트.
	if( mpNode != 0)
	{
		AddDynamicObjectToTree( dwIndex);
	}

	md.dwCount++;

	return dwIndex;
}

BOOL MClipper::UpdateDecal( DWORD dwIndex)
{
	_asm int 3;			// 어차피 고정될건데 의미가 없지 않은가.?

	DWORD	dwDeleteClock;
	DWORD	dwAddClock;
	// 그 인덱스가 사용되는가.?
	if( md.ppObj[dwIndex] == 0)	// 사용되고 있지 않으면.
	{
		_asm int 3;
		return FALSE;
	}
	
	dwDeleteClock	=	GetLowClock();
	// 트리에서 지운다.
	if( mpNode)
	{
		DeleteDynamicObjectFromTree( dwIndex);
	}
	dwDeleteClock	=	GetLowClock()	-	dwDeleteClock;

	CGXDecal*	pDecal	=	(CGXDecal*)md.ppObj[dwIndex];
	// aabb 바운딩박스.
	pDecal->GetAABB( &(md.pAABB[dwIndex]));
	// 바운딩 스피어.
	pDecal->GetBoundingSphere( &(md.pBS[dwIndex]));

	dwAddClock		=	GetLowClock();
	// 트리 갱신.
	if( mpNode)
	{
		AddDynamicObjectToTree(dwIndex);
	}
	dwAddClock		=	GetLowClock()	-	dwAddClock;

	return TRUE;

}

BOOL MClipper::DeleteDecal( DWORD dwIndex)
{
	// 실제로 그 인덱스가 사용되는가.?
	if( md.ppObj[dwIndex] == 0)		// 사용되고 있지 않으면.
	{
//		_asm int 3;
		return FALSE;
	}
	// 트리에서 그 오브젝트를 지우고,
	if( mpNode )
	{
		DeleteDynamicObjectFromTree(dwIndex);
	}
	// 지워지기 전에 해당 gxmapobject의 클리퍼인덱스를 0xffffffff로 만든다.
	// 이는 gxmap이 지워져서 클리퍼가 없어졌다가 gxmap이 다시 생성되어 클리퍼 역시 다시 생성 된 상태에서,
	// 바깥에서 지워지지 않은 gxmapobject가 updatetogxmap을 호출하지 않은 상태에서 클리퍼를 다루지 못하게 방어 하기 위함이다.
	// 같은 내용의 코드가 DeleteDynamicObject()와 DeleteDecal()에 존재한다.
	md.ppObj[dwIndex]->SetClipperIndex( 0xffffffff);

	// 인덱스의 내용을 지우고,
	md.ppObj[dwIndex]	=	NULL;
	memset( &(md.pAABB[dwIndex]), 0, sizeof( MAABB));
	// 인덱스 반환.
//	md.IndexManager.ReleaseIndex( dwIndex);
	ICFreeIndex( md.ICHandle, dwIndex + 1);
	md.dwCount--;

	return TRUE;
}
 