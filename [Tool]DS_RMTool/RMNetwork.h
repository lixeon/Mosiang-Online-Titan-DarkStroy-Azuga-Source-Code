#ifndef _RMNETWORK_
#define _RMNETWORK_

#include "basenetworkinterface.h"
//#include ".\common\Commondefine.h"

//#include "Crypt.h"
class CGameState;

#define WM_SOCKEVENTMSG WM_USER+155

#define NETWORK USINGTON(CRMNetwork)

class CRMNetwork  
{
	HWND m_hMainWnd;
	CGameState* m_pCurState;

	ISC_BaseNetwork* m_pBaseNetwork;
	DWORD m_ConnectionIndex;
	
	DWORD m_RecvedDataSize;		// ¸Þ¼¼Áö¸¦ ¾ó¸¶³ª ¹Þ´ÂÁö ÅÂ½ºÆ® ÇÏ±â À§ÇØ¼­
	DWORD m_RecvCount;

//	CCrypt m_crypt;
	BYTE m_CheckSum;

#ifdef _DEBUG
	DWORD m_MsgIn[MP_MAX];
	DWORD m_MsgOut[MP_MAX];
	void AddMsgIn(BYTE Category,DWORD length)	{	m_MsgIn[Category] += length;	}
	void AddMsgOut(BYTE Category,DWORD length)	{	m_MsgOut[Category] += length;	}
	DWORD GetMsgIn(BYTE Category)	{	return m_MsgIn[Category];	}
	DWORD GetMsgOut(BYTE Category)	{	return m_MsgOut[Category];	}
	DWORD GetTotalMsgIn()
	{
		DWORD total = 0;
		for(int n=0;n<MP_MAX;++n)
			total += m_MsgIn[n];
	}
	DWORD GetTotalMsgOut()
	{
		DWORD total = 0;
		for(int n=0;n<MP_MAX;++n)
			total += m_MsgIn[n];
	}
#endif

public:
//	//MAKESINGLETON(CMHNetwork);

	CRMNetwork();
	virtual ~CRMNetwork();
	
	BOOL Init(HWND hWnd);
	void Release();

	BOOL ConnectToServer(char* ip,WORD port);
	void Disconnect();

	void SetCurState(CGameState* pState);
	
	void Send(MSGROOT* pMsg,int MsgLen);

	BOOL IsConnected()	{	return m_ConnectionIndex ? TRUE : FALSE;	}
	
	void SetCheckSum(BYTE CS)	{	m_CheckSum = CS;	}
//	void SetKey( HselInit eninit, HselInit deinit ) {	m_crypt.Init(eninit, deinit); }


	ISC_BaseNetwork* GetBaseNetwork()							{ return m_pBaseNetwork; }
	friend void OnCon(DWORD dwConIndex);
	friend void OnDisCon(DWORD dwConIndex);
	friend void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen);
};

EXTERNGLOBALTON(CRMNetwork)


#endif //_RMNETWORK_