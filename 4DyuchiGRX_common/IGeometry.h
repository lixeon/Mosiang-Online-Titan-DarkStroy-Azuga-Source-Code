#pragma once

#include "stdafx.h"
#include "IGeometry_GUID.h"
#include "IRenderer.h"
#include "IFileStorage.h"
#include "typedef.h"

interface I3DModel;


interface I3DStaticModel : public IUnknown
{	
	virtual BOOL			__stdcall	BeginCreateMesh(DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	AddModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pTransformMatrix,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	EndCreateMesh() = 0;

	virtual BOOL			__stdcall	DeleteObject(DWORD dwIndex) = 0;

	virtual COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc(DWORD dwObjIndex) = 0;
	
	virtual	void			__stdcall	GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc) = 0;
	virtual DWORD			__stdcall	CreateIVertexList(IVERTEX** ppVertex) = 0;
	virtual DWORD			__stdcall	CreateIVertexListWithIndex(IVERTEX** ppVertex,DWORD dwObjIndex) = 0;
	virtual void			__stdcall	ReleaseIVertexList(IVERTEX*	pVertex) = 0;
	virtual DWORD			__stdcall	GetObjectIndex(char* szObjName) = 0;
	virtual void			__stdcall	Optimize() = 0;	
	virtual DWORD			__stdcall	GetObjectNum() = 0;
	virtual	DWORD			__stdcall	GetObjectIndex(DWORD dwSeqIndex) = 0;

