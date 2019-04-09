// ObjectStateManager.h: interface for the CObjectStateManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTSTATEMANAGER_H__281BEDAA_4216_40F2_8554_FC805865957A__INCLUDED_)
#define AFX_OBJECTSTATEMANAGER_H__281BEDAA_4216_40F2_8554_FC805865957A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OBJECTSTATEMGR USINGTON(CObjectStateManager)

class CObject;


enum eCHECKAVAILABLESTATE
{
	eCAS_EQUIPITEM,
};


class CObjectStateManager  
{
//	BASEOBJECT_INFO m_BaseObjectInfo;
public:
	//MAKESINGLETON(CObjectStateManager);
	
	CObjectStateManager();
	virtual ~CObjectStateManager();

	void InitObjectState(CObject* pObject);
	BOOL StartObjectState(CObject* pObject,BYTE State);
	void EndObjectState(CObject* pObject,BYTE State,DWORD EndStateCount = 0);
	BYTE GetObjectState(CObject* pObject);
	BOOL IsEndStateSetted(CObject* pObject);
	
	void StateProcess(CObject* pObject);
	
	BOOL CheckAvailableState(CObject* pObject,eCHECKAVAILABLESTATE cas);
};
EXTERNGLOBALTON(CObjectStateManager);
#endif // !defined(AFX_OBJECTSTATEMANAGER_H__281BEDAA_4216_40F2_8554_FC805865957A__INCLUDED_)
