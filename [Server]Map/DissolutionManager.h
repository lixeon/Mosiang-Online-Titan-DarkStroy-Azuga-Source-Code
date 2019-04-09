// DissolutionManager.h: interface for the CDissolutionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISSOLUTIONMANAGER_H__4DC2FCE7_90CE_4A4A_9D6B_1387A73417AA__INCLUDED_)
#define AFX_DISSOLUTIONMANAGER_H__4DC2FCE7_90CE_4A4A_9D6B_1387A73417AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_DISSOLUTION_UNITNUM		60

struct sITEM_DISSOLUTION_INFO;
struct sITEM_DISSOLUTIONITEM;

#define DISSOLUTIONMGR CDissolutionManager::GetInstance()

class CDissolutionManager  
{
protected:
	CYHHashTable<sITEM_DISSOLUTION_INFO> m_DissolutionInfoList;

	int			m_nTotalUnitOutNum;

	ITEMBASE	m_DissolutionItemArray[MAX_DISSOLUTION_UNITNUM];

protected:
	int FillDissolutionUnitToArray( WORD wDissolutionItemIdx );
	BOOL LoadDissolutionInfo();

public:
	MAKESINGLETON( CDissolutionManager );
	CDissolutionManager();
	virtual ~CDissolutionManager();

	BOOL Init();
	void Release();

	BOOL CanDissolutionItem( WORD wItemIdx );
	sITEM_DISSOLUTIONITEM* GetDissolutionUnitItemInfo( WORD wDissolutionItemIdx, WORD* pwSize );

	int ItemDissollution( CPlayer* pPlayer, ITEMBASE* pDissolutionItemInfo );
	int AddDissolutionUnitToPlayer( CPlayer* pPlayer, ITEMBASE* pDissolutionItemInfo );

};

#endif // !defined(AFX_DISSOLUTIONMANAGER_H__4DC2FCE7_90CE_4A4A_9D6B_1387A73417AA__INCLUDED_)
