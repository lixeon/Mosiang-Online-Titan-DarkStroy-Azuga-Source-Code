// MugongInfoManager.h: interface for the CMugongInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGINFOMANAGER_H__AB7B489F_B93B_48E2_BCE6_4DE23D60F9BB__INCLUDED_)
#define AFX_MUGONGINFOMANAGER_H__AB7B489F_B93B_48E2_BCE6_4DE23D60F9BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MugongInfo.h"

struct MUGONG_INFO
{
	WORD MugongIdx;
};

class CMugongInfoManager  
{
	CYHHashTable<CMugongInfo> m_MugongInfoTable;

public:
	CMugongInfoManager();
	virtual ~CMugongInfoManager();
	
	BOOL AddMugongInfo(WORD MugongIdx,WORD MugongKind);

};

#endif // !defined(AFX_MUGONGINFOMANAGER_H__AB7B489F_B93B_48E2_BCE6_4DE23D60F9BB__INCLUDED_)
