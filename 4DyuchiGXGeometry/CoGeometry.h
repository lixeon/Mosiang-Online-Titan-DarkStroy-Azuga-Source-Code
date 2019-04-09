#pragma once

#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "../4DyuchiGRX_common/IFileStorage.h"
#include "SimpleMemoryPool.h"
#include "model.h"
#include "CoStaticModel.h"
#include "HeightField.h"
#include "ModelRefManager.h"
#include "MaterialLib.h"

class CoHeightField;
enum FILE_ITEM_TYPE
{
	FILE_ITEM_TYPE_MODEL		= 0,
	FILE_ITEM_TYPE_MOTION		= 1,
	FILE_ITEM_TYPE_MTL			= 2,
	
	FILE_ITEM_TYPE_STATIC_MODEL = 3,
	FILE_ITEM_TYPE_HFIELD		= 4,
	FILE_ITEM_TYPE_SPRITE		= 5,
	
};
enum FILE_HANDLE_FLAG
{
	FILE_HANDLE_FLAG_PRELOADED						= 0x00000001,
	FILE_HANDLE_FLAG_PRELOADED_BUT_DISABLE_UNLOAD	= 0x00000002
};
struct FILE_HANDLE
{
	void*				pSearchHandle;
	void*				pFileItem;
	DWORD				dwFlag;
	FILE_ITEM_TYPE		type;
};

#define MAX_SENDER_POINT_PER_SHADOWLIGHT 32

struct SHADOW_SENDER
{
	DWORD			dwPointNum;
	VECTOR3			v3LightPoint;
	BOUNDING_SPHERE	sphere[MAX_SENDER_POINT_PER_SHADOWLIGHT];

	
};

class CoGeometry : public I4DyuchiGXGeometry
{
	VBHASH_HANDLE				m_pFileNameHash;
	FILE_HANDLE*				m_pFileList;
	CSimpleMemoryPool			m_LightIndexPool;
	CSimpleMemoryPool			m_MatrixPool;

	CModelRefManager			m_ModelRefManager;
	DWORD						m_dwMaxFileNum;
	DWORD						m_dwFileNum;
	DWORD						m_dwRefCount;
	I4DyuchiGXRenderer*			m_pRenderer;
	I4DyuchiFileStorage*		m_pFileStorage;
	INDEXCR_HANDLE				m_pIC;

	HMODULE						m_hModGFunc;
	ITEMQ_HANDLE				m_pRenderQ;
	
	BOOL						m_bIsBeginRender;

	LIGHT_DESC					m_RTLightList[MAX_REALTIME_LIGHT_NUM];
	DWORD						m_dwDynamicRTLightNum;
	DWORD						m_dwStaticRTLightNum;

	LIGHT_DESC					m_spotLightList[MAX_SPOT_LIGHT_NUM_PER_SCENE];
	VIEW_VOLUME					m_vvSpot[MAX_SPOT_LIGHT_NUM_PER_SCENE];
	DWORD						m_dwSpotLightNum[3];	//0은 이미지 프로젝션용, 1은 그림자용,2는 거울용
	DWORD						m_dwTotalSpotLightNum;	// 총 스팟라이트 개수
	
	DWORD						m_dwLightFlag;
	DWORD						m_dwShadowFlag;
	DWORD						m_dwShadowTexSize;
	DWORD						m_dwDrawDebugFlag;
	DWORD						m_dwMaxShadowNum;

	WORD						m_wScreenWidth;
	WORD						m_wScreenHeight;
	SHORT_RECT					m_rcWindow;

	BOOL						m_bViewVolumeClip;
	static MATRIX4				m_matIdentity;

	VECTOR3						m_v3ShadowSenderPos;
	// 뷰관련..
	BOOL						m_bUpIsValid;		// SetCamera 할때 Up이 valid인가 아닌가 판별. -_-;

//	VIEWPORT					m_Viewport[MAX_VIEWPORT_NUM];
	VIEWPORT*					m_pViewport;
	DWORD						m_dwViewportNum;
	
	DWORD						m_dwTickCount;		// 단위는 초
	BOOL						m_bGameFrame;
	DWORD						m_dwCurrentViewportIndex;
	float						m_fRSForLoadResource;
	BOOL						m_AXIS_ALIGN_OK;
	BOOL						m_bShadowUpdatePerFrame;

