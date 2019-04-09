// cBoardContents.cpp: implementation of the cBoardContents class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cBoard.h"
#include "cBoardContents.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cAuctionBoardContents::cAuctionBoardContents()
{

}

cAuctionBoardContents::~cAuctionBoardContents()
{

}

void cAuctionBoardContents::SetContents(AUCTION_BOARDCONTENTS_INFO* pContentsInfo)
{
	memcpy(&m_ContentsInfo,pContentsInfo,sizeof(AUCTION_BOARDCONTENTS_INFO));
	//m_pTitleText->SetStaticText(pContentsInfo->TitleName);
	//m_pContentsText->SetText(pContentsInfo->strContents);
}



//////////////////////////////////////////////////////////////////////////
// °Ô½ÃÆÇ
/*
	MP_AUCTIONBOARD_LIST_SYN,		// ÆäÀÌÁö ¹øÈ£¸¦ ³Ñ±ä´Ù
	MP_AUCTIONBOARD_CONTENTS_SYN,	// ±Û¹øÈ£¸¦ ³Ñ±ä´Ù
	MP_AUCTIONBOARD_WRITE_SYN,		// ±ÛÀÇ ³»¿ëÀ» ³Ñ±ä´Ù.
	MP_AUCTIONBOARD_DELETE_SYN,	// ±Û¹øÈ£¸¦ ³Ñ±ä´Ù.
	MP_AUCTIONBOARD_EDIT_SYN,		// ±Û¹øÈ£¿Í ³»¿ëÀ» ³Ñ±ä´Ù.
*/
extern DWORD gHeroID;

void BO_Delete(LONG iId, void* p, DWORD we)
{
	cAuctionBoardContents* pContents = (cAuctionBoardContents*)p;
	
	MSG_DWORD msg;
	msg.Category = MP_AUCTIONBOARD;
	msg.Protocol = MP_AUCTIONBOARD_DELETE_SYN;
	msg.dwObjectID = gHeroID;
	msg.dwData = pContents->m_ContentsInfo->BidIdx;

	NETWORK->Send(&msg,sizeof(msg));
}/*
void BO_Edit(LONG iId, void* p, DWORD we)
{
	cAuctionBoardContents* pContents = (cAuctionBoardContents*)p;
	cAuctionBoard* pBoard = (cAuctionBoard*)pContents->m_pParent;

	pBoard->ShowBoardEditor(pContents->m_ContentsInfo->TitleName,
							pContents->m_ContentsInfo->strContents);
}*/
void BO_Reply(LONG iId, void* p, DWORD we)
{
	cAuctionBoardContents* pContents = (cAuctionBoardContents*)p;
	cAuctionBoard* pBoard = (cAuctionBoard*)pContents->m_pParent;

	char temp[256];
//	sprintf(temp,"[re]%s",pContents->m_ContentsInfo->TitleName);
	pBoard->ShowBoardEditor(temp);
}