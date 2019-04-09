// cSpin.cpp: implementation of the cSpin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cSpin.h"
#include "cButton.h"
#include "../cIMEex.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cSpin::cSpin()
{
	m_upBtn=NULL;
	m_downBtn=NULL;
	m_Unit=10;
	m_minValue=0;
	m_type = WT_SPIN;
	m_maxValue=100;
}

cSpin::~cSpin()
{
	SAFE_DELETE(m_upBtn);
	SAFE_DELETE(m_downBtn);
}
void cSpin::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cbFUNC Func, LONG ID)
{
	cEditBox::Init(x,y,wid,hei,basicImage,basicImage,ID);

	m_bCaret = FALSE;

	if(Func)
		cbWindowFunc = Func;
}

void cSpin::Add(cWindow * wnd)
{
	if(wnd->GetType() == WT_BUTTON)
	{
		if(!m_upBtn)
		{
			m_upBtn = (cButton *)wnd;
			m_upBtn->SetAbsXY((LONG)(m_absPos.x+m_upBtn->m_relPos.x), (LONG)(m_absPos.y+m_upBtn->m_relPos.y));
			m_upBtn->SetcbFunc(SpinUpBtnProc);
			m_upBtn->SetParent(this);
		}
		else if(!m_downBtn)
		{
			m_downBtn = (cButton *)wnd;
			m_downBtn->SetAbsXY((LONG)(m_absPos.x+m_downBtn->m_relPos.x), (LONG)(m_absPos.y+m_downBtn->m_relPos.y));
			m_downBtn->SetcbFunc(SpinDownBtnProc);
			m_downBtn->SetParent(this);
		}
	}
}
void cSpin::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);
	if(m_upBtn)m_upBtn->SetAbsXY((LONG)(m_absPos.x+m_upBtn->m_relPos.x), (LONG)(m_absPos.y+m_upBtn->m_relPos.y));
	if(m_downBtn)m_downBtn->SetAbsXY((LONG)(m_absPos.x+m_downBtn->m_relPos.x), (LONG)(m_absPos.y+m_downBtn->m_relPos.y));
}

DWORD cSpin::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cEditBox::ActionEvent(mouseInfo);

	if(m_upBtn->ActionEvent(mouseInfo) & WE_BTNCLICK)
		(*cbWindowFunc)(m_ID, m_pParent, WE_SPINBTNUP);
	if(m_downBtn->ActionEvent(mouseInfo) & WE_BTNCLICK)
		(*cbWindowFunc)(m_ID, m_pParent, WE_SPINBTNDOWN);
	
	return we;
}

void cSpin::Render()
{
	m_upBtn->Render();
	m_downBtn->Render();
	cEditBox::Render();
}

void cSpin::InitSpin(WORD spinStrSize, WORD strSize)
{
	cEditBox::InitEditbox(spinStrSize, strSize);
	SetValue(0);
}

void SpinUpBtnProc(LONG lId, void * p, DWORD we)
{
	cSpin * spin = (cSpin *)p;
	spin->IncUnit();
}
void SpinDownBtnProc(LONG lId, void * p, DWORD we)
{
	cSpin * spin = (cSpin *)p;
	spin->DecUnit();
}

LONG cSpin::GetValue()
{ 
//	m_pIMEBox->SetTopLine();
//	return (LONG)atoi(m_pIMEBox->GetNextLine()->line); 

	char strBuf[32];
	m_pIMEex->GetLineText( 0, strBuf );
	SPINUNIT dwValue = (SPINUNIT)_atoi64( RemoveComma( strBuf ) );
	
	if( dwValue < m_minValue )
	{
		dwValue = m_minValue;
	}
	else if( dwValue > m_maxValue )
	{
		dwValue = m_maxValue;
	}

	return (LONG)dwValue;
}


void cSpin::SetValue(SPINUNIT value)
{
//	m_pIMEBox->SetTopLine();

//	if(value < m_minValue)
//		itoa(m_minValue, m_pIMEBox->GetNextLine()->line, 10);
//	else if(value > m_maxValue)
//		itoa(m_maxValue, m_pIMEBox->GetNextLine()->line, 10);
//	else
//		itoa(value, m_pIMEBox->GetNextLine()->line, 10);

	char strBuf[32];
	m_pIMEex->GetScriptText( strBuf );

	if(value < m_minValue)
		_i64toa(m_minValue, strBuf, 10);
	else if(value > m_maxValue)
		_i64toa(m_maxValue, strBuf, 10);
	else
		_i64toa(value, strBuf, 10);

	AddComma( strBuf );
	m_pIMEex->SetScriptText( strBuf );
	
}


void cSpin::IncUnit()
{
//	m_pIMEBox->SetTopLine();
//	LONG value = atoi(m_pIMEBox->GetNextLine()->line);
//	if(value + m_Unit <= m_maxValue)
//		value += m_Unit;
//	itoa(value, m_pIMEBox->GetNextLine()->line, 10);

	char strBuf[32];
	m_pIMEex->GetScriptText( strBuf );

	SPINUNIT value = (unsigned long)_atoi64( RemoveComma( strBuf ) );

	if( value + m_Unit < value ) // 데이터형의 범위때문에 넣음..
	{
		value = m_maxValue;
	}
	else
	{
		if(value + m_Unit > m_maxValue)
			value = m_maxValue;
		else
			value += m_Unit;
	}
	
	_i64toa(value, strBuf, 10);
	AddComma( strBuf );
	m_pIMEex->SetScriptText( strBuf );
}

void cSpin::DecUnit()
{
//	m_pIMEBox->SetTopLine();
//	LONG value = atoi(m_pIMEBox->GetNextLine()->line);
//	value -= m_Unit;
//	if(value<=m_minValue)value=m_minValue;
//	itoa(value, m_pIMEBox->GetNextLine()->line, 10);

	char strBuf[32];
	m_pIMEex->GetScriptText( strBuf );

	SPINUNIT value = (unsigned long)_atoi64( RemoveComma( strBuf ) );

	if( value - m_Unit > value ) // 데이터형의 범위 때문에 넣음..
	{
		value = m_minValue;
	}
	else
	{
		if( value - m_Unit < m_minValue )
			value = m_minValue;
		else
			value -= m_Unit;
	}

	_i64toa(value, strBuf, 10);
	
	AddComma( strBuf );
	m_pIMEex->SetScriptText( strBuf );
}

