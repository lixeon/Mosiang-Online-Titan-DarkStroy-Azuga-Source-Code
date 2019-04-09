// MNStgFront.h: interface for the CMNStgFront class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNSTGFRONT_H__51BD9B39_D722_4BF9_8301_6EEC26DB5EE4__INCLUDED_)
#define AFX_MNSTGFRONT_H__51BD9B39_D722_4BF9_8301_6EEC26DB5EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MNStage.h"


class CMNFrontDialog;

////////////////////////////////////
// Front
//	- channel list
//	- notice

class CMNStgFront : public CMNStage
{

protected:
	
	static CMNFrontDialog*		m_pInterface;

public:
	CMNStgFront();
	virtual ~CMNStgFront();

	virtual void Init();
	virtual void Release();

	virtual void Open();
	virtual void Close();

	virtual void Process();
	virtual void Render();	

	virtual void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

//Inline
	static CMNFrontDialog* GetInterface() { return m_pInterface; }

};

#endif // !defined(AFX_MNSTGFRONT_H__51BD9B39_D722_4BF9_8301_6EEC26DB5EE4__INCLUDED_)
