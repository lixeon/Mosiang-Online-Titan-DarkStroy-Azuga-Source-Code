#pragma once

#include "IRenderer_GUID.h"
#include "stdafx.h"
#include "typedef.h"
#include "IFileStorage.h"

class CBaseObject;
class CCameraObject;
class CFaceGroup;
class CMaterial;


interface IGeometryController;
interface IGeometryControllerStatic;
interface IDIMeshObject : public IUnknown
{
	
	virtual		BOOL		__stdcall	StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic) = 0;
	virtual		void		__stdcall	EndInitialize() = 0;
	virtual		BOOL		__stdcall	InsertFaceGroup(FACE_DESC* pDesc) = 0;
	virtual		BOOL		__stdcall	Render(
		DWORD dwRefIndex,DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag
		) = 0;

	virtual 	BOOL	__stdcall		RenderProjection(
		DWORD		dwRefIndex,										// 애니메이션 컨트롤블럭
		DWORD		dwAlpha,
		BYTE*		pSpotLightIndex,
		DWORD		dwViewNum,
		DWORD		dwFlag
		) = 0;
	
	virtual		BOOL		__stdcall	Update(DWORD dwFlag) = 0;
	virtual		void		__stdcall	DisableUpdate() = 0;
	
};


interface IDIHFieldObject : public IUnknown
{
	virtual		BOOL		__stdcall	Create(DWORD dwPosX,DWORD dwPosZ,DWORD dwDetailLevel,DWORD dwFacesNumPerX,DWORD dwFacesNumPerZ,VECTOR3* pv3Rect,HFIELD_DESC* pHFDesc) = 0;
	virtual		BOOL		__stdcall	SetYFactor(DWORD dwDestPitch,HFIELD_DESC* pHFDesc) = 0;
	virtual 	BOOL		__stdcall	SetVertexColor(DWORD* pdwColor,DWORD dwVerticesNum) = 0;
	virtual		BOOL		__stdcall	SetVertexColorAll(DWORD dwColor) = 0;
	virtual		BOOL		__stdcall	SetDetailLevel(DWORD dwDetailLevel) = 0;
	virtual		void		__stdcall	SetDistanceFromViewPoint(float fDistance) = 0;
	virtual		void		__stdcall	SetPositionMask(DWORD dwPosMask) = 0;			// 0-15
	virtual		BOOL		__stdcall	ReBuildMesh(DWORD dwDestPitch,HFIELD_DESC* pHFDesc , DWORD* pdwColor) = 0;
	virtual 	void		__stdcall	SetMustUpdate() = 0;
	virtual		BOOL		__stdcall	UpdateAlphaMap(TILE_BUFFER_DESC* pTileBufferDesc) = 0;
	virtual		void		__stdcall	CleanupAlphaMap() = 0;
};
interface IDIImmMeshObject : public IUnknown
{
	virtual		BOOL		__stdcall	GetTriBufferPtr(BYTE** ppDest,DWORD* pdwSize) = 0;
	virtual		BOOL		__stdcall	Update(DWORD dwFlag) = 0;
};
interface IDIFontObject : public IUnknown
{
	virtual		void		__stdcall 	BeginRender() = 0;
	virtual		void		__stdcall	EndRender() = 0;
	virtual		BOOL		__stdcall	DrawText(TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,DWORD dwFlag) = 0;


};

interface IDISpriteObject : public IUnknown
{
	virtual		BOOL		__stdcall	Draw(VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,DWORD dwFlag) = 0;
	virtual		BOOL		__stdcall	Resize(float fWidth,float fHeight) = 0;
	virtual 	BOOL		__stdcall	GetImageHeader(IMAGE_HEADER* pImgHeader,DWORD dwFrameIndex) = 0;
	virtual		BOOL		__stdcall	LockRect(LOCKED_RECT* pOutLockedRect, RECT* pRect, TEXTURE_FORMAT TexFormat)=0;
	virtual		BOOL		__stdcall	UnlockRect()=0;
};