	virtual BOOL			__stdcall	BeignShadeLightMapObject(DWORD dwColor) = 0;
	virtual BOOL			__stdcall	ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc) = 0;
	virtual void			__stdcall	EndShadeLightMapObject() = 0;

	virtual BOOL			__stdcall	ReadFile(char* szFileName,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag) = 0;
	virtual	BOOL			__stdcall	WriteFile(char* szFileName) = 0;
	virtual	DWORD			__stdcall	GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual	BOOL			__stdcall	IsCollisionMeshObject(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwObjIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	SetLightTexel(POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwColor,DWORD dwViewportIndex) = 0;
	virtual BOOL			__stdcall	GetLightTexel(DWORD* pdwColor,POINT* ptCursor,DWORD dwObjIndex,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex) = 0;
	virtual BOOL			__stdcall	RenderCollisionStaticModel(DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	RunManageResource(DWORD dwViewportIndex) = 0;

	
	virtual	BOOL			__stdcall	CollisionTestAAELLIPSOID( VECTOR3* pOut, VECTOR3* pOutLastVelocity, BOOL* pbOutLanding, CollisionTestCallBackProcedure CallBack, AAELLIPSOID* pEllipsoid, VECTOR3* pTo, DWORD dwOutputIndex)=0;
	virtual	BOOL			__stdcall	CollisionTestLine( float* pOutT, VECTOR3* pFrom, VECTOR3* pTo, DWORD dwOutputIndex)=0;

	virtual void			__stdcall	EnableRecvShadow(BOOL bEnable) = 0;

	virtual	void			__stdcall	SetSimultaneousAccessCount( DWORD dwCount)=0;		// 동시에 호출할 갯수. 아마 쓰레드만큼.?
};

interface I3DModel : public IUnknown
{
	virtual BOOL			__stdcall	GetCollisionMeshObjectDesc(COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwModelRefIndex,DWORD dwObjIndex) = 0;
	virtual BOOL			__stdcall	GetCollisionModelDesc(COLLISION_MODEL_DESC* pColMeshModelDesc,DWORD dwModelRefIndex) = 0;
	virtual void			__stdcall	UpdateColMeshDesc(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag) = 0;

	virtual DWORD			__stdcall	Reference() = 0;
	virtual void			__stdcall	UnReference(DWORD dwRefIndex) = 0;
	virtual BOOL			__stdcall	Render(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag) = 0;

	virtual BOOL			__stdcall 	CreateMotionList(DWORD dwNum,DWORD dwRefIndex) = 0;
	virtual BOOL			__stdcall	CreateMaterialList(DWORD dwNum,DWORD dwRefIndex) = 0;

	virtual BOOL			__stdcall	Initialize(I4DyuchiGXRenderer* pDev,DWORD dwFlag) = 0;
	virtual DWORD			__stdcall	AddMotion(char* szFileName,DWORD dwRefIndex,DWORD dwMotionIndex) = 0;
	virtual DWORD			__stdcall	AddMaterial(char* szFileName,DWORD dwRefIndex,DWORD dwMtlIndex) = 0;

	
	virtual void			__stdcall	DeleteMotion(DWORD dwRefIndex,DWORD dwMotionIndex) = 0;
	virtual void			__stdcall	DeleteAllMotionPerReference(DWORD dwRefIndex) = 0;
	virtual void			__stdcall 	DeleteAllMotion() = 0;		
	
	virtual void			__stdcall	DeleteMaterial(DWORD dwRefIndex,DWORD dwMtlIndex) = 0;
	virtual void			__stdcall	DeleteAllMaterialPerReference(DWORD dwRefIndex) = 0;
	virtual void			__stdcall	DeleteAllMaterial() = 0;

	virtual BOOL			__stdcall	UnloadMotionPerReference(DWORD dwRefIndex) = 0;
	virtual BOOL			__stdcall	UnloadMaterialPerReference(DWORD dwRefIndex) = 0;
	virtual void			__stdcall	DisableUnloadPreLoadedResource(DWORD dwRefIndex) = 0;
	virtual void			__stdcall	EnableUnloadPreLoadedResource(DWORD dwRefIndex) = 0;
 		
	virtual DWORD			__stdcall	CreateIVertexList(IVERTEX** ppVertex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex) = 0;
	virtual void			__stdcall	ReleaseIVertexList(IVERTEX*	pVertex) = 0;

	virtual DWORD			__stdcall	GetMotionNum(DWORD dwRefIndex) = 0;
	virtual BOOL			__stdcall	GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwRefIndex) = 0;
	virtual DWORD			__stdcall	GetMaterialNum(DWORD dwRefIndex) = 0;
	virtual DWORD			__stdcall	GetObjectIndex(char* szObjName) = 0;
	virtual	MATRIX4*		__stdcall	GetObjectResultMatrix(DWORD dwRefIndex,DWORD dwObjIndex) = 0;
	
	virtual void			__stdcall	Optimize() = 0;	
	virtual DWORD			__stdcall	GetCameraList(CAMERA_DESC* pDesc,DWORD dwNum) = 0;
	virtual DWORD			__stdcall	GetLightList(LIGHT_DESC* pDesc,DWORD dwNum) = 0;
	virtual DWORD			__stdcall	GetMeshObjectIndex(DWORD dwSeqIndex) = 0;
	virtual DWORD			__stdcall	GetMeshObjectNum() = 0;
	virtual DWORD			__stdcall	GetObjectNum() = 0;
		

	virtual DWORD			__stdcall	GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual char*			__stdcall	GetObjectName(DWORD dwObjIndex) = 0;
	virtual BOOL			__stdcall	SetZOrder(int iOrder,DWORD dwRefIndex) =0;

	virtual BOOL			__stdcall	DuplicateAnimation(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex) = 0;
	virtual BOOL			__stdcall	DuplicateMaterial(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex) = 0;

	virtual DWORD			__stdcall	GetMaterialIndex(char* szMtlName) = 0;
	virtual char*			__stdcall	GetModelFileName() = 0;
	virtual	BOOL			__stdcall	Unload() = 0;

	virtual BOOL			__stdcall	InitializeIllusionEffect(DWORD dwRefIndex,DWORD dwMaxIllusionFrameNum,DWORD dwObjIndex,void* pMtlHandle,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	SetIllusionEffectMaterial(DWORD dwRefIndex,void* pMtlHandle) = 0;
	virtual void			__stdcall	SetBeginIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex) = 0;
	virtual void			__stdcall	SetEndIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex) = 0;
	virtual BOOL			__stdcall	GetWorldMatrix(MATRIX4* pMatWorld,DWORD dwRefIndex,DWORD dwObjIndex) = 0;
	virtual BOOL			__stdcall	SetAxisAlignOK(BOOL bSwitch,DWORD dwRefIndex) = 0;


};
interface IGeometryController
{
	virtual BOOL			__stdcall	ApplyResultMatrix(MATRIX4** ppResult,DWORD dwRefIndex,DWORD dwFlag)= 0;
	virtual BOOL			__stdcall	ApplyPhysique(BYTE* pVertex,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag) = 0;
	virtual	BOOL			__stdcall	ApplyMorph(VECTOR3** ppv3Local,TVERTEX** pptv,DWORD dwRefIndex,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	ApplyView(MATRIX4* pView,DWORD dwRefIndex,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	ApplyMotionStatus(MOTION_STATUS* pMotionStatus,DWORD dwRefIndex,DWORD dwFlag) = 0;
	virtual BOOL			__stdcall	ApplyWorldMatrixForPhysique(MATRIX4* pResultWorldMatrix,DWORD dwRefIndex,DWORD dwFlag) = 0;
//	virtual BOOL			__stdcall	ApplyVertexNormal(BYTE* pVertex,DWORD dwVertexStartOffset,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag) = 0;

};

interface IGeometryControllerStatic
{
	virtual BOOL				__stdcall	IsIncludeViewVolume() = 0;
	virtual BOOL				__stdcall	OnFault() = 0;
	virtual BOOL				__stdcall	OnOut() = 0;

	virtual void				__stdcall	GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum) = 0;
	virtual DWORD				__stdcall	GetTileBufferDescNum() = 0;
	virtual TILE_BUFFER_DESC*	__stdcall	GetTileBufferDesc(DWORD dwIndex) = 0;


};

interface IHeightField : public IUnknown
{
	virtual	COLLISION_MESH_OBJECT_DESC*		__stdcall		GetCollisionMeshObjectDesc(DWORD dwObjIndex) = 0;
	virtual void							__stdcall		GetWorldBoundingMesh(COLLISION_MESH_OBJECT_DESC* pColDesc) = 0;
	
