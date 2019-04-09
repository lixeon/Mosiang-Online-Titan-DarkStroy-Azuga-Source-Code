// AuctionContents.cpp: implementation of the CAuctionContents class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuctionContents.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuctionContents::CAuctionContents()
{

}

CAuctionContents::~CAuctionContents()
{

}

void CAuctionContents::InitAuctionContents(AUCTION_NEW_BOARDCONTENTS* pNewContensInfo)
{
	m_ItemIdx = pNewContensInfo->ItemIdx;
	m_ItemNum = pNewContensInfo->ItemNum;
	m_SellerID = pNewContensInfo->SellerID;
	m_EndDate = pNewContensInfo->EndDate;
	m_EndTime = pNewContensInfo->EndTime;
	m_StartPrice = pNewContensInfo->StartPrice;
	m_ImmediatePrice = pNewContensInfo->ImmediatePrice;
	//strcpy(m_Memo,pNewContensInfo->Memo);
	SafeStrCpy( m_Memo,pNewContensInfo->Memo, 256 );


	m_CurrentPrice = m_StartPrice;
	m_BidderNum = 0;
	m_CurrentBidder = 0;
}

void CAuctionContents::InitAuctionContents(AUCTION_BOARDCONTENTS_INFO* pContensInfo)
{
	m_ItemIdx = pContensInfo->ItemIdx;
	m_ItemNum = pContensInfo->ItemNum;
	m_SellerID = pContensInfo->SellerID;
	m_EndDate = pContensInfo->EndDate;
	m_EndTime = pContensInfo->EndTime;
	m_StartPrice = pContensInfo->StartPrice;
	m_ImmediatePrice = pContensInfo->ImmediatePrice;
//	strcpy(m_Memo,pContensInfo->Memo);
	SafeStrCpy( m_Memo,pContensInfo->Memo,256 );

	m_CurrentPrice = pContensInfo->CurrentPrice;
	m_BidderNum = pContensInfo->BidderNum;
	m_CurrentBidder = pContensInfo->CurrentBidder;
}

void CAuctionContents::Bid(DWORD dwBidderID,DWORD dwBiddingPrice)
{
	ASSERT(dwBiddingPrice > m_CurrentPrice);
	ASSERT(dwBidderID != m_CurrentBidder);

	// 즉구가에 도달했는지 검사
	ASSERT(m_ImmediatePrice >= dwBiddingPrice);
	if(m_ImmediatePrice == dwBiddingPrice)
	{
		// 경매종료하구 어쩌구 저쩌구... -_-a;
	}

	++m_BidderNum;
	m_CurrentBidder = dwBidderID;
	m_CurrentPrice = dwBiddingPrice;

	//DB에 업데이트 한다

	//방금 
}

void CAuctionContents::SetDataToStruct(AUCTION_BOARDTITLE* pTitle)
{
	pTitle->BidIdx = m_BidIdx;
	pTitle->ItemIdx = m_ItemIdx;
	pTitle->ItemNum = m_ItemNum;
	pTitle->EndDate = m_EndDate;
	pTitle->EndTime = m_EndTime;
	pTitle->CurrentPrice = m_CurrentPrice;
	pTitle->ImmediatePrice = m_ImmediatePrice;
}