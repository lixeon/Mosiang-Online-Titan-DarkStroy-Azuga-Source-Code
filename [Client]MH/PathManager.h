// PathManager.h: interface for the CPathManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHMANAGER_H__3841CC50_0D4C_4C17_ABE3_C36956811AD8__INCLUDED_)
#define AFX_PATHMANAGER_H__3841CC50_0D4C_4C17_ABE3_C36956811AD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PATHMGR USINGTON(CPathManager)
enum
{
	PATH_SUCCESS		= 0,
	PATH_FAILED			= 1,
	PATH_BLOCKFAILED	= 2,
};
class CSTRPath;
class CMHMap;
class CPathManager  
{
	CMHMap * m_pMap;
	MAPTYPE m_MapType;
	CSTRPath * m_pSTRPath;
public:
	GETINSTANCE(CPathManager)
	CPathManager();
	virtual ~CPathManager();
	void SetMap(CMHMap * pMap, MAPTYPE mapNum, int width);
	static BOOL ValidFunc(int cellX, int cellY,CObject* pObject);
	int GetPath(VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointPos, WORD wBufCount, BYTE& wWayPointNum, CObject* pObject, BOOL bReverse,  WORD wDepth=50);
	
};
EXTERNGLOBALTON(CPathManager);
#endif // !defined(AFX_PATHMANAGER_H__3841CC50_0D4C_4C17_ABE3_C36956811AD8__INCLUDED_)
