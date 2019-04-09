#include "stdafx.h"
#include "KeySettingTipDlg.h"

CKeySettingTipDlg::CKeySettingTipDlg(void)
{
	m_wMode = 2;
}

CKeySettingTipDlg::~CKeySettingTipDlg(void)
{
}

void CKeySettingTipDlg::Linking()
{
	RECT rt;
	rt.top = rt.left = 0;
	rt.right = 1024;
	rt.bottom = 768;

//	m_KeyImage[0].LoadSprite("Image/KeySetting1.tga");
	m_KeyImage[0].LoadSprite("Image/2D/KeySetting1.tga");
	m_KeyImage[0].SetImageSrcRect(&rt);
//	m_KeyImage[1].LoadSprite("Image/KeySetting2.tga");
	m_KeyImage[1].LoadSprite("Image/2D/KeySetting2.tga");
	m_KeyImage[1].SetImageSrcRect(&rt);
}

void CKeySettingTipDlg::Render()
{
	if( !m_bActive ) return;
	if(m_wMode > 1) return;

	cDialog::RenderWindow();

	DWORD color = RGBA_MAKE(255,255,255,255);
	VECTOR2 vScale = { 1.f, 768.f/1024.f };

	m_KeyImage[m_wMode].RenderSprite( &vScale, NULL, 0, &m_absPos, color );

	cDialog::RenderComponent();
}