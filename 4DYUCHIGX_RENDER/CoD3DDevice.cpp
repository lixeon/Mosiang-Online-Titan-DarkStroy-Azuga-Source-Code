#include "CoD3DDevice.h"
#include "renderer_typedef.h"
#include "VLMeshObject.h"
#include "VLSMeshObject.h"
#include "PLMeshObject.h"
#include "LMMeshObject.h"
#include "BillBoard.h"

#include "font.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "HFieldObject.h"
#include "3dsprite.h"
#include "render_type.h"
#include "ProjectionLight.h"
#include "FontObject.h"
#include "FontCache.h"
#include <crtdbg.h>
#include "ImmMeshObject.h"
#include "RenderTexturePool.h"

#include "IBHeap.h"
#include "VBHeap.h"
#include "StaticVBPool.h"
#include "StaticIBPool.h"

extern LONG			g_lComponent;

I4DyuchiFileStorage*			g_pFileStorage = NULL;


MATRIX4 CoD3DDevice::m_matIdentity;
ErrorHandleProc		g_pErrorHandleFunc = NULL;
DWORD __stdcall DefaultErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	/*
	char szFileName[] = __FILE__;
	int iLineNumber = __LINE__;*/
	char	txt[512];
	memset(txt,0,512);

	wsprintf(txt,"File not found, %s\n",szStr);
	OutputDebugString(txt);

	if (!dwErrorPriority)
		__asm int 3

	return 0;
}

