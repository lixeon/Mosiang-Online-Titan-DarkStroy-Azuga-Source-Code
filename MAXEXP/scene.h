#pragma once

#include "../max_common/stdafx.h"
#include "object.h"
#include "material.h"
#include "object_property.h"
#include "../4DyuchiGRX_Common/IGeometry.h"

class CLightObject;

struct FILE_SCENE_HEADER
{
	DWORD			dwVersion;
	DWORD			dwObjectNum;
	DWORD			dwMaterialNum;
	
	DWORD			dwMeshObjectNum;
	DWORD			dwLightObjectNum;
	DWORD			dwCameraObjectNum;
	DWORD			dwBoneObjectNum;

};

class CScene
{
	DWORD					m_dwObjectNum;
	DWORD					m_dwMaterialNum;

	DWORD					m_dwMeshObjectNum;
	DWORD					m_dwLightObjectNum;
	DWORD					m_dwCameraObjectNum;
	DWORD					m_dwBoneObjectNum;

	CMaterial*				m_pBaseMaterial;
	CBaseObject*			m_pBaseObject;

	CObjectPropertyList*	m_pMeshPropertyList;
	Interface*				m_pInterface;
	BOOL					m_bSuccess;
	

	CMaterial*				AllocMaterial();
	void					FreeObject(CBaseObject* pObj);
	CBaseObject*			AllocObject(OBJECT_TYPE type,char* szObjName);
	DWORD					AddMaterial(Mtl* pMtl,DWORD dwSubMtlIndex);
	Point3					GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	BOOL					TMNegParity(Matrix3 &m);
	TriObject*				GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
	DWORD					ReCreateVertexList(TFACE* ptFace,FACE* pFace,VECTOR3** ppv3,TVERTEX** pptv,DWORD dwTFacesNum,DWORD dwVertexNum,DWORD** ppdwExtVertexIndex,DWORD* pdwExtVertexNum);
	void					ReIndexFace(WORD* pFaceIndex,WORD dwTFaceIndex,POINT3D_REF_TVERTEX* pvRef,POINT3D_REF_TVERTEX* pvRefExt,VECTOR3* pv3,TVERTEX* ptv,DWORD* pCount,DWORD* pOriginalIndex);
	BOOL					DumpMatrix3(NODE_TM* pTM,Matrix3* m, Matrix3* pParentTM);
	BOOL					ConvertMesh(CBaseObject* pObj,INode* pNode,TriObject* tri);
	BOOL					ConvertIllusionMesh(CBaseObject* pObj,INode* pNode,TriObject* tri);
	BOOL					ConvertNodeHeader(CBaseObject* pObj,INode* pNode);
	
	CBaseObject*			ConvertGeometry(INode* pNode);
	CBaseObject*			ConvertCamera(INode* pNode);
	CBaseObject*			ConvertLight(INode* pNode);
	void					ConvertLightSetting(CLightObject* pObj,INode* node);
	BOOL					CreateBoundingMesh(CBaseObject* pObj,INode* pNode,TriObject* tri);
public:
	BOOL					IsSuccess() {return m_bSuccess;}

	BOOL					ConvertNodeTM(NODE_TM* pTM,INode* pNode);

	BOOL					SetHirerarchy();
	CBaseObject*			SelectObject(TCHAR* uszObjName);
	void					ReleaseObjects();
	void					ReleaseMaterials();
	BOOL					WriteFile(TCHAR* uszFileName);
	BOOL					ReadFile(TCHAR* uszFileName);
	
	CBaseObject*			InsertObject(INode* pNode);

	CScene(CObjectPropertyList* pMeshPropertyList,Interface* pInterface);
	~CScene();

	
};

extern CScene*		g_pScene;

