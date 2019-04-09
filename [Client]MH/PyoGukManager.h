// PyoGukManager.h: interface for the PyoGukManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PYOGUKMANAGER_H__542D9E11_C448_4CE6_BB1A_238B8A6BE1E9__INCLUDED_)
#define AFX_PYOGUKMANAGER_H__542D9E11_C448_4CE6_BB1A_238B8A6BE1E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define PYOGUKMGR USINGTON(CPyoGukManager)

class CPyoGukManager
{
public:
	CPyoGukManager();
	virtual ~CPyoGukManager();

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void BuyPyogukSyn();
	void DelPyogukSyn(BYTE num);
	
	void PutInOutMoney(int vals);
	void PutInOutMoneyResult(MSG_DWORD3 * msg);
	
	static void OnPutInMoneyPyogukOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnPutInMoneyPyogukCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnPutOutMoneyPyogukOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );	
};
EXTERNGLOBALTON(CPyoGukManager);
#endif // !defined(AFX_PYOGUKMANAGER_H__542D9E11_C448_4CE6_BB1A_238B8A6BE1E9__INCLUDED_)
