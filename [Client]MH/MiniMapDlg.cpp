// MiniMapDlg.cpp: implementation of the CMiniMapDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniMapDlg.h"
#include "BigMapDlg.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cScriptManager.h"
#include "Object.h"
#include "Npc.h"
#include "GameResourceManager.h"
#include "WindowIDEnum.h"
#include "./Input/UserInput.h"
#include "mhFile.h"

#include "GameIn.h"
#include "MainBarDialog.h"
#include "ChatManager.h"
#include "PartyIconManager.h"
#include "EventMapInfo.h"
#include "mhMap.h"

#include "cJackpotManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
Â»Ã§?ÃŒÃÃ®Â´Ã‚ 12*12Â·Ã ÂºÂ¯Â°Ã¦Ã‡ÃÂ¿Â´Â½?Â´ÃÂ´Ã™.

ÂºÂ»?Ã:	 514 355 526 367
Â¹Ã¦Ã†Ã„Â¿Ã¸:	 574 367 586 379
Â·ÃÂ±Ã—?Ã:	 550 355 562 367
Â¸ÃŠ?ÃŒÂµÂ¿:	 562 355 574 367
Â¹Â«Â±Ã¢Â»Ã³:	 574 355 586 367
Â¿ÃŠÂ»Ã³?Ã:	 586 355 598 367
?Ã‡Â¿Ã¸:	 610 355 622 367
Â¼Â­?Ã»Â»Ã³:	 586 367 598 379
ÃƒÂ¢Â°Ã­:	 598 367 610 379
Ã‡Â¥Â±Â¹?Ã:	 610 367 622 379
Â¹Â®Ã†Ã„:	 562 367 574 379
Â¿Â¬Â¹Â«?Ã¥:	 562 367 574 379

ÃˆÂ®Â´Ã«:	 526 355 538 367
ÃƒÃ Â¼Ã’:	 538 355 550 367
*/

DWORD MINIMAPICON_DESC[] = 
{
	0,	12,	17,			//eMNMICON_SELF,
	1,	12,	12,			//eMNMICON_PARTYMEMBER,
	2,	12,	12,			//eMNMICON_LOGIN,
	3,	12,	12,			//eMNMICON_MAPCHANGE,
	4,	12,	12,			//eMNMICON_WEAPON,
	5,	12,	12,			//eMNMICON_DRESS,
	6,	12,	12,			//eMNMICON_DOCTOR,
	7,	12,	12,			//eMNMICON_BOOK,
	8,	12,	12,			//eMNMICON_WAREHOUSE,
	9,	12,	12,			//eMNMICON_PYOGUK,
	10,	12,	12,			//eMNMICON_MUNPA,
	11,	12,	12,			//eMNMICON_YEONMU,
	11, 12, 12,			//eMNMICON_CASTLEMANAGER,

	30,	14, 14,			//eMNICON_QUEST_EXCLAMATION1,
	31, 14, 14,			//eMNICON_QUEST_EXCLAMATION2,
	32, 14, 14,			//eMNICON_QUEST_EXCLAMATION3,

	34,	21,	48,			//eMNICON_CAMERA,
};



CMiniMapDlg::CMiniMapDlg()
{
	m_pMapName	= NULL;
	m_pPosXY	= NULL;
	m_pZoom		= NULL;
	
	m_IconTable.Initialize(64);
	m_ImageDisplayWH.x = 140;
	m_ImageDisplayWH.y = 155;
	m_DiplayOriginPosOfImage.x = 0;	// ?ÃŒÂ¹ÃŒÃÃ¶?Ã‡ Â¾Ã®Â´? ÂºÃÂºÃ?ÃŒ Â¹ÃŒÂ´ÃÂ¸ÃŠ ÃƒÂ¢Â¿Â¡Â¼Â­?Ã‡ 0,0Â¿Â¡ Ã‚Ã¯ÃˆÃ·Â´Ã‚Â°Â¡
	m_DiplayOriginPosOfImage.y = 0;
	
	for(int n=0;n<eMINIMAPMODE_MAX;++n)
	{
		m_MinimapImage[n].ImageHeight = 0;
		m_MinimapImage[n].ImageWidth = 0;
	}	

	m_IconPool.Init(100, 10, "MinimapIconPool");



}

