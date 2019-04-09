#include "stdafx.h"
#include "BalloonOutLine.h"
#include "./Interface/cImage.h"

cBalloonOutline::cBalloonOutline()
{
	m_nBalloonWidth = 0;
	m_nBalloonHeight = 0;

	m_fBorder = 8;
}

cBalloonOutline::~cBalloonOutline()
{
}

void cBalloonOutline::InitBalloon( cImage* lefttop, cImage* righttop, cImage* leftbottom, cImage* rightbottom,
							cImage* left, cImage* right, cImage* top, cImage* bottom, cImage* middle, cImage* tail, FLOAT border )
{
	if(lefttop)
	m_pImage[LeftTopCorner] = *lefttop;
	if(righttop)
	m_pImage[RightTopCorner] = *righttop;
	if(leftbottom)
	m_pImage[LeftBottomCorner] = *leftbottom;
	if(rightbottom)
	m_pImage[RightBottomCorner] = *rightbottom;
	if(left)
	m_pImage[LeftPole] = *left;
	if(right)
	m_pImage[RightPole] = *right;
	if(top)
	m_pImage[TopPole] = *top;
	if(bottom)
	m_pImage[BottomPole] = *bottom;
	if(middle)
	m_pImage[MiddleSp] = *middle;
	if(tail)
	m_pImage[Tail] = *tail;

	m_fBorder = border;

	m_nCurAlpha = 180;
}


void cBalloonOutline::InitBalloonForChat(cImage* pLeft1, cImage* pCenter1, cImage* pRight1,
										 cImage* pLeft2, cImage* pCenter2, cImage* pRight2,
										 cImage* pLeft3, cImage* pCenter3, cImage* pRight3, FLOAT border)
{
	if(pLeft1)
		m_pChatImage[LeftChatBalloon1] = *pLeft1;
	if(pCenter1)
		m_pChatImage[CenterChatBalloon1] = *pCenter1;
	if(pRight1)
		m_pChatImage[RightChatBalloon1] = *pRight1;
	if(pLeft2)
		m_pChatImage[LeftChatBalloon2] = *pLeft2;
	if(pCenter2)
		m_pChatImage[CenterChatBalloon2] = *pCenter2;
	if(pRight2)
		m_pChatImage[RightChatBalloon2] = *pRight2;
	if(pLeft3)
		m_pChatImage[LeftChatBalloon3] = *pLeft3;
	if(pCenter3)
		m_pChatImage[CenterChatBalloon3] = *pCenter3;
	if(pRight3)
		m_pChatImage[RightChatBalloon3] = *pRight3;
	
	m_fBorder = border;
}

// ÂïÈú ÁÂÇ¥¸¦ ³Ö¾îÁØ´Ù. (½ÃÀÛÁ¡)  
void cBalloonOutline::RenderOutline( float fSx, float fSy, float fTailX )
{
	VECTOR2	vPos, vScale;

	// »ó´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = (float) m_nBalloonWidth;
	vScale.y = 1;

	vPos.x = fSx;
	vPos.y = fSy;
	m_pImage[LeftTopCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_fBorder;
	m_pImage[TopPole].RenderSprite(&vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_nBalloonWidth;
	m_pImage[RightTopCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );


	// Áß´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = 1;
	vScale.y = (float) m_nBalloonHeight;

	vPos.x = fSx;
	vPos.y = fSy + m_fBorder;
	m_pImage[LeftPole].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vScale.x = (float) m_nBalloonWidth;
	vPos.x += m_fBorder;
	m_pImage[MiddleSp].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vScale.x = 1;
	vPos.x += m_nBalloonWidth;
	m_pImage[RightPole].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	// ÇÏ´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = (float) m_nBalloonWidth;
	vScale.y = 1;

	vPos.x = fSx;
	vPos.y = fSy + m_nBalloonHeight;
	m_pImage[LeftBottomCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_fBorder;
	m_pImage[BottomPole].RenderSprite(&vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += (float)m_nBalloonWidth;
	m_pImage[RightBottomCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x = fTailX;
	vPos.y = fSy + m_nBalloonHeight + m_fBorder*2;
	m_pImage[Tail].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );
}


//³ôÀÌ°ªÀ» ¹Þ¾Æ¼­ »ç¿ëÇÑ´Ù.(±âÁ¸ ³ôÀÌ°ª »ç¿ë¾ÈÇÔ)
void cBalloonOutline::RenderOutline( float fSx, float fSy, float fTailX, int nHeight )
{
	VECTOR2	vPos, vScale;

	// »ó´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = (float) m_nBalloonWidth;
	vScale.y = 1;

	vPos.x = fSx;
	vPos.y = fSy;
	m_pImage[LeftTopCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_fBorder;
	m_pImage[TopPole].RenderSprite(&vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_nBalloonWidth;
	m_pImage[RightTopCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );


	// Áß´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = 1;
	vScale.y = (float) nHeight;

	vPos.x = fSx;
	vPos.y = fSy + m_fBorder;
	m_pImage[LeftPole].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vScale.x = (float) m_nBalloonWidth;
	vPos.x += m_fBorder;
	m_pImage[MiddleSp].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vScale.x = 1;
	vPos.x += m_nBalloonWidth;
	m_pImage[RightPole].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	// ÇÏ´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = (float) m_nBalloonWidth;
	vScale.y = 1;

	vPos.x = fSx;
	vPos.y = fSy + nHeight + m_fBorder;
	m_pImage[LeftBottomCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += m_fBorder;
	m_pImage[BottomPole].RenderSprite(&vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x += (float)m_nBalloonWidth;
	m_pImage[RightBottomCorner].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	vPos.x = fTailX;
	vPos.y = fSy + nHeight + m_fBorder*2;
	m_pImage[Tail].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );
}


void cBalloonOutline::RenderOutlineSimple( float fSx, float fSy, float fTailX, int nHeight, BOOL bChat )
{
	VECTOR2	vPos, vScale;

	// »ó´Ü ÀÌ¹ÌÁö ±×¸®±â
	vScale.x = (float) m_nBalloonWidth;
	vScale.y = 1;

	vPos.x = fSx;
	vPos.y = fSy;
	
	m_pChatImage[(m_nLineNum*3)-3].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );
	vPos.x += m_fBorder;
	m_pChatImage[(m_nLineNum*3)-2].RenderSprite( &vScale, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );
	vPos.x += m_nBalloonWidth;
	m_pChatImage[(m_nLineNum*3)-1].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );

	// Ã¤ÆÃ¸¸ ²¿¸´¸» ´Þ¾Æ¾ß ÇÔ.
	if(bChat)
	{
		vPos.x = fTailX;
		vPos.y = fSy + nHeight + m_fBorder*2 - m_nLineNum;
		m_pImage[Tail].RenderSprite( NULL, NULL, 0, &vPos, RGBA_MERGE(0xffffff, m_nCurAlpha) );
	}
}
