/*
	MClipper.h
	옥트리의 노드와 인덱스를 관리한다.

	기본 좌표계는 왼손에 x, 오른쪽, y윗쪽, z앞쪽.

	오브젝트 종류:
		스테이틱 메쉬, 스테이틱 라이트.		<= 이놈들은 트리 만들때 노드 만드는데 관여하고...

		다이나믹 메쉬, 다이나믹 라이트.		<= 이놈들은 일단은 그냥 버퍼 타게 한 다음, 실제론 노드에 끼워지게... 관리를..

*/
#if !defined(AFX_MCLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_)
#define AFX_MCLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchigrx_common/typedef.h"
#include "../4dyuchigrx_myself97/MIndexManager.h"
#include "../4dyuchigxExecutive/GXStaticMapObject.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGXExecutive/GXDecal.h"
#include "../4DyuchiGRX_common/IExecutive.h"


#define		_LOG_

class	CoExecutive;

struct	CLIPPER_CANDIDATE_BUFF
{
	CGXMapObject**			pppBuff[GX_MAP_OBJECT_TYPE_NUM];			// 버퍼가 타입별로 2002,01,20 현재 여섯개.
	DWORD					pdwCount[GX_MAP_OBJECT_TYPE_NUM];
};

struct	MClipperObjLink
{
	MClipperObjLink*			pPrev;
	MClipperObjLink*			pNext;
	int							iIndex;
};

struct MClipperNode		// 트리 노드.
{
	DWORD							dwParent;		// 부모노드.
	DWORD							dwChildNum;		// 자기 자신은 부모의 몇번노드인가.

	DWORD							pdwChild[4];		// 자식노드들.

	DWORD							dwLevel;			// 이 노드의 층.
	MAABB							Box;			// 이 노드의 바운딩 박스.
	BOUNDING_SPHERE					Sphere;			// 바운딩 박스는 충돌처리 할때 너무 느려... (구려..)

	// static object
	DWORD*							pStaticObjectIndex;		// static object의 인덱스 버퍼.
	DWORD							dwStaticObjectCount;		// static object의 갯수.

	// dynamic object
	MClipperObjLink*				pDLink;					// 다이나믹 오브젝트 링크.

	//	
};

struct	MClipperStaticObject
{
	CGXMapObject**			ppObj;							// 스테이틱 오브젝트가 저장될 버퍼.
	DWORD*					pdwType;								// 오브젝트 타입.
	MAABB*					pAABB;						// 스테이틱 오브젝트의 aabb 버퍼.
	BOUNDING_SPHERE*		pBS;							// 바운딩 스피어.
	BOUNDING_BOX*			pBB;
	DWORD					dwCount;						// 스테이틱 오브젝특의 갯수.

};

struct	MClipperDynamicObject
{
	INDEXCR_HANDLE			ICHandle;
//	MIndexManager			IndexManager;
	CGXMapObject**			ppObj;						// 다이나믹 오브젝트가 저장될 버퍼.
	DWORD*					pdwType;
	MAABB*					pAABB;					// 다이나믹 오브젝트의 aabb 버퍼.
	BOUNDING_SPHERE*		pBS;
	DWORD					dwCount;					// 다이나믹 오브젝트의 갯수.
	DWORD					dwMaxCount;					// 다이나믹 오브젝트의 최대 갯수.
};

class MClipper  
{
public:
	MClipper( );
	~MClipper();

	DWORD	Init( CoExecutive* pExecutive, DWORD dwMaxDObjectCount );	/// 초기화.
	// 트리 빌드 함수군.
	DWORD	AddStaticObject( CGXMapObject* pObject);		// 트리를 만들기 위해 스테이틱 오브젝트(맵 오브젝트)를 넣는다.
	// for문으로 AddStaticObject를 한다음 for를 끝내고 BuildTree 한다.
	DWORD	BuildTree( DWORD dwMode, MAABB WorldBox, DWORD dwMaxLevel);							// 받은 스테이틱 오브젝트로 트리를 빌드한다.

	void	GetWorldBox( MAABB* pWorldBox){ *pWorldBox = mWorldBox;}

	// dynamic object 함수군.
	DWORD	AddDynamicObject( CGXMapObject* pObject);	// 다이나믹 오브젝트를 추가한다.
	BOOL	UpdateDynamicObject( DWORD dwIndex);			// 오브젝트의 상태를 갱신.
	BOOL	DeleteDynamicObject( DWORD dwIndex);			// 다이나믹 오브젝트를 삭제한다.

	// 데칼 함수군.
	DWORD	AddDecal( CGXDecal* pDecal);
	BOOL	UpdateDecal( DWORD dwIndex);
	BOOL	DeleteDecal( DWORD dwIndex);

	// 상태 변경.
	void	SetMinFindNodeObjectCount( DWORD dwMinFindNodeObjectCount);		// 트리 탈때 노드당 오브젝트갯수가 여기에 달하면 더이상 내려가지 않는다.
	void	SetMaxFindNodeDepth( DWORD dwMaxFindDepth);						// 트리 탈때 노드 깊이가 이이상은 내려가지 않는다.
	void	SetMaxRenderNodeDepth( DWORD dwMaxRenderDepth);					// 트리 탈때 노드 깊이가 이이상은 내려가지 않는다.