	SHADOW_SENDER				m_ShadowSender[MAX_SPOT_LIGHT_NUM_PER_SCENE];	

	//trustpak 2005/06/24

	DWORD						m_dwLodUsingMode;
	DWORD						m_dwLODUsingLevel;

	///

	void						ResourceCheck();
public:
	CModelRefManager*			GetModelRefManager() {return  &m_ModelRefManager;}

	LIGHT_INDEX_DESC*			AllocLightIndex(DWORD dwIndexNum) {return (LIGHT_INDEX_DESC*)m_LightIndexPool.Alloc(dwIndexNum);}
	
	LIGHT_DESC*					GetDynamicRTLightDescBuffer()		{return m_RTLightList;}
	DWORD						GetDynamicRTLightNum()			{return m_dwDynamicRTLightNum;}
	
	LIGHT_DESC*					GetStaticRTLightDescBuffer()		{return m_RTLightList+STATIC_RTLIGHT_START_INDEX;}
	DWORD						GetStaticRTLightNum()			{return m_dwStaticRTLightNum;}

	CMotion*				 	LoadMotion(char* szFileName);
	CMaterialLib*				LoadMaterial(char* szFileName);
	BOOL						DeleteFileItem(DWORD dwIndex);
	BOOL						UnloadPreLoadedItem(DWORD dwIndex);
	BOOL						DisableUnloadPreLoadedResource(DWORD dwIndex);
	BOOL						EnableUnloadPreLoadedResource(DWORD dwIndex);

	DWORD						AddSpotLight(LIGHT_DESC* pLight,SPOT_LIGHT_TYPE  type);
//	void						SetSpotLight(LIGHT_DESC* pLight,DWORD dwLightIndex,BOOL bOrtho);
	
	
	VIEW_VOLUME*				GetSpotLightViewVolume(DWORD dwIndex) {return m_vvSpot+dwIndex;}
	DWORD						GetSpotLightNum()						{return m_dwTotalSpotLightNum;}
	LIGHT_DESC*					GetSpotLightDesc(DWORD dwIndex)			{return m_spotLightList+dwIndex;}




	// external interface
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL			__stdcall	Initialize(I4DyuchiGXRenderer* pRenderer,I4DyuchiFileStorage* pFileStorage,DWORD dwViewportNum,DWORD dwMaxFileNum,DWORD dwMaxBucketNum,ErrorHandleProc pErrorHandleFunc);
	ULONG			__stdcall	LoadModel(I3DModel** ppModel,char* szFileName,DWORD dwFlag);
	BOOL			__stdcall	IsLoadedModel(I3DModel** ppModel,char* szFileName);
	ULONG			__stdcall	CreateHeightField(IHeightField** ppHeightField,DWORD dwFlag);
	ULONG			__stdcall	CreateStaticModel(I3DStaticModel** ppStaticModel,DWORD dwMaxObjectNum,DWORD dwMaxMtlNum,DWORD dwFlag);
	ULONG			__stdcall	CreateSkyMesh(ISkyMesh** ppSkyMesh,char* szFileName,DWORD dwFlag);

	BOOL			__stdcall	BeginRender(DWORD dwViewportIndex,DWORD dwColor,DWORD dwFlag);
	void			__stdcall	EndRender();
	void			__stdcall	Present(HWND hWnd);
	
