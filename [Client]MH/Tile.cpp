// Tile.cpp: implementation of the CTile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CTile::AddTileAttrib(AREATILE areatile)
{
	if(areatile == SKILLAREA_ATTR_BLOCK)
	{
		++m_Attr.uSkillObjectBlock;
	}
}

void CTile::RemoveTileAttrib(AREATILE areatile)
{
	if(areatile == SKILLAREA_ATTR_BLOCK)
	{
		ASSERT(m_Attr.uSkillObjectBlock > 0);
		if(m_Attr.uSkillObjectBlock != 0)
			--m_Attr.uSkillObjectBlock;
	}
}