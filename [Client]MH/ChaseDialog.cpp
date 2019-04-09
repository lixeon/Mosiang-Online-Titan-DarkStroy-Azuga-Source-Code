// ChaseDialog.cpp: implementation of the CChaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaseDialog.h"
#include "WindowIDEnum.h"
#include "MHMap.h"
#include "MHFile.h"
#include "GameResourceManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "MHMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char PosMsg[3][3][10] = 
{
	{ "북서", "북쪽", "북동", },
	{ "서쪽", "중심", "동쪽", },
	{ "남서", "남쪽", "남동", },
};

CChaseDialog::CChaseDialog()
{
	m_type = WT_CHASE_DLG;

}

CChaseDialog::~CChaseDialog()
{

}


void CChaseDialog::Linking()
{
	m_pMap = (cStatic*)GetWindowForID( SEE_CHASE_MAP );
	m_TextArea = (cTextArea*)GetWindowForID( SEE_CHASE_MSG );

	m_bActive = FALSE;	
	SCRIPTMGR->GetImage( 126, &m_pIconImage );
	m_MapNum = 0;

#ifdef _JP_LOCAL_
	for( int i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			SafeStrCpy( PosMsg[i][j], CHATMGR->GetChatMsg( 1293+i*3+j ), 10 );
		}
	}
#endif
#ifdef _HK_LOCAL_
	for( int i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			SafeStrCpy( PosMsg[i][j], CHATMGR->GetChatMsg( 1293+i*3+j ), 10 );
		}
	}
#endif
#ifdef _TL_LOCAL_
	for( int i = 0 ; i < 3 ; ++i )
	{
		for( int j = 0 ; j < 3 ; ++j )
		{
			SafeStrCpy( PosMsg[i][j], CHATMGR->GetChatMsg( 1293+i*3+j ), 10 );
		}
	}
#endif
}


void CChaseDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	m_bActive = val;
}


BOOL CChaseDialog::InitMiniMap( MAPTYPE MapNum, WORD PosX, WORD PosY, char* strName, MAPTYPE EventMapNum )
{
	MAPTYPE LoadMap = MapNum;
	if( EventMapNum == 44 )	
		LoadMap = EventMapNum;

	m_EventMapNum = EventMapNum;

	if( !LoadMinimapImageInfo( LoadMap ) )
			return FALSE;
	
	m_TargetPos.x = PosX;
	m_TargetPos.y = PosY;
	SafeStrCpy( m_WantedName, strName, MAX_NAME_LENGTH+1 );

	return TRUE;
}


BOOL CChaseDialog::LoadMinimapImageInfo(MAPTYPE MapNum)
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
		return FALSE;
	}
	
	for( int n=0; n<2; ++n )
	{
		file.GetString(MiniMapImageFilename);
		if( file.IsEOF() )
			break;

		m_MapImage.ImageWidth = file.GetDword();
		m_MapImage.ImageHeight = file.GetDword();			

		if( n == 1 )
			m_MapImage.pMiniMapImage.LoadSprite(MiniMapImageFilename);
	}
	
	file.Release();	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	m_MapNum = MapNum;

	return TRUE;
}


void CChaseDialog::Render()
{
	if( !m_bActive ) return;

	cDialog::RenderWindow();
	
	if(m_MapImage.pMiniMapImage.IsNull())
		return;

	cImageRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 0+m_MapImage.ImageWidth;
	rect.bottom = 0+m_MapImage.ImageHeight;
	m_MapImage.pMiniMapImage.SetImageSrcRect(&rect);

	VECTOR2 scale;
	scale.x = (float)(m_pMap->GetWidth() / m_MapImage.ImageWidth);
	scale.y = (float)(m_pMap->GetHeight() / m_MapImage.ImageHeight);

	VECTOR2 trans;
	trans.x = m_pMap->GetAbsX();
	trans.y = m_pMap->GetAbsY();	
	
	DWORD color = RGBA_MAKE(255,255,255,255);
	m_MapImage.pMiniMapImage.RenderSprite(&scale,NULL,0,&trans,color);

	// 범인 위치 표시
	DWORD x = (DWORD)(m_TargetPos.x / (51200/3));
	DWORD y = (DWORD)(m_TargetPos.y / (51200/3));

	trans.x = m_pMap->GetAbsX() + x*(m_pMap->GetWidth()/3);
	trans.y = m_pMap->GetAbsY() + (2-y)*(m_pMap->GetHeight()/3);
	
	color = RGBA_MAKE(255,255,255,200);
	m_pIconImage.RenderSprite( &scale, NULL, 0, &trans, color );

	// magi82(37) 맵 속성 데이터
	char buf[256];
	if( m_EventMapNum == 44 )
		sprintf( buf, CHATMGR->GetChatMsg(923), m_WantedName, MAP->GetMapName( m_MapNum ), MAP->GetMapName( m_EventMapNum ) );
	else
		sprintf( buf, CHATMGR->GetChatMsg(906), m_WantedName, MAP->GetMapName( m_MapNum ), m_EventMapNum, PosMsg[(2-y)][x] );
	m_TextArea->SetScriptText( buf );
	
	cDialog::RenderComponent();
}
