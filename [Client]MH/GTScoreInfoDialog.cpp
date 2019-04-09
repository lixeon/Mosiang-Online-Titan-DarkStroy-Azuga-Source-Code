// GTScoreInfoDailog.cpp: implementation of the CGTScoreInfoDailog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTScoreInfoDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTScoreInfoDialog::CGTScoreInfoDialog()
{
	m_type = WT_GTSCOREINFO_DLG;
	m_bStart = FALSE;
}

CGTScoreInfoDialog::~CGTScoreInfoDialog()
{

}


void CGTScoreInfoDialog::Linking()
{
	m_pGuildName[0] = (cStatic*)GetWindowForID( GDT_GUILDNAME1 );
	m_pGuildName[1] = (cStatic*)GetWindowForID( GDT_GUILDNAME2 );
	m_pGuildScore[0] = (cStatic*)GetWindowForID( GDT_GUILDMEMBER1 );
	m_pGuildScore[1] = (cStatic*)GetWindowForID( GDT_GUILDMEMBER2 );
	m_pTimeStatic = (cStatic*)GetWindowForID( GDT_GTIME );
	m_pOutBtn = (cButton*)GetWindowForID( GDT_OUTBTN );

	m_Score[0] = 0;
	m_Score[1] = 0;
	m_FightTime = 0;
	m_EntranceTime = 120000;

	for( int i=0; i<2; ++i)
	{
		m_ScoreImage[i].Init( 32, 0 );
		m_ScoreImage[i].SetFillZero( FALSE );
		m_ScoreImage[i].SetLimitCipher( 2 );
		m_ScoreImage[i].SetPosition( (int)m_pGuildScore[i]->GetAbsX(), (int)m_pGuildScore[i]->GetAbsY() );
		m_ScoreImage[i].SetScale( 1.0f, 1.0f );
		m_ScoreImage[i].Init( 32, 0 );
	}
	
	m_EntranceTimeImage.Init( 64, 32 );
	m_EntranceTimeImage.SetFillZero( FALSE );
	m_EntranceTimeImage.SetLimitCipher( 3 );
	m_EntranceTimeImage.SetPosition( 512+80, 80 );
	m_EntranceTimeImage.SetScale( 2.0f, 2.0f );
}


void CGTScoreInfoDialog::Render()
{
	cDialog::Render();
	
	if( m_bActive )
	{
		for(int i=0; i<2; ++i)
		{
			m_ScoreImage[i].SetNumber( m_Score[i] );
			m_ScoreImage[i].Render();
		}

		if( !m_bStart )
		{
			m_EntranceTimeImage.SetNumber( m_EntranceTime/1000 );
			m_EntranceTimeImage.Render();
		}
	}
}


void CGTScoreInfoDialog::Process()
{
	if( !m_bStart )
	{
		if( gTickTime < m_EntranceTime )	m_EntranceTime -= gTickTime;
		else								m_EntranceTime = 0;
		return;
	}

	if( gTickTime < m_FightTime )			m_FightTime -= gTickTime;
	else									m_FightTime = 0;

	char buf[32];
	sprintf(buf, "%02d:%02d", m_FightTime/60000, (m_FightTime%60000)/1000);
	m_pTimeStatic->SetStaticText(buf);
}


void CGTScoreInfoDialog::SetBattleInfo( SEND_BATTLESCORE_INFO* pInfo )
{
	for( int i=0; i<2; ++i)
	{
		m_pGuildName[i]->SetStaticText( pInfo->GuildName[i] );
		m_Score[i] = pInfo->Score[i];
	}

	m_EntranceTime = pInfo->EntranceTime;
	m_FightTime = pInfo->FightTime;
	char buf[32];
	sprintf(buf, "%02d:%02d", m_FightTime/60000, (m_FightTime%60000)/1000);
	m_pTimeStatic->SetStaticText(buf);
}



void CGTScoreInfoDialog::ShowOutBtn( BOOL bShow )
{
	m_pOutBtn->SetActive( bShow );		
}