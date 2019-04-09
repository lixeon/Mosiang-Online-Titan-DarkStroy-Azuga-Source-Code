#include "stdafx.h"
#include "cTextArea.h"

#include "../cIMEex.h"
#include "cFont.h"
#include "cButton.h"
#include "cGuageBar.h"
#include "../Input/cIMEWnd.h"
#include "../Input/Mouse.h"
#include "../Interface/cWindowManager.h"

cTextArea::cTextArea()
{
	m_nTopLineIdx	= 0;
	m_nLineNum		= 0;
	m_nLineHeight	= 0;
	SetRect( &m_rcTextRelRect, 0, 0, 0, 0 );

	m_type			= WT_TEXTAREA;
	m_pUpwardBtn	= NULL;
	m_pDownwardBtn	= NULL;
	m_pGuageBar		= NULL;

	m_pIMEex		= NULL;
	m_bFocus		= FALSE;
	m_bReadOnly		= FALSE;

	m_bCaret		= FALSE;
	m_MiddleScale.x = m_MiddleScale.y = 1;

	m_bUseBorderImage = FALSE;

	m_dwTextColor = 0x00ffffff;
}

cTextArea::~cTextArea()
{
	if(m_pIMEex)
	{
		m_pIMEex->Release();
		delete m_pIMEex;
		m_pIMEex = NULL;
	}
}

void cTextArea::InitTextArea( RECT* pTextRelRect, int nBufSize, cImage* topimg, WORD tophei, cImage* middleimg, WORD middlehei, cImage* downimg, WORD downhei  )
{
	CopyRect( &m_rcTextRelRect, pTextRelRect );

	m_nLineHeight	= LINE_HEIGHT;
	m_nLineNum		= ( m_rcTextRelRect.bottom - m_rcTextRelRect.top ) / m_nLineHeight;

	m_pIMEex = new cIMEex;
	m_pIMEex->Init( nBufSize, m_rcTextRelRect.right - m_rcTextRelRect.left, TRUE );
//프로시져 문제로 '(작은따옴표) 못들어가도록.
	m_pIMEex->SetValidCheckMethod( VCM_DBPARAM );
//
	m_TopImage = *topimg; m_topHeight = tophei;
	m_MiddleImage = *middleimg; m_middleHeight = middlehei;
	m_DownImage = *downimg; m_downHeight = downhei;

	m_bUseBorderImage = TRUE;
}

void cTextArea::InitTextArea( RECT* pTextRelRect, int nBufSize )
{
	CopyRect( &m_rcTextRelRect, pTextRelRect );

	m_nLineHeight	= LINE_HEIGHT;
	m_nLineNum		= ( m_rcTextRelRect.bottom - m_rcTextRelRect.top ) / m_nLineHeight;

	m_pIMEex = new cIMEex;
	m_pIMEex->Init( nBufSize, m_rcTextRelRect.right - m_rcTextRelRect.left, TRUE );
}

DWORD cTextArea::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cWindow::ActionEvent(mouseInfo);

	if( we & WE_LBTNCLICK )
	{
		SetFocusEdit(TRUE);
	}
	else if(mouseInfo->LButtonDown())
	{
		SetFocusEdit(FALSE);
	}

	DWORD we2 = NULL;
	if(m_pUpwardBtn)		we2 = m_pUpwardBtn->ActionEvent(mouseInfo);
	if(we2 & WE_BTNCLICK)	OnUpwardItem();

	if(m_pDownwardBtn)		we2 = m_pDownwardBtn->ActionEvent(mouseInfo);
	if(we2 & WE_BTNCLICK)	OnDownwardItem();

	if(m_pGuageBar)			m_pGuageBar->ActionEvent(mouseInfo);

	LONG cnt = m_pIMEex->GetTotalLine();
	int itemCnt = cnt - m_nLineNum;

	if(itemCnt > 0)
	{
		if(m_pGuageBar)
		if(m_pGuageBar->IsDrag() )
		{
			m_nTopLineIdx = (int)((float)(itemCnt) * m_pGuageBar->GetCurRate());
		}
	}
	else
	{
		m_nTopLineIdx = 0;
		if( m_pGuageBar ) m_pGuageBar->SetCurValue(0);
	}

	if( m_pIMEex->IsChanged() )
	{

		if( m_pIMEex->GetInsertPos()->nLine < m_nTopLineIdx )
		{
			m_nTopLineIdx = m_pIMEex->GetInsertPos()->nLine;

			if( m_pGuageBar )
				m_pGuageBar->SetCurRate( (float)m_nTopLineIdx / (float)itemCnt );
		}
		else if( m_pIMEex->GetInsertPos()->nLine >= m_nTopLineIdx + m_nLineNum )
		{
			m_nTopLineIdx = m_pIMEex->GetInsertPos()->nLine - m_nLineNum + 1;

			if( m_pGuageBar )
				m_pGuageBar->SetCurRate( (float)m_nTopLineIdx / (float)itemCnt );
		}
	}


	return we;
}

