// CTitanRecallDlg.h: interface for the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PROGRESSBARDLG_H
#define _PROGRESSBARDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "GameResourceStruct.h"

class CItem;
class CObjectGuagen;
class cStatic;

class CProgressBarDlg : public cDialog
{
protected:
	CObjectGuagen*	m_pProgressGuagen;
	cStatic*		m_pRemaintimeStatic;

	BOOL		m_bProgressStart;	//Process 시작 flag
	BOOL		m_bSuccessProgress;	//Process 완료  flag
	DWORD		m_dwProcessTime;	//목표 시간
	DWORD		m_dwCurrentTime;	//현재 시간
	DWORD		m_dwSuccessTime;	//카운터 설정 시간
	

public:
	CProgressBarDlg();
	virtual ~CProgressBarDlg();

	virtual void Render();	
	virtual void SetActive( BOOL val );	
	void Process();
	
	void InitProgress();
	void StartProgress();	
	BOOL GetSuccessProgress();
	void SetSuccessProgress(BOOL bVal);

	void SetSuccessTime(DWORD dwTime);
};

#endif // _PROGRESSBARDLG_H