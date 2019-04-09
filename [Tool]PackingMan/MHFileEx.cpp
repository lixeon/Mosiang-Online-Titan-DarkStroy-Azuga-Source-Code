#define __MHFILEEX_C__

#include "stdafx.h"
#include "MHFileEx.h"

CMHFileEx::CMHFileEx()
{
	m_fp = NULL;
	memset( m_szFullFileName, 0, 256 );
	memset( m_szFileName, 0, 256 );
	memset( m_szExt, 0, 256 );
	memset( &m_Header, 0, sizeof(MHFILE_HEADER) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;	
	m_pBinData = NULL;

	memset( &m_DOFHeader, 0, sizeof(DOF_HEADER) );
	memset( &m_DOFTail, 0, sizeof(DOF_TAIL) );

	strcpy( m_szDOF, "dof" );
}

CMHFileEx::~CMHFileEx()
{
	Clear();
}

void CMHFileEx::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	if( m_pBinData )
	{
		delete [] m_pBinData;
		m_pBinData = NULL;
	}
}

BOOL CMHFileEx::New( const char* fullfilename )
{
	Clear();
	m_pData = new char[MEGA];
	memset( m_pData, 0, MEGA );

	InitFileName( fullfilename );

	return TRUE;
}

BOOL CMHFileEx::OpenTxt( const char* fullfilename )
{
	InitFileName( fullfilename );

	//__asm int 3;
	m_fp = fopen( fullfilename, "rb" );
	if( m_fp )
	{
		Clear();
		m_pData = new char[MEGA];
		memset( m_pData, 0, MEGA );

		m_Header.dwDataSize = 0;
		while( !feof(m_fp) )
		{
			m_pData[m_Header.dwDataSize] = fgetc( m_fp );
			m_Header.dwDataSize++;
			if( m_Header.dwDataSize >= MEGA )
				break;
		}
		m_Header.dwDataSize--;
		m_pData[m_Header.dwDataSize] = 0;		
		
		m_DOFHeader.dwDataSize = m_Header.dwDataSize;

		fclose( m_fp );
	}

	return TRUE;
}
//#define  _MSO50_  //打开编译=解码50mso工具
#define  _RXMSO_    //打开编译=解码rxmso工具
BOOL CMHFileEx::OpenBin( const char* fullfilename )
{
	InitFileName( fullfilename );

	m_fp = fopen( fullfilename, "rb" );
	if( m_fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, m_fp );	// header
		fread( &m_crc1, sizeof(char), 1, m_fp );		// crc1
#ifdef _RXMSO_
		fread( &m_crc1, sizeof(char), 1, m_fp );		
#endif
#ifdef _MSO50_
		//50加密
		fread( &m_crc1, sizeof(char), 1, m_fp );	
		fread( &m_crc1, sizeof(char), 1, m_fp );	
		fread( &m_crc1, sizeof(char), 1, m_fp );	
		//50加密
#endif
		if( m_Header.dwDataSize )						// data
		{
			m_pData = new char[MEGA];
			memset( m_pData, 0, MEGA );
			fread( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
			m_pData[m_Header.dwDataSize] = 0;
		}
#ifdef _RXMSO_
	fread( &m_crc2, sizeof(char), 1, m_fp );	
#endif
#ifdef _MSO50_
		//50加密
		fread( &m_crc2, sizeof(char), 1, m_fp );	
		fread( &m_crc2, sizeof(char), 1, m_fp );	
		//50加密
#endif

		fread( &m_crc2, sizeof(char), 1, m_fp );		// crc2
	}	
	fclose( m_fp );

	if( !CheckHeader() )	return FALSE;
	if( !CheckCRC() )		return FALSE;

	m_DOFHeader.dwDataSize = m_Header.dwDataSize;

	return TRUE;
}

BOOL CMHFileEx::SaveToTxt( const char* filename )
{
	m_fp = fopen( filename, "wb" );
	if( m_fp )
	if( m_pData )
		fwrite( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
	fclose( m_fp );

	return TRUE;
}

BOOL CMHFileEx::SaveToTxt()
{
	MakeFullFileName();
	m_fp = fopen( m_szFullFileName, "wb" );
	if( m_fp )
	if( m_pData )
		fwrite( m_pData, sizeof(char), m_Header.dwDataSize, m_fp );
	fclose( m_fp );

	return TRUE;
}

BOOL CMHFileEx::SaveToBin( const char* filename )
{
	if( m_pData )
	{
		m_fp = fopen( filename, "wb" );
		if( m_fp )
		{
			ConvertBin();
			fwrite( &m_Header, sizeof(m_Header), 1, m_fp );

#ifdef _RXMSO_
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
#endif
#ifdef _MSO50_
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
#endif

			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( m_pBinData, sizeof( char ), m_Header.dwDataSize, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );

#ifdef _RXMSO_
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
#endif
#ifdef _MSO50_			
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
#endif
		}
		fclose( m_fp );
	}

	return TRUE;
}

BOOL CMHFileEx::SaveToBin()
{
	MakeFullFileName();
	return SaveToBin( m_szFullFileName );
}

BOOL CMHFileEx::CheckHeader()
{
	return TRUE;
	if( m_Header.dwVersion != (20040308+m_Header.dwType+m_Header.dwDataSize) )
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
	if( m_Header.dwDataSize > MEGA )
	{
		Clear();
		MessageBox( NULL, "Header DataSize Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	return TRUE;
}

BOOL CMHFileEx::CheckCRC()
{
	/*
	if( m_crc1 != m_crc2 )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	*/

	// decode
	char crc = m_Header.dwType;
	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= (char)i;
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= m_Header.dwType;
	}

	/*
	if( m_crc1 != crc )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	*/

	return TRUE;
}

BOOL CMHFileEx::ConvertBin()
{
	// encode
	m_Header.dwVersion = 20040308;
	//m_Header.dwVersion = 20070522;
	if( m_Header.dwDataSize == 0 )
		m_Header.dwType = 1;
	else
		m_Header.dwType = rand()%m_Header.dwDataSize + 1;
	m_Header.dwVersion += (m_Header.dwType+m_Header.dwDataSize);

	m_pBinData = new char[m_Header.dwDataSize+1];
	memcpy( m_pBinData, m_pData, m_Header.dwDataSize+1 );
	m_pBinData[m_Header.dwDataSize] = 0;

	char crc = m_Header.dwType;
	for( DWORD i = 0; i < m_Header.dwDataSize; ++i )
	{
		m_pBinData[i] += (char)i;		
		if( i%m_Header.dwType == 0 )
			m_pBinData[i] += m_Header.dwType;
		crc += m_pBinData[i];
	}

	m_crc1 = m_crc2 = crc;

	return TRUE;
}

void CMHFileEx::InitFileName( const char* fullfilename )
{
	strcpy( m_szFullFileName, fullfilename );

	int len = strlen(m_szFullFileName);
	int i, j, k;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
			j = i;
		if( m_szFullFileName[i] == '\\' )
		{
			k = i;
			break;
		}
	}

	for( i = 0; i < len-j; ++i )
		m_szExt[i] = m_szFullFileName[j+1+i];
	for( i = 0; i < len-k; ++i )
		m_szFileName[i] = m_szFullFileName[k+1+i];	
}

void CMHFileEx::MakeFullFileName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szExt);
	int i, j;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	for( i = 0; i < lenext; ++i )
		m_szFullFileName[j+1+i] = m_szExt[i];

	m_szFullFileName[j+1+i] = 0;
}

