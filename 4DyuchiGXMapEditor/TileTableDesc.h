#pragma once

#define		MAX_SECTION_LINK_MANY	20

#pragma pack( push, 1 )
// Section Link...
struct Section_Link_Info
{
	WORD	wSectionNum;
	WORD	x1, y1, x2, y2;

	WORD	pwSectionLinkNum[MAX_SECTION_LINK_MANY];

	Section_Link_Info()
	{
		memset( this, 0, sizeof( Section_Link_Info ) );
	}
};

// Tile table desc struct
struct TileTableDesc
{
	DWORD	dwTileManyX;
	DWORD	dwTileManyZ;
	DWORD	dwFaceSize;
	DWORD	dwFacesNumPerObjAxis;

	WORD	wSectionMany;
	Section_Link_Info*	pSectionLinkInfo;

	TileTableDesc()
	{
		memset( this, 0, sizeof( TileTableDesc ) );
	}

	~TileTableDesc()
	{
		if( pSectionLinkInfo )
		{
			delete[] pSectionLinkInfo;
			pSectionLinkInfo = NULL;
		}
	}
};

#pragma pack( pop )