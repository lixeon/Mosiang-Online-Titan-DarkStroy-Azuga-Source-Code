// BigMapDlg.cpp: implementation of the CBigMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BigMapDlg.h"
#include "MHFile.h"
#include "GameIn.h"
#include "Npc.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "./Input/Mouse.h"
#include "./MiniMapDlg.h"
#include "MHMap.h"

#ifdef _TESTCLIENT_
#include "GameResourceManager.h"
#endif
/*
	eBMICON_SELF,13
	eBMICON_PARTYMEMBER,12
	eBMICON_LOGIN,25
	eBMICON_MAPCHANGE,26
	eBMICON_WEAPON,16
	eBMICON_DRESS,14
	eBMICON_DOCTOR,17
	eBMICON_BOOK,19
	eBMICON_WAREHOUSE,18
	eBMICON_PYOGUK,23
	eBMICON_MUNPA,20
	eBMICON_YEONMU,22
	eBMICON_ACCESSARY,15
	eBMICON_GWANJOL,21
	eBMICON_CASTLEMANAGER,33
	eBMICON_ETC,24

	eBMICON_QUEST_EXCLAMATION1,27
	eBMICON_QUEST_EXCLAMATION2,28
	eBMICON_QUEST_EXCLAMATION3,29
*/
DWORD BIGMAPICON_DESC[] = 
{
	13,	15,	19,			//eBMICON_SELF,
	12,	15,	15,			//eBMICON_PARTYMEMBER,
	25,	20,	20,			//eBMICON_LOGIN,
	26,	20,	20,			//eBMICON_MAPCHANGE,
	16,	20,	20,			//eBMICON_WEAPON,
	14,	20,	20,			//eBMICON_DRESS,
	17,	20,	20,			//eBMICON_DOCTOR,
	19,	20,	20,			//eBMICON_BOOK,
	18,	20,	20,			//eBMICON_WAREHOUSE,
	23,	20,	20,			//eBMICON_PYOGUK,
	20,	20,	20,			//eBMICON_MUNPA,
	22,	20,	20,			//eBMICON_YEONMU,
	15,	20,	20,			//eBMICON_ACCESSARY,
	21,	20,	20,			//eBMICON_GWANJOL,
	33, 20, 20,			//eBMICON_CASTLEMANAGER,
	24,	20,	20,			//eBMICON_ETC,
	27,	26,	26,			//eBMICON_QUEST_EXCLAMATION1,
	28,	26,	26,			//eBMICON_QUEST_EXCLAMATION2,
	29,	26,	26,			//eBMICON_QUEST_EXCLAMATION3,
	34, 21, 48,			//eBMICON_CAMERA,
};


//#define BIGMAP_WIDTH	(512+256)
//#define BIGMAP_HEIGHT	(512+256)

#define BIGMAP_WIDTH	(512)
#define BIGMAP_HEIGHT	(512)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBigMapDlg::CBigMapDlg()
{
	m_IconTable.Initialize(64);
	m_IconPool.Init(100, 10, "MinimapIconPool");

	m_dwImageRGB = RGBA_MAKE(255,255,255,120);
}

CBigMapDlg::~CBigMapDlg()
{
	m_MapImage.Release();
	
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
		ReleaseMinniMapIcon(pIcon);
	m_IconTable.RemoveAll();
}

void CBigMapDlg::Linking()
{
	memset(m_pIconImage,0,sizeof(cImage*)*eBMICON_MAX);
	InitIconImage();
}

void CBigMapDlg::LoadMinimapImageInfo( MAPTYPE MapNum )
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);

	char descFile[64];
	char MiniMapImageFilename[256];

#ifdef _TESTCLIENT_
	MapNum = GAMERESRCMNGR->m_TestClientInfo.Map;