void cTextArea::Render()
{
	cDialog::Render();

	if(m_pGuageBar)	
		m_pGuageBar->Render();

	char strBuf[256];
	int nStrLen;
	RECT rcText;

	// 테두리 찍기 (border를 나중에 넣을수 있도록 한다. test)
	if( m_bUseBorderImage )
	{
		VECTOR2 start_pos;
		start_pos.x = m_absPos.x;
		start_pos.y = m_absPos.y + m_rcTextRelRect.top - m_topHeight;
		m_TopImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		
		start_pos.y += m_topHeight;
		m_MiddleImage.RenderSprite(&m_MiddleScale,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		
		start_pos.y += m_middleHeight*m_MiddleScale.y;
		m_DownImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
	
	if( m_bFocus && !m_bReadOnly )
	{
		for( int i = 0 ; i < m_nLineNum ; ++i )
		{
			if( m_pIMEex->GetLineText( m_nTopLineIdx + i, strBuf, &nStrLen ) )
			{
				if( nStrLen )
				{
					SetRect( &rcText, (LONG)m_absPos.x + m_rcTextRelRect.left,
						(LONG)m_absPos.y + m_rcTextRelRect.top + ( m_nLineHeight * i ), 1, 1 );
				
					//텍스트 칼라도 지정가능하도록
					CFONT_OBJ->RenderFont( m_wFontIdx, strBuf, nStrLen, &rcText, 
//						RGBA_MERGE( 0x00ffffff, m_alpha * m_dwOptionAlpha / 100 ) );
						RGBA_MERGE( m_dwTextColor, m_alpha * m_dwOptionAlpha / 100 ) );
				}
			}			
		}
		
		m_dwCaretCurTick = GetTickCount();
		
		if( m_dwCaretCurTick - m_dwCaretLastTick >= 500 )
		{
			m_bCaret = !m_bCaret;
			while( m_dwCaretCurTick - m_dwCaretLastTick >= 500 )
				m_dwCaretLastTick += 500;
		}
		
		if( m_bCaret )
		{
			if( m_pIMEex->GetInsertPos()->nLine < m_nTopLineIdx ||
				m_pIMEex->GetInsertPos()->nLine >= m_nTopLineIdx + m_nLineNum )
				return;

			m_pIMEex->GetLineText( m_pIMEex->GetInsertPos()->nLine, strBuf );
			LONG lExtent = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex );
			
			SetRect( &rcText, 
				(LONG)m_absPos.x + lExtent + m_rcTextRelRect.left - 2,
				(LONG)m_absPos.y + m_rcTextRelRect.top
							+ ( m_pIMEex->GetInsertPos()->nLine - m_nTopLineIdx ) * LINE_HEIGHT,
				1, 1 );
			
			CFONT_OBJ->RenderFont( m_wFontIdx, "|", 1, &rcText, 
//				RGBA_MERGE( 0x00ffffff, m_alpha * m_dwOptionAlpha / 100 ) );
				RGBA_MERGE( m_dwTextColor, m_alpha * m_dwOptionAlpha / 100 ) );
		}
	}
	else
	{
		for( int i = 0 ; i < m_nLineNum ; ++i )
		{
			if( m_pIMEex->GetLineText( m_nTopLineIdx + i, strBuf, &nStrLen ) )
			{
				if( nStrLen )
				{
					SetRect( &rcText, (LONG)m_absPos.x + m_rcTextRelRect.left,
						(LONG)m_absPos.y + m_rcTextRelRect.top + ( m_nLineHeight * i ), 1, 1 );
				
					//텍스트 칼라도 지정가능하도록
					CFONT_OBJ->RenderFont( m_wFontIdx, strBuf, nStrLen, &rcText, 
//						RGBA_MERGE( 0x00ffffff, m_alpha * m_dwOptionAlpha / 100 ) );
						RGBA_MERGE( m_dwTextColor, m_alpha * m_dwOptionAlpha / 100 ) );
				}
			}
		}
	}
}

void cTextArea::Add(cWindow * window)
{
	if(!m_pUpwardBtn)			m_pUpwardBtn	= (cButton *)window;
	else if(!m_pDownwardBtn)	m_pDownwardBtn	= (cButton *)window;
	else if(!m_pGuageBar)		m_pGuageBar		= (cGuageBar *)window;
	else						ASSERT(0);

	cDialog::Add(window);
}



void cTextArea::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	
	cWindow::SetActive( val );
	
	if( !val )
		SetFocusEdit( FALSE );
}


