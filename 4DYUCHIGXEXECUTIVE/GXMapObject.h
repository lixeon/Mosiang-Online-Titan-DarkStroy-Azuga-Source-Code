#pragma once

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGRX_Common/IGeometry.h"


class CGXMap;
class CoExecutive;
class CGXMapObject
{
protected:
	GX_MAP_OBJECT_TYPE		m_dwObjectType;
	DWORD					m_dwID;
	DWORD					m_dwClipperIndex;
	DWORD					m_dwScheduleFlag;
	DWORD					m_dwRenderFlag;
	DWORD					m_dwAlphaFlag;
	DWORD					m_dwPropertyFlag;
	CoExecutive*			m_pExecutive;
	
	void*					m_pHashHandle;
	DWORD					m_dwIndex;	
	DWORD					m_dwIndexInInitialTable;
	DWORD					m_dwRenderFrameCount;

	void*					m_pData;
	PICK_TYPE				m_dwPickTypeFlag;

	

public:
	void				SetIndexInIntialTable(DWORD dwIndex) {m_dwIndexInInitialTable = dwIndex;}

	DWORD				GetRenderFrameCount() {return m_dwRenderFrameCount;}
	void				SetRenderFrameCount(DWORD dwFrameCount) {m_dwRenderFrameCount = dwFrameCount;}

	void				Duplcate(CGXMapObject* pDestGXMObj);
	void				SetClipperIndex(DWORD dwIndex)		{m_dwClipperIndex = dwIndex;}
	DWORD				GetClipperIndex()				{return m_dwClipperIndex;}
	
	void				DisableUnloadPreLoaded()	{m_dwPropertyFlag |= GXMAP_OBJECT_COMMON_TYPE_DISABLE_UNLOAD;}
	void				EnableUnloadPreLoaded()		{m_dwPropertyFlag &= (~GXMAP_OBJECT_COMMON_TYPE_DISABLE_UNLOAD);}
	
	void				EnablePick()				{m_dwPropertyFlag &= (~GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE);}
	void				DisablePick()				{m_dwPropertyFlag |= GXMAP_OBJECT_COMMON_TYPE_NOT_PICKABLE;}

	void				LockTransform()				{m_dwPropertyFlag |= GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM;}
	void				UnLockTransform()			{m_dwPropertyFlag &= (~GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM);}
	BOOL				IsLockTransform()			{return (BOOL)(bool)(m_dwPropertyFlag & GXMAP_OBJECT_COMMON_TYPE_LOCK_TRANSFORM);}

	void				DisableRender()				{ m_dwScheduleFlag |= SCHEDULE_FLAG_NOT_RENDER;}
	void				EnableRender()				{ m_dwScheduleFlag &= (~SCHEDULE_FLAG_NOT_RENDER);}
	
	void				DisableSchedule()			{ m_dwScheduleFlag |= SCHEDULE_FLAG_NOT_SCHEDULE;}
	void				EnableSchedule()			{ m_dwScheduleFlag &= (~SCHEDULE_FLAG_NOT_SCHEDULE);}

	void				EnableUpdateShading()		{ m_dwRenderFlag |= RENDER_TYPE_UPDATE_SHADING;}
	void				DisableUpdateShading()		{ m_dwRenderFlag &= (~RENDER_TYPE_UPDATE_SHADING);}
	
	void				EnableSendShadow()			{ m_dwRenderFlag |= RENDER_TYPE_SEND_SHADOW;}
	void				DisableSendShadow()			{ m_dwRenderFlag &= (~RENDER_TYPE_SEND_SHADOW);}

	void				SetScheduleFlag(DWORD dwFlag)	{m_dwScheduleFlag = dwFlag;}
	DWORD				GetScheduleFlag()			{ return m_dwScheduleFlag;}
	BOOL				IsRenderable();	

	void				SetRenderFlag(DWORD dwFlag) {m_dwRenderFlag = dwFlag;}
	DWORD				GetRenderFlag()				{return m_dwRenderFlag;}
	
	void				SetAlphaFlag(DWORD dwFlag) {m_dwAlphaFlag = dwFlag;}
	DWORD				GetAlphaFlag()				{return m_dwAlphaFlag;}
	
	void				SetPickType(PICK_TYPE type) {m_dwPickTypeFlag = type;}
	PICK_TYPE			GetPickType() {	return m_dwPickTypeFlag; }

	GX_MAP_OBJECT_TYPE	GetObjectType() {return m_dwObjectType;}
	void*				GetData()					{return m_pData;}
	void				SetData(void* pData)		{m_pData = pData;}
	DWORD				GetID()						{return m_dwID;}
	BOOL				SetID(DWORD dwID);
	void				ReleaseID();
	void				SetIndex(DWORD dwIndex)				{m_dwIndex = dwIndex;}
	DWORD				GetIndex()							{return m_dwIndex;}
		
	DWORD				GetPropertyFlag()			{return m_dwPropertyFlag;}
	void				SetPropertyFlag(DWORD dwFlag)	{m_dwPropertyFlag = dwFlag;}
	


	void				Cleanup();
	


	
	virtual	COLLISION_MESH_OBJECT_DESC*	__stdcall	GetCollisionMeshObjectDesc() = 0;
	virtual DWORD						__stdcall	OnFrame(I4DyuchiGXExecutive* pExecutive,DWORD msg,int arg1,int arg2) = 0;
	virtual DWORD						__stdcall	GetObjectIndexInModel() = 0;
	virtual BOOL						__stdcall	Render() = 0;
	virtual	BOOL						__stdcall	IsCollisionWithScreenCoord(VECTOR3* pv3IntersectPoint,DWORD* pdwModelIndex,DWORD* pdwObjIndex,float* pfDist,POINT* ptCursor,DWORD dwViewportIndex,DWORD dwFlag) = 0;
	virtual DWORD						__stdcall	CreateIVertexList(IVERTEX** ppVertex) = 0;
	virtual void						__stdcall	ReleaseIVertexList(IVERTEX*	pVertex) = 0;
	virtual BOOL						__stdcall	ShadeLightMapObject(VECTOR3* pv3,DWORD dwFacesNum,LIGHT_DESC* pLight,SHADE_FUNC pFunc) = 0;


};
