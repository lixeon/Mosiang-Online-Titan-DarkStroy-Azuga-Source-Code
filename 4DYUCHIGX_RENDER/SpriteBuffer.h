#ifndef SPRITE_BUFFER_H
#define SPRITE_BUFFER_H

#include "VariableBuffer.h"
#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"

enum FONT_RENDER_TYPE
{
	ENABLE_TRANS		= 0x00000001,
	ENABLE_ROT			= 0x00000002,
	ENABLE_SCALE		= 0x00000004,
	ENABLE_PART_RENDER	= 0x00000008
};

struct SPRITE_ITEM
{
	IDISpriteObject*	pSprite;
	VECTOR2				v2Scaling;
	float				fRot;
	VECTOR2				v2Trans;
	RECT				rect;
	DWORD				dwColor;
	DWORD				dwFlag;
	SPRITE_ITEM*		pNextItem;
};


class CSpriteBuffer
{
	DWORD				m_dwCurrentSpriteNum;
	CVariableBuffer		m_Buffer;
	
	
public:
	BOOL				Initialize(DWORD dwMaxSpriteNum,DWORD dwMaxZOrderNum);
	BOOL				PushSpriteItem(DPCQ_HANDLE pDPCQ,IDISpriteObject* pSprite,VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,int iZOrder,DWORD dwFlag);
	void				Process();
	BOOL				Render(SPRITE_ITEM* pSpriteItem);
	void				Clear() {m_Buffer.Clear();}

	CSpriteBuffer();
	~CSpriteBuffer();


};

#endif
