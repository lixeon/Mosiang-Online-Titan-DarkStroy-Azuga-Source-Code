// GTStandingDialog.cpp: implementation of the CGTStandingDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTStandingDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
WORD g32th[GUILDINFOCOUNT] = { 14, 16, 18, 20, 22, 24, 26, 28, };
WORD g16th[4][2][2] = 
{
	{{13,6},{15,6}},{{17,8},{19,8}},{{21,10},{23,10}},{{25,12},{27,12}},
};
WORD g8th[2][2][2] = 
{
	{{5,2},{7,2}},{{9,4},{11,4}},
};
WORD g4th[2][2] = 
{
	{1,0},{3,0},
};
WORD gSemiFinalLine[4] = { 6, 8, 10, 12, };

DWORD gRanking[eGTT_MAX] = 
{
	32,			// 32	16	eGTFight_32
	16,			// 16	8	eGTFight_16
	8,			// 8	4	eGTFight_8
	4,			// 4	2	eGTFight_4
	2,			// 3,4	2	eGTFignt_3_4
	1,			// 1,2	2	eGTFight_1_2
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTStandingDialog::CGTStandingDialog()
{
	m_type = WT_GTSTANDING_DLG;
	memset( m_GuildInfo, 0, sizeof(REGISTEDGUILDINFO)*MAXGUILD_INTOURNAMENT );
	m_dwCurPage = 0;
	for(int i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMarkImage[i] = NULL;
	m_CurTournament = eGTFight_8;
	m_pNonGuildMark = NULL;
}

CGTStandingDialog::~CGTStandingDialog()
{

}


void CGTStandingDialog::Linking()
{
	m_pMainBase[0] = (cStatic*)GetWindowForID( GDT_MAIN1 );
	m_pMainBase[1] = (cStatic*)GetWindowForID( GDT_MAIN2 );

	for(int i=0; i<MAXGROUP; ++i)
	{
		m_pGroupBtn[i] = (cPushupButton*)GetWindowForID( GDT_SEMIFINAL_PUSHUPBTN+i );
		m_pGroupBtn[i]->SetPush( FALSE );
	}
	for(i=0; i<STATICCOUNT; ++i)
		m_pLineStatic[i] = (cStatic*)GetWindowForID( GDT_LINE1+i );
	for(i=0; i<GUILDINFOCOUNT_SF+1; ++i)
		m_pSFGuildMark[i] = (cStatic*)GetWindowForID( GDT_MAINMARK0 + i );
	for(i=0; i<GUILDINFOCOUNT_SF; ++i)
		m_pSFGuildName[i] = (cStatic*)GetWindowForID( GDT_MAINMENBER1 + i );
	for(i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMark[i] = (cStatic*)GetWindowForID( GDT_SECONDMARK0 + i );
	for(i=0; i<GUILDINFOCOUNT; ++i)
		m_pGuildName[i] = (cStatic*)GetWindowForID( GDT_SECONDMEMBER1 + i );

	m_pNonGuildMark = GUILDMARKMGR->LoadGuildMarkImg( NOMARKGUILDIDX );	
}


void CGTStandingDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val )
	{
		m_pGroupBtn[0]->SetPush( TRUE );
		m_dwCurPage = 0;
		RefreshGuildInfo();
	}
	else
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
}



void CGTStandingDialog::Render()
{
	cDialog::Render();

	if( !m_bActive )		return;


	int Count = GUILDINFOCOUNT+1;
	if( m_dwCurPage == 0 )		Count = GUILDINFOCOUNT_SF+1;

	for(int i=0; i<Count; ++i)
	{
		cStatic* pStatic = m_pGuildMark[i];
		if( m_dwCurPage == 0 )		pStatic = m_pSFGuildMark[i];
		
		if( m_pGuildMarkImage[i] )
		{
			VECTOR2 pos;
			pos.x = pStatic->GetAbsX();
			pos.y = pStatic->GetAbsY();
			m_pGuildMarkImage[i]->Render( &pos );
		}
	}
}


void CGTStandingDialog::AddGuildInfo( REGISTEDGUILDINFO* pGuildInfo )
{
	if( !pGuildInfo )			return;
	
	if( pGuildInfo->MarkExist )	
		GUILDMARKMGR->UpdateGuildMarkImg( pGuildInfo->GuildIdx, pGuildInfo->MarkExist, pGuildInfo->ImageData );

	memcpy( &m_GuildInfo[pGuildInfo->Position], pGuildInfo, sizeof(REGISTEDGUILDINFO) );
}


void CGTStandingDialog::SetCurPage( DWORD Page )
{
	m_dwCurPage = Page;
	RefreshGuildInfo();
}


void CGTStandingDialog::ResetActive()
{
	m_pMainBase[0]->SetActive( m_dwCurPage==0?TRUE:FALSE );
	m_pMainBase[1]->SetActive( m_dwCurPage==0?FALSE:TRUE );

	for(int i=0; i<MAXGROUP; ++i)
		m_pGroupBtn[i]->SetPush( FALSE );
	for(i=0; i<STATICCOUNT; ++i)	
		m_pLineStatic[i]->SetActive( FALSE );
	for(i=0; i<GUILDINFOCOUNT_SF+1; ++i)
		m_pSFGuildMark[i]->SetActive( m_dwCurPage>0?FALSE:TRUE );
	for(i=0; i<GUILDINFOCOUNT_SF; ++i)
	{
		m_pSFGuildName[i]->SetActive( m_dwCurPage>0?FALSE:TRUE );
		m_pSFGuildName[i]->SetStaticText("");
	}
	for(i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMark[i]->SetActive( m_dwCurPage>0?TRUE:FALSE );
	for(i=0; i<GUILDINFOCOUNT; ++i)
	{
		m_pGuildName[i]->SetActive( m_dwCurPage>0?TRUE:FALSE );
		m_pGuildName[i]->SetStaticText("");
		m_pGuildName[i]->SetFGColor( RGBA_MAKE( 255, 255, 255, 255 ) );
	}
	for(i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMarkImage[i] = NULL;
//	for(i=0; i<MAXGUILD_INTOURNAMENT; ++i)
//		memset( &m_GuildInfo[i], 0, sizeof(REGISTEDGUILDINFO) );

	m_pGroupBtn[m_dwCurPage]->SetPush( TRUE );
}

void CGTStandingDialog::ResetAll()
{
	m_pMainBase[0]->SetActive( m_dwCurPage==0?TRUE:FALSE );
	m_pMainBase[1]->SetActive( m_dwCurPage==0?FALSE:TRUE );

	for(int i=0; i<MAXGROUP; ++i)
		m_pGroupBtn[i]->SetPush( FALSE );
	for(i=0; i<STATICCOUNT; ++i)	
		m_pLineStatic[i]->SetActive( FALSE );
	for(i=0; i<GUILDINFOCOUNT_SF+1; ++i)
		m_pSFGuildMark[i]->SetActive( m_dwCurPage>0?FALSE:TRUE );
	for(i=0; i<GUILDINFOCOUNT_SF; ++i)
	{
		m_pSFGuildName[i]->SetActive( m_dwCurPage>0?FALSE:TRUE );
		m_pSFGuildName[i]->SetStaticText("");
	}
	for(i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMark[i]->SetActive( m_dwCurPage>0?TRUE:FALSE );
	for(i=0; i<GUILDINFOCOUNT; ++i)
	{
		m_pGuildName[i]->SetActive( m_dwCurPage>0?TRUE:FALSE );
		m_pGuildName[i]->SetStaticText("");
		m_pGuildName[i]->SetFGColor( RGBA_MAKE( 255, 255, 255, 255 ) );
	}
	for(i=0; i<GUILDINFOCOUNT+1; ++i)
		m_pGuildMarkImage[i] = NULL;
	for(i=0; i<MAXGUILD_INTOURNAMENT; ++i)
		memset( &m_GuildInfo[i], 0, sizeof(REGISTEDGUILDINFO) );

	m_pGroupBtn[m_dwCurPage]->SetPush( TRUE );
}



void CGTStandingDialog::RefreshGuildInfo()
{
	ResetActive();

	for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
	{
		if( m_GuildInfo[i].GuildIdx == 0 )			continue;

		if( m_dwCurPage == eGTPAGEVIEW_SEMIFANAL && m_CurTournament < eGTFight_4 )			continue;		
		else if( m_dwCurPage == eGTPAGEVIEW_SEMIFANAL && m_GuildInfo[i].Ranking > 4 )		continue;		
		if( m_dwCurPage > eGTPAGEVIEW_SEMIFANAL && m_dwCurPage != (DWORD)(m_GuildInfo[i].Position/GUILDINFOCOUNT+1) )	continue;

		BOOL bColor = TRUE;
		// 본선
		if( m_dwCurPage == eGTPAGEVIEW_SEMIFANAL )
		{
			if( m_CurTournament >= eGTFight_1_2 && m_GuildInfo[i].Ranking == eGTStanding_1ST )
				DrawRankingLine1( m_GuildInfo[i] );
			else if( m_CurTournament <= eGTFight_1_2 && m_CurTournament >= eGTFight_4 && m_GuildInfo[i].Ranking == 2 )
				DrawRankingLine2( m_GuildInfo[i] );

			AddGuildMarkName( m_GuildInfo[i], bColor  );
		}
		// 예선
		else
		{
			if( m_CurTournament == eGTFight_32 && m_GuildInfo[i].Ranking == eGTStanding_16TH )
				DrawRankingLine16( m_GuildInfo[i] );
			else if( m_CurTournament == eGTFight_16 && m_GuildInfo[i].Ranking <= eGTStanding_16TH )
				DrawRankingLine16( m_GuildInfo[i] );
			else if( m_CurTournament == eGTFight_8 && m_GuildInfo[i].Ranking <= eGTStanding_8TH )
				DrawRankingLine8( m_GuildInfo[i] );
			else if( m_CurTournament >= eGTFight_4 && m_GuildInfo[i].Ranking <= eGTStanding_4TH )
				DrawRankingLine4( m_GuildInfo[i] );
			else
				bColor = FALSE;

			AddGuildMarkName( m_GuildInfo[i], bColor );
		}
	}
}


void CGTStandingDialog::AddGuildMarkName( REGISTEDGUILDINFO GuildInfo, BOOL bWin )
{
	DWORD Pos = GuildInfo.Position%GUILDINFOCOUNT + 1;

	cStatic* pStaticMark = m_pGuildMark[Pos];
	cStatic* pStaticName = m_pGuildName[Pos-1];

	if( m_dwCurPage == 0 )
	{
		Pos = GuildInfo.Position/GUILDINFOCOUNT + 1;
		pStaticMark = m_pSFGuildMark[Pos];
		pStaticName = m_pSFGuildName[Pos-1];
	}

	if( m_CurTournament == eGTFight_32 )
	{
		if( GuildInfo.ProcessTournament == eGTFight_16 )				// 32강에서 부전승으로 16강으로 갔음.
			pStaticName->SetFGColor( RGBA_MAKE( 255, 255, 0, 255 ) );
		else
			pStaticName->SetFGColor( RGBA_MAKE( 255, 255, 255, 255 ) );
	}
	else
	{
		if( bWin )
			pStaticName->SetFGColor( RGBA_MAKE( 255, 255, 0, 255 ) );
		else
			pStaticName->SetFGColor( RGBA_MAKE( 180, 180, 180, 255 ) );
	}
	
	CGuildMark* pMark = GUILDMARKMGR->GetGuildMark( GuildInfo.MarkExist );
	if( pMark )
		m_pGuildMarkImage[Pos] = pMark;
	else
	{
		if( m_pNonGuildMark )
			m_pGuildMarkImage[Pos] = m_pNonGuildMark;
	}
	
	if( m_dwCurPage == eGTPAGEVIEW_SEMIFANAL && GuildInfo.Ranking == eGTStanding_1ST )
		m_pGuildMarkImage[0] = m_pGuildMarkImage[Pos];
	else if( m_dwCurPage != eGTPAGEVIEW_SEMIFANAL && GuildInfo.Ranking <= eGTStanding_4TH && m_CurTournament >= eGTFight_4 )
		m_pGuildMarkImage[0] = m_pGuildMarkImage[Pos];

	if( strlen( GuildInfo.GuildName )  )
		pStaticName->SetStaticText( GuildInfo.GuildName );
}



void CGTStandingDialog::DrawRankingLine16( REGISTEDGUILDINFO GuildInfo )
{
	m_pLineStatic[g32th[GuildInfo.Position%GUILDINFOCOUNT]]->SetActive(TRUE);
	
	DWORD pos = (GuildInfo.Position%GUILDINFOCOUNT)/2;			// 4자리중 어느위치인가.
	DWORD pos2 = (GuildInfo.Position%GUILDINFOCOUNT)%2;			// 위, 아래중 어디인가
	m_pLineStatic[g16th[pos][pos2][0]]->SetActive(TRUE);
	m_pLineStatic[g16th[pos][pos2][1]]->SetActive(TRUE);
}


void CGTStandingDialog::DrawRankingLine8( REGISTEDGUILDINFO GuildInfo )
{
	DrawRankingLine16( GuildInfo );

	DWORD pos = (GuildInfo.Position%GUILDINFOCOUNT)/4;			// 2자리중 어느위치인가.
	DWORD pos2 = GuildInfo.Position%GUILDINFOCOUNT;
	if( pos2 >= 4)		pos2 = (pos2-4)/2;
	else				pos2 = pos2/2;							// 0,1-(0,0) 2,3-(0,1) 4,5-(1,0) 6,7-(1,1)
	m_pLineStatic[g8th[pos][pos2][0]]->SetActive(TRUE);
	m_pLineStatic[g8th[pos][pos2][1]]->SetActive(TRUE);
}


void CGTStandingDialog::DrawRankingLine4( REGISTEDGUILDINFO GuildInfo )
{
	DrawRankingLine8( GuildInfo );

	DWORD pos = (GuildInfo.Position%GUILDINFOCOUNT)/4;			// 위, 아래중 어디인가
	m_pLineStatic[g4th[pos][0]]->SetActive(TRUE);
	m_pLineStatic[g4th[pos][1]]->SetActive(TRUE);
}


void CGTStandingDialog::DrawRankingLine2( REGISTEDGUILDINFO GuildInfo )
{
	m_pLineStatic[gSemiFinalLine[GuildInfo.Position/GUILDINFOCOUNT]]->SetActive( TRUE );
	
	DWORD pos = (GuildInfo.Position/GUILDINFOCOUNT)/2;
	DWORD pos2 = (GuildInfo.Position/GUILDINFOCOUNT)%2;
	
	m_pLineStatic[g8th[pos][pos2][0]]->SetActive(TRUE);
	m_pLineStatic[g8th[pos][pos2][1]]->SetActive(TRUE);
}


void CGTStandingDialog::DrawRankingLine1( REGISTEDGUILDINFO GuildInfo )
{
	DrawRankingLine2( GuildInfo );

	DWORD pos = (GuildInfo.Position/GUILDINFOCOUNT)/2;
	m_pLineStatic[g4th[pos][0]]->SetActive(TRUE);
	m_pLineStatic[g4th[pos][1]]->SetActive(TRUE);
}
