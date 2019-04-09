// ObjectEvent.cpp: implementation of the CObjectEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectEvent::CObjectEvent()
{

}

CObjectEvent::~CObjectEvent()
{

}
void CObjectEvent::ObjectEvent( CObject * pObject, eOBJECTEVENT dwEvent, void * vData )
{
EVENT_TYPE(dwEvent)
	EVENTOF(OE_LEVELUP)
	{
	}
	EVENTOF(OE_DIE)
	{
	}
	EVENTOF(OE_LIFERECOVERCOMPLETED)
	{
		
	}
EVENT_END
}