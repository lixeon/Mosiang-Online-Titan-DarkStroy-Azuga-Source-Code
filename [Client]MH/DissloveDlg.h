// DissloveDlg.h: interface for the CDissloveDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISSLOVEDLG_H__B7EEFDD8_3534_42E8_91DF_BB6FA5976008__INCLUDED_)
#define AFX_DISSLOVEDLG_H__B7EEFDD8_3534_42E8_91DF_BB6FA5976008__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include "VirtualItem.h"
#include "ItemShow.h"

enum eDissolveReleaseOpt
{
	eDissolveBasicRelease		= 0x01,
	eDissolveViewRelease		= 0x02,
	eDissolveAllRelease			= 0x03,
};
class cStatic;
class CItem;
class cIconDialog;
class cIconGridDialog;

class CDissloveDlg : public cDialog  
{
public:
	CDissloveDlg();
	virtual ~CDissloveDlg();

	void Release(eDissolveReleaseOpt op=eDissolveAllRelease);
	void Linking();

	BOOL IsEmpty(POSTYPE pos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void DissolveCancelBtn(CDissloveDlg * pThis);
	BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	
	BOOL AddVirtualItemWrap(POSTYPE relPos, CItem * pItem);
	void SetCost(DWORD Cost);
	BOOL EnoughMoney(LEVELTYPE ItemLevel);
	BOOL EnoughSpace(WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * wMatItemOut, WORD & wMatItemNumOut);
	BOOL EnoughInvSpace(WORD wMatItemNum);
protected:
	BOOL AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem);
	cIconDialog *		m_pDissolveDlg;
	cIconGridDialog *	m_pDissolveGridDlg;
	cStatic *			m_pDissolveCostStatic;

	CVirtualItem	m_VirtualItem[MAX_DISSOLVEGRID_NUM+1];
};

#endif // !defined(AFX_DISSLOVEDLG_H__B7EEFDD8_3534_42E8_91DF_BB6FA5976008__INCLUDED_)
