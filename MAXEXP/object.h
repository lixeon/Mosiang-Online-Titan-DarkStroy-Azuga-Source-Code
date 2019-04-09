#pragma once

#include "../max_common/stdafx.h"
#include "face_group.h"

struct FILE_BASE_OBJECT_HEADER
{
	DWORD				dwIndex;
	NODE_TM				TM;
	DWORD				dwChildObjectNum;
	DWORD				dwParentObjecIndex;
	TCHAR				uszObjName[MAX_NAME_LEN];	
};

class CBaseObject
{
	OBJECT_TYPE			m_objType;
	DWORD				m_dwIndex;
	NODE_TM				m_TM;
	DWORD				m_dwChildObjectNum;
	DWORD				m_dwParentObjectIndex;
	TCHAR				m_uszObjName[MAX_NAME_LEN];	

	DWORD				m_dwMatrixNum;
	MATRIX4				m_matResult;
	MATRIX4**			m_ppMatrixList;
	CBaseObject*		m_pParentObject;
	DWORD*				m_pChildObjectIndexList;
	TCHAR				m_uszParentObjectName[MAX_NAME_LEN];

	DWORD				m_dwObjectNameSize;
	

public:
	virtual	DWORD		WriteFile(FILE* fp);
	
	BOOL				InsertChildObjectIndex(DWORD dwIndex);
	void				AddChildObjectCount()		{m_dwChildObjectNum++;}

	BOOL				CreateChildObjectList();
	OBJECT_TYPE			GetObjectType() {return m_objType;}
	void				SetObjectType(OBJECT_TYPE type) {m_objType = type;}

	NODE_TM*			GetTM()	{return &m_TM;}
	void				SetTM(NODE_TM* tm) {m_TM = *tm;}

	void				SetParentObject(CBaseObject* obj) { m_pParentObject = obj;}
	CBaseObject*		GetParentObject() {return m_pParentObject;}

	
	void				SetObjectName(TCHAR* name) {_tcscpy(m_uszObjName,name);m_dwObjectNameSize = _tcslen(name);}
	void				SetParentObjectIndex(DWORD dwIndex) {m_dwParentObjectIndex = dwIndex;}
	void				SetParentObjectName(TCHAR* uszObjName) {_tcscpy(m_uszParentObjectName,uszObjName);}

	TCHAR*				GetParentObjectName() {return m_uszParentObjectName;}
	TCHAR*				GetObjectName() {return m_uszObjName;}
	DWORD				GetObjectNameSize() {return m_dwObjectNameSize;}

	OBJECT_TYPE			GetType() {return m_objType;}
	void				SetType(OBJECT_TYPE type) {m_objType = type;}
	void				SetIndex(DWORD i) {m_dwIndex = i;}
	DWORD				GetIndex() {return m_dwIndex;}

	CBaseObject*		m_pNextObject;

	CBaseObject();
	virtual ~CBaseObject();

};