	BOOL			__stdcall 	RenderModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag);
	BOOL			__stdcall	RenderStaticModel(I3DStaticModel* pModel,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwFlag);
	BOOL			__stdcall	RenderHField(IHeightField* pHField,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag);
	BOOL			__stdcall	RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag);
	BOOL			__stdcall	RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag);
	BOOL			__stdcall	RenderLight(LIGHT_DESC* pLightDesc,DWORD dwAlphaFlag,DWORD dwRenderFlag);
	void			__stdcall	RenderSphere(VECTOR3* pv3Point,float fRs,DWORD dwColor);
	void			__stdcall	RenderBox(VECTOR3* pv3Oct,DWORD dwColor);
	BOOL			__stdcall	RenderSkyMesh(ISkyMesh* pSkyMesh,DWORD dwFlag);

	BOOL			__stdcall	AddMirror(LIGHT_DESC* pMirror,DWORD dwFlag);
	BOOL			__stdcall	AddLight(LIGHT_DESC* pLight,DWORD dwFlag);
	DWORD			__stdcall	CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName);
	BOOL			__stdcall	DeleteDynamicLight(DWORD dwIndex);

	
	void			__stdcall	SetShadowFlag(DWORD dwFlag);
	DWORD			__stdcall	GetShadowFlag();
	
	void			__stdcall	SetLightFlag(DWORD dwFlag);
	DWORD			__stdcall	GetLightFlag();

	void			__stdcall	GetProjectionMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex);
	void			__stdcall	GetViewMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex);
	void			__stdcall	GetViewport(SHORT_RECT* pClipRect,WORD* pwWidth,WORD* pwHeight,DWORD dwViewportIndex);
	void			__stdcall	SetViewport(SHORT_RECT* pRect,DWORD dwViewportIndex);

	void			__stdcall	GetViewVolume(VIEW_VOLUME* pVolume,DWORD dwViewportIndex);
	BOOL			__stdcall	GetRayWithScreenCoord(VECTOR3* pv3Pos,VECTOR3* pv3Dir,POINT* pt,DWORD dwViewportIndex);
	BOOL			__stdcall	ResetCamera(VECTOR3* pv3Eye,float fNear,float fFar,float fFovX,DWORD dwViewportIndex);
	BOOL			__stdcall	MoveCamera(VECTOR3* pv3Trans,DWORD dwViewportIndex);
	BOOL			__stdcall	SetCameraPos(VECTOR3* pv3Eye,DWORD dwViewportIndex);
	BOOL			__stdcall	SetCameraViewDistance(float fNear,float fFar,DWORD dwViewportIndex);
	BOOL			__stdcall	SetCameraFitCollisionMesh(COLLISION_MESH_OBJECT_DESC* pColMeshDesc,float fNear,float fFar,float fFovX,DWORD dwViewportIndex);
	BOOL			__stdcall	SetCameraFitModel(I3DModel* pModel,DWORD dwRefIndex,float fNear,float fFar,float fFovX,DWORD dwViewportIndex);

	void			__stdcall	SetFog(FOG_DESC* pFogDesc,DWORD dwViewportIndex);
	void			__stdcall	EnableFog(DWORD dwViewportIndex);
	void			__stdcall	DisableFog(DWORD dwViewportIndex);

	void			__stdcall	SetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex);
	void			__stdcall	GetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex);
	void			__stdcall	EnableDirectionalLight(DWORD dwViewportIndex);
	void			__stdcall	DisableDirectionalLight(DWORD dwViewportIndex);

	void			__stdcall	GetCameraDesc(CAMERA_DESC* pCamera,DWORD dwViewportIndex);
	void			__stdcall	SetCameraDesc(CAMERA_DESC* pCamera,DWORD dwViewportIndex);
	
	void			__stdcall	SetCameraAngleRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex);	/// 추가. 2002/02/05
	void			__stdcall	RotateCameraRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex);
	void			__stdcall	GetCameraAngleRad(VECTOR3* pv3Angle,DWORD dwViewportIndex);

	BOOL			__stdcall	GetIntersectPointWithScreenCoord(VECTOR3* pv3Pos,PLANE* pPlane,POINT* ptCursor,DWORD dwViewportIndex);
	
	
	void			__stdcall	SetAmbientColor(DWORD dwViewportIndex,DWORD dwColor);
	DWORD			__stdcall	GetAmbientColor(DWORD dwViewportIndex);
	
	void			__stdcall	SetEmissiveColor(DWORD dwViewportIndex,DWORD dwColor);
	DWORD			__stdcall	GetEmissiveColor(DWORD dwViewportIndex);
	
	void			__stdcall	EnableViewVolumeClip();
	void			__stdcall	DisableViewVolumeClip();
	
	void			__stdcall	SetDrawDebugFlag(DWORD dwFlag);
	DWORD			__stdcall	GetDrawDebugFlag();

	void			__stdcall	GetSystemStatus(SYSTEM_STATUS* pStatus);
	void			__stdcall	UpdateWindowSize();

	BOOL			__stdcall	PreLoadModel(char* szFileName);
	BOOL			__stdcall	PreLoadMotion(char* szFileName);
	BOOL			__stdcall	PreLoadMaterial(char* szFileName);
	void			__stdcall	UnloadAllPreLoadedItem(DWORD dwFlag);
	BOOL			__stdcall	DisableUnloadPreLoadedResource(char* szFileName);
	BOOL			__stdcall	EnableUnloadPreLoadedResource(char* szFileName);
				
	DWORD			__stdcall	GetReadCountInMOD(char* szFileName);
	DWORD			__stdcall	GetReadCountInSTM(char* szFileName);
	DWORD			__stdcall	GetReadCountInHField(char* szFileName);
	void			__stdcall	CalcViewVolume(VIEW_VOLUME* pVV,RECT* pRect,DWORD dwViewportIndex);
	void			__stdcall 	GetCameraEyePlane(PLANE* pPlane,DWORD dwViewportIndex);
	void			__stdcall	GetBillBoardMatrix(MATRIX4* pMat,DWORD dwViewportIndex);
	void			__stdcall	GetCameraAttachedMatrix(MATRIX4* pMat,DWORD dwViewportIndex,float fDist);	

	void			__stdcall	SetTickCount(DWORD dwTickCount,BOOL bGameFrame);
	void			__stdcall	SetRadiusForLoadResource(float fRS);
	void			__stdcall	SetShadowUpdatePerFrame(BOOL bSwitch);

	//trustpak 2005/06/24

	void			__stdcall	SetModelLODUsingMode(DWORD dwLodUsingMode, DWORD dwLevel = 0);
	DWORD			__stdcall	GetModelLODUsingMode(VOID) { return m_dwLodUsingMode; }
	DWORD			__stdcall	GetModelLodUsingLevel(VOID) { return m_dwLODUsingLevel; }

	///

	BOOL						IsLoadedMotion(CMotion** ppMotion,char* szFileName);
	BOOL						IsLoadedMaterial(CMaterialLib** ppMtlLib,char* szFileName);

	float						GetCameraFar(DWORD dwViewportIndex)	{return m_pViewport[dwViewportIndex].cameraDesc.fFar;}
	VECTOR3*					GetCameraEye(DWORD dwViewportIndex)	{return &m_pViewport[dwViewportIndex].cameraDesc.v3From;}
	VECTOR3*					GetCameraAt(DWORD dwViewportIndex)			{return &m_pViewport[dwViewportIndex].cameraDesc.v3To;}
	PLANE*						GetCameraEyePlane(DWORD dwViewportIndex)	{return &m_pViewport[dwViewportIndex].planeCameraEye;}
	MATRIX4*					GetBillBoardMatrix(DWORD dwViewportIndex)	{return &m_pViewport[dwViewportIndex].matForBillBoard;}

	VECTOR3*					GetCameraEye()			{return &m_pViewport[m_dwCurrentViewportIndex].cameraDesc.v3From;}
	VECTOR3*					GetCameraAt()			{return &m_pViewport[m_dwCurrentViewportIndex].cameraDesc.v3To;}
	
	MATRIX4*					GetBillBoardMatrix()	{return &m_pViewport[m_dwCurrentViewportIndex].matForBillBoard;}
	
	PLANE*						INL_GetCameraEyePlane()		{return &m_pViewport[m_dwCurrentViewportIndex].planeCameraEye;}
	MATRIX4*					INL_GetViewMatrix()	{return &m_pViewport[m_dwCurrentViewportIndex].matView;}
	DWORD						INL_GetDrawDebugFlag() {return m_dwDrawDebugFlag;}
	float						INL_GetCameraFar()	{return m_pViewport[m_dwCurrentViewportIndex].cameraDesc.fFar;}
	float						INL_GetRadiusForLoadResource()	{return m_fRSForLoadResource;}
	VECTOR3*					INL_GetCameraEye()	{return &m_pViewport[m_dwCurrentViewportIndex].cameraDesc.v3From;}
	VIEW_VOLUME*				INL_GetViewvolume()		{return &m_pViewport[m_dwCurrentViewportIndex].ViewVolume;}
	VIEWPORT*					INL_GetViewport(DWORD dwViewportIndex)	{return m_pViewport + dwViewportIndex;}
	I4DyuchiGXRenderer*			INL_GetRenderer() {return m_pRenderer;}
	
	void						SetShadowLightSenderSphere(BOUNDING_SPHERE* pSphere,DWORD dwIndex);
	

	CoGeometry();
	~CoGeometry();

};
