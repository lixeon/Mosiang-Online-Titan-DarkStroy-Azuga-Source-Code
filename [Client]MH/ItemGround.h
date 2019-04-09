// ItemGround.h: interface for the CItemGround class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMGROUND_H__A2F0AD1F_C57B_4D0B_9CEC_DA89BD7F536D__INCLUDED_)
#define AFX_ITEMGROUND_H__A2F0AD1F_C57B_4D0B_9CEC_DA89BD7F536D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Engine\EngineObject.h"

class CEngineObject;

class CItemGround : public CObjectBase
{
	CEngineObject* m_pEngineObject;

protected:
	VECTOR3 m_Pos;
	ITEMBASE m_ItemBaseInfo;

public:
	CItemGround(ITEMBASE* pInfo,VECTOR3* pPos);
	virtual ~CItemGround();
	
	void Process();

	DWORD GetDBIdx()		{	return m_ItemBaseInfo.dwDBIdx;		}
	friend class CAppearanceManager;
};

#endif // !defined(AFX_ITEMGROUND_H__A2F0AD1F_C57B_4D0B_9CEC_DA89BD7F536D__INCLUDED_)