CMiniMapDlg::~CMiniMapDlg()
{
	
	for(int n=0;n<eMINIMAPMODE_MAX;++n)
	{
		m_MinimapImage[n].pMiniMapImage.Release();
		m_MinimapImage[n].ImageHeight = 0;
		m_MinimapImage[n].ImageWidth = 0;
	}

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
		ReleaseMinniMapIcon(pIcon);
	m_IconTable.RemoveAll();

	ReleaseNumImage();
}
/*
void CMiniMapDlg::SetZoomRate(float zoomrate)
{
	m_ZoomRate = zoomrate;

	m_DisplayMinX = m_DiplayOriginPosOfImage.x*m_ZoomRate;
	m_DisplayMinZ = m_DiplayOriginPosOfImage.y*m_ZoomRate;
	m_DisplayMaxX = (m_DiplayOriginPosOfImage.x+m_ImageDisplayWH.x)*m_ZoomRate;
	m_DisplayMaxZ = (m_DiplayOriginPosOfImage.y+m_ImageDisplayWH.y)*m_ZoomRate;
}
*/

void CMiniMapDlg::InitMiniMap(MAPTYPE MapNum)
{
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter ||
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
	{
		SetActive(FALSE);
		//ÕâÀïÊÇ´ò²¹¶¡,ÓÃÀ´´ò¿ªĞÂµØÍ¼µÄÓÒÉÏ½ÇĞ¡Í¼
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
		|| MapNum == PKEVENTMAP || MapNum == DIMENSION0 || MapNum == DIMENSION ) // boss map
		SetActive(FALSE);
	*/

	//SW061019 ¼­¹ÙÀÌ¹ú¸ğµå °ü·Ã
	//if(MAP->CheckMapKindIs(eSurvival))
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		SetActive(FALSE);
		//ÕâÀïÊÇ´ò²¹¶¡,ÓÃÀ´´ò¿ªĞÂµØÍ¼µÄÓÒÉÏ½ÇĞ¡Í¼
		if (MapNum == 23 || MapNum == 24 || MapNum == 26 || MapNum == 29 )
		{
			SetActive(TRUE);
		}
	}
//	TempID = SKILLOBJECT_ID_START;		// Â½ÂºÃ…Â³?ÃŒ Â¹ÃŒÂ´ÃÂ¸ÃŠÂ¿Â¡ ÂµÃ®Â·Ã ÂµÃ‰Â¸Â® Â¾Ã¸?Â¸Â´ÃÂ±Ã®.

	m_MapNum = MapNum;
	m_CurMode = eMINIMAPMODE_FULL;	
	LoadMinimapImageInfo(MapNum);
	if(m_pMapName)
	{
		char buf[128];

		// magi82(37) ¸Ê ¼Ó¼º µ¥ÀÌÅÍ
#ifdef TAIWAN_LOCAL	//chat_msg order
		wsprintf( buf, CHATMGR->GetChatMsg(442),gChannelNum+1,MAP->GetMapName());
#elif defined _HK_LOCAL_	//chat_msg order
		wsprintf( buf, CHATMGR->GetChatMsg(442),gChannelNum+1,MAP->GetMapName());
#else
		wsprintf( buf, CHATMGR->GetChatMsg(442),MAP->GetMapName(),gChannelNum+1);
#endif
//		m_pMapName->SetStaticText(MAP->GetMapName());
		m_pMapName->SetStaticText(buf);
	}
}


void CMiniMapDlg::InitIconImage()
{
	cImage* pImg;
	DWORD w,h;
	for(int n=0;n<eMNMICON_MAX;++n)
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(MINIMAPICON_DESC[3*n+0],pImg,PFT_MINIMAPPATH);
		w = MINIMAPICON_DESC[3*n+1];
		h = MINIMAPICON_DESC[3*n+2];
		m_pIconImage[n].InitIconImage(pImg,w,h);
	}

//KES
	SCRIPTMGR->GetImage( 63, &m_TooltipImage, PFT_HARDPATH );
}

void CMiniMapDlg::Linking()
{
	memset(m_pIconImage,0,sizeof(cImage*)*eMNMICON_MAX);
	InitIconImage();
	m_pMapName	= (cStatic*)GetWindowForID(MNM_MAPNAME);
	m_pPosXY	= (cStatic*)GetWindowForID(MNM_POSXY);
	m_pZoom		= (cPushupButton*)GetWindowForID( MNM_TOGGLEFULL );

	JPInit();
	InitNumImage();	
}

