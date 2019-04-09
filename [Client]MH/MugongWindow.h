// MugongWindow.h: interface for the CMugongWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUGONGWINDOW_H__90624ABD_6CD1_459A_9E94_D35790541095__INCLUDED_)
#define AFX_MUGONGWINDOW_H__90624ABD_6CD1_459A_9E94_D35790541095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cIconDialog;
class cIconGridDialog;
class cStatic;
class CMugongBase;
class cIcon;
class cDialog;
class CObjectGuagen;

#ifdef _JAPAN_LOCAL_ 
enum { MGI_NAME, MGI_SUNG, MGI_EXPPOINT, MGI_MAX };
#endif

#ifdef _HK_LOCAL_ 
enum { MGI_NAME, MGI_SUNG, MGI_EXPPOINT, MGI_MAX };
#endif

#ifdef _TL_LOCAL_ 
enum { MGI_NAME, MGI_SUNG, MGI_EXPPOINT, MGI_MAX };
#endif

class CMugongWindow  
{
	MUGONG_TYPE m_MugongType;

	POSTYPE m_SelectPos;
	POSTYPE m_StartPos;
	POSTYPE m_EndPos;

	cIconGridDialog* m_pAllIconGrid;

#ifdef _JAPAN_LOCAL_
 	cIconDialog* m_pWearedIconDlg;
 
 	cStatic * m_pMugongInfo[MGI_MAX];
 	CObjectGuagen	* m_pExpGuage;
 	void SetSkillInfoStatic(CMugongBase* pMugongBase);
#endif
#ifdef _HK_LOCAL_
 	cIconDialog* m_pWearedIconDlg;
 
 	cStatic * m_pMugongInfo[MGI_MAX];
 	CObjectGuagen	* m_pExpGuage;
 	void SetSkillInfoStatic(CMugongBase* pMugongBase);
#endif
#ifdef _TL_LOCAL_
 	cIconDialog* m_pWearedIconDlg;
 
 	cStatic * m_pMugongInfo[MGI_MAX];
 	CObjectGuagen	* m_pExpGuage;
 	void SetSkillInfoStatic(CMugongBase* pMugongBase);
#endif

public:
	CMugongWindow();
	virtual ~CMugongWindow();

	void Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,cDialog* pParent,DWORD GridID);
	CMugongBase * GetCurSelMugong();
	BOOL GetBlankPositionRef(WORD & absPos);
	BOOL IaAddable(POSTYPE pos);
	void AddMugong(POSTYPE pos/*abs pos*/, CMugongBase * mugong);
	BOOL DeleteMugong(POSTYPE pos/*abs pos*/, CMugongBase ** mugong);
	BOOL GetPositionForXYRef(LONG x, LONG y, POSTYPE& pos);
	CMugongBase * GetMugong(POSTYPE abs_Pos);
	void SetMugongSung(POSTYPE abspos,BYTE sung);
	void SetMugongExpPoint(POSTYPE abspos,DWORD expPoint);
	BOOL CanBeMoved(cIcon * pIcon, POSTYPE abs_pos);

	void SelectMugong(POSTYPE abspos);
	BOOL IsSelectedMugong(POSTYPE abspos);
	void RefreshSelectedMugong();

	//
	void GridActive( BOOL val );

#ifdef _JAPAN_LOCAL_
	void SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos );	
	void Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
				cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID);
#endif	// _JAPAN_LOCAL_
#ifdef _HK_LOCAL_
	void SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos );	
	void Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
				cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID);
#endif	// _HK_LOCAL_
#ifdef _TL_LOCAL_
	void SetDiableIcon( POSTYPE FromPos, POSTYPE EndPos );	
	void Init(MUGONG_TYPE mtype,POSTYPE startpos,POSTYPE endpos,
				cDialog* pParentDlg,DWORD wearWndID,DWORD gridWndID,DWORD nameWndID,DWORD levelWndID,DWORD expWndID,DWORD guageID);
#endif
};

#endif // !defined(AFX_MUGONGWINDOW_H__90624ABD_6CD1_459A_9E94_D35790541095__INCLUDED_)
