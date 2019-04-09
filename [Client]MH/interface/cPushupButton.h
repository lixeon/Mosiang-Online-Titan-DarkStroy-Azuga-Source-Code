// cPushupButton.h: interface for the cPushupButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cPUSHUPBUTTON_H_
#define _cPUSHUPBUTTON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cButton.h"

class cPushupButton : public cButton  
{
public:
	cPushupButton();
	virtual ~cPushupButton();

	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, void (*Func)(LONG lId, void * p, DWORD we), LONG ID=0);
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	
	void SetPush(BOOL val);
	void SetPushEx(BOOL val);
	void SetPassive(BOOL val){ m_fPassive = val; }
	BOOL IsPushed(){ return m_fPushed; }
protected:
	BOOL m_fPushed;
	BOOL m_fPassive;
	
};

#endif // _cPUSHUPBUTTON_H_
