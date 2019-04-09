// cBoardEditor.h: interface for the cBoardEditor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBOARDEDITOR_H__6032E0AC_10D7_44F2_ABFF_DE418F387A58__INCLUDED_)
#define AFX_CBOARDEDITOR_H__6032E0AC_10D7_44F2_ABFF_DE418F387A58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cDialog.h"

class cEditBox;


class cAuctionBoardEditor  : public cDialog  
{
	cEditBox* m_pTitleEdit;
	cEditBox* m_pContentsEdit;

public:
	cAuctionBoardEditor();
	virtual ~cAuctionBoardEditor();

};

#endif // !defined(AFX_CBOARDEDITOR_H__6032E0AC_10D7_44F2_ABFF_DE418F387A58__INCLUDED_)
