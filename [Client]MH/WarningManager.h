// WarningManager.h: interface for the CWarningManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WARNINGMANAGER_H__5B659AF7_6505_488D_B7E4_ADDBBDE9E7E0__INCLUDED_)
#define AFX_WARNINGMANAGER_H__5B659AF7_6505_488D_B7E4_ADDBBDE9E7E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WARNINGMGR USINGTON(CWarningManager)

class CWarningManager  
{
	BOOL IsWarning(WORD CurLife,WORD MaxLife);
	
	void OnStartWarning();
	void OnEndWarning();
public:
	//MAKESINGLETON(CWarningManager);

	CWarningManager();
	virtual ~CWarningManager();

	void CheckWarning(WORD BeforeLife,WORD AfterLife,WORD MaxLife);
};
EXTERNGLOBALTON(CWarningManager);
#endif // !defined(AFX_WARNINGMANAGER_H__5B659AF7_6505_488D_B7E4_ADDBBDE9E7E0__INCLUDED_)
