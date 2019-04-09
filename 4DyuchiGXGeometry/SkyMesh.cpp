#include "SkyMesh.h"
#include "CoGeometry.h"

CoSkyMesh::CoSkyMesh()
{
	memset((char*)this+4,0,sizeof(CoSkyMesh)-4);
}


STDMETHODIMP CoSkyMesh::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_SkyMesh == refiid)
		*ppv = (ISkyMesh*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CoSkyMesh::AddRef(void)
{
	m_dwRefCount++;
	return 0;
}
STDMETHODIMP_(ULONG) CoSkyMesh::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
BOOL CoSkyMesh::Initialize(CoGeometry* pGeometry,I3DModel* pModel,DWORD dwFlag)
{
	m_pGeometry = pGeometry;
	
	m_pModel = pModel;
	pModel->AddRef();
	
	m_dwRefIndex = pModel->Reference();
		
	SetIdentityMatrix(&m_matTransform);
	m_dwAlpha = 255;
	m_dwCurrentMotionIndex = 1;
	m_dwCurrentFrame = 0;
	
	return TRUE;
}
void __stdcall CoSkyMesh::SetViewportIndex(DWORD dwViewportIndex)
{
	m_dwViewportIndex = dwViewportIndex;
}
void __stdcall CoSkyMesh::SetOffset(VECTOR3* pv3Offset)
{
	m_v3Offset = *pv3Offset;
}

BOOL __stdcall CoSkyMesh::Render(DWORD dwFlag)
{
	VECTOR3	v3Camera;
	v3Camera = *m_pGeometry->GetCameraEye(m_dwViewportIndex);
	VECTOR3_ADD_VECTOR3((VECTOR3*)&m_matTransform._41,&m_v3Offset,m_pGeometry->GetCameraEye(m_dwViewportIndex));
	
	return m_pModel->Render(m_dwRefIndex,NULL,&m_matTransform,m_dwCurrentFrame,m_dwCurrentMotionIndex,m_dwAlpha,0,0,RENDER_TYPE_ENABLE_CLIP_PER_OBJECT);
}
BOOL __stdcall CoSkyMesh::CreateMotionList(DWORD dwNum)
{
	return m_pModel->CreateMotionList(dwNum,m_dwRefIndex);
}
DWORD __stdcall CoSkyMesh::AddMotion(char* szFileName,DWORD dwMotionIndex)
{
	return m_pModel->AddMotion(szFileName,m_dwRefIndex,dwMotionIndex);
}
DWORD __stdcall CoSkyMesh::GetMotionNum()
{
	return m_pModel->GetMotionNum(m_dwRefIndex);
}
BOOL __stdcall CoSkyMesh::GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex)
{
	return m_pModel->GetMotionDesc(pMotionDesc,dwMotionIndex,m_dwRefIndex);
}

DWORD __stdcall CoSkyMesh::GetCurrentFrame()
{
	return m_dwCurrentFrame;
}
DWORD __stdcall CoSkyMesh::GetCurrentMotionIndex()
{
	return m_dwCurrentMotionIndex;
}
void __stdcall CoSkyMesh::SetCurrentFrame(DWORD dwFrame)
{
	m_dwCurrentFrame = dwFrame;
}
void __stdcall CoSkyMesh::SetCurrentMotionIndex(DWORD dwMotionIndex)
{
	m_dwCurrentMotionIndex = dwMotionIndex;
}
DWORD __stdcall CoSkyMesh::GetAlpha()
{
	return m_dwAlpha;
}
void __stdcall CoSkyMesh::SetAlpha(DWORD dwAlpha)
{
	m_dwAlpha = dwAlpha;
}
void __stdcall CoSkyMesh::IncreaseFrame(DWORD dwIncFrame)
{
	MOTION_DESC	motDesc;
	if (m_pModel->GetMotionDesc(&motDesc,m_dwCurrentMotionIndex,m_dwRefIndex))
	{
		m_dwCurrentFrame += dwIncFrame;

		if (m_dwCurrentFrame >= motDesc.dwLastFrame)
			m_dwCurrentFrame = 0;
	}
}
CoSkyMesh::~CoSkyMesh()
{
	if (m_pModel)
	{
		m_pModel->DeleteAllMotion();
		m_pModel->UnReference(m_dwRefIndex);
		m_pModel->Release();
		m_pModel = NULL;
	}
}