BOOL __stdcall CoD3DDevice::GetD3DDevice(REFIID refiid,void** ppVoid)
{
	BOOL		bResult = FALSE;

	
	*ppVoid = NULL;
	
	if (refiid != IID_IDirect3DDevice8)
		goto lb_return;

	m_pD3DDevice->AddRef();
	*ppVoid = m_pD3DDevice;


	bResult = TRUE;
lb_return:
	return bResult;
}
STDMETHODIMP CoD3DDevice::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiGXRenderer == refiid)
		*ppv = (I4DyuchiGXRenderer*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoD3DDevice::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CoD3DDevice::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CoD3DDevice::CoD3DDevice()
{
	InterlockedIncrement( &g_lComponent);
	
//	memset(((char*)this)+4,0,sizeof(CoD3DDevice)-4);
	m_dwLightMapFlag = 1;
#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif

	m_pFontCache = NULL;
	m_pDPCQFor2DItem = NULL;
	m_dwRefCount = 0;
	m_pFont = NULL;

//	MERA_DESC					m_cameraDesc;
//	VIEW_VOLUME					m_ViewVolume;

	m_wWidth = 0;
	m_wHeight = 0;
	memset(&m_rcClip,0,sizeof(SHORT_RECT));
	memset(&m_rcWindow,0,sizeof(SHORT_RECT));
	memset(&m_rcDirty,0,sizeof(RECT));
	m_dispType = WINDOW_WITH_BLT;
		
	m_dwAmbientColor = 0;
	m_dwEmissiveColor = 0;

	m_dvcAmbient.r = 0.0f;
	m_dvcAmbient.g = 0.0f;
	m_dvcAmbient.b = 0.0f;
	m_dvcEmissive.r = 0.0f;
	m_dvcEmissive.g = 0.0f;
	m_dvcEmissive.b = 0.0f;
	m_pFileStorage = NULL;

	m_dwBehaviorFlags = 0;
	m_bCanUseCompressedTexture = FALSE;
	
	m_pVLMeshEffectDescList = NULL;
	m_dwVLMeshEffectNum = 0;


	m_dwRenderedMeshObjectNum = 0;
	m_dwRenderedSpriteObjectNum = 0;
	m_dwRenderedFontObjectNum = 0;
	m_dwRenderedFacesNum = 0;
	m_dwRenderedVertexNum = 0;
	m_dwRedneredLitFacesNum = 0;
	m_dwRedneredShadowedFacesNum = 0;
	
	m_dwRenderHFObjectNum = 0;
	m_dwCacheMissHFObjectNum = 0;
	m_dwHFVBNum = 0;
	m_dwHFIBNum = 0;
	m_dwShadowTexSize = 0;
	m_dwShadowTextureNum = 0;

	memset(m_spotLightDesc,0,sizeof(m_spotLightDesc));
	memset(m_RTLightList,0,sizeof(m_RTLightList));

	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	
	m_hWindow = NULL;
	m_dwFramePerSec = 0;
	m_dwColorDepth = 0;
	m_dwRenderMode = 0;
	m_dwShadowFlag = 0;

	memset(&m_cxtPerformance,0,sizeof(PERFORMANCE_CONTEXT));
	
	m_dwBeginClockHI = 0;
	m_dwBeginClockLO = 0;
	m_dwEndClockHI = 0;
	
	m_dwRenderStatus = 0;
	m_dwLightMapFlag = 1;
	
	m_bDirectionalLight = 0;
	m_bFog = 0;
	m_fFogDensity = 0.0f;
	m_fStart = 0.0f;
	m_fEnd = 0.0f;
	m_dwFogColor = 0;

	m_fAttentuation0 = 0.5f;
	m_fAttentuation1 = 0.0f;
	m_fAttentuation2 = 0.0f;


	m_fDefaultSpecularPower = 0.0f;
	m_dwSpecularMask = 0;
		
	memset(m_dwShader,0,sizeof(DWORD)*MAX_SHADER_NUM);
	
	m_dwDefaultLightIndex = 0;
	memset(m_pTexProjectionLightTexture,0,sizeof(CProjectionLight*)*MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM);
	m_dwTexProjectionDynamicLightNum = 0;


	memset(m_szSysInfo1,0,256);
	memset(m_szSysInfo2,0,256);
	memset(&m_rcSysInfo1,0,sizeof(RECT));
	memset(&m_rcSysInfo2,0,sizeof(RECT));

	memset(m_pResourcePoolList,0,sizeof(m_pResourcePoolList));
	m_dwResourcePoolNum = 0;
	m_dwHFieldNum = 0;

	m_dwAlphaRefValue = DEFAULT_ALPHA_REF_VALUE;

	//trustpak 2005/06/08
	m_bRenderWireSolidBothMode = FALSE;



	///

	m_pHFieldHead = NULL;
	m_pHFieldTail = NULL;
}

void __stdcall CoD3DDevice::SetAlphaRefValue(DWORD dwAlphaRefValue)
{
	m_dwAlphaRefValue = 0x000000ff & dwAlphaRefValue;
	m_pD3DDevice->SetRenderState( D3DRS_ALPHAREF, (DWORD)m_dwAlphaRefValue);
	m_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
}


//trustpak 2005/04/04
BOOL __stdcall CoD3DDevice::SetLoadFailedTextureTable(TEXTURE_TABLE* pLoadFailedTextureTable, DWORD dwLoadFailedTextureTableSize)
{
	if (NULL != m_pTextureManager)
	{
		if (FALSE == m_pTextureManager->SetLoadFailedTextureTable(pLoadFailedTextureTable, dwLoadFailedTextureTableSize))
		{
			return FALSE;
		}
	}

	return TRUE;
}
VOID __stdcall CoD3DDevice::GetLoadFailedTextureTable(TEXTURE_TABLE** ppoutLoadFailedTextureTable, DWORD* poutdwLoadFailedTextureTableSize, DWORD* poutdwFailedTextureCount)
{
	if (NULL != ppoutLoadFailedTextureTable && NULL != poutdwLoadFailedTextureTableSize)
	{
		if (NULL != m_pTextureManager)
		{
			m_pTextureManager->GetLoadFailedTextureTable(ppoutLoadFailedTextureTable, 
														 poutdwLoadFailedTextureTableSize, 
														 poutdwFailedTextureCount);
		}
	}
}


///


//trustpak 2005/06/08			// 아래 기능들은 실제 게임에서 사용해서는 안됩니다. 툴을 위해서만 존재하는 기능임.
VOID __stdcall CoD3DDevice::SetRenderWireSolidBothMode(BOOL bMode)
{
	m_bRenderWireSolidBothMode = bMode;
}

BOOL __stdcall CoD3DDevice::GetRenderWireSolidBothMode(VOID)
{
	return m_bRenderWireSolidBothMode;
}
///


/*
BOOL __stdcall CoD3DDevice::Create(HWND hWnd,DISPLAY_INFO* pInfo,I4DyuchiFileStorage* pFileStorage,ErrorHandleProc pErrorHandleFunc)

{
	BOOL	bResult = FALSE;

	m_pFileStorage = pFileStorage;
	g_pFileStorage = pFileStorage;

	m_pFileStorage->AddRef();

	g_pErrorHandleFunc = pErrorHandleFunc;
	if (!g_pErrorHandleFunc)
		g_pErrorHandleFunc = DefaultErrorHandleProc;

	m_hWindow = hWnd;
	
	
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( m_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
		goto lb_return;

    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );

	RECT	rect;
	D3DDISPLAYMODE d3ddm;

	if (!pInfo)
		goto lb_windowed;

	
	if (!pInfo->bWindowed)
		goto lb_full_screen;

//	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
lb_windowed:	

	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		goto lb_return;

	m_bWindowed = TRUE;
	m_bVSync = FALSE;
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
 	m_d3dpp.EnableAutoDepthStencil = TRUE;
	// 	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;//DFMT_D16;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
 	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.hDeviceWindow = hWnd;
		


	::GetClientRect(hWnd,&rect);
	m_rcWindow.left = (short)rect.left;
	m_rcWindow.right = (short)rect.right;
	m_rcWindow.top = (short)rect.top;
	m_rcWindow.bottom = (short)rect.bottom;
	
	m_wWidth = m_rcWindow.right - m_rcWindow.left;
	m_wHeight = m_rcWindow.bottom - m_rcWindow.top;

	m_d3dpp.BackBufferWidth = m_wWidth;
	m_d3dpp.BackBufferHeight  = m_wHeight;
	

//	m_d3dpp.BackBufferWidth = d3ddm.Width;
//	m_d3dpp.BackBufferHeight  = d3ddm.Height;
	
	goto lb_create;
	
lb_full_screen:
	
	

	

	m_bVSync = TRUE;
	DEVMODE DevMode;
	memset(&DevMode,0,sizeof(DevMode));
	DevMode.dmBitsPerPel = pInfo->dwBPS*8;
	DevMode.dmPelsWidth = pInfo->dwWidth;
	DevMode.dmPelsHeight = pInfo->dwHeight;
	DevMode.dmSize=sizeof(DevMode);
	
	DevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
	ChangeDisplaySettings(&DevMode,0);

	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		goto lb_return;
	
	m_bWindowed = FALSE;
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
 	m_d3dpp.EnableAutoDepthStencil = TRUE;
	// 	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;//DFMT_D16;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
 	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.hDeviceWindow = hWnd;
		
	
	SetWindowPos(hWnd,HWND_TOP,0,0,pInfo->dwWidth,pInfo->dwHeight,0);
	SetWindowLong( hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
	SetMenu( hWnd, NULL );

	
	::GetClientRect(hWnd,&rect);
	m_rcWindow.left = (short)rect.left;
	m_rcWindow.right = (short)rect.right;
	m_rcWindow.top = (short)rect.top;
	m_rcWindow.bottom = (short)rect.bottom;
	
	m_wWidth = m_rcWindow.right - m_rcWindow.left;
	m_wHeight = m_rcWindow.bottom - m_rcWindow.top;

	m_d3dpp.BackBufferWidth = d3ddm.Width;
	m_d3dpp.BackBufferHeight  = d3ddm.Height;


lb_create:
	m_dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	wsprintf(m_Status.szDeviceType,"HAL HW-VP");
lb_create_device:
//	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ;
//	m_d3dpp.FullScreen_RefreshRateInHz = 85;;
    

	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
										GetDesktopWindow(),
										m_dwBehaviorFlags,
										&m_d3dpp, &m_pD3DDevice ) ) )
    {
		if (m_dwBehaviorFlags != D3DCREATE_SOFTWARE_VERTEXPROCESSING)
		{
			m_dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			wsprintf(m_Status.szDeviceType,"HAL SW-VP");
			goto lb_create_device;
		}								
		MessageBox(NULL,"Fail to Create D3DDevice","Error",MB_OK);
		goto lb_return;
    }
	m_bCanUseCompressedTexture = IsCompressedTextureFormatOk();


	// 리소스 매니져부터 초기화한다.
	m_rcManager.Initialize(this,m_pD3DDevice,8192*4);

	m_pTextureManager = new CTextureManager;

	if (m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8)
		m_pTextureManager->SetBytesPerPixel(4);
	else 
		m_pTextureManager->SetBytesPerPixel(2);

	DefaultEnvSet();

	SetIdentityMatrix((MATRIX4*)&m_matIdentity);    
	
	
	m_pTextureManager->Initialize(this,&m_rcManager,MAX_TEXTURE_NUM,MAX_TEXBUCKET_NUM);

		
	// 매쉬 오브젝트용 렌더링 큐
	m_RenderQ.Initialize(sizeof(RENDER_MESHOBJECT_DESC),MAX_RENDER_MESHOBJ_NUM);

	// 2D리소스용 dpc큐.zorder때문에 사용한다.
	m_pDPCQFor2DItem = DPCQCreate();
	DPCQInitialize(m_pDPCQFor2DItem,16384);

	// 폰트 오브젝트용 렌더링 큐
	m_TextBuffer.Initialize(MAX_RENDER_TEXTBUFFER_SIZE);

	// 스프라이트용 렌더링 큐
	m_SpriteBuffer.Initialize(MAX_RENDER_SPRITE_NUM,MAX_SPRITE_ZORDER_NUM);

	// 외부 입력 삼각형 렌더링 큐
	m_RenderTriBuffer.Initialize(this,256,4096);


	// 원을 그리기 위한 써클 클래스 초기화.
	m_Circle.Initialize(DEFULAT_CIRCLE_PIECES_NUM);


	// 디폴트 다이나믹 라이트를 하나 만들어둔다..
	m_dwDefaultLightIndex = CreateDynamicLight(16,0x000000ff,NULL);
		
	m_VBCache.Initialize(this,4096,65536,2);
	m_IBCache.Initialize(this,2048,32768,1);

	m_pImmMeshObjectPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pImmMeshObjectPool,sizeof(CImmMeshObject),64,512);


	CreateResourcePool((IResourcePool**)&m_pVBHeapForImmMesh,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBHeapForImmMesh->Initialize(&m_rcManager,D3DFVF_IVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(IVERTEX),4096,32,0,0);

	CreateResourcePool((IResourcePool**)&m_pVBLMMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBLMMeshHeap->Initialize(&m_rcManager,D3DFVF_D3DDUALVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DDUALVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVector3MeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVector3MeshHeap->Initialize(&m_rcManager,D3DFVF_XYZ,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(VECTOR3),16384,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVLSMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVLSMeshHeap->Initialize(&m_rcManager,D3DFVF_VLSVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DVLSVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVLMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVLMeshHeap->Initialize(&m_rcManager,D3DFVF_VLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DVLVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBSpriteHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBSpriteHeap->Initialize(&m_rcManager,D3DFVF_D3DTLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DTLVERTEX),1024,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pIBHeapSprite,RESORUCE_POOL_TYPE_IBHEAP);
	m_pIBHeapSprite->Initialize(&m_rcManager,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,1024,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pIBHeapManaged,RESORUCE_POOL_TYPE_IBHEAP);
	m_pIBHeapManaged->Initialize(&m_rcManager,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,65536,32,0,0);


	

	m_pFontCache = new CFontCache;
	m_pFontCache->Initialize(this,1024,1024,256,4);
	m_pFont = CreateFontObject(NULL,0);

	// 매터리얼 매니져 초기화
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Initialize(this,MAX_MATERIAL_NUM,MAX_MATERIAL_SET_NUM,0);



	m_pRenderTexturePool = new CRenderTexturePool;
	m_dwRenderTargetTexelSize = DEFAULT_RENDER_TEXTURE_SIZE;
	m_dwRenderTargetNum = DEFAULT_RENDER_TEXTURE_NUM;
	m_pRenderTexturePool->Initialize(this,m_dwRenderTargetTexelSize,m_dwRenderTargetNum,0);
	

	


	bResult = TRUE;

lb_return:
	return bResult;
}
*/
/*	m_d3dpp.Windowed = FALSE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	//m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;


//	d3dpp.BackBufferFormat = d3ddm.Format;

	//	d3dpp.EnableAutoDepthStencil = TRUE;
	// 	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;//DFMT_D16;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
 	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.hDeviceWindow = hWnd;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.FullScreen_RefreshRateInHz = pInfo->dwRefreshRate;
		

	d3ddm.Width = m_d3dpp.BackBufferWidth  = pInfo->dwWidth;
	d3ddm.Height = m_d3dpp.BackBufferHeight  = pInfo->dwHeight;
	

	m_wWidth = (WORD)d3ddm.Width;
	m_wHeight = (WORD)d3ddm.Height;

	m_rcWindow.left = 0;
	m_rcWindow.top = 0;
	m_rcWindow.right = m_wWidth;
	m_rcWindow.bottom = m_wHeight;

	if (pInfo->dwBPS == 2)
	{
	
		m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}
	else if (pInfo->dwBPS == 4)
	{
		m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		
	}
	SetWindowLong( hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
	SetMenu( hWnd, NULL );
*/

BOOL __stdcall CoD3DDevice::Create(HWND hWnd,DISPLAY_INFO* pInfo,I4DyuchiFileStorage* pFileStorage,ErrorHandleProc pErrorHandleFunc)

{
	HRESULT			hr;
	BOOL	bResult = FALSE;

	m_pFileStorage = pFileStorage;
	g_pFileStorage = pFileStorage;

	m_pFileStorage->AddRef();

	g_pErrorHandleFunc = pErrorHandleFunc;
	if (!g_pErrorHandleFunc)
		g_pErrorHandleFunc = DefaultErrorHandleProc;

	m_hWindow = hWnd;
	
	
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( m_pD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
		goto lb_return;

    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );

	RECT	rect;
	D3DDISPLAYMODE d3ddm;

	if (!pInfo)
		goto lb_windowed;

	// 풀모드라면...백버퍼포맷을 체크한다.	
	if (pInfo->dwBPS == 2)
		m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	else if (pInfo->dwBPS == 4)
		m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	else
		goto lb_return;

	if (pInfo->dispType == FULLSCREEN_WITH_BLT)
		goto lb_full_screen;

	if (pInfo->dispType == FULLSCREEN_WITH_FLIP)
		goto lb_full_flip;

//	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
lb_windowed:	
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		goto lb_return;

	m_dispType = WINDOW_WITH_BLT;
	m_bVSync = FALSE;
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
 	m_d3dpp.EnableAutoDepthStencil = TRUE;
	// 	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;//DFMT_D16;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
 	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.hDeviceWindow = hWnd;
		


	::GetClientRect(hWnd,&rect);
	m_rcWindow.left = (short)rect.left;
	m_rcWindow.right = (short)rect.right;
	m_rcWindow.top = (short)rect.top;
	m_rcWindow.bottom = (short)rect.bottom;
	
	m_wWidth = m_rcWindow.right - m_rcWindow.left;
	m_wHeight = m_rcWindow.bottom - m_rcWindow.top;

	m_d3dpp.BackBufferWidth = m_wWidth;
	m_d3dpp.BackBufferHeight  = m_wHeight;
	

//	m_d3dpp.BackBufferWidth = d3ddm.Width;
//	m_d3dpp.BackBufferHeight  = d3ddm.Height;
	
	goto lb_create;
	
lb_full_screen:
	
	DEVMODE DevMode;
	memset(&DevMode,0,sizeof(DevMode));
	DevMode.dmBitsPerPel = pInfo->dwBPS*8;
	DevMode.dmPelsWidth = pInfo->dwWidth;
	DevMode.dmPelsHeight = pInfo->dwHeight;
	DevMode.dmSize=sizeof(DevMode);
	
	DevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
	ChangeDisplaySettings(&DevMode,0);
	if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
		goto lb_return;

	
	m_dispType = FULLSCREEN_WITH_BLT;
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_bVSync = TRUE;
	
	goto lb_fullscreen_common;


lb_full_flip:
	m_dispType = FULLSCREEN_WITH_FLIP;
	m_d3dpp.Windowed = FALSE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_d3dpp.FullScreen_RefreshRateInHz = pInfo->dwRefreshRate;
	m_d3dpp.BackBufferWidth = pInfo->dwWidth;
	m_d3dpp.BackBufferHeight  = pInfo->dwHeight;


lb_fullscreen_common:
	m_d3dpp.BackBufferWidth = pInfo->dwWidth;
	m_d3dpp.BackBufferHeight  = pInfo->dwHeight;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
 	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.hDeviceWindow = hWnd;
		
	
	SetWindowPos(hWnd,HWND_TOP,0,0,pInfo->dwWidth,pInfo->dwHeight,0);
	SetWindowLong( hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
	SetMenu( hWnd, NULL );

	
	::GetClientRect(hWnd,&rect);
	m_rcWindow.left = (short)rect.left;
	m_rcWindow.right = (short)rect.right;
	m_rcWindow.top = (short)rect.top;
	m_rcWindow.bottom = (short)rect.bottom;
	
	m_wWidth = m_rcWindow.right - m_rcWindow.left;
	m_wHeight = m_rcWindow.bottom - m_rcWindow.top;

	


lb_create:
	m_dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	wsprintf(m_Status.szDeviceType,"HAL HW-VP");
lb_create_device:
//	m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ;
//	m_d3dpp.FullScreen_RefreshRateInHz = 85;;
    
	hr	=	m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
										GetDesktopWindow(),
										m_dwBehaviorFlags,
										&m_d3dpp, &m_pD3DDevice );
	if( FAILED( hr) )
    {
		if (m_dwBehaviorFlags != D3DCREATE_SOFTWARE_VERTEXPROCESSING)
		{
			m_dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			wsprintf(m_Status.szDeviceType,"HAL SW-VP");
			goto lb_create_device;
		}								
		MessageBox(NULL,"Fail to Create D3DDevice","Error",MB_OK);
		goto lb_return;
    }
	m_bCanUseCompressedTexture = IsCompressedTextureFormatOk();


	// 리소스 매니져부터 초기화한다.
	m_rcManager.Initialize(this,m_pD3DDevice,8192*4);

	m_pTextureManager = new CTextureManager;

	if (m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8)
		m_pTextureManager->SetBytesPerPixel(4);
	else 
		m_pTextureManager->SetBytesPerPixel(2);

	DefaultEnvSet();

	SetIdentityMatrix((MATRIX4*)&m_matIdentity);    
	
	
	m_pTextureManager->Initialize(this,&m_rcManager,MAX_TEXTURE_NUM,MAX_TEXBUCKET_NUM);

		
	// 매쉬 오브젝트용 렌더링 큐
	m_RenderQ.Initialize(sizeof(RENDER_MESHOBJECT_DESC),MAX_RENDER_MESHOBJ_NUM);

	// 2D리소스용 dpc큐.zorder때문에 사용한다.
	m_pDPCQFor2DItem = DPCQCreate();
	//trustpak 2005/07/08
	//DPCQInitialize(m_pDPCQFor2DItem,16384);
	DPCQInitialize(m_pDPCQFor2DItem,32768);
	///

	// 폰트 오브젝트용 렌더링 큐
	//trustpak 2005/07/08
	//m_TextBuffer.Initialize(MAX_RENDER_TEXTBUFFER_SIZE);
	m_TextBuffer.Initialize(MAX_RENDER_TEXTBUFFER_SIZE * 2);
	//

	// 스프라이트용 렌더링 큐
	m_SpriteBuffer.Initialize(MAX_RENDER_SPRITE_NUM,MAX_SPRITE_ZORDER_NUM);

	// 외부 입력 삼각형 렌더링 큐
	m_RenderTriBuffer.Initialize(this,256,4096);


	// 원을 그리기 위한 써클 클래스 초기화.
	m_Circle.Initialize(DEFULAT_CIRCLE_PIECES_NUM);


	// 디폴트 다이나믹 라이트를 하나 만들어둔다..
	m_dwDefaultLightIndex = CreateDynamicLight(16,0x000000ff,NULL);
		
	m_VBCache.Initialize(this,4096,65536,2);
	m_IBCache.Initialize(this,2048,32768,1);

	m_pImmMeshObjectPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pImmMeshObjectPool,sizeof(CImmMeshObject),64,512);


	CreateResourcePool((IResourcePool**)&m_pVBHeapForImmMesh,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBHeapForImmMesh->Initialize(&m_rcManager,D3DFVF_IVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(IVERTEX),4096,32,0,0);

	CreateResourcePool((IResourcePool**)&m_pVBLMMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBLMMeshHeap->Initialize(&m_rcManager,D3DFVF_D3DDUALVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DDUALVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVector3MeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVector3MeshHeap->Initialize(&m_rcManager,D3DFVF_XYZ,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(VECTOR3),16384,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVLSMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVLSMeshHeap->Initialize(&m_rcManager,D3DFVF_VLSVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DVLSVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBVLMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBVLMeshHeap->Initialize(&m_rcManager,D3DFVF_VLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DVLVERTEX),32768,32,0,0);

	CreateResourcePool((IResourcePool**)&m_pVBPLMeshHeap,RESORUCE_POOL_TYPE_VBHEAP);
	m_pVBPLMeshHeap->Initialize(&m_rcManager,D3DFVF_PLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DPLVERTEX),32768,32,0,0);


	CreateResourcePool((IResourcePool**)&m_pVBSpriteHeap,RESORUCE_POOL_TYPE_VBHEAP);	
	m_pVBSpriteHeap->Initialize(&m_rcManager,D3DFVF_D3DTLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,sizeof(D3DTLVERTEX),1024,32,0,0);	
	
	
	
	CreateResourcePool((IResourcePool**)&m_pIBHeapSprite,RESORUCE_POOL_TYPE_IBHEAP);	
	m_pIBHeapSprite->Initialize(&m_rcManager,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,4096,32,0,0);

	CreateResourcePool((IResourcePool**)&m_pIBHeapManaged,RESORUCE_POOL_TYPE_IBHEAP);
	m_pIBHeapManaged->Initialize(&m_rcManager,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,65536,32,0,0);


	

	m_pFontCache = new CFontCache;
	m_pFontCache->Initialize(this,1024,1024,256,4);
	m_pFont = CreateFontObject(NULL,0);

	// 매터리얼 매니져 초기화
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Initialize(this,MAX_MATERIAL_NUM,MAX_MATERIAL_SET_NUM,0);



	m_pRenderTexturePool = new CRenderTexturePool;
	m_dwRenderTargetTexelSize = DEFAULT_RENDER_TEXTURE_SIZE;
	m_dwRenderTargetNum = DEFAULT_RENDER_TEXTURE_NUM;
	m_pRenderTexturePool->Initialize(this,m_dwRenderTargetTexelSize,m_dwRenderTargetNum,0);
	

//	g_pScreenFilter = new CScreenFilter;
//	g_pScreenFilter->Initialize(this,"d:\\4DyuchiGX\\4DyuchiGRX\\water.tga",800,600,0);
	


	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoD3DDevice::InitializeRenderTarget(DWORD dwTexelSize,DWORD dwMaxTexNum)
{
	
	if (dwMaxTexNum > MAX_RENDER_TEXTURE_NUM)
		dwMaxTexNum = MAX_RENDER_TEXTURE_NUM;

	if (dwTexelSize > MAX_RENDER_TEXTURE_SIZE)
		dwTexelSize = MAX_RENDER_TEXTURE_SIZE;

	if (m_pRenderTexturePool)
		m_pRenderTexturePool->Cleanup();
	
	m_dwRenderTargetTexelSize = dwTexelSize;
	m_dwRenderTargetNum = dwMaxTexNum;
	
	m_pRenderTexturePool->Initialize(this,m_dwRenderTargetTexelSize,m_dwRenderTargetNum,0);

	return TRUE;
}
void CoD3DDevice::SetFreeVBCacheRate(float fVal)
{
	m_VBCache.SetFreeVBCacheRate(fVal);
}
float CoD3DDevice::GetFreeVBCacheRate()
{
	return m_VBCache.GetFreeVBCacheRate();
}
	
void __stdcall CoD3DDevice::UpdateWindowSize()
{
	
	RECT	rect;
	::GetClientRect(m_hWindow,&rect);

	m_rcWindow.left = (short)rect.left;
	m_rcWindow.top = (short)rect.top;
	m_rcWindow.right = (short)rect.right;
	m_rcWindow.bottom = (short)rect.bottom;

	m_wWidth = m_rcWindow.right - m_rcWindow.left;
	m_wHeight = m_rcWindow.bottom - m_rcWindow.top;

	m_d3dpp.BackBufferWidth = m_wWidth;
	m_d3dpp.BackBufferHeight  = m_wHeight;

	ResetDevice(FALSE);


}
void __stdcall CoD3DDevice::SetAmbientColor(DWORD dwColor)
{

	m_dwAmbientColor = dwColor;
	m_pD3DDevice->SetRenderState( D3DRS_AMBIENT, m_dwAmbientColor );
	DWORDtoCOLOR(
		m_dwAmbientColor,
		&m_dvcAmbient.r,
		&m_dvcAmbient.g,
		&m_dvcAmbient.b,
		&m_dvcAmbient.a);
}
DWORD __stdcall CoD3DDevice::GetAmbientColor()
{
	return m_dwAmbientColor;
}
void __stdcall CoD3DDevice::SetEmissiveColor(DWORD dwColor)
{
	m_dwEmissiveColor = dwColor;

	DWORDtoCOLOR(
		m_dwEmissiveColor,
		&m_dvcEmissive.r,
		&m_dvcEmissive.g,
		&m_dvcEmissive.b,
		&m_dvcEmissive.a);

}
DWORD __stdcall CoD3DDevice::GetEmissiveColor()
{
	return m_dwEmissiveColor;

}
float __stdcall	CoD3DDevice::GetAttentuation0()
{
	return m_fAttentuation0;
}
void __stdcall	CoD3DDevice::SetAttentuation0(float att)
{
	m_fAttentuation0 = att;
}
BOOL __stdcall CoD3DDevice::CreateEffectShaderPalette(CUSTOM_EFFECT_DESC* pEffectDescList,DWORD dwNum)
{
	VLMESH_EFFECT_DESC* pEffectPaletteTemp = new VLMESH_EFFECT_DESC[dwNum];
	DWORD dwVLMeshEffectNumTemp = dwNum;
	memset(pEffectPaletteTemp,0,sizeof(VLMESH_EFFECT_DESC)*dwNum);

	for (DWORD i=0; i<dwNum; i++)
	{
		if (AllocTexture(&pEffectPaletteTemp[i].m_texture,pEffectDescList[i].szTexName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
		{
			pEffectPaletteTemp[i].bDisableSrcTex = pEffectDescList[i].bDisableSrcTex;
			pEffectPaletteTemp[i].method = pEffectDescList[i].method;
			pEffectPaletteTemp[i].dwFlag = pEffectDescList[i].dwFlag;
			pEffectPaletteTemp[i].bSuccess = TRUE;
		}
	}

	if (m_pVLMeshEffectDescList)
	{
		DeleteEffectShaderPalette();
	}
	m_pVLMeshEffectDescList = pEffectPaletteTemp;
	m_dwVLMeshEffectNum = dwVLMeshEffectNumTemp;

	
	return TRUE;
}
void __stdcall CoD3DDevice::DeleteEffectShaderPalette()
{
	if (m_pVLMeshEffectDescList)
	{
		delete [] m_pVLMeshEffectDescList;
		m_pVLMeshEffectDescList = NULL;
		m_dwVLMeshEffectNum = 0;
	}
}
BOOL __stdcall CoD3DDevice::CreateEffectShaderPaletteFromFile(char* szFileName)
{
	BOOL		bResult = FALSE;
	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	DWORD	dwVersion;
	DWORD	dwEffectNum;

	g_pFileStorage->FSRead(pFP,&dwVersion,sizeof(DWORD));
	g_pFileStorage->FSRead(pFP,&dwEffectNum,sizeof(DWORD));


	if (dwEffectNum >= MAX_EFFECT_SHADER_NUM)
	{
		goto lb_close;
	}

	CUSTOM_EFFECT_DESC*	pEffectTable;
	pEffectTable = new CUSTOM_EFFECT_DESC[dwEffectNum];
	memset(pEffectTable,0,sizeof(CUSTOM_EFFECT_DESC)*dwEffectNum);

	g_pFileStorage->FSRead(pFP,pEffectTable,sizeof(CUSTOM_EFFECT_DESC)*dwEffectNum);
	bResult = CreateEffectShaderPalette(pEffectTable,dwEffectNum);

	if (pEffectTable)
		delete pEffectTable;
lb_close:

	g_pFileStorage->FSCloseFile(pFP);
lb_return:
	return bResult;
	
}
DWORD __stdcall CoD3DDevice::CreateDynamicLight(DWORD dwRS,DWORD dwColor,char* szFileName)
{
	DWORD	dwResult = 0xffffffff;
	for (DWORD i=0; i<MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM; i++)
	{
		if (!m_pTexProjectionLightTexture[i])
		{
			m_pTexProjectionLightTexture[i] = new CProjectionLight;
			if (!m_pTexProjectionLightTexture[i]->Create(this,dwRS,dwColor,szFileName,0))
			{
				delete m_pTexProjectionLightTexture[i];
				goto lb_return;
			}

			m_dwTexProjectionDynamicLightNum++;
			dwResult = i;
			goto lb_return;
		}
	}
lb_return:
	return dwResult;


}

LPDIRECT3DTEXTURE8 CoD3DDevice::GetDynamicLightTexture(DWORD dwIndex)
{
	LPDIRECT3DTEXTURE8	pResult = NULL;
	if (dwIndex >= MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM)
		goto lb_return;


	pResult = m_pTexProjectionLightTexture[dwIndex]->GetTexture();
lb_return:
	return pResult;
}

BOOL __stdcall CoD3DDevice::DeleteDynamicLight(DWORD dwIndex)
{
	BOOL		bResult = FALSE;
	if (dwIndex >= MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM)
		goto lb_return;

	if (!m_pTexProjectionLightTexture[dwIndex])
		goto lb_return;
	
	delete m_pTexProjectionLightTexture[dwIndex];
	m_pTexProjectionLightTexture[dwIndex] = NULL;
	bResult = TRUE;

lb_return:
	return bResult;
}
void __stdcall CoD3DDevice::GetSystemStatus(SYSTEM_STATUS* pStatus)
{
	m_Status.dwAvaliableTexMem = m_pD3DDevice->GetAvailableTextureMem();
	*pStatus = m_Status;
}/*
void CoD3DDevice::InitializeShader()
{
     // Create vertex shader
    
	// test

//	LPD3DXBUFFER pCode;
//	HRESULT hr;

//	DWORD dwDecl[] =
  //  {
//		D3DVSD_STREAM(0),
//		D3DVSD_REG(0,D3DVSDT_FLOAT3),
//		D3DVSD_REG(1,D3DVSDT_FLOAT3),
//		D3DVSD_REG(2,D3DVSDT_D3DCOLOR),
	//	D3DVSD_REG(3,D3DVSDT_FLOAT2),
   // 	D3DVSD_END()
 //   };
	
	// Assemble the vertex shader from the file
  //  if( FAILED( hr = D3DXAssembleShaderFromFile("..//4DyuchiGXShaders//test.vsh",0, NULL, &pCode, NULL ) ) )
//		__asm int 3

	
    // Create the vertex shader
//	hr = m_pD3DDevice->CreateVertexShader( dwDecl, (DWORD*)pCode->GetBufferPointer(),&m_dwShader[VL_TEST], 0 );
//	pCode->Release();

	DWORD dwDecl[] =
    {
		D3DVSD_STREAM(0),
		D3DVSD_REG(0,D3DVSDT_FLOAT3),
		D3DVSD_REG(1,D3DVSDT_FLOAT2),
		D3DVSD_REG(2,D3DVSDT_FLOAT2),
    	D3DVSD_END()
    };
	
	// Assemble the vertex shader from the file
    if( FAILED( hr = D3DXAssembleShaderFromFile("..//4DyuchiGXShaders//dynamic_light_map.vsh",0, NULL, &pCode, NULL ) ) )
		__asm int 3
	
    // Create the vertex shader
	hr = m_pD3DDevice->CreateVertexShader( dwDecl, (DWORD*)pCode->GetBufferPointer(),&m_dwShader[LM_DYNAMIC_LIGHT], 0 );
	pCode->Release();
	
}
*/



DWORD __stdcall CoD3DDevice::ClearVBCacheWithIDIMeshObject(IDIMeshObject* pObject)
{
	return m_VBCache.ClearCacheWithIDIMeshObject(pObject);
}

DWORD __stdcall CoD3DDevice::ClearCacheWithMotionUID(void* pMotionUID)
{
	return m_VBCache.ClearCacheWithMotionUID(pMotionUID);
}


BOOL __stdcall CoD3DDevice::RenderMeshObject(
											 IDIMeshObject* pMeshObj,
											 DWORD dwRefIndex,
											 float fDistance,
											 DWORD dwAlpha,
											 LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
											 LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
											 DWORD dwMtlSetIndex,
											 DWORD dwEffectIndex,
											 DWORD dwFlag)
{
	RENDER_MESHOBJECT_DESC itemDesc;
	itemDesc.pMeshObject = pMeshObj;
	itemDesc.dwRefIndex = dwRefIndex;
	itemDesc.fDistance = fDistance;
	itemDesc.dwAlpha = dwAlpha;
	itemDesc.pDynamicLightIndexList = pDynamicLightIndexList;
	itemDesc.dwLightNum = dwLightNum;
	itemDesc.pSpotLightIndexList = pSpotLightIndexList;
	itemDesc.dwSpotLightNum = dwSpotLightNum;
	itemDesc.dwMtlSetIndex = dwMtlSetIndex;
	itemDesc.dwEffectIndex = dwEffectIndex;
	itemDesc.dwFlag = dwFlag;

//	RENDER_MESHOBJECT_DESC* pItem = (RENDER_MESHOBJECT_DESC*)&pMeshObj;
	/*float	factor;

	__asm
	{
		mov			esi,dword ptr[pMeshObj]
		mov			eax,dword ptr[esi+4]
		mov			dword ptr[factor],eax
	}*/


	// 모든 IDIMeshObject는 첫 4바이트가 렌더링 우선순위(z값)이다.
	fDistance += ((CBaseMeshObject*)pMeshObj)->GetRenderPriorityFactor();
	if (dwFlag & RENDER_TYPE_DISABLE_ZCLIP)
		fDistance += RENDER_PRIORITY_FACTOR_BILLBOARDOBJECT;


	m_dwRenderedMeshObjectNum++;
	
	m_dwRenderedFacesNum += ((CBaseMeshObject*)pMeshObj)->GetFacesNum();
	m_dwRenderedVertexNum += ((CBaseMeshObject*)pMeshObj)->GetVertexNum();

	return m_RenderQ.InsertItem((char*)&itemDesc,fDistance);
}
BOOL __stdcall CoD3DDevice::RenderSprite(IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag)
{
#ifdef _DEBUG
	if (!pSprite)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail to RenderSprite(), Sprite is NULL!!\n");
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
#endif

	m_dwRenderedSpriteObjectNum++;
	return m_SpriteBuffer.PushSpriteItem(m_pDPCQFor2DItem,pSprite,pv2Scaling,fRot,pv2Trans,pRect,dwColor,iZOrder,dwFlag);
}
BOOL __stdcall CoD3DDevice::RenderFont(IDIFontObject* pFont,TCHAR* str,DWORD dwLen,RECT* pRect,DWORD dwColor,CHAR_CODE_TYPE type,int iZOrder,DWORD dwFlag)
{

	BOOL		bHasCRLF = RemoveCRLF(str,dwLen);

#ifdef	_DEBUG
	char	txt[2048];
	DWORD	dwAddr;

	if (bHasCRLF)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), string has CR or LF or HT!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,1,(void*)dwAddr,txt);
	}
	if (0 == pRect->right - pRect->left)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's width zero!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (0 == pRect->bottom - pRect->top)
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's height zero!! %s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (pRect->right - pRect->left > m_pFontCache->GetMaxWidth())
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's width too large!!%s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
	if (pRect->bottom - pRect->top > m_pFontCache->GetMaxWidth())
	{
		memset(txt,0,sizeof(txt));
		wsprintf(txt,"Fail to RenderFont(), rect's height too large!!%s\n",str);
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
	}
#endif

	m_dwRenderedFontObjectNum++;
	return m_TextBuffer.PushTextItem(m_pDPCQFor2DItem,pFont,str,dwLen,pRect,dwColor,type,iZOrder,dwFlag);
}
/*
BOOL __stdcall CoD3DDevice::CreateShadowMap(char* szFileName,DWORD dwMaxShadowNum,DWORD dwShadowTexSize,DWORD dwFlag)
{
	// 쉐도우맵 생성

	BOOL		bResult = FALSE;

	m_dwShadowTexSize = dwShadowTexSize;
	m_dwShadowTextureNum = dwMaxShadowNum;
	
	for (DWORD i=0; i<m_dwShadowTextureNum; i++)
	{
		m_pRenderTexture[i] = CreateRenderSurface(m_dwShadowTexSize);

	}
	
	bResult = TRUE;
lb_return:
	return bResult;

}

*/


DWORD __stdcall CoD3DDevice::CreateMaterialSet(MATERIAL_TABLE* pMtlEntry,DWORD dwNum)
{
	return m_pMaterialManager->CreateMaterialSet(pMtlEntry,dwNum);
}
void __stdcall CoD3DDevice::DeleteMaterialSet(DWORD dwMtlSetIndex)
{
	m_pMaterialManager->DeleteMaterialSet(dwMtlSetIndex);
}


void __stdcall CoD3DDevice::SetRenderMode(DWORD dwFlag)
{
	DWORD	dwFillMode;

	switch ( dwFlag )
	{
	case RENDER_MODE_SOLID:
		dwFillMode = D3DFILL_SOLID;
		break;
	case RENDER_MODE_POINT:
		dwFillMode = D3DFILL_POINT;
		break;
	case RENDER_MODE_WIREFRAME:
		dwFillMode = D3DFILL_WIREFRAME;
		break;

	default:
		return;
	}
//	dwFillMode = D3DFILL_WIREFRAME;

	m_dwRenderMode = dwFlag;
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,dwFillMode);
	
}
DWORD __stdcall CoD3DDevice::GetRenderMode()
{
	return m_dwRenderMode;
}




void __stdcall CoD3DDevice::SetViewFrusturm(VIEW_VOLUME* pViewVolume,CAMERA_DESC* camera,MATRIX4* pMatView,MATRIX4* pMatProj,MATRIX4* pMatForBilboard)
{
//	if (memcmp(pMatView,&m_matView,sizeof(MATRIX4)))
//	{
	m_ViewVolume = *pViewVolume;

	m_matView = *pMatView;
	m_pD3DDevice->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)pMatView );
	
	SetInverseMatrix(&m_matViewInverse,&m_matView);

	m_matProj = *pMatProj;
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)pMatProj);

	m_cameraDesc = *camera;
	m_matForBilBoard = *pMatForBilboard;
}
/*
BOOL __stdcall CoD3DDevice::SetShadowMatrixSet(MATRIX4* pMatView,MATRIX4* pMatTex,MATRIX4* pMatProj,VECTOR3* pv3From,DWORD dwIndex)
{
	BOOL		bResult = FALSE;

	if (dwIndex >= MAX_SPOT_LIGHT_NUM_PER_SCENE)
		goto lb_return;


	m_matSetShadow[dwIndex].matView = *pMatView;
	m_matSetShadow[dwIndex].matTex = *pMatTex;
	m_matSetShadow[dwIndex].matProj = *pMatProj;
	m_matSetShadow[dwIndex].v3From = *pv3From;

	bResult = TRUE;
lb_return:
	return bResult;
}
*/



