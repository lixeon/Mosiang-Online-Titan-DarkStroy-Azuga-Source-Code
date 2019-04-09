#pragma once

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGRX_Common/IGeometry.h"
#include "GXMapObject.h"
#include "executive.h"

class CoGXLight;
class CoGXObject : public CGXMapObject
{
	friend	void MClipper::DeleteAll();
	MODEL_HANDLE		m_ModelList[MAX_MODEL_NUM_PER_GXOBJECT];
	DWORD				m_dwModelNum;
	
	MATRIX4				m_matTransform;
	MATRIX4				m_matScale;
	MATRIX4				m_matRot;
	
	MATRIX4*			m_pParentMatrix;
		
	CoGXObject*			m_pParentGXObject;
	CoGXObject*			m_pChildGXObject[MAX_ATTATCH_OBJECTS_NUM];
	DWORD				m_dwChildGXObjectNum;

	CoGXLight*			m_pChildLight[MAX_ATTATCH_OBJECTS_NUM];
	DWORD				m_dwChildGXLightNum;
	
	DWORD				m_dwCurrentFrame;
	DWORD				m_dwCurrentMotionIndex;
	DWORD				m_dwCurrentMtlSetIndex;

	VECTOR3				m_v3Angle;
	
	VECTOR3				m_v3Axis;
	float				m_fRad;
	VECTOR3				m_v3Pos;
	VECTOR3				m_v3Scale;
	GXSchedulePROC		m_pProc;
	DWORD				m_dwUpdateFlag;

	VECTOR3				m_v3Velocity;				// 속도벡터.
	BOOL				m_bLanding;					// 땅에 닿아있는가? 이거 여기서 하는거 맞나.?

	//	2004/01/08	보간을 위해 추가.
	VECTOR3				m_v3OldPos;					//	MovePosition 하는 순간 예전 좌표를 저장해둔다.
	VECTOR3				m_v3InterpolatedRenderPos;		//	랜더링이 들어오는 순간 프레임을 비교해서 중간위치 보정.
	float				m_fCurrentRenderTime;		//	랜더링이 들어오는 순간 프레임타임, 0.0f가 예전게임프렘, 1.0f가 새 게임프레임.

	DWORD				m_dwEffectIndex;
	I3DModel*			m_pModelForIllusion;
	DWORD				m_dwObjIndexForIllusion;
	DWORD				m_dwRefIndexForIllusion;

	COLLISION_MESH_OBJECT_DESC	m_colMeshDescLocal;
	COLLISION_MESH_OBJECT_DESC	m_colMeshDescWorld;
	MODEL_FILE_DESC*			m_pModelFileItem;
	
	BOUNDING_VOLUME		m_BV;						// 충돌 처리용 볼륨.				2002/03/06
//	void				(*m_pCollisionTestFn)( CoExecutive* pExecutive, CoGXObject* pThis, VECTOR3* pOutCandidate, VECTOR3* pOutLastVelocity, VECTOR3* pTo);
	void				(*m_pCollisionTestFn)( CoExecutive* pExecutive, CoGXObject* pThis, COLLISION_TEST_RESULT* pCollTestResult, VECTOR3* pTo, DWORD dwBufferIndex);
//	BOOL				m_bApplyVelocity;

	void				InitializeCollisionMesh();
	void				Cleanup();

	//	2004/01/08	보간을 위해 추가.
	void SetOldPosition( VECTOR3* pPos);

public:
	MATRIX4*			GetParentMatrix() {return m_pParentMatrix;}
	void GetLastVelocityAfterCollisionTest( VECTOR3* pLastVelocity);
//	void GetVelocity( VECTOR3* pOutVelocity);
//	void OnFrameApplyVelocity();
//	void SetAcceleratedVelocity( VECTOR3* pv3Velocity);
	void InterpolatePosition( float fTime);

	friend	void	CoExecutive::ResourceCheck();
	
	void				SetScheduleProc(GXSchedulePROC pProc) {m_pProc = pProc;}
	GXSchedulePROC		GetScheduleProc() {return m_pProc;}

