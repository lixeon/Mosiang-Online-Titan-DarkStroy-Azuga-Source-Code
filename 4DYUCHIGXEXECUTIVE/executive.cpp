#include "gxobject.h"
#include "GXEventTriggerObject.h"
#include "executive.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_myself97/myself97common.h"
#include "../4DyuchiGRX_myself97/CheckClock.h"
#include <crtdbg.h>
#include "GXDefault.h"

#include "printtofile.h"
extern LONG			g_lComponent;
extern ofstream logFile;
// warning number 4012 is 'unreferenced label' warning.
#pragma warning( disable : 4102 )


ErrorHandleProc			g_pErrorHandleFunc = NULL;
I4DyuchiFileStorage*	g_pFileStorage = NULL;


DWORD __stdcall DefaultErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	/*
	char szFileName[] = __FILE__;
	int iLineNumber = __LINE__;*/
/*
	ERROR_TYPE_ENGINE_CODE			=	0,
	ERROR_TYPE_PARAMETER_INVALID	=	1,
	ERROR_TYPE_DEVICE_NOT_SUPPROT	=	2,
	ERROR_TYPE_D3D_ERROR			=	3,
	ERROR_TYPE_RESOURCE_LEAK		=	4,
	ERROR_TYPE_FILE_NOT_FOUND		=	5
*/
	char*	szErrorType;
	switch( type)
	{
	case	ERROR_TYPE_ENGINE_CODE:
		szErrorType	=	"ERROR_TYPE_ENGINE_CODE";
		break;
	case	ERROR_TYPE_PARAMETER_INVALID:
		szErrorType	=	"ERROR_TYPE_PARAMETER_INVALID";
		break;
	case	ERROR_TYPE_DEVICE_NOT_SUPPROT:
		szErrorType	=	"ERROR_TYPE_DEVICE_NOT_SUPPROT";
		break;
	case	ERROR_TYPE_D3D_ERROR:
		szErrorType	=	"ERROR_TYPE_D3D_ERROR";
		break;
	case	ERROR_TYPE_RESOURCE_LEAK:
		szErrorType	=	"ERROR_TYPE_RESOURCE_LEAK";
		break;
	case	ERROR_TYPE_FILE_NOT_FOUND:
		szErrorType	=	"ERROR_TYPE_FILE_NOT_FOUND";
		break;
	default:
		_asm int 3;
	}

	PrintfDebugString(":DefaultErrorHandleProc %s, %s\n",szErrorType, szStr);

	if (!dwErrorPriority)
		__asm int 3
	return 0;
}

#ifdef	_DEBUG
void CoExecutive::CheckHandle(GXMAP_OBJECT_HANDLE gxh)
{
	if( IsValidHandle(gxh) == GX_MAP_OBJECT_TYPE_INVALID)
	{
		char	txt[512];
		wsprintf(txt,"Invalid GXMapObjectHandle : %x\n",gxh);
		OutputDebugString(txt);
		__asm int 3;
	}

}

void CoExecutive::CheckGXLightsList(CoGXLight* pInLight)
{
	DWORD dwLightNum = ITGetItemNum(m_pIndexItemTableGXLight);
	CoGXLight* pLight;
	for (DWORD i=0; i<dwLightNum; i++)
	{
		pLight = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
		if (pLight == pInLight)
			__asm int 3
	}
}

void CoExecutive::CheckGXObjectsList(CoGXObject* pInObject)
{	
	DWORD dwNum = ITGetItemNum(m_pIndexItemTableGXObject);
	CoGXObject* pGXO;
	for (DWORD i=0; i<dwNum; i++)
	{
		pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);
		if (pGXO == pInObject)
			__asm int 3
	}
}

void CoExecutive::CheckGXTriggersList(CoGXEventTrigger* pInTrigger)
{	
	DWORD dwNum = ITGetItemNum(m_pIndexItemTableGXTrigger);
	CoGXEventTrigger* pGXT;
	for (DWORD i=0; i<dwNum; i++)
	{
		pGXT = (CoGXEventTrigger*)ITGetItemSequential(m_pIndexItemTableGXTrigger,i);
		if (pGXT == pInTrigger)
			__asm int 3
	}
}
void CoExecutive::CheckGXDecalsList(CGXDecal* pInDecal)
{
	DWORD dwNum = ITGetItemNum(m_pIndexItemTableGXDecal);
	CGXDecal* pGXD;
	for (DWORD i=0; i<dwNum; i++)
	{
		pGXD = (CGXDecal*)ITGetItemSequential(m_pIndexItemTableGXDecal,i);
		if (pGXD == pInDecal)
			__asm int 3
	}
}

#endif



BOOL __stdcall Def_DeleteGXObject(CoExecutive* pExecutive,GXOBJECT_HANDLE gxh)
{
	return pExecutive->ImmDeleteGXObject(gxh);
}

BOOL __stdcall Def_DeleteGXLight(CoExecutive* pExecutive,GXLIGHT_HANDLE gxh)
{	
	return pExecutive->ImmDeleteGXLight(gxh);
}
BOOL __stdcall Def_DeleteGXEventTrigger(CoExecutive* pExecutive,GXTRIGGER_HANDLE gxh)
{
	return pExecutive->ImmDeleteGXEventTrigger(gxh);	
}

BOOL __stdcall Def_DeleteGXDecal(CoExecutive* pExecutive,GXDECAL_HANDLE gxh)
{
	return pExecutive->ImmDeleteGXDecal(gxh);
}

STDMETHODIMP CoExecutive::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiGXExecutive == refiid)
		*ppv = (I4DyuchiGXExecutive*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoExecutive::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CoExecutive::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
CoExecutive::CoExecutive()
{
	InterlockedIncrement( &g_lComponent);

	memset((char*)this+4,0,sizeof(CoExecutive)-4);
	
	m_dwGameFPS = 30;
	SetFramePerSec(m_dwGameFPS);
	m_pDummyGXObject = new CoGXObject;
	m_pDummyGXLight = new CoGXLight;
	m_pDummyGXDecal = new CGXDecal;

	SetCanDelete();


#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif

	m_dwPickingMode = 0L;
	m_dwRenderMode  = 0L;

//	m_Collision.Init();		// 2002/05/29
}
void CoExecutive::SetFramePerSec(DWORD dwFrame)
{

	DWORD	dwCurFrameCount;
	DWORD	dwCurrentTick;

	if (dwFrame > 1000)
		goto lb_return;
	
	if (!dwFrame)
		goto lb_return;

	dwCurrentTick = GetTickCount();
	
	m_dwGameFPS = dwFrame;
	m_dwTicksPerFrame = 1000 / m_dwGameFPS;
	
	m_dwInitialTick = dwCurrentTick;
	dwCurFrameCount = (dwCurrentTick - m_dwInitialTick) / m_dwTicksPerFrame;
	m_dwPrvFrameCount = dwCurFrameCount;


lb_return:
	return;

}
DWORD CoExecutive::GetFramePerSec()
{
	return m_dwGameFPS;
	
}

void __stdcall CoExecutive::SetViewport(DWORD dwViewportIndex)
{
	m_dwViewportIndex = dwViewportIndex;
}
BOOL __stdcall CoExecutive::InitializeFileStorageWithoutRegistry(char* szFileName,DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum)
{
	
	CREATE_INSTANCE_FUNC	pFunc;


	HRESULT		hr;
	void*	pCreateFunc = NULL;
	BOOL	bResult = FALSE;
	
	m_hStorage = LoadLibrary(szFileName);
	if (!m_hStorage)
		goto lb_fail;

	pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(m_hStorage,"DllCreateInstance");
	hr = pFunc((void**)&m_pFileStorage);

	if (hr != S_OK)
	{
lb_fail:
		MessageBox(NULL,"Fail to Create 4DyuchiFileStorage","Error",MB_OK);
		goto lb_return;
	}
	bResult = InitializeFileStorageCommon(dwMaxFileNum,dwMaxFileHandleNumAtSameTime,dwMaxFileNameLen,accessMethod,pPackFileList,dwNum);

lb_return:
	return bResult;

}
BOOL __stdcall CoExecutive::InitializeFileStorage(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum)
{
	BOOL	bResult = FALSE;

	HRESULT hr;

	hr = CoCreateInstance(
           CLSID_4DyuchiFileStorage,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiFileStorage,
           (void**)&m_pFileStorage);

	if (hr != S_OK)
	{
		__asm int 3
		goto lb_return;
	}
	bResult = InitializeFileStorageCommon(dwMaxFileNum,dwMaxFileHandleNumAtSameTime,dwMaxFileNameLen,accessMethod,pPackFileList,dwNum);

	
lb_return:
	return bResult;
}

BOOL CoExecutive::InitializeFileStorageCommon(DWORD dwMaxFileNum,DWORD dwMaxFileHandleNumAtSameTime,DWORD dwMaxFileNameLen,FILE_ACCESS_METHOD accessMethod,PACKFILE_NAME_TABLE* pPackFileList,DWORD dwNum)
{
	BOOL	bResult = FALSE;

	g_pFileStorage = m_pFileStorage;


	if (!pPackFileList || !dwNum)
		accessMethod = FILE_ACCESS_METHOD_ONLY_FILE;

	bResult = m_pFileStorage->Initialize(dwMaxFileNum,dwMaxFileHandleNumAtSameTime,dwMaxFileNameLen,accessMethod);
	if (!bResult)
		goto lb_return;


	DWORD i;
	for (i=0; i<dwNum; i++)
	{
		m_pPackFileHandleList[m_dwPackFilesNum] = m_pFileStorage->MapPackFile(pPackFileList[i].szFileName);
		m_dwPackFilesNum++;
	}
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::Initialize(HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc)
{
	HRESULT hr;

	BOOL	bResult = FALSE;

	I4DyuchiGXGeometry*		pGeometry = NULL;
	I4DyuchiGXRenderer*		pRenderer = NULL;

	if (hWnd)
	{
		hr = CoCreateInstance(
			   CLSID_4DyuchiGXRenderer,
			   NULL,
			   CLSCTX_INPROC_SERVER,
			   IID_4DyuchiGXRenderer,
			   (void**)&pRenderer);

		if (hr != S_OK)
		{
			MessageBox(NULL,"Fail to Create 4DyuchiGXRenderer","Error",MB_OK);
			goto lb_return;
		}

	
	}
	
	hr = CoCreateInstance(
           CLSID_4DyuchiGXGeometry,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiGXGeometry,
           (void**)&pGeometry);


	if (hr != S_OK)
	{
		MessageBox(NULL,"Fail to Create 4DyuchiGXGeometry","Error",MB_OK);
		goto lb_return;
	}

	bResult = Initialize(pGeometry,pRenderer,hWnd,pInfo,dwMaxObjectNum,dwMaxLightNum,dwMaxTriggerNum,dwViewportNum,dwMaxDecalNum,pErrorHandleFunc);

lb_return:
	return bResult;
}



BOOL __stdcall CoExecutive::InitializeWithoutRegistry(char* szGeometryFileName,char* szRendererFileName,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc)
{
	I4DyuchiGXGeometry*		pGeometry = NULL;
	I4DyuchiGXRenderer*		pRenderer = NULL;

	CREATE_INSTANCE_FUNC	pFunc;


	HRESULT		hr;
	void*	pCreateFunc = NULL;
	BOOL	bResult = FALSE;
	if (hWnd)
	{
		m_hRenderer = LoadLibrary(szRendererFileName);
		if (!m_hRenderer)
			goto lb_fail_renderer;

		pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(m_hRenderer,"DllCreateInstance");
		hr = pFunc((void**)&pRenderer);

		if (hr != S_OK)
		{
lb_fail_renderer:
			MessageBox(NULL,"Fail to Create 4DyuchiGXRenderer","Error",MB_OK);
			goto lb_return;
		}
	}

	m_hGeometry = LoadLibrary(szGeometryFileName);
	if (!m_hGeometry)
		goto lb_fail_geometry;

	pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(m_hGeometry,"DllCreateInstance");
	hr = pFunc((void**)&pGeometry);

	if (hr != S_OK)
	{
lb_fail_geometry:
		MessageBox(NULL,"Fail to Create 4DyuchiGXGeometry","Error",MB_OK);
		goto lb_return;
	}
	bResult = Initialize(pGeometry,pRenderer,hWnd,pInfo,dwMaxObjectNum,dwMaxLightNum,dwMaxTriggerNum,dwViewportNum,dwMaxDecalNum,pErrorHandleFunc);

lb_return:
	return bResult;


}
BOOL CoExecutive::Initialize(I4DyuchiGXGeometry* pGeometry,I4DyuchiGXRenderer* pRenderer,HWND hWnd,DISPLAY_INFO* pInfo,DWORD dwMaxObjectNum,DWORD dwMaxLightNum,DWORD dwMaxTriggerNum,DWORD dwViewportNum, DWORD dwMaxDecalNum,ErrorHandleProc pErrorHandleFunc)
{
	BOOL	bResult = FALSE;

	
	m_pGeometry = pGeometry;
	m_pRenderer = pRenderer;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if (m_pRenderer)
		m_pRenderer->Create(hWnd,pInfo,m_pFileStorage,pErrorHandleFunc);
	

	m_dwMaxObjectNum = dwMaxObjectNum;
	m_dwMaxTriggerNum = dwMaxTriggerNum;
	m_dwMaxDecalNum	=	dwMaxDecalNum;
	m_dwMaxMapObjectsNum = m_dwMaxObjectNum + m_dwMaxTriggerNum + m_dwMaxDecalNum;



	g_pErrorHandleFunc = pErrorHandleFunc;
	if (!g_pErrorHandleFunc)
		g_pErrorHandleFunc = DefaultErrorHandleProc;

	m_dwCurrentDecalCount	=	0;
//	m_DecalIndexManager.Init( dwMaxDecalNum);
	
	

		

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	m_pGeometry->Initialize(m_pRenderer,m_pFileStorage,dwViewportNum,MAX_FILEITEM_NUM,MAX_FILEBUCKET_NUM,pErrorHandleFunc);	// 초기화

	m_pIndexItemTableGXObject = ITCreate();
	ITInitialize(m_pIndexItemTableGXObject,dwMaxObjectNum);

	m_pStaticPoolGXObject = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pStaticPoolGXObject,sizeof(CoGXObject),dwMaxObjectNum / 8,dwMaxObjectNum);

	m_pIndexItemTableGXLight = ITCreate();
	ITInitialize(m_pIndexItemTableGXLight,dwMaxLightNum);

	m_pIndexItemTableGXTrigger = ITCreate();
	ITInitialize(m_pIndexItemTableGXTrigger,dwMaxTriggerNum);

	m_pIndexItemTableGXDecal = ITCreate();
	ITInitialize(m_pIndexItemTableGXDecal,dwMaxDecalNum);



	m_pStaticPoolGXLight = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pStaticPoolGXLight,sizeof(CoGXLight),dwMaxLightNum / 8,dwMaxLightNum);
	

	m_pStaticPoolGXDecal = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pStaticPoolGXDecal,sizeof(CGXDecal),dwMaxDecalNum / 8,dwMaxDecalNum);


	m_ppGXLightList = new CoGXLight*[dwMaxLightNum];
	memset(m_ppGXLightList,0,sizeof(CoGXLight*)*dwMaxLightNum);


	m_pIDHash = QBHCreate();
	QBHInitialize(m_pIDHash,1024,dwMaxObjectNum+dwMaxLightNum);

	m_pDPCQ = DPCQCreate();
	DPCQInitialize(m_pDPCQ,8192*4);


	m_pFileNameHash = VBHCreate();
	VBHInitialize(m_pFileNameHash,128,MAX_NAME_LEN,dwMaxObjectNum);
//	m_pStaticPoolFileItem = CreateStaticMemoryPool();
//	InitializeStaticMemoryPool(m_pStaticPoolFileItem,sizeof(FILE_ITEM),dwMaxObjectNum / 8,dwMaxObjectNum);
	
	m_dwInitialTick = GetTickCount();

	bResult = TRUE;

lb_return:
	return bResult;
}
HRESULT __stdcall CoExecutive::GetFileStorage(I4DyuchiFileStorage** ppFileStorage)
{
	HRESULT		hResult = -1;
	if (!m_pFileStorage)
		goto lb_return;
	
	*ppFileStorage = m_pFileStorage;
	m_pFileStorage->AddRef();
	hResult = S_OK;

lb_return:
	return hResult;
}

HRESULT __stdcall CoExecutive::GetRenderer(I4DyuchiGXRenderer** ppRenderer)
{
	HRESULT		hResult = -1;
	if (!m_pRenderer)
		goto lb_return;
	
	*ppRenderer = m_pRenderer;
	m_pRenderer->AddRef();
	hResult = S_OK;
lb_return:
	return hResult;
}
HRESULT __stdcall CoExecutive::GetGeometry(I4DyuchiGXGeometry** ppGeometry)
{
	HRESULT		hResult = -1;
	if (!m_pGeometry)
		goto lb_return;
	
	*ppGeometry = m_pGeometry;
	m_pGeometry->AddRef();
	hResult = S_OK;
lb_return:
	return hResult;
}

I4DyuchiGXRenderer* CoExecutive::GetRenderer()	
{
	return m_pRenderer;
}

