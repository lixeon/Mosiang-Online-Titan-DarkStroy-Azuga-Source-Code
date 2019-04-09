#pragma once


#include "../4DyuchiGRX_common/IRenderer.h"

#include <d3dx8.h>
#include "texture_manager.h"

#include "../4DyuchiGXGFunc/global.h"
#include "HFieldManager.h"
#include "RenderTexture.h"
#include "RenderQ.h"

#include "ProjectionLight.h"
#include "circle.h"

#include "MaterialManager.h"
#include "TextBuffer.h"
#include "SpriteBuffer.h"
#include "RenderTriBuffer.h"
#include "VBCache.h"
#include "IBCache.h"



#include "D3DResourceManager.h"


class CFontCache;
class CImmMeshObject;
class CRenderTexturePool;
struct SHADOWMAP_HANDLE
{
	CRenderTexture			RenderTexture;
	CTexture				Texture;
};


#define MAX_SHADER_NUM		8
#define	LM_DYNAMIC_LIGHT	0
#define VL_TEST				7



class CoD3DDevice : public I4DyuchiGXRenderer
{
	CD3DResourceManager			m_rcManager;
	I4DyuchiFileStorage*		m_pFileStorage;


	IResourcePool*				m_pResourcePoolList[MAX_RESOURCE_POOL_NUM];
	DWORD						m_dwResourcePoolNum;
	
	

	CVBHeap*					m_pVBVLMeshHeap;
	CVBHeap*					m_pVBVLSMeshHeap;
	CVBHeap*					m_pVBPLMeshHeap;
	CVBHeap*					m_pVBLMMeshHeap;
	CVBHeap*					m_pVBVector3MeshHeap;
	CVBHeap*					m_pVBSpriteHeap;
	CVBHeap*					m_pVBHeapForImmMesh;
	
	STMPOOL_HANDLE				m_pImmMeshObjectPool;

		
	CIBHeap*					m_pIBHeapManaged;
	CIBHeap*					m_pIBHeapSprite;
	
	// D3DPOOL_DEFAULT를 사용하는 버텍스버퍼,인덱스버퍼 캐쉬이다.
	CVBCache					m_VBCache;
	CIBCache					m_IBCache;

	

	CFontCache*					m_pFontCache;
	CRenderTexturePool*			m_pRenderTexturePool;

	CRenderQueue				m_RenderQ;
	DPCQ_HANDLE					m_pDPCQFor2DItem;
	CTextBuffer					m_TextBuffer;
	CSpriteBuffer				m_SpriteBuffer;
	CRenderTriBuffer			m_RenderTriBuffer;

	DWORD						m_dwRefCount;
	IDIFontObject*				m_pFont;
	CTextureManager*			m_pTextureManager;
	CMaterialManager*			m_pMaterialManager;
	static MATRIX4				m_matIdentity;

	MATRIX4						m_matView;
	
	MATRIX4						m_matForBilBoard;
	MATRIX4						m_matProj;
	MATRIX4						m_matViewInverse;
	CAMERA_DESC					m_cameraDesc;
	VIEW_VOLUME					m_ViewVolume;

	WORD						m_wWidth;
	WORD						m_wHeight;
	SHORT_RECT					m_rcClip;
	SHORT_RECT					m_rcWindow;
	RECT						m_rcDirty;
	D3DVIEWPORT8				m_vpCurrent;
	DISPLAY_TYPE				m_dispType;
		
	DWORD						m_dwAmbientColor;
	D3DCOLORVALUE				m_dvcAmbient;
	DWORD						m_dwEmissiveColor;
	D3DCOLORVALUE				m_dvcEmissive;


	DWORD						m_dwBehaviorFlags;
	D3DPRESENT_PARAMETERS		m_d3dpp; 
	BOOL						m_bCanUseCompressedTexture;
	D3DFORMAT					m_fmtDefaultCompress;


	DWORD						m_dwRenderedMeshObjectNum;
	DWORD						m_dwRenderedSpriteObjectNum;
	DWORD						m_dwRenderedFontObjectNum;
	
	DWORD						m_dwRenderedFacesNum;
	DWORD						m_dwRenderedVertexNum;

