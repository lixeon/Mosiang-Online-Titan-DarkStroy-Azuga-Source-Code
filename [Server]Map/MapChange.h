// MapChange.h: interface for the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCHANGE_H__E9ACFD39_12AF_404F_9894_AB9D5256F92C__INCLUDED_)
#define AFX_MAPCHANGE_H__E9ACFD39_12AF_404F_9894_AB9D5256F92C__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// palpal

#define MAPCHANGE_OBJ CMapChange::GetInstance()

class CMapChange  
{
private:
	
	
public:
	GETINSTANCE(CMapChange)
	CMapChange();
	virtual ~CMapChange();

	MAPCHANGE_INFO* GetMapChangeArea(WORD MapNum);
	LOGINPOINT_INFO* GetLoginPoint(WORD MapNum);
	void GetLoginPointPosition(WORD MapNum,VECTOR3* pRtPos);

};

#endif // !defined(AFX_MAPCHANGE_H__E9ACFD39_12AF_404F_9894_AB9D5256F92C__INCLUDED_)