I4DyuchiGXGeometry* CoExecutive::GetGeometry()
{
	return m_pGeometry;
}
/*
GXOBJECT_HANDLE	__stdcall CoExecutive::GXOGetTracedGXObject()
{
	return (GXOBJECT_HANDLE)m_pTracedGXObject;
}

void __stdcall CoExecutive::GXOSetTracedGXObject(GXOBJECT_HANDLE gxo) 
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	m_pTracedGXObject = (CoGXObject*)gxo;
}
*/
ULONG __stdcall	CoExecutive::GXOGetModel(GXOBJECT_HANDLE gxh,I3DModel** ppModel,DWORD* pdwRefIndex,DWORD dwModelIndex)
{

#ifdef	_DEBUG
	CheckHandle(gxh);
#endif

	*pdwRefIndex = ((CoGXObject*)gxh)->GetModelRefIndex(dwModelIndex);
	*ppModel = ((CoGXObject*)gxh)->GetModel(dwModelIndex);
	(*ppModel)->AddRef();

	return 0;
}

void __stdcall CoExecutive::GXOSetEffectIndex(GXOBJECT_HANDLE gxo,DWORD dwEffectIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->SetEffectIndex(dwEffectIndex);
}
DWORD __stdcall	CoExecutive::GXOGetEffectIndex(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif

	return ((CoGXObject*)gxo)->GetEffectIndex();
}

BOOL CoExecutive::GXOGetMotionDesc(GXOBJECT_HANDLE gxh,MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwModelIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif

	return ((CoGXObject*)gxh)->GetMotionDesc(pMotionDesc,dwMotionIndex,dwModelIndex);
}
DWORD CoExecutive::GXOGetCurrentFrame(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetCurrentFrame();
}
DWORD CoExecutive::GXOGetCurrentMotionIndex(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetCurrentMotionIndex();
}
void CoExecutive::GXOSetCurrentFrame(GXOBJECT_HANDLE gxh,DWORD dwFrame)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetCurrentFrame(dwFrame);
}
void CoExecutive::GXOSetCurrentMotionIndex(GXOBJECT_HANDLE gxh,DWORD dwMotionIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetCurrentMotionIndex(dwMotionIndex);
}
void CoExecutive::GXOSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos, BOOL bDoInterpolation)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetPosition(pv3Pos, bDoInterpolation);

}
void CoExecutive::GXOMovePosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->MovePosition(pv3Pos);
	
}
DWORD __stdcall CoExecutive::GXOGetMaterialNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetMaterialNum(dwModelIndex);

}
DWORD __stdcall CoExecutive::GXOGetCurrentMaterialIndex(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetCurrentMaterialIndex();
}
void __stdcall CoExecutive::GXOSetCurrentMaterialIndex(GXOBJECT_HANDLE gxh,DWORD dwMaterialIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetCurrentMaterialIndex(dwMaterialIndex);
}

void __stdcall CoExecutive::GXOSetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetScale(pv3Scale);
	
}
void __stdcall CoExecutive::GXOGetScale(GXOBJECT_HANDLE gxh,VECTOR3* pv3Scale)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetScale(pv3Scale);
}

void CoExecutive::GXOSetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float fRad)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->SetDirection(pv3Axis,fRad);
}

void __stdcall CoExecutive::GXOSetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angles /* in */)
{
#ifdef _DEBUG
	CheckHandle(gxh);	
#endif
	((CoGXObject*)gxh)->SetDirectionFPSStyle(pv3Angles);
}


void __stdcall CoExecutive::GXOChangeDirection(GXOBJECT_HANDLE gxh,float fRad)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->ChangeDirection(fRad);
}
void CoExecutive::GXOGetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetPosition(pv3Pos);
}
// 보간되어서 랜더링 되는 포지션을 출력. 실제 GXO의 위치는 아니다. 카메라위치 갱신할 때 쓰면 좋을듯.
void CoExecutive::GXOGetInterpolatedPosition( GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetInterpolatedPosition(pv3Pos);
}


void CoExecutive::GXOGetDirection(GXOBJECT_HANDLE gxh,VECTOR3* pv3Axis,float* pfRad)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetDirection(pv3Axis,pfRad);
}

void CoExecutive::GXOGetDirectionFPSStyle(GXOBJECT_HANDLE gxh, VECTOR3* pv3Angle /* out */)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetDirectionFPSStyle(pv3Angle);
}


BOOL __stdcall CoExecutive::GXOReplaceModel(GXOBJECT_HANDLE gxh,DWORD dwModelIndex,char* szFileName)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->Replace(dwModelIndex,szFileName);
}


void __stdcall CoExecutive::GXOGetCollisionMesh(GXOBJECT_HANDLE gxh,COLLISION_MESH_OBJECT_DESC* pColMeshDesc)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	*pColMeshDesc = *((CoGXObject*)gxh)->GetCollisionMeshObjectDesc();
}

BOOL __stdcall CoExecutive::GXOGetWorldMatrixPerObject(GXOBJECT_HANDLE gxh,MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetWorldMatrixPerObject(pMatWorld,dwModelIndex,dwObjIndex);
}

DWORD __stdcall CoExecutive::GXOGetObjectIndex(GXOBJECT_HANDLE gxh,char* szObjName,DWORD dwModelIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetObjectIndex(szObjName,dwModelIndex);
}

void* __stdcall CoExecutive::GetData(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetData();
}


void __stdcall CoExecutive::SetData(GXMAP_OBJECT_HANDLE gxh,void* pData)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->SetData(pData);
}

void __stdcall CoExecutive::SetPickType(GXMAP_OBJECT_HANDLE	gxh,PICK_TYPE type)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->SetPickType(type);
}

PICK_TYPE __stdcall CoExecutive::GetPickType(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetPickType();
}
void __stdcall CoExecutive::EnablePick(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->EnablePick();
}
void __stdcall CoExecutive::DisablePick(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->DisablePick();
}
void __stdcall CoExecutive::GXTSetPosition(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXEventTrigger*)gxh)->SetPosition(pv3Pos);
}

void __stdcall CoExecutive::GXTSetScale(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Scale)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXEventTrigger*)gxh)->SetScale(pv3Scale);
}

void __stdcall CoExecutive::GXTSetRotation(GXTRIGGER_HANDLE gxh,VECTOR3* pv3Rot)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXEventTrigger*)gxh)->SetRotation(pv3Rot);
}

void __stdcall CoExecutive::GXTGetEventTriggerDesc(GXTRIGGER_HANDLE gxh,EVENT_TRIGGER_DESC* pEVDesc)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXEventTrigger*)gxh)->GetEventTriggerDesc(pEVDesc);

}

BOOL __stdcall CoExecutive::IsInViewVolume(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	BOOL	bResult = FALSE;
	if ( ((CGXMapObject*)gxh)->GetRenderFrameCount() == m_dwRenderFrameCount)
		bResult = TRUE;

	return bResult;
}
void __stdcall CoExecutive::DisableRender(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->DisableRender();
}
void __stdcall CoExecutive::EnableRender(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->EnableRender();
}
void __stdcall CoExecutive::DisableSchedule(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->DisableSchedule();
}


void __stdcall CoExecutive::LockTransform(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->LockTransform();

}
void __stdcall CoExecutive::UnLockTransform(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->UnLockTransform();
}

BOOL __stdcall CoExecutive::IsLockTransform(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->IsLockTransform();
}



void __stdcall CoExecutive::EnableSchedule(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->EnableSchedule();
}
void __stdcall CoExecutive::SetRenderFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->SetRenderFlag(dwFlag);
}
DWORD __stdcall	CoExecutive::GetRenderFlag(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetRenderFlag();
}
void __stdcall CoExecutive::SetAlphaFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->SetAlphaFlag(dwFlag);
}
DWORD __stdcall CoExecutive::GetAlphaFlag(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetAlphaFlag();
}
DWORD __stdcall CoExecutive::GetID(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetID();
}


BOOL __stdcall CoExecutive::SetID(GXMAP_OBJECT_HANDLE gxh,DWORD dwID)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->SetID(dwID);
}
void __stdcall CoExecutive::ReleaseID(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->ReleaseID();
}
void __stdcall CoExecutive::SetPropertyFlag(GXMAP_OBJECT_HANDLE gxh,DWORD dwFlag)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->SetPropertyFlag(dwFlag);
}
DWORD __stdcall CoExecutive::GetPropertyFlag(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->GetPropertyFlag();
}
BOOL __stdcall CoExecutive::IsRenderable(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CGXMapObject*)gxh)->IsRenderable();
}
void __stdcall CoExecutive::EnableUpdateShading(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->EnableUpdateShading();
}
void __stdcall CoExecutive::DisableUpdateShading(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->DisableUpdateShading();
}

void __stdcall CoExecutive::EnableSendShadow(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->EnableSendShadow();
}

void __stdcall CoExecutive::DisableSendShadow(GXMAP_OBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CGXMapObject*)gxh)->DisableSendShadow();
}

GXMAP_OBJECT_HANDLE __stdcall CoExecutive::GetGXMapObjectWithID(DWORD dwID)
{
	//	QBHSelect(QBHASH_HANDLE pHash,DWORD OUT* pItems,DWORD dwMaxItemNum,DWORD dwKeyData);
	CGXMapObject*	pObj = NULL;
	void*	pSearchHandle;
	QBHSelect(m_pIDHash,&pSearchHandle,(DWORD*)&pObj,1,dwID);
	return pObj;
}

DWORD CoExecutive::GXOGetMotionNum(GXOBJECT_HANDLE gxh,DWORD dwModelIndex)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	return ((CoGXObject*)gxh)->GetMotionNum(dwModelIndex);
}
void CoExecutive::GXOMoveForward(GXOBJECT_HANDLE gxh,float fDistance)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->MoveForward(fDistance);

}
void __stdcall CoExecutive::EstimatedMoveForward(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos,float fDistance)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->EstimatedMoveForward(pv3Pos,fDistance);
}



BOOL CoExecutive::GXOAttachLight(GXOBJECT_HANDLE gxoh,char* szObjName,GXLIGHT_HANDLE gxlh,VECTOR3* pv3Offset,DWORD dwAttachType)
{
#ifdef	_DEBUG
	CheckHandle(gxoh);
	CheckHandle(gxlh);
#endif

	return ((CoGXObject*)gxoh)->AttachLight((CoGXLight*)gxlh,szObjName,pv3Offset,dwAttachType);
}
BOOL __stdcall CoExecutive::GXODetachLight(GXOBJECT_HANDLE gxoh,GXLIGHT_HANDLE child_gxlh)
{
#ifdef	_DEBUG
	CheckHandle(gxoh);
	CheckHandle(child_gxlh);
#endif
	return ((CoGXObject*)gxoh)->DetachLight((CoGXLight*)child_gxlh);
}
void __stdcall CoExecutive::GXLDisableStaticShadow(GXLIGHT_HANDLE gxl)
{
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif

	((CoGXLight*)gxl)->DisableStaticShadow();
}
void __stdcall CoExecutive::GXLEnableStaticShadow(GXLIGHT_HANDLE gxl)
{
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif

	((CoGXLight*)gxl)->EnableStaticShadow();
}
BOOL __stdcall CoExecutive::GXLIsDisableStaticShadow(GXLIGHT_HANDLE gxl)
{
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif

	return ((CoGXLight*)gxl)->IsDiableStaticShadow();
}

void __stdcall CoExecutive::GXLDisableDynamicLight(GXLIGHT_HANDLE gxl)
{
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif
	((CoGXLight*)gxl)->DisableDynamicLight();
}
void __stdcall CoExecutive::GXLEnableDynamicLight(GXLIGHT_HANDLE gxl)
{
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif
	((CoGXLight*)gxl)->EnableDynamicLight();
}

BOOL __stdcall CoExecutive::GXLIsEnableDynamicLight(GXLIGHT_HANDLE gxl)
{	
#ifdef	_DEBUG
	CheckHandle(gxl);
#endif
	return ((CoGXLight*)gxl)->IsEnableDynamicLight();
}
void __stdcall CoExecutive::GXLSetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXLight*)gxh)->SetLightDesc(pLightDesc);
}

void __stdcall CoExecutive::GXLGetLightDesc(GXLIGHT_HANDLE gxh,LIGHT_DESC* pLightDesc)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif

	((CoGXLight*)gxh)->GetLightDesc(pLightDesc);
}
void __stdcall CoExecutive::GXLSetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXLight*)gxh)->SetPosition(pv3Pos);
}
void __stdcall CoExecutive::GXLMovePosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXLight*)gxh)->MovePosition(pv3Pos);
}
void __stdcall CoExecutive::GXLGetPosition(GXLIGHT_HANDLE gxh,VECTOR3* pv3Pos)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXLight*)gxh)->GetPosition(pv3Pos);
}

void __stdcall CoExecutive::GXOEnableHFieldApply(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->EnableHFieldApply();
}
void __stdcall CoExecutive::GXODisableHFieldApply(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->DisableHFieldApply();
}
BOOL __stdcall CoExecutive::GXOIsHFieldApply(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	return ((CoGXObject*)gxo)->IsHFieldApply();
}
BOOL __stdcall CoExecutive::GXOIsAsEffect(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	return ((CoGXObject*)gxo)->IsAsEffect();
}

void __stdcall CoExecutive::GXOSetZOrder(GXOBJECT_HANDLE gxo,int iZOrder)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->SetZOrder(iZOrder);

}
void __stdcall CoExecutive::GXOEnableAsEffect(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->EnableAsEffect();
}


void __stdcall CoExecutive::GXODisableAsEffect(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->DisableAsEffect();
}

void __stdcall CoExecutive::GXOEnableScaleOfAttachedModel(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->SetAxisAlignOK(TRUE);
}

void __stdcall CoExecutive::GXODisableScaleOfAttachedModel(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->SetAxisAlignOK(FALSE);

}


void __stdcall	CoExecutive::GXOEnableSelfIllumin(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->EnableSelfIllumin();
}
void __stdcall	CoExecutive::GXODisableSelfIllumin(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	((CoGXObject*)gxo)->DisableSelfIllumin();
}

BOOL __stdcall CoExecutive::GXOIsEnableSelfIllumin(GXOBJECT_HANDLE gxo)
{
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	return ((CoGXObject*)gxo)->IsEnableSelfIllumin();
}

GXOBJECT_HANDLE CoExecutive::CreateGXObject(char* szFileName,GXSchedulePROC pProc,void* pData,DWORD dwFlag)
{
	CoGXObject*	gxh = NULL;
	
	char		szSearchFileName[256];
	GetNameRemovePath(szSearchFileName,szFileName);

	DWORD			dwSearchNameLen = lstrlen(szSearchFileName);
	CharToSmallASCII(szSearchFileName,szSearchFileName,dwSearchNameLen);

	DWORD			dwModelNum = 0;
	MODEL_HANDLE	modelHandle[MAX_MODEL_NUM_PER_GXOBJECT];
	DWORD			i;

	DWORD				dwItem;
	MODEL_FILE_DESC*	pModDesc = NULL;
	
	
	if (GXOBJECT_CREATE_TYPE_NOT_USE_MODEL & dwFlag)
	{
		goto lb_add_gxo;
	}


	if (VBHSelect(m_pFileNameHash,&dwItem,1,szSearchFileName,dwSearchNameLen))
	{
		pModDesc = (MODEL_FILE_DESC*)dwItem;
	
		dwModelNum = pModDesc->pGXObject->DuplicateModelHandle(modelHandle);
		if (!dwModelNum)
			goto lb_return;

		pModDesc->dwRefCount++;
		goto lb_add_gxo;

	}
	




	dwModelNum = LoadGXObject(modelHandle,MAX_MODEL_NUM_PER_GXOBJECT,szFileName);
	if (!dwModelNum)
		goto lb_return;

	pModDesc = new MODEL_FILE_DESC;
	memset(pModDesc,0,sizeof(MODEL_FILE_DESC));
	pModDesc->dwRefCount = 1;
	memcpy(pModDesc->szFileName,szSearchFileName,dwSearchNameLen);

	pModDesc->pSearchHandle = VBHInsert(m_pFileNameHash,(DWORD)pModDesc,szSearchFileName,dwSearchNameLen);
	if (!pModDesc->pSearchHandle)
	{
		delete pModDesc;
		pModDesc = NULL;
		goto lb_return;
	}

	
lb_add_gxo:
	gxh = AddGXObject(modelHandle,dwModelNum,pProc,dwFlag);

	if (!gxh)
	{
		for (i=0; i<dwModelNum; i++)
		{
			modelHandle[i].pModel->UnReference(modelHandle[i].dwRefIndex);
			modelHandle[i].pModel->Release();
		}

		DeleteModelFileDesc(pModDesc);
		goto lb_return;
	}
	if (pModDesc)
	{
		gxh->m_pPrvObject = NULL;
		gxh->m_pNextObject = NULL;

		if (!pModDesc->pGXObject)
			pModDesc->pGXObject = gxh;
		else
		{
			pModDesc->pGXObject->m_pPrvObject = gxh;
			gxh->m_pNextObject = pModDesc->pGXObject;
			pModDesc->pGXObject = gxh;
		}
	}

	gxh->SetData(pData);
	gxh->SetModelFileDesc(pModDesc);

	if (m_pgxMap)
		m_pgxMap->AddDynamicObject(gxh);
	
lb_return:
	return gxh;
}
void CoExecutive::DeleteModelFileDesc(MODEL_FILE_DESC* pModelFileDesc)
{
	if (pModelFileDesc)
	{
		pModelFileDesc->dwRefCount--;
		if (!pModelFileDesc->dwRefCount)
		{
			VBHDelete(m_pFileNameHash,pModelFileDesc->pSearchHandle);
			delete pModelFileDesc;
		}
	}
}

