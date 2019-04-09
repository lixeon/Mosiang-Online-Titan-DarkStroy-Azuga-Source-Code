#pragma once

#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IFileStorage.h"
#include "../4DyuchiGXGFunc/global.h"


#include "GxLight.h"
#include "map.h"
#include "GXDecal.h"
#include "../4DyuchiGRX_myself97/MClipper.h"


struct MODEL_FILE_DESC
{
	DWORD			dwRefCount;
	void*			pSearchHandle;
	CoGXObject*		pGXObject;
	char			szFileName[MAX_NAME_LEN];


};

class CoGXObject;
class CoExecutive : public I4DyuchiGXExecutive
{
	DWORD						m_dwRefCount;

	I4DyuchiFileStorage*		m_pFileStorage;
	I4DyuchiGXRenderer*			m_pRenderer;
	I4DyuchiGXGeometry*			m_pGeometry;
	
	DWORD						m_dwViewportIndex;

	DWORD						m_dwInitialTick;
	DWORD						m_dwPrvFrameCount;
	DWORD						m_dwFrameCount;
	DWORD						m_dwTicksPerFrame;
	DWORD						m_dwGameFPS;

	DWORD						m_dwTickIncrease;			// 최근 게임프레임에서 다음게임프레임 전까지 경과한 틱, 0에서 m_dwTicksPerFrame 사이값이 들어있어야 한다. Process에서 갱신.

	BOOL						m_bCanDeleteObject;

	DWORD						m_dwMaxObjectNum;
	DWORD						m_dwMaxTriggerNum;
	DWORD						m_dwMaxDecalNum;

	DWORD						m_dwMaxMapObjectsNum;


	DWORD*						m_pdwIndexList;
	
	DWORD						m_dwRenderMode;
	DWORD						m_dwPickingMode;





	DPCQ_HANDLE					m_pDPCQ;
	STMPOOL_HANDLE				m_pStaticPoolGXObject;
	STMPOOL_HANDLE				m_pStaticPoolGXLight;
	STMPOOL_HANDLE				m_pStaticPoolGXDecal;

	ITEMTABLE_HANDLE			m_pIndexItemTableGXObject;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXLight;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXTrigger;
	ITEMTABLE_HANDLE			m_pIndexItemTableGXDecal;
	
		
	QBHASH_HANDLE				m_pIDHash;
	VBHASH_HANDLE				m_pFileNameHash;

	I3DModel*					m_pModelList[SYMBOL_TYPE_NUM];

//	CoGXObject*					m_pTracedGXObject;
	

	
	
	
	DWORD						m_dwCurrentGXLightNum;
	CoGXLight**					m_ppGXLightList;
	CoGXObject*					m_pCameraAttatchedGXObject;
	float						m_fDistCameraFront;

	FLOAT_RECT					m_frcMapBoundingRect;
	CGXMap*						m_pgxMap;
	CoGXObject*					m_pDummyGXObject;
	CoGXLight*					m_pDummyGXLight;
	CGXDecal*					m_pDummyGXDecal;
	
	I3DModel*					m_pPreLoadedModelList;
	DWORD						m_dwPreLoadedModelNum;

	void*						m_pPackFileHandleList[MAX_PACK_FILE_NUM];
	DWORD						m_dwPackFilesNum;



	int							LoadModelData(MODEL_HANDLE* pModelHandle,void* pFP);
	int							PreLoadModelData(void* pFP);

//trustpak 2005/06/28
public:
	DWORD						LoadGXObject(MODEL_HANDLE* pModelHandle,DWORD dwMaxModelNum,char* szFileName);
private:
	CoGXObject*					AddGXObject(MODEL_HANDLE* pModelHandle,DWORD dwModelNum,GXSchedulePROC pProc,DWORD dwFlag);
	
	// gxobject에서 충돌처리할때 쓰일 헬퍼.		2002/05/29
	DBG_COLLISION_INFO			m_CollisionInfo;

	DWORD						m_dwBuildTreeFlag;		// octree, bsp tree 등의 존재 여부. BUILD_TREE_FLAG타입.

	HMODULE						m_hGeometry;
	HMODULE						m_hRenderer;
	HMODULE						m_hStorage;


