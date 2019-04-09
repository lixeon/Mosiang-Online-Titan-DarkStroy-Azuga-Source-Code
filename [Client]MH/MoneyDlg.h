// MoneyDlg.h: interface for the CMoneyDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONEYDLG_H__182B0589_4897_43AD_9CA4_E1B702099288__INCLUDED_)
#define AFX_MONEYDLG_H__182B0589_4897_43AD_9CA4_E1B702099288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CMoneyDlg : public cDialog  
{
	int m_MsgLen;
	char m_SavedMsg[1024];
	DWORD m_dwParam;
public:
	CMoneyDlg();
	virtual ~CMoneyDlg();
	
	void Show(void* pmsg,int msglen,DWORD dwParam=0,BOOL (*OnPushFunc)(DWORD Money,DWORD dwParam)=0);
	void OkPushed();

	BOOL (*m_OnPushFunc)(DWORD Money,DWORD dwParam);
};

#endif // !defined(AFX_MONEYDLG_H__182B0589_4897_43AD_9CA4_E1B702099288__INCLUDED_)
