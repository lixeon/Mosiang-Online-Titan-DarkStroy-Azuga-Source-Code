#ifndef CO_STATIC_MODEL_H
#define CO_STATIC_MODEL_H


#include "../4DyuchiGRX_common/stdafx.h"
#include "MaterialList.h"
#include "../4DyuchiGRX_common/IGeometry.h"


class CoGeometry;
class CMeshObject;
class CLightTexture;

// 현재 버젼 정보
#define	STATIC_MODEL_VERSION		 0x00000001

class CCollisionStaticModel;

class CoStaticModel : public I3DStaticModel
{
	DWORD					m_dwRefCount;
//	CMaterial*				m_pBaseMaterial;

	DWORD					m_dwMotionNum;
	I4DyuchiGXRenderer*		m_pRenderer;
	VECTOR3					m_v3PrvCameraPos;

	CoGeometry*				m_pGeometry;
	DWORD					m_dwIndexInGeometry;

	COLLISION_MODEL_DESC*	m_pColMeshModelDescWorld;
	CCollisionStaticModel*	m_pCollisionStaticModel;

		
	DWORD					m_dwAmbientColor;
	BOOL					m_bOptimizedFlag;
	BOOL					m_bRecvShadowEnable;
	
	char					m_szTempLMFileName[MAX_NAME_LEN];
	char					m_szTempVCLFileName[MAX_NAME_LEN];

	DWORD					m_dwColMeshSize;
	ITEMTABLE_HANDLE		m_pIndexItemTableMeshObject;
	DWORD					m_dwMaxMaterialNum;
	CMaterialList*			m_pMaterialList;

	
	MATRIX4*				m_pDefaultMatrixList;

	SORT_LINK*				m_pDevAllocObjHead;
	SORT_LINK*				m_pDevAllocObjTail;
	char					m_szSTMPath[_MAX_PATH];
	char					m_szFileName[_MAX_PATH];

	CMeshObject*			AllocObject();
	CMeshObject*			GetObject(char* szObjName);
	void					ReleaseObjects();
	void					Clean();
	void					ResetMaterial(CFaceGroup* pFaceGroup,CMaterialList* pMtlList);
	void					ResetMaterialAll();
	void					InitializeObjects();
	void					CleanColMesh();
	void					OnFaultObject(CMeshObject* pMeshObj);
	BOOL					CommitIDIMeshObject(CMeshObject* pObj);
	BOOL					DecommitIDIMeshObject(CMeshObject* pObj);
	BOOL					CommitAllIDIMeshObjectsInViewVolume();
	BOOL					CommitAllIDIMeshObjectsInViewSphere();

public:	

	//external interface
	/////////////////////////////////////////////////////////////////////////////////////////////////
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
	
	BOOL			__stdcall	BeginCreateMesh(DWORD dwFlag);
	BOOL			__stdcall	AddModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pTransformMatrix,DWORD dwFlag);
	BOOL			__stdcall	EndCreateMesh();

	BOOL			__stdcall	DeleteObject(DWORD dwIndex);

	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc(DWORD dwObjIndex);

	void			__stdcall	GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc);
	BOOL			__stdcall	RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwFlag);
		
	DWORD			__stdcall	CreateIVertexList(IVERTEX** ppVertex);
	DWORD			__stdcall	CreateIVertexListWithIndex(IVERTEX** ppVertex,DWORD dwObjIndex);
	void			__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);
	
	DWORD			__stdcall	GetObjectIndex(char* szObjName);
	

	void			__stdcall	Optimize();	
	DWORD			__stdcall	GetObjectNum();
	DWORD			__stdcall	GetObjectIndex(DWORD dwSeqIndex);

		
	BOOL			__stdcall	BeignShadeLightMapObject(DWORD dwColor);
	BOOL			__stdcall	ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc);
	void			__stdcall	EndShadeLightMapObject();

	BOOL			__stdcall	ReadFile(char* szFileName,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag);
	BOOL			__stdcall	WriteFile(char* szFileName);
	DWORD			__stdcall	GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	BOOL			__stdcall	IsCollisionMeshObject(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwObjIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	BOOL			__stdcall	SetLightTexel(POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwColor,DWORD dwViewportIndex);
	BOOL			__stdcall	GetLightTexel(DWORD* pdwColor,POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex);
	BOOL			__stdcall	RenderCollisionStaticModel(DWORD dwFlag);
	BOOL			__stdcall	RunManageResource(DWORD dwViewportIndex);

	void			__stdcall	SetCollisionTestCallBackProc( CollisionTestCallBackProcedure pCallBack);
	BOOL			__stdcall	CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	BOOL			__stdcall	CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputBufferIndex);
	
	void			__stdcall	SetSimultaneousAccessCount( DWORD dwCount);		// 동시에 호출할 갯수. 아마 쓰레드만큼.?

	void			__stdcall	EnableRecvShadow(BOOL bEnable);

	// 주어진 AA타원체를 To로 밀어서 결과를 리턴한다.
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	char*					GetStaticModelFileName() {return m_szFileName;}
	BOOL					Initialize(I4DyuchiGXRenderer* pDev,DWORD dwMaxStaticObjectNum,DWORD dwMaxMaterialNum);	

	BOOL					GetLightTexturePatch(CMeshObject** ppMeshObj,CLightTexture** ppLightTexture,TEXTURE_PLANE** ppTexturePlane,PATCH** ppPatch,POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex);
	void					SetGeometry(CoGeometry*	pGeometry) {m_pGeometry = pGeometry;}
	void					SetIndexInGeometry(DWORD dwIndex) {m_dwIndexInGeometry = dwIndex;}
	COLLISION_MODEL_DESC*	CreateCollisionDesc(CMeshFlag flag);
	
	void					ReleaseMaterials();
	DWORD					WriteCollisionMesh(FILE* fp);
	DWORD					ReadCollisionMesh(void* pFP);

	CoStaticModel();
	~CoStaticModel();


};

#endif


