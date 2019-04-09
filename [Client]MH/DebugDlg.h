// DebugDlg.h: interface for the CDebugDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGDLG_H__C57B5240_F050_4048_B9C0_CA00DB93E024__INCLUDED_)
#define AFX_DEBUGDLG_H__C57B5240_F050_4048_B9C0_CA00DB93E024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cListDialog.h"

enum
{
	DBG_ATTACK,
	DBG_ITEM,
	DBG_MOVE,
	DBG_MUGONG,	
	DBG_CHAT,
	DBG_USERCONN,
};

class CDebugDlg : public cListDialog  
{
	BOOL m_bAttackFlag;
	BOOL m_bItemFlag;
	BOOL m_bMoveFlag;
	BOOL m_bMugongFlag;
	BOOL m_bChatFlag;
	BOOL m_bUserConnFlag;

public:
	CDebugDlg();
	virtual ~CDebugDlg();

	void DebugMsgParser(BYTE type, char* msg, ...);

	void SetAttackBtnFlag(BOOL flag){m_bAttackFlag = flag;}
	void SetItemBtnFlag(BOOL flag){m_bItemFlag = flag;}
	void SetMoveBtnFlag(BOOL flag){m_bMoveFlag = flag;}
	void SetMugongBtnFlag(BOOL flag){m_bMugongFlag = flag;}
	void SetChatBtnFlag(BOOL flag){m_bChatFlag = flag;}
	void SetUserConnBtnFlag(BOOL flag){m_bUserConnFlag = flag;}
	
	BOOL GetAttackBtnFalg(){return m_bAttackFlag;}
	BOOL GetItemBtnFlag(){return m_bItemFlag;}
	BOOL GetMoveBtnFlag(){return m_bMoveFlag;}
	BOOL GetMugongBtnFlag(){return m_bMugongFlag;}
	BOOL GetChatBtnFlag(){return m_bChatFlag;}
	BOOL GetUserConnBtnFlag(){return m_bUserConnFlag;}
};

#endif // !defined(AFX_DEBUGDLG_H__C57B5240_F050_4048_B9C0_CA00DB93E024__INCLUDED_)
