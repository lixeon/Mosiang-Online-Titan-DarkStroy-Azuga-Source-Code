// CollisionStaticModel.cpp: implementation of the CCollisionStaticModel class.
//
//////////////////////////////////////////////////////////////////////

#include "CollisionStaticModel.h"
#include "global_variable.h"

#include	"../4DyuchiGRX_myself97/CollisionTest/CollisionTestMovingEllipsoidMeetTriangle.h"
#include	"../4DyuchiGRX_myself97/CheckClock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCollisionStaticModel::CCollisionStaticModel()
{
	m_dwObjectsNum				=	0;
	m_pCollisionMeshObjectList	=	NULL;
	m_pSearchTree				=	NULL;
}

BOOL CCollisionStaticModel::Render(I4DyuchiGXRenderer* pRenderer,DWORD dwFlag)
{
	BOOL	bResult = FALSE;

	if (!pRenderer)
		goto lb_return;


	DWORD i;
	for (i=0; i<m_dwObjectsNum; i++)
	{
		m_pCollisionMeshObjectList[i].Render(pRenderer,dwFlag);
	}
	

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CCollisionStaticModel::ReadFile(char* szFileName)
{
	DWORD	dwReadFileTick	=	GetTickCount();

	BOOL		bResult = FALSE;

	
	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////
		goto lb_return;
	}

	FILE_COLLISION_STATIC_MODEL_HEADER	header;
	g_pFileStorage->FSRead(pFP,&header,sizeof(header));
	m_dwObjectsNum = header.dwObjectNum;

	m_pCollisionMeshObjectList = new CCollisionMeshObject[m_dwObjectsNum];

	DWORD	i;

	for (i=0; i<m_dwObjectsNum; i++)
	{
		m_pCollisionMeshObjectList[i].ReadFile(pFP,header.dwVersion);
	}

	g_pFileStorage->FSCloseFile(pFP);

	bResult = TRUE;

	// 일단 오브젝트가 하나라도 있으면 빌드 트리.
	if( m_dwObjectsNum != 0)
	{
		BuildSearchTree(0);
	}

lb_return:
	dwReadFileTick	=	GetTickCount()	-	dwReadFileTick;

	PrintfDebugString("CCollisionStaticModel::ReadFile(\"%s\") Ticks = %d\n", szFileName, dwReadFileTick);
	return bResult;
}


CCollisionStaticModel::~CCollisionStaticModel()
{
	if (m_pCollisionMeshObjectList)
	{
		ReleaseSearchTree();

		delete [] m_pCollisionMeshObjectList;
		m_pCollisionMeshObjectList = NULL;
	}
}

BOOL CCollisionStaticModel::BuildSearchTree(DWORD dwFlag)
{
	PrintfDebugString( "CCollisionStaticModel::BuildSearchTree()\n");


	if( m_dwObjectsNum == 0 )
	{
		return	FALSE;
	}

	if( m_dwObjectsNum > 10000)	// 10000은 임의의수. 상식적으로 저까지 가면 안되지~
	{
		PrintfDebugString( "CCollisionStaticModel::BuildSearchTree() m_dwObjectsNum is more than 10,000\n");
		_asm int 3;
		return	FALSE;
	}


	DWORD	dwBuildTreeClock	=	GetTickCount();
	m_pSearchTree	=	new	CCollisionMeshObjectTree;
	m_pSearchTree->CreateTree( m_pCollisionMeshObjectList, m_dwObjectsNum);
	dwBuildTreeClock	=	GetTickCount()	-	dwBuildTreeClock;
	PrintfDebugString( "CCollisionStaticModel::BuildSearchTree() BuildTree Tick = %d\n", dwBuildTreeClock);

	PrintfDebugString( "CCollisionStaticModel::BuildSearchTree() Finished\n");
	return	TRUE;
}

/*
	평면 0 = N*X + D
	라인 X = F + V*t;

	0	=	N*(F + V*t) + D;
 */
BOOL CCollisionStaticModel::CollisionTestLine(float *pOutT, VECTOR3 *pFrom, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
	DWORD	dwClock	=	GetLowClock();
	DWORD	dwResult	=	this->m_pSearchTree->CollisionTestLine( pOutT, pFrom, pTo, dwOutputBufferIndex);
	dwClock	=	GetLowClock()	-	dwClock;

	return	dwResult;
}

void CCollisionStaticModel::ReleaseSearchTree()
{
	if( m_pSearchTree)
	{
		m_pSearchTree->DeleteTree();
		delete	m_pSearchTree;
		m_pSearchTree	=	0;
	}
}

BOOL CCollisionStaticModel::CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID *pEllipsoid, VECTOR3 *pTo, DWORD dwOutputBufferIndex)
{
	DWORD	dwClock	=	GetLowClock();
	// 트리에 체크하게 해야되나.?
	BOOL	bResult	=	m_pSearchTree->CollisionTestAAELLIPSOID( pOut, pOutLastVelocity, pbOutLanding, pEllipsoid, pTo, dwOutputBufferIndex);
	dwClock	=	GetLowClock()	-	dwClock;

	return	bResult;
}

// 동시에 접근할 갯수.
void CCollisionStaticModel::SetSimultaneousAccessCount( DWORD dwSimultaneousAccessCount)
{
	m_pSearchTree->SetSimultaneousAccessCount( dwSimultaneousAccessCount);
}
