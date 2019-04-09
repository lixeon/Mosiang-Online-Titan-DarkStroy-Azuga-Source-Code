// ModelRefManager.h: interface for the CModelRefManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODELREFMANAGER_H__79F578B0_7E03_4DDF_8C45_EC352D2AA53C__INCLUDED_)
#define AFX_MODELREFMANAGER_H__79F578B0_7E03_4DDF_8C45_EC352D2AA53C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../4DyuchiGXGFunc/global.h"

struct MOTION_LIST_DESC;
struct MATERIAL_LIST_DESC;
class CoModel;

struct ILLUSION_KEY_DESC
{
	MATRIX4					tm;
	DWORD					dwFrame;
	DWORD					dwFacesNum;
	
	ILLUSION_KEY_DESC*		pNext;
	VECTOR3					pv3List[1];
};

struct ILLUSION_DESC
{
	CMeshObject*			pMeshObj;
	BOOL					bEnable;
	DWORD					dwVerticesNum;
	DWORD					dwResultVerticesNum;
	DWORD					dwFacesNum;
	DWORD					dwIllusionFacesNum;
	DWORD					dwMaxIllutionKeyNum;
	DWORD					dwCurIllusionKeyNum;
	ILLUSION_KEY_DESC*		pIllusionKeyHead;
	ILLUSION_KEY_DESC*		pIllusionKeyTail;

	void*					pMtlHandle;

	void*					pMtlHandleAllocated;		// 해제 필요함.
	void*					pTriBufferHandle;
	IVERTEX*				pIVertexList;
	
};

struct MODEL_REF_DESC
{
	DWORD					dwRefIndex;
	CoModel*				pModel;
	
	DWORD					dwSize;

	DWORD					dwDataSize;
	char*					pDataEntry;

	MATRIX4*				pMatrixEntry;
	MATRIX4*				pMatrixEntryForPhysiqueLocal;
	MODEL_STATUS*			pModelStatus;
	SET_FRAME_ARGS*			pArgsSetFrame;
	MOTION_LIST_DESC*		pMotionListDesc;
	MATERIAL_LIST_DESC*		pMaterialListDesc;
	DWORD					dwColMeshSize;
	COLLISION_MODEL_DESC*	pColModelDesc;
	ILLUSION_DESC*			pIllusionDesc;
	
	
};
interface I4DyuchiGXRenderer;

class CModelRefManager  
{
	DWORD					m_dwRefNum;
	DWORD					m_dwMaxRefNum;
	MODEL_REF_DESC**		m_ppModelRefDescList;
	INDEXCR_HANDLE			m_pIC;
	I4DyuchiGXRenderer*		m_pRenderer;


public:
	DWORD					GetRefNum()			{return m_dwRefNum;}
	DWORD					GetMaxRefNum()		{return m_dwMaxRefNum;}
	BOOL					IsValidRefIndex(DWORD dwRefIndex,DWORD dwColMeshSize);
	BOOL					Initialize(I4DyuchiGXRenderer* pRenderer,DWORD dwMaxRefNum);

	DWORD					AllocRefMemory(DWORD dwColMeshSize,DWORD dwObjNum,CoModel* pModel);
	void					FreeRefMemory(DWORD dwRefIndex);
	DWORD					GetRefIndexWithModelPtr(DWORD* pdwIndexList,CoModel* pModel,DWORD dwMaxNum);
	
	BOOL					GetModelRefDesc(MODEL_REF_DESC* pModelRefDesc,DWORD dwRefIndex);
	BOOL					GetModelStatus(MODEL_STATUS** ppModelStatus,DWORD dwRefIndex);
	BOOL					GetMatrixEntry(MATRIX4** ppMatrixEntry,MATRIX4** ppMatrixEntryForPhysiqueLocal,DWORD dwRefIndex);
	BOOL					GetMotionListDesc(MOTION_LIST_DESC** ppMotionListDesc,DWORD dwRefIndex);
	BOOL					GetMaterialListDesc(MATERIAL_LIST_DESC** ppMaterialListDesc,DWORD dwRefIndex);
	BOOL					GetModelSetFrameArgs(SET_FRAME_ARGS** ppArgs,DWORD dwRefIndex);
	ILLUSION_DESC*			GetModelIllusionDesc(DWORD dwRefIndex);

	BOOL					GetColModelDesc(COLLISION_MODEL_DESC**	ppColModelDesc,DWORD dwRefIndex);
	BOOL					GetModelRefData(char** ppDataEntry,DWORD* pdwSize,DWORD dwRefIndex);
	BOOL					DuplicateStatus(DWORD dwDestRefIndex,DWORD dwSrcRefIndex);
	BOOL					InitializeIllusionEffect(DWORD dwRefIndex,DWORD dwMaxIllusionKeyNum,CMeshObject* pMeshObj,void* pMtlHandle,DWORD dwFlag);



	CModelRefManager();
	~CModelRefManager();

};

#endif // !defined(AFX_MODELREFMANAGER_H__79F578B0_7E03_4DDF_8C45_EC352D2AA53C__INCLUDED_)
