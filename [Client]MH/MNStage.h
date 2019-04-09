// MNStage.h: interface for the CMNStage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNSTAGE_H__604CC83C_5BAB_46B2_84DC_E704A4BEA329__INCLUDED_)
#define AFX_MNSTAGE_H__604CC83C_5BAB_46B2_84DC_E704A4BEA329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CMNPlayer;

//////////////////////
// MurimNet Base Stage
class CMNStage  
{
protected:


public:
	CMNStage();
	virtual ~CMNStage();

	virtual void Init()		{}
	virtual void Release()	{}

	virtual void Open()		{}
	virtual void Close()	{}

	virtual void Process()	{}
	virtual void Render()	{}

	virtual void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg )	{}
	
//---
	virtual void ChatMsg( int nClass, CMNPlayer* pMNPlayer, char* strMsg )		{}


};

#endif // !defined(AFX_MNSTAGE_H__604CC83C_5BAB_46B2_84DC_E704A4BEA329__INCLUDED_)