#endif

	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(descFile,"Minimap%d.bin",MapNum);	
	file.Init(descFile,"rb",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#else
	sprintf(descFile,"Minimap%d.txt",MapNum);	
	file.Init(descFile,"r",MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT);
#endif

	if(file.IsInited() == FALSE)
	{
		DIRECTORYMGR->SetLoadMode(eLM_Root);
		return;
	}
	
	for( int i = 0 ; i <eMINIMAPMODE_MAX ; ++i )
	{
		file.GetString( MiniMapImageFilename );
		if( file.IsEOF() )
			break;
		
		file.GetDword();
		file.GetDword();

		if( i == eMINIMAPMODE_SMALL )
		{
			m_MapImage.LoadSprite( MiniMapImageFilename );
			cImageRect rect = { 0, 0, BIGMAP_WIDTH, BIGMAP_HEIGHT };
			m_MapImage.SetImageSrcRect(&rect);
		}
	}
	
	file.Release();	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

void CBigMapDlg::InitBigMap( MAPTYPE MapNum )
{
	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter ||
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
	{
		SetActive(FALSE);
		//ÕâÀïÊÇ´ò²¹¶¡,ÓÃÀ´´ò¿ªÐÂµØÍ¼µÄÓÒÉÏ½ÇÐ¡Í¼
		if (MapNum == 23 || MapNum == 24 || MapNum == 26 || MapNum == 29 )
		{
			SetActive(TRUE);
		}
	}

	// magi82(37) ¸Ê ¼Ó¼º µ¥ÀÌÅÍ
	BOOL rt = MAP->IsMapKind(eMapView, MapNum);
	if( FALSE == rt )
		SetActive(FALSE);
	/*
	if( MapNum == BOSSMONSTER_MAP || MapNum == BOSSMONSTER_2NDMAP || MapNum == RUNNINGMAP
		|| MapNum == PKEVENTMAP ) // boss map
		SetActive(FALSE);
	*/

	//SW061019 ¼­¹ÙÀÌ¹ú¸ðµå °ü·Ã
	//if(MAP->CheckMapKindIs(eSurvival))
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		SetActive(FALSE);
		//ÕâÀïÊÇ´ò²¹¶¡,ÓÃÀ´´ò¿ªÐÂµØÍ¼µÄÓÒÉÏ½ÇÐ¡Í¼
		if (MapNum == 23 || MapNum == 24 || MapNum == 26 || MapNum == 29 )
		{
			SetActive(TRUE);
		}
	}

	m_MapNum = MapNum;

	LoadMinimapImageInfo( MapNum );
}

void CBigMapDlg::AddStaticNpcIcon(CObject* pObject)
{
	CNpc* pNpc = (CNpc*)pObject;
	WORD kind = pNpc->GetNpcKind();

	switch( kind )
	{
	case 1:		AddIcon(eBMICON_WEAPON,pObject);	break;
	case 2:		AddIcon(eBMICON_DRESS,pObject);		break;
	case 3:		AddIcon(eBMICON_ACCESSARY,pObject);	break;
	case 4:		AddIcon(eBMICON_DOCTOR,pObject);	break;
	case 5:		AddIcon(eBMICON_BOOK,pObject);		break;
	case 8:		AddIcon(eBMICON_MUNPA,pObject);		break;
	case 10:
	case 84:
		AddIcon(eBMICON_WAREHOUSE,pObject);
		break;
	case 11:	AddIcon(eBMICON_CASTLEMANAGER, pObject);	break;
	case 12:	AddIcon(eBMICON_PYOGUK,pObject);	break;
	case 13:	AddIcon(eBMICON_GWANJOL,pObject);	break;
	case 15:	AddIcon(eBMICON_YEONMU,pObject);	break;
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 39:
	case 43:
	case 44:
	case 45:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 71:
	case 72:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
		AddIcon(eBMICON_ETC,pObject);
		break;		
	case 31:
	case 81:
	case 82:	
		AddIcon(eBMICON_MAPCHANGE,pObject);
		break;
	case 30:
	case 83:
		AddIcon(eBMICON_LOGIN,pObject);
		break;
	default:
		AddIcon(eBMICON_ETC,pObject);
		break;
	}

/*	if(pNpc->GetNpcKind() == 31)
		AddIcon(eBMICON_MAPCHANGE,pObject);

	else if(pNpc->GetNpcKind() == 30)
		AddIcon(eBMICON_LOGIN,pObject);

	else if(pNpc->GetNpcKind() == 1)
		AddIcon(eBMICON_WEAPON,pObject);

	else if(pNpc->GetNpcKind() == 2)
		AddIcon(eBMICON_DRESS,pObject);

	else if(pNpc->GetNpcKind() == 3)
		AddIcon(eBMICON_ACCESSARY,pObject);

	else if(pNpc->GetNpcKind() == 4)	
		AddIcon(eBMICON_DOCTOR,pObject);

	else if(pNpc->GetNpcKind() == 5)
		AddIcon(eBMICON_BOOK,pObject);

	else if(pNpc->GetNpcKind() == 8)
		AddIcon(eBMICON_MUNPA,pObject);

	else if(pNpc->GetNpcKind() == 10)
		AddIcon(eBMICON_WAREHOUSE,pObject);

	else if(pNpc->GetNpcKind() == 12)
		AddIcon(eBMICON_PYOGUK,pObject);	

	else if(pNpc->GetNpcKind() == 13)
		AddIcon(eBMICON_GWANJOL,pObject);	

	else if(pNpc->GetNpcKind() == 14)
		AddIcon(eBMICON_PYOGUK,pObject);

	else if(pNpc->GetNpcKind() == 15)
		AddIcon(eBMICON_YEONMU,pObject);

	else if(pNpc->GetNpcKind() == 22)
		AddIcon(eBMICON_ETC, pObject);

	else if(pNpc->GetNpcKind() == 24)
		AddIcon(eBMICON_ETC, pObject);
	
	else if(pNpc->GetNpcKind() == 25)
		AddIcon(eBMICON_ETC,pObject);

	else if(pNpc->GetNpcKind() == 11)
		AddIcon(eBMICON_CASTLEMANAGER, pObject);
		*/
}