	virtual BOOL		__stdcall	Create(HFIELD_CREATE_DESC* pDesc,DWORD dwFlag) = 0;
	virtual BOOL		__stdcall	LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum) = 0;
	virtual	BOOL		__stdcall	ReplaceTile(char* szFileName,DWORD dwTexIndex) = 0;
//	virtual BOOL		__stdcall	RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag) = 0;

	virtual BOOL		__stdcall	GetHFieldDesc(HFIELD_DESC* pDesc) = 0;
	virtual BOOL		__stdcall	SetYFacorPosList(float* pyfList,DWORD dwYFNumX,DWORD dwYFNumZ) = 0;
	virtual BOOL		__stdcall	SetYFactorFromModel(I3DModel* pModel,DWORD dwRefIndex,float fGridLength,float fMinY,float fMaxY) = 0;
		
	virtual DWORD		__stdcall	GetObjectNum() = 0;
	virtual DWORD		__stdcall	GetObjectIndex(DWORD dwSeqIndex) = 0;
	virtual BOOL		__stdcall	ReadFile(char* szFileName,DWORD dwIndexBufferNumLV0,LOAD_CALLBACK_DESC* pCallbackDesc,DWORD dwFlag) = 0;
	virtual BOOL		__stdcall	WriteFile(char* szFileName,DWORD dwFlag) = 0;
		