BOOL __stdcall CoD3DDevice::SetRTLight(LIGHT_DESC* pLightDesc,DWORD dwLightIndex,DWORD dwFlag)
{
	BOOL		bResult = FALSE;
	if (dwLightIndex >= MAX_REALTIME_LIGHT_NUM)
		goto lb_return;

	m_RTLightList[dwLightIndex] = *pLightDesc;
/*
	D3DLIGHT8	d3dLight;

	memset(&d3dLight,0,sizeof(d3dLight));
	d3dLight.Type = D3DLIGHT_POINT;
	d3dLight.Attenuation0 = 1.0f;
	d3dLight.Attenuation1 = 0.0f;
	d3dLight.Attenuation2 = 0.0f;
	d3dLight.Falloff = 1.0f;
		

	DWORDtoCOLOR(m_RTLightList[dwLightIndex].dwColor,
		&d3dLight.Diffuse.r,
		&d3dLight.Diffuse.g,
		&d3dLight.Diffuse.b,
		&d3dLight.Diffuse.a);
	d3dLight.Specular = d3dLight.Diffuse;
	d3dLight.Ambient = d3dLight.Diffuse;

	*(VECTOR3*)&d3dLight.Position.x = *(VECTOR3*)&m_RTLightList[dwLightIndex].v3Point.x;
	d3dLight.Range = m_RTLightList[dwLightIndex].fRs;
	m_pD3DDevice->SetLight(dwLightIndex,&d3dLight);
	m_pD3DDevice->LightEnable(dwLightIndex,TRUE);
*/
	bResult = TRUE;
lb_return:
	return bResult;
	
}

