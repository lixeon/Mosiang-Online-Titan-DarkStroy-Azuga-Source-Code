// FileBin.cpp: implementation of the CFileBin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerTool.h"
#include "FileBin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileBin::CFileBin()
{
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;
	memset( m_sFileName, 0, MAX_PATH );
}

CFileBin::~CFileBin()
{
	Clear();
}

void CFileBin::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	m_Dfp = 0;
	m_bInit = FALSE;
}

BOOL CFileBin::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	FILE* fp = fopen( filename, "rb" );
	if( fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, fp );	// header
		fread( &m_crc1, sizeof(char), 1, fp );			// crc1
		if( m_Header.FileSize )							// data
		{
			m_pData = new char[m_Header.FileSize];
			fread( m_pData, sizeof(char), m_Header.FileSize, fp );
		}
		fread( &m_crc2, sizeof(char), 1, fp );			// crc2
	}	
	else
	{
		return FALSE;
	}
	fclose( fp );
	fp = NULL;

	if( !CheckHeader() )	return FALSE;
	if( !CheckCRC() )		return FALSE;

	m_bInit = TRUE;

	return TRUE;
}

BOOL CFileBin::CheckHeader()
{
	if( m_Header.dwVersion != (DWORD)(20040308+m_Header.dwType+m_Header.FileSize) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Type Error!!", "Error!!", MB_OK );
		return FALSE;
	}	

	return TRUE;
}

BOOL CFileBin::CheckCRC()
{
	if( m_crc1 != m_crc2 )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	// decode
	char crc = (char)m_Header.dwType;
	for( DWORD i = 0; i < m_Header.FileSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= (char)i;
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= (char)m_Header.dwType;
	}

	if( m_crc1 != crc )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	return TRUE;
}

char* CFileBin::GetString()
{
	static char buf[256];
	GetString( buf );
	return buf;
}

void CFileBin::GetString( char* pBuf )
{
	char buf[256] = {0,};
	DWORD j = 0;
	BOOL bStart, bEnd;
	bStart = bEnd = FALSE;	
	
	if( m_Dfp == m_Header.FileSize )
		return;
	
	for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
	{
		if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
		{
			if( bStart )
				break;
			else
			{
				m_Dfp += 2;
				++i;
			}
		}
		else if( m_pData[i] == 0x20 || m_pData[i] == 0x09 )	// spacebar, tab
		{
			m_Dfp++;
			if( bStart )
				bEnd = TRUE;
		}
		else
		{
			if( bEnd ) break;
			
			buf[j] = m_pData[i];
			j++;
			m_Dfp++;
			bStart = TRUE;
		}
	}
	buf[j] = 0;
	
	_parsingKeywordString( buf, (char**)(&pBuf) );
}

char* CFileBin::GetStringInQuotation()
{
	char buf[256]={0,};
	static char buf2[256];
	int c;

	while( (c=m_pData[m_Dfp]) != '"' )	// 첫따옴표 나올때까지 스킵
	{
		++m_Dfp;
		if( c == 10 )					//첫따옴표 나오기전에 엔터가 있으면 중지
		{
			buf2[0] = 0;
			return buf2;
		}
	}
	int n = 0;
	while( 1 )
	{
		++m_Dfp;
		buf[n] = m_pData[m_Dfp];
		if( buf[n] == '"' )
		{
			buf[n] = 0;
			++m_Dfp;
			break;
		}
		if( buf[n] == 10 )				// 엔터
		{
			buf[n] = 0;
			break;
		}
		++n;
	}
	char* aa = &buf2[0];
	memcpy( buf2, buf, n+1 );	

	return buf2;
}

void CFileBin::GetStringInQuotation( char* pBuf )
{
	strcpy( pBuf, GetStringInQuotation() );
}

void CFileBin::GetLine( char* pBuf, int len )
{
	char buf[256] = {0,};
	DWORD j = 0;
	
	for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
	{
		if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
		{
			m_Dfp += 2;
			break;
		}
		else
		{
			buf[j] = m_pData[i];
			j++;
			m_Dfp++;
		}
	}
	buf[j] = 0;

	_parsingKeywordString(buf, &pBuf);
}

void CFileBin::GetLineX( char * pBuf, int len )
{
	char buf[256] = {0,};
	DWORD j = 0;
	
	if( m_Dfp > 2 ) m_Dfp -= 2;
	for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
	{
		if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
		{
			m_Dfp += 2;
			break;
		}
		else
		{
			buf[j] = m_pData[i];
			j++;
			m_Dfp++;
		}
	}
	buf[j] = 0;	
	
	_parsingKeywordString(buf, &pBuf);
}

BOOL CFileBin::IsEOF()
{
	if( m_Header.FileSize == (DWORD)m_Dfp )
		return TRUE;

	return FALSE;
}

void _parsingKeywordString(char * inString, char ** outString)
{
	char * sp = inString;
	char * sp2 = *outString;
	while(*sp != 0)
	{
		if(*sp & 0x80)
		{
			*sp2++ = *sp++;
			*sp2++ = *sp++;
		}
		else
		{
			switch(*sp)
			{
			case TEXT_DELIMITER:
				{
					++sp;
					char tmp = *sp;
					switch(tmp)
					{
					case TEXT_SPACECHAR:
						{	
							*sp2 = ' ';
							++sp2;
						}
						break;
					case TEXT_EMPTYCHAR:
						{
							*outString[0] = 0;
						}
						break;
					}
				}
				break;
			default:
				{
					*sp2 = *sp;
					++sp2;
				}
			}
			++sp;
		}
	}
	*sp2 = 0;
}