	virtual BOOL		__stdcall	BeignShadeLightMapObject(DWORD dwColor) = 0;
	virtual BOOL		__stdcall	ShadeLightMapObject(DWORD dwObjIndex,VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pDesc,SHADE_FUNC pFunc) = 0;
	virtual void		__stdcall	EndShadeLightMapObject() = 0;
	
	virtual		DWORD		__stdcall	GetTileListWithScreenCoord(HFIELD_POS* pTileList,DWORD dwMaxTileNum,RECT* pRect,DWORD dwViewportIndex) = 0;
	virtual		DWORD		__stdcall	GetVertexListWithScreenCoord(HFIELD_POS* pVertexList,DWORD dwMaxVertexNum,RECT* pRect,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetVertexPosWithScreenCoord(DWORD* pdwPosX, DWORD* pdwPosZ, VECTOR3 OUT *pWhere, POINT* ptCursor, float fVertexRadius,DWORD dwViewportIndex) = 0;
	virtual		DWORD		__stdcall	GetObjectPosWithScreenCoord(VECTOR3* pv3IntersectPoint,POINT* ptCursor,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetTilePosWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,POINT* ptCursor,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetTileRectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,RECT* pRect,POINT* ptCursor,DWORD dwBrushSize,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetIntersectPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float*pfDist,POINT* ptCursor,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetIntersectPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwPosX,DWORD* pdwPosZ,VECTOR3* pv3Orig,VECTOR3* pv3Dir) = 0;
	
	virtual		BOOL		__stdcall	GetY(float* pfHeight,float x,float z) = 0;
	
	virtual		void		__stdcall	CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosZ) = 0;
	virtual		void		__stdcall	CalcY(float* py,DWORD dwPosX,DWORD dwPosZ) = 0;
	virtual 	BOOL		__stdcall	ResetCollisionMesh() = 0;
	virtual		void		__stdcall	ResetHeight(float h) = 0;
	virtual		void		__stdcall	MakeHeightFieldHigh(DWORD dwX, DWORD dwZ, float fHeight, float fRadius,DWORD dwActionType) = 0;
	virtual 	BOOL		__stdcall	AdjustHeight(HFIELD_POS* pVertexList,DWORD dwVertexNum,float fValue,DWORD dwActionType) = 0;
	virtual		BOOL		__stdcall	InterpolateField(DWORD dwX, DWORD dwZ, float fBias, float fRadius) = 0;
	virtual		BOOL		__stdcall	SetTile(DWORD dwPosX,DWORD dwPosZ,WORD wTileIndex) = 0;
	virtual		BOOL		__stdcall	GetTile(WORD* pwTileIndex,DWORD dwPosX,DWORD dwPosZ) = 0;
	virtual		BOOL		__stdcall	SetTileRect(RECT* pRect,WORD wTileIndex) = 0;
	virtual		BOOL		__stdcall	SetTileList(HFIELD_POS* phfStart,HFIELD_POS* pTileList,WORD* pTexTileIndexList,DWORD dwTileNum,WORD wTileIndex) = 0;
	virtual		void		__stdcall	SetVertexColor(DWORD dwColor) = 0;

