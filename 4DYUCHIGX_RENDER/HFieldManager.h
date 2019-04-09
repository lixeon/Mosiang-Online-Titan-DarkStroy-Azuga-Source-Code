#ifndef HFIELD_MANAGER_H
#define HFIELD_MANAGER_H

#include "texture_manager.h"

class CHFieldObject;
class CoD3DDevice;

class CIndexBuffer;
class CStaticVBPool;
class CStaticIBPool;
class CIBHeap;

struct DETAIL_LEVEL_INDICES_DESC
{
	DWORD		dwIndicesNumPerObj;
	DWORD		dwMaxObjNumNum;
	DWORD		dwMaxTotalIndicesNum;
};

class CHFieldManager : public IDIHeightField
{
	DWORD							m_dwRefCount;
	CStaticVBPool*					m_pStaticVBPool;			
	CIBHeap*						m_pIBHeap;			
	DWORD							m_dwMipmapLevelNum;
	HFIELD_DESC						m_hfDesc;

	DWORD							m_dwVerticesNumPerObj;
	DWORD							m_dwLv0TriNumPerObj;
	DWORD							m_dwLv0IndicesNumUnit;
	DWORD							m_dwLv0IndicesPitch;

	CoD3DDevice*					m_pRenderer;
	LPDIRECT3DDEVICE8				m_pD3DDevice;



	CTextureManager*				m_pTextureManager;
	
	DETAIL_LEVEL_INDICES_DESC		m_DetailLevelIndicesDesc[8];
	INDEX_BUFFER_DESC				m_IndexTable[MAX_HFIELD_DETAIL_NUM][16];
	CTexture*						m_pTileTexture;
	

	DWORD							m_dwIndexBufferNum;


	DWORD							m_dwMaxIndicesNum;
	DWORD							m_dwUnitIndicesNum;
	DWORD							m_dwMaxViewObjNum;

	BOOL							m_bHFieldTileBlendEnable;
	
	CHFieldObject**					m_ppHFieldObjectList;
	DWORD							m_dwMaxHFieldObjNum;
	DWORD							m_dwHFieldObjNum;

	
public:
	void							OnReset();
	CStaticVBPool*					GetStaticVBPool() {return m_pStaticVBPool;}

	HFIELD_DESC*					GetHFieldDesc()	{return &m_hfDesc;}

	void							SetIndexedTexture(DWORD dwIndex,DWORD dwTexStageIndex,IMAGE_HEADER** ppImgHeader);
	BOOL							AllocIndexBuffer(DWORD* pdwTriNum,CHFieldObject* pHFObj,DWORD dwDetailLevel,DWORD dwPositionMask,INDEX_LIST_DESC* pQuadIndexList);
	
		
	DWORD							GetYFNumX() {return m_hfDesc.dwYFNumX;}
	DWORD							GetYFNumZ() {return m_hfDesc.dwYFNumZ;}
	DWORD							GetVerticesNumPerObj() {return m_hfDesc.dwVerticesNumPerObj;}
	DWORD							GetDetailLevelNum()	{return (DWORD)m_hfDesc.bDetailLevelNum;}
	float							GetFaceSize()		{return m_hfDesc.fFaceSize;}
	float							GetTileSize()		{return m_hfDesc.fTileSize;}
	

	LPDIRECT3DDEVICE8				GetD3DDevice() {return m_pD3DDevice;}
	CoD3DDevice*					GetRenderer() {return m_pRenderer;}
	

	STDMETHODIMP					QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)			AddRef(void);
    STDMETHODIMP_(ULONG)			Release(void);

	BOOL				__stdcall	StartInitialize(HFIELD_DESC* pDesc);
	void				__stdcall	EndInitialize();
	IDIMeshObject*		__stdcall	CreateHeightFieldObject(HFIELD_OBJECT_DESC* pDesc);
	
	BOOL				__stdcall	InitiallizeIndexBufferPool(DWORD dwDetailLevel,DWORD dwIndicesNum,DWORD dwNum);
	BOOL				__stdcall	LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum);
	BOOL				__stdcall	ReplaceTile(char* szFileName,DWORD dwTexIndex);


	BOOL				__stdcall	CreateIndexBuffer(DWORD dwIndicesNum,DWORD dwDetailLevel,DWORD dwPositionMask,DWORD dwNum);
	BOOL				__stdcall	LockIndexBufferPtr(WORD** ppWord,DWORD dwDetailLevel,DWORD dwPositionMask);
	void				__stdcall	UnlcokIndexBufferPtr(DWORD dwDetailLevel,DWORD dwPositionMask);
	BOOL				__stdcall	RenderGrid(VECTOR3* pv3Quad,DWORD dwTexTileIndex,DWORD dwAlpha);
	void				__stdcall	SetHFieldTileBlend(BOOL bSwitch);
	BOOL				__stdcall	IsEnableHFieldTileBlend();

	CHFieldManager(CoD3DDevice* pDevice);
	~CHFieldManager();

	SORT_LINK						m_linkInRenderer;
};

#endif

