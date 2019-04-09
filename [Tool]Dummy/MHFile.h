// MHFile.h: interface for the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHFILE_H__EE0422A0_04C3_44FD_ABC4_4542D92E9D58__INCLUDED_)
#define AFX_MHFILE_H__EE0422A0_04C3_44FD_ABC4_4542D92E9D58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMHFile  
{
protected:
	FILE*	fp;

public:
	CMHFile();
	virtual ~CMHFile();

	BOOL	IsInited()							{ return fp ? TRUE : FALSE;	}

	BOOL	Init( char* filename, char* mode );
	void	Release();

	char*	GetStringInQuotation();
	void	GetStringInQuotation( char* pBuf );
	char*	GetString();
	void	GetString( char* pBuf )				{ fscanf( fp, "%s", pBuf ); }
	void	GetLine( char * pBuf, int len )		{ fgets( pBuf, len, fp ); }
	int		GetInt()							{ return atoi(GetString()); }
	LONG	GetLong()							{ return (LONG)atoi(GetString()); }	
	float	GetFloat()							{ return (float)atof(GetString()); }
	DWORD	GetDword()							{ return (DWORD)atof(GetString()); }
	WORD	GetWord()							{ return (WORD)atof(GetString()); }
	BYTE	GetByte()							{ return (BYTE)atof(GetString()); }
	BOOL	GetBool()							{ return (BOOL)atof(GetString()); }
	
	BOOL	Seek( int n )						{ return fseek( fp, n, SEEK_CUR ); }
	BOOL	IsEOF()								{ return feof( fp ); }
	
	void	GetFloat(float* pArray,int count);
	void	GetWord(WORD* pArray,int count);
	void	GetDword(DWORD* pArray,int count);	

};

#endif // !defined(AFX_MHFILE_H__EE0422A0_04C3_44FD_ABC4_4542D92E9D58__INCLUDED_)
