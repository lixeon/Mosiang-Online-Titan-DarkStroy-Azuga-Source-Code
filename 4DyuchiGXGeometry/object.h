#pragma once

#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGRX_common/IGeometry.h"
#include "face_group.h"
#include "motion_obj.h"

struct FILE_BASE_OBJECT_HEADER
{
	DWORD				dwIndex;
	NODE_TM				TM;
	DWORD				dwChildObjectNum;
	DWORD				dwParentObjectIndex;
	char				szObjName[MAX_NAME_LEN];	
};

class CoModel;
class CoGeometry;

class CBaseObject : public IGeometryController
{
protected:
	OBJECT_TYPE			m_objType;
	DWORD				m_dwIndex;
	NODE_TM				m_TM;
	DWORD				m_dwChildObjectNum;
	DWORD				m_dwParentObjectIndex;
	char				m_szObjName[MAX_NAME_LEN];	

	I4DyuchiGXRenderer*	m_pRenderer;
	CoModel*			m_pModel;
	CoGeometry*			m_pGeometry;

	CBaseObject*		m_pParentObject;
	CBaseObject**		m_ppChildObjectList;
	DWORD*				m_pChildObjectIndexList;
	DWORD				m_dwNameLen;
	
	MATRIX4*			m_pDefaultResultMatrix;
public:
	void				SetModelPtr(CoModel* pModel) {m_pModel = pModel;}
	void				ResetDefaultResultMatrix();
	void				SetResultMatrix(MATRIX4* pMat) {*m_pDefaultResultMatrix = *pMat;}
	void				ResetResultMatrix() {*m_pDefaultResultMatrix = m_TM.mat4;}
	void				SetResultMatrixPtr(MATRIX4* pResult) { m_pDefaultResultMatrix = pResult;}
	MATRIX4*			GetResultMatrix() {return m_pDefaultResultMatrix;}

	DWORD				GetChildObjectNum() {return m_dwChildObjectNum;}
	DWORD				GetChildObjectIndex(DWORD dwIndex) {return m_pChildObjectIndexList[dwIndex];}
	BOOL				SetFrame(MATRIX4* pResult,MATRIX4* pMatrixEntry,DWORD dwRefIndex,DWORD dwFrame,DWORD dwMotionIndex);
			
	MATRIX4*			GetWorldMatrix() {return &m_TM.mat4;}

	virtual	DWORD		ReadFile(void* pFP,DWORD dwVersion);
	virtual DWORD		WriteFile(FILE* fp);


	OBJECT_TYPE			GetObjectType() {return m_objType;}
	void				SetObjectType(OBJECT_TYPE type) {m_objType = type;}

	NODE_TM*			GetTM()	{return &m_TM;}
	CBaseObject*		GetParentObject() {return m_pParentObject;}
	void				SetParentObject(CBaseObject* pObj) {m_pParentObject = pObj;}

	DWORD				GetParentObjectIndex() {return m_dwParentObjectIndex;}
	BOOL				Duplicate(CBaseObject* pBaseObject);

	
	char*				GetObjectName() {return m_szObjName;}
	OBJECT_TYPE			GetType() {return m_objType;}
	void				SetType(OBJECT_TYPE type) {m_objType = type;}
	
	void				SetIndex(DWORD i) {m_dwIndex = i;}
	DWORD				GetIndex() {return m_dwIndex;}

	BOOL	__stdcall	ApplyResultMatrix(MATRIX4** ppResult,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyPhysique(BYTE* pVertex,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyMorph(VECTOR3** ppv3Local,TVERTEX** pptv,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyView(MATRIX4* pView,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyVertexNormal(BYTE* pVertex,DWORD dwVertexStartOffset,DWORD dwVertexSize,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyMotionStatus(MOTION_STATUS* pMotionStatus,DWORD dwRefIndex,DWORD dwFlag);
	BOOL	__stdcall	ApplyWorldMatrixForPhysique(MATRIX4* pResultWorldMatrix,DWORD dwRefIndex,DWORD dwFlag);

	CBaseObject();
	virtual ~CBaseObject();

};