GXTRIGGER_HANDLE __stdcall CoExecutive::CreateGXEventTrigger(GXSchedulePROC pProc,void* pData,DWORD dwFlag)
{
	CoGXEventTrigger*	pTrigger = new CoGXEventTrigger;
#ifdef _DEBUG
	CheckGXTriggersList(pTrigger);
#endif

	DWORD dwIndex = ITAddItem(m_pIndexItemTableGXTrigger,(void*)pTrigger);
	
	if (dwIndex == 0xffffffff)
	{
		delete pTrigger;
		pTrigger = NULL;
		goto lb_return;
	}
	pTrigger->Initialize(this,pProc,dwFlag);
	pTrigger->SetIndex(dwIndex);
	
	
	if (m_pgxMap)
		m_pgxMap->AddDynamicObject(pTrigger);

lb_return:
	return (GXTRIGGER_HANDLE)pTrigger;
}

BOOL __stdcall CoExecutive::PreCreateLight(char* szFileName,DWORD dwFlag)
{
	/*
	total	7			
		255 255 255	16	
		255	255	0	16	
		255	  0	255	16
		0	255	255	16
		0	0	255	16
		0	255	0	16
		255	0	0	16
	*/
	BOOL	bResult = FALSE;

	char	buf[MAX_NAME_LEN];
	int		r,g,b,dwRS;
	DWORD	i,dwNum,dwColor;

	void* pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);

	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		goto lb_return;
	}

	g_pFileStorage->FSScanf(pFP,"%s",buf);			// total
	g_pFileStorage->FSScanf(pFP,"%d",&dwNum);		// 7

	
	for (i=0; i<dwNum; i++)
	{
		g_pFileStorage->FSScanf(pFP,"%d %d %d %d",&r,&g,&b,&dwRS);
		dwColor = (r<<16) | (g<<8) | b;
		
		m_pGeometry->CreateDynamicLight(dwRS,dwColor,NULL);	// 인덱스는 순서대로 부여된다..
	}





	g_pFileStorage->FSCloseFile(pFP);
//	fclose(fp);

	bResult = TRUE;
lb_return:
	return bResult;

}


DWORD CoExecutive::LoadGXObject(MODEL_HANDLE* pModelHandle,DWORD dwMaxModelNum,char* szFileName)
{
	char		buf[_MAX_PATH];
	DWORD		i;
	DWORD		len = lstrlen(szFileName);
	DWORD		dwModelNum = 0;
	I3DModel*	pModel = NULL;
	DWORD		dwRefIndex = 0xffffffff;
	void*		pFP = NULL;
	DWORD		dwLoadModelNum;

	char		ext[32];
	if (!GetExt(ext,szFileName))
		goto lb_return;
	
	if (!memcmp(ext,".mod",4) || !memcmp(ext,".MOD",4))
	{
		if (m_pGeometry->LoadModel(&pModel,szFileName,0) == 0xffffffff)
			goto lb_return;

		pModel->Initialize(m_pRenderer,0);
		

		dwRefIndex = pModel->Reference();
		if (dwRefIndex == 0xffffffff)
		{
			pModel->Release();
			goto lb_return;
		}

		dwModelNum = 1;
		pModelHandle[0].pModel = pModel;
		pModelHandle[0].dwRefIndex = dwRefIndex;
		
		goto lb_return;
	}

	if (!memcmp(ext,".chr",4) || !memcmp(ext,".CHR",4))
	{
		pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
		if (!pFP)
		{
			// FILE_NOT_FOUND /////////////////////////////////////////////////////////
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
			///////////////////////////////////////////////////////////////////////////
			goto lb_return;
		}

		while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
		{
			if (!lstrcmp(buf,PID_MOD_FILENAME))
			{
				if (LoadModelData(&pModelHandle[0],pFP) >= 1)
					dwModelNum = 1;
			}
		}
		goto lb_return;
	}

	if (!memcmp(ext,".chx",4) || !memcmp(ext,".CHX",4))
	{
		pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
		if (!pFP)
		{
			// FILE_NOT_FOUND /////////////////////////////////////////////////////////
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
			///////////////////////////////////////////////////////////////////////////
			goto lb_return;
		}
		g_pFileStorage->FSScanf(pFP,"%s",buf);
		g_pFileStorage->FSScanf(pFP,"%u",&dwLoadModelNum);

		if (dwLoadModelNum > dwMaxModelNum)
			dwLoadModelNum = dwMaxModelNum;


		while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
		{
			if (!lstrcmp(buf,PID_MOD_FILENAME))
				goto lb_load;
		}
		goto lb_return;

lb_load:
		for (i=0; i<dwLoadModelNum; i++)
		{
			int iResult;
			iResult = LoadModelData(&pModelHandle[i],pFP);
			if (iResult >= 1)
			{
				dwModelNum++;
			}
			else
			{
				// 앞의 모델을 읽어들이는데는 실패했으나 일단 가는데까지 가보자.이것은 디버그용으로
				// 없는 파일의 로그를 남기기 위함이다.
				if (i<dwLoadModelNum)
				{
					while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
					{
						if (!lstrcmp(buf,PID_MOD_FILENAME))
							goto lb_search_exit;
					}
				}
			}
lb_search_exit:
			__asm nop
		}
	}
lb_return:
	if (pFP)
		g_pFileStorage->FSCloseFile(pFP);
	return dwModelNum;
	
}
int CoExecutive::LoadModelData(MODEL_HANDLE* pModelHandle,void* pFP)
{


	char		buf[_MAX_PATH];
	int			iResult = 0;
	DWORD		i;

	I3DModel*	pModel = NULL;
	DWORD		dwRefIndex = 0xffffffff;
	DWORD		dwMtlSetIndex = 0;

	g_pFileStorage->FSScanf(pFP,"%s",buf);

	if (m_pGeometry->LoadModel(&pModel,buf,0) == 0xffffffff)
		goto lb_return;

	
	iResult = 1;
	pModel->Initialize(m_pRenderer,0);
	dwRefIndex = pModel->Reference();
	if (dwRefIndex == 0xffffffff)
	{
		pModel->Release();
		goto lb_return;
	}

	DWORD	dwMotionNum,dwMaterialNum;
	while (EOF != 	g_pFileStorage->FSScanf(pFP,"%s",buf))
	{
		if (!lstrcmp(buf,PID_MOTION_NUM))
		{
			g_pFileStorage->FSScanf(pFP,"%d",&dwMotionNum);

			//trustpak 2005/06/28
			if (0 < dwMotionNum)
			{
				pModel->CreateMotionList(dwMotionNum,dwRefIndex);

				for (i=0; i<dwMotionNum; i++)
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					pModel->AddMotion(buf,dwRefIndex,0);
				}
			}
			///

		}
		else if (!lstrcmp(buf,PID_MATERIAL_NUM))
		{
			g_pFileStorage->FSScanf(pFP,"%d",&dwMaterialNum);

			//trustpak 2005/06/28
			if (0 < dwMaterialNum)
			{
				pModel->CreateMaterialList(dwMaterialNum,dwRefIndex);

				for (i=0; i<dwMaterialNum; i++)
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					pModel->AddMaterial(buf,dwRefIndex,0);
				}
			}
			///

		}
		else if (!lstrcmp(buf,PID_MOD_FILENAME))
		{
			iResult = 2;
			goto lb_return;

		}
	}
lb_return:
	pModelHandle->dwRefIndex = dwRefIndex;
	pModelHandle->pModel = pModel;
	return iResult;
}
DWORD CoExecutive::PreLoadGXObject(char* szFileName)
{
	char		buf[_MAX_PATH];
	DWORD		dwLoadModelNum;
	DWORD		dwModelNum = 0;
	DWORD		i;
	DWORD		len = lstrlen(szFileName);
	I3DModel*	pModel = NULL;
	void*		pFP = NULL;


	char		ext[32];
	if (!GetExt(ext,szFileName))
		goto lb_return;
	


	if (!memcmp(ext,".mod",4) || !memcmp(ext,".MOD",4))
	{
		if (!m_pGeometry->PreLoadModel(szFileName))
			goto lb_return;

		dwModelNum = 1;
		goto lb_return;
	}

	if (!memcmp(ext,".chr",4) || !memcmp(ext,".CHR",4))
	{
		pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
	 		
		if (!pFP)
		{
			// FILE_NOT_FOUND /////////////////////////////////////////////////////////
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
			///////////////////////////////////////////////////////////////////////////
			goto lb_return;
		}
				
		while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
		{
			if (!lstrcmp(buf,PID_MOD_FILENAME))
			{
				if (PreLoadModelData(pFP) >= 1)
					dwModelNum++;
			}
		}
		goto lb_return;
	}

	if (!memcmp(ext,".chx",4) || !memcmp(ext,".CHX",4))
	{
		pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
 		
		if (!pFP)
		{
			// FILE_NOT_FOUND /////////////////////////////////////////////////////////
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
			///////////////////////////////////////////////////////////////////////////

			goto lb_return;
		}

		g_pFileStorage->FSScanf(pFP,"%s",buf);
		g_pFileStorage->FSScanf(pFP,"%u",&dwLoadModelNum);

		while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
		{
			if (!lstrcmp(buf,PID_MOD_FILENAME))
				goto lb_load;
		}
		goto lb_return;

lb_load:
		for (i=0; i<dwLoadModelNum; i++)
		{
			int iResult;
			iResult = PreLoadModelData(pFP);
			if (iResult >= 1)
			{
				dwModelNum++;
			}
			else
			{
				// 앞의 모델을 읽어들이는데는 실패했으나 일단 가는데까지 가보자.이것은 디버그용으로
				// 없는 파일의 로그를 남기기 위함이다.
				if (i<dwLoadModelNum)
				{
					while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
					{
						if (!lstrcmp(buf,PID_MOD_FILENAME))
							goto lb_search_exit;
					}
				}
			}
lb_search_exit:
			__asm nop
		}/*
		for (i=0; i<dwLoadModelNum; i++)
		{
			if (PreLoadModelData(fp) >= 1)
				dwModelNum++;
		}*/
	}
lb_return:
	if (pFP)
		g_pFileStorage->FSCloseFile(pFP);

#ifdef _DEBUG
	char txt[512];
	wsprintf(txt,"PreLoadGXObject %s, result:%d\n",szFileName,dwModelNum);
	OutputDebugString(txt);
#endif
	return dwModelNum;
	
}
void __stdcall CoExecutive::UnloadPreLoadedGXObject(GXOBJECT_HANDLE gxo)
{
	#ifdef	_DEBUG
	CheckHandle(gxo);
#endif

	CoGXObject*	pGXO = (CoGXObject*)gxo;
	pGXO->Unload();
}


void __stdcall CoExecutive::GXOSetScheduleProc(GXOBJECT_HANDLE gxo,GXSchedulePROC pProc)
{
	CoGXObject*	pGXO = (CoGXObject*)gxo;
	pGXO->SetScheduleProc(pProc);
}
GXSchedulePROC __stdcall CoExecutive::GXOGetScheduleProc(GXOBJECT_HANDLE gxo)
{
	CoGXObject*	pGXO = (CoGXObject*)gxo;
	return pGXO->GetScheduleProc();
}

int CoExecutive::PreLoadModelData(void* pFP)
{


	char		buf[_MAX_PATH];
	int			iResult = 0;
	DWORD		i;

	g_pFileStorage->FSScanf(pFP,"%s",buf);
	if (!m_pGeometry->PreLoadModel(buf))
		goto lb_return;

	iResult = 1;

	DWORD	dwMotionNum,dwMaterialNum;
	while (EOF != g_pFileStorage->FSScanf(pFP,"%s",buf))
	{
		if (!lstrcmp(buf,PID_MOTION_NUM))
		{
			g_pFileStorage->FSScanf(pFP,"%d",&dwMotionNum);
			for (i=0; i<dwMotionNum; i++)
			{
				g_pFileStorage->FSScanf(pFP,"%s",buf);
				m_pGeometry->PreLoadMotion(buf);
			}
		}
		else if (!lstrcmp(buf,PID_MATERIAL_NUM))
		{
			g_pFileStorage->FSScanf(pFP,"%d",&dwMaterialNum);
			for (i=0; i<dwMaterialNum; i++)
			{
				g_pFileStorage->FSScanf(pFP,"%s",buf);
				m_pGeometry->PreLoadMaterial(buf);
			}
		}
		else if (!lstrcmp(buf,PID_MOD_FILENAME))
		{
			iResult = 2;
			goto lb_return;

		}
	}
lb_return:
	return iResult;
}
void __stdcall CoExecutive::UnloadAllPreLoadedGXObject(DWORD dwFlag)
{
	m_pGeometry->UnloadAllPreLoadedItem(dwFlag);
}

	
GXMAP_HANDLE __stdcall CoExecutive::CreateGXMap(GXMapSchedulePROC pProc,void* pData,DWORD dwFlag)
{

	if (m_pgxMap)
	{
#ifdef _DEBUG
		PrintfDebugString("이미 GXMap이 존재하는 상태에서 CreateGXMap()을 호출하였다. (Did you forget to delete previous GXMap?\n");
		__asm int 3
#endif
		delete m_pgxMap;
	}

	CGXMapObject*	pGXMapObject;
	DWORD	dwCurGXMapObjectNum = 0;
		
	DWORD	dwGXONum,dwGXLNum,dwGXDNum,dwMaxGXMapObjectsNum;
	dwGXONum = ITGetItemNum(m_pIndexItemTableGXObject);
	dwGXLNum = ITGetItemNum(m_pIndexItemTableGXLight);
	dwGXDNum = ITGetItemNum(m_pIndexItemTableGXDecal);

	dwMaxGXMapObjectsNum = dwGXONum + dwGXLNum + dwGXDNum;

	if (m_dwMaxMapObjectsNum < dwCurGXMapObjectNum)
	{
#ifdef _DEBUG
		PrintfDebugString("Fail to CreateGXMap(), number of GXMapObjects is over than initiali number\n");
		__asm int 3
#endif
		m_dwMaxMapObjectsNum  =  dwCurGXMapObjectNum + 1024;
	}

	m_pgxMap = new CGXMap;
	m_pgxMap->Initialize(this,pProc,m_dwMaxMapObjectsNum);




	// 맵에 넣을 오브젝트를 걸러낸다.
	DWORD	i;
	for (i=0; i<dwGXONum; i++)
	{
		pGXMapObject = (CGXMapObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);
		m_pgxMap->AddDynamicObject(pGXMapObject);
	}
	for (i=0; i<dwGXLNum; i++)
	{

		pGXMapObject = (CGXMapObject*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
		m_pgxMap->AddDynamicObject(pGXMapObject);
	}
	for (i=0; i<dwGXDNum; i++)
	{
		pGXMapObject = (CGXMapObject*)ITGetItemSequential(m_pIndexItemTableGXDecal,i);
		m_pgxMap->AddDynamicObject(pGXMapObject);
	}

	
lb_return:
	return (GXMAP_HANDLE*)m_pgxMap;

}
BOOL __stdcall CoExecutive::InsertHFieldToGXMap(IHeightField* pHField)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_return;

	bResult = m_pgxMap->InsertHField(pHField);
	
lb_return:
	return bResult;
	

}
BOOL __stdcall CoExecutive::InsertStaticModelTOGXMap(I3DStaticModel* pModel)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_return;

	bResult = m_pgxMap->InsertSTM(pModel, m_dwBuildTreeFlag);
	
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::BeginBuildMap(DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_return;

	bResult = m_pgxMap->BeginBuildMap(dwFlag);
	
lb_return:
	return bResult;
}
	
BOOL __stdcall CoExecutive::EndBuildMap(float fTop,float fBottom)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_retrn;

	bResult = m_pgxMap->EndBuildMap(fTop,fBottom);
lb_retrn:
	return bResult;
}
BOOL __stdcall	CoExecutive::RebuildMap(float fTop,float fBottom)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_retrn;

	m_pgxMap->RebuildMap(fTop,fBottom);
	bResult = TRUE;
lb_retrn:
	return bResult;
	
}

BOOL __stdcall CoExecutive::GetWorldBoundingBox( MAABB* pWorldBox)
{
	BOOL	bResult = FALSE;
	if (!m_pgxMap)
		goto lb_return;


	m_pgxMap->GetWorldBoundingBox(pWorldBox);
	bResult = TRUE;

lb_return:
	return bResult;
}

void __stdcall CoExecutive::SetHFieldDetail(DWORD dwDetail)
{
	if (m_pgxMap)
	{
		if (m_pgxMap->GetHField())
		{
			m_pgxMap->GetHField()->SetHeightFieldDetail(dwDetail);
		}
	}
}
CoGXObject*	CoExecutive::AddGXObject(MODEL_HANDLE* pModelHandle,DWORD dwModelNum,GXSchedulePROC pProc,DWORD dwFlag)
{
	CoGXObject*		pObj = NULL;
	DWORD			dwIndex;
	CoGXObject*		pDummy = m_pDummyGXObject;
	
	DWORD			i;

	pObj = (CoGXObject*)LALAlloc(m_pStaticPoolGXObject);
	
	if (!pObj)
		goto lb_return;

#ifdef _DEBUG
	CheckGXObjectsList(pObj);
#endif

	dwIndex = ITAddItem(m_pIndexItemTableGXObject,(void*)pObj);
	
	if (dwIndex == 0xffffffff)
	{
		LALFree(m_pStaticPoolGXObject,pObj);
		pObj = NULL;
		goto lb_return;
	}
	if (dwFlag & GXOBJECT_CREATE_TYPE_OPTIMIZE)
	{
		for (i=0; i<dwModelNum; i++)
		{
			pModelHandle[i].pModel->Optimize();
		}
	}
	pObj->Initialize(this,pModelHandle,dwModelNum,pProc,dwFlag);
	pObj->SetIndex(dwIndex);

	

	


	__asm
	{
		; new 를 사용하지 않으므로 더미 인스턴스를 하나 만들어 함수테이블을 복사한다.
		mov		esi,dword ptr[pDummy]
		mov		edi,dword ptr[pObj]
		movsd		
	}

lb_return:
	return pObj;
}


