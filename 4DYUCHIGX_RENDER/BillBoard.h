#include "BaseMeshObject.h"
#include "vertexbuffer.h"
#include "CTexture.h"
#include "d3d_helper.h"

struct DIMATERIAL;

class CBillBoardObject :  public CBaseMeshObject
{
	CVertexBuffer			m_VB;
	WORD*					m_pIndex;
	/*
	DWORD					m_dwAlpha;
	CTexture				m_Texture;
	
	BOOL					m_b2Side;
	*/
	
	TVERTEX*				m_ptvTex;
	VECTOR3*				m_pv3Local;
	DIMATERIAL*				m_pMaterial;
		

	BOOL					CreateRenderPrimitive(VECTOR3* pv3,TVERTEX* ptv,WORD* pIndex,DWORD dwFacesNum);
//	BOOL					SetMaterial(MATERIAL* pMaterial);

public:

	BOOL	__stdcall		StartInitialize(MESH_DESC* pDesc,IGeometryController* pControl,IGeometryControllerStatic* pControlStatic);
	BOOL	__stdcall		InsertFaceGroup(FACE_DESC* pDesc);
	void	__stdcall		EndInitialize();
	BOOL	__stdcall		Render(
		DWORD dwRefIndex,
		DWORD dwAlpha,
		LIGHT_INDEX_DESC* pDynamicLightIndexList,DWORD dwLightNum,
		LIGHT_INDEX_DESC* pSpotLightIndexList,DWORD dwSpotLightNum,
		DWORD dwMtlSetIndex,
		DWORD dwEffectIndex,
		DWORD dwFlag);
	BOOL	__stdcall		Update(DWORD dwFlag);
	
//	BOOL					Get2SideFlag()	{return m_b2Side;}
//	void					Set2Side(BOOL bType) {m_b2Side = bType;}	
	
	CBillBoardObject();
	virtual ~CBillBoardObject();
};