	virtual		BOOL		__stdcall	RenderVertex(HFIELD_POS* pVertexList,DWORD dwVertexNum,DWORD dwColor) = 0;
	virtual		BOOL		__stdcall	RenderTile(HFIELD_POS* phfStart,HFIELD_POS* pTileList,DWORD dwTileNum,WORD* pwTileIndexList,DWORD dwColor) = 0;
	virtual 	void		__stdcall	SetHeightFieldDetail(DWORD dwDetailLevel) = 0;
	virtual 	void		__stdcall	EnableRender(DWORD dwObjIndex) = 0;
	virtual		void		__stdcall	DisableRender(DWORD dwObjIndex) = 0;
	virtual		void		__stdcall	FreezeDetailLevel(DWORD dwObjIndex,DWORD dwDetailLevel) = 0;
	virtual		void		__stdcall	UnFreezeDetailLevel(DWORD dwObjIndex) = 0;
	virtual		BOOL		__stdcall	SetLightVertexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwColor,DWORD dwViewportIndex) = 0;
	virtual		BOOL		__stdcall	GetLightVertexel(DWORD* pdwColor,POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex) = 0;
	virtual 	void		__stdcall	Optimize() = 0;
	virtual 	DWORD		__stdcall	GetXZFrom3DCoord(HFIELD_POS* phfPos,DWORD dwMaxHFPosNum,VECTOR3* pv3IntersectPoint,float fRs) = 0;
	virtual		BOOL		__stdcall	SetAlphaTexel(POINT* ptCursor,VECTOR3* pv3IntersectPoint,float fRs,DWORD dwTileIndex,DWORD dwActionFlag,DWORD dwViewportIndex) = 0;
	virtual 	DWORD		__stdcall	GetExtTileIndexTop(POINT* ptCursor,VECTOR3* pv3IntersectPoint,DWORD dwViewportIndex) = 0;
	virtual		void		__stdcall	EnableBlend(BOOL bEnable) = 0;
	virtual 	void		__stdcall	UpdateAllTiles() = 0;
	
	virtual		void		__stdcall	ClearAlphaMapAll() = 0;
	virtual		void		__stdcall	ClearAlphaMapWithObjectIndex(DWORD dwObjIndex) = 0;
	virtual		void		__stdcall	ClearAlphaMapWithTileIndex(DWORD dwObjIndex,DWORD dwTileIndex) = 0;
	virtual		void		__stdcall	GetAlphaMapResourceStatus(DWORD* pdwMapNum,DWORD* pdwMemSize) = 0;

	virtual		DWORD		__stdcall	QueryNeedTriNumWithAABB(DWORD* pdwStartX,DWORD* pdwStartZ,DWORD* pdwWidthNum,DWORD* pdwHeightNum,MAABB* pAABB) = 0;
	virtual		DWORD		__stdcall	GetTriListWithAABB(VECTOR3* pv3TriListOut, DWORD dwMaxTriCount, MAABB* pAABB) = 0;

};

interface ISkyMesh : public IUnknown
{
	virtual		BOOL	__stdcall		Render(DWORD dwFlag) = 0;
	virtual		void	__stdcall		SetViewportIndex(DWORD dwViewportIndex) = 0;
	virtual		void	__stdcall		SetOffset(VECTOR3* pv3Offset) = 0;
	