	void						SetCanDelete()				{m_bCanDeleteObject = TRUE;}
	void						SetCannotDelete()			{m_bCanDeleteObject = FALSE;}
	BOOL						IsCanDeleteObject()			{return m_bCanDeleteObject;}

	// Decal;
//	CGXDecal*					m_pDecalBuff;
//	int*						m_pDecalClipperIndexBuff;
	DWORD						m_dwCurrentDecalCount;
	
	DWORD						m_dwRenderFrameCount;
	void						ResourceCheck();

	AfterInterpolationCallBack	m_pfAfterInterpolationCallBack;

//	void						RenderDecal();
	
	BOOL							Initialize(I4DyuchiGXGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL							InitializeFileStorageCommon(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
public:
#ifdef _DEBUG
	void						CheckHandle(GXMAP_OBJECT_HANDLE gxh);
	void						CheckGXLightsList(CoGXLight* pInLight);
	void						CheckGXObjectsList(CoGXObject* pInObject);
	void						CheckGXTriggersList(CoGXEventTrigger* pInTrigger);
	void						CheckGXDecalsList(CGXDecal* pInDecal);
#endif

//	MCollisionHelper*			GetCollisionHelper(){return &m_Collision;}
	I4DyuchiGXRenderer*			INL_GetRenderer()	{return m_pRenderer;}
	I4DyuchiGXGeometry*			INL_GetGeometry()	{return m_pGeometry;}
	CGXMap*						GetGXMap()	{return m_pgxMap;}
	void						GetCameraDesc(CAMERA_DESC*	pCameraDesc) {m_pGeometry->GetCameraDesc(pCameraDesc,m_dwViewportIndex);}
	void						GetCameraEyePlane(PLANE* pPlane) {m_pGeometry->GetCameraEyePlane(pPlane,m_dwViewportIndex);}

	I3DModel*					GetSymbolModel(SYMBOL_TYPE type) {return m_pModelList[type];}

	QBHASH_HANDLE				GetIDHash()	{return m_pIDHash;}
	void						DeleteModelFileDesc(MODEL_FILE_DESC* pModelFileDesc);
//	CoGXObject*					GetTracedGXObject() {return m_pTracedGXObject;}

	// external interface
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	I4DyuchiGXRenderer*	__stdcall	GetRenderer();
	I4DyuchiGXGeometry*	__stdcall	GetGeometry();

	HRESULT				__stdcall	GetRenderer(I4DyuchiGXRenderer** ppRenderer);		// Release필요함
	HRESULT				__stdcall	GetGeometry(I4DyuchiGXGeometry** ppGeometry);		// Release필요함
	HRESULT				__stdcall	GetFileStorage(I4DyuchiFileStorage** ppFileStorage);
		
	BOOL				__stdcall	InitializeWithoutRegistry(char* szGeometryFileName,char* szRendererFileName,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL				__stdcall	Initialize(HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc);
	BOOL				__stdcall	InitializeFileStorageWithoutRegistry(char* szFileName,DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
	BOOL				__stdcall	InitializeFileStorage(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum);
	void				__stdcall	SetViewport(DWORD dwViewportIndex);

	DWORD				__stdcall	IsValidHandle(GXMAP_OBJECT_HANDLE gxh);


	DWORD				__stdcall	GXOProcess(GXOBJECT_HANDLE gxo,DWORD dwFrameInc);
	BOOL				__stdcall	GXORender(GXOBJECT_HANDLE gxo);
	
	void				__stdcall	GXOSetCurrentFrame(GXOBJECT_HANDLE gxh,DWORD dwFrame);
	void				__stdcall	GXOSetCurrentMotionIndex(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex);
	BOOL				__stdcall	GXOGetMotionDesc(GXOBJECT_HANDLE gxh,MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwModelIndex);
	DWORD				__stdcall	GXOGetMotionNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex);
	DWORD				__stdcall	GXOGetCurrentFrame(GXOBJECT_HANDLE gxh);
	DWORD				__stdcall	GXOGetCurrentMotionIndex(GXOBJECT_HANDLE gxh);
	
	void				__stdcall	GXOSetCurrentMaterialIndex(GXOBJECT_HANDLE gxh,DWORD dwMaterialIndex);
	DWORD				__stdcall	GXOGetMaterialNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex);
	DWORD				__stdcall	GXOGetCurrentMaterialIndex(GXOBJECT_HANDLE gxh);
	

	void				__stdcall	GXOSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos, BOOL bDoInterpolation);
	void				__stdcall	GXOMovePosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);
	void				__stdcall	GXOGetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);
	void				__stdcall	GXOSetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float fRad);
	void				__stdcall	GXOSetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angles /* in */);

	void				__stdcall	GXOGetInterpolatedPosition( GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos); // 보간되어서 랜더링 되는 포지션을 출력. 실제 GXO의 위치는 아니다. 카메라위치 갱신할 때 쓰면 좋을듯.
	void				__stdcall	GXOChangeDirection(GXOBJECT_HANDLE gxh,float fRad);
	void				__stdcall	GXOGetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float* pfRad);
	void				__stdcall	GXOGetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angle /* out */);
	void				__stdcall	GXOSetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale);

	void				__stdcall	GXOGetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale);
	
