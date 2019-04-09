// TileSetPiece.h: interface for the CTileSetPiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESETPIECE_H__189EDE53_D6F5_46ED_B718_590AAE0368CD__INCLUDED_)
#define AFX_TILESETPIECE_H__189EDE53_D6F5_46ED_B718_590AAE0368CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TILEBIT_NONE 0x00
#define TILEBIT_LU	0x01
#define TILEBIT_RU	0x02
#define TILEBIT_LD	0x04
#define TILEBIT_RD	0x08
#define TILEBIT_ALL (TILEBIT_LU|TILEBIT_RU|TILEBIT_LD|TILEBIT_RD)


//trustpak
const int c_iEntryTable[] =
{
	TILEBIT_NONE,
	TILEBIT_RD,
	TILEBIT_LD|TILEBIT_RD,
	TILEBIT_LD,
	TILEBIT_RU|TILEBIT_RD,
	TILEBIT_ALL,
	TILEBIT_LU|TILEBIT_LD,
	TILEBIT_RU,
	TILEBIT_LU|TILEBIT_RU,
	TILEBIT_LU,
	TILEBIT_ALL^TILEBIT_LU,
	TILEBIT_ALL^TILEBIT_RU,
	TILEBIT_ALL^TILEBIT_LD,
	TILEBIT_ALL^TILEBIT_RD
};

///



struct TILETEXTURE 
{
	TILETEXTURE()
	{
		Set(NULL);
	}
	DWORD TextureNum;
	DWORD Direction;
	void Set(TILETEXTURE* pTexture)
	{
		if(pTexture == NULL)
			TextureNum = Direction = 0;
		else
		{
			TextureNum = pTexture->TextureNum;
			Direction = pTexture->Direction;
		}
	}
	BOOL IsSameTexture(TILETEXTURE& texture)
	{
		if( this->TextureNum == texture.TextureNum )
			return TRUE;
		else
			return FALSE;
	}
	WORD GetTileIndex()
	{
		WORD wDirFlag = (WORD)Direction;
		WORD wTileIndex = (WORD)TextureNum;
		return (WORD)( (wTileIndex & 0x3fff) | (wDirFlag<<14) );
	}
	void SetTileIndex(WORD wTileIndex)
	{
		Direction = ((wTileIndex & 0xc000)>>14);
		TextureNum = (WORD)(wTileIndex & 0x3fff);
	}
};

class CTileSetPiece  
{
	TILETEXTURE m_Texture;

	DWORD m_Probability;
public:
	CTileSetPiece();
	virtual ~CTileSetPiece();

	void SetTexturePiece(TILETEXTURE* pTexture,DWORD Probability);

	inline TILETEXTURE& GetTexture()	{	return m_Texture;	}
	inline DWORD GetProbability()	{	return m_Probability;	}
};

#endif // !defined(AFX_TILESETPIECE_H__189EDE53_D6F5_46ED_B718_590AAE0368CD__INCLUDED_)