GXLIGHT_HANDLE CoExecutive::CreateGXLight(LIGHT_DESC* pDesc,GXLightSchedulePROC pProc,void* pData,DWORD dwProjTexIndex,MATERIAL* pMtl,DWORD dwFlag)
{
	CoGXLight*		pLight = NULL;
	CoGXLight*		pDummy = m_pDummyGXLight;
	DWORD			dwIndex;

	pLight = (CoGXLight*)LALAlloc(m_pStaticPoolGXLight);
	


	if (!pLight)
	{
#ifdef _DEBUG
		PrintfDebugString("Fail to CreateGXLight(),Fail to LALAlloc()\n");
		__asm int 3
#endif
		goto lb_return;
	}

	
#ifdef _DEBUG
	CheckGXLightsList(pLight);
#endif

	dwIndex = ITAddItem(m_pIndexItemTableGXLight,(void*)pLight);
	if (dwIndex == 0xffffffff)
	{
		LALFree(m_pStaticPoolGXLight,pLight);
		pLight = NULL;
#ifdef _DEBUG
		PrintfDebugString("Fail to CreateGXLight(),Fail to ITAddItem()\n");
		__asm int 3
#endif
		goto lb_return;
	}
	pDesc->dwProjTexIndex = dwProjTexIndex;
	pLight->Initialize(this,pDesc,pMtl,pProc,dwFlag);
	pLight->SetIndex(dwIndex);
	

	


	__asm
	{
		; new 를 사용하지 않으므로 더미 인스턴스를 하나 만들어 함수테이블을 복사한다.
		mov		esi,dword ptr[pDummy]
		mov		edi,dword ptr[pLight]
		movsd		
	}


	if (m_pgxMap)
		m_pgxMap->AddDynamicObject(pLight);



lb_return:
	return (GXLIGHT_HANDLE)pLight;
}


void __stdcall CoExecutive::SetRenderMode(DWORD dwRenderMode)
{	
	//m_dwRenderMode |= dwRenderMode;
	m_dwRenderMode = dwRenderMode;	
}

void __stdcall CoExecutive::SetPickingMode(DWORD dwPickingMode)
{
	m_dwPickingMode = dwPickingMode;
}

BOOL __stdcall CoExecutive::SetSymbol(SYMBOL_TYPE type,char* szFileName)
{

	BOOL		bResult = FALSE;
	I3DModel*	pModel;

	

	if (0xffffffff == m_pGeometry->LoadModel(&pModel,szFileName,0))
		goto lb_return;

	if (m_pModelList[type])
		m_pModelList[type]->Release();

	m_pModelList[type] = pModel;

	
	bResult = m_pModelList[type]->Initialize(m_pRenderer,0);


lb_return:

	

	return bResult;
}
	GXOBJECT_HANDLE	gxo;
	VECTOR3			pv3IntersectPoint;
	float			fDist;

DWORD __stdcall CoExecutive::GXOGetMultipleObjectWithScreenCoord(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	// 뷰볼륨안에 들어오는 놈만 잡아서 골라낸다.

	

	if (dwMaxDescNum >= MAX_PICK_OBJECT_NUM)
		dwMaxDescNum = MAX_PICK_OBJECT_NUM;

	VECTOR3				v3IntersectPoint;
	float				fDist;
	CGXMapObject**		ppMapObjList;
	DWORD				dwModelIndex;
	DWORD				dwObjIndex;
	DWORD			dwGXOCount = 0;

	if (!m_pgxMap)
		goto lb_return;

	DWORD				dwNum;
	dwNum			=	m_pgxMap->GetRenderTargetBuffer( &ppMapObjList, GX_MAP_OBJECT_TYPE_OBJECT);

	VECTOR3			v3RayPos;
	VECTOR3			v3RayDir;

	m_pGeometry->GetRayWithScreenCoord(&v3RayPos,&v3RayDir,ptCursor,dwViewportIndex);

	DWORD	i;
	for (i=0; i<dwNum; i++)
	{

		if (ppMapObjList[i]->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
			continue;

		if (ppMapObjList[i]->GetPropertyFlag() & GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE)
		{
			
			// 툴에선 무조건 찝을수 있게 하자.
			//if (!(m_dwRenderMode & RENDER_MODE_TOOL))
			//	continue;

			if (!(m_dwPickingMode& PICKING_MODE_TOOL))
			{
				continue;
			}			
		}

		if (ppMapObjList[i]->IsCollisionWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,dwViewportIndex,dwFlag))
		{
			if (dwGXOCount >= dwMaxDescNum)
				goto lb_return;

			pPickDesc[dwGXOCount].gxo = (CoGXObject*)ppMapObjList[i];
			pPickDesc[dwGXOCount].fDist = fDist;
			pPickDesc[dwGXOCount].v3IntersectPoint = v3IntersectPoint;
			pPickDesc[dwGXOCount].dwModelIndex = dwModelIndex;
			pPickDesc[dwGXOCount].dwObjIndex = dwObjIndex;

			dwGXOCount++;
			
		}
	}
lb_return:

	return dwGXOCount;
}
DWORD __stdcall CoExecutive::GXOGetMultipleObjectWithScreenCoordRect(PICK_GXOBJECT_DESC* pPickDesc,DWORD dwMaxDescNum,RECT* pRect,DWORD dwViewportIndex,DWORD dwFlag)
{
    
	VIEW_VOLUME		vv;	
	m_pGeometry->CalcViewVolume(&vv,pRect,dwViewportIndex);


	if (dwMaxDescNum >= MAX_PICK_OBJECT_NUM)
		dwMaxDescNum = MAX_PICK_OBJECT_NUM;


	DWORD				dwNum;
	CGXMapObject**		ppMapObjList;
	DWORD			dwGXOCount = 0;
	if (!m_pgxMap)
		goto lb_return;

	dwNum			=	m_pgxMap->GetRenderTargetBuffer( &ppMapObjList, GX_MAP_OBJECT_TYPE_OBJECT);

	DWORD	i;
	for (i=0; i<dwNum; i++)
	{

		if (ppMapObjList[i]->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
			continue;

		if (ppMapObjList[i]->GetPropertyFlag() & GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE)
		{
			// 툴에선 무조건 찝을수 있게 하자.
			//if (!(m_dwRenderMode & RENDER_MODE_TOOL))
			//	continue;

			if (!(m_dwPickingMode& PICKING_MODE_TOOL))
			{
				continue;
			}
		}
		if (ClipRenderObjectWithViewVolume(&vv,ppMapObjList[i]->GetCollisionMeshObjectDesc(),0))
		{
			if (dwGXOCount >= dwMaxDescNum)
				goto lb_return;

			pPickDesc[dwGXOCount].gxo = (CoGXObject*)ppMapObjList[i];
			pPickDesc[dwGXOCount].fDist = 0;
			pPickDesc[dwGXOCount].v3IntersectPoint.x = 0.0f;
			pPickDesc[dwGXOCount].v3IntersectPoint.y = 0.0f;
			pPickDesc[dwGXOCount].v3IntersectPoint.z = 0.0f;

			
			dwGXOCount++;
			
		}
	}
lb_return:

	return dwGXOCount;
}
BOOL __stdcall CoExecutive::IsCollisionWithScreenCoord(GXOBJECT_HANDLE gxh,VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif

	return ((CGXMapObject*)gxh)->IsCollisionWithScreenCoord(pv3IntersectPoint,pdwModelIndex,pdwObjIndex,pfDist,ptCursor,dwViewportIndex,dwFlag);
	
}

GXOBJECT_HANDLE __stdcall CoExecutive::GXOGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	// 뷰볼륨안에 들어오는 놈만 잡아서 골라낸다.

	float				fMinDist	=	900000.0f;
	VECTOR3				v3IntersectPointResult;
	float				fDistResult = 0.0f;
	CoGXObject*			pObjResult = NULL;
	DWORD				dwObjIndexResult = 0xffffffff;
	DWORD				dwModelIndexResult = 0xffffffff;


	VECTOR3				v3IntersectPoint;
	
	float				fDist = 0.0f;
	DWORD				dwNum = 0;
	CGXMapObject**		ppMapObjList = NULL;
	DWORD				dwObjIndex = 0xffffffff;
	
	DWORD				dwModelIndex = 0xffffffff;

	v3IntersectPoint.x = 0.0f;
	v3IntersectPoint.y = 0.0f;
	v3IntersectPoint.z = 0.0f;

	v3IntersectPointResult.x = 0.0f;
	v3IntersectPointResult.y = 0.0f;
	v3IntersectPointResult.z = 0.0f;


	if (!m_pgxMap)
		goto lb_return;

	dwNum			=	m_pgxMap->GetRenderTargetBuffer( &ppMapObjList, GX_MAP_OBJECT_TYPE_OBJECT);

	VECTOR3			v3RayPos;
	VECTOR3			v3RayDir;

	m_pGeometry->GetRayWithScreenCoord(&v3RayPos,&v3RayDir,ptCursor,dwViewportIndex);

	
//	dwNum = m_Clipper.GetDynamicRenderTargetNum();
//	ppMapObjList = m_Clipper.GetDynamicRenderTargetList();

	DWORD i;
	for (i=0; i<dwNum; i++)
	{

		if (ppMapObjList[i]->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
			continue;

		if (ppMapObjList[i]->GetPropertyFlag() & GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE)
		{
			// 툴에선 무조건 찝을수 있게 하자.
			//if (!(m_dwRenderMode & RENDER_MODE_TOOL))
			//	continue;

			if (!(m_dwPickingMode& PICKING_MODE_TOOL))
			{
				continue;
			}		
		}

		if (ppMapObjList[i]->IsCollisionWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,dwViewportIndex,dwFlag))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pObjResult = (CoGXObject*)ppMapObjList[i];
				v3IntersectPointResult = v3IntersectPoint;
				fDistResult = fDist;
				dwModelIndexResult = dwModelIndex;
				dwObjIndexResult = dwObjIndex;

			}
		}
	}
	*pv3IntersectPoint = v3IntersectPointResult;
	*pfDist = fDistResult;
	*pdwModelIndex = dwModelIndexResult;
	*pdwObjIndex = dwObjIndexResult;


lb_return:
	return pObjResult;
}
GXLIGHT_HANDLE __stdcall CoExecutive::GXLGetLightWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
		// 뷰볼륨안에 들어오는 놈만 잡아서 골라낸다.

	float				fMinDist	=	900000.0f;
	VECTOR3				v3IntersectPointResult;
	float				fDistResult;
	CoGXLight*			pLightResult = NULL;


	VECTOR3				v3IntersectPoint;
	float				fDist;
	DWORD				dwModelIndex;
	DWORD				dwObjIndex;

	CoGXLight*	pGXL;
	DWORD	dwLightNum = ITGetItemNum(m_pIndexItemTableGXLight);
	for (DWORD i=0; i<dwLightNum; i++)
	{
		pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);

		if (pGXL->IsCollisionWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,dwViewportIndex,dwFlag))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pLightResult = pGXL;
				v3IntersectPointResult = v3IntersectPoint;
				fDistResult = fDist;
			}
		}
	}
		
	*pfDist = fDistResult;
	*pv3IntersectPoint = v3IntersectPointResult;
	
	
	return pLightResult;
}
GXTRIGGER_HANDLE __stdcall CoExecutive::GXTGetObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag)
{
	float				fMinDist	=	900000.0f;
	VECTOR3				v3IntersectPointResult;
	float				fDistResult;
	CoGXEventTrigger*	pTriggerResult = NULL;

	DWORD				dwModelIndex;
	DWORD				dwObjIndex;
	VECTOR3				v3IntersectPoint;
	float				fDist;
	
	CoGXEventTrigger*	pGXT;
	DWORD	dwTriggerNum = ITGetItemNum(m_pIndexItemTableGXTrigger);
	for (DWORD i=0; i<dwTriggerNum; i++)
	{
		pGXT = (CoGXEventTrigger*)ITGetItemSequential(m_pIndexItemTableGXTrigger,i);

		if (pGXT->IsCollisionWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,dwViewportIndex,dwFlag))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pTriggerResult = pGXT;
				v3IntersectPointResult = v3IntersectPoint;
				fDistResult = fDist;
			}
		}
	}
		
	*pfDist = fDistResult;
	*pv3IntersectPoint = v3IntersectPointResult;
	
	
	return pTriggerResult;
}


BOOL __stdcall CoExecutive::GXMGetCollisionPointWithScreenCoord(DWORD* pdwObjIndex,VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor,DWORD dwFlag)
{
	float				fMinDist	=	900000.0f;
	VECTOR3				v3IntersectPointResult;
	float				fDistResult;
	DWORD				dwObjIndexResult = 0xffffffff;
	


	VECTOR3				v3IntersectPoint;
	float				fDist;
	DWORD				dwNum;
	CGXMapObject**		ppMapObjList;
	IHeightField*		pHField;
	BOOL				bResult = FALSE;
	DWORD				i;
	DWORD				dwModelIndex;
	DWORD				dwObjIndex;
	
	
	if (!m_pgxMap)
		goto lb_return;

//	dwNum = m_Clipper.GetStaticRenderTargetNum();
//	ppMapObjList = m_Clipper.GetStaticRenderTargetList();

	dwNum			=	m_pgxMap->GetRenderTargetBuffer( &ppMapObjList, GX_MAP_OBJECT_TYPE_STRUCT);

	VECTOR3			v3RayPos;
	VECTOR3			v3RayDir;

	m_pGeometry->GetRayWithScreenCoord(&v3RayPos,&v3RayDir,ptCursor,m_dwViewportIndex);

	for (i=0; i<dwNum; i++)
	{

		if (ppMapObjList[i]->GetObjectType() == GX_MAP_OBJECT_TYPE_HFIELD)
			continue;

		if (ppMapObjList[i]->IsCollisionWithScreenCoord(&v3IntersectPoint,&dwModelIndex,&dwObjIndex,&fDist,ptCursor,m_dwViewportIndex,dwFlag))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				v3IntersectPointResult = v3IntersectPoint;
				fDistResult = fDist;
				dwObjIndexResult = dwObjIndex;
				bResult = TRUE;
				
			}
		}
	}

	if (bResult)
		goto lb_return;


	pHField = m_pgxMap->GetHField();
	if (pHField)
	{
		if (pHField->GetIntersectPointWithScreenCoord(&v3IntersectPointResult,&fDistResult,ptCursor,m_dwViewportIndex))
			bResult = TRUE;
	}
