// MPGuageDialog.h: interface for the CMPGuageDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPGUAGEDIALOG_H__26885A4C_EB69_4E97_A00F_04DA03D92960__INCLUDED_)
#define AFX_MPGUAGEDIALOG_H__26885A4C_EB69_4E97_A00F_04DA03D92960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
class CObjectGuagen;
class cStatic;
class CMPGuageDialog : public cDialog  
{
	CObjectGuagen * m_ExpGuage;
	cStatic* m_Time;
	cStatic* m_ExpPercent;
	cStatic* m_pTitle;
public:
	CMPGuageDialog();
	virtual ~CMPGuageDialog();

	void Linking();
	void SetExpGuage(float Percent);
	void SetTime(DWORD RemainTime);
	void SetEventMapTimer( DWORD RemainTime, BYTE bFlag );
	void ShowEventMap();
};

#endif // !defined(AFX_MPGUAGEDIALOG_H__26885A4C_EB69_4E97_A00F_04DA03D92960__INCLUDED_)
