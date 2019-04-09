// TileSetPiece.cpp: implementation of the CTileSetPiece class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileSet.h"
#include "TileSetPiece.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileSetPiece::CTileSetPiece()
{
	m_Probability = 0;
}

CTileSetPiece::~CTileSetPiece()
{

}

void CTileSetPiece::SetTexturePiece(TILETEXTURE* pTexture,DWORD Probability)
{
	m_Texture.Set(pTexture);
	m_Probability = Probability;
}