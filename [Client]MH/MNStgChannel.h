// MNStgChannel.h: interface for the CMNStgChannel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNSTGCHANNEL_H__B16CE5A7_A876_4EBE_A912_88D24410DB25__INCLUDED_)
#define AFX_MNSTGCHANNEL_H__B16CE5A7_A876_4EBE_A912_88D24410DB25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MNStage.h"

class cDialog;
class CMNChannelDialog;
class CMNJoinDialog;
class CMNCreateDialog;


//////////////////////////////
// Channel
//	- participant user list		: interface 1
//	- playroom list (to join)	: interface 2
//	- create a playroom			: interface 3
//	- chat

//enum eCHANNELSTAGE
//{
//	CNLSTG_CHANNEL,
//	CNLSTG_JOIN,
//	CNLSTG_CREATE,
//	CNLSTG_COUNT,
//};


class CMNStgChannel : public CMNStage
{
protected:

	static CMNChannelDialog*		m_pInterface;

	DWORD		m_dwChannelIndex;
	int			m_nChannelMode;

public:

	CMNStgChannel();
	virtual ~CMNStgChannel();

	virtual void Init();
	virtual void Release();

	virtual void Open();
	virtual void Close();

	virtual void Process();
	virtual void Render();

	void SetChannelMode( int nChannelMode );

	virtual void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );
	
	virtual void ChatMsg( int nClass, CMNPlayer* pMNPlayer, char* strMsg );

//---Inline
	static CMNChannelDialog* GetInterface() { return m_pInterface; }
	
};

#endif // !defined(AFX_MNSTGCHANNEL_H__B16CE5A7_A876_4EBE_A912_88D24410DB25__INCLUDED_)