void CBigMapDlg::AddPartyMemberIcon(PARTY_MEMBER* pMember)
{
	AddIcon(eBMICON_PARTYMEMBER,pMember);
}

void CBigMapDlg::AddHeroIcon(CObject* pObject)
{
	m_HeroIcon.Init(&m_pIconImage[eBMICON_SELF],pObject, eBMICON_SELF);
	m_HeroIcon.SetRGBColor( RGBA_MAKE(255,255,255,220) );
	m_HeroIcon.SetAlwaysTooltip( TRUE );

	m_HeroIcon.SetCameraImage( &m_pIconImage[eBMICON_CAMERA] );
}


CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,CObject* pObject)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind],pObject, Kind);
	pIcon->SetMarkImage(&m_pIconImage[eBMICON_QUEST_EXCLAMATION1],NULL);//&m_pIconImage[eBMICON_QUEST_QUESTION]);
	return pIcon;
}

CMiniMapIcon* CBigMapDlg::GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pMember, Kind);
	return pIcon;
}


void CBigMapDlg::ReleaseMinniMapIcon(CMiniMapIcon* pIcon)
{
	m_IconPool.Free(pIcon);
}


void CBigMapDlg::AddIcon(WORD Kind,CObject* pObject)
{
	ASSERT(m_IconTable.GetData(pObject->GetID()) == NULL);
	RemoveIcon(pObject->GetID());
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pObject);
	m_IconTable.Add(pIcon,pObject->GetID());
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}

void CBigMapDlg::AddIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	ASSERT(m_IconTable.GetData(pMember->dwMemberID) == NULL);
	RemoveIcon(pMember->dwMemberID);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pMember);
	m_IconTable.Add(pIcon,pMember->dwMemberID);
	pIcon->SetRGBColor( RGBA_MAKE(255,255,255,220) );
	pIcon->SetAlwaysTooltip( TRUE );
}

void CBigMapDlg::RemoveIcon(DWORD ObjectID)
{
	CMiniMapIcon* pIcon = (CMiniMapIcon*)m_IconTable.GetData(ObjectID);
	if(pIcon == NULL)
		return;

	ReleaseMinniMapIcon(pIcon);
	m_IconTable.Remove(ObjectID);
}


void CBigMapDlg::Render()
{
	if( !m_bActive ) return;
	if( m_MapImage.IsNull() ) return;

	cDialog::RenderWindow();

//	m_MapImage.SetImageSrcRect(&rect);

//	DWORD color = RGBA_MAKE(255,255,255,m_alpha);
	DWORD color = RGBA_MAKE(255,255,255,130);
//	DWORD color = m_dwImageRGB;

//	VECTOR2 trans;
//	trans.x = GetAbsX();
//	trans.y = GetAbsY();

	VECTOR2 vScale = { BIGMAP_WIDTH/512.f, BIGMAP_HEIGHT/512.f };

//	m_MapImage.RenderSprite( NULL, NULL, 0, &m_absPos, color );
	m_MapImage.RenderSprite( &vScale, NULL, 0, &m_absPos, color );

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Render( &m_absPos, NULL );
	}

	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->RenderQuestMark(&m_absPos,NULL);
	}

	m_HeroIcon.Render(&m_absPos);

