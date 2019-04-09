// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)
#define AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef WORD TILEATTR;

struct TILE_ATTR
{
	TILEATTR uAttr : 8;			//  0 ~ 255
	TILEATTR uSkillObjectBlock : 8;		//	0 ~ 255
};

class CTile  
{
	TILE_ATTR m_Attr;
public:
	CTile();
	~CTile();

	void AddTileAttrib(AREATILE areatile);
	void RemoveTileAttrib(AREATILE areatile);

	void SetTileAttrib(TILE_ATTR Attr);
	inline BOOL IsCollisonTile()
	{
		return m_Attr.uAttr != 0 || m_Attr.uSkillObjectBlock != 0;
	}
};

#endif // !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)
