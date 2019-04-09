// MouseEventReceiver.h: interface for the CMouseEventReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_)
#define AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/typedef.h"
//class CPlayer;
//class CMonster;
//class CNpc;



#define MOUSEEVENT_NONE		0
#define MOUSEEVENT_LCLICK	1
#define MOUSEEVENT_RCLICK	2


class CMouseEventReceiver  
{
public:
	CMouseEventReceiver();
	virtual ~CMouseEventReceiver();

//	virtual void OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)		{}
//	virtual void OnClickMonster(DWORD MouseEvent,CMonster* pMonster)	{}
//	virtual void OnClickNpc(DWORD MouseEvent,CNpc* pNpc)				{}
	virtual void OnClickGround(DWORD MouseEvent,VECTOR3* pPos)			{}

};

#endif // !defined(AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_)
