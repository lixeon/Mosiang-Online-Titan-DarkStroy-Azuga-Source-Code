#pragma once

#include "../4DyuchiGRX_Common/IGeometry.h"


#define TILE_BRUSH_NAME_LEN 128
struct TILE_BRUSH_PROPERTY
{
	char	szName[TILE_BRUSH_NAME_LEN];

};

class CTileBrush
{
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	DWORD					m_dwTileNum;
	HFIELD_POS*				m_pTileList;
	WORD*					m_pwTexTileIndexList;
	char					m_szName[TILE_BRUSH_NAME_LEN];
	
	int						m_iListBoxIndex;

public:
	BOOL			CreateBrush(char* szBrushName,HFIELD_POS* pTileList,DWORD dwTileNum,WORD* pwTexTileIndexTable,DWORD dwPitch);
//	void			SetListBoxIndex(int iIndex) {m_iListBoxIndex = iIndex;}
//	int				GetListBoxIndex() {return m_iListBoxIndex;}
	char*			GetName() {return m_szName;}
	DWORD			GetTileList(HFIELD_POS** ppTileList,WORD** ppwTexTileIndexList);
	BOOL			Save(FILE* fp);
	BOOL			Load(FILE* fp);
	CTileBrush();
	~CTileBrush();

};