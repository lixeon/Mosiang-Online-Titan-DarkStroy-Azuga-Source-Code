// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)
#define AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef WORD TILEATTR;
typedef WORD FIXEDATTR;

//FIXED TILE ATTRIB
#define TFA_FLYCOLLISON		0x00000010
#define TFA_PEACEZONE		0x00000100
#define TFA_JSAZONE			0x00001000

#define TMA_COLLISON		0x00000001
#define TMA_FIRSTEFFECT		0x00000010
#define TMA_CONTEFFECT		0x00000100
#define TMA_OBJECT			0x00001000

struct TILE_ATTR
{
	TILEATTR uCharPreoccupied : 8;	//	0 ~ 255
	TILEATTR uMuCollisonNum : 8;
};

class CTile  
{
	TILE_ATTR m_Attr;
public:
	CTile();
	~CTile();

	
	void AddTileAttrib(AREATILE areatile);
	void RemoveTileAttrib(AREATILE areatile);

	void InitTileAttrib(TILE_ATTR Attr);
/*	
	inline BOOL IsCollisonTile()
	{
		return m_Attr.uFixedAttr & TFA_COLLISON || m_Attr.uMuCollisonNum != 0;
	}
*/	
	inline void IncreasePreoccupied()
	{
		++m_Attr.uCharPreoccupied;
	}
	
	inline void DecreasePreoccupied()
	{
		--m_Attr.uCharPreoccupied;	
	}

	inline GetPreoccupied()	{	return m_Attr.uCharPreoccupied;	}
};

#endif // !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)
