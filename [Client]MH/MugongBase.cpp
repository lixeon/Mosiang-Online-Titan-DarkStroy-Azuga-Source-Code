// MugongBase.cpp: implementation of the CMugongBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MugongBase.h"
#include "./Interface/cFont.h"
#include "./Input/Mouse.h"
#include "interface/cScriptManager.h"
#include "SkillManager_client.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "MugongManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMugongBase::CMugongBase()
{
	SCRIPTMGR->GetImage( 127, &m_DelayImage  );	
	m_bRemainTime	= FALSE;
	m_dwLastSecond	= 0;
	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	SCRIPTMGR->GetImage( 87, &m_OptionImage, PFT_MUGONGPATH );
	//////////////////////////////////////////////////////////////////////////
}

CMugongBase::~CMugongBase()
{

}

void CMugongBase::SetMugongBase(MUGONGBASE * mugong)
{
	memcpy(&m_MugongBaseInfo, mugong, sizeof(MUGONGBASE));
	SetExpPoint(mugong->ExpPoint);
}
void CMugongBase::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * lowImage, cImage * highImage, LONG ID)
{
	cIcon::Init(x,y,wid,hei,lowImage,ID);
	m_type = WT_MUGONG;
	if(highImage)
		m_pHighLayerImage = *highImage;
	
	cImage bimg,gimg;
	cImageRect rect;
	SetRect(&rect,1010,216,1011,226);
	SCRIPTMGR->GetImage(0,&bimg,&rect);	//basicimage
	SetRect(&rect,1006,216,1007,226);
	SCRIPTMGR->GetImage(0,&gimg,&rect); //guageimage

	m_Guage.Init(x-1,y+42,40,6, &bimg);
	cImageScale scale;
	scale.x = 40;
	scale.y = 0.6f;
	m_Guage.SetScale(&scale);
	m_Guage.SetGuageImagePos(0,0);
	m_Guage.SetValue(0);
	m_Guage.SetPieceImage(&gimg);
	m_Guage.SetGuageWidth(40);
	m_Guage.SetGuagePieceWidth(1);
	m_Guage.SetGuagePieceHeightScale((float)0.6);

	m_dwFontColor = 0xffffffff;
}
void CMugongBase::Render()
{
//	if(GetSung() == 0)
//		SetAlpha(120);
//	else
//		SetAlpha(255);
	cIcon::Render();
	if(!m_pHighLayerImage.IsNull())
		m_pHighLayerImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	if(m_MugongBaseInfo.OptionIndex)
		m_OptionImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	//////////////////////////////////////////////////////////////////////////
	
	if(m_MugongBaseInfo.Sung > 0)
	{
		static char nums[3];
		if(m_MugongBaseInfo.Sung >= 12)
			wsprintf(nums,"%s", CHATMGR->GetChatMsg(179));
		else
			wsprintf(nums,"%d", m_MugongBaseInfo.Sung);
		RECT rect={(LONG)m_absPos.x+1, (LONG)m_absPos.y+28, 1,1};
		
		CFONT_OBJ->RenderFont( 0, nums, strlen(nums), &rect, m_dwFontColor );

		//////////////////////////////////////////////////////////////////////////
		// 딜레이 표시
		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, GetItemIdx(), &dwRemainTime );
		
		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}
/*
		if( WINDOWMGR->IsMouseOverUsed() )
		{
			
			int mx = g_UserInput.GetMouse()->GetMouseX();
			int my = g_UserInput.GetMouse()->GetMouseY();
			
			if( PtInWindow( mx, my ) == TRUE )
			{
				
				if( dwRemainTime )
				{
					if( m_dwLastSecond != dwRemainTime/1000+1 )
					{
						m_dwLastSecond = dwRemainTime/1000+1;
						
						//모든 무공을 refresh할 필요는 없다.
						MUGONGMGR->RefreshMugong();
						
						m_bRemainTime = TRUE;
					}
				}
				else if( m_bRemainTime == TRUE )
				{
					m_bRemainTime = FALSE;
					m_dwLastSecond = 0;
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}
*/
	}
	if(IsDepend())
	{
		BYTE Sung = GetSung();
		if(Sung < 12 && Sung > 0)
		{
			m_Guage.SetAbsXY((LONG)(m_absPos.x-1),(LONG)(m_absPos.y+39));
			m_Guage.Render();
		}
	}
}

void CMugongBase::SetExpPoint(DWORD point)
{
	m_MugongBaseInfo.ExpPoint = point;

	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(GetItemIdx());	
	if(pSkillInfo == NULL)
		return;

	BYTE Sung = GetSung();
	if(Sung != 12 && Sung != 0)
	{
		GUAGEVAL gval = point / (GUAGEVAL)pSkillInfo->GetNeedExp(Sung);
		m_Guage.SetValue(gval);
	}
}
