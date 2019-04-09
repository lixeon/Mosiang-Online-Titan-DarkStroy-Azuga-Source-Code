// StatusIconDlg.h: interface for the CStatusIconDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)
#define AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cImage.h"
#include "./Interface/cMultiLineText.h"
class CObject;
class CMouse;
#define STATUSICONDLG USINGTON(CStatusIconDlg)



struct ICONRENDERINFO
{
	DWORD	ItemIndex;
	BOOL	bPlus;
	BOOL	bAlpha;
	int		Alpha;
};



class CStatusIconDlg  
{
	int m_MaxDesc;
	StaticString* m_pDescriptionArray;

	CObject* m_pObject;
	WORD m_IconCount[eStatusIcon_Max];
	cImage m_StatusIcon[eStatusIcon_Max];
	ICONRENDERINFO m_IconInfo[eStatusIcon_Max];
	
	DWORD m_dwRemainTime[eStatusIcon_Max];
	DWORD m_dwStartTime[eStatusIcon_Max];

	VECTOR2 m_DrawPosition;		// 그릴 위치
	int m_MaxIconPerLine;		// 한줄에 아이콘 몇개?

	cMultiLineText m_toolTip;
	int m_CurIconNum;
//	LONG m_ttX, m_ttY;

//	BOOL m_bInit;

	int		m_nQuestIconCount;
	
	void LoadDescription();
public:
	CStatusIconDlg();
	virtual ~CStatusIconDlg();

	void AddIcon(CObject* pObject,WORD StatusIconNum,WORD ItemIdx=0, DWORD dwRemainTime = 0);
	void AddQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveIcon(CObject* pObject,WORD StatusIconNum, WORD ItemIdx=0);
	void RemoveQuestTimeIcon(CObject* pObject,WORD StatusIconNum);
	void RemoveAllQuestTimeIcon();

	void Render();

	void Init(CObject* pObject,VECTOR2* pDrawPosition,int MaxIconPerLine);
	void Release();

	void SetOneMinuteToShopItem(DWORD ItemIdx);

	void AddQuestIconCount()	{ ++m_nQuestIconCount; }

//	void ActionEvent(CMouse * mouseInfo);
//	BOOL IsInited() { return m_bInit; }
//	void SetToolTip( char* msg, DWORD color, cImage * image, DWORD imgColor );
};

EXTERNGLOBALTON(CStatusIconDlg)

#endif // !defined(AFX_STATUSICONDLG_H__C0B71631_2257_49D3_B0A0_DC28AA71B614__INCLUDED_)