	BOOL				InitializeIllusionEffect(DWORD dwMaxIllusionFrameNum,char* szObjName,void* pMtlHandle,DWORD dwFlag);
	void				SetModelFileDesc(MODEL_FILE_DESC* pModFileDesc) {m_pModelFileItem = pModFileDesc;}
	DWORD				DuplicateModelHandle(MODEL_HANDLE* pDest);
	
	void				EnableHFieldApply()	{m_dwPropertyFlag |= GXOBJECT_CREATE_TYPE_APPLY_HFIELD;}
	void				DisableHFieldApply() {m_dwPropertyFlag &= (~GXOBJECT_CREATE_TYPE_APPLY_HFIELD);}
	BOOL				IsHFieldApply()		{if (m_dwPropertyFlag & GXOBJECT_CREATE_TYPE_APPLY_HFIELD) return TRUE; else return FALSE;}

	void				SetZOrder(int iZOrder);

	void				EnableAsEffect();
	void				DisableAsEffect();
	BOOL				IsAsEffect()		{if (m_dwPropertyFlag & GXOBJECT_CREATE_TYPE_EFFECT) return TRUE; else return FALSE;}

	void				EnableSelfIllumin()		{ m_dwRenderFlag |= RENDER_TYPE_SELF_ILLUMIN;}
	void				DisableSelfIllumin()	{ m_dwRenderFlag &= (~RENDER_TYPE_SELF_ILLUMIN);}
	BOOL				IsEnableSelfIllumin()	{ if (m_dwRenderFlag & RENDER_TYPE_SELF_ILLUMIN) return TRUE; else return FALSE;}

	void				SetEffectIndex(DWORD dwEffectIndex);
	DWORD				GetEffectIndex();
	
	void				UpdateCollisionMesh();	
	BOOL				Initialize(CoExecutive* pExecutive,MODEL_HANDLE* pModelHandle,DWORD dwModelNum,GXSchedulePROC pProc,DWORD dwFlag);
	

	I3DModel*			GetModel(DWORD dwModelIndex)		{return m_ModelList[dwModelIndex].pModel;}
	DWORD				GetModelRefIndex(DWORD dwModelIndex){return m_ModelList[dwModelIndex].dwRefIndex;}
	DWORD				GetModelNum()						{return m_dwModelNum;}
	
	DWORD				GetCurrentFrame()					{return m_dwCurrentFrame;}
	DWORD				GetCurrentMotionIndex()				{return m_dwCurrentMotionIndex;}
	DWORD				GetCurrentMaterialIndex()			{return m_dwCurrentMtlSetIndex;}


	void				SetCurrentFrame(DWORD dwFrame)		{m_dwCurrentFrame = dwFrame;}
	void				SetCurrentMotionIndex(DWORD dwMotionIndex) {m_dwCurrentMotionIndex = dwMotionIndex;}
	void				SetCurrentMaterialIndex(DWORD dwMtlIndex) { m_dwCurrentMtlSetIndex = dwMtlIndex;}

	void				SetPosition(VECTOR3* pv3Pos, BOOL bDoInterpolation);
	void				MovePosition(VECTOR3* pv3Pos);
//	void				GetPosition(VECTOR3* pv3Pos) {*pv3Pos = *(VECTOR3*)&m_matTransform._41;}
	void				GetPosition(VECTOR3* pv3Pos) {*pv3Pos = m_v3Pos;}
//	void				GetInterpolatedPosition( VECTOR3* pv3Pos){ *pv3Pos = m_v3CurrentRenderPos;}
	void				GetInterpolatedPosition( VECTOR3* pv3Pos);
	void				ResetPosition(CGXMap* pMap);
	
	void				MoveForward(float fDistance);
	void				EstimatedMoveForward(VECTOR3* pv3Pos,float fDistance);
	
	void				SetScale(VECTOR3* pv3Scale);
	void				GetScale(VECTOR3* pv3Scale);
	void				SetDirection(VECTOR3* pv3Axis,float fRad);
	void				SetDirectionFPSStyle(VECTOR3* pv3Angles);
	void				ChangeDirection(float fRad);
	void				GetDirection(VECTOR3* pv3Axis,float* pfRad)
	{
		*pv3Axis = m_v3Axis; *pfRad = m_fRad;
	} 	
	void				GetDirectionFPSStyle(VECTOR3* pv3Angle /* out */) 
	{
		*pv3Angle = m_v3Angle; 
	}
	