void CMiniMapDlg::AddMonsterIcon(CObject* pObject)
{
}
void CMiniMapDlg::AddBossMonsterIcon(CObject* pObject)
{
}
void CMiniMapDlg::AddStaticNpcIcon(CObject* pObject)
{
	CNpc* pNpc = (CNpc*)pObject;
	WORD kind = pNpc->GetNpcKind();

	switch( kind )
	{
	case 1:		AddIcon(eMNMICON_WEAPON,pObject);	break;
	case 2:		AddIcon(eMNMICON_DRESS,pObject);		break;
	case 3:		AddIcon(eMNMICON_DRESS,pObject);	break;
	case 4:		AddIcon(eMNMICON_DOCTOR,pObject);	break;
	case 5:		AddIcon(eMNMICON_BOOK,pObject);		break;
	case 8:		AddIcon(eBMICON_MUNPA,pObject);		break;
	case 10:
	case 84:
		AddIcon(eMNMICON_WAREHOUSE,pObject);
		break;
	case 11:	AddIcon(eMNMICON_CASTLEMANAGER, pObject);	break;
	case 12:	AddIcon(eMNMICON_PYOGUK,pObject);	break;
	case 13:	AddIcon(eMNMICON_PYOGUK,pObject);	break;
	case 15:	AddIcon(eMNMICON_YEONMU,pObject);	break;
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
		AddIcon(eMNMICON_YEONMU,pObject);
		break;		
	case 31:
	case 81:
	case 82:	
		AddIcon(eMNMICON_MAPCHANGE,pObject);
		break;
	case 30:
	case 83:
		AddIcon(eMNMICON_LOGIN,pObject);
		break;
	default:
		AddIcon(eMNMICON_YEONMU,pObject);
		break;
	}
/*
	if(pNpc->GetNpcKind() == 31)
		AddIcon(eMNMICON_MAPCHANGE,pObject);

	else if(pNpc->GetNpcKind() == 30)
		AddIcon(eMNMICON_LOGIN,pObject);

	else if(pNpc->GetNpcKind() == 1)
		AddIcon(eMNMICON_WEAPON,pObject);

	else if(pNpc->GetNpcKind() == 2)
		AddIcon(eMNMICON_DRESS,pObject);

	else if(pNpc->GetNpcKind() == 3)
		AddIcon(eMNMICON_DRESS,pObject);

	else if(pNpc->GetNpcKind() == 4)
		AddIcon(eMNMICON_DOCTOR,pObject);

	else if(pNpc->GetNpcKind() == 5)
		AddIcon(eMNMICON_BOOK,pObject);

	else if(pNpc->GetNpcKind() == 10)
		AddIcon(eMNMICON_WAREHOUSE,pObject);

	else if(pNpc->GetNpcKind() == 12)
		AddIcon(eMNMICON_PYOGUK,pObject);	

	else if(pNpc->GetNpcKind() == 13)
		AddIcon(eMNMICON_PYOGUK,pObject);	

	else if(pNpc->GetNpcKind() == 8)
		AddIcon(eMNMICON_MUNPA,pObject);
	
	else if(pNpc->GetNpcKind() == 25)
		AddIcon(eMNMICON_PYOGUK,pObject);

	else if(pNpc->GetNpcKind() == 15)
		AddIcon(eMNMICON_YEONMU,pObject);
	
	else if(pNpc->GetNpcKind() == 22)	//ÃÊº¸ÀÚ ¾È³»ÀÎ
		AddIcon(eMNMICON_YEONMU,pObject);

	else if(pNpc->GetNpcKind() == 24)
		AddIcon(eMNMICON_YEONMU, pObject);	//È²¸ª¿µÈ¥

	else if(pNpc->GetNpcKind() == 11)
		AddIcon(eMNMICON_CASTLEMANAGER, pObject);

	else if(pNpc->GetObjectKind() == 45)	// ¼öÇà¾È³»ÀÎ
		AddIcon(eMNMICON_YEONMU, pObject);
*/

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->AddStaticNpcIcon( pObject );
}

void CMiniMapDlg::AddPartyMemberIcon(PARTY_MEMBER* pMember)
{
	AddIcon(eMNMICON_PARTYMEMBER,pMember);
//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->AddPartyMemberIcon( pMember );
}
/*
void CMiniMapDlg::AddQuestIcon( CObject* pTarget, int nKind )
{
	if( nKind == 0 )
		AddIcon(eMNMICON_QUEST_EXCLAMATION, pTarget);
	else
		AddIcon(eMNMICON_QUEST_QUESTION, pTarget);
//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->AddQuestIcon( pTarget, nKind );
}
*/
void CMiniMapDlg::AddHeroIcon(CObject* pObject)
{
	m_HeroIcon.Init(&m_pIconImage[eMNMICON_SELF],pObject, eMNMICON_SELF);
	m_HeroIcon.SetCameraImage( &m_pIconImage[eMNMICON_CAMERA] );
//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->AddHeroIcon( pObject );
}


CMiniMapIcon* CMiniMapDlg::GetEmptyMiniMapIcon(WORD Kind,CObject* pObject)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind],pObject, Kind);
	pIcon->SetMarkImage(&m_pIconImage[eMNMICON_QUEST_EXCLAMATION1],NULL);//&m_pIconImage[eMNICON_QUEST_QUESTION]);
	return pIcon;
}

