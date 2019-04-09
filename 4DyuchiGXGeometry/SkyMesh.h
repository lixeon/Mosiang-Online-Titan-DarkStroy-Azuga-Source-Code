#ifndef SKY_MESH_H
#define SKY_MESH_H

#include "../4DyuchiGRX_Common/IGeometry.h"

class CoGeometry;

class CoSkyMesh : public ISkyMesh
{
	DWORD			m_dwRefCount;
	CoGeometry*		m_pGeometry;
	VECTOR3			m_v3Offset;
	
	DWORD			m_dwCurrentMotionIndex;
	DWORD			m_dwCurrentFrame;
	DWORD			m_dwAlpha;

	DWORD			m_dwRefIndex;
	I3DModel*		m_pModel;

	DWORD			m_dwViewportIndex;
	MATRIX4			m_matTransform;



public:
	BOOL			Initialize(CoGeometry* pGeometry,I3DModel* pModel,DWORD dwFlag);

	// external interface
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);

	BOOL	__stdcall		Render(DWORD dwFlag);
	void	__stdcall		SetViewportIndex(DWORD dwViewportIndex);
	void	__stdcall		SetOffset(VECTOR3* pv3Offset);
	
	BOOL	__stdcall		CreateMotionList(DWORD dwNum);
	DWORD	__stdcall		AddMotion(char* szFileName,DWORD dwMotionIndex);	
	DWORD	__stdcall		GetMotionNum();
	BOOL	__stdcall		GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex);
	DWORD	__stdcall		GetCurrentMotionIndex();
	void	__stdcall		SetCurrentMotionIndex(DWORD dwMotionIndex);
	DWORD	__stdcall		GetCurrentFrame();
	void	__stdcall		SetCurrentFrame(DWORD dwFrame);
	DWORD	__stdcall		GetAlpha();
	void	__stdcall		SetAlpha(DWORD dwAlpha);
	void	__stdcall		IncreaseFrame(DWORD dwIncFrame);


	CoSkyMesh();
	~CoSkyMesh();


};

#endif

