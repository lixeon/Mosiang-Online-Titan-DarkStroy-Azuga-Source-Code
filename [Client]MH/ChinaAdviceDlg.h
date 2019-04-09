// ChinaAdviceDlg.h: interface for the CChinaAdviceDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINAADVICEDLG_H__BA153720_7891_461B_9A9E_21476EED1949__INCLUDED_)
#define AFX_CHINAADVICEDLG_H__BA153720_7891_461B_9A9E_21476EED1949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

class CChinaAdviceDlg : public cDialog
{
public:
	CChinaAdviceDlg();
	virtual ~CChinaAdviceDlg();
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};

#endif // !defined(AFX_CHINAADVICEDLG_H__BA153720_7891_461B_9A9E_21476EED1949__INCLUDED_)