void __stdcall CoD3DDevice::EnableDirectionalLight(DIRECTIONAL_LIGHT_DESC* pLightDesc,DWORD dwFlag)
{
	
	D3DLIGHT8	d3dLight;
	memset(&d3dLight,0,sizeof(d3dLight));
	d3dLight.Type = D3DLIGHT_DIRECTIONAL;
	
	DWORDtoCOLOR(pLightDesc->dwDiffuse,
		&d3dLight.Diffuse.r,
		&d3dLight.Diffuse.g,
		&d3dLight.Diffuse.b,
		&d3dLight.Diffuse.a);

	DWORDtoCOLOR(pLightDesc->dwAmbient,
		&d3dLight.Ambient.r,
		&d3dLight.Ambient.g,
		&d3dLight.Ambient.b,
		&d3dLight.Ambient.a);

	DWORDtoCOLOR(pLightDesc->dwDiffuse,
		&d3dLight.Specular.r,
		&d3dLight.Specular.g,
		&d3dLight.Specular.b,
		&d3dLight.Specular.a);

	

	*(VECTOR3*)&d3dLight.Direction.x = *(VECTOR3*)&pLightDesc->v3Dir.x;
	
	
	m_pD3DDevice->SetLight(7,&d3dLight);
	m_pD3DDevice->LightEnable(7,TRUE);
	m_bDirectionalLight = TRUE;
}

