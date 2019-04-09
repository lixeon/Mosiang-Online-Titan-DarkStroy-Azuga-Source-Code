#pragma once

#include "../4DyuchiGRX_common/stdafx.h"
#include "object.h"
#include "MaterialList.h"
#include "motion.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "bone_object.h"
#include "light_obj.h"

class CoGeometry;
class CModelRefManager;


struct TRANSFORM_OBJECT_SET
{
	DWORD			dwObjectNum;
	CBaseObject**	ppObjectList;
};


struct FILE_SCENE_HEADER
{
	DWORD			dwVersion;
	DWORD			dwObjectNum;
	DWORD			dwMaterialNum;

	DWORD			dwMaxMeshObjectNum;
	DWORD			dwMaxLightObjectNum;
	DWORD			dwMaxCameraObjectNum;
	DWORD			dwMaxBoneObjectNum;
};

struct MOTION_LIST_DESC
{
	DWORD			dwMaxMotionNum;
	DWORD			dwMotionNum;
	CMotion**		ppMotionList;
};
class CMaterialLib;
struct MATERIAL_LIST_DESC
{
	DWORD			dwMaxMaterialNum;
	DWORD			dwMaterialNum;
	CMaterialLib**	ppMtlLibList;
};


class CoModel : public I3DModel
{
	// file header //
	DWORD					m_dwObjectNum;
	DWORD					m_dwMaterialNum;

	DWORD					m_dwMaxMeshObjectNum;
	DWORD					m_dwMaxLightObjectNum;
	DWORD					m_dwMaxCameraObjectNum;
	DWORD					m_dwMaxBoneObjectNum;

	///////////////////////////////////////////////////

	DWORD					m_dwRefCount;
	DWORD					m_dwBoneObjectNum;
	DWORD					m_dwMeshObjectNum;
	DWORD					m_dwLightObjectNum;
	DWORD					m_dwCameraObjectNum;

	CMaterialList			m_MaterialList;
//	DWORD					m_dwDefaultMtlSetIndex;
	DWORD					m_dwTransformSetNum;
	TRANSFORM_OBJECT_SET*	m_pTransformSetList;
	I4DyuchiGXRenderer*		m_pRenderer;

	CBaseObject**			m_ppObjectList;
	CMeshObject*			m_pMeshObjectList;
	CCameraObject*			m_pCameraObjectList;
	CBoneObject*			m_pBoneObjectList;
	CLightObject*			m_pLightObjectList;
				
	CoGeometry*				m_pGeometry;
	CModelRefManager*		m_pModelRefManager;
	DWORD					m_dwIndexInGeometry;

	COLLISION_MODEL_DESC*	m_pColMeshModelDescLocal;
	COLLISION_MODEL_DESC*	m_pColMeshModelDescWorld;
	

	
	BOOL					m_bOptimizedFlag;
	BOOL					m_bInitFlag;
	BOOL					m_AXIS_ALIGN_OK;

	char					m_szFileName[_MAX_PATH];
	DWORD					m_dwColMeshSize;
	DWORD					m_dwColMeshDescOffsetBoneEntry;

	CMeshObject**			m_ppViewAlignedObjectsList;
	DWORD					m_dwViewAlignedObjectsNum;
	
		
	DWORD					m_dwSubModelNum;
	CoModel*				m_pSubModel[MAX_LOD_MODEL_NUM];
	
	BOOL					PrepareAnimation();
	CBaseObject*			GetObject(char* szObjName);

	COLLISION_MODEL_DESC*	CreateCollisionDesc();
	void					ReadLODModels(char* szFileName,DWORD dwSubModelNum);
	CMotion*				GetMotion(DWORD dwRefIndex,DWORD dwMotionIndex);
	CMaterialLib*			GetMaterialLib(DWORD dwRefIndex,DWORD dwMtlIndex);


public:	
	BOOL					DuplicateStatus(CoModel* pDestModel,DWORD dwDestRefIndex,DWORD dwSrcRefIndex);
	CMotionObject*			GetMotionObject(char* szObjName,DWORD dwNameLen,DWORD dwRefIndex,DWORD dwMotionIndex);
	DWORD					AddMotion(CMotion* pMotion,DWORD dwRefIndex,DWORD dwMotionIndex);	
	DWORD					AddMaterial(CMaterialLib* pMtlLib,DWORD dwRefIndex,DWORD dwMtlIndex);


