// QuickManager.h: interface for the CQuickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
#define AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndexGenerator.h"

#define QUICKMGR USINGTON(CQuickManager)

class cIcon;
class CItem;
class cImage;
class CQuickItem;
class CBaseItem;
class CQuickManager  
{
//	int m_selectedQuickAbsPos;	// 스킬 매니저에서 마지막으로 사용된 무공정보를 가지고 있으므로
								// 더이상 필요 없음
	CYHHashTable<CQuickItem> m_QuickHashForID;
public:
	//MAKESINGLETON(CQuickManager);
	CQuickManager();
	virtual ~CQuickManager();

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
	BOOL CanEquip(cIcon * pIcon);
	void Release();
	CQuickItem * GetQuickItem(DWORD dwDBIdx);
	
	void UseQuickItem(POSTYPE pos);
	
//	int	GetSelectedQuickAbsPos() { return m_selectedQuickAbsPos; } m_selectedQuickAbsPos를 더이상 사용안함

	void MoveQuickItemReal( POSTYPE FromSrcPos, WORD wFromSrcItemIdx, POSTYPE FromQuickPos, POSTYPE ToSrcPos, WORD wToSrcItemIdx, POSTYPE ToQuickPos, bool DeleteToQuickPos);
	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	BOOL AddQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx, POSTYPE OldSrcPos=0, WORD OldSrcItemIdx=0);
	// 06. 01 이영준 - 단축창 변경
	// 퀵아이템 초기 설정 함수
	BOOL SetQuickItemReal(POSTYPE QuickPos, POSTYPE SrcPos, WORD SrcItemIdx);
	// 삭제할 퀵포지션 값을 인자로 넘겨줘야 한다
	void RemoveQuickItemReal(POSTYPE SrcPos, WORD wSrcItemIdx, POSTYPE QuickPos);
	void OverlapCheck(POSTYPE SrcPos);

	void RefreshQickItem();
	void RemQuickItem(POSTYPE QuickPos);

	BOOL CheckQPosForItemIdx( WORD ItemIdx );
	void ReleaseQuickItem(CQuickItem * quick);

	void SetQuickItem(POSTYPE QuickPos, CItem * pItem);
private:
	CQuickItem * NewQuickItem(CBaseItem * pSrcIcon);
	CIndexGenerator m_IconIndexCreator;
	void GetImage( CQuickItem * pQuickItem );
};
EXTERNGLOBALTON(CQuickManager);
#endif // !defined(AFX_QUICKMANAGER_H__B3C070DF_4FEC_4F04_BD3C_8F534A23A442__INCLUDED_)
