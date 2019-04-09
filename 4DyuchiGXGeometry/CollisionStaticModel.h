// CollsionStaticModel.h: interface for the CCollisionStaticModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_)
#define AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "colmeshobj.h"

#pragma pack(push,4)
struct FILE_COLLISION_STATIC_MODEL_HEADER
{
	DWORD					dwVersion;
	DWORD					dwObjectNum;
};
#pragma pack(pop)

//! CoStaticObject의 충돌 처리 단위.
/*!
	충돌 처리용 메쉬와 충돌처리 메소드를 내부에 가지고 있다.
*/
class CCollisionStaticModel  
{
	DWORD							m_dwObjectsNum;
	CCollisionMeshObject*			m_pCollisionMeshObjectList;

	CCollisionMeshObjectTree*		m_pSearchTree;

	BOOL BuildSearchTree( DWORD dwFlag);	// 파일을 읽은 후 빌드트리.
	void ReleaseSearchTree();

public:
	void	SetSimultaneousAccessCount( DWORD dwSimultaneousAccessCount);			// 동시에 접근할 갯수.
	/// 라인과 만나는 점 감지.
	BOOL CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	/// 주어진 타원체를 가지고 충돌 처리 테스트. 콜백은 어떻게 빼주나?
	BOOL CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex);

	BOOL							Render(I4DyuchiGXRenderer* pRenderer,DWORD dwFlag);
	BOOL							ReadFile(char* szFileName);

	CCollisionStaticModel();
	~CCollisionStaticModel();

};

#endif // !defined(AFX_COLLSIONSTATICMODEL_H__0C5C9E86_1BDD_4EDD_B9D5_F5EC86C0CD22__INCLUDED_)