void __stdcall CoD3DDevice::DisableDirectionalLight()
{
	m_bDirectionalLight = FALSE;
	m_pD3DDevice->LightEnable(7,FALSE);

}

void __stdcall CoD3DDevice::SetSpotLightDesc(VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up,float fFov,float fNear,float fFar,float fWidth,BOOL bOrtho,void* pMtlHandle,DWORD dwColorOP,DWORD dwLightIndex,SPOT_LIGHT_TYPE type)
{
	SPOT_LIGHT_DESC*	pSpotDesc = INL_GetSpotDesc(dwLightIndex);

	pSpotDesc->prjDesc.v3From = *pv3From;
	pSpotDesc->prjDesc.v3To = *pv3To;
	pSpotDesc->prjDesc.v3Up = *pv3Up;
	pSpotDesc->prjDesc.fFov = fFov;
	pSpotDesc->prjDesc.fNear = fNear;
	pSpotDesc->prjDesc.fFar = fFar;
	pSpotDesc->prjDesc.fWidth = fWidth;
	pSpotDesc->prjDesc.bOrtho = bOrtho;
	pSpotDesc->type = type;
	pSpotDesc->pMtl = (DIMATERIAL*)pMtlHandle;

	pSpotDesc->dwColorOP = dwColorOP;
	
	

//					DIMATERIAL*	pMtl = (DIMATERIAL*)pSpotLightIndexList[m].pMtlHandle;
//				pMtl->TextureDiffuse.SetTexture(pDevice,0);

	SetPrjTexMatrix(pSpotDesc);
	

}


void __stdcall CoD3DDevice::SetShadowLightSenderPosition(BOUNDING_SPHERE* pSphere,DWORD dwLightIndex)
{
	SPOT_LIGHT_DESC*	pSpotDesc = INL_GetSpotDesc(dwLightIndex);
	pSpotDesc->prjDesc.sphereSenderShadow = *pSphere;

	

}

void CoD3DDevice::SetPrjTexMatrix(SPOT_LIGHT_DESC* pSpotDesc)
{
	MATRIX4	matLightProj;

	float	fvpWidth,fvpHeight;
	fvpWidth = 0.0f;
	fvpHeight = 0.0f;
	
	if (pSpotDesc->type == SPOT_LIGHT_TYPE_SHADOW)
	{
		if (!pSpotDesc->pRenderTexture)
		{
			pSpotDesc->pRenderTexture = m_pRenderTexturePool->AllocRenderTexture(
											&pSpotDesc->pRenderTexture,
											&pSpotDesc->bUseRenderTexture
											);

			if (!pSpotDesc->pRenderTexture)
				goto lb_return;

		}

		fvpWidth = (float)pSpotDesc->pRenderTexture->GetWidth();
		fvpHeight = (float)pSpotDesc->pRenderTexture->GetHeight();
	}
	else if (pSpotDesc->type == SPOT_LIGHT_TYPE_PRJIMAGE)
	{

		IMAGE_HEADER*	pImgHeader = pSpotDesc->pMtl->TextureDiffuse.GetImageHeader();
		if (pImgHeader)
		{
			fvpWidth = (float)pImgHeader->dwWidth;
			fvpHeight = (float)pImgHeader->dwHeight;
		}
	}
	
	PRJLIGHT_DESC*		prjDesc;
	prjDesc = &pSpotDesc->prjDesc;

	if (prjDesc->bOrtho)
	{
		D3DXMatrixOrthoLH((D3DXMATRIX*)&matLightProj,prjDesc->fWidth,prjDesc->fWidth,prjDesc->fNear,prjDesc->fFar);
		prjDesc->matPrjProj = matLightProj;
	}
	else
	{	
		SetAdjustedProjectionMatrix(
			&matLightProj,
			prjDesc->fFov, 1.0f,
			prjDesc->fNear,prjDesc->fFar,
			0.0f, 0.0f, fvpWidth, fvpHeight );


		::SetProjectionMatrix(&prjDesc->matPrjProj,prjDesc->fFov,1.0f,prjDesc->fNear,prjDesc->fFar);
	//	prjDesc->matPrjProj = matLightProj;


	}

	
	MATRIX4		matTexScale;
	MATRIX4*	pMatTex;
	MATRIX4*	pMatView;
	
	pMatTex = &prjDesc->matPrjTex;
	pMatView = &prjDesc->matPrjView;
	
	

	::SetViewMatrix(pMatView,&prjDesc->v3From,&prjDesc->v3To,&prjDesc->v3Up);
	


    // Create texture scaling matrix:
    // | 0.5      0        0        0 |
    // | 0        -0.5     0        0 |
    // | 0        0        0        0 |
    // | 0.5      0.5      1        1 |

    // This will scale and offset -1 to 1 range of x, y
    // coords output by projection matrix to 0-1 texture
    // coord range.
	memset(&matTexScale,0,sizeof(matTexScale));

	matTexScale._11 = 0.5f;
    matTexScale._22 = -0.5f;
    matTexScale._33 = 0.0f; 
    matTexScale._41 = 0.5f; 
    matTexScale._42 = 0.5f;
    matTexScale._43 = 1.0f; 
    matTexScale._44 = 1.0f;

//	MatrixMultiply3(pMatTex,pMatView,&matLightProj,&matTexScale);
	MatrixMultiply2(pMatTex,pMatView,&matLightProj);
	MatrixMultiply2(pMatTex,pMatTex,&matTexScale);

lb_return:
	return;
}

IDISpriteObject* __stdcall CoD3DDevice::CreateSpriteObject(char* szFileName,DWORD dwFlag)
{
	C3DSprite*		pDevObj;
	pDevObj = new C3DSprite;
	if (pDevObj->Initialize(this,szFileName,0,0,0,0,0xffffffff,dwFlag))
		pDevObj->AddRef();
	else
	{
		delete pDevObj;
		pDevObj = NULL;
	}
	
	return pDevObj;
}
IDISpriteObject* __stdcall CoD3DDevice::CreateSpriteObject(char* szFileName,DWORD dwXPos,DWORD dwYPos,DWORD dwWidth,DWORD dwHeight,DWORD dwFlag)
{
	C3DSprite*		pDevObj;
	pDevObj = new C3DSprite;
	if (pDevObj->Initialize(this,szFileName,dwXPos,dwYPos,dwWidth,dwHeight,0xffffffff,dwFlag))
		pDevObj->AddRef();
	else
	{
		delete pDevObj;
		pDevObj = NULL;
	}
	
	return pDevObj;
}

IDISpriteObject*	__stdcall	CoD3DDevice::CreateEmptySpriteObject( DWORD dwWidth, DWORD dwHeight, TEXTURE_FORMAT TexFormat, DWORD dwFlag)
{
	C3DSprite*		pDevObj;
	pDevObj	=	new	C3DSprite;
	if( pDevObj->InitializeNameless( this, dwWidth, dwHeight, TexFormat, dwFlag))
		pDevObj->AddRef();
	else
	{
		delete pDevObj;
		pDevObj = NULL;
	}

	return	pDevObj;
}

void __stdcall CoD3DDevice::GetClientRect(SHORT_RECT* pRect,WORD* pwWidth,WORD* pwHeight)

{
	*pwWidth = m_wWidth;
	*pwHeight = m_wHeight;
	*pRect = m_rcWindow;
}

IDIMeshObject* __stdcall CoD3DDevice::CreateMeshObject(CMeshFlag flag)
{
	static	DWORD dwObjCount = 0;
	IDIMeshObject*		pDevObj = NULL;
	
	if (flag.GetShadeType() == SHADE_TYPE_LIGHT_MAP)
	{
		pDevObj = new CLMMeshObject;

		((CLMMeshObject*)pDevObj)->SetRenderer(this);

	}
	else if (flag.GetTransformType() == TRANSFORM_TYPE_ALIGN_VIEW)
	{
		pDevObj = new CBillBoardObject;
		((CBillBoardObject*)pDevObj)->SetRenderer(this);

	}
	else if (flag.GetRigidType() == RIGID_TYPE_RIGID)
	{
		pDevObj = new CVLSMeshObject;
		((CVLSMeshObject*)pDevObj)->SetRenderer(this);
	}
	else 
	{
		pDevObj = new CVLMeshObject;
		((CVLMeshObject*)pDevObj)->SetRenderer(this);
//		pDevObj = new CPLMeshObject;
//		((CPLMeshObject*)pDevObj)->SetRenderer(this);
	}
		

	((CBaseMeshObject*)pDevObj)->SetObjCount(dwObjCount);
	dwObjCount++;

	pDevObj->AddRef();

	return pDevObj;
}


IDIMeshObject*	__stdcall CoD3DDevice::CreateImmMeshObject(IVERTEX* piv3Tri,DWORD dwTriCount,void* pMtlHandle,DWORD dwFlag)
{
	CImmMeshObject*	pMeshObj = (CImmMeshObject*)LALAlloc(m_pImmMeshObjectPool);

	if (!pMeshObj)
		goto lb_return;
	

	pMeshObj->Initialize(this);

	if (piv3Tri)
	{

		if (!pMeshObj->Build(piv3Tri,dwTriCount,pMtlHandle,dwFlag))
		{
			LALFree(m_pImmMeshObjectPool,pMeshObj);
			pMeshObj = NULL;
			goto lb_return;
		}
	}
	pMeshObj->AddRef();
lb_return:
	return (IDIMeshObject*)pMeshObj;
}
void CoD3DDevice::DeleteImmMeshObject(CImmMeshObject* pMeshObj)
{
	LALFree(m_pImmMeshObjectPool,pMeshObj);
}
IDIFontObject* __stdcall CoD3DDevice::CreateFontObject(LOGFONT* pLogFont,DWORD dwFlag)
{
	CBaseFontObject*	pFontObject;

//	dwFlag = 0;
	if (D3DX_FONT == dwFlag)
	{
		pFontObject = new CFontBuffer;
	}
	else
	{
		pFontObject = new CFontObject;
	}
	if (!pFontObject->CreateFont(this,pLogFont))
	{
		delete pFontObject;
		pFontObject = NULL;
		goto lb_return;
	}
	

		
	pFontObject->AddRef();
	

lb_return:
	return pFontObject;

}

void CoD3DDevice::LinkHFieldManager(CHFieldManager* pHFieldManager)
{
	LinkToSortLink(&m_pHFieldHead,&m_pHFieldTail,&pHFieldManager->m_linkInRenderer);
	m_dwHFieldNum++;
}
void CoD3DDevice::UnLinkHFieldManager(CHFieldManager* pHFieldManager)
{
	UnLinkFromSortLink(&m_pHFieldHead,&m_pHFieldTail,&pHFieldManager->m_linkInRenderer);
	m_dwHFieldNum--;
}

IDIHeightField* __stdcall CoD3DDevice::CreateHeightField(DWORD dwFlag)
{
	CHFieldManager* pHFieldManager = new CHFieldManager(this);
	pHFieldManager->AddRef();
	m_dwHFieldNum++;
	return pHFieldManager;
}



