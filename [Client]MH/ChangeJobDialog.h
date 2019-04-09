// ChangeJobDialog.h: interface for the CChangeJobDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEJOBDIALOG_H__007D0B99_BB1F_4869_AFC6_97412FDF57E1__INCLUDED_)
#define AFX_CHANGEJOBDIALOG_H__007D0B99_BB1F_4869_AFC6_97412FDF57E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"



class CChangeJobDialog : public cDialog
{
	DWORD			m_ItemPos;
	DWORD			m_ItemDBIdx;

public:
	CChangeJobDialog();
	virtual ~CChangeJobDialog();

	void SetItemInfo( DWORD ItemPos, DWORD ItemDBIdx )
	{
		m_ItemPos = ItemPos;
		m_ItemDBIdx = ItemDBIdx;
	}
	DWORD GetItemPos()		{	return m_ItemPos;	}
	DWORD GetItemDBIdx()	{	return m_ItemDBIdx;	}
	void ChangeJobSyn();
	void CancelChangeJob();
};

#endif // !defined(AFX_CHANGEJOBDIALOG_H__007D0B99_BB1F_4869_AFC6_97412FDF57E1__INCLUDED_)
