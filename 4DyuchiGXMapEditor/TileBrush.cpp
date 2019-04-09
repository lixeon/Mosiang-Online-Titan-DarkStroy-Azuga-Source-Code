#include "TileBrush.h"
CTileBrush::CTileBrush()
{
	memset(this,0,sizeof(CTileBrush));
}
BOOL CTileBrush::CreateBrush(char* szBrushName,HFIELD_POS* pTileList,DWORD dwTileNum,WORD* pwTexTileIndexTable,DWORD dwPitch)
{
	lstrcpy(m_szName,szBrushName);
	DWORD dwMinX,dwMinZ,dwMaxX,dwMaxZ;
	dwMinX = 0xffffffff;
	dwMinZ = 0xffffffff;
	dwMaxX = 0;
	dwMaxZ = 0;

	m_dwTileNum = dwTileNum;
	for (DWORD i=0; i<dwTileNum; i++)
	{
		if (dwMinX > pTileList[i].dwX)
			dwMinX = pTileList[i].dwX;

		if (dwMinZ > pTileList[i].dwZ)
			dwMinZ = pTileList[i].dwZ;

		if (dwMaxX < pTileList[i].dwX)
			dwMaxX = pTileList[i].dwX;

		if (dwMaxZ < pTileList[i].dwZ)
			dwMaxZ = pTileList[i].dwZ;
	}
	m_dwWidth = dwMaxX - dwMinX + 1;
	m_dwHeight = dwMaxZ - dwMinZ + 1;

	m_pTileList = new HFIELD_POS[dwTileNum];
	m_pwTexTileIndexList = new WORD[dwTileNum];

	for (i=0; i<dwTileNum; i++)
	{
		m_pTileList[i].dwX = pTileList[i].dwX - dwMinX;
		m_pTileList[i].dwZ = pTileList[i].dwZ - dwMinZ;
		m_pwTexTileIndexList[i] = pwTexTileIndexTable[pTileList[i].dwX + pTileList[i].dwZ*dwPitch];
	}
	return TRUE;;
}

DWORD CTileBrush::GetTileList(HFIELD_POS** ppTileList,WORD** ppwTexTileIndexList)
{
	*ppTileList = m_pTileList;
	*ppwTexTileIndexList = m_pwTexTileIndexList;
	return m_dwTileNum;

}
BOOL CTileBrush::Save(FILE* fp)
{
	// Å¸ÀÏ°¹¼ö
	fwrite(m_szName,sizeof(char),TILE_BRUSH_NAME_LEN,fp);
	fwrite(&m_dwTileNum,sizeof(DWORD),1,fp);
	fwrite(m_pTileList,sizeof(HFIELD_POS),m_dwTileNum,fp);
	fwrite(m_pwTexTileIndexList,sizeof(WORD),m_dwTileNum,fp);

	return TRUE;
}
BOOL CTileBrush::Load(FILE* fp)
{
	memset(m_szName,0,sizeof(m_szName));
	fread(m_szName,sizeof(char),TILE_BRUSH_NAME_LEN,fp);
	fread(&m_dwTileNum,sizeof(DWORD),1,fp);

	m_pTileList = new HFIELD_POS[m_dwTileNum];
	m_pwTexTileIndexList = new WORD[m_dwTileNum];

	fread(m_pTileList,sizeof(HFIELD_POS),m_dwTileNum,fp);
	fread(m_pwTexTileIndexList,sizeof(WORD),m_dwTileNum,fp);

	return TRUE;
}
CTileBrush::~CTileBrush()
{
	if (m_pwTexTileIndexList)
	{
		delete [] m_pwTexTileIndexList;
		m_pwTexTileIndexList = NULL;
	}
	if (m_pTileList)
	{
		delete [] m_pTileList;
		m_pTileList = NULL;
	}
}