// NameChangeDialog.h: interface for the CNameChangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMECHANGEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)
#define AFX_NAMECHANGEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"

class cEditBox;


class CNameChangeDialog : public cDialog
{
	cEditBox*		m_pNameBox;
	DWORD			m_dwDBIdx;


public:
	CNameChangeDialog();
	virtual ~CNameChangeDialog();

	void Linking();
	void SetActive( BOOL val );
	void NameChangeSyn();

	void SetItemDBIdx(DWORD dbIdx)		{	m_dwDBIdx = dbIdx;	}
	DWORD GetItemDBIdx()				{	return m_dwDBIdx;	}
};

#endif // !defined(AFX_NAMECHANGEDIALOG_H__73CE85B3_4B5D_4484_A158_A3067F4458BF__INCLUDED_)
