// QuickManager.h: interface for the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKMANAGER_H__81B555F7_56A1_47A6_93C7_D3F6D13C4EA6__INCLUDED_)
#define AFX_QUICKMANAGER_H__81B555F7_56A1_47A6_93C7_D3F6D13C4EA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUICKMNGR CQuickManager::GetInstance()

class CPlayer;
enum QUPDATE_BIT{ QUB_ICONIDX=1, QUB_KINDS=2, QUB_SRCPOS=4, QUB_ALL=7, };
class CQuickManager  
{
	CQuickManager();
public:
	// 06. 01 이영준 - 단축창 변경
	//------------------------------------------------------
	// 퀵포지션 연산 함수들
	static void AddQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos);
	static void DeleteQuickPosition(POSTYPE& QuickPos, WORD SheetNum);
	static WORD GetQuickPosition(POSTYPE QuickPos, WORD SheetNum);
	static void AnalyzeQuickPosition(POSTYPE QuickPos, WORD* QuickPosList);
	static void UpdateQuickPosition(POSTYPE& QuickPos, WORD SheetNum, WORD Pos);
	static POSTYPE MergeQuickPosition(WORD* QuickPosList);
	// 어빌리티들은 DB기록방식이 다르다!
	static POSTYPE GetAbilityQuickPosition(BYTE Kind, BYTE Pos, ABILITY_TOTALINFO* pOutAbilityTotalInfo);
	static void AddAbilityQuickPosition(BYTE Kind, BYTE Pos, POSTYPE QuickPos, ABILITY_TOTALINFO* pOutAbilityTotalInfo);
	//------------------------------------------------------

	virtual ~CQuickManager();
	GETINSTANCE(CQuickManager);
	void SendErrorMsg( CPlayer * pPlayer, MSG_QUICK_ERROR * msg, int msgSize, int ECode );
	void SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize);
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	// 06. 01 이영준 - 단축창 변경
	// 프로토콜 변경 및 함수 인자값 추가
//	BOOL AddQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx, POSTYPE OldSrcPos, WORD wOldSrcItemIdx);
//	BOOL RemQuickItem(CPlayer * pPlayer, WORD SrcPos, WORD wSrcItemIdx);
//	BOOL MoveQuickItem(CPlayer * pPlayer, POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos );
	BOOL AddQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx, POSTYPE SrcQuickPos, POSTYPE OldSrcPos, WORD wOldSrcItemIdx, POSTYPE OldSrcQuickPos);
	BOOL RemQuickItem(CPlayer * pPlayer, WORD SrcPos, WORD wSrcItemIdx, POSTYPE QuickPos, POSTYPE SrcQuickPos);
	BOOL MoveQuickItem(CPlayer * pPlayer, POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE FromSrcQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos, POSTYPE ToSrcQuickPos, bool DeleteToSrcQuickPos);
	// 수량 아이템 삭제시 퀵포지션 재설정에 필요
	BOOL SetQuickItem( CPlayer * pPlayer, POSTYPE QuickPos, POSTYPE srcPos, WORD wSrcItemIdx );
	/*
	
		void RemQuickItem( CPlayer * pPlayer, MSG_QUICK_REM_SYN * pmsg );
		void RemQuickItem( CPlayer * pPlayer, WORD tableIdx, WORD absPosition );
		void MoveQuickItem( CPlayer * pPlayer, MSG_QUICK_MOVE_SYN * pmsg );
	
		void UpdateSrcPosition( CPlayer * pPlayer, POSTYPE QAbsPos, POSTYPE srcAbsPos );
		void PureUpdateQuickItem(CPlayer * pPlayer, WORD whatQuickAbsPos, DWORD dwIconIdx, WORD eIGKinds, WORD srcAbsPos, WORD flag=QUB_ALL);
	
	private:
		void PureRemQuickItemAbs(CPlayer * pPlayer, WORD abs_pos);*/
	
};

#endif // !defined(AFX_QUICKMANAGER_H__81B555F7_56A1_47A6_93C7_D3F6D13C4EA6__INCLUDED_)
