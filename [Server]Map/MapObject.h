// MapObject.h: interface for the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__D84C4D07_428D_427E_A1F0_390A8401E3E4__INCLUDED_)
#define AFX_MAPOBJECT_H__D84C4D07_428D_427E_A1F0_390A8401E3E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Object.h"


class CMapObject : public CObject
{
	MAPOBJECT_INFO	m_MapObjectInfo;
	DWORD			m_Level;

public:
	CMapObject();
	virtual ~CMapObject();

	void InitMapObject( MAPOBJECT_INFO* pMOInfo );

	virtual void DoDie( CObject* pAttacker );

	virtual float GetRadius()	{	return m_MapObjectInfo.Radius;	}
	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);
	void SetCastlegateAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

	virtual DWORD GetLife();
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	virtual DWORD GetShield();
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);
	virtual DWORD DoGetMaxLife();
	virtual DWORD DoGetMaxShield();
	virtual DWORD DoGetPhyDefense();
	virtual float DoGetAttDefense(WORD Attrib);
	void SetLevel( DWORD Level )		{	m_Level = Level;	}

};

#endif // !defined(AFX_MAPOBJECT_H__D84C4D07_428D_427E_A1F0_390A8401E3E4__INCLUDED_)