void CMHFileEx::SetData( char* str )
{
	if( m_pData )
	{
		memcpy( m_pData, str, MEGA );
		m_Header.dwDataSize = strlen( str );
	}
}

BOOL CMHFileEx::MakeDOFName()
{
	int len = strlen(m_szFullFileName);
	int lenext = strlen(m_szDOF);
	int i, j;

	for( i = len-1; i >= 0; --i )
	{
		if( m_szFullFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	for( i = 0; i < lenext; ++i )
		m_szFullFileName[j+1+i] = m_szDOF[i];

	m_szFullFileName[j+1+i] = 0;

	return TRUE;
}

BOOL CMHFileEx::SaveToDOF()
{
	MakeDOFName();
	return SaveToDOF( m_szFullFileName );
}

BOOL CMHFileEx::SaveToDOF( const char* filename )
{
	if( m_pData )
	{
		m_fp = fopen( filename, "wb" );
		if( m_fp )
		{
			ConvertDOF();
			fwrite( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );
			fwrite( &m_crc1, sizeof( char ), 1, m_fp );
			fwrite( m_pBinData, sizeof( char ), m_DOFHeader.dwDataSize, m_fp );
			fwrite( &m_DOFTail, sizeof( m_DOFTail ), 1, m_fp );
		}
		fclose( m_fp );
	}

	return TRUE;
}

BOOL CMHFileEx::ConvertDOF()
{
	// encode
	m_DOFHeader.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFTail.dwSeed = rand()%m_DOFHeader.dwDataSize;
	m_DOFHeader.dwVersion = (DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize;
	m_DOFTail.dwVersion = (DWORD)DOFTAIL + m_DOFTail.dwSeed;

	m_pBinData = new char[m_DOFHeader.dwDataSize];
	memcpy( m_pBinData, m_pData, m_DOFHeader.dwDataSize );
//	m_pBinData[m_DSOFHeader.dwDataSize] = 0;

	m_crc1 = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		m_pBinData[i] += ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
		m_crc1 += m_pBinData[i];
	}

	return TRUE;
}

BOOL CMHFileEx::OpenDOF( const char* fullfilename )
{
	InitFileName( fullfilename );

	m_fp = fopen( fullfilename, "rb" );
	if( m_fp )
	{
		Clear();
		fread( &m_DOFHeader, sizeof(m_DOFHeader), 1, m_fp );	// header
		fread( &m_crc1, sizeof(char), 1, m_fp );				// crc1
		if( m_DOFHeader.dwDataSize )							// data
		{
			m_pData = new char[MEGA];
			memset( m_pData, 0, MEGA );
			fread( m_pData, sizeof(char), m_DOFHeader.dwDataSize, m_fp );
			m_pData[m_DOFHeader.dwDataSize] = 0;
		}
		fread( &m_DOFTail, sizeof(m_DOFTail), 1, m_fp );		// tail
	}	
	fclose( m_fp );

	if( !CheckDOF() )	return FALSE;

	return TRUE;
}

BOOL CMHFileEx::CheckDOF()
{
	if( m_DOFHeader.dwVersion != ((DWORD)DOFHEADER + m_DOFHeader.dwSeed + m_DOFHeader.dwDataSize) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFHeader.dwSeed == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Seed Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFHeader.dwDataSize > MEGA )
	{
		Clear();
		MessageBox( NULL, "Header DataSize Error!!", "Error!!", MB_OK );
		return FALSE;
	}
	if( m_DOFTail.dwVersion != ((DWORD)DOFTAIL + m_DOFTail.dwSeed) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	// decode
	char crc = 0;
	for( DWORD i = 0; i < m_DOFHeader.dwDataSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= ((char)i + m_DOFHeader.dwSeed + m_DOFTail.dwSeed);
	}
	if( m_crc1 != crc )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}

	return TRUE;
}