interface IDIHeightField : public IUnknown
{

	virtual		BOOL				__stdcall	StartInitialize(HFIELD_DESC* pDesc) = 0;
	virtual		void				__stdcall	EndInitialize() = 0;
	virtual		IDIMeshObject*		__stdcall	CreateHeightFieldObject(HFIELD_OBJECT_DESC* pDesc) = 0;

	virtual		BOOL				__stdcall	InitiallizeIndexBufferPool(DWORD dwDetailLevel,DWORD dwIndicesNum,DWORD dwNum) = 0;
	virtual		BOOL				__stdcall	LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum) = 0;
	virtual		BOOL				__stdcall	ReplaceTile(char* szFileName,DWORD dwTexIndex) = 0;

	virtual		BOOL				__stdcall	CreateIndexBuffer(DWORD dwIndicesNum,DWORD dwDetailLevel,DWORD dwPositionMask,DWORD dwNum) = 0;
	virtual		BOOL				__stdcall	LockIndexBufferPtr(WORD** ppWord,DWORD dwDetailLevel,DWORD dwPositionMask) = 0;
	virtual		void				__stdcall	UnlcokIndexBufferPtr(DWORD dwDetailLevel,DWORD dwPositionMask) = 0;
	virtual 	BOOL				__stdcall	RenderGrid(VECTOR3* pv3Quad,DWORD dwTexTileIndex,DWORD dwAlpha) = 0;
	virtual		void				__stdcall	SetHFieldTileBlend(BOOL bSwitch) = 0;
	virtual		BOOL				__stdcall	IsEnableHFieldTileBlend() = 0;

};

interface I4DyuchiGXRenderer : public IUnknown
{
	virtual		BOOL				__stdcall	Create(HWND hWnd,DISPLAY_INFO* pInfo,I4DyuchiFileStorage* pFileStorage,ErrorHandleProc pErrorHandleFunc) = 0;
	
	virtual		IDISpriteObject*	__stdcall	CreateSpriteObject(char* szFileName,DWORD dwFlag) = 0;
	virtual		IDISpriteObject*	__stdcall	CreateSpriteObject(char* szFileName,DWORD dwXPos,DWORD dwYPos,DWORD dwWidth,DWORD dwHeight,DWORD dwFlag) = 0;
	virtual		IDISpriteObject*	__stdcall	CreateEmptySpriteObject( DWORD dwWidth,DWORD dwHeight, TEXTURE_FORMAT TexFormat, DWORD dwFlag)=0; // 넓이, 높이는 꼭 2의 n승이어야 한다. D3DFORMAT, 사용안함.
	virtual		IDIMeshObject*		__stdcall	CreateMeshObject(CMeshFlag flag) = 0;
	virtual		IDIFontObject*		__stdcall	CreateFontObject(LOGFONT* pLogFont,DWORD dwFlag) = 0;
	virtual 	IDIHeightField*		__stdcall	CreateHeightField(DWORD dwFlag) = 0;
	virtual		IDIMeshObject*		__stdcall	CreateImmMeshObject(IVERTEX* piv3Tri,DWORD dwTriCount,void* pMtlHandle,DWORD dwFlag) = 0;

	virtual		void				__stdcall	BeginRender(SHORT_RECT* pRect,DWORD dwColor,DWORD dwFlag) = 0;
	virtual		void				__stdcall	EndRender() = 0;
	
	virtual		void				__stdcall	SetShadowFlag(DWORD dwFlag) = 0;
	virtual		DWORD				__stdcall	GetShadowFlag() = 0;

	virtual		void				__stdcall	SetLightMapFlag(DWORD dwFlag) = 0;
	virtual		DWORD				__stdcall	GetLightMapFlag() = 0;

	virtual		void				__stdcall	SetRenderMode(DWORD dwFlag) = 0;
	virtual		DWORD				__stdcall	GetRenderMode() = 0;

