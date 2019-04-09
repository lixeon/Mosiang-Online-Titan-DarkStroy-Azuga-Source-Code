// MugongDialog.h: interface for the CMugongDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGDIALOG_H__E1B5A207_7E98_4653_AAA4_7515E7C8A3DB__INCLUDED_)
#define AFX_MUGONGDIALOG_H__E1B5A207_7E98_4653_AAA4_7515E7C8A3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"
#include "MugongWindow.h"


#ifdef _JAPAN_LOCAL_
#define	SLOT_MUGONG_NUM_JAP		20
enum MLOC{ MUGONGWINDOW_NORMAL, MUGONGWINDOW_JINBUB, MUGONGWINDOW_MAX};

#elif defined _HK_LOCAL_
#define	SLOT_MUGONG_NUM_JAP		20
enum MLOC{ MUGONGWINDOW_NORMAL, MUGONGWINDOW_JINBUB, MUGONGWINDOW_MAX};

#elif defined _TL_LOCAL_
#define	SLOT_MUGONG_NUM_JAP		20
enum MLOC{ MUGONGWINDOW_NORMAL, MUGONGWINDOW_JINBUB, MUGONGWINDOW_MAX};

#else
enum MLOC{ MUGONGWINDOW_NORMAL, MUGONGWINDOW_EXTEND, MUGONGWINDOW_JINBUB, MUGONGWINDOW_TITAN, MUGONGWINDOW_MAX};	// magi82 - Titan(070910) 타이탄 무공업데이트
enum { MGI_NAME, MGI_SUNG, MGI_EXPPOINT, MGI_MAX };
#endif


class CItem;
class CMugongBase;
class cPushupButton;


class CMugongDialog  : public cIconDialog
{
	cPushupButton*	m_pMugongRectBtn[MUGONGWINDOW_MAX];
	CMugongWindow	m_pMugongRect[MUGONGWINDOW_MAX];			// ÀÏ¹Ý,Áø¹ý,½É¹ý
	
	friend class CMugongManager;

	CMugongBase*	m_pTempDeleteMugong;

	cIconDialog*	m_pWearedIconDlg;
	cStatic*		m_pMugongInfo[MGI_MAX];
	CObjectGuagen*	m_pExpGuage;
	DWORD			m_CurSelectedGrid;
	DWORD			m_PreSelectedGrid;

	BOOL			m_bMugongGridExtend;
	BOOL			m_bPushTabWithMouseOver;
	DWORD			m_BtnPushstartTime;
	int				m_nDeleteMsgBoxCount;

public:
	CMugongDialog();
	virtual ~CMugongDialog();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	
	void SetActive(BOOL val);
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	virtual void Add(cWindow * window);
	void AddMugong(MUGONGBASE * mugongBase);
	void AddMugong(POSTYPE pos, CMugongBase * mugong);
	CMugongBase * GetMugongAbs(POSTYPE abs_Pos);
//	void FakeDeleteMugong(POSTYPE absPos);
	void FakeDeleteMugong(CMugongBase* pMugong);
	void SendDeleteMugong();
	BOOL DeleteMugong(POSTYPE pos, CMugongBase ** mugong);
	
	virtual BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	void MoveMugong(POSTYPE ToPos, POSTYPE FromPos);
	
	BOOL GetBlankPositionRef(BYTE type, WORD & absPos);
	BOOL GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos);
	MLOC GetWindowLocate(POSTYPE abs_pos);
	BOOL IsAddMugong(POSTYPE pos);

	// µ¥ÀÌÅÍ ¼ÂÆÃ°ú ÀÎÅ×ÆäÀÌ½º Ãâ·Â
	void SetMugongSung(POSTYPE abspos, BYTE sung);
	void SetMugongExpPoint(POSTYPE abspos, DWORD expPoint);
	void FakeAddItem(LONG mouseX, LONG mouseY, CItem * icon);

#ifdef _JAPAN_LOCAL_
	void Linking_JP();
#elif defined _HK_LOCAL_
	void Linking_JP();
#elif defined _TL_LOCAL_
	void Linking_JP();
#else
	void Linking();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void SetSkillInfoStatic(CMugongBase* pMugongBase);
	void ActiveMogongGrid( LONG lId );
	void SetMugongExtend( BOOL val )		{	m_bMugongGridExtend = val;	}
	BOOL IsMugongExtend()					{	return m_bMugongGridExtend;	}
#endif //_JAPAN_LOCAL_


private:
	void ExcuteMugong(POSTYPE pos);
	void SelectMugong(POSTYPE pos);
	BOOL CanBeMoved(cIcon * pIcon, POSTYPE abs_pos);
	BOOL FakeMoveMugong(LONG mouseX, LONG mouseY, CMugongBase * icon);
	cIconDialog * m_pMugongDlg1_1;			// ³»°ø, °æ°ø
	cIconGridDialog * m_pMugongDlg1_2;		// ÆîÄ£ ¹«°ø
	cPushupButton * m_pMugongSpreadBtn;

public:
	CMugongBase* GetSelectMugong() { return (CMugongBase*)(m_pWearedIconDlg->GetIconForIdx(0)); }
};

#endif // !defined(AFX_MUGONGDIALOG_H__E1B5A207_7E98_4653_AAA4_7515E7C8A3DB__INCLUDED_)
