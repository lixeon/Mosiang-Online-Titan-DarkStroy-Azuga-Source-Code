// cBoard.cpp: implementation of the cBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cBoard.h"
#include "cBoardContents.h"
#include "cBoardEditor.h"
#include "cButton.h"
#include "../GameIn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BOARDTITLE_COLOR		0xff000000
#define BOARDTITLE_OVERCOLOR	0xffffff00
#define BOARDTITLE_PRESSCOLOR	0xff00ffff

#define MAX_PAGELINK_NUM		10

cAuctionBoard::cAuctionBoard()
{

}

cAuctionBoard::~cAuctionBoard()
{

}

void cAuctionBoard::InitBoard(BOARDDESC* pDesc)
{

}

void cAuctionBoard::SetCurrentPage(AUCTION_BOARDPAGE_INFO* pPageInfo)
{
	int n;
	m_CurrentPage = pPageInfo->wCurPage;
	m_TotalPageNum = pPageInfo->wTotalPageNum;
	memcpy(&m_Title,pPageInfo->Title,sizeof(AUCTION_BOARDTITLE)*MAX_BOARDTITLE_NUM);
	for(n=0;n<MAX_BOARDTITLE_NUM;++n)
	{
		if(pPageInfo->Title[n].BidIdx)
		{
			m_pBtnTitle[n]->SetActive(TRUE);
//			m_pBtnTitle[n]->SetText(pPageInfo->Title[n].TitleName,
//				BOARDTITLE_COLOR,BOARDTITLE_OVERCOLOR,BOARDTITLE_PRESSCOLOR);
		}
		else
		{
			m_pBtnTitle[n]->SetActive(FALSE);
		}
	}

	// m_CurrentPage와 m_MaxPage에 따라서 밑에 있는 페이지 번호 등을 설정한다.
	m_StartPage = (m_CurrentPage / 10) * 10;
	WORD tempPage;
	char strPageNum[8];

	for(n=0;n<MAX_PAGELINK_NUM;++n)
	{
		tempPage = m_StartPage + n;
		if(tempPage >= m_TotalPageNum)
		{
			// 버튼을 그냥 안보이게 한다.
			m_pPageNumBtn[n]->SetActive(FALSE);
		}
		else
		{
			// 버튼을 보이게 한다.
			sprintf(strPageNum,"%d",tempPage+1);
			m_pPageNumBtn[n]->SetText(strPageNum,BOARDTITLE_COLOR,BOARDTITLE_OVERCOLOR,BOARDTITLE_PRESSCOLOR);
			m_pPageNumBtn[n]->SetActive(TRUE);
		}		
	}

	// 앞으로 10 페이지 넘김 버튼 설정
	if(m_StartPage == 1)
		m_pBefore10Page->SetActive(FALSE);
	else
		m_pBefore10Page->SetActive(TRUE);

	// 뒤로 10 페이지 넘김 버튼 설정
	if(m_StartPage+MAX_PAGELINK_NUM >= m_TotalPageNum)
		m_pAfter10Page->SetActive(FALSE);
	else
		m_pAfter10Page->SetActive(TRUE);
	

}

void cAuctionBoard::OnRecvContents(AUCTION_BOARDCONTENTS_INFO* pContentsInfo)
{
	m_pBoardContents->SetContents(pContentsInfo);
	m_pBoardContents->SetActive(TRUE);
}

void cAuctionBoard::ShowBoardEditor(char* strTitle,char* strContents)
{
	m_pBoardContents->SetActive(FALSE);
	m_pBoardEditor->SetActive(TRUE);
}


//////////////////////////////////////////////////////////////////////////
// 게시판
/*
	MP_AUCTIONBOARD_LIST_SYN,		// 페이지 번호를 넘긴다
	MP_AUCTIONBOARD_CONTENTS_SYN,	// 글번호를 넘긴다
	MP_AUCTIONBOARD_WRITE_SYN,		// 글의 내용을 넘긴다.
	MP_AUCTIONBOARD_DELETE_SYN,	// 글번호를 넘긴다.
	MP_AUCTIONBOARD_EDIT_SYN,		// 글번호와 내용을 넘긴다.
*/

extern DWORD gHeroID;