	BOOL				GetMotionDesc(MOTION_DESC* pMotionDesc,DWORD dwMotionIndex,DWORD dwModelIndex)	
	{
		return m_ModelList[dwModelIndex].pModel->GetMotionDesc(pMotionDesc,dwMotionIndex,m_ModelList[dwModelIndex].dwRefIndex);
	}
	DWORD				GetMotionNum(DWORD dwModelIndex)	{return m_ModelList[dwModelIndex].pModel->GetMotionNum(m_ModelList[dwModelIndex].dwRefIndex);}
	DWORD				GetMaterialNum(DWORD dwModelIndex)	{return m_ModelList[dwModelIndex].pModel->GetMaterialNum(m_ModelList[dwModelIndex].dwRefIndex);}
	
	void				SetAlphaFlag(DWORD dwFlag);

	BOOL				RequestAttach(CoGXObject* pFromObj,DWORD dwModelIndex,char* szObjName);
	BOOL				RequestDetach(CoGXObject* pFromObj);
	BOOL				PostAttach(CoGXObject* pFromObj,MATRIX4* pMat);
	BOOL				PostDetach(CoGXObject* pFromObj);
	BOOL				DetachAllGXObjects();
	
//	BOOL				AttachCamera(IDICameraObject* pCamera,char* szCameraName,char*szCameraTargetName);
//	void				DetachCamera();
	BOOL				AttachLight(CoGXLight* pLight,char* szObjName,VECTOR3* pv3Offset,DWORD dwAttachType);
	BOOL				DetachLight(CoGXLight* pLight);
	BOOL				DetachAllLights();
	BOOL				RenderByParent(MATRIX4* pParentMatrix);	
	BOOL				RenderByParentDirectModel(MATRIX4* pParentMatrix);
	void				DefaultProc();
	
	BOOL				Replace(DWORD dwModelIndex,char* szFileName);
	BOOL				RenderToViewport(DWORD dwViewport);

	GXSchedulePROC		GetProcedure(){ return m_pProc;}		
	void				UnRegistFileNameHash();

	DWORD				Release();
	void				Unload();
	BOOL				GetWorldMatrixPerObject(MATRIX4* pMatWorld,DWORD dwModelIndex,DWORD dwObjIndex);
	DWORD				GetObjectIndex(char* szObjName,DWORD dwModelIndex);
	
	void				DisableUnloadPreLoaded();
	void				EnableUnloadPreLoaded();
	void				SetAxisAlignOK(BOOL bResult);



	
	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc();
	DWORD						__stdcall	OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2);
	DWORD						__stdcall	GetObjectIndexInModel();
	BOOL						__stdcall	Render();
	DWORD						__stdcall	CreateIVertexList(IVERTEX** ppVertex);
	void						__stdcall	ReleaseIVertexList(IVERTEX*	pVertex);
	BOOL						__stdcall	ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc);
	BOOL						__stdcall	IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag);

	///////////////////////////////////////////////////////////////////////////////////////

	// 충돌처리 함수군. 2002/03/06
	void						SetBoundingVolume( BOUNDING_VOLUME* pBV);
	BOUNDING_VOLUME*			GetBoundingVolume( void ){return &m_BV;}
	void						TryMoveTo( COLLISION_TEST_RESULT* pResult, VECTOR3* pTo, DWORD dwBufferIndex);	// 마지막의 dwBufferIndex는 모르면 0 -_-;
	BOOL						IsLanding( void);
//	void						SetApplyVelocityFlag(BOOL bApply){ m_bApplyVelocity = bApply; }

	void						MovePositionWithCollide(VECTOR3* pv3Pos);
	
	void						BeginIllusionEffect();
	void						EndIllusionEffect();
	DWORD						GetAttachedGXObjects(GXOBJECT_HANDLE* pGXOList,DWORD dwMaxNum);

	CoGXObject*					m_pPrvObject;		// 같은 파일리소스를 사용하는 이전 오브젝트 
	CoGXObject*					m_pNextObject;		// 같은 파일리소스를 사용하는 다음 오브젝트 

};