lb_return:
	if (pdwObjIndex)
		*pdwObjIndex = dwObjIndexResult;
	*pv3IntersectPoint = v3IntersectPointResult;
	*pfDist = fDistResult;

	return bResult;
}
BOOL __stdcall CoExecutive::GXMGetHFieldCollisionPointWithRay(VECTOR3* pv3IntersectPoint,float* pfDist,VECTOR3* pv3Orig,VECTOR3* pv3Dir)
{
	BOOL			bResult = FALSE;
	
	IHeightField*	pHField;
	
	if (!m_pgxMap)
		goto lb_return;

	pHField = m_pgxMap->GetHField();
	if (!pHField)
		goto lb_return;

	DWORD	dwX,dwZ;
	
	bResult = pHField->GetIntersectPointWithRay(pv3IntersectPoint,pfDist,&dwX,&dwZ,pv3Orig,pv3Dir);
lb_return:
	return bResult;
}
ULONG __stdcall CoExecutive::GXMGetHField(IHeightField** ppHField)
{
	ULONG	ulResult = 0xffffffff;

	if (!m_pgxMap)
		goto lb_return;

	
	*ppHField = m_pgxMap->GetHField();
	if (!m_pgxMap->GetHField())
		goto lb_return;

	ulResult = 0;
lb_return:
	return ulResult;


}
BOOL __stdcall CoExecutive::GXMGetHFieldHeight(float* py,float x,float z)
{
	BOOL		bResult = FALSE;
	if (!m_pgxMap)
		goto lb_return;
	
	if (!m_pgxMap->GetHField())
		goto lb_return;

	bResult = m_pgxMap->GetHField()->GetY(py,x,z);
		
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::GXMGetHFieldCollisionPointWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,POINT* ptCursor)
{
	IHeightField*		pHField;
	BOOL				bResult = FALSE;
	
	if (!m_pgxMap)
		goto lb_return;

	pHField = m_pgxMap->GetHField();
	if (pHField)
	{
		if (pHField->GetIntersectPointWithScreenCoord(pv3IntersectPoint,pfDist,ptCursor,m_dwViewportIndex))
			bResult = TRUE;
	}
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::GXOInitializeIllusionEffect(GXOBJECT_HANDLE gxh,DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	CoGXObject*	pObj = (CoGXObject*)gxh;

	return pObj->InitializeIllusionEffect(dwMaxIllusionFrameNum,szObjName,pMtlHandle,dwFlag);
}

void __stdcall CoExecutive::GXOBeginIllusionEffect(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	CoGXObject*	pObj = (CoGXObject*)gxh;
	pObj->BeginIllusionEffect();
}
void __stdcall CoExecutive::GXOEndIllusionEffect(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	CoGXObject*	pObj = (CoGXObject*)gxh;
	pObj->EndIllusionEffect();
}





void __stdcall CoExecutive::GXODisableUnloadPreLoaded(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->DisableUnloadPreLoaded();
}

void __stdcall CoExecutive::GXOEnableUnloadPreLoaded(GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->EnableUnloadPreLoaded();
}












DWORD __stdcall CoExecutive::GXOGetAttachedGXObjects(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	CoGXObject*	pObj = (CoGXObject*)gxh;

	return pObj->GetAttachedGXObjects(pGXOList,dwMaxNum);
}



	
BOOL __stdcall CoExecutive::GXOAttachCameraFront(GXOBJECT_HANDLE gxo,float fDist)
{
	BOOL	bResult = FALSE;
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
	CoGXObject*	pObj = (CoGXObject*)gxo;
	if (pObj->GetParentMatrix())
		goto lb_return;
	// 맨 마지막에 임의로 렌더링 할것이므로..
	pObj->DisableRender();
	m_pCameraAttatchedGXObject = pObj;
	m_fDistCameraFront = fDist;
	
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::GXODetachCameraFront(GXOBJECT_HANDLE gxo)
{
	BOOL	bResult = FALSE;
#ifdef	_DEBUG
	CheckHandle(gxo);
	
#endif
	CoGXObject*	pObj = (CoGXObject*)gxo;
	if (m_pCameraAttatchedGXObject != pObj)
		goto lb_return;

	pObj->EnableRender();
	m_pCameraAttatchedGXObject = NULL;

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::GXOAttach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,char* szObjName)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
	CheckHandle(to_gxh);
#endif
	BOOL		bResult = FALSE;

	DWORD	dwToGXObjectIndex = ((CoGXObject*)to_gxh)->GetIndex();
	CoGXObject*	pObj = (CoGXObject*)ITGetItem(m_pIndexItemTableGXObject,dwToGXObjectIndex);
	
	if (!pObj)
		goto lb_return;

	bResult = pObj->RequestAttach((CoGXObject*)gxh,-1,szObjName);
	
lb_return:
	return bResult;
}

BOOL __stdcall CoExecutive::GXOAttachWithModelIndex(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE to_gxh,DWORD dwModelIndex,char* szObjName)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
	CheckHandle(to_gxh);
#endif
	BOOL		bResult = FALSE;

	DWORD	dwToGXObjectIndex = ((CoGXObject*)to_gxh)->GetIndex();
	CoGXObject*	pObj = (CoGXObject*)ITGetItem(m_pIndexItemTableGXObject,dwToGXObjectIndex);
	
	if (!pObj)
		goto lb_return;

	bResult = pObj->RequestAttach((CoGXObject*)gxh,dwModelIndex,szObjName);
	
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::GXODetach(GXOBJECT_HANDLE gxh,GXOBJECT_HANDLE child_gxh)
{
	#ifdef	_DEBUG
		CheckHandle(gxh);
		CheckHandle(child_gxh);
	#endif
	CoGXObject*	pObj = (CoGXObject*)gxh;

	return pObj->RequestDetach((CoGXObject*)child_gxh);
}

DWORD __stdcall CoExecutive::GetFrameCount()
{
	return m_dwFrameCount;
}

BOOL __stdcall CoExecutive::GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc)
{
	BOOL	bResult = FALSE;
	
	if (!m_pgxMap)
		goto lb_return;

	bResult = m_pgxMap->ShadeMap(dwAmbientColor,pLight,dwLightNum,dwFlag,pFunc);
//	m_pRenderer->ResetDevice(TRUE);
lb_return:
	return bResult;

}

void __stdcall CoExecutive::GXMSetHFieldAlphaFlag(DWORD dwAlpha)
{
	if (m_pgxMap)
		m_pgxMap->SetHFieldAlphaFlag(dwAlpha);

}
void __stdcall CoExecutive::GXMSetSTMAlphaFlag(DWORD dwAlpha)
{
	if (m_pgxMap)
		m_pgxMap->SetSTMAlphaFlag(dwAlpha);
}

/*
BOOL __stdcall CoExecutive::GXMStaticShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag)
{
	I3DStaticModel*	pModel;
	IHeightField*	pHField;
	
	CGXMapObject*	pMapObjectShade[8192];
	DWORD			dwObjNumPerLight;
	DWORD			dwTotalFacesNum;
	DWORD			dwExecLightNum;
	
	IVERTEX_SET*	pIVSet;
	VECTOR3*		pv3 = NULL;
	VECTOR3*		pv3Offset = NULL;
	DWORD			i,j,k;
	BOOL			bResult = FALSE;
	BOOL			bNeedDel = FALSE;
	
	CoGXLight*		pGXL;

	if (!m_pgxMap)
		goto lb_return;

	dwExecLightNum = ITGetItemNum(m_pIndexItemTableGXLight);
	if (dwExecLightNum)
	{
		dwLightNum = dwExecLightNum;
		pLight = new LIGHT_DESC[dwLightNum];
		bNeedDel = TRUE;

		for (i=0; i<dwLightNum; i++)
		{
			pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
			if (pGXL->GetPropertyFlag() & GXLIGHT_TYPE_STATIC)
				pGXL->GetLightDesc(pLight+i);
		}
	}

	pModel = m_pgxMap->GetBaseModel();
	pHField = m_pgxMap->GetHField();

	if (pHField)
		pHField->BeignShadeLightMapObject(dwAmbientColor);
	
	if (pModel)
		pModel->BeignShadeLightMapObject(dwAmbientColor);
	
	for (i=0; i<dwLightNum; i++)
	{
		dwTotalFacesNum = 0;
		
		m_Clipper.FindNearObject(pLight[i].v3Point,pLight[i].fRs);
	
		dwObjNumPerLight = m_Clipper.GetMeshObjects(pMapObjectShade,8192);
		if (dwObjNumPerLight > 8192)
			__asm int 3


		if (!dwObjNumPerLight)
			continue;

		pIVSet = new IVERTEX_SET[dwObjNumPerLight];
		memset(pIVSet,0,sizeof(IVERTEX_SET)*dwObjNumPerLight);

		for (j=0; j<dwObjNumPerLight; j++)
		{
			pIVSet[j].pMapObject = pMapObjectShade[j];
			if (dwFlag & STATIC_MODEL_SHADE_TYPE_ENABLE_SHADOW)
			{
				pIVSet[j].dwNum = pIVSet[j].pMapObject->CreateIVertexList(&pIVSet[j].pVertex);
				dwTotalFacesNum += pIVSet[j].dwNum;
			}
		}
		if (dwTotalFacesNum)
		{
			pv3Offset = pv3 = new VECTOR3[dwTotalFacesNum*3];
		}
		for (j=0; j<dwObjNumPerLight; j++)
		{
			for (k=0; k<pIVSet[j].dwNum*3; k++)
			{
				*(VECTOR3*)&pv3Offset->x = *(VECTOR3*)&pIVSet[j].pVertex[k].x;
				pv3Offset++;
			}
			if (pIVSet[j].dwNum)
				pIVSet[j].pMapObject->ReleaseIVertexList(pIVSet[j].pVertex);
		}

		for (j=0; j<dwObjNumPerLight; j++)
		{
			pIVSet[j].pMapObject->ShadeLightMapObject(pv3,dwTotalFacesNum,pLight+i);
		}
		delete [] pv3;
		delete [] pIVSet;

	}
	if (pModel)
		pModel->EndShadeLightMapObject();

	if (pHField)
		pHField->EndShadeLightMapObject();


	if (bNeedDel)
		delete [] pLight;

	bResult = TRUE;

lb_return:
	return bResult;
}
*/