void __stdcall CoD3DDevice::BeginRender(SHORT_RECT* pRect,DWORD dwColor,DWORD dwFlag)
{
	HRESULT		hr;

	
	DPCQClear(m_pDPCQFor2DItem);
	m_RenderQ.Clear();
	m_TextBuffer.Clear();
	m_SpriteBuffer.Clear();
	
	
	DWORD		dwClearFlag = 0;
	if (!(dwFlag & BEGIN_RENDER_FLAG_DONOT_CLEAR_ZBUFFER))
		dwClearFlag |= D3DCLEAR_ZBUFFER;

	if (!(dwFlag & BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER))
		dwClearFlag |= D3DCLEAR_TARGET;

	BOOL	bUseRenderTexture;
	if (dwFlag & BEGIN_RENDER_FLAG_USE_RENDER_TEXTURE)
		bUseRenderTexture = TRUE;
	else 
		bUseRenderTexture = FALSE;

	m_pRenderTexturePool->Enable(bUseRenderTexture);

	if (!pRect)
	{
		m_rcClip = m_rcWindow;
		m_rcDirty.left = m_rcWindow.left; 
		m_rcDirty.top = m_rcWindow.top;
		m_rcDirty.right = m_rcWindow.right;
		m_rcDirty.bottom = m_rcWindow.bottom;
	}
	else
	{
		if (m_rcDirty.left > pRect->left)
			m_rcDirty.left = pRect->left;
		
		if (m_rcDirty.top > pRect->top)
			m_rcDirty.top = pRect->top;

		if (m_rcDirty.right < pRect->right)
			m_rcDirty.right = pRect->right;
		
		if (m_rcDirty.bottom < pRect->bottom)
			m_rcDirty.bottom = pRect->bottom;

		m_rcClip = *pRect;
	}

	
	
	m_vpCurrent.X      = m_rcClip.left;
	m_vpCurrent.Y      = m_rcClip.top;
	m_vpCurrent.Width  = m_rcClip.right - m_rcClip.left;
	m_vpCurrent.Height = m_rcClip.bottom - m_rcClip.top;
	m_vpCurrent.MinZ   = 0.0f;
	m_vpCurrent.MaxZ   = 1.0f;
	
	
//	CLEAR_ZBUFFER = 0,
//	CLEAR_FRAMEBUFFER = 0,
//		= 0x00000001,
//	DONOT_CLEAR_FRAMEBUFFER	= 0x00000002
	

	m_pD3DDevice->SetViewport(&m_vpCurrent);	
	if (dwClearFlag)
		hr = m_pD3DDevice->Clear(0,NULL, dwClearFlag ,dwColor, 1.0f, 0 );

	m_pD3DDevice->BeginScene();
}
/*
void __stdcall CoD3DDevice::EnableZBuffer()
{
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

}
void __stdcall CoD3DDevice::DisableZBuffer()
{
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
}
*/

void __stdcall CoD3DDevice::SetLightMapFlag(DWORD dwFlag)
{
	m_dwLightMapFlag = dwFlag;

	if (dwFlag & LIGHTMAP_FLAG_DISABLE_MAGFILTER)
	{
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_POINT );
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_POINT );
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_POINT );
	}
	else
	{
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );


	}	
}
DWORD __stdcall	CoD3DDevice::GetLightMapFlag()
{
	return m_dwLightMapFlag;
}
void __stdcall CoD3DDevice::EnableFog(float fStart,float fEnd,float fDensity,DWORD dwColor,DWORD dwFlag)
{
	m_bFog = TRUE;
	m_fFogDensity = fDensity;
	m_dwFogColor = dwColor;
	m_fStart = fStart;
	m_fEnd = fEnd;

	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR,m_dwFogColor);
//	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_EXP);
//	m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD*)&m_fFogDensity);
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_LINEAR);

	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD*)&m_fStart);
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND,*(DWORD*)&m_fEnd);
}
void __stdcall CoD3DDevice::DisableFog()
{
	m_bFog = FALSE;
}

void __stdcall CoD3DDevice::EndRender()
{
	// rendering
	RENDER_MESHOBJECT_DESC*	pDesc;

	m_RenderQ.Sort();
	for (DWORD i=0; i<m_RenderQ.GetItemNum(); i++)
	{
		pDesc = (RENDER_MESHOBJECT_DESC*)m_RenderQ.GetItemSeq(i);
		pDesc->pMeshObject->Render(
			pDesc->dwRefIndex,
			pDesc->dwAlpha,
			pDesc->pDynamicLightIndexList,pDesc->dwLightNum,
			pDesc->pSpotLightIndexList,pDesc->dwSpotLightNum,
			pDesc->dwMtlSetIndex,
			pDesc->dwEffectIndex,
			pDesc->dwFlag);
	}

/*	// 스프라이트 출력 
	m_SpriteBuffer.Process();

	// 폰트 출력 
	m_TextBuffer.Process();
*/
	m_RenderTriBuffer.Process();
	DPCQProcessWithAscSort(m_pDPCQFor2DItem);


	// End the scene
    m_pD3DDevice->EndScene();
}
void __stdcall CoD3DDevice::Present(HWND hWnd)
{
	RECT*	pRect = NULL;
	if (m_dispType == WINDOW_WITH_BLT)
		pRect = &m_rcDirty;		

//	if (m_bWindowed)
//		pRect = &m_rcDirty;

	/*
	RECT	crect;
	crect.bottom = 256;
	crect.top = 0;
	crect.left = 0;
	crect.right = 256;
	POINT	point;
	point.x = 0;
	point.y = 0;

	LPDIRECT3DSURFACE8	pDest;
	
	m_pD3DDevice->GetRenderTarget(&pDest);
	
	
	if (m_pShadowTexture[0])
	{


		m_pD3DDevice->CopyRects(m_pShadowTexture[0]->GetSurface(),
						&crect,
						1,
						pDest,
						&point);
	}
	pDest->Release();
*/

	HRESULT hr = m_pD3DDevice->Present(pRect,pRect,hWnd, NULL );
	if (D3DERR_DEVICELOST == hr)
		ResetDevice(TRUE);
}
void CoD3DDevice::ResetDevice(BOOL bTest)
{
//	LPDIRECT3DSURFACE8			pDefaultRenderSurface;		
//	LPDIRECT3DSURFACE8			pDefaultZBuffer;
	

	SORT_LINK*				pCur = m_pHFieldHead;
	while (pCur)
	{
		CHFieldManager*		pHFieldManager;
		pHFieldManager = (CHFieldManager*)pCur->pItem;
		pHFieldManager->OnReset();
		
		

		pCur = pCur->pNext;
	}

	m_pRenderTexturePool->Cleanup();
	
	// D3DPOOL_DEFAULT를 사용하는 버텍스버퍼,인덱스버퍼 캐쉬이다.
	m_VBCache.Cleanup();
	m_IBCache.Cleanup();

	DWORD	i;
	for (i=0; i<m_dwResourcePoolNum; i++)
	{
		m_pResourcePoolList[i]->OnLostDevice();
	}


	m_rcManager.OnLostFont();

#ifdef _DEBUG
	m_rcManager.Check();
#endif
	

	if (bTest)
	{

		while (D3DERR_DEVICENOTRESET != m_pD3DDevice->TestCooperativeLevel())
		{
			Sleep(1);
		}
	}



	m_pD3DDevice->Reset(&m_d3dpp);

	m_rcManager.OnResetFont();


	m_pRenderTexturePool->Initialize(this,m_dwRenderTargetTexelSize,m_dwRenderTargetNum,0);

	m_VBCache.Initialize(this,4096,65536,2);
	m_IBCache.Initialize(this,2048,32768,1);

	for (i=0; i<m_dwResourcePoolNum; i++)
	{
		m_pResourcePoolList[i]->Reset();
	}
	DefaultEnvSet();
}
BOOL CoD3DDevice::CreateResourcePool(IResourcePool** ppResourcePool,RESORUCE_POOL_TYPE type)
{
	BOOL	bResult = FALSE;
	if (m_dwResourcePoolNum >= MAX_RESOURCE_POOL_NUM)
		goto lb_return;

	IResourcePool*	pResourcePool;

	switch(type)
	{
	case RESORUCE_POOL_TYPE_VBSTATICPOOL:
		{
			CStaticVBPool*	pVBStaticPool;
			pVBStaticPool = new CStaticVBPool;
			pResourcePool = (IResourcePool*)pVBStaticPool;
		}
		break;
	case RESORUCE_POOL_TYPE_IBSTATICPOOL:
		{
			CStaticIBPool*	pIBStaticPool;
			pIBStaticPool = new CStaticIBPool;
			pResourcePool = (IResourcePool*)pIBStaticPool;
		}
		break;
	case RESORUCE_POOL_TYPE_VBHEAP:
		{
			CVBHeap*	pVBHeap;
			pVBHeap = new CVBHeap;
			pResourcePool = (IResourcePool*)pVBHeap;
		}
		break;

	case RESORUCE_POOL_TYPE_IBHEAP:
		{
			CIBHeap*	pIBHeap;
			pIBHeap = new CIBHeap;
			pResourcePool = (IResourcePool*)pIBHeap;

		}
		break;
	}
	*ppResourcePool = pResourcePool;
	m_pResourcePoolList[m_dwResourcePoolNum] = pResourcePool;
	m_dwResourcePoolNum++;
	bResult = TRUE;
lb_return:
	return bResult;
	
}
BOOL CoD3DDevice::ReleaseResourcePool(IResourcePool* pResourcePool)
{
	BOOL	bResult = TRUE;
	for (DWORD i=0; i<m_dwResourcePoolNum; i++)
	{
		if (pResourcePool == m_pResourcePoolList[i])
		{
			m_pResourcePoolList[i]->Release();
			m_pResourcePoolList[i] = NULL;
			m_pResourcePoolList[i] = m_pResourcePoolList[m_dwResourcePoolNum-1];
			m_pResourcePoolList[m_dwResourcePoolNum-1] = NULL;
			m_dwResourcePoolNum--;
			goto lb_exit;
		}
	}
	bResult = FALSE;
#ifdef _DEBUG
	if (!bResult)
		__asm int 3

#endif
lb_exit:
	return bResult;
}

void CoD3DDevice::DefaultEnvSet()
{
    m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	

	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );

	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );


	SetLightMapFlag(m_dwLightMapFlag);


	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );
	
    m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
    m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE2X );
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );    
	SetAmbientColor(DEFAULT_AMBIENT_COLOR);
	SetRenderMode(0);
	m_Status.dwTotalTexMem = m_pD3DDevice->GetAvailableTextureMem();

	 


	m_pD3DDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);

	SetAlphaRefValue(m_dwAlphaRefValue);
	

	
}
void __stdcall CoD3DDevice::EnableSpecular(float fVal)
{
	
	m_fDefaultSpecularPower = fVal;
	m_dwSpecularMask = 0xffffffff;

	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
}
void __stdcall CoD3DDevice::DisableSpecular()
{
	
	m_dwSpecularMask = 0x00000000;
	
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,FALSE);
}
BOOL __stdcall CoD3DDevice::BeginShadowMap()
{
	return m_pRenderTexturePool->BeginShadowMap();

}
void __stdcall CoD3DDevice::EndShadowMap()
{

	m_pRenderTexturePool->EndShadowMap();
	m_pD3DDevice->SetViewport(&m_vpCurrent);
}
/*
BOOL CoD3DDevice::BeginCreateProjectionMap(DWORD dwShadowTextureIndex,DWORD dwFlag)
{
	return m_pRenderTexturePool->BeginCreateProjectionMap(dwShadowTextureIndex,dwFlag);
}


BOOL CoD3DDevice::EndCreateProjectionMap(DWORD dwShadowTextureIndex)
{
	return m_pRenderTexturePool->EndCreateProjectionMap(dwShadowTextureIndex);
}
*/
void __stdcall CoD3DDevice::SetRenderTextureMustUpdate(BOOL bMustUpdate)
{
	m_pRenderTexturePool->SetMustUpdate(bMustUpdate);
}


void __stdcall CoD3DDevice::SetShadowFlag(DWORD dwFlag)
{
	m_dwShadowFlag = dwFlag;
}
DWORD __stdcall CoD3DDevice::GetShadowFlag()
{
	return m_dwShadowFlag;
}

