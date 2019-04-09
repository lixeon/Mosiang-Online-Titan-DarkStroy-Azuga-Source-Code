// FileBin.h: interface for the CFileBin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEBIN_H__DAE3F2E0_F457_45C6_A8AB_F1CE45BD8168__INCLUDED_)
#define AFX_FILEBIN_H__DAE3F2E0_F457_45C6_A8AB_F1CE45BD8168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct MHFILE_HEADER
{
	DWORD	dwVersion;	// version
	DWORD	dwType;		// file Á¾·ù
	DWORD	FileSize;	// data size
};

class CFileBin  
{
protected:
	DWORD			m_Dfp;
	MHFILE_HEADER	m_Header;				// ÆÄÀÏ Çì´õ
	char			m_crc1;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char			m_crc2;					// ¾ÏÈ£È­ °Ë»ç µ¥ÀÌÅÍ
	char*			m_pData;				// ½ÇÁ¦ µ¥ÀÌÅÍ
	BOOL			m_bInit;
	char			m_sFileName[MAX_PATH];

public:
	CFileBin();
	virtual ~CFileBin();

	BOOL	OpenBin( char* filename );
	void	Clear();
	BOOL	CheckHeader();
	BOOL	CheckCRC();
	char*	GetFileName()	{ return m_sFileName; }
	char*	GetData()		{ return m_pData; }
	DWORD	GetDataSize()	{ return m_Header.FileSize; }

	char*	GetString();
	void	GetString( char* pBuf );
	char*	GetStringInQuotation();
	void	GetStringInQuotation( char* pBuf );
	void	GetLine( char * pBuf, int len );
	void	GetLineX( char * pBuf, int len );	// ÁÖ¼®Ã³¸® µÈ °÷ ÀÐÀ»¶§ »ç¿ë
	int		GetInt()		{ return atoi(GetString()); }
	LONG	GetLong()		{ return (LONG)atoi(GetString()); }
	float	GetFloat()		{ return (float)atoi(GetString()); }
	DWORD	GetDword()		{ return (DWORD)atoi(GetString()); }
	WORD	GetWord()		{ return (WORD)atoi(GetString()); }
	BYTE	GetByte()		{ return (BYTE)atoi(GetString()); }
	BOOL	GetBool()		{ return (BOOL)atoi(GetString()); }
	BOOL	IsEOF();
	BOOL	IsInited()		{ return m_bInit ? TRUE : FALSE; }
};

void _parsingKeywordString(char * inString, char ** outString);

#endif // !defined(AFX_FILEBIN_H__DAE3F2E0_F457_45C6_A8AB_F1CE45BD8168__INCLUDED_)
