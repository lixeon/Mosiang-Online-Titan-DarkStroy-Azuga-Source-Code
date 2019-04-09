#include "ctexture.h"
#include "texture_manager.h"

CTexture::CTexture()
{
	memset(this,0,sizeof(CTexture));
}
	
void CTexture::SetTextureInfo(CBaseTextureContainer* pContainer)
{

	m_pTexContainer = pContainer;
	pContainer->AddRef();
	if (pContainer)
	{
		dwSuccessMask = 0xffffffff;
		m_dwTexFlag = pContainer->GetTexFlag();
	}
}
LPDIRECT3DTEXTURE8 CTexture::GetTexture()
{
	LPDIRECT3DTEXTURE8	pTex = NULL;
	
	if (!m_pTexContainer)
		goto lb_return;

	pTex = (LPDIRECT3DTEXTURE8)( dwSuccessMask & (DWORD)(m_pTexContainer->GetTexture()) );
lb_return:
	return pTex;
	
}
void CTexture::SetTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex)
{
	if (m_pTexContainer)
	{
		m_pTexContainer->SetTexture(pDevice,dwTexStageIndex);
	}
}

void CTexture::Release()
{
	if (m_pTexContainer)
	{
		m_pTexContainer->Release();
		m_pTexContainer = NULL;
		dwSuccessMask = 0x00000000;
	}
}
CTexture::~CTexture()
{
	Release();
}