void __stdcall CoD3DDevice::BeginPerformanceAnalyze()
{
	BeginAnalyze(&m_cxtPerformance);
}
void __stdcall CoD3DDevice::EndPerformanceAnalyze()
{
	if (!EndAnalyze(&m_cxtPerformance))
		goto lb_render;


	
	


	int	iMissPercentage;
	iMissPercentage = 0;

	if (m_dwRenderHFObjectNum)
	{
		iMissPercentage = (int)( ((float)m_dwCacheMissHFObjectNum  / (float)m_dwRenderHFObjectNum)*100.0f );
	}


	DWORD	dwAvaliableTexMemSize;
	dwAvaliableTexMemSize = m_pD3DDevice->GetAvailableTextureMem();
	int	iFreeVBCacheRate;
	iFreeVBCacheRate = (int)(m_VBCache.GetFreeVBCacheRate()* 100.0f);
	
	wsprintf(m_szSysInfo1,"f:%u obj:%u spr:%u font:%u P:%u V:%u LF:%u SF:%u %s %d %d",
		m_cxtPerformance.dwAvgFrame,
		m_dwRenderedMeshObjectNum,
		m_dwRenderedSpriteObjectNum,
		m_dwRenderedFontObjectNum,
		m_dwRenderedFacesNum,
		m_dwRenderedVertexNum,
		m_dwRedneredLitFacesNum,
		m_dwRedneredShadowedFacesNum,
		m_Status.szDeviceType,
		m_VBCache.GetCacheHitRate(),
		iFreeVBCacheRate);

	m_rcSysInfo1.left = 10;
	m_rcSysInfo1.top = 10;
	m_rcSysInfo1.bottom = 22;

	int iWidth;
	iWidth = (lstrlen(m_szSysInfo1)+1)*7;
	m_rcSysInfo1.right = m_rcSysInfo1.left + iWidth;

	

	wsprintf(m_szSysInfo2,"Tex:%d VLS:%d VL:%d LM:%d V3:%d HF:%d, IBMan:%d IBHF:%d mem:%d",
		m_pTextureManager->GetTextureNum(),
		m_pVBVLSMeshHeap->GetVertexBufferNum(),
		m_pVBVLMeshHeap->GetVertexBufferNum(),
		m_pVBLMMeshHeap->GetVertexBufferNum(),
		m_pVBVector3MeshHeap->GetVertexBufferNum(),
		m_dwHFVBNum,
		m_pIBHeapManaged->GetIndexBufferNum(),
		m_dwHFIBNum,
		dwAvaliableTexMemSize);

	m_rcSysInfo2.left = 10;
	m_rcSysInfo2.top = 30;
	m_rcSysInfo2.bottom = 42;
	m_rcSysInfo2.right = 441 ;

	iWidth = (lstrlen(m_szSysInfo2)+1)*8;
	m_rcSysInfo2.right = m_rcSysInfo2.left + iWidth;

lb_render:
		
	D3DVIEWPORT8	vp;
	
	vp.X      = m_rcSysInfo1.left;
	vp.Y      = m_rcSysInfo1.top;
	vp.Width  = m_rcSysInfo2.right - m_rcSysInfo1.left;
	vp.Height = m_rcSysInfo2.bottom - m_rcSysInfo1.top;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;

	m_pD3DDevice->SetViewport(&vp);	
	m_pD3DDevice->BeginScene();

		

//	m_pFont->BeginRender();
	if (m_rcSysInfo1.right - m_rcSysInfo1.left > 0)
		m_pFont->DrawText(m_szSysInfo1,lstrlen(m_szSysInfo1),&m_rcSysInfo1,0xff00ff00,CHAR_CODE_TYPE_ASCII,0);

	if (m_rcSysInfo2.right - m_rcSysInfo2.left > 0)
		m_pFont->DrawText(m_szSysInfo2,lstrlen(m_szSysInfo2),&m_rcSysInfo2,0xff00ff00,CHAR_CODE_TYPE_ASCII,0);

//	m_pFont->EndRender();


	m_pD3DDevice->EndScene();

	
	m_dwRenderedMeshObjectNum = 0;
	m_dwRenderedSpriteObjectNum = 0;
	m_dwRenderedFontObjectNum = 0;
	m_dwRenderedFacesNum = 0;
	m_dwRenderedVertexNum = 0;
	m_dwRedneredLitFacesNum = 0;
	m_dwRedneredShadowedFacesNum = 0;

	m_dwRenderHFObjectNum = 0;
	m_dwCacheMissHFObjectNum = 0;

}

void CoD3DDevice::Cleanup()
{
	m_VBCache.Cleanup();
	m_IBCache.Cleanup();


	DWORD	i;
//	if (!m_bWindowed)
//		ChangeDisplaySettings(NULL,0);

	if (m_dispType == FULLSCREEN_WITH_BLT)
		ChangeDisplaySettings(NULL,0);


	if (m_pFontCache)
	{
		delete m_pFontCache;
		m_pFontCache = NULL;
	}


	DeleteEffectShaderPalette();
	DeleteDynamicLight(m_dwDefaultLightIndex);

	for (i=0; i<MAX_TEX_PROJECTION_DYNAMIC_LIGHT_NUM; i++)
	{
		if (m_pTexProjectionLightTexture[i])
			delete m_pTexProjectionLightTexture[i];
	}
	if (m_pRenderTexturePool)
	{
		delete m_pRenderTexturePool;
		m_pRenderTexturePool = NULL;
	}
	for (i=0; i<MAX_SHADER_NUM; i++)
	{
		if (m_dwShader[i])
			m_pD3DDevice->DeleteVertexShader(m_dwShader[i]);
	}
	if (m_pMaterialManager)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = NULL;
	}
	if (m_pTextureManager)
	{
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
	if (m_pImmMeshObjectPool)
	{
		ReleaseStaticMemoryPool(m_pImmMeshObjectPool);
		m_pImmMeshObjectPool = NULL;
	}

	DWORD	dwNum = m_dwResourcePoolNum;
	for (i=0; i<dwNum; i++)
	{
		ReleaseResourcePool(m_pResourcePoolList[0]);
	}



	DPCQRelease(m_pDPCQFor2DItem);

	if( m_pD3DDevice != NULL) 
        m_pD3DDevice->Release();

    if( m_pD3D != NULL)
        m_pD3D->Release();
	if (m_pFileStorage)
	{
		m_pFileStorage->Release();
		m_pFileStorage = NULL;
	}

	
	
//	FreeLibrary(m_hModGFunc);
}




CoD3DDevice::~CoD3DDevice()
{
	Cleanup();
#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
	InterlockedDecrement( &g_lComponent);

}


void __stdcall CoD3DDevice::RenderGrid(VECTOR3* pv3Quad,DWORD dwColor)
{
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	SetColorFactorShader(m_pD3DDevice,dwColor,0);

	RenderGridWithTexture(pv3Quad,D3DFILL_WIREFRAME);
	
	RestoreTextureShader(m_pD3DDevice,0);
}
void CoD3DDevice::RenderGridWithTexture(VECTOR3* pv3Quad,DWORD dwRenderMode)
{
	VECTOR3 v3Oct[6];

	v3Oct[0] = pv3Quad[0];
	v3Oct[1] = pv3Quad[2];
	v3Oct[2] = pv3Quad[3];

	v3Oct[3] = pv3Quad[0];
	v3Oct[4] = pv3Quad[1];
	v3Oct[5] = pv3Quad[2];

	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,dwRenderMode);
	m_pD3DDevice->SetVertexShader(D3DFVF_XYZ);
	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		2,v3Oct,sizeof(VECTOR3));

	SetRenderMode(m_dwRenderMode);

}
void __stdcall CoD3DDevice::RenderBox(VECTOR3* pv3Oct,DWORD dwColor)
{
	VECTOR3	v3Oct[36];
	CreateVertexListWithBox((char*)v3Oct,sizeof(VECTOR3),pv3Oct);
	RenderTriVector3(v3Oct,12,0);
	

/*
	IVERTEX	iv[36];
	for (DWORD i=0; i<36; i++)
	{
		iv[i].u1 = 0.5f;
		iv[i].v1 = 0.5f;
		*(VECTOR3*)&iv[i].x = *(VECTOR3*)&v3Oct[i].x;


	}
	for (i=0; i<12; i++)
	{
		RenderTriIvertex(iv+i*3,NULL,0);
	}
	return;

	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	m_pD3DDevice->SetVertexShader(D3DFVF_XYZ);

	
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	SetColorFactorShader(m_pD3DDevice,dwColor,0);

	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());

	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		12,v3Oct,sizeof(VECTOR3));

	RestoreTextureShader(m_pD3DDevice,0);

	SetRenderMode(m_dwRenderMode);*/
}

BOOL __stdcall CoD3DDevice::RenderTriIvertex(IVERTEX* piv3Tri,void* pMtlHandle,DWORD dwFacesNum,DWORD dwFlag)
{
	DWORD	dwRenderFlag = D3DFILL_WIREFRAME;

	if (dwFlag & 0x00000001)
		dwRenderFlag = D3DFILL_SOLID;

	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,dwRenderFlag);

	m_pD3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
	
	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

//	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	DIMATERIAL*	pMtl = (DIMATERIAL*)pMtlHandle;

	if (!pMtl)
		pMtl = m_pMaterialManager->GetDefaultMaterial();

	D3DMATERIAL8	d3dmtl;
	d3dmtl = pMtl->d3dMtl;

	if (!(pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_SELF_ILLUMIN))
	{

		if (dwFlag & RENDER_TYPE_SELF_ILLUMIN)
		{
			d3dmtl.Emissive = d3dmtl.Diffuse;
		}
		else
		{
			INL_GetEmissiveColor(&d3dmtl.Emissive);
		}
		
	}
	m_pD3DDevice->SetMaterial(&d3dmtl);

	// 2side 처리
//	if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		
	

	BeginAlphaMeshObject(m_pD3DDevice,1,pMtl->dwAlpha,255);
	pMtl->TextureDiffuse.SetTexture(m_pD3DDevice,0);

//	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

//	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DCMP_LESSEQUAL );
//	m_pD3DDevice->SetRenderState( D3DRS_ZBIAS, 10);

	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		dwFacesNum,piv3Tri,sizeof(IVERTEX));

	m_pD3DDevice->SetRenderState( D3DRS_ZBIAS, 0);

	EndAlphaMeshObject(m_pD3DDevice);
				
//	if (pMtl->dwProperty & DIMATERIAL_PROPERTY_TYPE_DUAL_SIDE)
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

//	SetColorFactorShader(m_pD3DDevice,0xffffffff,0);
//	RestoreTextureShader(m_pD3DDevice,0);

	SetRenderMode(m_dwRenderMode);

	

	return TRUE;
}
BOOL __stdcall CoD3DDevice::RenderTriVector3(VECTOR3* pv3Tri,DWORD dwFacesNum,DWORD dwFlag)
{
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	m_pD3DDevice->SetVertexShader(D3DFVF_XYZ);
	
	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	

//	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	DIMATERIAL*	pMtl;
	pMtl = m_pMaterialManager->GetDefaultMaterial();

	D3DMATERIAL8	d3dmtl;
	d3dmtl = pMtl->d3dMtl;

	m_pD3DDevice->SetMaterial(&d3dmtl);

	// 2side 처리

//	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		dwFacesNum,pv3Tri,sizeof(VECTOR3));

//	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

//	SetColorFactorShader(m_pD3DDevice,0xffffffff,0);
//	RestoreTextureShader(m_pD3DDevice,0);

	SetRenderMode(m_dwRenderMode);

	

	return TRUE;
}