//°¡ÀåÀ§¿¡ ÇÑ¹ø´õ
	if( m_pIconForToolTip )
		m_pIconForToolTip->Render( &m_absPos, NULL );

	cDialog::RenderComponent();
}

void CBigMapDlg::SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
	{
		pIcon->SetObjectPos(posX, posZ);
		pIcon->SetTargetPos(posX, posZ);
	}
}

void CBigMapDlg::SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
		pIcon->SetTargetPos(posX, posZ);
}

BOOL CBigMapDlg::CanActive()
{
	/*
	BOOL rt = TRUE;
	switch(m_MapNum)
	{
	case BOSSMONSTER_MAP:		// bossmap
	case BOSSMONSTER_2NDMAP:	// 2nd bossmap
	case Tournament:			// Åä³Ê¸ÕÆ®
	case QuestRoom:				// ±â¿¬ÀÇ¹æ
	case dongkyung:				// ÀÌº¥Æ®¸Ê
	case jangbeak:				// ÀÌº¥Æ®¸Ê
	case RUNNINGMAP:
	case SURVIVALMAP:
	case PKEVENTMAP:
	case DIMENSION0:
	case DIMENSION:
		rt = FALSE;
		break;
	default:
		rt = TRUE;
		break;
	}
	return rt;
	*/

	// magi82(37) ¸Ê ¼Ó¼º µ¥ÀÌÅÍ
	return MAP->IsMapKind(eMapView);
}

void CBigMapDlg::InitIconImage()
{
	cImage* pImg;
	DWORD w,h;
	for(int n=0;n<eBMICON_MAX;++n)
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(BIGMAPICON_DESC[3*n+0],pImg,PFT_MINIMAPPATH);
		w = BIGMAPICON_DESC[3*n+1];
		h = BIGMAPICON_DESC[3*n+2];
		m_pIconImage[n].InitIconImage(pImg,w,h);
	}

//KES
	SCRIPTMGR->GetImage( 63, &m_TooltipImage, PFT_HARDPATH );

	m_pIconForToolTip = NULL;
}

void CBigMapDlg::Process()
{
//	MINIMAPIMAGE* pMinimap = &m_MinimapImage[m_CurMode];
//	DWORD iWidth,iHeight;
//	if(m_CurMode == eMINIMAPMODE_FULL)
//	{
//		iWidth = m_ImageDisplayWH.x;
//		iHeight = m_ImageDisplayWH.y;
//	}
//	else
//	{
//		iWidth = pMinimap->ImageWidth;
//		iHeight = pMinimap->ImageHeight;
//	}	

	m_HeroIcon.Update(BIGMAP_WIDTH,BIGMAP_HEIGHT);

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Update(BIGMAP_WIDTH,BIGMAP_HEIGHT);
	}

//	VECTOR2* pHeroPos = m_HeroIcon.GetScreenPos();

//	SetAbsXY( (1024-BIGMAP_WIDTH)/2 - pHeroPos->x, (768-BIGMAP_HEIGHT)/2 - pHeroPos->y + 70 );
//	SetAbsXY( 1024/2 - pHeroPos->x, 768/2 - pHeroPos->y - 70 );
//	m_absPos.x = (1024-768)/2;
//	m_absPos.y = 0;


//	PICONMGR->Process();	
}

