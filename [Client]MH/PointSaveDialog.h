// PointSaveDialog.h: interface for the PointSaveDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTSAVEDIALOG_H__B1850F07_25B8_4A01_9CC0_8D296FF156DE__INCLUDED_)
#define AFX_POINTSAVEDIALOG_H__B1850F07_25B8_4A01_9CC0_8D296FF156DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"


class cEditBox;
class cTextArea;



class CPointSaveDialog : public cDialog
{
	cEditBox*		m_pNameEdtBox;
	cTextArea*		m_pText;

	BOOL			m_bNewPoint;
	DWORD			m_ItemPos;
	DWORD			m_ItemIdx;

public:
	CPointSaveDialog();
	virtual ~CPointSaveDialog();

	void Linking();
	void SetActive( BOOL val );
	void SetItemToMapServer( DWORD ItemIdx, DWORD ItemPos )	{	m_ItemIdx=ItemIdx; m_ItemPos=ItemPos;	}

	void ChangePointName();
	void CancelPointName();
	void SetDialogStatus( BOOL bNewPoint )		{	m_bNewPoint=bNewPoint;	}
};

#endif // !defined(AFX_POINTSAVEDIALOG_H__B1850F07_25B8_4A01_9CC0_8D296FF156DE__INCLUDED_)
