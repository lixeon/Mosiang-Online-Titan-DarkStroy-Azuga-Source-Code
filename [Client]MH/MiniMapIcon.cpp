// MiniMapIcon.cpp: implementation of the CMiniMapIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniMapIcon.h"
#include "./Interface/cImage.h"
#include "Object.h"
#include "../[cc]header/GameResourceManager.h"
#include "Npc.h"
#include "./Interface/cFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniMapIconImage::~CMiniMapIconImage()
{
	SAFE_DELETE(m_pImage);
}

CMiniMapIcon::CMiniMapIcon()
{
	m_pIconImage = NULL;
	m_Kind = 0;
	m_Interpolation = 10;
	m_dwColor = 0xffffffff;
	m_bAlwaysTooltip = FALSE;

	ZeroMemory( m_pMarkExclamation, sizeof(m_pMarkExclamation) );
	ZeroMemory( m_pMarkQuestion, sizeof(m_pMarkQuestion) );

	m_ShowQuestMark = eQM_NONE;
}

CMiniMapIcon::~CMiniMapIcon()
{

}


void CMiniMapIcon::SetMarkImage(CMiniMapIconImage* pMarkExclamation, CMiniMapIconImage* pMarkQuestion)
{
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( pMarkExclamation )
			m_pMarkExclamation[i] = pMarkExclamation+i;
		if( pMarkQuestion )
			m_pMarkQuestion[i]	= pMarkQuestion+i;
	}
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage, int Kind )
{
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,CObject* pObject, int Kind)
{
	m_pObject = pObject;
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,VECTOR3* pObjectPos, int Kind)
{
	m_pObject = NULL;
	m_ObjectPos = *pObjectPos;
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}

void CMiniMapIcon::Init(CMiniMapIconImage* pIconImage,PARTY_MEMBER* pMember, int Kind)
{
	m_pObject = NULL;
	strcpy(m_MemberName, pMember->Name);
	m_pIconImage = pIconImage;
	m_Kind = Kind;
}



VECTOR3 CMiniMapIcon::GetPosition()
{
	if(m_pObject)
	{
		m_pObject->GetPosition(&m_ObjectPos);
	}
	return m_ObjectPos;
}

void CMiniMapIcon::Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight)
{
	if(m_Kind == eMNMICON_PARTYMEMBER)
	{
		m_ObjectPos.x += (m_TargetPos.x - m_ObjectPos.x)/m_Interpolation;
		m_ObjectPos.z += (m_TargetPos.z - m_ObjectPos.z)/m_Interpolation;		
	}
	else if(m_pObject)
	{
		if(m_pObject->IsInited() == FALSE)
			return;

		m_pObject->GetPosition(&m_ObjectPos);
	}

	m_ScreenPos.x = (m_ObjectPos.x / 51200)*MinimapImageWidth;
	m_ScreenPos.y = MinimapImageHeight - (m_ObjectPos.z / 51200)*MinimapImageHeight;
}

void CMiniMapIcon::Render(VECTOR2* pMapStartPixel,RECT* pCullRect)
{
	if( pCullRect )
	{
		if(pCullRect->left > m_ScreenPos.x || m_ScreenPos.x > pCullRect->right)
			return;
		if(pCullRect->top > m_ScreenPos.y || m_ScreenPos.y > pCullRect->bottom)
			return;
	}

	VECTOR2 trans;
	trans.x = m_ScreenPos.x - m_pIconImage->m_ImageWH.x*0.5f + pMapStartPixel->x;
	trans.y = m_ScreenPos.y - m_pIconImage->m_ImageWH.y*0.5f + pMapStartPixel->y;
	
	m_pIconImage->m_pImage->RenderSprite(NULL,NULL,0,&trans,m_dwColor);


/*
	if( m_bAlwaysTooltip )
	{
		char name[64];
		GetToolTip( 0, 0, NULL, name );
		name[63] = 0;
		RECT rect;
		SetRect(&rect,trans.x,trans.y,30,10);
		CFONT_OBJ->RenderFontAlpha( 0, name, strlen(name), &rect, RGBA_MAKE( 255, 255, 255, 100 ) );
	}
*/
}

void CMiniMapIcon::RenderQuestMark(VECTOR2* pMapStartPixel,RECT* pCullRect)
{
	if( m_ShowQuestMark == eQM_NONE )
		return;

	if( pCullRect )
	{
		if(pCullRect->left > m_ScreenPos.x || m_ScreenPos.x > pCullRect->right)
			return;
		if(pCullRect->top > m_ScreenPos.y || m_ScreenPos.y > pCullRect->bottom)
			return;
	}

	VECTOR2 trans;

	static DWORD dwTime = gCurTime;
	static DWORD dwSpecialTime = gCurTime;

	static int n = 0;
//	static int nSpecial = 0;
	
	if( m_ShowQuestMark == eQM_EXCLAMATION )
	{
		if( m_pMarkExclamation[n] )
		{
			trans.x = m_ScreenPos.x - m_pMarkExclamation[n]->m_ImageWH.x*0.5f + pMapStartPixel->x -1;
			trans.y = m_ScreenPos.y - m_pMarkExclamation[n]->m_ImageWH.y*0.5f + pMapStartPixel->y -1;
			m_pMarkExclamation[n]->m_pImage->RenderSprite( NULL, NULL, 0.0f, &trans, m_dwColor );		
		}
	}
	else if( m_ShowQuestMark == eQM_QUESTION )
	{
		if( m_pMarkQuestion[n] )
		{
			trans.x = m_ScreenPos.x - m_pMarkQuestion[n]->m_ImageWH.x*0.5f + pMapStartPixel->x -1;
			trans.y = m_ScreenPos.y - m_pMarkQuestion[n]->m_ImageWH.y*0.5f + pMapStartPixel->y -1;
			m_pMarkQuestion[n]->m_pImage->RenderSprite( NULL, NULL, 0.0f, &trans, m_dwColor );				
		}
	}

	if( gCurTime - dwTime > 150 )
	{
		n = (n+1)%3;

		dwTime = gCurTime;
	}

}

int CMiniMapIcon::GetToolTip(int x,int y,VECTOR2* pMapStartPixel,char* pOutText, float* distOut )
{
	if(m_pObject == FALSE && m_Kind != eMNMICON_PARTYMEMBER)
		return 0;

	if( pMapStartPixel == NULL ) return 0;

	int gapx = x - int(m_ScreenPos.x+pMapStartPixel->x);
	int gapy = y - int(m_ScreenPos.y+pMapStartPixel->y);
	int halfx = int(m_pIconImage->m_ImageWH.x*0.5f);
	int halfy = int(m_pIconImage->m_ImageWH.y*0.5f);
		
	if(gapx > halfx || gapx < -halfx)
		return 0;
	if(gapy > halfy || gapy < -halfy)
		return 0;

	if( distOut )
		*distOut = sqrtf( (float)(gapx*gapx + gapy*gapy) );

	if(m_Kind == eMNMICON_PARTYMEMBER)
	{
		strcpy(pOutText, m_MemberName);
		return 2;
	}
	else if(m_pObject->GetObjectKind() == eObjectKind_Npc )
	{
		NPC_LIST* pInfo = GAMERESRCMNGR->GetNpcInfo(((CNpc*)m_pObject)->GetNpcKind());
		wsprintf( pOutText, "%s(%s)", m_pObject->GetObjectName(), pInfo->Name );
	}
	else
	{
		strcpy(pOutText,m_pObject->GetObjectName());
	}

	return 1;
}



