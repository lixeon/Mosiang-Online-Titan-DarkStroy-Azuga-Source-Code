// TileSet1.cpp: implementation of the CTileSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileSet::CTileSet()
{	
}

CTileSet::~CTileSet()
{

}

CTileSetPiece* CTileSet::GetCenterPiece(DWORD PieceNum)
{
	return m_EntryArray[TILEBIT_ALL].GetPiece(PieceNum);
}

CTileSetPiece* CTileSet::GetRandomPiece(BYTE Bits)
{
	CTileSetPiece* pPiece = m_EntryArray[Bits].GetRandomPiece();
//	ASSERT(pPiece);
	return pPiece;
}

void CTileSet::SetPiece(DWORD EntryNum,DWORD PieceNum,TILETEXTURE* pTexture,DWORD Probability)
{
	ASSERT(EntryNum<16);
	m_EntryArray[EntryNum].SetPiece(PieceNum,pTexture,Probability);
}

void CTileSet::CopyFrom(CTileSet* pSet)
{
	SetName(pSet->GetName());
	memcpy(m_EntryArray,pSet->m_EntryArray,sizeof(CTileSetEntry)*16);
}

void CTileSet::SetEntry(DWORD EntryNum,CTileSetEntry* pEntry)
{
	memcpy(&m_EntryArray[EntryNum],pEntry,sizeof(CTileSetEntry));
}

void CTileSet::Save(FILE* fp)
{
	fprintf(fp,GetName());
	fprintf(fp,"\n");

	for(int entry=0;entry<16;++entry)
	{
		CTileSetEntry* pEntry = GetEntry(entry);
		for(int piece=0;piece<4;++piece)
		{
			CTileSetPiece* pPiece = pEntry->GetPiece(piece);
			fprintf(fp,"%d %d %d ",
				pPiece->GetTexture().TextureNum,
				pPiece->GetTexture().Direction,
				pPiece->GetProbability());
		}
		fprintf(fp,"\n");
	}
}
void CTileSet::Load(FILE* fp)
{
	char buf[256];
	fgets(buf,256,fp);
	buf[strlen(buf)-1] = 0;
	CString name = buf;
	SetName(name);

	for(int entry=0;entry<16;++entry)
	{
		CTileSetEntry* pEntry = GetEntry(entry);
		for(int piece=0;piece<4;++piece)
		{
			TILETEXTURE tex;
			DWORD Prob;
			fscanf(fp,"%d %d %d ",&tex.TextureNum,&tex.Direction,&Prob);
			pEntry->SetPiece(piece,&tex,Prob);
		}
	}
}