	//external interface
	/////////////////////////////////////////////////////////////////////////////////////////////////
    STDMETHODIMP			QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)	AddRef(void);
    STDMETHODIMP_(ULONG)	Release(void);
	
	BOOL			__stdcall	GetCollisionMeshObjectDesc(COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwModelRefIndex,DWORD dwObjIndex);
	BOOL			__stdcall	GetCollisionModelDesc(COLLISION_MODEL_DESC* pColMeshModelDesc,DWORD dwModelRefIndex);
	
	void			__stdcall	UpdateColMeshDesc(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag);

	DWORD			__stdcall	Reference();
	void			__stdcall	UnReference(DWORD dwRefIndex);
	BOOL			__stdcall	Render(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwAlpha,DWORD dwMtlSetIndex,DWORD dwEffectIndex,DWORD dwFlag);

	BOOL			__stdcall	RenderObject(DWORD dwObjIndex,DWORD dwAlpha,DWORD dwFlag);

	BOOL			__stdcall 	CreateMotionList(DWORD dwNum,DWORD dwRefIndex);
	BOOL			__stdcall	CreateMaterialList(DWORD dwNum,DWORD dwRefIndex);
	BOOL			__stdcall	Initialize(I4DyuchiGXRenderer* pDev,DWORD dwFlag);
	DWORD			__stdcall	AddMotion(char* szFileName,DWORD dwRefIndex,DWORD dwMotionIndex);
	DWORD			__stdcall	AddMaterial(char* szFileName,DWORD dwRefIndex,DWORD dwMtlIndex);

	
	void			__stdcall	DeleteMotion(DWORD dwRefIndex,DWORD dwMotionIndex);
	void			__stdcall	DeleteAllMotionPerReference(DWORD dwRefIndex);
	void			__stdcall 	DeleteAllMotion();		

	void			__stdcall	DeleteMaterial(DWORD dwRefIndex,DWORD dwMtlIndex);
	void			__stdcall	DeleteAllMaterialPerReference(DWORD dwRefIndex);
	void			__stdcall	DeleteAllMaterial();
	
	BOOL			__stdcall	UnloadMotionPerReference(DWORD dwRefIndex);
	BOOL			__stdcall	UnloadMaterialPerReference(DWORD dwRefIndex);
	
	void			__stdcall	DisableUnloadPreLoadedResource(DWORD dwRefIndex);	// 바인딩 되어있는 모든 모션과 매터리얼까지 포함된다
	void			__stdcall	EnableUnloadPreLoadedResource(DWORD dwRefIndex);	// 바인딩 되어있는 모든 모션과 매터리얼까지 포함된다
	
	
	
	DWORD			__stdcall	CreateIVertexList(IVERTEX** ppVertex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex);
	void			__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);

	DWORD			__stdcall	GetMaterialNum(DWORD dwRefIndex);
	DWORD			__stdcall	GetMotionNum(DWORD dwRefIndex);
	BOOL			__stdcall	GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwRefIndex);
	DWORD			__stdcall	GetObjectIndex(char* szObjName);
	MATRIX4*		__stdcall	GetObjectResultMatrix(DWORD dwRefIndex,DWORD dwObjIndex);

	
	

	void			__stdcall	Optimize();	
	DWORD			__stdcall	GetCameraList(CAMERA_DESC* pDesc,DWORD dwNum);
	DWORD			__stdcall	GetLightList(LIGHT_DESC* pDesc,DWORD dwNum);
	DWORD			__stdcall	GetMeshObjectIndex(DWORD dwSeqIndex);
	DWORD			__stdcall	GetMeshObjectNum();
	
	DWORD			__stdcall	GetObjectNum();
	
		

	DWORD			__stdcall	GetMeshObjectWithScreenCoord(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);
	DWORD			__stdcall	GetMeshObjectWithScreenCoordAndMatrix(VECTOR3* pv3IntersectPoint,float* pfDist,DWORD* pdwFaceGroupIndex,DWORD dwRefIndex,MATRIX4* pParent,MATRIX4* pTransform,DWORD dwFrame,DWORD dwMotionIndex,POINT* ptCursor,MATRIX4* pMatView,MATRIX4* pMatProj,SHORT_RECT* pClipRect,DWORD dwFlag);
		
	char*			__stdcall	GetObjectName(DWORD dwObjIndex);
	BOOL			__stdcall	SetZOrder(int iOrder,DWORD dwRefIndex);
	
	BOOL			__stdcall	DuplicateAnimation(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex);
	BOOL			__stdcall	DuplicateMaterial(DWORD dwDestRefIndex,I3DModel* pDestModel,DWORD dwSrcRefIndex);
	DWORD			__stdcall	GetMaterialIndex(char* szMtlName);
	char*			__stdcall	GetModelFileName();
	BOOL			__stdcall	Unload();
	
	BOOL			__stdcall	InitializeIllusionEffect(DWORD dwRefIndex,DWORD dwMaxIllusionFrameNum,DWORD dwObjIndex,void* pMtlHandle,DWORD dwFlag);
	BOOL			__stdcall	SetIllusionEffectMaterial(DWORD dwRefIndex,void* pMtlHandle);
	
	void			__stdcall	SetBeginIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex);
	void			__stdcall	SetEndIllusionEffect(DWORD dwRefIndex,DWORD dwObjIndex);
	BOOL			__stdcall	GetWorldMatrix(MATRIX4* pMatWorld,DWORD dwRefIndex,DWORD dwObjIndex);
	BOOL			__stdcall	SetAxisAlignOK(BOOL bSwitch,DWORD dwRefIndex);
