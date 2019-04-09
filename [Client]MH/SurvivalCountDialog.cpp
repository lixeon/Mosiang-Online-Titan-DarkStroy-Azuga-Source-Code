#include "stdafx.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cDialog.h"
#include "./interface/cStatic.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"

#include "survivalcountdialog.h"

CSurvivalCountDialog::CSurvivalCountDialog(void)
{
	//m_pCounterNum[FrontNum] = NULL;
	//m_pCounterNum[BackNum]	= NULL;
	m_pCounterNum =	NULL;
	m_pWinnerName= NULL;
}

CSurvivalCountDialog::~CSurvivalCountDialog(void)
{
}

void CSurvivalCountDialog::Linking()
{
	//m_pCounterNum[FrontNum]	= (cStatic*)GetWindowForID(SVV_FRONTNUMBER);
	//m_pCounterNum[BackNum]	= (cStatic*)GetWindowForID(SVV_BACKNUMBER);
	m_pCounterNum = (cStatic*)GetWindowForID(SVV_ALIVECOUNTER);
	m_pWinnerName	= (cStatic*)GetWindowForID(SVV_WINNERNAME);

	//LoadNumImage();
	SetCounterNumber(0);
	//m_pCounterNum[FrontNum]->SetBasicImage(&m_NumImage[0]);
	//m_pCounterNum[BackNum]->SetBasicImage(&m_NumImage[0]);
	m_pWinnerName->SetStaticText(CHATMGR->GetChatMsg(484));
}

void CSurvivalCountDialog::InitSurvivalCountDlg( MAPTYPE MapNum )
{
	//if(MAP->CheckMapKindIs(eSurvival))
	if( MAP->IsMapKind(eSurvivalMap) )
		SetActive(TRUE);
	else
		SetActive(FALSE);
}

//void CSurvivalCountDialog::LoadNumImage()
//{
//	for( int n = 0; n < NUMBER_MAX; ++n )
//	{
//		SCRIPTMGR->GetImage(n, &m_NumImage[n], PFT_JACKPOTPATH);
//	}
//}

//void CSurvivalCountDialog::SetCounterNumber( DWORD num )
//{
//	if( num > 99 )
//	{
//		num = 99;
//	}
//
//	int cipher1, cipher2;
//
//	cipher1 = num%10;	//	1의 자리
//	cipher2 = num/10;	//	10의 자리
//
//	m_pCounterNum[FrontNum]->SetBasicImage(&m_NumImage[cipher2]);
//	m_pCounterNum[BackNum]->SetBasicImage(&m_NumImage[cipher1]);
//}

void CSurvivalCountDialog::SetCounterNumber( DWORD num )
{
	int c1, c2;
	c1 = num%10;
	c2 = num/10;
	
	char temp[128] = {0,};
	sprintf( temp, "%d%d", c2, c1 );

	if(m_pCounterNum)
		m_pCounterNum->SetStaticText(temp);
}

void CSurvivalCountDialog::SetWinnerName( char* pName )
{
	if(pName)
		m_pWinnerName->SetStaticText(pName);
	else
		m_pWinnerName->SetStaticText(CHATMGR->GetChatMsg(484));
}
