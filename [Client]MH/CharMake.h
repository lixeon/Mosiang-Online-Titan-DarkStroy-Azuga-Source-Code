// CharMake.h: interface for the CCharMake class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARMAKE_H__122C0E59_91B5_40F9_B325_54A36AA240FB__INCLUDED_)
#define AFX_CHARMAKE_H__122C0E59_91B5_40F9_B325_54A36AA240FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "cImageSelf.h"
#include "CharMakeCamera.h"

class cCharMakeDlg;

#define CHARMAKE USINGTON(CCharMake)

class CPlayer;

class CCharMake : public CGameState  
{
	//trustpak 2005/03/19
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	CCharMakeCamera*	m_pCharMakeCamera;

	//
	cCharMakeDlg*	m_pCharMakeDlg;

	// Dialog Pointer

	friend class cWindowManager;
	/*
	friend void CM_GengoalSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_SimmakSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DexSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_StaSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DuplCheckEditFunc(LONG lId, void * p, DWORD we);
	friend void CM_HeadChangeLBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_HeadChangeRBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FaceChangeLBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FaceChangeRBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_MuInBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_GwunSaBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DoJukBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_GungSaBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_SangInBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_EuWonBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_OkBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_MaleBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FemaleBtnFunc(LONG lId, void * p, DWORD we);
	*/
	friend extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
	friend extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
	friend extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
	friend extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
	friend extern void  CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);

public:
	CCharMake();
	virtual ~CCharMake();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	// LBS 03.10.29
	void SetCharMakeDialog(cCharMakeDlg* pDlg) { m_pCharMakeDlg = pDlg; }
	cCharMakeDlg* GetCharMakeDialog() { return m_pCharMakeDlg; }

	void DisplayNotice( int MsgNum );
	void SetDisableDlg( BOOL val );

//	void SetComboList(CharMakeCombo* pList);
};
EXTERNGLOBALTON(CCharMake)
#endif // !defined(AFX_CHARMAKE_H__122C0E59_91B5_40F9_B325_54A36AA240FB__INCLUDED_)
