// TileSetEntry.h: interface for the CTileSetEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESETENTRY_H__FB6A584F_7F14_43FE_B6CA_6FDFCA477C43__INCLUDED_)
#define AFX_TILESETENTRY_H__FB6A584F_7F14_43FE_B6CA_6FDFCA477C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSetPiece.h"

#define MAX_PIECE_PER_ENTRY	4

class CTileSetEntry  
{
	CTileSetPiece m_PieceArray[MAX_PIECE_PER_ENTRY];
public:
	CTileSetEntry();
	virtual ~CTileSetEntry();

	void SetPiece(DWORD PieceNum,TILETEXTURE* pTexture,DWORD Probability);
	CTileSetPiece* GetRandomPiece();
	CTileSetPiece* GetPiece(DWORD PieceNum);
};

#endif // !defined(AFX_TILESETENTRY_H__FB6A584F_7F14_43FE_B6CA_6FDFCA477C43__INCLUDED_)