	DWORD						m_dwRedneredLitFacesNum;
	DWORD						m_dwRedneredShadowedFacesNum;
	
	DWORD						m_dwRenderHFObjectNum;
	DWORD						m_dwCacheMissHFObjectNum;
	
	DWORD						m_dwHFVBNum;
	DWORD						m_dwHFIBNum;

	
	DWORD						m_dwShadowTexSize;
	DWORD						m_dwShadowTextureNum;
	SPOT_LIGHT_DESC				m_spotLightDesc[MAX_SPOT_LIGHT_NUM_PER_SCENE];

	
	LIGHT_DESC					m_RTLightList[MAX_REALTIME_LIGHT_NUM];

	LPDIRECT3D8					m_pD3D;
	LPDIRECT3DDEVICE8			m_pD3DDevice;
	
	HWND						m_hWindow;
	DWORD						m_dwFramePerSec;
	DWORD						m_dwColorDepth;
	DWORD						m_dwRenderMode;
	DWORD						m_dwShadowFlag;

	PERFORMANCE_CONTEXT			m_cxtPerformance;
	DWORD						m_dwBeginClockHI;
	DWORD						m_dwBeginClockLO;
	DWORD						m_dwEndClockHI;
	
	DWORD						m_dwRenderStatus;
	DWORD						m_dwLightMapFlag;
	
	BOOL						m_bDirectionalLight;
	BOOL						m_bFog;
	float						m_fFogDensity;
	float						m_fStart;
	float						m_fEnd;
	DWORD						m_dwFogColor;

	float						m_fAttentuation0;
	float						m_fAttentuation1;
	float						m_fAttentuation2;
	float						m_fDefaultSpecularPower;
	DWORD						m_dwSpecularMask;
		
	DWORD						m_dwTickCount;
	BOOL						m_bGameFrame;
	DWORD						m_dwShader[MAX_SHADER_NUM];
	
	DWORD						m_dwDefaultLightIndex;
	CProjectionLight*			m_pTexProjectionLightTexture[MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM];
	DWORD						m_dwTexProjectionDynamicLightNum;


	VLMESH_EFFECT_DESC*			m_pVLMeshEffectDescList;
	DWORD						m_dwVLMeshEffectNum;

	CCircle						m_Circle;
	SYSTEM_STATUS				m_Status;
	char						m_szSysInfo1[256];
	char						m_szSysInfo2[256];
	RECT						m_rcSysInfo1;
	RECT						m_rcSysInfo2;
	BOOL						m_bVSync;
	
	DWORD						m_dwRenderTargetTexelSize;
	DWORD						m_dwRenderTargetNum;
	DWORD						m_dwAlphaRefValue;	

	//trustpak 2005/06/08

	BOOL						m_bRenderWireSolidBothMode;

	///

	SORT_LINK*					m_pHFieldHead;
	SORT_LINK*					m_pHFieldTail;
	DWORD						m_dwHFieldNum;

	void						Cleanup();

	void							DefaultEnvSet();
	
//	void							InitializeShader();



public:

