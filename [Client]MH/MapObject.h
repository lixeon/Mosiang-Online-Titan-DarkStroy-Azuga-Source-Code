// MapObject.h: interface for the CMapObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPOBJECT_H__97F2A552_1CD9_4BB6_BBDC_289B209B7D4C__INCLUDED_)
#define AFX_MAPOBJECT_H__97F2A552_1CD9_4BB6_BBDC_289B209B7D4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Object.h"


class CMapObject : public CObject
{
	MAPOBJECT_INFO		m_MapObjectInfo;

public:
	CMapObject();
	virtual ~CMapObject();

	void InitMapObject( MAPOBJECT_INFO* pInitInfo );

	virtual float GetRadius()	{	return m_MapObjectInfo.Radius;	}
	virtual void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin) {}
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void SetLife(DWORD val, BYTE type = 1);
	virtual void SetShield(DWORD val, BYTE type = 1);


	DWORD GetMaxLife()		{	return m_MapObjectInfo.MaxLife;		}
	DWORD GetMaxShield()	{	return m_MapObjectInfo.MaxShield;	}
	DWORD GetLife() {	return m_MapObjectInfo.Life;	}
	DWORD GetShield() {	return m_MapObjectInfo.Shield;	}	
};

#endif // !defined(AFX_MAPOBJECT_H__97F2A552_1CD9_4BB6_BBDC_289B209B7D4C__INCLUDED_)