	virtual		void				__stdcall	EnableFog(float fStart,float fEnd,float fDensity,DWORD dwColor,DWORD dwFlag) = 0;
	virtual		void				__stdcall	DisableFog() = 0;
	

	virtual		BOOL				__stdcall	BeginShadowMap() = 0;
	virtual		void				__stdcall	EndShadowMap() = 0;

	virtual		void				__stdcall	GetClientRect(SHORT_RECT* pRect,WORD* pwWidth,WORD* pwHeight) = 0;
	virtual 	DWORD				__stdcall	CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName) = 0;
	virtual		BOOL				__stdcall	DeleteDynamicLight(DWORD dwIndex) = 0;

	virtual		BOOL				__stdcall	CreateEffectShaderPaletteFromFile(char* szFileName) = 0;
	virtual		BOOL				__stdcall	CreateEffectShaderPalette(CUSTOM_EFFECT_DESC* pEffectDescList,DWORD dwNum) = 0;
	virtual		void				__stdcall	DeleteEffectShaderPalette() = 0;


	virtual		BOOL				__stdcall	RenderMeshObject(
		IDIMeshObject* pMeshObj,
		DWORD dwRefIndex,
		float fDistance,
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag) = 0;

	virtual		BOOL				__stdcall	RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag) = 0;
	virtual		BOOL				__stdcall	RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag) = 0;
	virtual		void				__stdcall	RenderBox(VECTOR3* pv3Oct,DWORD dwColor) = 0;
	virtual		void				__stdcall	RenderPoint(VECTOR3* pv3Point,DWORD dwColor) = 0;
	virtual		void				__stdcall	RenderCircle(VECTOR2* pv2Point,float fRs,DWORD dwColor) = 0;
	virtual		void				__stdcall	RenderLine(VECTOR2* pv2Point0,VECTOR2* pv2Point1,DWORD dwColor) = 0;
	virtual		void				__stdcall	RenderGrid(VECTOR3* pv3Quad,DWORD dwColor) = 0;
	virtual 	BOOL				__stdcall	RenderTriIvertex(IVERTEX* piv3Tri,void* pMtlHandle,DWORD dwFacesNum,DWORD dwFlag) = 0;
	virtual		BOOL				__stdcall	RenderTriVector3(VECTOR3* pv3Tri,DWORD dwFacesNum,DWORD dwFlag) = 0;

	virtual		void*				__stdcall	AllocRenderTriBuffer(IVERTEX** ppIVList,DWORD dwFacesNum,DWORD dwRenderFlag) = 0;
	virtual		void				__stdcall	EnableRenderTriBuffer(void* pTriBufferHandle,void* pMtlHandle,DWORD dwRenderFacesNum) = 0;
	virtual		void				__stdcall	DisableRenderTriBuffer(void* pTriBufferHandle) = 0;
	virtual		void				__stdcall	FreeRenderTriBuffer(void* pTriBufferHandle) = 0;
	
	virtual		BOOL				__stdcall	SetRTLight(LIGHT_DESC* pLightDesc,DWORD dwLightIndex,DWORD dwFlag) = 0;
	virtual		void				__stdcall	EnableDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLightDesc,DWORD dwFlag) = 0;
	virtual		void				__stdcall	DisableDirectionalLight() = 0;

	virtual		void				__stdcall	SetSpotLightDesc(VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up,float fFov,float fNear,float fFar,float fWidth,BOOL bOrtho,void* pMtlHandle,DWORD dwColorOP,DWORD dwLightIndex,SPOT_LIGHT_TYPE type) = 0;
	virtual		void				__stdcall	SetShadowLightSenderPosition(BOUNDING_SPHERE* pSphere,DWORD dwLightIndex) = 0;
	virtual		void				__stdcall	SetViewFrusturm(VIEW_VOLUME* pViewVolume,CAMERA_DESC* camera,MATRIX4* pMatView,MATRIX4* pMatProj,MATRIX4* pMatForBilboard) = 0;
	virtual		void				__stdcall	GetSystemStatus(SYSTEM_STATUS* pStatus) = 0;
	virtual		void				__stdcall	UpdateWindowSize() = 0;
	virtual 	void				__stdcall	Present(HWND hWnd) = 0;
	virtual		void				__stdcall	SetAmbientColor(DWORD dwColor) = 0;
	virtual		DWORD				__stdcall	GetAmbientColor() = 0;
	virtual		void				__stdcall	SetEmissiveColor(DWORD dwColor) = 0;
	virtual		DWORD				__stdcall	GetEmissiveColor() = 0;

	virtual		void				__stdcall	BeginPerformanceAnalyze() = 0;
	virtual		void				__stdcall	EndPerformanceAnalyze() = 0;
	virtual		BOOL				__stdcall	CaptureScreen(char* szFileName) = 0;
	
	virtual		DWORD				__stdcall	CreateMaterialSet(MATERIAL_TABLE* pMtlEntry,DWORD dwNum) = 0;
	virtual		void				__stdcall	DeleteMaterialSet(DWORD dwMtlSetIndex) = 0;

	virtual		void*				__stdcall	CreateMaterial(MATERIAL* pMaterial,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag) = 0;
	virtual 	void				__stdcall	SetMaterialTextureBorder(void* pMtlHandle,DWORD dwColor) = 0;
	virtual		void				__stdcall	DeleteMaterial(void* pMtlHandle) = 0;
	virtual		void				__stdcall	SetAttentuation0(float att) = 0;
	virtual		float				__stdcall	GetAttentuation0() = 0;
	virtual		BOOL				__stdcall	ConvertCompressedTexture(char* szFileName,DWORD dwFlag) = 0;
	virtual		void				__stdcall	EnableSpecular(float fVal) = 0;
	virtual		void				__stdcall	DisableSpecular() = 0;
	virtual 	void				__stdcall	SetVerticalSync(BOOL bSwitch) = 0;
	virtual		BOOL				__stdcall	IsSetVerticalSync() = 0;
	virtual		void				__stdcall	ResetDevice(BOOL bTest) = 0;
	virtual 	void				__stdcall	SetFreeVBCacheRate(float fVal) = 0;
	virtual		float				__stdcall	GetFreeVBCacheRate() = 0;
	virtual		DWORD				__stdcall	ClearVBCacheWithIDIMeshObject(IDIMeshObject* pObject) = 0;
	virtual 	DWORD				__stdcall	ClearCacheWithMotionUID(void* pMotionUID) = 0;
	virtual		void				__stdcall	SetTickCount(DWORD dwTickCount,BOOL bGameFrame) = 0;
	virtual		BOOL				__stdcall	GetD3DDevice(REFIID refiid,void** ppVoid) = 0;
	virtual		BOOL				__stdcall	InitializeRenderTarget(DWORD dwTexelSize,DWORD dwMaxTexNum) = 0;
	virtual		void				__stdcall	SetRenderTextureMustUpdate(BOOL bMustUpdate) = 0;
	virtual 	void				__stdcall	SetAlphaRefValue(DWORD dwRefVaule) = 0;

	//trustpak 2005/04/04
	virtual		BOOL				__stdcall	SetLoadFailedTextureTable(TEXTURE_TABLE* pLoadFailedTextureTable, DWORD dwLoadFailedTextureTableSize) = 0;
	virtual		VOID				__stdcall	GetLoadFailedTextureTable(TEXTURE_TABLE** ppoutLoadFailedTextureTable, DWORD* poutdwLoadFailedTextureTableSize, DWORD* poutdwFailedTextureCount) = 0;
	///

	//trustpak 2005/06/08
	virtual		VOID				__stdcall	SetRenderWireSolidBothMode(BOOL bMode) = 0;
	virtual		BOOL				__stdcall	GetRenderWireSolidBothMode(VOID) = 0;
	///
};