    STDMETHODIMP					QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)			AddRef(void);
    STDMETHODIMP_(ULONG)			Release(void);

	BOOL			__stdcall		Create(HWND hWnd,DISPLAY_INFO* pInfo,I4DyuchiFileStorage* pFileStorage,ErrorHandleProc pErrorHandleFunc);
	BOOL			__stdcall		InitializeRenderTarget(DWORD dwTexelSize,DWORD dwMaxTexNum);
	
	
	IDISpriteObject*	__stdcall	CreateSpriteObject(char* szFileName,DWORD dwFlag);
	IDISpriteObject*	__stdcall	CreateSpriteObject(char* szFileName,DWORD dwXPos,DWORD dwYPos,DWORD dwWidth,DWORD dwHeight,DWORD dwFlag);
	IDISpriteObject*	__stdcall	CreateEmptySpriteObject( DWORD dwWidth,DWORD dwHeight, TEXTURE_FORMAT TexFormat, DWORD dwFlag);
	IDIMeshObject*		__stdcall	CreateMeshObject(CMeshFlag flag);
	IDIFontObject*		__stdcall	CreateFontObject(LOGFONT* pLogFont,DWORD dwFlag);
	IDIHeightField*		__stdcall	CreateHeightField(DWORD dwFlag);
	IDIMeshObject*		__stdcall	CreateImmMeshObject(IVERTEX* piv3Tri,DWORD dwTriCount,void* pMtlHandle,DWORD dwFlag);
	


	void			__stdcall		BeginRender(SHORT_RECT* pRect,DWORD dwColor,DWORD dwFlag);
	void			__stdcall		EndRender();

	void			__stdcall		SetShadowFlag(DWORD dwFlag);
	DWORD			__stdcall		GetShadowFlag();
	void			__stdcall		SetLightMapFlag(DWORD dwFlag);
	DWORD			__stdcall		GetLightMapFlag();	
	void			__stdcall		SetRenderMode(DWORD dwFlag);
	DWORD			__stdcall		GetRenderMode();

	void			__stdcall		EnableFog(float fStart,float fEnd,float fDensity,DWORD dwColor,DWORD dwFlag);
	void			__stdcall		DisableFog();
	
	

	void			__stdcall		GetClientRect(SHORT_RECT* pRect,WORD* pwWidth,WORD* pwHeight);
		
	BOOL			__stdcall		BeginShadowMap();
	void			__stdcall		EndShadowMap();

	BOOL			__stdcall		CreateEffectShaderPaletteFromFile(char* szFileName);
	BOOL			__stdcall		CreateEffectShaderPalette(CUSTOM_EFFECT_DESC* pEffectDescList,DWORD dwNum);
	void			__stdcall		DeleteEffectShaderPalette();

	BOOL			__stdcall		RenderMeshObject(
		IDIMeshObject* pMeshObj,DWORD dwRefIndex,float fDistance,DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag);
	BOOL			__stdcall		RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag);
	BOOL			__stdcall		RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag);
	
	void			__stdcall		RenderBox(VECTOR3* pv3Oct,DWORD dwColor);
	void			__stdcall		RenderPoint(VECTOR3* pv3Point,DWORD dwColor);
	void			__stdcall		RenderCircle(VECTOR2* pv2Point,float fRs,DWORD dwColor);
	void			__stdcall		RenderLine(VECTOR2* pv2Point0,VECTOR2* pv2Point1,DWORD dwColor);
	void			__stdcall		RenderGrid(VECTOR3* pv3Quad,DWORD dwColor);
	BOOL			__stdcall		RenderTriIvertex(IVERTEX* piv3Tri,void* pMtlHandle,DWORD dwFacesNum,DWORD dwFlag);
	BOOL			__stdcall		RenderTriVector3(VECTOR3* pv3Tri,DWORD dwFacesNum,DWORD dwFlag);
		
	void*			__stdcall		AllocRenderTriBuffer(IVERTEX** ppIVList,DWORD dwFacesNum,DWORD dwRenderFlag);
	void			__stdcall		EnableRenderTriBuffer(void* pTriBufferHandle,void* pMtlHandle,DWORD dwRenderFacesNum);
	void			__stdcall		DisableRenderTriBuffer(void* pTriBufferHandle);
	void			__stdcall		FreeRenderTriBuffer(void* pTriBufferHandle);


	BOOL			__stdcall		SetRTLight(LIGHT_DESC* pLightDesc,DWORD dwLightIndex,DWORD dwFlag);
	void			__stdcall		EnableDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLightDesc,DWORD dwFlag);
	void			__stdcall		DisableDirectionalLight();

	void			__stdcall		SetSpotLightDesc(VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up,float fFov,float fNear,float fFar,float fWidth,BOOL bOrtho,void* pMtlHandle,DWORD dwColorOP,DWORD dwLightIndex,SPOT_LIGHT_TYPE type);
	void			__stdcall		SetShadowLightSenderPosition(BOUNDING_SPHERE* pSphere,DWORD dwLightIndex);

	void			__stdcall		SetViewFrusturm(VIEW_VOLUME* pViewVolume,CAMERA_DESC* camera,MATRIX4* pMatView,MATRIX4* pMatProj,MATRIX4* pMatForBilboard);
	DWORD			__stdcall		CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName);
	BOOL			__stdcall		DeleteDynamicLight(DWORD dwIndex);
	void			__stdcall		GetSystemStatus(SYSTEM_STATUS* pStatus);
	void			__stdcall		UpdateWindowSize();
	void			__stdcall		Present(HWND hWnd);
	void			__stdcall		SetAmbientColor(DWORD dwColor);
	DWORD			__stdcall		GetAmbientColor();
	void			__stdcall		SetEmissiveColor(DWORD dwColor);
	DWORD			__stdcall		GetEmissiveColor();
	void			__stdcall		BeginPerformanceAnalyze();
	void			__stdcall		EndPerformanceAnalyze();
	BOOL			__stdcall		CaptureScreen(char* szFileName);
	
	DWORD			__stdcall		CreateMaterialSet(MATERIAL_TABLE* pMtlEntry,DWORD dwNum);
	void			__stdcall		SetMaterialTextureBorder(void* pMtlHandle,DWORD dwColor);
	void			__stdcall		DeleteMaterialSet(DWORD dwMtlSetIndex);

	void*			__stdcall		CreateMaterial(MATERIAL* pMaterial,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag);
	void			__stdcall		DeleteMaterial(void* pMtlHandle);
			
	void			__stdcall		SetAttentuation0(float att);
	float			__stdcall		GetAttentuation0();
	BOOL			__stdcall		IsCompressedTextureFormatOk();
	BOOL			__stdcall		ConvertCompressedTexture(char* szFileName,DWORD dwFlag);
	void			__stdcall		EnableSpecular(float fVal);
	void			__stdcall		DisableSpecular();
	void			__stdcall		SetVerticalSync(BOOL bSwitch);
	BOOL			__stdcall		IsSetVerticalSync();
	void			__stdcall		ResetDevice(BOOL bTest);
	void			__stdcall		SetFreeVBCacheRate(float fVal);
	float			__stdcall		GetFreeVBCacheRate();
	DWORD			__stdcall		ClearVBCacheWithIDIMeshObject(IDIMeshObject* pObject);
	DWORD			__stdcall		ClearCacheWithMotionUID(void* pMotionUID);
	void			__stdcall		SetTickCount(DWORD dwTickCount,BOOL bGameFrame);
	BOOL			__stdcall		GetD3DDevice(REFIID refiid,void** ppVoid);
	void			__stdcall		SetRenderTextureMustUpdate(BOOL bMustUpdate);
	void			__stdcall		SetAlphaRefValue(DWORD dwAlphaRefValue);

	//trustpak 2005/04/04
	BOOL			__stdcall		SetLoadFailedTextureTable(TEXTURE_TABLE* pLoadFailedTextureTable, DWORD dwLoadFailedTextureTableSize);
	VOID			__stdcall		GetLoadFailedTextureTable(TEXTURE_TABLE** ppoutLoadFailedTextureTable, DWORD* poutdwLoadFailedTextureTableSize, DWORD* poutdwFailedTextureCount);
	///

	//trustpak 2005/06/08			// 아래 기능들은 실제 게임에서 사용해서는 안됩니다. 툴을 위해서만 존재하는 기능임.
	VOID			__stdcall		SetRenderWireSolidBothMode(BOOL bMode);
	BOOL			__stdcall		GetRenderWireSolidBothMode(VOID);
	///

