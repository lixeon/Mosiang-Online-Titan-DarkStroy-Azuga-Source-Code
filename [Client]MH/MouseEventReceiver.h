// MouseEventReceiver.h: interface for the CMouseEventReceiver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_)
#define AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer;
class CMonster;
class CNpc;
class CSkillObject;
class CPetBase;



enum MOUSEEVENT // LBS 03.09.30 ¼öÁ¤
{
	MOUSEEVENT_NONE				=0,
	MOUSEEVENT_LCLICK			=1,
	MOUSEEVENT_RCLICK			=2,
	MOUSEEVENT_DBLCLICK			=4,
	MOUSEEVENT_LPRESS			=8,
};



class CMouseEventReceiver  
{
public:
	CMouseEventReceiver();
	virtual ~CMouseEventReceiver();

	virtual void OnMouseOver( CObject* pObject )							{}
	virtual void OnMouseLeave( CObject* pObject )							{}
	virtual void OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)			{}
	virtual void OnClickMonster(DWORD MouseEvent,CMonster* pMonster)		{}
	virtual void OnClickNpc(DWORD MouseEvent,CNpc* pNpc)					{}
	virtual void OnClickGround(DWORD MouseEvent,VECTOR3* pPos)				{}
	virtual void OnClickSkillObject(DWORD MouseEvent,CSkillObject* pObj)	{}
	virtual void OnClickPet( DWORD MouseEvent, CPetBase* pPet )				{}
	virtual void OnClickMapObject(  DWORD MouseEvent, CObject* pObj )		{}
	
	// LBS 03.09.30  Player¿¡ ´ëÇÑ ´õºíÅ¬¸¯ Ã¼Å©¸¦ À§ÇØ¼­..
	virtual void OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)			{}
	virtual void OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster)		{}
	virtual void OnDbClickNpc(DWORD MouseEvent,CNpc* pNpc)					{}

};

#endif // !defined(AFX_MOUSEEVENTRECEIVER_H__925076FC_FD16_47F1_8259_136B9C5D0C3E__INCLUDED_)
