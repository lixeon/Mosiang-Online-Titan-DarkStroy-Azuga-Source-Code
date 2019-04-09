// cBoard.h: interface for the cBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBOARD_H__093EB728_452E_457D_9DA5_085B449A6DDE__INCLUDED_)
#define AFX_CBOARD_H__093EB728_452E_457D_9DA5_085B449A6DDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

class cAuctionBoardContents;
class cAuctionBoardEditor;
class cButton;

struct BOARDDESC
{
	LONG x,y,wid,hei;
	LONG ID;
};

class cAuctionBoard : public cDialog  
{
	//////////////////////////////////////////////////////////////////////////
	// ÀÎÅÍÆäÀÌ½ºµé
	cAuctionBoardContents* m_pBoardContents;
	cAuctionBoardEditor* m_pBoardEditor;
	cButton* m_pBtnTitle[MAX_BOARDTITLE_NUM];

	cButton* m_pPageNumBtn[10];
	cButton* m_pBefore10Page;
	cButton* m_pAfter10Page;

	//////////////////////////////////////////////////////////////////////////
	// µ¥ÀÌÅÍµé
	WORD m_CurrentPage;
	WORD m_TotalPageNum;
	WORD m_StartPage;	// ¹Ø¿¡ ÀÖ´Â 10°³ÀÇ ÆäÀÌÁöµéÁß ½ÃÀÛÇÏ´Â ¼ýÀÚ.
	AUCTION_BOARDTITLE m_Title[MAX_BOARDTITLE_NUM];
public:
	cAuctionBoard();
	virtual ~cAuctionBoard();

	void SetCurrentPage(AUCTION_BOARDPAGE_INFO* pPageInfo);
	int GetCurrentPage()			{	return m_CurrentPage;	}
	
	AUCTION_BOARDTITLE* GetTitle(int TitleNum)	{	return &m_Title[TitleNum];	}

	void InitBoard(BOARDDESC* pDesc);

	void OnRecvContents(AUCTION_BOARDCONTENTS_INFO* pContentsInfo);
	void ShowBoardEditor(char* strTitle = NULL,char* strContents = NULL);

	inline WORD GetStartPage()	{	return m_StartPage;	}
};

#endif // !defined(AFX_CBOARD_H__093EB728_452E_457D_9DA5_085B449A6DDE__INCLUDED_)