void CoExecutive::ResourceCheck()
{

//	GLOBAL_FUNC_DLL		DWORD				__stdcall	ITGetItemNum(ITEMTABLE_HANDLE pIT);
//	GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
//	GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItemSequential(ITEMTABLE_HANDLE pIT,DWORD dwSeqIndex);
#ifdef _DEBUG
	char	txt[512];
	DWORD	dwNum = 0;
	if (m_pIndexItemTableGXObject)
	{

		dwNum = ITGetItemNum(m_pIndexItemTableGXObject);
		if (dwNum)
		{
			wsprintf(txt,"\n\n\n%s\n","GXObject Not Released!!! Look the VC++'s Output Window");
			OutputDebugString(txt);
			for (DWORD i=0; i<dwNum; i++)
			{
				CoGXObject*	pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);
				wsprintf(txt,"Handle:%x,Relative Resource : %s \n",(DWORD)pGXO,pGXO->m_pModelFileItem->szFileName);
				OutputDebugString(txt);
			}
			memset(txt,0,512);
			wsprintf(txt,"CoExecutive::ResourceCheck(), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,txt);
		}
	}

	if (m_pIndexItemTableGXLight)
	{

		dwNum = ITGetItemNum(m_pIndexItemTableGXLight);
		if (dwNum)
		{
			wsprintf(txt,"\n\n\n%s\n","GXLight Not Released!!! Look the VC++'s Output Window");
			OutputDebugString(txt);
			for (DWORD i=0; i<dwNum; i++)
			{
				CoGXLight*	pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
				wsprintf(txt,"Handle:%x\n",(DWORD)pGXL);
				OutputDebugString(txt);
			}
			memset(txt,0,512);
			wsprintf(txt,"CoExecutive::ResourceCheck(), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,txt);
		}
	}
	if (m_pIndexItemTableGXTrigger)
	{

		dwNum = ITGetItemNum(m_pIndexItemTableGXTrigger);
		if (dwNum)
		{
			wsprintf(txt,"\n\n\n%s\n","GXTrigger Not Released!!! Look the VC++'s Output Window");
			OutputDebugString(txt);
			for (DWORD i=0; i<dwNum; i++)
			{
				CoGXEventTrigger*	pGXT = (CoGXEventTrigger*)ITGetItemSequential(m_pIndexItemTableGXTrigger,i);
				wsprintf(txt,"Handle:%x\n",(DWORD)pGXT);
				OutputDebugString(txt);
			}
			memset(txt,0,512);
			wsprintf(txt,"CoExecutive::ResourceCheck(), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,txt);
		}
	}
	if (m_pIndexItemTableGXDecal)
	{

		dwNum = ITGetItemNum(m_pIndexItemTableGXDecal);
		if (dwNum)
		{
			wsprintf(txt,"\n\n\n%s\n","GXDecal Not Released!!! Look the VC++'s Output Window");
			OutputDebugString(txt);
			for (DWORD i=0; i<dwNum; i++)
			{
				CGXDecal*	pGXD = (CGXDecal*)ITGetItemSequential(m_pIndexItemTableGXDecal,i);
				wsprintf(txt,"Handle:%x\n",(DWORD)pGXD);
				OutputDebugString(txt);

			}
			memset(txt,0,512);
			wsprintf(txt,"CoExecutive::ResourceCheck(), File:%s , Line:%d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_RESOURCE_LEAK,0,(void*)dwAddr,txt);
		}
	}
#endif 
}

CoExecutive::~CoExecutive()
{
	PrintfDebugString("CoExecutive::~CoExecutive()\n");
	ResourceCheck();
	
	if (m_pDPCQ)
	{
		DPCQProcess(m_pDPCQ);			// dpcq에 들어있던거 한방에 처리.
		DPCQClear(m_pDPCQ);
		DPCQRelease(m_pDPCQ);

		m_pDPCQ = NULL;
	}
	DeleteAllGXLights();
	DeleteAllGXObjects();
	DeleteAllGXEventTriggers();

	if (m_pgxMap)
	{
		DeleteGXMap(m_pgxMap);
		m_pgxMap = NULL;
	}


	for (DWORD i=0; i<SYMBOL_TYPE_NUM; i++)
	{
		if (m_pModelList[i])
		{
			m_pModelList[i]->Release();
			m_pModelList[i] = NULL;
		}

	}
	if (m_pStaticPoolGXObject)
	{
		ReleaseStaticMemoryPool(m_pStaticPoolGXObject);
		m_pStaticPoolGXObject = NULL;
	}
	if (m_pStaticPoolGXLight)
	{
		ReleaseStaticMemoryPool(m_pStaticPoolGXLight);
		m_pStaticPoolGXLight = NULL;

	}
	if (m_pStaticPoolGXDecal)
	{
		ReleaseStaticMemoryPool(m_pStaticPoolGXDecal);
		m_pStaticPoolGXDecal = NULL;

	}
	if (m_pFileNameHash)
	{
		VBHRelease(m_pFileNameHash);
		m_pFileNameHash = NULL;
	}

	
	if (m_pRenderer)
	{
		m_pRenderer->Release();
		m_pRenderer = NULL;
	}
	if (m_pGeometry)
	{
		m_pGeometry->Release();
		m_pGeometry = NULL;
	}


	if (m_ppGXLightList)
	{
		delete [] m_ppGXLightList;
		m_ppGXLightList = NULL;
	}
	if (m_pIDHash)
	{
		QBHRelease(m_pIDHash);
		m_pIDHash = NULL;
	}

	if (m_pIndexItemTableGXObject)
	{
		ITRelease(m_pIndexItemTableGXObject);
		m_pIndexItemTableGXObject = NULL;
	}
	if (m_pIndexItemTableGXLight)
	{
		ITRelease(m_pIndexItemTableGXLight);
		m_pIndexItemTableGXLight = NULL;
	}

	if (m_pIndexItemTableGXTrigger)
	{
		ITRelease(m_pIndexItemTableGXTrigger);
		m_pIndexItemTableGXTrigger = NULL;
	}
	if (m_pIndexItemTableGXDecal)
	{
		ITRelease(m_pIndexItemTableGXDecal);
		m_pIndexItemTableGXDecal = NULL;
	}
	delete m_pDummyGXObject;
	delete m_pDummyGXLight;
	delete m_pDummyGXDecal;

	if (m_hGeometry)
	{
		FreeLibrary(m_hGeometry);
		m_hGeometry = NULL;
	}
	if (m_hRenderer)
	{
		FreeLibrary(m_hRenderer);
		m_hRenderer = NULL;
	}

	for (i=0; i<m_dwPackFilesNum; i++)
	{
		if (m_pPackFileHandleList[i])
		{
			m_pFileStorage->UnmapPackFile(m_pPackFileHandleList[i]);
			m_pPackFileHandleList[i] = NULL;
		}
	}

	if (m_pFileStorage)
	{
		m_pFileStorage->Release();
		m_pFileStorage = NULL;
	}
	if (m_hStorage)
	{
		FreeLibrary(m_hStorage);
		m_hStorage = NULL;
	}
	
//	m_Collision.DeleteAll();		// 2002/05/29

	


#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif	

	InterlockedDecrement( &g_lComponent);
}

/*
CLIPPER_CANDIDATE_BUFF* __stdcall CoExecutive::FindNearObject( BOUNDING_SPHERE* pBS)
{
	m_Clipper.FindNearObject( pBS->v3Point, pBS->fRs);
	return	m_Clipper.GetResult();
}
*/
void __stdcall	CoExecutive::GXOSetBoundingVolume( GXOBJECT_HANDLE gxh, BOUNDING_VOLUME* pBV)
{
	#ifdef	_DEBUG
		CheckHandle(gxh);
	#endif
	((CoGXObject*)gxh)->SetBoundingVolume( pBV);
}


void __stdcall	CoExecutive::GXOMovePositionWithCollide(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos)		// 2002/05/22
{
	((CoGXObject*)gxh)->MovePositionWithCollide(pv3Pos);
}


DWORD __stdcall CoExecutive::GetGXObjectsNum()
{
	return ITGetItemNum(m_pIndexItemTableGXObject);
}
GXOBJECT_HANDLE __stdcall CoExecutive::GetGXObjectWithSeqIndex(DWORD dwIndex)
{
	return (GXLIGHT_HANDLE)ITGetItemSequential(m_pIndexItemTableGXObject,dwIndex);
}
DWORD __stdcall CoExecutive::GetGXLightsNum()
{
	return ITGetItemNum(m_pIndexItemTableGXLight);
}

GXLIGHT_HANDLE __stdcall CoExecutive::GetGXLightWithSeqIndex(DWORD dwIndex)
{
	return (GXLIGHT_HANDLE)ITGetItemSequential(m_pIndexItemTableGXLight,dwIndex);
}

DWORD __stdcall CoExecutive::GetGXEventTriggersNum()
{
	return ITGetItemNum(m_pIndexItemTableGXTrigger);

}
GXTRIGGER_HANDLE __stdcall CoExecutive::GetGXEventTriggerWithSeqIndex(DWORD dwIndex)
{
	return (GXTRIGGER_HANDLE)ITGetItemSequential(m_pIndexItemTableGXTrigger,dwIndex);
}

BOOL __stdcall CoExecutive::LoadMapScript(char* szFileName,LOAD_CALLBACK_FUNC pFunc,DWORD dwLoadFlag)
{
	PrintfDebugString( "CoExecutive::LoadMapScript( \"%s\")\n", szFileName);

	char	buf[128];

	MAABB	worldBox;
	memset(&worldBox,0,sizeof(worldBox));
	worldBox.Max.y = WORLDMAP_DEFAULT_TOP;
	worldBox.Min.y = WORLDMAP_DEFAULT_BOTTOM;

	if( m_pgxMap)
	{
		PrintfDebugString("CoExecutive::LoadMapScript() 이전 맵이 해제가 안되었는데 LoadMapScript가 다시 호출됬다. (Previous map is not Released!!!)\n");
		_asm int 3;
	}
	DeleteGXMap(m_pgxMap);
	m_pgxMap = (CGXMap*)CreateGXMap(NULL,0,NULL);	

	IHeightField*	pHField = NULL;
	I3DStaticModel*	pStaticModel = NULL;
	
	void*	pFP;
	
	
	DWORD				dwCurReadCount = 0;
	LOAD_CALLBACK_DESC*	pArgCallbackDesc = NULL;
	LOAD_CALLBACK_DESC	callbackDesc;
	
	
	if (pFunc)
	{
		pArgCallbackDesc = &callbackDesc;
		callbackDesc.pFunc = pFunc;
		callbackDesc.dwTotalCount = GetMapReadCount(szFileName);
		callbackDesc.pdwCurCount = &dwCurReadCount;
	}

	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
	if (!pFP)
	{
		// FILE_NOT_FOUND /////////////////////////////////////////////////////////
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_FILE_NOT_FOUND,1,(void*)dwAddr,szFileName);
		///////////////////////////////////////////////////////////////////////////

		return FALSE;
	}
	
#ifdef _DEBUG
	DWORD	dwPrvTick;
#endif

	while (g_pFileStorage->FSScanf(pFP,"%s",buf)!=EOF)
	{
		if( !lstrcmp(buf, PID_GX_METADATA))
		{
			while( lstrcmp( buf, "}"))
			{
				g_pFileStorage->FSScanf(pFP, "%s", buf);
				if( !lstrcmp( buf, PID_BOUNDINGBOX_MAX))
				{
					g_pFileStorage->FSScanf( pFP, "%f %f %f", &worldBox.Max.x, &worldBox.Max.y, &worldBox.Max.z);
				}
				else if( !lstrcmp( buf, PID_BOUNDINGBOX_MIN))
				{
					g_pFileStorage->FSScanf( pFP, "%f %f %f", &worldBox.Min.x, &worldBox.Min.y, &worldBox.Min.z);
				}
			}
		}
		if (!lstrcmp(buf,PID_GX_MAP))
		{
#ifdef _DEBUG
			dwPrvTick = GetTickCount();
#endif
			BeginBuildMap(dwLoadFlag);
			while (lstrcmp(buf,"}"))
			{
				g_pFileStorage->FSScanf(pFP,"%s",buf);
				if (!lstrcmp(buf,PID_STATIC_MODEL))
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					m_pGeometry->CreateStaticModel(&pStaticModel,4000,500,0);
					if (!pStaticModel->ReadFile(buf,pArgCallbackDesc,0))
					{
						pStaticModel->Release();
						pStaticModel = NULL;
					}
				}
				if (!lstrcmp(buf,PID_HEIGHT_FIELD))
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					m_pGeometry->CreateHeightField(&pHField,0);
					if (!pHField->ReadFile(buf,0,pArgCallbackDesc,0))
					{
						pHField->Release();
						pHField = NULL;
					}
				}
			}
			if (pStaticModel)
			{
				InsertStaticModelTOGXMap(pStaticModel);
			}
			if (pHField)
			{
				InsertHFieldToGXMap(pHField);
			}
			EndBuildMap(worldBox.Max.y,worldBox.Min.y);
			
						
			if (pStaticModel)
			{
				if (dwLoadFlag & STATIC_MODEL_LOAD_OPTIMIZE)
					pStaticModel->Optimize();

				pStaticModel->Release();
			}
			if (pHField)
			{
				if (dwLoadFlag & HFIELD_MODEL_LOAD_OPTIMIZE)
					pHField->Optimize();

				pHField->Release();
			}
#ifdef _DEBUG
			PrintfDebugString("Elapsed time for Load MapMesh : %d\n",GetTickCount()-dwPrvTick);
#endif


		}
		
		if (!lstrcmp(buf,PID_GX_OBJECT))
		{
#ifdef _DEBUG
			dwPrvTick = GetTickCount();
#endif

			DWORD	dwGXObjectsNum;
			g_pFileStorage->FSScanf(pFP,"%u",&dwGXObjectsNum);
			m_pgxMap->CreateInitialGXObjectList(dwGXObjectsNum);
			
			PrintfDebugString("CoExecutive::LoadMapScript() dwGXObjectsNum = %d\n", dwGXObjectsNum);

			g_pFileStorage->FSScanf(pFP,"%s",buf);		// {
			for (DWORD i=0; i<dwGXObjectsNum; i++)
			{
				DWORD		dwFlag;
				DWORD		dwID;
				VECTOR3		v3Scale,v3Pos,v3Axis;
				float		fRad;

				// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
				g_pFileStorage->FSScanf(pFP,"%s",buf);		// 파일명

				g_pFileStorage->FSScanf(pFP,"%u",&dwID);		// 고유ID
				g_pFileStorage->FSScanf(pFP,"%f %f %f",&v3Scale.x,&v3Scale.y,&v3Scale.z);
				g_pFileStorage->FSScanf(pFP,"%f %f %f",&v3Pos.x,&v3Pos.y,&v3Pos.z);
				g_pFileStorage->FSScanf(pFP,"%f %f %f %f",&v3Axis.x,&v3Axis.y,&v3Axis.z,&fRad);
				g_pFileStorage->FSScanf(pFP,"%x",&dwFlag);
				
				if (pFunc)
				{
					pFunc(dwCurReadCount,callbackDesc.dwTotalCount,NULL);
					dwCurReadCount++;
				}

				CoGXObject* gxo = (CoGXObject*)CreateGXObject(buf,NULL,NULL,dwFlag);
				if (gxo)
				{
					
					if (dwLoadFlag & LOAD_MAP_FLAG_DEFAULT_PROC_AUTOANIMATION)
					{
						if (!gxo->GetScheduleProc())
						{
							for (DWORD i=0; i<gxo->GetModelNum(); i++)
							{
								if (gxo->GetMotionNum(i))
								{
									gxo->SetScheduleProc(GXDefaultSchedulePROC);
									goto lb_set_proc_exit;
								}
							}
						}
					}
lb_set_proc_exit:
					SetID(gxo,dwID);

					GXOSetScale(gxo,&v3Scale);
					GXOSetPosition(gxo,&v3Pos, FALSE);
					GXOSetDirection(gxo,&v3Axis,fRad);
					m_pgxMap->InsertInitialGXObject(gxo);
				}
#ifdef _DEBUG
				else
				{
					char	txt[512];
					memset(txt,0,512);
					wsprintf(txt,"CoExecutive::LoadMapScript(), Fail to CreateGXObject,File:%s , Line:%d \n",__FILE__,__LINE__);
					DWORD	dwAddr;
					GetEIP(&dwAddr);
					g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
				}
#endif 
			}
			g_pFileStorage->FSScanf(pFP,"%s",buf);		// }
#ifdef _DEBUG
			PrintfDebugString("Elapsed time for Load GXOs : %d\n",GetTickCount()-dwPrvTick);
#endif

		}
		if (!lstrcmp(buf,PID_GX_LIGHT))
		{
			DWORD	dwGXLightsNum;
			g_pFileStorage->FSScanf(pFP,"%u",&dwGXLightsNum);
			m_pgxMap->CreateInitialGXLightList(dwGXLightsNum);

		
			g_pFileStorage->FSScanf(pFP,"%s",buf);		// {
			for (DWORD i=0; i<dwGXLightsNum; i++)
			{
				
				LIGHT_DESC	lightDesc;
				memset(&lightDesc,0,sizeof(LIGHT_DESC));
				DWORD		dwFlag;

				// | RGBA | 위치 |범위(CM) | 플래그 
				g_pFileStorage->FSScanf(pFP,"%x",&lightDesc.dwDiffuse);
				g_pFileStorage->FSScanf(pFP,"%f %f %f %f",&lightDesc.v3Point.x,&lightDesc.v3Point.y,&lightDesc.v3Point.z,&lightDesc.fRs);
				g_pFileStorage->FSScanf(pFP,"%x",&dwFlag);

				dwFlag |= GXLIGHT_TYPE_STATIC;
				CoGXLight*	gxl = (CoGXLight*)CreateGXLight(&lightDesc,NULL,NULL,0xffffffff,NULL,dwFlag);
				if (gxl)
				{
					m_pgxMap->InsertInitialGXLight(gxl);
				}
#ifdef _DEBUG
				else
				{
					char	txt[512];
					memset(txt,0,512);
					wsprintf(txt,"CoExecutive::LoadMapScript(), Fail to CreateGXLight, File:%s , Line:%d \n",__FILE__,__LINE__);
					DWORD	dwAddr;
					GetEIP(&dwAddr);
					g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
				}
#endif

			}
			g_pFileStorage->FSScanf(pFP,"%s",buf);		// }
		}
		
		if (!lstrcmp(buf,PID_GX_TRIGGER))
		{
			DWORD	dwGXTriggerNum;
			g_pFileStorage->FSScanf(pFP,"%u",&dwGXTriggerNum);
			m_pgxMap->CreateInitialGXTriggerList(dwGXTriggerNum);

		
			g_pFileStorage->FSScanf(pFP,"%s",buf);		// {
			for (DWORD i=0; i<dwGXTriggerNum; i++)
			{
				EVENT_TRIGGER_DESC	triggerDesc;
				memset(&triggerDesc,0,sizeof(triggerDesc));
				DWORD		dwFlag;
				DWORD		dwID;

					
				// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
				g_pFileStorage->FSScanf(pFP,"%u",&dwID);
				g_pFileStorage->FSScanf(pFP,"%x",&triggerDesc.dwColor);
				g_pFileStorage->FSScanf(pFP,"%f %f %f",&triggerDesc.v3Pos.x,&triggerDesc.v3Pos.y,&triggerDesc.v3Pos.z);
				g_pFileStorage->FSScanf(pFP,"%f %f %f",&triggerDesc.v3Scale.x,&triggerDesc.v3Scale.y,&triggerDesc.v3Scale.z);
				g_pFileStorage->FSScanf(pFP,"%f %f %f",&triggerDesc.v3Rot.x,&triggerDesc.v3Rot.y,&triggerDesc.v3Rot.z);
				g_pFileStorage->FSScanf(pFP,"%x",&dwFlag);

				CoGXEventTrigger*	gxt = (CoGXEventTrigger*)CreateGXEventTrigger(NULL,NULL,dwFlag);
				if (gxt)
				{

					SetID(gxt,dwID);
					GXTSetPosition(gxt,&triggerDesc.v3Pos);
					GXTSetScale(gxt,&triggerDesc.v3Scale);
					GXTSetRotation(gxt,&triggerDesc.v3Rot);
					m_pgxMap->InsertInitialGXTrigger(gxt);
				}
#ifdef	_DEBUG
				else
				{
					char	txt[512];
					memset(txt,0,512);
					wsprintf(txt,"CoExecutive::LoadMapScript(), Fail to CreateGXEventTrigger, File:%s , Line:%d \n",__FILE__,__LINE__);
					DWORD	dwAddr;
					GetEIP(&dwAddr);
					g_pErrorHandleFunc(ERROR_TYPE_PARAMETER_INVALID,0,(void*)dwAddr,txt);
				}
#endif
			}
			g_pFileStorage->FSScanf(pFP,"%s",buf);		// }
		}

	} 

	g_pFileStorage->FSCloseFile(pFP);
	return TRUE;

}
void __stdcall CoExecutive::DeleteAllGXMapObjectsWitLoadMapScript()
{
	PrintfDebugString("CoExecutive::DeleteAllGXMapObjectsWitLoadMapScript()\n");
	if (m_pgxMap)
	{
		m_pgxMap->DeleteAllInitialObjects();
	}
}
DWORD __stdcall CoExecutive::GXOGetAllObjectsWitLoadMapScript(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum)
{
	
	DWORD	dwResult = 0;
	if (m_pgxMap)
	{
		dwResult = m_pgxMap->GetAllInitialGXObjects(pObjArray,dwMaxNum);
	}
	return dwResult;
}


BOOL __stdcall CoExecutive::GetMapReadCount(char* szFileName)
{

	char	buf[128];

	DWORD	dwCount = 0;
	void*	pFP;

	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_TEXT);
	if (!pFP)
		goto lb_return;
	

	while (g_pFileStorage->FSScanf(pFP,"%s",buf)!=EOF)
	{
		if (!lstrcmp(buf,PID_GX_MAP))
		{
			while (lstrcmp(buf,"}"))
			{
				g_pFileStorage->FSScanf(pFP,"%s",buf);
				if (!lstrcmp(buf,PID_STATIC_MODEL))
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					dwCount += m_pGeometry->GetReadCountInSTM(buf);
					
				}
				if (!lstrcmp(buf,PID_HEIGHT_FIELD))
				{
					g_pFileStorage->FSScanf(pFP,"%s",buf);
					dwCount += m_pGeometry->GetReadCountInHField(buf);
				}
			}
		}
		
		
		if (!lstrcmp(buf,PID_GX_OBJECT))
		{
			DWORD	dwGXObjectsNum;
			g_pFileStorage->FSScanf(pFP,"%u",&dwGXObjectsNum);
			dwCount += dwGXObjectsNum;
		}
	} 

	g_pFileStorage->FSCloseFile(pFP);

lb_return:
	return dwCount;
}


BOOL __stdcall CoExecutive::SetCameraFitGXObject(GXOBJECT_HANDLE gxo,float fNear,float fFar,float fFov,DWORD dwViewportIndex)
{
	CoGXObject*	pGXO = (CoGXObject*)gxo;
	m_pGeometry->SetCameraFitCollisionMesh(pGXO->GetCollisionMeshObjectDesc(),fNear,fFar,fFov,dwViewportIndex);
	return TRUE;
}

DWORD __stdcall CoExecutive::Run(DWORD dwBackColor,GX_FUNC pfBeforeRenderFunc,GX_FUNC pfAfterRenderFunc,DWORD dwFlag)
{
	DWORD		dwResult;

	dwResult = Process();

	m_pGeometry->BeginRender(m_dwViewportIndex,dwBackColor,dwFlag | BEGIN_RENDER_FLAG_USE_RENDER_TEXTURE);
	if (pfBeforeRenderFunc)
		pfBeforeRenderFunc();

	Render();

	if (pfAfterRenderFunc)
		pfAfterRenderFunc();

	RenderCameraFrontObject(m_fDistCameraFront);
	m_pGeometry->EndRender();

lb_return:
	return dwResult;
}
BOOL __stdcall CoExecutive::RenderCameraFrontObject(float fDist)
{
	BOOL	bResult = FALSE;
	if (!m_pCameraAttatchedGXObject)
		goto lb_return;

	MATRIX4	matCam;
	m_pGeometry->GetCameraAttachedMatrix(&matCam,m_dwViewportIndex,fDist);
//	m_pRenderer->DisableZBuffer();
	m_pCameraAttatchedGXObject->RenderByParentDirectModel(&matCam);
//	m_pRenderer->EnableZBuffer();
	bResult = TRUE;


lb_return:
	return bResult;
}

DWORD __stdcall CoExecutive::Process()
{
	DWORD		i;

	CoGXObject*	pGXO;
	CoGXLight*	pGXL;
	CGXDecal*	pGXD;

	DWORD		dwGXONum;
	DWORD		dwGXLNum;
	DWORD		dwGXDNum;
	DWORD		dwResult = 0;

	
	int			frame_inc;
	DWORD		dwCurrentTick = GetTickCount();
	BOOL		bGameFrame;

	
	
	/*
	frame_inc = (dwCurrentTick - m_dwPrvTick) / m_dwTicksPerFrame;
	if (!frame_inc)
		goto lb_return;
*/

	// 랜더링 프레임 틱 증가분.
	m_dwTickIncrease	=	(dwCurrentTick - m_dwInitialTick) % m_dwTicksPerFrame;

	char inc[30];
	sprintf(inc,"tick藤좆 %d",m_dwTickIncrease );
	logFile<<inc<<"    ";

	memset(inc,0,30);
	if( m_dwTickIncrease >= 2)
	{
//		_asm int 3;
	}

	frame_inc	=	0;
	DWORD	dwCurFrameCount = (dwCurrentTick - m_dwInitialTick) / m_dwTicksPerFrame;
	if (dwCurFrameCount <m_dwPrvFrameCount)
	{
		bGameFrame = FALSE;
		logFile<<"藁놔琦뻔"<<endl;
		goto lb_return;
	}



	if (m_pgxMap)
	{
		I3DStaticModel* pStaticModel;
		pStaticModel = m_pgxMap->GetBaseModel();
		if (pStaticModel)
			pStaticModel->RunManageResource(m_dwViewportIndex);
	}

	
	

	bGameFrame = TRUE;
	
	frame_inc = dwCurFrameCount - m_dwPrvFrameCount;


	char inc1[30];
	sprintf(inc1,"踏狗煉좆 %d",frame_inc);
	logFile<<inc1<<endl;
	m_dwPrvFrameCount = dwCurFrameCount;
	m_dwFrameCount += frame_inc;


	SetCannotDelete();
	dwGXONum = ITGetItemNum(m_pIndexItemTableGXObject);
	for (i=0; i<dwGXONum; i++)
	{
		pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);
		if (!(pGXO->GetScheduleFlag() & SCHEDULE_FLAG_NOT_SCHEDULE))
		{
			pGXO->OnFrame(this,0,frame_inc,0);
		}
			
	}
	
	
	dwGXLNum = ITGetItemNum(m_pIndexItemTableGXLight);
	for (i=0; i<dwGXLNum; i++)
	{
		pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
			if (!(pGXL->GetScheduleFlag() & SCHEDULE_FLAG_NOT_SCHEDULE))
				pGXL->OnFrame(this,0,frame_inc,0);
	}
	
	dwGXDNum = ITGetItemNum(m_pIndexItemTableGXDecal);
	for (i=0; i<dwGXDNum; i++)
	{
		pGXD = (CGXDecal*)ITGetItemSequential(m_pIndexItemTableGXDecal,i);
			if (!(pGXD->GetScheduleFlag() & SCHEDULE_FLAG_NOT_SCHEDULE))
			pGXD->OnFrame(this,0,frame_inc,0);
	}
	SetCanDelete();