CMiniMapIcon* CMiniMapDlg::GetEmptyMiniMapIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	CMiniMapIcon* pIcon = m_IconPool.Alloc();
	pIcon->Init(&m_pIconImage[Kind], pMember, Kind);
	return pIcon;
}


void CMiniMapDlg::ReleaseMinniMapIcon(CMiniMapIcon* pIcon)
{
	m_IconPool.Free(pIcon);
}


void CMiniMapDlg::AddIcon(WORD Kind,CObject* pObject)
{
	ASSERT(m_IconTable.GetData(pObject->GetID()) == NULL);
	RemoveIcon(pObject->GetID());
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pObject);
	m_IconTable.Add(pIcon,pObject->GetID());
}

void CMiniMapDlg::AddIcon(WORD Kind,PARTY_MEMBER* pMember)
{
	ASSERT(m_IconTable.GetData(pMember->dwMemberID) == NULL);
	RemoveIcon(pMember->dwMemberID);
	CMiniMapIcon* pIcon = GetEmptyMiniMapIcon(Kind,pMember);
	m_IconTable.Add(pIcon,pMember->dwMemberID);
}

void CMiniMapDlg::RemoveIcon(DWORD ObjectID)
{
	CMiniMapIcon* pIcon = (CMiniMapIcon*)m_IconTable.GetData(ObjectID);
	if(pIcon == NULL)
		return;

	ReleaseMinniMapIcon(pIcon);
	m_IconTable.Remove(ObjectID);
}

void CMiniMapDlg::Process()
{
	MINIMAPIMAGE* pMinimap = &m_MinimapImage[m_CurMode];
	DWORD iWidth,iHeight;
	if(m_CurMode == eMINIMAPMODE_FULL)
	{
		iWidth = (DWORD)m_ImageDisplayWH.x;
		iHeight = (DWORD)m_ImageDisplayWH.y;
	}
	else
	{
		iWidth = pMinimap->ImageWidth;
		iHeight = pMinimap->ImageHeight;
	}	

	m_HeroIcon.Update(iWidth,iHeight);
	// PartyIconÂµÂµ UpdateÃƒÃŸÂ°Â¡
	PICONMGR->Process();
	
	char temp[64];
	if(m_pPosXY)
	{
		VECTOR3 heropos = m_HeroIcon.GetPosition();

		sprintf( temp, "%4.0f,%4.0f", heropos.x*.01f,heropos.z*.01f );
		//sprintf(temp,"%s-ÃƒÂ¤Â³Ã%d %4.0f,%4.0f",MAP->GetMapName(),gChannelNum+1,heropos.x*.01f,heropos.z*.01f);
		//Â¸ÃŠ?ÃŒÂ¸Â§Â°Ãº ÃÃ‚Ã‡Â¥Â´Ã‚ ÂµÃ»Â·ÃÃƒÂ³Â¸Â®.
		m_pPosXY->SetStaticText(temp);
	}

	if(m_CurMode == eMINIMAPMODE_FULL)
		FitToCenterIcon(NULL);
	else
		FitToCenterIcon(&m_HeroIcon);

	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Update(iWidth,iHeight);
	}

	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->Process();

	//Jackpot
	JPProcess();
}

void CMiniMapDlg::FitToCenterIcon(CMiniMapIcon* pIcon)
{
	if(pIcon == NULL)
	{
		m_DiplayOriginPosOfImage.x = 0;
		m_DiplayOriginPosOfImage.y = 0;
		return;
	}
	
	m_DiplayOriginPosOfImage.x = m_HeroIcon.GetScreenPos()->x - (m_ImageDisplayWH.x*.5f);
	m_DiplayOriginPosOfImage.y = m_HeroIcon.GetScreenPos()->y - (m_ImageDisplayWH.y*.5f);

	m_DisplayMinX = m_DiplayOriginPosOfImage.x;
	m_DisplayMinZ = m_DiplayOriginPosOfImage.y;
	m_DisplayMaxX = (m_DiplayOriginPosOfImage.x+m_ImageDisplayWH.x);
	m_DisplayMaxZ = (m_DiplayOriginPosOfImage.y+m_ImageDisplayWH.y);
}

