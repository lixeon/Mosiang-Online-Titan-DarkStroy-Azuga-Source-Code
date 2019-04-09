#include "stdafx.h"
#include "cComboBoxEx.h"
#include "./interface/cWindowManager.h"
#include "./interface/cPushupButton.h"
#include "./Input/Mouse.h"


cComboBoxEx::cComboBoxEx()
{
	m_type = WT_COMBOBOXEX;
}

cComboBoxEx::~cComboBoxEx()
{
}

void cComboBoxEx::ListMouseCheck( CMouse * mouseInfo )
{
	LONG x = mouseInfo->GetMouseX();
	LONG y = mouseInfo->GetMouseY();

	BOOL LeftDown = mouseInfo->LButtonDown();
	
	if( !WINDOWMGR->IsMouseOverUsed() )
	{
		m_nOverIdx = PtIdxInComboList(x, y);
		
		if( m_nOverIdx > GetItemCount() )
			m_nOverIdx = -1;
		else
			WINDOWMGR->SetMouseOverUsed();	//KES Ãß°¡
	}
	else
	{
		m_nOverIdx = -1;
	}


	if( LeftDown )
	{
		m_nCurSelectedIdx = PtIdxInComboList(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY());
		
		if(m_nCurSelectedIdx > GetItemCount() || m_nCurSelectedIdx == -1 )
		{
			m_pComboBtn->SetPush(FALSE);
			m_nCurSelectedIdx = -1;
		}
		else if( !WINDOWMGR->IsMouseDownUsed() )
		{
			m_pComboBtn->SetPush(FALSE);
			m_nOverIdx = -1;
			
			PTRLISTPOS p = m_ListItem.FindIndex(m_nCurSelectedIdx);
			OPTIONITEM * item = (OPTIONITEM*)m_ListItem.GetAt(p);
			if( !item ) return;
			
			strcpy(m_szComboText, item->string);
			
			SetOptionValue(item->wComboType);
			
			// ¸®½ºÆ® ¼±ÅÃ ÄÝ¹é
			cbWindowFunc(m_ID, m_pParent, WE_COMBOBOXSELECT);	//PPP

			WINDOWMGR->SetMouseDownUsed();
		}	
	}
}