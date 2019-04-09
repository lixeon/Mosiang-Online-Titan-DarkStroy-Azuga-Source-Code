// TileSetApplyer.cpp: implementation of the CTileSetApplyer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileSet.h"
#include "TileSetApplyer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CTileSetApplyer::TILEBITARRAY::PutCenterTile(int n)
{
	ASSERT(n<25);
	int x = n%5;
	int y = n/5;
	
	AddBits(x-1	,	y-1	,TILEBIT_RD);
	AddBits(x	,	y-1	,TILEBIT_LD|TILEBIT_RD);
	AddBits(x+1	,	y-1	,TILEBIT_LD);
	AddBits(x-1	,	y	,TILEBIT_RU|TILEBIT_RD);
	AddBits(x	,	y	,TILEBIT_ALL);
	AddBits(x+1	,	y	,TILEBIT_LU|TILEBIT_LD);
	AddBits(x-1	,	y+1	,TILEBIT_RU);
	AddBits(x	,	y+1	,TILEBIT_LU|TILEBIT_RU);
	AddBits(x+1	,	y+1	,TILEBIT_LU);
}

void CTileSetApplyer::TILEBITARRAY::AddBits(int x,int y,BYTE Bit)
{
	if(x<0 || y<0 || x>=5 || y>=5)
		return;
	
	Bits[y*5+x] |= Bit;
}

CTileSetApplyer::CTileSetApplyer()
{

}

CTileSetApplyer::~CTileSetApplyer()
{

}

void CTileSetApplyer::Apply(CTileSet* pTileSet,TILETEXTURE TileTextureSrc[25],TILETEXTURE* TileTextureOut)
{
	TILEBITARRAY TileBits;

	for(int n=0;n<25;++n)
	{
		BOOL bCenterTile = FALSE;
		for(int piece=0;piece<4;++piece)
		{
			CTileSetPiece* pPiece = pTileSet->GetCenterPiece(piece);
			if(pPiece->GetProbability() == 0)
				continue;

			bCenterTile |= TileTextureSrc[n].IsSameTexture(pPiece->GetTexture());
		}

		if(bCenterTile)
		{
			TileBits.PutCenterTile(n);
		}
	}

	for(int y=0;y<3;++y)
	{
		for(int x=0;x<3;++x)
		{
			CTileSetPiece* pPiece = pTileSet->GetRandomPiece(TileBits.Bits[(y+1)*5+x+1]);
			if(pPiece)
				TileTextureOut[y*3+x] = pPiece->GetTexture();
		}
	}
}