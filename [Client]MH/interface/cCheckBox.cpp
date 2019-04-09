// cCheckBox.cpp: implementation of the cCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cCheckBox.h"
#include "cFont.h"
#include "./Input/Mouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cCheckBox::cCheckBox()
{
	memset(m_szCheckBoxText, 0, MAX_CHECKBOXTEXT_SIZE);
	m_dwCheckBoxTextColor=RGB_HALF(255,255,255);
	m_fChecked = FALSE;
	m_type = WT_CHECKBOX;
}

cCheckBox::~cCheckBox()
{

}

void cCheckBox::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * checkBoxImage, cImage * checkImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage, ID);
	m_type = WT_CHECKBOX;
	m_CheckBoxImage = *checkBoxImage;
	m_CheckImage = *checkImage;
	if(Func != NULL)
		cbWindowFunc = Func;
}
DWORD cCheckBox::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cWindow::ActionEvent(mouseInfo);

	if( m_bDisable )
	{
		return we;
	}

//KES SMI 031008	
//	if(mouseInfo->LButtonDown())
	if( we & WE_LBTNCLICK )
	{
		if(PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()))
		{
			m_fChecked ^= TRUE;
			(*cbWindowFunc)(m_ID, m_pParent, (m_fChecked?WE_CHECKED:WE_NOTCHECKED));	//PPP
		}
	}
	return we;
}
void cCheckBox::Render()
{
	VECTOR2 start_pos;
	start_pos.x = m_absPos.x;
	start_pos.y = m_absPos.y;

	m_CheckBoxImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	if(m_fChecked)
		m_CheckImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));


	RECT rect={(LONG)m_absPos.x+15, (LONG)m_absPos.y+6, 1,1};
	//rect.right = rect.left + 1;
	//rect.bottom = rect.top + 1;	
	CFONT_OBJ->RenderFont(m_wFontIdx, m_szCheckBoxText,lstrlen(m_szCheckBoxText),&rect,RGBA_MERGE(m_dwCheckBoxTextColor, m_alpha * m_dwOptionAlpha / 100));

	cWindow::Render();
}