//	bResult = TRUE;
	DPCQProcess(m_pDPCQ);			// dpcq에 들어있던거 한방에 처리.
	DPCQClear(m_pDPCQ);
//	m_pRenderer->UpdateProcess();

lb_return:	
	m_pGeometry->SetTickCount(dwCurrentTick, bGameFrame);
	return frame_inc;
}



DWORD __stdcall CoExecutive::GXOProcess(GXOBJECT_HANDLE gxo,DWORD dwFrameInc)
{
	#ifdef	_DEBUG
		CheckHandle(gxo);
	#endif
	return ((CoGXObject*)gxo)->OnFrame(this,0,dwFrameInc,0);

}
BOOL __stdcall CoExecutive::GXORender(GXOBJECT_HANDLE gxo)
{
	#ifdef	_DEBUG
		CheckHandle(gxo);
	#endif
	
	return ((CoGXObject*)gxo)->Render();
}

DWORD __stdcall CoExecutive::IsValidHandle(GXMAP_OBJECT_HANDLE gxh)
{
	// 나중에 해쉬로 바꾸도록 하자.
	CoGXObject*			pGXO;
	CoGXLight*			pGXL;
	CoGXEventTrigger*	pTrigger;
	CGXDecal*			pDecal;

	DWORD			dwResult = GX_MAP_OBJECT_TYPE_INVALID;
	DWORD			i;	

	DWORD dwNum = ITGetItemNum(m_pIndexItemTableGXObject);
	for (i=0; i<dwNum; i++)
	{
		pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);
		if (gxh == pGXO)
		{
			dwResult = GX_MAP_OBJECT_TYPE_OBJECT;
			goto lb_return;
		}
	}
	dwNum = ITGetItemNum(m_pIndexItemTableGXLight);
	for (i=0; i<dwNum; i++)
	{
		pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
		if (gxh == pGXL)
		{
			dwResult = GX_MAP_OBJECT_TYPE_LIGHT;
			goto lb_return;
		}
	}
	
	dwNum = ITGetItemNum(m_pIndexItemTableGXTrigger);
	for (i=0; i<dwNum; i++)
	{
		pTrigger = (CoGXEventTrigger*)ITGetItemSequential(m_pIndexItemTableGXTrigger,i);
		if (gxh == pTrigger)
		{
			dwResult = GX_MAP_OBJECT_TYPE_TRIGGER;
			goto lb_return;
		}
	}
	dwNum = ITGetItemNum(m_pIndexItemTableGXDecal);
	for (i=0; i<dwNum; i++)
	{
		pDecal = (CGXDecal*)ITGetItemSequential(m_pIndexItemTableGXDecal,i);
		if (gxh == pDecal)
		{
			dwResult = GX_MAP_OBJECT_TYPE_DECAL;
			goto lb_return;
		}
	}
