// TileSetApplyer.h: interface for the CTileSetApplyer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESETAPPLYER_H__2EDEFE5C_B343_43E0_BC56_F85BCEAF6B2F__INCLUDED_)
#define AFX_TILESETAPPLYER_H__2EDEFE5C_B343_43E0_BC56_F85BCEAF6B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSet.h"

class CTileSetApplyer  
{
	struct TILEBITARRAY
	{
		TILEBITARRAY()	{	memset(Bits,0,25);		}
		BYTE Bits[25];
		void PutCenterTile(int n);
	protected:
		void AddBits(int x,int y,BYTE Bit);
	};
public:
	CTileSetApplyer();
	virtual ~CTileSetApplyer();

	void Apply(CTileSet* pTileSet,TILETEXTURE TileTextureSrc[25],TILETEXTURE* TileTextureOut);
};

#endif // !defined(AFX_TILESETAPPLYER_H__2EDEFE5C_B343_43E0_BC56_F85BCEAF6B2F__INCLUDED_)