	// 랜더링 후보 찾기.
	void		FindRenderCandidate( VIEW_VOLUME &vv);		// 트리를 타고 랜더링 후보를 찾는다.
	DWORD		GetRenderTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID);

	// 근처꺼 찾기 
	void		FindNearObject( BOUNDING_SPHERE& bs);
	void		FindNearObjectWithCapsule( BOUNDING_CAPSULE& bc);
	DWORD		GetTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID);

	DWORD		GetMeshObjects(CGXMapObject** ppBuffer,DWORD dwMaxNum);		// 이놈은 결과버퍼의 것들을 다 뭉쳐서 준다.

	// rebuildTree
	void		Rebuild(float fTop,float fBottom);

	// 모두 지우기.
	void		DeleteAll(void);

private:

	CoExecutive*				m_pExecutive;
	// static object가 저장된 버퍼.
	MClipperStaticObject	ms;
	DWORD					mdwTempStaticObjectCount;					// 임시변수. -_-;

	// dynamic object가 저장된 버퍼.
	MClipperDynamicObject	md;

	// 트리 정보.
	MAABB				mWorldBox;								// 트리의 전체 바운딩 박스.
	MAABB				mTempWorldBox;							// 외부에서 준게 아닌 자체적인 박스. 없어져야겠군.
	MClipperNode*		mpNode;
	DWORD				mdwNodeCount;
	DWORD				mdwNodeUsedCount;								// 트리의 최대 깊이, 0베이스.
	DWORD				mdwMaxLevel;								// 트리의 최대 깊이, 0베이스.
	DWORD				mdwMode;

	DWORD				mdwMaxFindNodeDepth;									// 찾을때의 최대 깊이 제한.
	DWORD				mdwMinFindNodeObjectCount ;							// 찾을때 노드가 가진 오브젝트가 이 갯수 이하이면 그만 들어간다.
	DWORD				mdwMaxRenderNodeDepth;								// 찾을때의 최대 깊이 제한.

	// 랜더링 버퍼.
	CLIPPER_CANDIDATE_BUFF		mRCBuff;			// Render Candidate Buff

	// 그외 결과물 버퍼.
	CLIPPER_CANDIDATE_BUFF		mCBuff;				// Candidate Buff

	// 다이나믹 오브젝트 결과물 링크.
	STMPOOL_HANDLE			mDObjCandidatePool;
	MClipperObjLink*		mpDObjCandidateLink;
	DWORD					mdwDObjCandidateCount;

	// 임시버퍼.
	BYTE*					mpTempStaticIndexBuffer;

	DWORD					mdwDBG_TraveledNodeCount;		// 노드 뒤진 갯수.

	STMPOOL_HANDLE			mDynamicNodePool;			// 노드가 가질 다이나믹 오브젝트 인덱스 메모리풀.
	DWORD					mdwMaxDynamicNodePool;

protected:
	BOOL	TestWorldBox( MAABB& TempBox);

	// 트리 만들때 내부 함수들.
	DWORD	CreateNodeReculsive( DWORD dwParent, DWORD dwChildNum);	// 트리를 만든다. 재귀.
	MAABB	GetChildAABB( DWORD dwParent, DWORD dwChildNum);	// pParent의 iChildNum번째 자식의 AABB를 구한다.

	void	CreateCandidateBuffer( DWORD dwMaxDynamicObjectCount, DWORD dwStaticObjectCount);

	// 다이나믹 오브젝트 추가, 삭제 함수군.
	void	AddDynamicObjectToTree( DWORD dwIndex);
	void	DeleteDynamicObjectFromTree( DWORD dwIndex);

	// 찾기 함수군 내부 함수.
	void	FindRenderCandidateReculsive( DWORD dwNode, VIEW_VOLUME& vv);	// 랜더링 후보를 찾는 재귀함수.
	void	FindNearObjectReculsive( DWORD dwNode, BOUNDING_SPHERE& bs);
	void	FindNearObjectWithCapsuleReculsive( BOUNDING_CAPSULE& bc);

	// 노드링크, 랜더링 버퍼링크 다루는 함수군.
	MClipperObjLink*	CreateDummyLink( STMPOOL_HANDLE pool);
	void	DeleteLink(STMPOOL_HANDLE pool, MClipperObjLink* pLink);
	void	AddIndexToLink( STMPOOL_HANDLE pool, MClipperObjLink* pNode, int iIndex);
	BOOL	DeleteIndexFromLink( STMPOOL_HANDLE pool, MClipperObjLink* pNode, int iIndex);			// 그 인덱스가 있었으면 true

	// rebuild tree
	void	RefreshDObjects();			// 리빌드 트리한 다음 다이나믹 오브젝트를 트리에 넣는다.

	// DeleteAll 내부 함수들.
	void	DeleteNodes( );
	void	DeleteCandidateBuffer();
};

#endif // !defined(AFX_MOCTREECLIPPER_H__9B2F5580_11F8_411E_B263_51D0AC5EE021__INCLUDED_)
