// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameIn.h"
#include "NumberPadDialog.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"


CNumberPadDialog::CNumberPadDialog()
{	
	m_pStaticPN = NULL;	
	m_pLogInWindow = NULL;
	m_pCombo = NULL;	
}

CNumberPadDialog::~CNumberPadDialog()
{
}

void CNumberPadDialog::Linking()
{	
	m_pLogInWindow = WINDOWMGR->GetWindowForID( MT_LOGINDLG );	
	m_pStaticPN = (cStatic *)((cDialog*)m_pLogInWindow)->GetWindowForID(MT_PNSTATIC);
	m_pStaticPN->SetMultiLine(FALSE);
	m_pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);

	InitProtectionStr();
}

void CNumberPadDialog::SetActive( BOOL val )
{	
	cDialog::SetActive( val );
}

BOOL CNumberPadDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{			
	case WE_CLOSEWINDOW:
		{	
			return TRUE;
		}
		break;
	}
	
	//버디버디면 클릭 해도 반응 없게한다.
	int nGate = m_pCombo->GetCurSelectedIdx();
	if( nGate == 3)
	{
		return TRUE;
	}

	switch( lId )
	{		
	case NUMBERPAD_BACKSPACE:	//삭제
		{			
			InitProtectionStr();
		}
		break;
	case NUMBERPAD_0:	//0
		{
			InsertStr("0");
		}
		break;
	case NUMBERPAD_1:	//1
		{			
			InsertStr("1");
		}
		break;
	case NUMBERPAD_2:	//2
		{
			InsertStr("2");
		}
		break;
	case NUMBERPAD_3:	//3
		{
			InsertStr("3");
		}
		break;
	case NUMBERPAD_4:	//4
		{
			InsertStr("4");
		}
		break;
	case NUMBERPAD_5:	//5
		{
			InsertStr("5");
		}
		break;
	case NUMBERPAD_6:	//6
		{
			InsertStr("6");
		}
		break;
	case NUMBERPAD_7:	//7
		{
			InsertStr("7");
		}
		break;
	case NUMBERPAD_8:	//8
		{
			InsertStr("8");
		}
		break;
	case NUMBERPAD_9:	//9
		{
			InsertStr("9");
		}
		break;
	}	

	return TRUE;
}

void CNumberPadDialog::InsertStr(char* pStr)
{	
	char str[32] = {0,};
	char* numberStr = m_pStaticPN->GetStaticText();
	int nLen = strlen(numberStr);	

	if(nLen < 4)	//보안번호는 4자이상 입력 못하게 막는다.
	{
		//보안번호는 따로 저장하고 화면에 보일때는 *로 보이게 하기 위해 Static 클래스에 "*"로 셋팅한다.
		strcpy(str, numberStr);
		strcat(str, "*");
		m_pStaticPN->SetStaticText(str);		
		
		strcat(m_pProtectionStr, pStr);		
	}
}

void CNumberPadDialog::InitProtectionStr()
{
	m_pStaticPN->SetStaticText("");
	memset(m_pProtectionStr, 0, sizeof(ePROTECTIONSTR_MAXNUM));
}

char* CNumberPadDialog::GetProtectionStr()
{
	return m_pProtectionStr;
}
