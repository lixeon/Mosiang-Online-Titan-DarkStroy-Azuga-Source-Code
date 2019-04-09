// MiniFriendDialog.cpp: implementation of the CMiniFriendDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniFriendDialog.h"
#include "WindowIDEnum.h"
#include "cImeEx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniFriendDialog::CMiniFriendDialog()
{
	m_type = WT_MINIFRIENDDLG;
}

CMiniFriendDialog::~CMiniFriendDialog()
{

}

void CMiniFriendDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_MINIFRIENDDLG;
}

void CMiniFriendDialog::Linking()
{
	m_pName = (cStatic*)GetWindowForID(FRI_NAME);
	m_pNameEdit = (cEditBox*)GetWindowForID(FRI_NAMEEDIT);
	m_pNameEdit->SetValidCheck(VCM_CHARNAME);
	m_pNameEdit->SetEditText("");
	m_pAddOkBtn = (cButton*)GetWindowForID(FRI_ADDOKBTN);
	m_pAddCancelBtn = (cButton*)GetWindowForID(FRI_ADDCANCELBTN);
}

void CMiniFriendDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	if(val)
		m_pNameEdit->SetEditText("");
	cDialog::SetActiveRecursive(val);
}

void CMiniFriendDialog::SetName(char* Name)
{
	m_pNameEdit->SetEditText(Name);
}