void BO_NextPage(LONG iId, void* p, DWORD we)
{
	cAuctionBoard* pBoard = (cAuctionBoard*)p;
	MSG_WORD msg;
	msg.Category = MP_AUCTIONBOARD;
	msg.Protocol = MP_AUCTIONBOARD_LIST_SYN;
	msg.dwObjectID = gHeroID;
	msg.wData = pBoard->GetCurrentPage() + 1;

	NETWORK->Send(&msg,sizeof(msg));
}
void BO_PrevPage(LONG iId, void* p, DWORD we)
{
	cAuctionBoard* pBoard = (cAuctionBoard*)p;
	
	if(pBoard->GetCurrentPage() == 0)
		return;


	MSG_WORD msg;
	msg.Category = MP_AUCTIONBOARD;
	msg.Protocol = MP_AUCTIONBOARD_LIST_SYN;
	msg.dwObjectID = gHeroID;
	msg.wData = pBoard->GetCurrentPage() - 1;

	NETWORK->Send(&msg,sizeof(msg));
}
void BO_Write(LONG iId, void* p, DWORD we)
{
	cAuctionBoard* pBoard = (cAuctionBoard*)p;

	pBoard->ShowBoardEditor(NULL);
}

// TitleNum은 [0, MAX_BOARDTITLE_NUM) 범위의 값
void OnPushedTitle(int TitleNum)
{
	AUCTION_BOARDTITLE* pTitle;
	pTitle = GAMEIN->GetAuctionBoard()->GetTitle(TitleNum);
	ASSERT(pTitle->BidIdx);

	MSG_DWORD msg;
	msg.Category = MP_AUCTIONBOARD;
	msg.Protocol = MP_AUCTIONBOARD_CONTENTS_SYN;
	msg.dwObjectID = gHeroID;
	msg.dwData = pTitle->BidIdx;

	NETWORK->Send(&msg,sizeof(msg));
}
void BO_View0(LONG iId, void* p, DWORD we){	OnPushedTitle(0);}
void BO_View1(LONG iId, void* p, DWORD we){	OnPushedTitle(1);}
void BO_View2(LONG iId, void* p, DWORD we){	OnPushedTitle(2);}
void BO_View3(LONG iId, void* p, DWORD we){	OnPushedTitle(3);}
void BO_View4(LONG iId, void* p, DWORD we){ OnPushedTitle(4);}
void BO_View5(LONG iId, void* p, DWORD we){ OnPushedTitle(5);}
void BO_View6(LONG iId, void* p, DWORD we){ OnPushedTitle(6);}
void BO_View7(LONG iId, void* p, DWORD we){ OnPushedTitle(7);}
void BO_View8(LONG iId, void* p, DWORD we){ OnPushedTitle(8);}
void BO_View9(LONG iId, void* p, DWORD we){ OnPushedTitle(9);}


void OnPushedPageNum(int Page)
{
	WORD StartPage = GAMEIN->GetAuctionBoard()->GetStartPage();

	MSG_WORD msg;
	msg.Category = MP_AUCTIONBOARD;
	msg.Protocol = MP_AUCTIONBOARD_LIST_SYN;
	msg.dwObjectID = gHeroID;
	msg.wData = StartPage + Page;

	NETWORK->Send(&msg,sizeof(msg));	
}
void BO_Page0(LONG iId, void* p, DWORD we){ OnPushedPageNum(0);}
void BO_Page1(LONG iId, void* p, DWORD we){ OnPushedPageNum(1);}
void BO_Page2(LONG iId, void* p, DWORD we){ OnPushedPageNum(2);}
void BO_Page3(LONG iId, void* p, DWORD we){ OnPushedPageNum(3);}
void BO_Page4(LONG iId, void* p, DWORD we){ OnPushedPageNum(4);}
void BO_Page5(LONG iId, void* p, DWORD we){ OnPushedPageNum(5);}
void BO_Page6(LONG iId, void* p, DWORD we){ OnPushedPageNum(6);}
void BO_Page7(LONG iId, void* p, DWORD we){ OnPushedPageNum(7);}
void BO_Page8(LONG iId, void* p, DWORD we){ OnPushedPageNum(8);}
void BO_Page9(LONG iId, void* p, DWORD we){ OnPushedPageNum(9);}


//////////////////////////////////////////////////////////////////////////
