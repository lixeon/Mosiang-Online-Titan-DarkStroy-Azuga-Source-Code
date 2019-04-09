#pragma once

#include "GXStaticMapObject.h"
#include "GXHFieldMapObject.h"
#include "../4DyuchiGRX_myself97/MClipper.h"

class CoGXObject;
class CoGXLight;
class CoGXEventTrigger;

class CClipper;
class CoExecutive;


class CGXMap
{
	MClipper*				m_pClipper;


	CoGXObject**			m_ppInitialGXObjectList;
	DWORD					m_dwInitialGXObjectNum;
	DWORD					m_dwMaxInitialGXObectNum;

	CoGXLight**				m_ppInitialGXLightList;
	DWORD					m_dwInitialGXLightNum;
	DWORD					m_dwMaxInitialGXLightNum;

	CoGXEventTrigger**		m_ppInitialGXTriggerList;
	DWORD					m_dwInitialGXTriggerNum;
	DWORD					m_dwMaxInitialGXTriggerNum;

	CoExecutive*			m_pExecutive;
	I3DStaticModel*			m_pBaseModel;
	IHeightField*			m_pHField;

	void*					m_pData;
	
	CGXMapObject**			m_ppStaticGXMapObjList;
	DWORD					m_dwStaticGXMapObjectNum;
	DWORD					m_dwStaticMaxGXMapObjectNum;

	DWORD					m_dwBuildTreeFlag;

	
	CGXStaticMapObject*		m_pSTMGXMapObjList;
	DWORD					m_dwSTMGXMapObjNum;

	CGXHFieldMapObject*		m_pGXHFieldMapObjList;
	DWORD					m_dwGXHFieldMapObjNum;
	MAABB					m_aabbWorld;
	DWORD					m_dwAlphaFlag;

	DWORD					InsertStaticGXMapObject(CGXMapObject* pObj);
public:
	void					DeleteInitialGXObject(DWORD dwInitialiIndex);
	void					DeleteInitialGXLight(DWORD dwInitialiIndex);
	void					DeleteInitialGXTrigger(DWORD dwInitialiIndex);

	void					SetHFieldAlphaFlag(DWORD dwAlpha);
	void					SetSTMAlphaFlag(DWORD dwAlpha);


	DWORD					GetAllInitialGXObjects(GXOBJECT_HANDLE* pObjArray,DWORD dwMaxNum);
	BOOL					ShadeMap(DWORD dwAmbientColor,LIGHT_DESC* pLight,DWORD dwLightNum,DWORD dwFlag,SHADE_FUNC pFunc);
	BOOL					BeginBuildMap(DWORD dwFlag);
	BOOL					EndBuildMap(float fTop,float fBottom);
	void					RebuildMap(float fTop,float fBottom);

	MClipper*				GetClipper() {return m_pClipper;}
	void					GetWorldBoundingBox(MAABB* pAABB);
	
	I3DStaticModel*			GetBaseModel()			{return m_pBaseModel;}
	IHeightField*			GetHField()				{return m_pHField;}
	BOOL					Initialize(CoExecutive* pExecutive,GXMapSchedulePROC pProc,DWORD dwMaxObjectNum);

	BOOL					CreateInitialGXObjectList(DWORD dwNum);
	BOOL					InsertInitialGXObject(CoGXObject* pGXO);

	BOOL					CreateInitialGXLightList(DWORD dwNum);
	BOOL					InsertInitialGXLight(CoGXLight* pGXL);

	BOOL					CreateInitialGXTriggerList(DWORD dwNum);
	BOOL					InsertInitialGXTrigger(CoGXEventTrigger* pGXT);
	
	void					DeleteAllInitialObjects();

	BOOL					InsertSTM(I3DStaticModel* pStaticModel, DWORD BuildFlag);
	BOOL					InsertHField(IHeightField* pHField);
	
	DWORD					GetStaticMapObjectNum()			{return m_dwStaticGXMapObjectNum;}
	CGXMapObject*			GetStaticMapObject(DWORD dwSeqIndex) {return m_ppStaticGXMapObjList[dwSeqIndex];}

	DWORD					GetSTMMapObjectNum(VOID)			{ return m_dwSTMGXMapObjNum; }
	CGXStaticMapObject*		GetSTMMapObject(DWORD dwSeqIndex)	{ return &m_pSTMGXMapObjList[dwSeqIndex]; } 


	void*					GetData()					{return m_pData;}
	void					SetData(void* pData)		{m_pData = pData;}

	// 클리퍼를 캡슐화한 함수들

	DWORD	AddStaticObject( CGXMapObject* pObject)		{return m_pClipper->AddStaticObject(pObject);}
	DWORD	BuildTree( DWORD dwMode, MAABB WorldBox, DWORD dwMaxLevel)	{return m_pClipper->BuildTree(dwMode,WorldBox,dwMaxLevel);}

	void	GetClipperWorldBox( MAABB* pWorldBox)		{m_pClipper->GetWorldBox(pWorldBox);}
	

	// dynamic object 함수군.
	DWORD	AddDynamicObject( CGXMapObject* pObject);
	BOOL	UpdateDynamicObject( DWORD dwIndex)			{return m_pClipper->UpdateDynamicObject(dwIndex);}
	BOOL	DeleteDynamicObject( DWORD dwIndex)			{return m_pClipper->DeleteDynamicObject(dwIndex);}
	

	// 데칼 함수군.
	DWORD	AddDecal( CGXDecal* pDecal)	{return m_pClipper->AddDecal(pDecal);}
	BOOL	UpdateDecal( DWORD dwIndex)	{return m_pClipper->UpdateDecal(dwIndex);}
	BOOL	DeleteDecal( DWORD dwIndex)	{return m_pClipper->DeleteDecal(dwIndex);}

	
	// 랜더링 후보 찾기.
	void		FindRenderCandidate( VIEW_VOLUME &vv)	{m_pClipper->FindRenderCandidate(vv);}
	DWORD		GetRenderTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)	{return m_pClipper->GetRenderTargetBuffer(pppBuff,dwID);}

	// 근처꺼 찾기 
	void		FindNearObject( BOUNDING_SPHERE& bs)				{m_pClipper->FindNearObject(bs);}
	void		FindNearObjectWithCapsule( BOUNDING_CAPSULE& bc)		{m_pClipper->FindNearObjectWithCapsule(bc);}
	DWORD		GetTargetBuffer( CGXMapObject*** pppBuff, DWORD dwID)	{return m_pClipper->GetTargetBuffer(pppBuff,dwID);}

	DWORD		GetMeshObjects(CGXMapObject** ppBuffer,DWORD dwMaxNum)	{return m_pClipper->GetMeshObjects(ppBuffer,dwMaxNum);}	

	// rebuildTree
	void		Rebuild(float fTop,float fBottom)						{m_pClipper->Rebuild(fTop,fBottom);}

	// 모두 지우기.
	void		DeleteAll(void)		{m_pClipper->DeleteAll();}
	CGXMap();
	~CGXMap();
};