//	void				__stdcall	GXOSetAcceleratedVelocity( GXOBJECT_HANDLE gxh, VECTOR3* pv3AcceleratedVelocity);
	BOOL				__stdcall	GXOIsLanding( GXOBJECT_HANDLE gxh);
//	void				__stdcall	GXOSetApplyVelocityFlag( GXOBJECT_HANDLE gxh, BOOL bApply);		// 매 프레임마다 속도벡터를 적용할 것인가?


	void				__stdcall	GXOMoveForward(GXOBJECT_HANDLE gxh,float fDistance);
	void				__stdcall	GXOEnableHFieldApply(GXOBJECT_HANDLE gxo);
	void				__stdcall	GXODisableHFieldApply(GXOBJECT_HANDLE gxo);
	BOOL				__stdcall	GXOIsHFieldApply(GXOBJECT_HANDLE gxo);
	
	void				__stdcall	GXOEnableAsEffect(GXOBJECT_HANDLE gxo);
	void				__stdcall	GXODisableAsEffect(GXOBJECT_HANDLE gxo);
	BOOL				__stdcall	GXOIsAsEffect(GXOBJECT_HANDLE gxo);

	void				__stdcall	GXOSetZOrder(GXOBJECT_HANDLE gxo,int iZOrder);

	void				__stdcall	GXOEnableSelfIllumin(GXOBJECT_HANDLE gxo);
	void				__stdcall	GXODisableSelfIllumin(GXOBJECT_HANDLE gxo);
	BOOL				__stdcall	GXOIsEnableSelfIllumin(GXOBJECT_HANDLE gxo);

	void				__stdcall	EstimatedMoveForward(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos,float fDistance);

	BOOL				__stdcall	GXOAttach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,char* szObjName);
	BOOL				__stdcall	GXOAttachWithModelIndex(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,DWORD dwModelIndex,char* szObjName);
	BOOL				__stdcall	GXODetach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE child_gxh);
	BOOL				__stdcall	GXOAttachLight(GXOBJECT_HANDLE gxoh,char* szObjName,GXLIGHT_HANDLE gxlh,VECTOR3* pv3Offset,DWORD dwAttachType);
	BOOL				__stdcall	GXODetachLight(GXOBJECT_HANDLE gxoh,GXLIGHT_HANDLE child_gxlh);		
	BOOL				__stdcall	GXOAttachCameraFront(GXOBJECT_HANDLE gxo,float fDist);
	BOOL				__stdcall	GXODetachCameraFront(GXOBJECT_HANDLE gxo);
	ULONG				__stdcall	GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex);

	GXOBJECT_HANDLE		__stdcall	GXOGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	
	DWORD				__stdcall	GXOGetMultipleObjectWithScreenCoordRect(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,RECT* pRect,DWORD dwViewportIndex,DWORD dwFlag);
	DWORD				__stdcall	GXOGetMultipleObjectWithScreenCoord(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	BOOL				__stdcall	GXOReplaceModel(GXOBJECT_HANDLE gxh,DWORD dwModelIndex,char* szFileName);

	
	void				__stdcall	GXOGetCollisionMesh(GXOBJECT_HANDLE gxh,COLLISION_MESH_OBJECT_DESC* pColMeshDesc);

	DWORD				__stdcall	GXOGetObjectIndex(GXOBJECT_HANDLE gxh,char* szObjName,DWORD dwModelIndex);
	BOOL				__stdcall	GXOGetWorldMatrixPerObject(GXOBJECT_HANDLE gxh,MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex);

	DWORD				__stdcall	GXOGetAllObjectsWitLoadMapScript(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum);
	


	void				__stdcall 	DisableRender(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall 	EnableRender(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	DisableSchedule(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	EnableSchedule(GXMAP_OBJECT_HANDLE gxh);
	BOOL				__stdcall	IsRenderable(GXMAP_OBJECT_HANDLE gxh);

	void				__stdcall	SetPickType(GXMAP_OBJECT_HANDLE	gxh,PICK_TYPE type);
	PICK_TYPE			__stdcall	GetPickType(GXMAP_OBJECT_HANDLE	gxh);

	void				__stdcall	SetRenderFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD				__stdcall	GetRenderFlag(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	SetAlphaFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD				__stdcall	GetAlphaFlag(GXMAP_OBJECT_HANDLE gxh);
	void*				__stdcall	GetData(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	SetData(GXMAP_OBJECT_HANDLE gxh,void* pData);
	DWORD				__stdcall	GetID(GXMAP_OBJECT_HANDLE gxh);
	BOOL				__stdcall	SetID(GXMAP_OBJECT_HANDLE gxh,DWORD dwID);
	void 				__stdcall	ReleaseID(GXMAP_OBJECT_HANDLE gxh);
	

	GXMAP_OBJECT_HANDLE __stdcall	GetGXMapObjectWithID(DWORD dwID);
	
	void				__stdcall	SetPropertyFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag);
	DWORD				__stdcall	GetPropertyFlag(GXMAP_OBJECT_HANDLE gxh);

	void				__stdcall	EnablePick(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	DisablePick(GXMAP_OBJECT_HANDLE gxh);
	
	void				__stdcall	EnableUpdateShading(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	DisableUpdateShading(GXMAP_OBJECT_HANDLE gxh);

	void				__stdcall	LockTransform(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	UnLockTransform(GXMAP_OBJECT_HANDLE gxh);
	BOOL				__stdcall	IsLockTransform(GXMAP_OBJECT_HANDLE gxh);

	void				__stdcall	EnableSendShadow(GXMAP_OBJECT_HANDLE gxh);
	void				__stdcall	DisableSendShadow(GXMAP_OBJECT_HANDLE gxh);

	BOOL				__stdcall	IsInViewVolume(GXMAP_OBJECT_HANDLE gxh);
	BOOL				__stdcall	IsCollisionWithScreenCoord(GXOBJECT_HANDLE gxh,VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	

	

	DWORD				__stdcall	GetFrameCount();


	
	
		
	BOOL				__stdcall	PreCreateLight(char* szFileName,DWORD dwFlag);
	DWORD				__stdcall	PreLoadGXObject(char* szFileName);



	GXOBJECT_HANDLE		__stdcall	CreateGXObject(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag);
	GXLIGHT_HANDLE		__stdcall	CreateGXLight(LIGHT_DESC* pDesc,GXLightSchedulePROC pProc,void* pData,DWORD dwProjTexIndex,MATERIAL* pMtl,DWORD dwFlag);
	GXTRIGGER_HANDLE	__stdcall	CreateGXEventTrigger(GXSchedulePROC pProc,void* pData,DWORD dwFlag);
	GXMAP_HANDLE		__stdcall	CreateGXMap(GXMapSchedulePROC pProc,void* pData,DWORD dwFlag);
	
	void				__stdcall	UnloadPreLoadedGXObject(GXOBJECT_HANDLE gxo);
	void				__stdcall	UnloadAllPreLoadedGXObject(DWORD dwFlag);

	BOOL				__stdcall	BeginBuildMap(DWORD dwFlag);
	BOOL				__stdcall	InsertHFieldToGXMap(IHeightField* pHField);
	BOOL				__stdcall	InsertStaticModelTOGXMap(I3DStaticModel* pModel);
	BOOL				__stdcall	EndBuildMap(float fTop,float fBottom);
	BOOL				__stdcall	RebuildMap(float fTop,float fBottom);
	BOOL				__stdcall	GetWorldBoundingBox( MAABB* pWorldBox);
	
	
	
	DWORD				__stdcall	GetFramePerSec();
	void				__stdcall	SetFramePerSec(DWORD dwFrame);
	
	void				__stdcall	GXLSetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc);
	void				__stdcall	GXLGetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc);
	void				__stdcall	GXLSetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	void				__stdcall	GXLGetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	void				__stdcall	GXLMovePosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos);
	GXLIGHT_HANDLE		__stdcall	GXLGetLightWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	void				__stdcall	GXLDisableStaticShadow(GXLIGHT_HANDLE gxl);
	void				__stdcall	GXLEnableStaticShadow(GXLIGHT_HANDLE gxl);
	BOOL				__stdcall	GXLIsDisableStaticShadow(GXLIGHT_HANDLE gxl);
	
	void				__stdcall	GXLEnableDynamicLight(GXLIGHT_HANDLE gxl);
	void				__stdcall	GXLDisableDynamicLight(GXLIGHT_HANDLE gxl);
	BOOL				__stdcall	GXLIsEnableDynamicLight(GXLIGHT_HANDLE gxl);

	void				__stdcall	GXTSetPosition(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Pos);
	void				__stdcall	GXTSetScale(GXTRIGGER_HANDLE gxh,VECTOR3* pv3PScale);
	void				__stdcall	GXTSetRotation(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Rot);
	void				__stdcall	GXTGetEventTriggerDesc(GXTRIGGER_HANDLE gxh,EVENT_TRIGGER_DESC* pEVDesc);
	GXTRIGGER_HANDLE	__stdcall	GXTGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);

	BOOL				__stdcall	GXMGetCollisionPointWithScreenCoord(DWORD* pdwObjIndex,VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag);
	BOOL				__stdcall	GXMGetHFieldCollisionPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir);
	BOOL				__stdcall	GXMGetHFieldCollisionPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor);
	BOOL				__stdcall	GXMGetHFieldHeight(float* py,float x,float z);
	BOOL				__stdcall	GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc);
	
	void				__stdcall	GXMSetHFieldAlphaFlag(DWORD dwAlpha);
	void				__stdcall	GXMSetSTMAlphaFlag(DWORD dwAlpha);

	
	BOOL				__stdcall	SetSymbol(SYMBOL_TYPE type,char* szFileName);
	void				__stdcall	SetRenderMode(DWORD dwRenderMode);
	VOID				__stdcall	SetPickingMode(DWORD dwPickingMode);		// 툴이냐 게임이냐를 지정하는 것으로,
																				// 게임에서의 피킹 지정과는 관련이 없다. 
																				// 위의 SetRenderMode의 shape을 모방하여 만들다 보니 이름이 이렇게 된 것 임.	
	BOOL				__stdcall	DisableRenderAllStaticModels(VOID);
	BOOL				__stdcall	EnableRenderAllStaticModels(VOID);


	// 충돌처리 함수군. 2002/03/06
	void						__stdcall	GXOSetBoundingVolume( GXOBJECT_HANDLE gxh, BOUNDING_VOLUME* pBV);


	void				__stdcall	GXOMovePositionWithCollide(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos);		// 2002/05/22
	
	DWORD				__stdcall	GetGXObjectsNum();
	GXOBJECT_HANDLE		__stdcall	GetGXObjectWithSeqIndex(DWORD dwIndex);

	DWORD				__stdcall	GetGXLightsNum();
	GXLIGHT_HANDLE		__stdcall	GetGXLightWithSeqIndex(DWORD dwIndex);

	DWORD				__stdcall	GetGXEventTriggersNum();
	GXTRIGGER_HANDLE	__stdcall	GetGXEventTriggerWithSeqIndex(DWORD dwIndex);
	BOOL				__stdcall	LoadMapScript(char* szFileName,LOAD_CALLBACK_FUNC pFunc,DWORD dwFlag);
	void				__stdcall	DeleteAllGXMapObjectsWitLoadMapScript();


	void				__stdcall	Render();
	BOOL				__stdcall	RenderCameraFrontObject(float fDist);
	DWORD				__stdcall	Process();
	

	DWORD				__stdcall	Run(DWORD dwBackColor,GX_FUNC pfBeforeRenderFunc,GX_FUNC pfAfterRenderFunc,DWORD dwFlag);
	BOOL				__stdcall	SetCameraFitGXObject(GXOBJECT_HANDLE gxo,float fNear,float fFar,float fFov,DWORD dwViewportIndex);
	void				__stdcall	SetHFieldDetail(DWORD dwDetail);


	BOOL				__stdcall	DeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL				__stdcall	DeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL				__stdcall	DeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	
	BOOL				__stdcall	DeleteGXMap(GXMAP_HANDLE gxm);
	BOOL				__stdcall	DeleteAllGXObjects();
	BOOL				__stdcall	DeleteAllGXLights();
	BOOL				__stdcall	DeleteAllGXEventTriggers();

		// 즉시 지우는 함수들..
	BOOL				__stdcall	ImmDeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL				__stdcall	ImmDeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL				__stdcall	ImmDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	BOOL				__stdcall	ImmDeleteGXDecal( GXDECAL_HANDLE gxh);

	// 나중에 지우는 함수들..
	BOOL				__stdcall	DefDeleteGXObject(GXOBJECT_HANDLE gxh);
	BOOL				__stdcall	DefDeleteGXLight(GXLIGHT_HANDLE gxh);
	BOOL				__stdcall	DefDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh);
	BOOL				__stdcall	DefDeleteGXDecal(GXTRIGGER_HANDLE gxh);

	// 데칼 2003.01.15
	GXDECAL_HANDLE		__stdcall	CreateGXDecal( DECAL_DESC* pDesc, GXDecalSchedulePROC pProc, void* pData, DWORD dwFlag);
	BOOL				__stdcall	DeleteGXDecal( GXDECAL_HANDLE gxh);
//	BOOL				__stdcall	DeleteAllGXDecal();

	BOOL				__stdcall	GetMapReadCount(char* szFileName);
	
	BOOL				__stdcall	GXOInitializeIllusionEffect(GXOBJECT_HANDLE gxh,DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag);
	void				__stdcall	GXOBeginIllusionEffect(GXOBJECT_HANDLE gxh);
	void				__stdcall	GXOEndIllusionEffect(GXOBJECT_HANDLE gxh);
	DWORD				__stdcall	GXOGetAttachedGXObjects(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum);
	
	void				__stdcall	GXOSetScheduleProc(GXOBJECT_HANDLE gxo,GXSchedulePROC pProc);
	GXSchedulePROC		__stdcall	GXOGetScheduleProc(GXOBJECT_HANDLE gxo);
	
	void				__stdcall	GXOSetEffectIndex(GXOBJECT_HANDLE gxo,DWORD dwEffectIndex);
	DWORD				__stdcall	GXOGetEffectIndex(GXOBJECT_HANDLE gxo);
	
	void				__stdcall	SetAfterInterpolation( AfterInterpolationCallBack pfCallBack);
	// 익스큐티브::Run()에서 위치보간과 랜더링 사이에 호출되는 콜백.
	// 카메라를 캐릭터에 붙이거나할 때 쓰면 유용하다. 해제 하려면 NULL을 설정한다.

	// 현재의 게임 프레임을 리턴한다.
	DWORD				__stdcall	GetCurrentGameFrame(void);

//	void				__stdcall	GXOGetVelocity( GXOBJECT_HANDLE gxo, VECTOR3* pOutVelocity);
	ULONG				__stdcall	GXMGetStaticModel( I3DStaticModel** ppStaticModel);		
	ULONG				__stdcall	GXMGetHField(IHeightField** ppHField);
	
	void				__stdcall	GXODisableUnloadPreLoaded(GXOBJECT_HANDLE gxh);
	void				__stdcall	GXOEnableUnloadPreLoaded(GXOBJECT_HANDLE gxh);

	void				__stdcall	GXOGetLastVelocityAfterCollisionTest( GXOBJECT_HANDLE gxh, VECTOR3* pLastVelocity);

	void				__stdcall	GXOEnableScaleOfAttachedModel(GXOBJECT_HANDLE gxo);
	void				__stdcall	GXODisableScaleOfAttachedModel(GXOBJECT_HANDLE gxo);


	//trustpak 2005/06/24

	void			__stdcall	SetModelLODUsingMode(DWORD dwLodUsingMode, DWORD dwLevel = 0);
	DWORD			__stdcall	GetModelLODUsingMode(VOID);
	DWORD			__stdcall	GetModelLodUsingLevel(VOID);

	///

	CoExecutive();
	~CoExecutive();
};