void CMiniMapDlg::LoadMinimapImageInfo(MAPTYPE MapNum)
{
	DIRECTORYMGR->SetLoadMode(eLM_Minimap);

	char descFile[64],MiniMapImageFilename[256];

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
	
	for(int n=0;n<eMINIMAPMODE_MAX;++n)
	{
		file.GetString(MiniMapImageFilename);
		if( file.IsEOF() )
			break;

		m_MinimapImage[n].ImageWidth = file.GetDword();
		m_MinimapImage[n].ImageHeight = file.GetDword();
		
		m_MinimapImage[n].pMiniMapImage.LoadSprite(MiniMapImageFilename);
	}
	
	file.Release();	
	DIRECTORYMGR->SetLoadMode(eLM_Root);

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->LoadMinimapImageInfo( MapNum );
}

DWORD CMiniMapDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = cDialog::ActionEvent( mouseInfo );
	if( !m_bActive ) return we;

	if( we & WE_MOUSEOVER )
	{
		cImageRect rect;
		rect.left = (LONG)m_DiplayOriginPosOfImage.x;
		rect.top = (LONG)m_DiplayOriginPosOfImage.y;
		rect.right = (LONG)(m_DiplayOriginPosOfImage.x+m_ImageDisplayWH.x);
		rect.bottom = (LONG)(m_DiplayOriginPosOfImage.y+m_ImageDisplayWH.y);
		
		char temp[64] = {0,};
		int x = mouseInfo->GetMouseX();
		int y = mouseInfo->GetMouseY();
		VECTOR2 trans;
		trans.x = GetAbsX() + 5;
		trans.y = GetAbsY() + 25;
		trans.x -= rect.left;
		trans.y -= rect.top;
		
		CMiniMapIcon* pIcon;
		m_IconTable.SetPositionHead();
		while(pIcon = m_IconTable.GetData())
		{
			int sort = pIcon->GetToolTip(x,y,&trans,temp);
			if(sort == 1)				// Npc
			{
				SetToolTip( temp, RGB_HALF( 255, 255, 0 ) );
			}
			else if(sort == 2)			// Party
			{
				SetToolTip( temp, RGB_HALF(185, 241, 69) );
			}
		}
		if(temp[0] == 0)
		{
			SetToolTip("");
		}
	}

	return we;
}

void CMiniMapDlg::Render()
{
	if( !m_bActive ) return;

	cDialog::RenderWindow();
	
	MINIMAPIMAGE* pMinimap = &m_MinimapImage[m_CurMode];

	if(pMinimap->pMiniMapImage.IsNull())
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Â¹Ã¨Â°Ã¦ Â¶Ã§Â¿Ã¬Â±Ã¢
	cImageRect rect;
	rect.left = (LONG)m_DiplayOriginPosOfImage.x;
	rect.top = (LONG)m_DiplayOriginPosOfImage.y;
	rect.right = (LONG)(m_DiplayOriginPosOfImage.x+m_ImageDisplayWH.x);
	rect.bottom = (LONG)(m_DiplayOriginPosOfImage.y+m_ImageDisplayWH.y);
	pMinimap->pMiniMapImage.SetImageSrcRect(&rect);

	VECTOR2 scale;
	scale.x = m_ImageDisplayWH.x / pMinimap->ImageWidth;
	scale.y = m_ImageDisplayWH.y / pMinimap->ImageHeight;

	VECTOR2 trans;
	trans.x = GetAbsX() + 5;
	trans.y = GetAbsY() + 25;
	
	
	static BYTE al = 150;

	DWORD color = RGBA_MAKE(255,255,255,al);

	pMinimap->pMiniMapImage.RenderSprite(&scale,NULL,0,&trans,color);

	trans.x -= rect.left;
	trans.y -= rect.top;
		
//	char temp[64] = {0,};
	WORD x = MOUSE->GetMouseX();
	WORD y = MOUSE->GetMouseY();

	//////////////////////////////////////////////////////////////////////////
	// Â³ÂªÂ¸Ã“ÃÃ¶ Â¾Ã†?ÃŒÃ„Ãœ(Â¸Ã·,NPC)ÂµÃ© Â¶Ã§Â¿Ã¬Â±Ã¢
	CMiniMapIcon* pIcon;
	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->Render(&trans,&rect);
	}

	m_IconTable.SetPositionHead();
	while(pIcon = m_IconTable.GetData())
	{
		pIcon->RenderQuestMark(&trans,&rect);
	}

	//////////////////////////////////////////////////////////////////////////
	// ÃÃ–?ÃÂ°Ã¸ Â¶Ã§Â¿Ã¬Â±Ã¢
	m_HeroIcon.Render(&trans);

	cDialog::RenderComponent();

	//jackpot

	JPRender();

}

void CMiniMapDlg::RefreshMode()
{
	if( m_pZoom )
		m_pZoom->SetPush( GAMEIN->GetBigMapDialog()->IsActive() );
}

