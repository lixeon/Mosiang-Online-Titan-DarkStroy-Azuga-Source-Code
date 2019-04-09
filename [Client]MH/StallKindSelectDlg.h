// StallKindSelectDlg.h: interface for the CStallKindSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STALLKINDSELECTDLG_H__1876F324_6442_4E79_BBA4_CDDD17C947F4__INCLUDED_)
#define AFX_STALLKINDSELECTDLG_H__1876F324_6442_4E79_BBA4_CDDD17C947F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

enum STALL_KIND
{
	eSK_NULL,
	eSK_SELL,
	eSK_BUY,
};

class cButton;

class CStallKindSelectDlg : public cDialog  
{
	cButton* m_pSellBtn;
	cButton* m_pBuyBtn;
	cButton* m_pCancelBtn;
	
public:
	CStallKindSelectDlg();
	virtual ~CStallKindSelectDlg();

	void Linking();
	void Show();
	void Close();

	void OnActionEvnet(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_STALLKINDSELECTDLG_H__1876F324_6442_4E79_BBA4_CDDD17C947F4__INCLUDED_)