DWORD CBigMapDlg::ActionEvent( CMouse* mouseInfo )
{
//	DWORD we = cDialog::ActionEvent( mouseInfo );
//	if( !m_bActive ) return we;

	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;

//	we |= cWindow::ActionEvent(mouseInfo);
//	DWORD we = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;

	if( !WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
	{
		//³»°¡ µå·¡±×ÁßÀÎ À©µµ¿ì¸é over°¡ ¾Æ´Ï´Ù. //´Ù¸¥À©µµ¿ì¿¡ ¿À¹ö·Î ¹«¾ð°¡ Ã³¸®ÇØ¾ß ÇÒ¶§°¡ ÀÖ´Ù.(µå·¡±×¿£µå¶ø)
		if( !(WINDOWMGR->IsDragWindow() && (cWindow*)WINDOWMGR->GetDragDlg() == this ) )
		{
//			we |= WE_MOUSEOVER;
			
			//ÅøÆÁÀÌ ÀÖ´ÂÁö ¸ÕÀú °Ë»ç...ÃÖÀûÈ­¶§
			
			m_dwViewCurTime = GetTickCount();
			if(m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
			{
				m_ttX = mouseInfo->GetMouseX();
				m_ttY = mouseInfo->GetMouseY();
			}
		}
/*		
		if( !WINDOWMGR->IsDragWindow() )
		{
			if( m_type & WT_ICON )
				CURSOR->SetCursor( eCURSOR_ITEMSELECT );
			else
				CURSOR->SetCursor( eCURSOR_DEFAULT );
		}
*/
	}
	else
	{
		m_dwViewLastTime = m_dwViewCurTime = 0;
	}

/*
	if( !WINDOWMGR->IsMouseDownUsed() )
	if( PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
	{
		if( mouseInfo->LButtonDown() )
			we |= WE_LBTNCLICK;
		if( mouseInfo->RButtonDown() )
			we |= WE_RBTNCLICK;
		if( mouseInfo->LButtonDoubleClick() )
			we |= WE_LBTNDBLCLICK;
		if( mouseInfo->RButtonDoubleClick() )
			we |= WE_RBTNDBLCLICK;
	}
*/
//	return we;


//
	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);
//	return we;



//	if( we & WE_MOUSEOVER )
	{
/*
		cImageRect rect;
		rect.left = (LONG)m_DiplayOriginPosOfImage.x;
		rect.top = (LONG)m_DiplayOriginPosOfImage.y;
		rect.right = (LONG)(m_DiplayOriginPosOfImage.x+m_ImageDisplayWH.x);
		rect.bottom = (LONG)(m_DiplayOriginPosOfImage.y+m_ImageDisplayWH.y);
*/		
		char temp[64] = {0,};
		int x = mouseInfo->GetMouseX();
		int y = mouseInfo->GetMouseY();

		VECTOR2 trans;
		trans.x = GetAbsX();
		trans.y = GetAbsY();
//		trans.x -= rect.left;
//		trans.y -= rect.top;
		
		CMiniMapIcon* pIcon;
		float dist;
		float olddist = 10000.0f;	//ÃæºÐÈ÷ Å«¼ö

		m_pIconForToolTip = NULL;

		m_IconTable.SetPositionHead();
		while(pIcon = m_IconTable.GetData())
		{
			int sort = pIcon->GetToolTip(x,y,&trans,temp, &dist );			

			if(sort == 1)				// Npc
			{
				if( dist < olddist )
				{
					m_pIconForToolTip = pIcon;
					olddist = dist;

					SetToolTip( temp, RGB_HALF( 255, 255, 0 ), &m_TooltipImage, RGB_HALF( 0, 0, 0 ) );
					SetWH(BIGMAP_WIDTH, BIGMAP_HEIGHT);
				}
			}
			else if(sort == 2)			// Party
			{
				if( dist < olddist )
				{
					m_pIconForToolTip = pIcon;
					olddist = dist;

					SetToolTip( temp, RGB_HALF(185, 241, 69), &m_TooltipImage, RGB_HALF( 0, 0, 0 ) );
					SetWH(BIGMAP_WIDTH, BIGMAP_HEIGHT);
				}
			}
		}

		if(temp[0] == 0)
		{
			SetToolTip("");
			SetWH(0, 0);
		}
	}

	return we;
}


void CBigMapDlg::ShowQuestMarkIcon( CObject* pTarget, int nKind )
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( pTarget->GetID() );
	if( pIcon == NULL ) return;

	pIcon->ShowQuestMark( nKind );
}


void CBigMapDlg::SetActive( BOOL val )
{
	if( CanActive() == FALSE )
		val = FALSE;
	
	cDialog::SetActive( val );

	if( GAMEIN->GetMiniMapDialog() )
		GAMEIN->GetMiniMapDialog()->RefreshMode();
}