void CMiniMapDlg::ToggleMinimapMode()
{
	if( GAMEIN->GetBigMapDialog() )
	{
		GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
	}

	RefreshMode();	

/*
	if(m_CurMode == eMINIMAPMODE_FULL)
		m_CurMode = eMINIMAPMODE_SMALL;
	else
		m_CurMode = eMINIMAPMODE_FULL;
*/
}

void CMiniMapDlg::SetActive( BOOL val )
{
	if( CanActive() == FALSE )
		val = FALSE;
	
	cDialog::SetActive( val );

//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_MINIMAPDLGICON, m_bActive );
}


void CMiniMapDlg::SetPartyIconTargetPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
		pIcon->SetTargetPos(posX, posZ);

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->SetPartyIconTargetPos( PlayerID, posX, posZ );
}

void CMiniMapDlg::SetPartyIconObjectPos(DWORD PlayerID, int posX, int posZ)
{
	CMiniMapIcon* pIcon = m_IconTable.GetData(PlayerID);
	if(pIcon)
	{
		pIcon->SetObjectPos(posX, posZ);
		pIcon->SetTargetPos(posX, posZ);
	}

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->SetPartyIconObjectPos( PlayerID, posX, posZ );
}

BOOL CMiniMapDlg::CanActive()
{
	/*
	BOOL rt;
	
	MAPTYPE mapnum = MAP->GetMapNum();
	switch(mapnum)
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
	*/

	// magi82(37) ¸Ê ¼Ó¼º µ¥ÀÌÅÍ
	return MAP->IsMapKind(eMapView);
}

void CMiniMapDlg::ShowQuestMarkIcon( CObject* pTarget, int nKind )
{
	CMiniMapIcon* pIcon = m_IconTable.GetData( pTarget->GetID() );
	if( pIcon == NULL ) return;

	pIcon->ShowQuestMark( nKind );

//---
	if( GAMEIN->GetBigMapDialog() )
		GAMEIN->GetBigMapDialog()->ShowQuestMarkIcon( pTarget, nKind );
}


//SW050722 jackpot_number
LONG NumImgRelpos[] =
{
	135, 191,
	119, 191,
	103, 191,
	87, 191,
	71, 191,
	55, 191,
	39, 191,
	23, 191,
	7, 191,
};

void CMiniMapDlg::InitNumImage()
{
	cImage* pImg;
	for( int n = 0; n < IMAGE_COUNT; ++n )
	{
		pImg = new cImage;
		SCRIPTMGR->GetImage(n, pImg, PFT_JACKPOTPATH);
		m_stNumImage[n].pImage = pImg;
//		m_stNumImage[n].dwW = NUMIMAGE_W;
//		m_stNumImage[n].dwH = NUMIMAGE_H;
	}
}

void CMiniMapDlg::ReleaseNumImage()
{
	for( int n = 0; n < IMAGE_COUNT; ++n )
	{
		if(m_stNumImage[n].pImage)
		{
			delete m_stNumImage[n].pImage;
			m_stNumImage[n].pImage = NULL;
		}
	}
}

void CMiniMapDlg::SetNumImagePos()
{
	float fDialPosX = this->GetAbsX();
	float fDialPosY = this->GetAbsY();

	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		m_vPos[i].x = fDialPosX + (float)NumImgRelpos[i*2];
		m_vPos[i].y = fDialPosY + (float)NumImgRelpos[i*2+1];
	}

	//SW050805 ÀÚ¸®¼ö ±¸ºĞÁ¡ Ãß°¡
	m_vDotPos[0].x = fDialPosX + 98;
	m_vDotPos[0].y = fDialPosY + 205;
	m_vDotPos[1].x = fDialPosX + 50;
	m_vDotPos[1].y = fDialPosY + 205;
}

void CMiniMapDlg::ConvertCipherNum()
{// °¢ÀÚ¸® ¼ıÀÚ¸¦ ±¸ÇÑ´Ù. Get Number per Each Cipher
	// ¼ıÀÚ º¯È­½Ã °»½Å ¾Ö´Ï¸¦ ÇÑ´Ù. Do Animations When Number Changers
/*
	int n = 0;	// nÀÚ¸®¼ö
	DWORD d = 1;	// ´ÜÀ§

	DWORD money = m_dwTotalMoney;

	if( !money ) return;
	while( money / d > 0 )
	{//	ÀÚ¸®¼ö ±¸ÇÏ±â n
		d *= 10;
		n++;
	}

	ASSERT(n<CIPHER_NUM);
	d /= 10;

	m_dwMaxCipher = n;	// Need in ChangingAni

	for( int i = 1; i <= n; ++i )
	{// Å«ÀÚ¸® ºÎÅÍ
		m_stCipherNum[n-i].dwNumber = money / d;
		m_stCipherNum[n-i].bIsAni = TRUE;
		money = m_dwTotalMoney % d;
		d /= 10;
	}
*/
	int n = 1;	// nÀÚ¸®¼ö 0 µµ 1ÀÚ¸®·Î º»´Ù. ±×¸®±âÀ§ÇØ!
	DWORD d = 10;
	while( m_dwTotalMoney / d > 0 )
	{
		d *= 10;
		n++;
	}
	
	m_dwMaxCipher = n;

	DWORD dec = 10;
	DWORD tmp = m_dwTotalMoney;

	for( int i = 0; i < n; ++i )
	{
		m_stCipherNum[i].dwNumber = tmp % dec;
		m_stCipherNum[i].bIsAni = TRUE;
		tmp /= dec;
	}

	while( n < CIPHER_NUM )
	{
		m_stCipherNum[n].dwNumber = DEFAULT_IMAGE;
		m_stCipherNum[n].bIsAni = FALSE;
		n++;
	}
}

