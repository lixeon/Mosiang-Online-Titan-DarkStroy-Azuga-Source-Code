#include "SpriteBuffer.h"

BOOL __stdcall RenderSpriteItem(CSpriteBuffer* pSpriteBuffer,SPRITE_ITEM* pSpriteItem)
{
	return pSpriteBuffer->Render(pSpriteItem);
}

CSpriteBuffer::CSpriteBuffer()
{
	memset(this,0,sizeof(CSpriteBuffer));
}
BOOL CSpriteBuffer::Initialize(DWORD dwMaxSpriteNum,DWORD dwMaxZOrderNum)
{
	return m_Buffer.Initialize(sizeof(SPRITE_ITEM)*dwMaxSpriteNum);
}
BOOL CSpriteBuffer::PushSpriteItem(DPCQ_HANDLE pDPCQ,IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	SPRITE_ITEM* pItemBuffer = (SPRITE_ITEM*)m_Buffer.AllocBuffer(sizeof(SPRITE_ITEM));
	
	if (!pItemBuffer)
		goto lb_return;


/*
	IDISpriteObject*	pSPrite;
	VECTOR2				v2Scaling;
	VECTOR2				v2RotCenter;
	float				fRot;
	VECTOR2				v2Trans;
	DWORD				dwColor;
	DWORD				dwFlag;
*/
	pItemBuffer->pSprite = pSprite;
	pItemBuffer->dwColor = dwColor;
	pItemBuffer->dwFlag = dwFlag;
	pItemBuffer->pNextItem = NULL;
	pItemBuffer->fRot = fRot;
	

	if (pv2Scaling)
	{
		pItemBuffer->dwFlag |= ENABLE_SCALE;
		pItemBuffer->v2Scaling = *pv2Scaling;
	}
	if (pRect)
	{
		pItemBuffer->dwFlag |= ENABLE_PART_RENDER;
		pItemBuffer->rect = *pRect;
	}
	if (pv2Trans)
	{
		pItemBuffer->dwFlag |= ENABLE_TRANS;
		pItemBuffer->v2Trans = *pv2Trans;
	}

	DWORD	dwArgList[2];
	dwArgList[0] = (DWORD)this;
	dwArgList[1] = (DWORD)pItemBuffer;

	DPCQPushDPC(pDPCQ,RenderSpriteItem,2,dwArgList,NULL,iZOrder);

	m_dwCurrentSpriteNum++;
	bResult = TRUE;

lb_return:
	return bResult;

}

BOOL CSpriteBuffer::Render(SPRITE_ITEM* pSpriteItem)
{

	VECTOR2*		pv2Scaling;
	VECTOR2*		pv2Trans;
	RECT*			pRect;

	
	pv2Scaling = NULL;
	pv2Trans = NULL;
	pRect = NULL;

	if (pSpriteItem->dwFlag & ENABLE_TRANS)
		pv2Trans = &pSpriteItem->v2Trans;

	if (pSpriteItem->dwFlag & ENABLE_SCALE)
		pv2Scaling = &pSpriteItem->v2Scaling;

	if (pSpriteItem->dwFlag & ENABLE_PART_RENDER)
		pRect = &pSpriteItem->rect;
	

	return pSpriteItem->pSprite->Draw(
		pv2Scaling,
		pSpriteItem->fRot,
		pv2Trans,
		pRect,
		pSpriteItem->dwColor,
		pSpriteItem->dwFlag);

}
CSpriteBuffer::~CSpriteBuffer()
{

}
