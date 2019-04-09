// PyogukDialog.h: interface for the CPyogukDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PYOGUKDIALOG_H__20A47FB2_F872_47CD_8FF4_747FD52A07FC__INCLUDED_)
#define AFX_PYOGUKDIALOG_H__20A47FB2_F872_47CD_8FF4_747FD52A07FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CPrivateWarehouseDialog;
class cListCtrl;
class cPushupButton;
class cButton;
class cStatic;
class CItem;

struct PYOGUK_LISTINFO
{
	PYOGUKLIST Pyoguk[MAX_PYOGUKLIST_NUM];
};
enum pyogukdlg_mode
{
	ePyogukMode_PyogukListInfo,
	ePyogukMode_PyogukWare1,
	ePyogukMode_PyogukWare2,
	ePyogukMode_PyogukWare3,
	ePyogukMode_PyogukWare4,
	ePyogukMode_PyogukWare5,

	ePyogukMode_NoWare,	
		
	ePyogukMode_Max
};

class CPyogukDialog : public cDialog  
{
	int m_CurPyogukMode;
	int m_PyogukNum;
	BOOL m_bItemInit;

	cListCtrl* m_pPyogukListInfo;
	cPushupButton* m_pPyogukBtn[MAX_PYOGUKLIST_NUM];
	cStatic* m_pMoney;
	cStatic* m_pPyogukHaveStc;
	cPtrList m_PyogukCtlListArray[ePyogukMode_Max];

	CPrivateWarehouseDialog* m_PyogukItemDlg[MAX_PYOGUKLIST_NUM];
	void SetActivePyogukMode(int mode,BOOL bActive);
	void SelectedBtnPushDown(int i);
		
	PYOGUK_LISTINFO m_PyogukListInfo;

public:
	CPyogukDialog();
	virtual ~CPyogukDialog();
	
	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void SetActive( BOOL val );

	void LoadPyogukListInfo();
	void Linking();

	void SetMode(int mode);
	void SetPyogukListInfo();
	void ShowPyogukMode(int mode);
	void AddPyogukMode(int mode);
	void RefreshDlg();

	BYTE GetSelectedPyoguk() { return m_PyogukNum;	} //0, 1, 2, 3, 4
	void SetSelectedPyoguk(BYTE PyogukNum);
	void PyoGukMsgBoxClr();
	
	//개인창고	
	void SetItemInit(BOOL val) { m_bItemInit = val;	}
	BOOL IsItemInit() { return m_bItemInit;	}
	
	void MoveItem(MSG_ITEM_MOVE_SYN* pInfo);
	BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	void FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL AddItem(ITEMBASE* pItemInfo);
	BOOL AddItem(CItem* pItem);
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);
	void DeleteAllPyogukItem();

	int GetPyogukNum(POSTYPE absPos);
	
	void SetPyogukMoney(DWORD Money);
	
	int CanBuyPyoguk();
	
	MONEYTYPE GetPrice(int n) { return m_PyogukListInfo.Pyoguk[n].BuyPrice;	}
	
	CItem * GetItemForPos(POSTYPE absPos);

	void SetDividMsg( CItem* pFromItem, POSTYPE ToPos );
	MSG_ITEM_DIVIDE_SYN	m_divideMsg;
};

#endif // !defined(AFX_PYOGUKDIALOG_H__20A47FB2_F872_47CD_8FF4_747FD52A07FC__INCLUDED_)
