// UngijosikManager.h: interface for the CUngijosikManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNGIJOSIKMANAGER_H__682EA8ED_F5C5_4010_B531_3E4C5B7FA9BC__INCLUDED_)
#define AFX_UNGIJOSIKMANAGER_H__682EA8ED_F5C5_4010_B531_3E4C5B7FA9BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define UNGIMGR CUngijosikManager::GetInstance()

class CPlayer;

class CUngijosikManager  
{
public:
	MAKESINGLETON(CUngijosikManager);
	CUngijosikManager();
	virtual ~CUngijosikManager();

	void UngijosikMode(CPlayer* pPlayer, BOOL bUngi);

};

#endif // !defined(AFX_UNGIJOSIKMANAGER_H__682EA8ED_F5C5_4010_B531_3E4C5B7FA9BC__INCLUDED_)
