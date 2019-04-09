// cBoardContents.h: interface for the cBoardContents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBOARDCONTENTS_H__47DEAF87_DEA3_49F0_B7E1_E3FF62DD17A1__INCLUDED_)
#define AFX_CBOARDCONTENTS_H__47DEAF87_DEA3_49F0_B7E1_E3FF62DD17A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"


class cMultiLineText;
class cStatic;


class cAuctionBoardContents : public cDialog  
{
	cStatic* m_pTitleText;
	cMultiLineText* m_pContentsText;
	
	AUCTION_BOARDCONTENTS_INFO* m_ContentsInfo;
	
public:
	cAuctionBoardContents();
	virtual ~cAuctionBoardContents();
	
	void SetContents(AUCTION_BOARDCONTENTS_INFO* pContentsInfo);

	friend void BO_Delete(LONG iId, void* p, DWORD we);
	friend void BO_Edit(LONG iId, void* p, DWORD we);
	friend void BO_Reply(LONG iId, void* p, DWORD we);
};

#endif // !defined(AFX_CBOARDCONTENTS_H__47DEAF87_DEA3_49F0_B7E1_E3FF62DD17A1__INCLUDED_)
