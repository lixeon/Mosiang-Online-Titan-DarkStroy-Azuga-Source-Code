// SocietyActManager.h: interface for the CSocietyActManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCIETYACTMANAGER_H__85B89DE0_CA7A_4E5C_AE17_FF112DC74450__INCLUDED_)
#define AFX_SOCIETYACTMANAGER_H__85B89DE0_CA7A_4E5C_AE17_FF112DC74450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SOCIETYACTMGR USINGTON(CSocietyActManager)

class CSocietyActManager  
{
public:
	CSocietyActManager();
	virtual ~CSocietyActManager();

	void SendSocietyActMsg( WORD wActionIdx );
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
};


EXTERNGLOBALTON(CSocietyActManager)

#endif // !defined(AFX_SOCIETYACTMANAGER_H__85B89DE0_CA7A_4E5C_AE17_FF112DC74450__INCLUDED_)