lb_return:
	return dwResult;
}
void __stdcall CoExecutive::Render()
{
	m_dwRenderFrameCount++;

	DWORD				i;
	CGXMapObject**		ppGXMapObjList;
	int					iObjNum;

	CoGXLight*			pGXL;
	LIGHT_DESC			lightDesc;
	BOUNDING_SPHERE		lightSphere;
	DWORD				dwLightNum;

	VIEW_VOLUME			vv;

	// 위치보간용 시간.
	DWORD		dwTickIncrease		=	m_dwTickIncrease;
	DWORD		dwTickPerFrame		=	m_dwTicksPerFrame;
	float		fTimeScale	=	float( dwTickIncrease)	/	float(dwTickPerFrame);
	if( fTimeScale > 1.0f || fTimeScale < 0.0f)
		_asm int 3;


	

	if (!m_pgxMap)
	{
		dwLightNum = ITGetItemNum(m_pIndexItemTableGXLight);
		for (i=0; i<dwLightNum; i++)
		{
			pGXL = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,i);
			if (!pGXL->IsEnableDynamicLight())
				continue;

			if (pGXL->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
				continue;


			pGXL->GetLightDesc(&lightDesc);
			lightSphere.fRs = lightDesc.fRs;
			lightSphere.v3Point = lightDesc.v3Point;
			
			if (-1 != WhereIsSphereFromViewVolume(lightSphere,vv))
			{	
				// 적용받는 라이트를 골라서 오브젝트에 세팅해야한다...
				//	int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv);
				// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
				m_pGeometry->AddLight(&lightDesc,pGXL->GetPropertyFlag());
			}
		}

		DWORD dwGXONum = ITGetItemNum(m_pIndexItemTableGXObject);

		// 랜더링 위치보간.
		for (i=0; i<dwGXONum; i++)
		{
			CoGXObject* pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);		

			pGXO->SetRenderFrameCount(m_dwRenderFrameCount);
			if(pGXO->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
				continue;

			pGXO->InterpolatePosition(fTimeScale);
		}


		for (i=0; i<dwGXONum; i++)
		{
			CoGXObject* pGXO = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,i);		

			pGXO->SetRenderFrameCount(m_dwRenderFrameCount);
			if(pGXO->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
				continue;

			pGXO->Render();
		}
		goto lb_return;
	}
	
	m_pGeometry->GetViewVolume(&vv,m_dwViewportIndex);
/*	unsigned int	iHigh, iLow;
	_asm 
	{
		rdtsc
		mov dword ptr[iHigh],edx
		mov dword ptr[iLow],eax
	}
*/
	m_pgxMap->FindRenderCandidate( vv);
/*
	_asm 
	{
		rdtsc
		sub		eax,dword ptr[iLow]
		sub		edx,dword ptr[iHigh]

	}*/

//	#define GX_MAP_OBJECT_RENDER_START_INDEX	1
//	#define GX_MAP_OBJECT_RENDER_END_INDEX		5

	// 먼저 라이트를 골라내서 geometry에 넣는다.
	iObjNum		=	m_pgxMap->GetRenderTargetBuffer(&ppGXMapObjList,GX_MAP_OBJECT_TYPE_LIGHT);

	DWORD	dwIncLightNum;
	dwIncLightNum = 0;
	for (i=0; i<iObjNum; i++)
	{
		pGXL = (CoGXLight*)ppGXMapObjList[i];
		if (pGXL->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
			continue;

		if (!pGXL->IsEnableDynamicLight())
			continue;

		pGXL->GetLightDesc(&lightDesc);
		lightSphere.fRs = lightDesc.fRs;
		lightSphere.v3Point = lightDesc.v3Point;
		
		if (-1 != WhereIsSphereFromViewVolume(lightSphere,vv))
		{	
			// 적용받는 라이트를 골라서 오브젝트에 세팅해야한다...
			//	int			WhereIsSphereFromViewVolume( BOUNDING_SPHERE& Sphere, VIEW_VOLUME& vv);
			// 0: 걸친다. 1: 볼륨 안에 완전히 들어간다. -1: 볼륨에서 완전히 벗어난다.
			m_pGeometry->AddLight(&lightDesc,pGXL->GetPropertyFlag());
			dwIncLightNum++;

			
			// 지오메트리에 넣었음을 해당 라이트에 표시
			/////////
			// 

		}
	}


	int	j;
	_asm nop;

	// 랜더링 위치 보간.
	iObjNum		=	m_pgxMap->GetRenderTargetBuffer(&ppGXMapObjList, GX_MAP_OBJECT_TYPE_OBJECT);
	for( j = 0; j < iObjNum; j++)
	{
//		ppGXMapObjList[j]->SetRenderFrameCount(m_dwRenderFrameCount);
		if( ppGXMapObjList[j]->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
			continue;
		
		((CoGXObject*)(ppGXMapObjList[j]))->InterpolatePosition(fTimeScale);
	}

	// 위치보간된 상태에서 중간 콜백.
	if(m_pfAfterInterpolationCallBack)
	{
		AFTER_INTERPOLATION_CALL_BACK_ARG	CallbackArg;
		CallbackArg.dwIncreasedTick	=	m_dwTickIncrease;
		CallbackArg.dwTickPerFrame	=	m_dwTicksPerFrame;
		m_pfAfterInterpolationCallBack( &CallbackArg);
	}

	// 게임에서 렌더링할 오브젝트를 골라내서 렌더링한다.
//	int		j;
	for( i = GX_MAP_OBJECT_RENDER_START_INDEX; i <= GX_MAP_OBJECT_RENDER_END_INDEX; i++)
	{
		iObjNum		=	m_pgxMap->GetRenderTargetBuffer(&ppGXMapObjList, i);

//		if( i == GX_MAP_OBJECT_TYPE_DECAL)		// 데칼은 화면에 그리는 문제때문에 제일 나중에 랜더링 한다. (지금은 run()에 가있다.)
//			continue;

		for( j = 0; j < iObjNum; j++)
		{
			
			ppGXMapObjList[j]->SetRenderFrameCount(m_dwRenderFrameCount);
			if( ppGXMapObjList[j]->GetScheduleFlag() & SCHEDULE_FLAG_NOT_RENDER)
				continue;
			
			ppGXMapObjList[j]->Render();
		}
	}

	// 툴에서 렌더링할 개체들..
	if (m_dwRenderMode & RENDER_MODE_TOOL)
	{
		for( i = GX_MAP_OBJECT_RENDER_START_INDEX_TOOL; i <= GX_MAP_OBJECT_RENDER_END_INDEX_TOOL; i++)
		{
			iObjNum		=	m_pgxMap->GetRenderTargetBuffer(&ppGXMapObjList, i);



			for( j = 0; j < iObjNum; j++)
			{
				if (ppGXMapObjList[j]->GetPropertyFlag() & GXLIGHT_TYPE_DISABLE_NOT_RENDER_MODEL_IN_TOOL)
					continue;

				ppGXMapObjList[j]->SetRenderFrameCount(m_dwRenderFrameCount);
				ppGXMapObjList[j]->Render();
			}
		}
	}

	//
/*
	// 위치보간된 상태에서 중간 콜백.
	if(m_pfAfterInterpolationCallBack)
	{
		AFTER_INTERPOLATION_CALL_BACK_ARG	CallbackArg;
		CallbackArg.dwIncreasedTick	=	m_dwTickIncrease;
		CallbackArg.dwTickPerFrame	=	m_dwTicksPerFrame;
		m_pfAfterInterpolationCallBack( &CallbackArg);
	}
*/
lb_return:
	return;

}


BOOL __stdcall CoExecutive::DeleteGXObject(GXOBJECT_HANDLE gxh)
{
	#ifdef	_DEBUG
		CheckHandle(gxh);
	#endif

	BOOL	bResult;
	if (IsCanDeleteObject())
	{
		bResult = ImmDeleteGXObject(gxh);
	}
	else 
	{
		bResult = DefDeleteGXObject(gxh);
	}
	return bResult;

}
BOOL __stdcall CoExecutive::DeleteGXLight(GXLIGHT_HANDLE gxh)
{
	#ifdef	_DEBUG
		CheckHandle(gxh);
	#endif

	BOOL	bResult;
	if (IsCanDeleteObject())
	{
		bResult = ImmDeleteGXLight(gxh);
	}
	else 
	{
		bResult = DefDeleteGXLight(gxh);
	}
	return bResult;
}
BOOL __stdcall CoExecutive::DeleteGXEventTrigger(GXTRIGGER_HANDLE gxh)
{
	#ifdef	_DEBUG
		CheckHandle(gxh);
	#endif

	BOOL	bResult;
	if (IsCanDeleteObject())
	{
		bResult = ImmDeleteGXEventTrigger(gxh);
	}
	else 
	{
		bResult = DefDeleteGXEventTrigger(gxh);
	}
	return bResult;
}

BOOL __stdcall CoExecutive::DeleteGXDecal(GXLIGHT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif

	BOOL	bResult;
	if (IsCanDeleteObject())
	{
		bResult = ImmDeleteGXDecal(gxh);
	}
	else 
	{
		bResult = DefDeleteGXDecal(gxh);
	}
	return bResult;
}
BOOL __stdcall CoExecutive::DefDeleteGXObject(GXOBJECT_HANDLE gxh)
{
	BOOL	bResult;
	DWORD	argList[2];
	argList[0] = (DWORD)this;
	argList[1] = (DWORD)gxh;
	bResult =  DPCQPushDPC(m_pDPCQ,Def_DeleteGXObject,2,argList,NULL,0);
#ifdef	_DEBUG
	if (!bResult)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail to DeleteGXObject,DPCQ Overflow : %x\n",gxh);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);

	}
#endif
	return bResult;
}
BOOL __stdcall CoExecutive::DefDeleteGXLight(GXLIGHT_HANDLE gxh)
{
	BOOL	bResult;
	DWORD	argList[2];
	argList[0] = (DWORD)this;
	argList[1] = (DWORD)gxh;
	bResult = DPCQPushDPC(m_pDPCQ,Def_DeleteGXLight,2,argList,NULL,0);
#ifdef	_DEBUG
	if (!bResult)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail to DeleteGXLight,DPCQ Overflow : %x\n",gxh);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	return bResult;
}
BOOL __stdcall CoExecutive::DefDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh)
{
	BOOL	bResult;
	DWORD	argList[2];
	argList[0] = (DWORD)this;
	argList[1] = (DWORD)gxh;
	bResult = DPCQPushDPC(m_pDPCQ,Def_DeleteGXEventTrigger,2,argList,NULL,0);
#ifdef	_DEBUG
	if (!bResult)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail to DeleteGXEventTrigger,DPCQ Overflow : %x\n",gxh);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// DPC에 의해서 나중에 호출될 함수들..일단 delete함수들이 해당된다.
BOOL __stdcall CoExecutive::DeleteAllGXObjects()
{

	BOOL	bResult = FALSE;
	CoGXObject*	pObj;
	DWORD	dwNum;
	DWORD	i;

	if (!m_pIndexItemTableGXObject)
		goto lb_return;

	dwNum = ITGetItemNum(m_pIndexItemTableGXObject);
	PrintfDebugString("CoExecutive::DeleteAllGXObjects() GXObject Count = %d\n", dwNum);

	for (i=0; i<dwNum; i++)
	{
		pObj = (CoGXObject*)ITGetItemSequential(m_pIndexItemTableGXObject,0);
#ifdef _DEBUG
		CheckHandle(pObj);
#endif
		ITDeleteItem(m_pIndexItemTableGXObject,pObj->GetIndex());
		pObj->Release();
		LALFree(m_pStaticPoolGXObject,pObj);
	}
		
	VIEW_VOLUME			vv;
	m_pGeometry->GetViewVolume(&vv,m_dwViewportIndex);
	if (m_pgxMap)
		m_pgxMap->FindRenderCandidate(vv);

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL __stdcall CoExecutive::DeleteAllGXLights()
{
	BOOL	bResult = FALSE;
	CoGXLight*	pLight;
	if (!m_pIndexItemTableGXLight)
		goto lb_return;

	DWORD	dwLightNum;
	DWORD	i;

	dwLightNum = ITGetItemNum(m_pIndexItemTableGXLight);
	PrintfDebugString("CoExecutive::DeleteAllGXLights() GXLight Count = %d\n", dwLightNum);

	for (i=0; i<dwLightNum; i++)
	{
		pLight = (CoGXLight*)ITGetItemSequential(m_pIndexItemTableGXLight,0);
#ifdef _DEBUG
		CheckHandle(pLight);
#endif
		ITDeleteItem(m_pIndexItemTableGXLight,pLight->GetIndex());
		pLight->Release();
		LALFree(m_pStaticPoolGXLight,pLight);
	}
	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::DeleteAllGXEventTriggers()
{
	BOOL	bResult = FALSE;

	if (!m_pIndexItemTableGXTrigger)
		goto lb_return;

	DWORD	dwTriggerNum;

	dwTriggerNum = ITGetItemNum(m_pIndexItemTableGXTrigger);
	PrintfDebugString("CoExecutive::DeleteAllGXEventTriggers() GXEventTrigger Count = %d\n", dwTriggerNum);

	CoGXEventTrigger*		pTrigger;

	DWORD	i;
	for (i=0; i<dwTriggerNum; i++)
	{
		pTrigger = (CoGXEventTrigger*)ITGetItemSequential(m_pIndexItemTableGXTrigger,0);

#ifdef _DEBUG
		CheckHandle(pTrigger);
#endif
		ITDeleteItem(m_pIndexItemTableGXTrigger,pTrigger->GetIndex());
		pTrigger->Release();
		delete pTrigger;
	}
	
	VIEW_VOLUME			vv;
	m_pGeometry->GetViewVolume(&vv,m_dwViewportIndex);
	if (m_pgxMap)
		m_pgxMap->FindRenderCandidate( vv);
	
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::ImmDeleteGXObject(GXOBJECT_HANDLE gxh)
{

	BOOL	bResult = FALSE;
	
	DWORD	dwObjectIndex;
	CoGXObject*	pObj;

	if (!gxh)
		goto lb_return;


	dwObjectIndex = ((CoGXObject*)gxh)->GetIndex();
	pObj = (CoGXObject*)ITGetItem(m_pIndexItemTableGXObject,dwObjectIndex);

	if (!pObj)
		goto lb_return;

	pObj->Release();
	ITDeleteItem(m_pIndexItemTableGXObject,dwObjectIndex);


#ifdef	_DEBUG
	memset( pObj, 0xdd, sizeof(CoGXObject));
#endif


	LALFree(m_pStaticPoolGXObject,pObj);
	bResult = TRUE;

	VIEW_VOLUME			vv;
	m_pGeometry->GetViewVolume(&vv,m_dwViewportIndex);
	if (m_pgxMap)
		m_pgxMap->FindRenderCandidate( vv);

lb_return:
	return bResult;

}
BOOL __stdcall CoExecutive::ImmDeleteGXLight(GXLIGHT_HANDLE gxh)
{	

	BOOL	bResult = FALSE;
	DWORD	dwItemIndex = ((CoGXLight*)gxh)->GetIndex();

	DWORD	dwNum = ITGetItemNum(m_pIndexItemTableGXLight);
	CoGXLight*	pLight = (CoGXLight*)ITGetItem(m_pIndexItemTableGXLight,dwItemIndex);
	if (!pLight)
	{
		
		__asm int 3
		goto lb_return;
	}
	
	pLight->Release();
	ITDeleteItem(m_pIndexItemTableGXLight,dwItemIndex);
	LALFree(m_pStaticPoolGXLight,pLight);

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::ImmDeleteGXEventTrigger(GXTRIGGER_HANDLE gxh)
{
	BOOL	bResult = FALSE;
	DWORD	dwItemIndex = ((CoGXEventTrigger*)gxh)->GetIndex();

	CoGXEventTrigger*	pTrigger = (CoGXEventTrigger*)ITGetItem(m_pIndexItemTableGXTrigger,dwItemIndex);
	if (!pTrigger)
		goto lb_return;

	pTrigger->Release();
	ITDeleteItem(m_pIndexItemTableGXTrigger,dwItemIndex);
	delete pTrigger;

	bResult = TRUE;
lb_return:
	return bResult;
	
}


BOOL __stdcall CoExecutive::DeleteGXMap(GXMAP_HANDLE gxm)
{
	if (m_pgxMap)
	{
		delete m_pgxMap;
		m_pgxMap = NULL;
	}
	PrintfDebugString( "CoExecutive::DeleteGXMap\n");
	
	// 오브젝트들을 몽땅 다 지우면 아이디 해쉬테이블도 자동으로 다 지워질것이다.
//	DeleteAllGXObjects();
//	DeleteAllGXLights();
//	DeleteAllGXEventTriggers();
	


	return TRUE;
}



/*
DWORD CoExecutive::GetGXLightList(GXOBJECT_HANDLE gxh,LIGHT_DESC* pRelatedLight,DWORD dwMaxLightNum)
{
	// 여기선 인자로 받은 gxobject에 영향을 미칠 수 있는 라이트를 찾아준다.
	// 뷰포트 차원에서 라이트를 클립해서 넣어줘야 할 것이다.
	// 일단은 현재 존재하는 다이나믹 라이트를 모두 넣어준다.

	DWORD		dwLightNum = m_dwCurrentGXLightNum;

	if (dwLightNum > dwMaxLightNum)
		dwLightNum = dwMaxLightNum;

	for (DWORD i=0; i<dwLightNum; i++)
	{
		 m_ppGXLightList[i]->GetLightDesc(pRelatedLight+i);
	}
	return dwLightNum;
}*/
/*
// various heap
GLOBAL_FUNC_DLL		VHEAP_HANDLE		__stdcall	VHPCreateHeap();
GLOBAL_FUNC_DLL		void				__stdcall	VHPHeapCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		void				__stdcall	VHPLeakCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		BOOL				__stdcall	VHPInitialize(VHEAP_HANDLE pVHeap,void* pMassMemory,DWORD dwMassMemorySize,DWORD dwMaxBlockNum);
GLOBAL_FUNC_DLL		void*				__stdcall	VHPAlloc(VHEAP_HANDLE pVHeap,DWORD dwSize);
GLOBAL_FUNC_DLL		void				__stdcall	VHPFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void*				__stdcall	VHPDBGAlloc(VHEAP_HANDLE pVHeap,DWORD dwSize);
GLOBAL_FUNC_DLL		void				__stdcall 	VHPDBGFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void				__stdcall	VHPReleaseHeap(VHEAP_HANDLE pVHeap);
*/

// 데칼 2003.01.15
// 데칼 생성. 익스큐티브 이니셜라이즈때의 버퍼에서 하나 할당한다.
// 특별히 메모리에 뭔가를 생성하는건 아니다. 실시간 할당으로 고쳐야 하나.? 장래엔 그게 좋을지도.
const DWORD	dwMaxTriCount	=	400;			// 임시 상수. 어디쯤 잡아야 하는가?
GXDECAL_HANDLE __stdcall CoExecutive::CreateGXDecal( DECAL_DESC* pDesc, GXDecalSchedulePROC pProc, void* pData, DWORD dwFlag)
{
	DWORD	dwWholeFuncClock	=	GetLowClock();
	DWORD	dwAddOneObject;
	DWORD	dwAddTri;
	DWORD	dwFindTri;
//	DWORD	dwClock[5];
	BOOL	bResult;


	
	CGXDecal*	pDecal = NULL;
	if (!m_pgxMap)
		goto lb_return;


	// 인덱스 발급.
	pDecal = (CGXDecal*)LALAlloc(m_pStaticPoolGXDecal);
	
	if (!pDecal)
		goto lb_return;
#ifdef _DEBUG
	CheckGXDecalsList(pDecal);
#endif
	DWORD	dwIndex;
	dwIndex = ITAddItem(m_pIndexItemTableGXDecal,(void*)pDecal);
	
	if (dwIndex == 0xffffffff)
	{
lb_fail:
		LALFree(m_pStaticPoolGXDecal,pDecal);
		pDecal = NULL;
		goto lb_return;
	}
	

	DWORD	dwFindTriClock;
	Normalize( &(pDesc->v3FaceDirection), &(pDesc->v3FaceDirection));

	DWORD	i, j, dwResult;
	

	// 바운딩 스피어를 만든다.
	float	fFindRadius;
	fFindRadius	=	pDesc->v3XYZScale.x;

	if( fFindRadius < pDesc->v3XYZScale.y)
		fFindRadius	=	pDesc->v3XYZScale.y;
	if( fFindRadius < pDesc->v3XYZScale.z)
		fFindRadius	=	pDesc->v3XYZScale.z;
	
	BOUNDING_SPHERE		FindBoundingSphere;
	FindBoundingSphere.fRs	=	fFindRadius * 1.8f;		// 3루트. 대략값.
	FindBoundingSphere.v3Point	=	pDesc->v3Position;

	// 데칼이 씌여질 오브젝트를 찾는다. 일단 '스트럭쳐' 타입만.
	DWORD	dwFindNearObjectClock;
	dwFindNearObjectClock	=	GetLowClock();

	m_pgxMap->FindNearObject( FindBoundingSphere);
	dwFindNearObjectClock	=	GetLowClock()	-	dwFindNearObjectClock;

	// 찾아진 스트럭쳐 오브젝트에서 삼각형을 추출해서 바로 데칼로 넘긴다.
	pDecal->Initialize(this,pDesc,pProc);
	
	
	pDecal->SetIndex(dwIndex);

	CGXDecal*	pDummy;
	pDummy = m_pDummyGXDecal;
	__asm
	{
		; new 를 사용하지 않으므로 더미 인스턴스를 하나 만들어 함수테이블을 복사한다.
		mov		esi,dword ptr[pDummy]
		mov		edi,dword ptr[pDecal]
		movsd		
	}



	CGXStaticMapObject**	ppSObjBuff;
	DWORD	dwSObjCount;
	dwSObjCount	=	m_pgxMap->GetTargetBuffer( (CGXMapObject***)&ppSObjBuff, GX_MAP_OBJECT_TYPE_STRUCT);

	dwFindTriClock	=	GetLowClock();
//	VECTOR3		TriBuff[dwMaxTriCount*3];
	for( i = 0; i < dwSObjCount; i++)
	{
		dwAddOneObject	=	GetLowClock();
		DWORD	pTempTriIndexBuff[dwMaxTriCount];
		dwFindTri		=	GetLowClock();
		dwResult	=	ppSObjBuff[i]->GetTree()->FindTriWithSphere( pTempTriIndexBuff, dwMaxTriCount, &FindBoundingSphere, pDesc->bLookAtPivot); // FALSE 이놈은 바꿔볼 필요가 있다.
		dwFindTri		=	GetLowClock()	-	dwFindTri;
		if( dwResult > dwMaxTriCount)
		{
			_asm int 3;
			dwResult	=	dwMaxTriCount;			// 만약 한계를 넘으면 할 수 있는 한 끝까지 채우고,
		}
		if( dwResult == 0)
			continue;

		dwAddTri	=	GetLowClock();
		for( j = 0; j < dwResult; j++)
		{
			VECTOR3*	pSTriBuff	=	ppSObjBuff[i]->GetTree()->GetTriBuffer();
			DWORD	TriCount	=	pDecal->AddTri( &(pSTriBuff[pTempTriIndexBuff[j]*3]));
			if( TriCount != 0)
				_asm nop;


		}
		dwAddTri	=	GetLowClock()	-	dwAddTri;

		dwAddOneObject	=	GetLowClock()	-	dwAddOneObject;
		_asm nop;
	}
	dwFindTriClock	=	GetLowClock()	-	dwFindTriClock;


	// 삼각형 모드 add 했으면 빌드.
	DWORD	dwDecalCreateClock;
	dwDecalCreateClock	=	GetLowClock();
	dwResult	=	pDecal->Build( );
	dwDecalCreateClock	=	GetLowClock()	-	dwDecalCreateClock;

	if( bResult == 0)
	{
		_asm int 3;
		goto lb_fail;
	}

	// 트리에 인서트.
	DWORD	dwTreeIndex;
	dwTreeIndex	=	m_pgxMap->AddDecal(pDecal);
	pDecal->SetClipperIndex(dwTreeIndex);

	dwWholeFuncClock	=	GetLowClock()	-	dwWholeFuncClock;
	m_dwCurrentDecalCount++;
lb_return:

	return (GXDECAL_HANDLE)pDecal;
	
}

// 데칼 삭제. 익스큐티브 버퍼에서 그냥 하나 지운다.
BOOL __stdcall CoExecutive::ImmDeleteGXDecal( GXDECAL_HANDLE gxd)
{
	// 핸들체크 요..
#ifdef	_DEBUG
	CheckHandle(gxd);
#endif

	BOOL	bResult = FALSE;

	CGXDecal*	pDecal = (CGXDecal*)gxd;

	
	pDecal->Delete();

	ITDeleteItem(m_pIndexItemTableGXDecal,pDecal->GetIndex());
	LALFree(m_pStaticPoolGXDecal,pDecal);
	
	m_dwCurrentDecalCount--;
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL __stdcall CoExecutive::DefDeleteGXDecal(GXTRIGGER_HANDLE gxh)
{
	BOOL	bResult;
	DWORD	argList[2];
	argList[0] = (DWORD)this;
	argList[1] = (DWORD)gxh;
	bResult = DPCQPushDPC(m_pDPCQ,Def_DeleteGXDecal,2,argList,NULL,0);
#ifdef	_DEBUG
	if (!bResult)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail to DeleteGXDecal,DPCQ Overflow : %x\n",gxh);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif
	return bResult;
}
// 데칼 모두 삭제. 익스큐티브의 버퍼에서 모두 지운다. 
/*
BOOL __stdcall CoExecutive::DeleteAllGXDecal()
{
	DWORD	i;
	for( i = 0; i < m_dwMaxDecalNum; i++)
	{
		if( m_pDecalClipperIndexBuff[i] != -1)
		{
			m_Clipper.DeleteDecal( m_pDecalClipperIndexBuff[i]);
			m_pDecalBuff[i].Delete();
			m_pDecalClipperIndexBuff[i]	=	-1;

		}
	}
	m_dwCurrentDecalCount	=	0;
	
//	여기
//	// 일단 트리에서 지우고.
//	int*	pArray;
//	DWORD	dwCount	=	m_DecalIndexManager.GetCurrentIndex(&pArray);
//	DWORD	i;
//	for( i = 0; i < dwCount; i++)
//	{
//		m_Clipper.DeleteDecal( m_pDecalClipperIndexBuff[pArray[i]]);
//	}
//	// 인덱스 릴리즈 한다음. 
//	m_DecalIndexManager.ReleaseAllIndex();
//	m_dwCurrentDecalCount	=	0;
	
	return	TRUE;		// 실패할 일은 없을려나.?
}
*/


/*
void CoExecutive::RenderDecal()
{
	CGXMapObject**		ppGXMapObjList;
	DWORD	i;
	VIEW_VOLUME	vv;
	m_pGeometry->GetViewVolume(&vv,m_dwViewportIndex);
//	m_Clipper.FindRenderCandidate(vv);
	DWORD	dwObjCount	=	m_pgxMap->GetRenderTargetBuffer(&ppGXMapObjList, GX_MAP_OBJECT_TYPE_DECAL);

	if( dwObjCount != 0)
	{
		DWORD	dwRenderDecalClock	=	GetLowClock();
		DWORD	dwRenderOneDecalClock;
		for( i = 0; i < dwObjCount; i++)
		{
			CGXDecal*	pDecal	=	(CGXDecal*)(ppGXMapObjList[i]);
			dwRenderOneDecalClock	=	GetLowClock();
			pDecal->Render();
			dwRenderOneDecalClock	=	GetLowClock()	-	dwRenderOneDecalClock;
		}
		dwRenderDecalClock	=	GetLowClock()	-	dwRenderDecalClock;
	}
}
*/


void CoExecutive::SetAfterInterpolation( AfterInterpolationCallBack pfCallBack)
{
	m_pfAfterInterpolationCallBack	=	pfCallBack;	
}

/*
void CoExecutive::GXOSetAcceleratedVelocity( GXOBJECT_HANDLE gxh, VECTOR3* pv3Velocity)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
//	((CoGXObject*)gxh)->SetAcceleratedVelocity(pv3Velocity);
	_asm int 3;
}
*/

BOOL CoExecutive::GXOIsLanding( GXOBJECT_HANDLE gxh)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	BOOL	bLanding;
	bLanding	=	((CoGXObject*)gxh)->IsLanding();
	return	bLanding;
}

// 매 프레임마다 속도벡터를 적용할 것인가?
/*
void CoExecutive::GXOSetApplyVelocityFlag( GXOBJECT_HANDLE gxh, BOOL bApply)
{
	_asm int 3;
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
//	((CoGXObject*)gxh)->SetApplyVelocityFlag( bApply);
}*/


// 현재의 게임 프레임을 리턴한다.
DWORD CoExecutive::GetCurrentGameFrame(void)
{
	return	m_dwFrameCount;
}

/*
void CoExecutive::GXOGetVelocity( GXOBJECT_HANDLE gxo, VECTOR3* pOutVelocity)
{
	_asm int 3;
#ifdef	_DEBUG
	CheckHandle(gxo);
#endif
//	((CoGXObject*)gxo)->GetVelocity( pOutVelocity);
}
*/


ULONG __stdcall CoExecutive::GXMGetStaticModel( I3DStaticModel** ppStaticModel)
{
	ULONG	ulResult;
		
	if( m_pgxMap)
	{
		I3DStaticModel*		pModel	=	m_pgxMap->GetBaseModel();
		*ppStaticModel	=	pModel;
		pModel->AddRef();
		ulResult = 0;
	}
	else
	{
		ulResult = 0xffffffff;
		*ppStaticModel = NULL;
	}
	return ulResult;
}

void __stdcall CoExecutive::GXOGetLastVelocityAfterCollisionTest( GXOBJECT_HANDLE gxh, VECTOR3* pLastVelocity)
{
#ifdef	_DEBUG
	CheckHandle(gxh);
#endif
	((CoGXObject*)gxh)->GetLastVelocityAfterCollisionTest( pLastVelocity);
}


BOOL __stdcall CoExecutive::DisableRenderAllStaticModels(VOID)
{
	if (NULL != m_pgxMap)
	{
		DWORD dwStmNum = m_pgxMap->GetSTMMapObjectNum();

		CGXStaticMapObject* pGXMapObject = NULL;
		int i = 0;
		for (i = 0; i < (int)dwStmNum; i++)
		{
			pGXMapObject = m_pgxMap->GetSTMMapObject(i);
			pGXMapObject->DisableRender();
			pGXMapObject = NULL;
		}

	}

	return TRUE;
}


BOOL __stdcall CoExecutive::EnableRenderAllStaticModels(VOID)
{
	if (NULL != m_pgxMap)
	{
		DWORD dwStmNum = m_pgxMap->GetSTMMapObjectNum();

		CGXStaticMapObject* pGXMapObject = NULL;
		int i = 0;
		for (i = 0; i < (int)dwStmNum; i++)
		{
			pGXMapObject = m_pgxMap->GetSTMMapObject(i);
			pGXMapObject->EnableRender();
			pGXMapObject = NULL;
		}

	}

	return TRUE;
}




//trustpak 2005/06/24

void  __stdcall	CoExecutive::SetModelLODUsingMode(DWORD dwLodUsingMode, DWORD dwLevel)
{
	if (NULL != m_pGeometry)
	{
		m_pGeometry->SetModelLODUsingMode(dwLodUsingMode, dwLevel);
	}
}

DWORD __stdcall	CoExecutive::GetModelLODUsingMode(VOID)
{
	if (NULL == m_pGeometry)
	{	
		return 0xffffffff;
	}

	return m_pGeometry->GetModelLODUsingMode();
}

DWORD __stdcall	CoExecutive::GetModelLodUsingLevel(VOID)
{
	if (NULL == m_pGeometry)
	{	
		return 0xffffffff;
	}

	return m_pGeometry->GetModelLodUsingLevel();
}

///