//	void			__stdcall		EnableZBuffer();
//	void			__stdcall		DisableZBuffer();

	


	BOOL							CreateResourcePool(IResourcePool** ppResourcePool,RESORUCE_POOL_TYPE type);
	BOOL							ReleaseResourcePool(IResourcePool* pResourcePool);
	void							OutputResourcePoolStatus();

	HWND							GetWindowHandle()		{return m_hWindow;}
	BOOL							IsCanUseCompressedTexture()	{return m_bCanUseCompressedTexture;}
	D3DFORMAT						GetDefaultCopmressedTextureFormat() { return m_fmtDefaultCompress;}

	CD3DResourceManager*			GetD3DResourceManager() {return &m_rcManager;}
//	CProjectedIBManager*			GetPrjIBManager()		{return &m_PrjIBManager;}
	CVBCache*						GetVBCache() {return &m_VBCache;}
	CIBCache*						GetIBCache() {return &m_IBCache;}

	CVBHeap*						GetVBHeapForImmMesh()		{return m_pVBHeapForImmMesh;}
	CVBHeap*						GetVBVLMeshHeap() {return m_pVBVLMeshHeap;}
	CVBHeap*						GetVBPLMeshHeap() {return m_pVBPLMeshHeap;}
	CVBHeap*						GetVBVLSMeshHeap() {return m_pVBVLSMeshHeap;}
	CVBHeap*						GetVBLMMeshHeap() {return m_pVBLMMeshHeap;}
	CVBHeap*						GetVBVector3MeshHeap() {return m_pVBVector3MeshHeap;}
	CVBHeap*						GetVBSpriteHeap() {return m_pVBSpriteHeap;}

	CIBHeap*						GetIBHeapManaged() {return m_pIBHeapManaged;}
	CIBHeap*						GetIBHeapSprite() {return m_pIBHeapSprite;}

	CFontCache*						GetFontCache() {return m_pFontCache;}

	void							RenderGridWithTexture(VECTOR3* pv3Quad,DWORD dwRenderMode);
	BOOL							SetRenderTexture(DWORD dwIndex,RENDER_TEXTURE_TYPE type);

	DIMATERIAL*						AllocMaterial(MATERIAL* pMtl,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag) {return m_pMaterialManager->Alloc(pMtl,pdwWidth,pdwHeight,dwFlag);}
	void							FreeMaterial(DIMATERIAL* pMtl) {m_pMaterialManager->Free(pMtl);}

	BOOL							AllocTexture(CTexture* pTextureHandle,TCHAR* uszFileName,DWORD dwWidthHeight,BOOL bUseMipmap,BOOL bCompress,TEXTURE_MAP_TYPE texType) {return m_pTextureManager->AllocTexture(pTextureHandle,uszFileName,dwWidthHeight,bUseMipmap,bCompress,texType);}
	void							AllocDefaultDiffuseTexture(CTexture* pTexHandle) {m_pTextureManager->AllocDefaultDiffuseTexture(pTexHandle);}
	void							AllocDefaultBumpTexture(CTexture* pTexHandle) {m_pTextureManager->AllocDefaultBumpTexture(pTexHandle);}
	LPDIRECT3DDEVICE8				GetDevice() {return m_pD3DDevice;}
	static MATRIX4*					GetIdentityMatrix() {return (MATRIX4*)&m_matIdentity;}
	CTextureManager*				GetTextureManager() {return m_pTextureManager;}
	void							SetDefaultDiffuseTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex) {m_pTextureManager->SetDefaultDiffuseTexture(pDevice,dwTexStageIndex);}
	void							SetDefaultBumpTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex) {m_pTextureManager->SetDefaultBumpTexture(pDevice,dwTexStageIndex);}
	void							SetPrjTexMatrix(SPOT_LIGHT_DESC* pSpotDesc);


	SPOT_LIGHT_DESC*				INL_GetSpotDesc(DWORD dwIndex) {return &m_spotLightDesc[dwIndex];}
	
	VLMESH_EFFECT_DESC*				INL_GetVLMeshEffect(DWORD dwIndex) 
	{
		if (dwIndex < m_dwVLMeshEffectNum)
			return m_pVLMeshEffectDescList + dwIndex;
		else
			return NULL;

	//	return (VLMESH_EFFECT_DESC*)( (DWORD)( m_pVLMeshEffectDescList + dwIndex) & ( (((m_dwVLMeshEffectNum - dwIndex - 1) & 0x80000000) >> 31) - 1 ) );
		//return m_pVLMeshEffectDescList+dwIndex;
	}
	void							DecHFieldNum() {m_dwHFieldNum--;}

	DWORD							INL_GetAlphaRefVaule() {return m_dwAlphaRefValue;}
	DWORD							INL_GetLightMapFlag() {return m_dwLightMapFlag;}
	DWORD							INL_GetShadowFlag()		{return m_dwShadowFlag;}
	BOOL							INL_IsEnableFog()		{return m_bFog;}
	BOOL							INL_IsEnableDrectionalLight() {return m_bDirectionalLight;}
	MATRIX4*						INL_GetViewMatrix() {return &m_matView;}
	MATRIX4*						INL_GetProjMatrix() {return &m_matProj;}
	MATRIX4*						INL_GetBilBoardMatrix() {return &m_matForBilBoard;}
	MATRIX4*						INL_GetInverseViewMatriex() {return &m_matViewInverse;}
	DWORD							INL_GetTickCount() {return m_dwTickCount;}

	
	



	void							INL_GetEmissiveColor(D3DCOLORVALUE* pColorValue) {*pColorValue = m_dvcEmissive;}
	void							INL_GetAmbientColor(D3DCOLORVALUE* pColorValue) {*pColorValue = m_dvcAmbient;}
	float							INL_GetDefaultSpecularPower() {return m_fDefaultSpecularPower;}
	BOOL							INL_IsEnableSpecular() {return m_dwSpecularMask & 0x0000001;}
	DWORD							INL_GetSpecularMask() {return m_dwSpecularMask;}

	LIGHT_DESC*						INL_GetRTLightDesc(DWORD dwIndex) {return m_RTLightList+dwIndex;}


	CAMERA_DESC*					INL_GetCameraDesc() {return &m_cameraDesc;}
	VIEW_VOLUME*					INL_GetViewVolume() {return &m_ViewVolume;}



	float							INL_GetAttentuation0()	{return m_fAttentuation0;}
	float							INL_GetAttentuation1()	{return m_fAttentuation1;}
	float							INL_GetAttentuation2()	{return m_fAttentuation2;}

	void							SetSphereMapMatrix(MATRIX4* pResultMat,MATRIX4* pMatWorld,MATRIX4* pMatView);
	void							SetWaveTexMatrix(MATRIX4* pMatResult);
	DWORD							GetShader(DWORD dwIndex) {return m_dwShader[dwIndex];}
	DIMATERIAL*						GetMaterial(DWORD dwMtlSetIndex,DWORD dwFaceGroupIndex) {return m_pMaterialManager->GetMaterial(dwMtlSetIndex,dwFaceGroupIndex);}
	DIMATERIAL*						GetDefaultMaterial() {return m_pMaterialManager->GetDefaultMaterial();}


	LPDIRECT3DTEXTURE8				GetDynamicLightTexture(DWORD dwIndex);
	
