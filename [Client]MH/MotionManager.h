// MotionManager.h: interface for the CMotionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTIONMANAGER_H__7B30A145_E493_48D3_AAAE_8C6A40908F5E__INCLUDED_)
#define AFX_MOTIONMANAGER_H__7B30A145_E493_48D3_AAAE_8C6A40908F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MOTIONDESC.h"

#define MOTIONMGR USINGTON(CMotionManager)


class CMotionManager  
{
public:
	CMotionManager();
	virtual ~CMotionManager();

	void LoadMotionList();
	CMOTIONDESC * LoadMotionInfo(char * MotionFileName);
	
	CMOTIONDESC * GetMotionNDesc(DWORD id);
	void AddMotionNDesc(CMOTIONDESC * pData, DWORD dwID);
	void SetMotionInfo(CObject * pObject, DWORD dwMontionID);
protected:
	CYHHashTable<CMOTIONDESC> * m_pMontionHashTable;
};

EXTERNGLOBALTON(CMotionManager)

#endif // !defined(AFX_MOTIONMANAGER_H__7B30A145_E493_48D3_AAAE_8C6A40908F5E__INCLUDED_)
