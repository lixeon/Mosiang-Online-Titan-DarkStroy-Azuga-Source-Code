// TileSet1.h: interface for the CTileSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESET1_H__5739F4D1_1062_49B5_AC1D_318BF127A2C8__INCLUDED_)
#define AFX_TILESET1_H__5739F4D1_1062_49B5_AC1D_318BF127A2C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TileSetEntry.h"
#include "../4DyuchiGRX_Common/IRenderer.h"

class CTileSet  
{
	CString m_Name;
	CTileSetEntry m_EntryArray[16];
public:
	CTileSet();
	virtual ~CTileSet();

	CTileSetPiece* GetCenterPiece(DWORD PieceNum);
	CTileSetPiece* GetRandomPiece(BYTE Bits);

	inline CString& GetName()	{	return m_Name;	}
	inline void SetName(CString& str)	{	m_Name = str;	}
	
	void CopyFrom(CTileSet* pSet);

	void SetPiece(DWORD EntryNum,DWORD PieceNum,TILETEXTURE* pTexture,DWORD Probability);
	inline void ClearPiece(DWORD EntryNum,DWORD PieceNum)
	{
		SetPiece(EntryNum,PieceNum,0,0);
	}

	inline CTileSetEntry* GetEntry(DWORD EntryNum)
	{
		return &m_EntryArray[EntryNum];
	}

	void SetEntry(DWORD EntryNum,CTileSetEntry* pEntry);

	void Save(FILE* fp);
	void Load(FILE* fp);	
};

#endif // !defined(AFX_TILESET1_H__5739F4D1_1062_49B5_AC1D_318BF127A2C8__INCLUDED_)
