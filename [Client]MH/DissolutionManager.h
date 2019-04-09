// DissolutionManager.h: interface for the CDissolutionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISSOLUTIONMANAGER_H__CD3ABCBE_BF6A_49EE_9B96_45E00408A8E0__INCLUDED_)
#define AFX_DISSOLUTIONMANAGER_H__CD3ABCBE_BF6A_49EE_9B96_45E00408A8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DISSOLUTIONMGR USINGTON(CDissolutionManager)

struct sITEM_DISSOLUTION_INFO;
struct sITEM_DISSOLUTIONITEM;
class CDissolutionManager  
{
protected:
	CYHHashTable<sITEM_DISSOLUTION_INFO> m_DissolutionInfoList;
		
public:
	CDissolutionManager();
	virtual ~CDissolutionManager();
	
	BOOL Init();
	void Release();
	BOOL LoadDissolutionInfo();

	BOOL CanDissolutionItem( WORD wItemIdx );
	sITEM_DISSOLUTIONITEM* GetDissolutionUnitItemInfo( WORD wDissolutionItemIdx, WORD* pwSize );
};

EXTERNGLOBALTON(CDissolutionManager)

#endif // !defined(AFX_DISSOLUTIONMANAGER_H__CD3ABCBE_BF6A_49EE_9B96_45E00408A8E0__INCLUDED_)
