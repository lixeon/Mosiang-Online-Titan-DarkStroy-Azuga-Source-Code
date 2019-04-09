// AuctionContents.h: interface for the CAuctionContents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTIONCONTENTS_H__FC961A38_5D9D_458C_9AD3_5161A857A154__INCLUDED_)
#define AFX_AUCTIONCONTENTS_H__FC961A38_5D9D_458C_9AD3_5161A857A154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

class CAuctionContents  
{
	DWORD m_BidIdx;		// °æ¸Å¹øÈ£(¸Ê¼­¹ö¿¡¼­ °ü¸®ÇÏ´Â PrimaryKey)
	WORD m_ItemIdx;
	WORD m_ItemNum;
	DWORD m_SellerID;
	DWORD m_EndDate;
	DWORD m_EndTime;
	DWORD m_StartPrice;
	DWORD m_ImmediatePrice;
	char m_Memo[256];

	DWORD m_CurrentPrice;
	DWORD m_BidderNum;
	DWORD m_CurrentBidder;


public:
	CAuctionContents();
	virtual ~CAuctionContents();

	void SetBidIdx(DWORD BidIdx)	{	m_BidIdx = BidIdx;	}
	DWORD GetBidIdx()	{	return m_BidIdx;	}

	void SetDataToStruct(AUCTION_BOARDTITLE* pTitle);

	void InitAuctionContents(AUCTION_NEW_BOARDCONTENTS* pNewContensInfo);	// Ã³À½¸¸µé¾îÁú¶§
	void InitAuctionContents(AUCTION_BOARDCONTENTS_INFO* pContensInfo);	// DB¿¡¼­ ¹Þ¾Æ¿Í¼­
	
	void Bid(DWORD dwBidderID,DWORD dwBiddingPrice);
};

#endif // !defined(AFX_AUCTIONCONTENTS_H__FC961A38_5D9D_458C_9AD3_5161A857A154__INCLUDED_)
