// KyungGongManager.h: interface for the CKyungGongManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KYUNGGONGMANAGER_H__D0372DCB_6B01_43F1_9743_9CA0D89DC11D__INCLUDED_)
#define AFX_KYUNGGONGMANAGER_H__D0372DCB_6B01_43F1_9743_9CA0D89DC11D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KyungGongInfo.h"

#ifdef _MAPSERVER_
#define PLAYERTYPE	CPlayer
#define ITEMTYPE	ITEM_INFO
struct ITEMTYPE;
#else
#define PLAYERTYPE	CHero
#define ITEMTYPE	CItem
class ITEMTYPE;
#endif
class PLAYERTYPE;

#define KYUNGGONGMGR	CKyungGongManager::GetInstance()

class CKyungGongManager  
{
	CYHHashTable<CKyungGongInfo> m_InfoTable;

	CKyungGongManager();
public:
	MAKESINGLETON(CKyungGongManager);
	virtual ~CKyungGongManager();

	void Init();
	void Release();

	CKyungGongInfo* GetKyungGongInfo(WORD idx);

	BOOL IsSetableKyungGong(PLAYERTYPE* pHero,ITEMTYPE* pItem);
	void SetKyungGong(PLAYERTYPE* pHero,WORD KyungGongIdx);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
};

#endif // !defined(AFX_KYUNGGONGMANAGER_H__D0372DCB_6B01_43F1_9743_9CA0D89DC11D__INCLUDED_)