	virtual		BOOL	__stdcall		CreateMotionList(DWORD dwNum) = 0;
	virtual		DWORD	__stdcall		AddMotion(char* szFileName,DWORD dwMotionIndex) = 0;	
	virtual		DWORD	__stdcall		GetMotionNum() = 0;
	virtual		BOOL	__stdcall		GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex) = 0;
	virtual		DWORD	__stdcall		GetCurrentMotionIndex() = 0;
	virtual		void	__stdcall		SetCurrentMotionIndex(DWORD dwMotionIndex) = 0;
	virtual		DWORD	__stdcall		GetCurrentFrame() = 0;
	virtual		void	__stdcall		SetCurrentFrame(DWORD dwFrame) = 0;
	virtual		DWORD	__stdcall		GetAlpha() = 0;
	virtual		void	__stdcall		SetAlpha(DWORD dwAlpha) = 0;
	virtual		void	__stdcall		IncreaseFrame(DWORD dwIncFrame) = 0;
};
interface I4DyuchiGXGeometry : public IUnknown
{
	virtual		BOOL			__stdcall	Initialize(I4DyuchiGXRenderer* pRenderer,I4DyuchiFileStorage* pFileStorage,DWORD dwViewportNum,DWORD dwMaxFileNum,DWORD dwMaxBucketNum,ErrorHandleProc pErrorHandleFunc) = 0;
	virtual		ULONG			__stdcall 	LoadModel(I3DModel** ppModel,char* szFileName,DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	IsLoadedModel(I3DModel** ppModel,char* szFileName) = 0;
	virtual		ULONG			__stdcall	CreateHeightField(IHeightField** ppHeightField,DWORD dwFlag) = 0;
	virtual 	ULONG			__stdcall	CreateStaticModel(I3DStaticModel** ppStaticModel,DWORD dwMaxObjectNum,DWORD dwMaxMtlNum,DWORD dwFlag) = 0;
	virtual		ULONG			__stdcall	CreateSkyMesh(ISkyMesh** ppSkyMesh,char* szFileName,DWORD dwFlag) = 0;
	
	virtual		BOOL			__stdcall	BeginRender(DWORD dwViewportIndex,DWORD dwColor,DWORD dwFlag) = 0;
	virtual		void			__stdcall	EndRender() = 0;
	virtual		void			__stdcall	Present(HWND hWnd) = 0;
		
	virtual		BOOL			__stdcall 	RenderModel(I3DModel* pModel,DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	RenderStaticModel(I3DStaticModel* pModel,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	RenderHField(IHeightField* pHField,DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag) = 0;
	virtual 	BOOL			__stdcall	RenderLight(LIGHT_DESC* pLightDesc,DWORD dwAlphaFlag,DWORD dwRenderFlag) = 0;
	virtual		void			__stdcall	RenderSphere(VECTOR3* pv3Point,float fRs,DWORD dwColor) = 0;
	virtual		void			__stdcall	RenderBox(VECTOR3* pv3Oct,DWORD dwColor) = 0;
	virtual		BOOL			__stdcall	RenderSkyMesh(ISkyMesh* pSkyMesh,DWORD dwFlag) = 0;

	

	virtual		DWORD			__stdcall	CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName) = 0;
	virtual		BOOL			__stdcall	DeleteDynamicLight(DWORD dwIndex) = 0;
	virtual		BOOL			__stdcall	AddLight(LIGHT_DESC* pLight,DWORD dwFlag) = 0;
	
	virtual		void			__stdcall	SetShadowFlag(DWORD dwFlag) = 0;
	virtual		DWORD			__stdcall	GetShadowFlag() = 0;
	virtual 	void			__stdcall	SetLightFlag(DWORD dwFlag) = 0;
	virtual		DWORD			__stdcall	GetLightFlag() = 0;


	virtual		void			__stdcall	GetProjectionMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetViewMatrix(MATRIX4* pMatrix,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetViewVolume(VIEW_VOLUME* pVolume,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	GetRayWithScreenCoord(VECTOR3* pv3Pos,VECTOR3* pv3Dir,POINT* pt,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetViewport(SHORT_RECT* pClipRect,WORD* pwWidth,WORD* pwHeight,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	SetViewport(SHORT_RECT* pRect,DWORD dwViewportIndex) = 0;
	
	virtual		BOOL			__stdcall	ResetCamera(VECTOR3* pv3Eye,float fNear,float fFar,float fFovX,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	MoveCamera(VECTOR3* pv3Trans,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	SetCameraPos(VECTOR3* pv3Eye,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	SetCameraViewDistance(float fNear,float fFar,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	SetCameraFitModel(I3DModel* pModel,DWORD dwRefIndex,float fNear,float fFar,float fFovX,DWORD dwViewportIndex) = 0;
	virtual		BOOL			__stdcall	SetCameraFitCollisionMesh(COLLISION_MESH_OBJECT_DESC* pColMeshDesc,float fNear,float fFar,float fFovX,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	SetFog(FOG_DESC* pFogDesc,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	EnableFog(DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	DisableFog(DWORD dwViewportIndex) = 0;
	
	virtual		void			__stdcall	SetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLight,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	EnableDirectionalLight(DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	DisableDirectionalLight(DWORD dwViewportIndex) = 0;

	virtual		void			__stdcall	GetCameraDesc(CAMERA_DESC* pCamera,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	SetCameraDesc(CAMERA_DESC* pCamera,DWORD dwViewportIndex) = 0;

	virtual		void			__stdcall	SetCameraAngleRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex) = 0;	/// 추가. 2002/02/05
	virtual		void			__stdcall	RotateCameraRad( VECTOR3* pv3Rotate,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetCameraAngleRad(VECTOR3* pv3Angle,DWORD dwViewportIndex) = 0;
	
	virtual		BOOL			__stdcall	GetIntersectPointWithScreenCoord(VECTOR3* pv3Pos,PLANE* pPlane,POINT* ptCursor,DWORD dwViewportIndex) = 0;
	

	virtual		void			__stdcall	SetAmbientColor(DWORD dwViewportIndex,DWORD dwColor) = 0;
	virtual		DWORD			__stdcall	GetAmbientColor(DWORD dwViewportIndex) = 0;
	
	virtual		void			__stdcall	SetEmissiveColor(DWORD dwViewportIndex,DWORD dwColor) = 0;
	virtual		DWORD			__stdcall	GetEmissiveColor(DWORD dwViewportIndex) = 0;

	virtual		void			__stdcall	SetDrawDebugFlag(DWORD dwFlag) = 0;
	virtual		DWORD			__stdcall	GetDrawDebugFlag() = 0;

	virtual		void			__stdcall	EnableViewVolumeClip() = 0;
	virtual		void			__stdcall	DisableViewVolumeClip() = 0;
	virtual		void			__stdcall	GetSystemStatus(SYSTEM_STATUS* pStatus) = 0;
	virtual		void			__stdcall	UpdateWindowSize() = 0;
	virtual		BOOL			__stdcall	PreLoadModel(char* szFileName) = 0;
	virtual		BOOL			__stdcall	PreLoadMotion(char* szFileName) = 0;
	virtual 	BOOL			__stdcall	PreLoadMaterial(char* szFileName) = 0;
	virtual		void			__stdcall	UnloadAllPreLoadedItem(DWORD dwFlag) = 0;
	virtual		BOOL			__stdcall	DisableUnloadPreLoadedResource(char* szFileName) = 0;
	virtual		BOOL			__stdcall	EnableUnloadPreLoadedResource(char* szFileName) = 0;

	virtual 	DWORD			__stdcall	GetReadCountInMOD(char* szFileName) = 0;
	virtual		DWORD			__stdcall	GetReadCountInSTM(char* szFileName) = 0;
	virtual		DWORD			__stdcall	GetReadCountInHField(char* szFileName) = 0;
	virtual 	void			__stdcall	CalcViewVolume(VIEW_VOLUME* pVV,RECT* pRect,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall 	GetCameraEyePlane(PLANE* pPlane,DWORD dwViewportIndex) = 0;
	virtual 	void			__stdcall	GetBillBoardMatrix(MATRIX4* pMat,DWORD dwViewportIndex) = 0;
	virtual		void			__stdcall	GetCameraAttachedMatrix(MATRIX4* pMat,DWORD dwViewportIndex,float fDist) = 0;
	virtual		void			__stdcall	SetTickCount(DWORD dwTick,BOOL bGameFrame) = 0;
	virtual		void			__stdcall	SetRadiusForLoadResource(float fRS) = 0;
	virtual 	void			__stdcall	SetShadowUpdatePerFrame(BOOL bSwitch) = 0;

	//trustpak 2005/06/24

	virtual		void			__stdcall	SetModelLODUsingMode(DWORD dwLodUsingMode, DWORD dwLevel) = 0;
	virtual		DWORD			__stdcall	GetModelLODUsingMode(VOID) = 0;	
	virtual		DWORD			__stdcall	GetModelLodUsingLevel(VOID) = 0;

	///

};



