// MunpaMarkDialog.h: interface for the CMunpaMarkDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNPAMARKDIALOG_H__21D74E49_83EA_4985_A8F3_818325D77D57__INCLUDED_)
#define AFX_MUNPAMARKDIALOG_H__21D74E49_83EA_4985_A8F3_818325D77D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CMunpaMark;

class CMunpaMarkDialog : public cDialog  
{
	CMunpaMark* m_pMunpaMark;
	void Render();
public:
	CMunpaMarkDialog();
	virtual ~CMunpaMarkDialog();

	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);

	BOOL SetMunpaMark(DWORD MunpaID);
};

#endif // !defined(AFX_MUNPAMARKDIALOG_H__21D74E49_83EA_4985_A8F3_818325D77D57__INCLUDED_)
