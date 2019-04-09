// PKLootingDialog.h: interface for the CPKLootingDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKLOOTINGDIALOG_H__8EA93B25_A2D1_4490_A549_36946890F4E4__INCLUDED_)
#define AFX_PKLOOTINGDIALOG_H__8EA93B25_A2D1_4490_A549_36946890F4E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"

class cStatic;
class cIconGridDialog;

//¸Ê¼­¹ö¿¡ ¸¸µé¾ú´ø°¡????
enum eLOOTINGITEM_KIND
{
	eLIK_ITEM,
	eLIK_MONEY,
	eLIK_EXP,
	eLIK_NONE,
};

class CPKLootingDialog : public cDialog
{
protected:

	cStatic*			m_pStcBadFame;
	cStatic*			m_pStcTime;
	cStatic*			m_pStcChance;
	cStatic*			m_pStcTarget;
	cStatic*			m_pStcItem;
	cStatic*			m_pStcEnd;
	cStatic*			m_pStcNone;

	cIconGridDialog*	m_pIGDItem;

	DWORD		m_dwDiePlayerIdx;
	int			m_nTime;			//³²Àº½Ã°£(ÃÊ)
	DWORD		m_dwStartTime;		//½ÃÀÛ½Ã°£(ms)
	int			m_nChance;			//³²Àº±âÈ¸
	int			m_nLootItemNum;		//³²Àº È¹µæ ¾ÆÀÌÅÛ

	BOOL		m_bSelected[PKLOOTING_ITEM_NUM];

	BOOL		m_bLootingEnd;
	BOOL		m_bMsgSync;

//---Ã¢ Àá½ÃÈÄ¿¡ ¶ç¿ì±â
	DWORD		m_dwCreateTime;
	BOOL		m_bShow;

public:
	CPKLootingDialog();
	virtual ~CPKLootingDialog();

	void InitPKLootDlg( DWORD dwID, LONG x, LONG y, DWORD dwDiePlayerIdx );
	void Linking();

	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual void Render();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	DWORD GetDiePlayerIdx() { return m_dwDiePlayerIdx; }

	void ReleaseAllIcon();
	void ChangeIconImage( WORD pos, int nKind, WORD ItemIdx = 0 );

	void AddLootingItemNum();
	BOOL IsLootingEnd() { return m_bLootingEnd; }
	void SetLootingEnd( BOOL bLootingEnd ) { m_bLootingEnd = bLootingEnd; }

	void SetMsgSync( BOOL bSync );
};

#endif // !defined(AFX_PKLOOTINGDIALOG_H__8EA93B25_A2D1_4490_A549_36946890F4E4__INCLUDED_)