bool CMiniMapDlg::IsNumChanged()					// ¼ıÀÚ º¯È­ Ã¼Å©	// Check Num charnges
{
	if(m_dwOldTotalMoney != m_dwTotalMoney)
	{
		if( m_dwTotalMoney < m_dwOldTotalMoney || m_dwOldTotalMoney == 0 )	// µ· ÁÙ°Å³ª ¸ÊÃ³À½ Á¢¼ÓÀÌ¸é..
		{
			m_bDoSequenceAni = FALSE;
		}
		else m_bDoSequenceAni = TRUE;
		
		m_dwTempMoney = m_dwOldTotalMoney;	// For SequenceAni

		m_dwOldTotalMoney = m_dwTotalMoney;	// For CompareChanges
		return TRUE;
	}
	else
		return FALSE;
}

void CMiniMapDlg::DoAni()				// ¼ıÀÚ º¯È¯ ¾Ö´Ï	// Ticktime control
{
	if(!m_bIsAnimationing) return;

	DWORD curtime = gCurTime;

	if( curtime - m_dwNumChangeTime > NUM_CHANGE_TIMELENGTH )
	{
		for( int i = 0; i < CIPHER_NUM; ++i )
		{
			if( m_stCipherNum[i].bIsAni )
			{
				m_stCipherNum[i].dwNumber++;
				if(m_stCipherNum[i].dwNumber > 9)
				{
					m_stCipherNum[i].dwNumber = 0;
				}
			}
		}

//		m_dwNumChangeTime = curtime;
	}

	if( curtime - m_dwAniStartTime < BASIC_ANI_TIMELENGTH ) return;

	if( curtime - m_dwIntervalAniTime > BETWEEN_ANI_TIMELENGTH )
	{
		if(m_stCipherNum[m_dwCipherCount].bIsAni)
		{
			m_stCipherNum[m_dwCipherCount].dwNumber = m_stCipherNum[m_dwCipherCount].dwRealCipherNum;
			m_stCipherNum[m_dwCipherCount].bIsAni = FALSE;
		}

		if(m_dwCipherCount == m_dwMaxCipher)
		{
			m_bIsAnimationing = FALSE;
			return;
		}

		m_dwIntervalAniTime = curtime;
		m_dwCipherCount++;

	}
}

void CMiniMapDlg::DoSequenceAni()
{
	if(!m_bIsAnimationing)
	{
//		m_dwTotalMoney = m_dwOldTotalMoney;	// When TotalMoney Decrease // µ·ÁÙ¾îµé¸é ±×³É Âï°í¸¸´Ù.(KES PM´Ô Áö½Ã)
		return;
	}

	DWORD EllipsedTime =  gCurTime - m_dwAniStartTime;
	
	DWORD durMoney =  EllipsedTime * 33;

	m_dwTotalMoney = m_dwTempMoney + durMoney;

	if( m_dwTotalMoney >= m_dwOldTotalMoney )
	{
		m_dwTotalMoney = m_dwOldTotalMoney;
		m_bIsAnimationing = FALSE;
	}
}

void CMiniMapDlg::JPRender()
{//

	//if(!m_bActive) return;
	// ¼ıÀÚ ÀÌ¹ÌÁö Ãâ·Â // Æ÷Áî°ª, ÀÌ¹ÌÁö¹øÈ£
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		if(m_stCipherNum[i].dwNumber == DEFAULT_IMAGE)
			continue;
		m_stNumImage[m_stCipherNum[i].dwNumber].pImage->RenderSprite( NULL, NULL, 0.0f, &m_vPos[i], 0xffffffff );
	}

	//SW050805 ÀÚ¸®¼ö ±¸ºĞÁ¡ Ãß°¡
	if( m_dwMaxCipher < 4 || m_dwMaxCipher > 9 ) return;
	int j = ( m_dwMaxCipher - 1 ) / 3;
	while( j )
	{
		m_stNumImage[IMG_DOT].pImage->RenderSprite( NULL, NULL, 0.0f, &m_vDotPos[j-1], 0xffffffff );
		j--;
	}
}