//	BOOL							BeginCreateProjectionMap(DWORD dwShadowTextureIndex,DWORD dwFlag);
//	BOOL							EndCreateProjectionMap(DWORD dwShadowTextureIndex);

	void							AddLitFacesCount(DWORD dwFacesNum) {m_dwRedneredLitFacesNum+= dwFacesNum;}
	void							AddShadowedFacesCount(DWORD dwFacesNum) {m_dwRedneredShadowedFacesNum += dwFacesNum;}
	
	void							AddFacesNum(DWORD dwNum) {m_dwRenderedFacesNum += dwNum;}
	void							AddVerticesNum(DWORD dwNum) {m_dwRenderedVertexNum += dwNum;}

	void							AddHFVBNum(DWORD dwNum) {m_dwHFVBNum += dwNum;}
	void							AddHFIBNum(DWORD dwNum) {m_dwHFIBNum += dwNum;}

	void							LinkHFieldManager(CHFieldManager* pHFieldManager);
	void							UnLinkHFieldManager(CHFieldManager* pHFieldManager);
	
	



	void							SubHFVBNum(DWORD dwNum) {m_dwHFVBNum -= dwNum;}
	void							SubHFIBNum(DWORD dwNum) {m_dwHFIBNum -= dwNum;}

	void							IncRenderHFObjNum()		{m_dwRenderHFObjectNum++;}
	void							IncCacheMissRenderHFObjNum()	{m_dwCacheMissHFObjectNum++;}
	void							DeleteImmMeshObject(CImmMeshObject* pMeshObj);

	CoD3DDevice();
	~CoD3DDevice();
};




