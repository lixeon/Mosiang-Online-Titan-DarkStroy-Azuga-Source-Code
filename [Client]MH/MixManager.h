// MixManager.h: interface for the CMixManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIXMANAGER_H__9224F6BC_6E93_420A_A1E5_87ABA2CCF4C7__INCLUDED_)
#define AFX_MIXMANAGER_H__9224F6BC_6E93_420A_A1E5_87ABA2CCF4C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "HashTable.h"

#define MIXMGR USINGTON(CMixManager)

class CItemShow;
class CItem;
class CMHFile;
class ITEM_MIX_INFO;
class ITEM_MIX_RES;
class ITEM_MIX_MATERIAL;

class CVirtualItem;
class CMixManager  
{
	CYHHashTable<ITEM_MIX_INFO> m_MixItemTable;
public:
	CMixManager();
	virtual ~CMixManager();
	void	Release();
	void	LoadMixList();
	void	LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
//	CItemShow * NewShowItem(WORD wItemIdx);
//	void DeleteShowItem(CItemShow * pSItem);

	ITEM_MIX_INFO * GetMixItemInfo(WORD wItemIdx);
};
EXTERNGLOBALTON(CMixManager)
#endif // !defined(AFX_MIXMANAGER_H__9224F6BC_6E93_420A_A1E5_87ABA2CCF4C7__INCLUDED_)

