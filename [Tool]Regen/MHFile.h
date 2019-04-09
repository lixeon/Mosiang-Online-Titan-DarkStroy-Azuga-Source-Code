// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
#define AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4dyuchiGRX_common/IFileStorage.h"

#define MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR		0x00000001
#define MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT	0x00000010

/*
struct MHFILE_HEADER
{
	DWORD	Version;	// version
	DWORD	Type;		// file Á¾·ù
	DWORD	FileSize;	// file size
	DWORD	Etc1;		// ¿¹ºñ data
	DWORD	Etc2;
};
*/
struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file Á¾·ù
	DWORD	FileSize;	// data size
};

enum{
	MHFILE_NORMALMODE,
	MHFILE_PACKEDFILE,
	MHFILE_ENGINEPACKEDFILE,
};
class line_node;

class CMHFile  
{
	FILE* fp;
public:
	CMHFile();
	virtual ~CMHFile();

	BOOL Init(char* filename,char* mode,DWORD dwFlag = 0);
	void Release();

	char* GetStringInQuotation();
	void GetStringInQuotation(char* pBuf);
	char* GetString();
	int GetString(char* pBuf);
	void GetLine(char * pBuf, int len);
	void GetLineX(char * pBuf, int len);	// ÁÖ¼®Ã³¸® µÈ °÷ ÀÐÀ»¶§ »ç¿ë
	int GetInt();
	LONG GetLong();
	float GetFloat();
	DWORD GetDword();
	WORD GetWord();
	BYTE GetByte();
	BOOL GetBool();
	BOOL GetHex(DWORD* pOut);
	BOOL GetExpPoint();
	BOOL GetLevel();
	
	void GetFloat(float* pArray,int count);
	void GetWord(WORD* pArray,int count);
	void GetDword(DWORD* pArray,int count);
		
	int GetWord(WORD* pOut);

	BOOL Seek(int n);
	

	BOOL IsEOF();

	BOOL IsInited();
	
	// LBS 03.10.22 ÇÑ¹®ÀÚ¾¿ °Ë»çÇÏ±â À§ÇØ¼­..
	int GetChar();

// jsd - binary file ÀÐ±â À§ÇØ
protected:
	// yh
	DWORD			m_bReadMode;
	
	// ¿£ÁøÆÑÅ·¿ë
	I4DyuchiFileStorage* m_pFileStorage;
	void* m_pFilePointer;

	int				m_Dfp;
	MHFILE_HEADER	m_Header;				// ÆÄÀÏ Çì´õ
	char			m_crc1;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char			m_crc2;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char*			m_pData;				// ½ÇÁ¦ µ¥ÀÌÅÍ
	BOOL			m_bInit;
	char			m_sFileName[MAX_PATH];

public:
	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetFileName()	{ return m_sFileName; }
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.FileSize; }
//
};

void _parsingKeywordString(char * inString, char ** outString);
void _parsingKeywordStringEx(char * inString, line_node ** Top);

#endif // !defined(AFX_MHFILE_H__CF584ED8_B10C_4E1B_A1CF_4E098D34E802__INCLUDED_)
