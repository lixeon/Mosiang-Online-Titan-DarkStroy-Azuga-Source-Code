// ExitManager.h: interface for the CExitManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXITMANAGER_H__7A240F06_645B_4B70_818E_DD5468AAB59C__INCLUDED_)
#define AFX_EXITMANAGER_H__7A240F06_645B_4B70_818E_DD5468AAB59C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define EXITMGR USINGTON(CExitManager)


enum eEXIT_KIND
{
	eEK_GAMEEXIT,
	eEK_CHARSEL,
};

class CExitManager  
{

protected:
	BOOL	m_bExitProcessing;
	DWORD	m_dwStartTime;
	int		m_nExitCount;
	int		m_nCount;

	int		m_nExitKind;
	BOOL	m_bExitSync;

public:
	CExitManager();
	virtual ~CExitManager();

	int CanExit();
	BOOL SendExitMsg( int nExitKind );
	void StartExit();
	void RejectExit( int nExitCode );
	void Exit();
	void CancelExit( int nErrorCode );
	void Process();
	BOOL IsExitWaiting() { return m_bExitSync; }
};

EXTERNGLOBALTON(CExitManager)

#endif // !defined(AFX_EXITMANAGER_H__7A240F06_645B_4B70_818E_DD5468AAB59C__INCLUDED_)
