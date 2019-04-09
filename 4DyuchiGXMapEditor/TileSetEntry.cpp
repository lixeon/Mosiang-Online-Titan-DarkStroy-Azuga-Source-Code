// TileSetEntry.cpp: implementation of the CTileSetEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileSet.h"
#include "TileSetEntry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileSetEntry::CTileSetEntry()
{

}

CTileSetEntry::~CTileSetEntry()
{

}

void CTileSetEntry::SetPiece(DWORD PieceNum,TILETEXTURE* pTexture,DWORD Probability)
{
	ASSERT(PieceNum<MAX_PIECE_PER_ENTRY);
	m_PieceArray[PieceNum].SetTexturePiece(pTexture,Probability);	
}

CTileSetPiece* CTileSetEntry::GetPiece(DWORD PieceNum)
{
	return &m_PieceArray[PieceNum];
}

CTileSetPiece* CTileSetEntry::GetRandomPiece()
{
	DWORD ProbabilityMin[MAX_PIECE_PER_ENTRY];
	DWORD ProbabilityMax[MAX_PIECE_PER_ENTRY];
	
	ProbabilityMin[0] = 0;
	ProbabilityMax[0] = m_PieceArray[0].GetProbability();
	for(int n=1;n<MAX_PIECE_PER_ENTRY;++n)
	{
		ProbabilityMin[n] = ProbabilityMax[n-1];
		ProbabilityMax[n] = ProbabilityMin[n] + m_PieceArray[n].GetProbability();
	}

	DWORD TotalVal = ProbabilityMax[MAX_PIECE_PER_ENTRY-1];
	if(TotalVal == 0)
		return NULL;
	
	DWORD randVal = rand()%TotalVal;
	for(n=0;n<MAX_PIECE_PER_ENTRY;++n)
	{
		if( randVal >= ProbabilityMin[n] &&
			randVal < ProbabilityMax[n] )
		{
			return &m_PieceArray[n];
		}
	}

	return NULL;
}