void* __stdcall CoD3DDevice::AllocRenderTriBuffer(IVERTEX** ppIVList,DWORD dwFacesNum,DWORD dwRenderFlag)
{
	return (void*)m_RenderTriBuffer.AllocBuffer(ppIVList,dwFacesNum,dwRenderFlag);
}
void __stdcall CoD3DDevice::EnableRenderTriBuffer(void* pTriBufferHandle,void* pMtlHandle,DWORD dwRenderFacesNum)
{
	m_RenderTriBuffer.EnableRender((IVERTEX_TRI_ITEM*)pTriBufferHandle,pMtlHandle,dwRenderFacesNum);
}
void __stdcall CoD3DDevice::DisableRenderTriBuffer(void* pTriBufferHandle)
{
	m_RenderTriBuffer.DisableRender((IVERTEX_TRI_ITEM*)pTriBufferHandle);

}
void __stdcall CoD3DDevice::FreeRenderTriBuffer(void* pTriBufferHandle)
{
	m_RenderTriBuffer.FreeBuffer((IVERTEX_TRI_ITEM*)pTriBufferHandle);
}



void __stdcall CoD3DDevice::RenderPoint(VECTOR3* pv3Point,DWORD dwColor)
{
	D3DLVERTEX v3Point;
	
	v3Point.color = dwColor;

	*(VECTOR3*)&v3Point.x = *pv3Point;
		
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_POINT);

	m_pD3DDevice->SetVertexShader(D3DFVF_D3DLVERTEX);

	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	SetColorFactorShader(m_pD3DDevice,dwColor,0);

	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());

	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_POINTLIST,
		1,&v3Point,sizeof(D3DLVERTEX));

	RestoreTextureShader(m_pD3DDevice,0);

	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );



	SetRenderMode(m_dwRenderMode);
}
void __stdcall CoD3DDevice::RenderCircle(VECTOR2* pv2Point,float fRs,DWORD dwColor)
{
	m_Circle.BuildMesh(pv2Point,fRs);
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );

	m_pD3DDevice->SetVertexShader(D3DFVF_4DVERTEX);

	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	
//	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

	SetColorFactorShader(m_pD3DDevice,dwColor,0);
	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());

	m_pD3DDevice->DrawIndexedPrimitiveUP(
		D3DPT_LINELIST,
		0,
		m_Circle.GetVerticesNum(),
		m_Circle.GetLinesNum(),
		m_Circle.GetIndexList(),
		D3DFMT_INDEX16,
		m_Circle.GetPointList(),
		sizeof(VECTOR4));

	RestoreTextureShader(m_pD3DDevice,0);

	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );

	SetRenderMode(m_dwRenderMode);
}
void __stdcall CoD3DDevice::RenderLine(VECTOR2* pv2Point0,VECTOR2* pv2Point1,DWORD dwColor)
{
	VECTOR4		v4Point[2];
	
	DWORD	i;

	v4Point[0].x = pv2Point0->x;
	v4Point[0].y = pv2Point0->y;
	v4Point[1].x = pv2Point1->x;
	v4Point[1].y = pv2Point1->y;
	
	for (i=0; i<2; i++)
	{
		v4Point[i].z = 0.1f;
		v4Point[i].w = 1.0f / v4Point[i].z;
	}

	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	m_pD3DDevice->SetVertexShader(D3DFVF_4DVERTEX);
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	

	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

	SetColorFactorShader(m_pD3DDevice,dwColor,0);
	m_pD3DDevice->SetTransform( D3DTS_WORLD,(D3DMATRIX*)GetIdentityMatrix());

	
	m_pD3DDevice->DrawPrimitiveUP(
		D3DPT_LINELIST,
		1,
		v4Point,
		sizeof(VECTOR4));

	RestoreTextureShader(m_pD3DDevice,0);

	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );

	SetRenderMode(m_dwRenderMode);
}

void* __stdcall	CoD3DDevice::CreateMaterial(MATERIAL* pMaterial,DWORD* pdwWidth,DWORD* pdwHeight,DWORD dwFlag)
{
	return m_pMaterialManager->Alloc(pMaterial,pdwWidth,pdwHeight,dwFlag);
}
void __stdcall CoD3DDevice::SetMaterialTextureBorder(void* pMtlHandle,DWORD dwColor)
{
	m_pMaterialManager->SetMaterialTextureBorder((DIMATERIAL*)pMtlHandle,dwColor);
}

void __stdcall CoD3DDevice::DeleteMaterial(void* pMtlHandle)
{
	m_pMaterialManager->Free((DIMATERIAL*)pMtlHandle);
}

BOOL __stdcall CoD3DDevice::CaptureScreen(char* szFileName)
{

/*	RECT	crect;
	crect.bottom = 256;
	crect.top = 0;
	crect.left = 0;
	crect.right = 256;
	POINT	point;
	point.x = 0;
	point.y = 0;
*/
	BOOL	bResult = FALSE;

	LPDIRECT3DSURFACE8	pPrimarySurface;
	LPDIRECT3DSURFACE8	pTempSurface;

	if (D3D_OK != m_pD3DDevice->GetRenderTarget(&pPrimarySurface))
		goto lb_return;

	
//	char*	pBits;
//	DWORD	dwWidth,dwHeight,dwBPS;
//	CreateBitmapWithSurface(m_pD3DDevice,&pBits,&dwWidth,&dwHeight,&dwBPS,pPrimarySurface,D3DFMT_R5G6B5);


	D3DSURFACE_DESC	desc;
	if (D3D_OK != pPrimarySurface->GetDesc(&desc))
		goto lb_return;


	IDirect3DTexture8* pTempTex;
	if (D3D_OK != m_pD3DDevice->CreateTexture(desc.Width,desc.Height,1,0,desc.Format,D3DPOOL_SYSTEMMEM,&pTempTex))
		goto lb_return;

	pTempTex->GetSurfaceLevel(0,&pTempSurface);
	m_pD3DDevice->CopyRects(pPrimarySurface,
						NULL,
						0,
						pTempSurface,
						NULL);

	D3DLOCKED_RECT		lockRect;

	pTempSurface->LockRect(&lockRect,NULL,D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY );
	DWORD bpp;
	if (desc.Format == D3DFMT_X8R8G8B8)
		bpp = 32;
	else if (desc.Format == D3DFMT_R5G6B5)
		bpp = 16;

	WriteTGA(szFileName,(char*)lockRect.pBits,desc.Width,desc.Height,lockRect.Pitch,bpp);
	
	pTempSurface->UnlockRect();
	pTempSurface->Release();
	pTempTex->Release();

	pPrimarySurface->Release();


/*
	typedef struct _D3DSURFACE_DESC {
    D3DFORMAT           Format;
    D3DRESOURCETYPE     Type;
    DWORD               Usage;
    D3DPOOL             Pool;
    UINT                Size;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    UINT                Width;
    UINT                Height
		*/

	

	



	/*
	if (m_pShadowTexture[0])
	{


		m_pD3DDevice->CopyRects(m_pShadowTexture[0]->GetSurface(),
						&crect,
						1,
						pDest,
						&point);
	}*/

	
	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL __stdcall CoD3DDevice::IsCompressedTextureFormatOk()
{
	D3DFORMAT	fmt[5];
	fmt[0] = D3DFMT_DXT1;
	fmt[1] = D3DFMT_DXT2;
	fmt[2] = D3DFMT_DXT3;
	fmt[3] = D3DFMT_DXT4;
	fmt[4] = D3DFMT_DXT5;

	BOOL	bResult = FALSE;
	HRESULT hr;

	for (DWORD i=0; i<5; i++)
	{
		hr = m_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          m_d3dpp.BackBufferFormat,
                                          0,
                                          D3DRTYPE_TEXTURE,
                                          fmt[i]);
		if (hr == D3D_OK)
		{
			m_fmtDefaultCompress = fmt[i];
			bResult = TRUE;
			goto lb_return;
		}
	}

lb_return:
	return bResult;
}

BOOL __stdcall CoD3DDevice::ConvertCompressedTexture(char* szFileName,DWORD dwFlag)
{
	return m_rcManager.ConvertCompressedTexture(szFileName,dwFlag);
}

void __stdcall CoD3DDevice::SetVerticalSync(BOOL bSwitch)
{
	if (bSwitch == m_bVSync)
		return;

	m_bVSync = bSwitch;

	if (m_bVSync)
	{
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
	}
	else
	{
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}
	ResetDevice(FALSE);
}

BOOL __stdcall CoD3DDevice::IsSetVerticalSync()
{
	return m_bVSync;
}
void __stdcall CoD3DDevice::SetTickCount(DWORD dwTickCount,BOOL bGameFrame)
{
	m_dwTickCount = dwTickCount;
	m_bGameFrame = bGameFrame;
	
}
void CoD3DDevice::OutputResourcePoolStatus()
{
	char	txt[512];
	memset(txt,0,512);
	wsprintf(txt,"HField Num : %d \n",m_dwHFieldNum);
	OutputDebugString(txt);



	/*
		BOOL	bResult = FALSE;
	if (m_dwResourcePoolNum >= MAX_RESOURCE_POOL_NUM)
		goto lb_return;

	IResourcePool*	pResourcePool;

	switch(type)
	{
	case RESORUCE_POOL_TYPE_VBSTATICPOOL:
		{
			CStaticVBPool*	pVBStaticPool;
			pVBStaticPool = new CStaticVBPool;
			pResourcePool = (IResourcePool*)pVBStaticPool;
		}
		break;
	case RESORUCE_POOL_TYPE_IBSTATICPOOL:
		{
			CStaticIBPool*	pIBStaticPool;
			pIBStaticPool = new CStaticIBPool;
			pResourcePool = (IResourcePool*)pIBStaticPool;
		}
		break;
	case RESORUCE_POOL_TYPE_VBHEAP:
		{
			CVBHeap*	pVBHeap;
			pVBHeap = new CVBHeap;
			pResourcePool = (IResourcePool*)pVBHeap;
		}
		break;

	case RESORUCE_POOL_TYPE_IBHEAP:
		{
			CIBHeap*	pIBHeap;
			pIBHeap = new CIBHeap;
			pResourcePool = (IResourcePool*)pIBHeap;

		}
		break;*/
	wsprintf(txt,"HField Num : %d \n",m_dwHFieldNum);
	OutputDebugString(txt);


	__asm int 3


}

void CoD3DDevice::SetSphereMapMatrix(MATRIX4* pResultMat,MATRIX4* pMatWorld,MATRIX4* pMatView)
{
	MatrixMultiply2(pResultMat,pMatWorld,pMatView);

	pResultMat->_11 *= 0.5f;
	pResultMat->_21 *= 0.5f;
	pResultMat->_31 *= 0.5f;

	pResultMat->_12 *= -0.5f;
	pResultMat->_22 *= -0.5f;
	pResultMat->_32 *= -0.5f;

	pResultMat->_14 = 0.0f;
	pResultMat->_24 = 0.0f;
	pResultMat->_34 = 0.0f;


	pResultMat->_41 = 0.5f;
	pResultMat->_42 = 0.5f;

	pResultMat->_43 = 0.0f;
	pResultMat->_44 = 1.0f;
	
}


void CoD3DDevice::SetWaveTexMatrix(MATRIX4* pMatResult)
{

	SetIdentityMatrix(pMatResult);
/*	__asm 
	{
		rdtsc	
		mov		dword ptr[dwTick],eax
	}*/
	float	fXTrans, fYTrans;

	fXTrans	=	float(m_dwTickCount)	/	1000.0f;
	fXTrans	=	sinf( fXTrans);
	pMatResult->_31	=	fXTrans;		// count2라서 3*3행렬을 쓴다

	fYTrans	=	float(m_dwTickCount)	/	1250.0f;
	fYTrans	=	sinf( fYTrans);
	pMatResult->_32	=	fYTrans;
}