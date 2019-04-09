// MouseEventThrower.h: interface for the CMouseEventThrower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEEVENTTHROWER_H__CA64761F_AD68_448B_AA54_5394A0467267__INCLUDED_)
#define AFX_MOUSEEVENTTHROWER_H__CA64761F_AD68_448B_AA54_5394A0467267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mouse.h"

class CMouseEventReceiver;

class CMouseEventThrower  
{
	CMouseEventReceiver* m_pDefaultReceiver;
	CMouseEventReceiver* m_pCurrentReceiver;

public:
	CMouseEventThrower();
	virtual ~CMouseEventThrower();

	void SetDefaultReceiver(CMouseEventReceiver* pDefaultReceiver);
	void SetCurrentReceiver(CMouseEventReceiver* pCurrentReceiver);
	void SetReceiverToDefault();

	void Process(CMouse& Mouse);
};

#endif // !defined(AFX_MOUSEEVENTTHROWER_H__CA64761F_AD68_448B_AA54_5394A0467267__INCLUDED_)
