// MsgRouter.h: interface for the CMsgRouter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGROUTER_H__6C2248BA_291A_45DF_BE3D_E1CF30BF6B01__INCLUDED_)
#define AFX_MSGROUTER_H__6C2248BA_291A_45DF_BE3D_E1CF30BF6B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MemoryPoolTempl.h>
#include "cLooseLinkedList.h"

class MSG_OBJECT
{
public:
	WORD	msgID;
	DWORD	src;
	DWORD	dest;
	DWORD	start;
	DWORD	endDelay;
	DWORD	startDelay;
	DWORD	dwData;
};

class CMsgRouter  
{
	//CPriorityList<MSG_OBJECT> * m_pMsgList;
	cLooseLinkedList<MSG_OBJECT> * m_pMsgList;
	CMemoryPoolTempl<MSG_OBJECT> m_MsgPool;

public:
	CMsgRouter();
	virtual ~CMsgRouter();

	void InitMsgRouter();
	void ReleaseMsgRouter();

	void SendMsg(WORD msgID, DWORD src, DWORD dest, DWORD delay, DWORD dwData);
	void MsgLoop();

	void DeliveryMsgTo(MSG_OBJECT * obj, CObject * pSrcObj, CObject * pDestObj);
	
private:
	void DeliveryMsg(MSG_OBJECT * obj);

};

#endif // !defined(AFX_MSGROUTER_H__6C2248BA_291A_45DF_BE3D_E1CF30BF6B01__INCLUDED_)
