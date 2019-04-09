// Tile.cpp: implementation of the CTile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTile::CTile() 
{
	m_Attr.uCharPreoccupied = 0;
}

CTile::~CTile()
{
}


void CTile::InitTileAttrib(TILE_ATTR Attr)
{
//	m_Attr.uFixedAttr = Attr.uFixedAttr;
	m_Attr.uCharPreoccupied = 0;
}



void CTile::AddTileAttrib(AREATILE areatile)
{
	if(areatile == SKILLAREA_ATTR_BLOCK)
	{
		++m_Attr.uMuCollisonNum;
	}
}

void CTile::RemoveTileAttrib(AREATILE areatile)
{
	if(areatile == SKILLAREA_ATTR_BLOCK)
	{
		//ASSERT(m_Attr.uMuCollisonNum > 0);
		--m_Attr.uMuCollisonNum;
	}
}