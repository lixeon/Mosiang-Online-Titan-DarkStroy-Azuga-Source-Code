// MousePointer.cpp: implementation of the CMousePointer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MousePointer.h"
#include "WindowIDEnum.h"
#include "./interface/cAni.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMousePointer::CMousePointer()
{
	m_pAniBasic = NULL;
	m_pAniClick = NULL;
}

CMousePointer::~CMousePointer()
{

}

void CMousePointer::Linking()
{
//	m_pAniBasic = (cAni *)GetWindowForID(INT_MOUSEBASIC);
//	m_pAniClick = (cAni *)GetWindowForID(INT_MOUSECLICK);
}
void CMousePointer::MonsterAttack()
{
//	m_pAniBasic->SetActive(FALSE);
//	m_pAniClick->SetActive(TRUE);
//	m_pAniClick->Play();
}
void CMousePointer::MonsterMouseOver()
{
//	m_pAniBasic->SetActive(FALSE);
//	m_pAniClick->SetActive(TRUE);
}

void CMousePointer::MonsterLeave()
{
//	m_pAniBasic->SetActive(TRUE);
//	m_pAniClick->SetActive(FALSE);
}