void cTextArea::SetFocus( BOOL val )
{
	cWindow::SetFocus( val );
}

void cTextArea::SetFocusEdit(BOOL val)
{
	if( m_bFocus == val ) return;

	if( val && !m_bActive ) return;//엑티브가 아닌 상태에서는 포커스를 가지면 안된다.

	SetFocus(val);

	if(val)
	{
		if(!m_bReadOnly)
		{
			CIMEWND->SetDockingEx(m_pIMEex, TRUE);
			m_bCaret = TRUE;
			m_dwCaretLastTick = m_dwCaretCurTick = GetTickCount();

			WINDOWMGR->SetFocusedEdit( this, TRUE );
		}
		else
		{
			m_bCaret = FALSE;
		}
	}
	else
	{
		m_bCaret = FALSE;
		CIMEWND->SetDockingEx(m_pIMEex, FALSE);

		WINDOWMGR->SetFocusedEdit( this, FALSE );
	}
}
	
void cTextArea::OnUpwardItem()		//위로 스크롤
{
	LONG cnt = m_pIMEex->GetTotalLine();
	if( cnt < m_nLineNum )	return;
	
	if( m_nTopLineIdx < 1 )	return;

	--m_nTopLineIdx;

	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_nTopLineIdx/(float)(cnt-m_nLineNum) );

}
void cTextArea::OnDownwardItem()		//밑으로 스크롤
{
	LONG cnt = m_pIMEex->GetTotalLine();
	if(cnt < m_nLineNum ) return;

	if( cnt - m_nTopLineIdx - 1 < m_nLineNum ) return;

	++m_nTopLineIdx;

	if( m_pGuageBar )
	m_pGuageBar->SetCurRate( (float)m_nTopLineIdx/(float)(cnt-m_nLineNum) );
}


void cTextArea::GetScriptText( char* outText )
{
	m_pIMEex->GetScriptText( outText );
}

void cTextArea::SetScriptText( char* inText )
{
	if( m_pIMEex )
		m_pIMEex->SetScriptText( inText );

//	if( m_bReadOnly )
//		SetCaretMoveFirst();
}

void cTextArea::SetCaretMoveFirst()
{
	if( m_pIMEex )
		m_pIMEex->CaretMoveFirst();
}

BOOL cTextArea::SetLimitLine( int nMaxLine )
{
	if( m_pIMEex )
		return m_pIMEex->SetLimitLine( nMaxLine );

	return FALSE;
}

/*
void cTextArea::SetValidCheck( int nSetNum )
{
//KES INTERFACE 031013
	m_pIMEex->SetValidCheckMethod( nSetNum );
}
*/

void cTextArea::SetEnterAllow( BOOL bAllow )
{
	if( m_pIMEex )
		m_pIMEex->SetEnterAllow( bAllow );
}


void cTextArea::GetCaretPos( LONG* X, LONG* Y )
{
	LONG TxtPosX = (LONG)m_absPos.x + m_rcTextRelRect.left;
	LONG TxtPosY = (LONG)m_absPos.y + m_rcTextRelRect.top - m_topHeight;

	*X = TxtPosX + m_pIMEex->GetInsertPos()->nIndex * CFONT_OBJ->GetTextWidth( m_wFontIdx );
	*Y = TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT;
/*
	char strBuf[256] = {0,};
	int nStrLen = 0;

	if( m_pIMEex->GetLineText( m_pIMEex->GetInsertPos()->nLine, strBuf, &nStrLen ) == FALSE )
	{
		*X = 0;
		*Y = 0;
		return;
	}

	if( m_nAlign & TXT_LEFT )
	{
		TxtPosX = (LONG)m_absPos.x + m_rcTextRelRect.left;
	}
	else if( m_nAlign & TXT_RIGHT )
	{
		TxtPosX = (LONG)m_absPos.x + m_width 
			- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) - m_rcTextRelRect.right;
		
	}
	else	//TXT_CENTER
	{
		TxtPosX = (LONG)m_absPos.x + ( m_width 
			- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) ) / 2;
	}

	LONG lExtent = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex );

	*X = TxtPosX + lExtent - 2;
	*Y = TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT;			
*/
}
