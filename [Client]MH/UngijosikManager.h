// UngijosikManager.h: interface for the CUngijosikManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNGIJOSIKMANAGER_H__BA48D7A5_9BC0_4111_A3B8_A67BA8266139__INCLUDED_)
#define AFX_UNGIJOSIKMANAGER_H__BA48D7A5_9BC0_4111_A3B8_A67BA8266139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  UNGIMGR	USINGTON(CUngijosikManager)
class CUngijosikManager  
{
public:
	//MAKESINGLETON(CUngijosikManager);
	CUngijosikManager();
	virtual ~CUngijosikManager();

	void ToggleHero_UngiMode();
	void Toggle_UngiMode(CObject* pObject, BOOL bUngi);
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

};
EXTERNGLOBALTON(CUngijosikManager);
#endif // !defined(AFX_UNGIJOSIKMANAGER_H__BA48D7A5_9BC0_4111_A3B8_A67BA8266139__INCLUDED_)