//	void			__stdcall	ClearCache(DWORD dwRefIndex);
	
	
	

	
	
	////////////////////////////////////////////////////////////////////////////////////////////////

	BOOL						RenderShadow(DWORD dwRefIndex,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwFrame,DWORD dwMotionIndex,LIGHT_DESC* pLightDesc,VIEW_VOLUME* pShadowViewVolume,DWORD dwShadowNum,DWORD dwAlpha,DWORD dwFlag);
	CoGeometry*					GetGeometry()	{return m_pGeometry;}
	BOOL						Duplicate(CMeshObject* pMeshObjectDest,DWORD dwSeqIndex,DWORD dwRefIndex);
	BOOL						SetFrame(MODEL_STATUS* pModelStatus,MATRIX4* pMatrixEntry,MATRIX4* pMatrixEntryForPhysiqueLocal,COLLISION_MODEL_DESC* pColModelDesc,MATRIX4* pParentMat,MATRIX4* pTransformMat,DWORD dwRefIndex,DWORD dwFrame,DWORD dwMotionIndex,DWORD dwFlag);
	
	BOOL						ReadFile(char* szFileName,DWORD dwFlag);
	void						SetGeometry(CoGeometry*	pGeometry);
	
	void						SetIndexInGeometry(DWORD dwIndex) {m_dwIndexInGeometry = dwIndex;}
	DWORD						GetFileIndex() {return m_dwIndexInGeometry;}
	
	CBaseObject*				SelectObject(DWORD dwIndex);
	CMaterial*					GetMaterial(DWORD dwMtlIndex);
	BOOL						GetModelStatus(MODEL_STATUS** ppModelStatus,DWORD dwRefIndex);
	BOOL						GetModelSetFrameArgs(SET_FRAME_ARGS** ppArgs,DWORD dwRefIndex);
	BOOL						GetMatrixEntry(MATRIX4** ppMatrixEntry,MATRIX4** ppMatrixEntryForPhysiqueLocal,DWORD dwRefIndex);
	
	BOOL						GetCameraMatrix(MATRIX4* pMatView,DWORD dwRefIndex);
	COLLISION_MESH_OBJECT_DESC*	GetColMeshObjectLocal(DWORD	dwOffset);
	
	
	void						ReleaseObjects();
	void						ReleaseMaterials();
	void						ReleaseTransformSet();
	


	CoModel();
	~CoModel();

	
};

