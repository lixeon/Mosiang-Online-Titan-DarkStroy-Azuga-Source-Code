#ifndef _BALLOON_OUTLINE_
#define _BALLOON_OUTLINE_

///////////////////////////////////////////////////////////////////////////////////
// LBS 03.11.5
// ¸»Ç³¼±ÀÇ ¿Ü°ûÀ» Âï´Âµ¥ »ç¿ëÇÑ´Ù.
///////////////////////////////////////////////////////////////////////////////////

#include "./interface/cwindowdef.h"
#include "./interface/cImage.h"

enum { LeftTopCorner = 0, TopPole = 1, RightTopCorner = 2, 
	   LeftPole = 3, MiddleSp = 4, RightPole = 5, 
	   LeftBottomCorner = 6, BottomPole = 7, RightBottomCorner = 8, 
	   Tail = 9, MAX_BOI_NUM = 10, };

enum {
	LeftChatBalloon1=0,
	CenterChatBalloon1,
	RightChatBalloon1,
	LeftChatBalloon2,
	CenterChatBalloon2,
	RightChatBalloon2,
	LeftChatBalloon3,
	CenterChatBalloon3,
	RightChatBalloon3,
	MaxChatBalloon,
};

class cImage;

class cBalloonOutline 
{
protected:
	cImage			m_pImage[MAX_BOI_NUM];
	cImage			m_pChatImage[MaxChatBalloon];

	int				m_nBalloonWidth;
	int				m_nBalloonHeight;
	int				m_nLineNum;

	FLOAT			m_fBorder;

	int				m_nCurAlpha;

public:
	cBalloonOutline();
	~cBalloonOutline();

	void InitBalloon( cImage* lefttop, cImage* righttop, cImage* leftbottom, cImage* rightbottom,
			   cImage* left, cImage* right, cImage* top, cImage* bottom, cImage* middle, cImage* tail, FLOAT border );
	void InitBalloonForChat(cImage* pLeft1, cImage* pCenter1, cImage* pRight1,
										 cImage* pLeft2, cImage* pCenter2, cImage* pRight2,
										 cImage* pLeft3, cImage* pCenter3, cImage* pRight3, FLOAT border);

	virtual void RenderOutline( float fSx, float fSy, float fTailX );
	virtual void RenderOutline( float fSx, float fSy, float fTailX, int nHeight );
	virtual void RenderOutlineSimple( float fSx, float fSy, float fTailX, int nHeight, BOOL bChat=FALSE );

	void SetBalloonSize( int nWidth, int nHeight, int nLineNum ) { m_nBalloonWidth = nWidth; m_nBalloonHeight = nHeight; m_nLineNum = nLineNum;	}
	void SetCurAlhpa( int alpha ) { m_nCurAlpha = alpha; }
	
	FLOAT	GetBalloonBorder() { return m_fBorder; }
};

#endif //_BALLOON_OUTLINE_