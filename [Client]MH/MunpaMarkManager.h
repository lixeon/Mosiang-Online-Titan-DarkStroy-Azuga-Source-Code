// MunpaMarkManager.h: interface for the CMunpaMarkManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNPAMARKMANAGER_H__E7BB1972_18B4_42D9_9892_873BBCDA441E__INCLUDED_)
#define AFX_MUNPAMARKMANAGER_H__E7BB1972_18B4_42D9_9892_873BBCDA441E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MunpaMark.h"

#define MUNPAMARKMGR USINGTON(CMunpaMarkManager)

struct MUNPAMARKDESC
{
	int MarkNum;
	char MarkFileName[64];
	CMunpaMark Mark;
};

struct MUNPAMARKNUM
{
	DWORD MunpaID;
	int MarkNum;
};

class CMunpaMarkManager  
{
	CYHHashTable<MUNPAMARKDESC> m_DescTable;
	CYHHashTable<MUNPAMARKNUM> m_MunpaMarkNumTable;

	BOOL m_bIsInited;
	
	CMunpaMark* GetMunpaMark(int MarkNum);

	
public:
	CMunpaMarkManager();
	////MAKESINGLETON(CMunpaMarkManager);
	virtual ~CMunpaMarkManager();

	BOOL IsInited();

	void Init();
	BOOL LoadMunpaMarkNum();
	void Release();
	CMunpaMark* GetMunpaMark(DWORD MunpaID);
};
EXTERNGLOBALTON(CMunpaMarkManager)
#endif // !defined(AFX_MUNPAMARKMANAGER_H__E7BB1972_18B4_42D9_9892_873BBCDA441E__INCLUDED_)
