// ObjectEvent.h: interface for the CObjectEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTEVENT_H__A055EAC4_E98B_4AAC_86CB_5669E1D0CBFD__INCLUDED_)
#define AFX_OBJECTEVENT_H__A055EAC4_E98B_4AAC_86CB_5669E1D0CBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eOBJECTEVENT
{
	OE_LEVELUP,
	OE_DIE,
	OE_LIFERECOVERCOMPLETED,
};

class CObjectEvent  
{
public:
	CObjectEvent();
	virtual ~CObjectEvent();

	static void ObjectEvent( CObject * pObject, eOBJECTEVENT dwEvent, void * vData );
};

#endif // !defined(AFX_OBJECTEVENT_H__A055EAC4_E98B_4AAC_86CB_5669E1D0CBFD__INCLUDED_)