void CMiniMapDlg::JPProcess()
{// JACKPOTMGR·Î ºÎÅÍ TotalMoney °»½Å
	m_dwTotalMoney = JACKPOTMGR->GetJPTotalMoney();
	if( m_dwTotalMoney > 999999999 )
	{
		m_dwTotalMoney = 999999999;
	}
	
	SetNumImagePos();
/*
	// route for DoAni	//ÇÏ³ª¾¿ º¯ÇÏ´Â ¾Ö´Ï
//	if(IsNumChanged())
//	{
//		ConvertCipherNum();
//		InitForAni();
//	}
//
//	DoAni();
*/

	// route for DoSequenceAni	//½×ÀÌ´Â µíÇÑ ¾Ö´Ï
	if(IsNumChanged())
	{
		InitForSequenceAni();
	}

	DoSequenceAni();
	
	ConvertCipherNum();
}

void CMiniMapDlg::InitForAni()
{
	//½ÇÁ¦ ¼ıÀÚ¸¦ ÀúÀåÇØµÎ°í ¸ğµç ¼ıÀÚ ¾Ö´Ï¸¦ ºÒ°¡ »óÅÂ·Î µĞ´Ù.
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		m_stCipherNum[i].dwRealCipherNum = m_stCipherNum[i].dwNumber;
	}
	// ÀüÃ¼ ¾Ö´Ï ½ÃÀÛ
	m_bIsAnimationing = TRUE;
	// ¾Ö´Ï Ä«¿îÅÍ ÃÊ±âÈ­
	m_dwCipherCount = 0;
	// ¾Ö´Ï ½ÃÀÛ ½Ã°¢
	m_dwAniStartTime = gCurTime;
}

void CMiniMapDlg::InitForSequenceAni()
{
	if(!m_bDoSequenceAni) return;

	m_bIsAnimationing = TRUE;

	m_dwAniStartTime = gCurTime;

	m_dwTotalMoney = m_dwOldTotalMoney;// When TotalMoney Decrease // µ·ÁÙ¾îµé¸é ±×³É Âï°í¸¸´Ù.(KES PM´Ô Áö½Ã)
}

void CMiniMapDlg::JPInit()
{
	memset(m_vPos, 0, sizeof(VECTOR2)*CIPHER_NUM);		// ¼ıÀÚÃ¢ ÀÚ¸®¼ö À§Ä¡

	m_dwTotalMoney	= 0;			// ÃÑ±İ¾×
	m_dwOldTotalMoney	= 0;
	m_dwTempMoney	= 0;

	m_dwAniStartTime	= 0;
	m_dwNumChangeTime	= 0;
	m_dwIntervalAniTime	= 0;	
	m_dwMaxCipher	= 0;
	m_dwCipherCount	= 0;
	m_bIsAnimationing	= FALSE;
	m_bDoSequenceAni	= FALSE;

	ConvertCipherNum();
	//SW050802 snake_legs
//	m_BeforeTime = 0;
}

/*	//SW050802 snake_legs
void CMiniMapDlg::SetParticlePos()
{
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		for( int j = 0; j < PARTICLE_PER_NUM; ++j )
		{
			int sign_x = rand()%2;
			sign_x > 0 ? sign_x = 1 : sign_x = -1;
			m_stPtc[i][j].vDir.x = rand()%RND_RANGE * sign_x;
			m_stPtc[i][j].vPos.x = m_vPos[i].x;
			int sign_y = rand()%2;
			sign_y > 0 ? sign_y = 1 : sign_y = -1;
			m_stPtc[i][j].vDir.y = rand()%RND_RANGE * sign_y;
			m_stPtc[i][j].vPos.y = m_vPos[i].y;
		}
	}
}
	
void CMiniMapDlg::CalcMotion()
{
	if( gCurTime - m_BeforeTime < 250 ) return;
	for( int i = 0; i < CIPHER_NUM; ++i )
	{
		for( int j = 0; j < PARTICLE_PER_NUM; ++j )
		{
			m_stPtc[i][j].vPos.x += m_stPtc[i][j].vDir.x;
			m_stPtc[i][j].vDir.y -= GRAVITY;
			m_stPtc[i][j].vPos.y += m_stPtc[i][j].vDir.y;
		}
	}

	m_BeforeTime = gCurTime;
}
*/
