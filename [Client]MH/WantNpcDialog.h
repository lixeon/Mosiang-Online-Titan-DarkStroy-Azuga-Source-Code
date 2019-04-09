// WantNpcDialog.h: interface for the CWantNpcDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTNPCDIALOG_H__5DDA77AF_5511_4857_83F9_B30B8B9285E8__INCLUDED_)
#define AFX_WANTNPCDIALOG_H__5DDA77AF_5511_4857_83F9_B30B8B9285E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
class cListCtrl;
class cStatic;
class cComboBox;
class cPushupButton;
class cEditBox;

#define MAX_WANTNPC_PAGEUNIT 5 //5ÆäÀÌÁö ´ÜÀ§
#define MAX_WANTNPC_PAGEBTN 7

class CWantNpcDialog : public cDialog  
{
	cListCtrl * pWantedListCtrl;
	//eWantNpcMode_Npc
	cButton * m_pMyWantNpcBtn;
	cButton * m_pBuyNpcBtn;
	cButton * m_pBuySearchBtn;
	cButton * m_pSearchModeBtn;
	cButton * m_pSearchBtn;
	cButton * m_pGiveupBtn;
	cButton * m_pMyWantSearchBtn;
	cComboBox * m_pOrderWantNpcCombo;
	cPushupButton * m_OrderPageBtn[MAX_WANTNPC_PAGEBTN]; //front, 1~5, back

	//eWantNpcMode_My
	cButton * m_pSortBtn;
	cStatic* m_pMyStc;
	cPushupButton * m_MyWantPageBtn[MAX_WANTNPC_PAGEBTN]; //front, 1~5, back

	//eWantNpcMode_Search
	cPushupButton * m_SearchPageBtn[MAX_WANTNPC_PAGEBTN]; //front, 1~5, back
	cEditBox * m_pSearchEdit;
	//////////////////////////////////////////////////////////////////////////

	BYTE m_WantedOrderType;
	WORD m_CurPage;
	WORD m_PastPage;

	int m_Mode;
	cPtrList m_WantNpcCtlListArray[eWantNpcMode_Max];

	void SetMode(int Mode) { m_Mode = Mode;	}
	int GetMode() { return m_Mode; }

	WANTNPCLIST m_CurSelectWanted;
	int m_SelectRowIndex;

	void SetWhiteFontColor();
public:
	CWantNpcDialog();
	virtual ~CWantNpcDialog();

	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void Linking();
	BYTE GetOrderType();
	void SetOrderType(BYTE OrderType);
	void SetWantedList(SEND_WANTNPCLIST* pInfo);
	
	void ShowWantPageBtn(WORD TotalPage, cPushupButton* pBtnArray[]);

	void SetActiveWantMode(int mode,BOOL bActive);
	void ShowWantMode(int mode);
	
	void SetCurPage(WORD page);
	WORD GetCurPage();

	void SetPushUpBtn(cPushupButton* pArray[], int num);

	DWORD GetCurWantedIdx();
	void SetCurSelectWanted(WANTNPCLIST * pInfo, int SelectRowIndex);
	char* GetCurWantedName();

	void CloseMsgBox();

	void AddShowWantMode(int mode);
};

#endif // !defined(AFX_WANTNPCDIALOG_H__5DDA77AF_5511_4857_83F9_B30B8B9285E8__INCLUDED_)
