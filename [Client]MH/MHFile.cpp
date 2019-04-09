// MHFile.cpp: implementation of the CMHFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHFile.h"
//#include "./interface/Line_Node.h"
//#include "./interface/cWindowFunc.h"
#include "Engine/GraphicEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMHFile::CMHFile()
{
	fp = NULL;
	memset( &m_Header, 0, sizeof(m_Header) );
	m_crc1 = m_crc2 = 0;
	m_pData = NULL;
	m_Dfp = 0;
	m_bInit = FALSE;
	memset( m_sFileName, 0, MAX_PATH );

	m_pFileStorage = 0;
	m_pFilePointer = 0;
}

CMHFile::~CMHFile()
{
	Release();
}
/*
BOOL CMHFile::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	fp = fopen( filename, "rb" );
	strcpy(m_sFileName,filename);
	if( fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, fp );
		fread( &m_crc1, sizeof(char), 1, fp );

		if( CheckHeader() )
		{
			if( m_Header.FileSize )
			{
				m_pData = new char[m_Header.FileSize];
				fread( m_pData, sizeof(char), m_Header.FileSize, fp );
			}
		}
		else
		{
			fclose( fp );
			fp = NULL;
			return FALSE;
		}
	}	
	else
	{
		return FALSE;
	}
	fclose( fp );
	fp = NULL;

	if( !CheckCRC() ) return FALSE;

	m_bInit = TRUE;

	return TRUE;
}

BOOL CMHFile::CheckHeader()
{
	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	char crc = 0;
	// data 변환
	for( DWORD i = 0; i < m_Header.FileSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= (char)i;		
	}

	if( m_crc1 != crc )
	{
		Clear();
		return FALSE;
	}

	return TRUE;
}
*/
#define  _RXMSO_
//#define  _MSO50_
BOOL CMHFile::OpenBin( char* filename )
{
	if( filename == NULL )
		return FALSE;

	fp = fopen( filename, "rb" );
	strcpy(m_sFileName,filename);
	if( fp )
	{
		Clear();
		fread( &m_Header, sizeof(m_Header), 1, fp );	// header
		fread( &m_crc1, sizeof(char), 1, fp );			// crc1

#ifdef _RXMSO_
		fread( &m_crc1, sizeof(char), 1, fp );		
#endif

#ifdef _MSO50_
		//50속쵱
		fread( &m_crc1, sizeof(char), 1, fp );	
		fread( &m_crc1, sizeof(char), 1, fp );	
		fread( &m_crc1, sizeof(char), 1, fp );	
		//50속쵱
#endif

		if( m_Header.FileSize )							// data
		{
			m_pData = new char[m_Header.FileSize];
			fread( m_pData, sizeof(char), m_Header.FileSize, fp );
		}

#ifdef _RXMSO_
		fread( &m_crc2, sizeof(char), 1, fp );	
#endif

#ifdef _MSO50_
		//50속쵱
		fread( &m_crc2, sizeof(char), 1, fp );	
		fread( &m_crc2, sizeof(char), 1, fp );	
		//50속쵱
#endif

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

BOOL CMHFile::CheckHeader()
{
	return TRUE;
	if( m_Header.dwVersion != (DWORD)(20040308+m_Header.dwType+m_Header.FileSize) )
	{
		Clear();
		MessageBox( NULL, "Header Version Error!!", m_sFileName, MB_OK );
		return FALSE;
	}
	if( m_Header.dwType == 0 )
	{
		Clear();
		MessageBox( NULL, "Header Type Error!!", m_sFileName, MB_OK );
		return FALSE;
	}	

	return TRUE;
}

BOOL CMHFile::CheckCRC()
{
	/*if( m_crc1 != m_crc2 )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}*/

	// decode
	char crc = (char)m_Header.dwType;
	for( DWORD i = 0; i < m_Header.FileSize; ++i )
	{
		crc += m_pData[i];
		m_pData[i] -= (char)i;
		if( i%m_Header.dwType == 0 )
			m_pData[i] -= (char)m_Header.dwType;
	}

	/*if( m_crc1 != crc )
	{
		Clear();
		MessageBox( NULL, "CheckCrc Error!!", "Error!!", MB_OK );
		return FALSE;
	}*/

	return TRUE;
}

void CMHFile::Clear()
{
	if( m_pData )
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	m_Dfp = 0;
	m_bInit = FALSE;
}

BOOL CMHFile::Init(char* filename,char* mode,DWORD dwFlag)
{
	if(!(dwFlag & MHFILE_FLAG_DONOTCHANGE_DIRECTORY_TOROOT))
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	
//	if( strcmp( filename, "./image/image_msg.bin" ) == 0 )
//		int a = 1;
//	printf( "%s\n", filename );	//KES DEBUG
	
	if(g_bUsingEnginePack)
	{
		if( strcmp( "rb", mode ) == 0 )		m_bReadMode = MHFILE_PACKEDFILE;
		else if( strcmp( "ET", mode ) == 0 )m_bReadMode = MHFILE_ENGINEPACKEDFILE;
		else if( strcmp( "EB", mode ) == 0 )m_bReadMode = MHFILE_ENGINEPACKEDFILE;
		else								m_bReadMode = MHFILE_NORMALMODE;
	}
	else
	{
		if( strcmp( "rb", mode ) == 0 )		m_bReadMode = MHFILE_PACKEDFILE;
		else								m_bReadMode = MHFILE_NORMALMODE;
	}

	
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			return OpenBin( filename );
		yCASE(MHFILE_NORMALMODE)
			fp = fopen(filename,"r");
			if(fp)
				return TRUE;
			else
			{
				if(!(dwFlag & MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR))
				{
					char buffer[255];
					sprintf(buffer, "[%s] file is not found",filename);
					LOGEX(buffer,PT_MESSAGEBOX);
				}
				
				return FALSE;
			}
		yCASE(MHFILE_ENGINEPACKEDFILE)
			g_pExecutive->GetFileStorage(&m_pFileStorage);
			m_pFilePointer = m_pFileStorage->FSOpenFile(filename,FSFILE_ACCESSMODE_TEXT);
			if(m_pFilePointer && m_pFileStorage)
				return TRUE;
			else
				return FALSE;
	yENDSWITCH

	return FALSE;
}

void CMHFile::Release()
{
	if(fp)
	{
		fclose(fp);
		fp = NULL;
	}
	Clear();

	if(m_pFilePointer)
	{
		m_pFileStorage->FSCloseFile(m_pFilePointer);
		m_pFileStorage->Release();
		m_pFilePointer = NULL;
		m_pFileStorage = NULL;
	}
}

char* CMHFile::GetString()
{
	static char buf[512];
	GetString(buf);
	return buf;
}

int CMHFile::GetString( char* pBuf )
{
	char buf[512] = {0,};
	DWORD j = 0;
	BOOL bStart, bEnd;
	bStart = bEnd = FALSE;
	// yh;
	int rt = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if(m_Dfp == (int)m_Header.FileSize) return (rt=EOF);

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
					++m_Dfp;
					if( bStart )
						bEnd = TRUE;
				}
				else
				{
					if( bEnd ) break;

					buf[j] = m_pData[i];
					++j;
					++m_Dfp;
					bStart = TRUE;
				}
			}
			buf[j] = 0;
		yCASE(MHFILE_NORMALMODE)
			rt = fscanf( fp, "%s", buf );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			rt = m_pFileStorage->FSScanf(m_pFilePointer,"%s",buf);

	yENDSWITCH

	_parsingKeywordString( buf, (char**)(&pBuf) );

	return rt;
}
/*
int CMHFile::GetString( char* pBuf )
{
	char buf[256] = {0,};
	DWORD j = 0;
	BOOL bflag = FALSE;
	// yh;
	int rt = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if(m_Dfp == m_Header.FileSize) return (rt=EOF);

			for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
			{
				if( m_pData[i] == 0x0d && m_pData[i+1] == 0x0a )	// return
				{
					m_Dfp += 2;
					break;
				}
				else if( m_pData[i] == 0x20 || m_pData[i] == 0x09 )	// spacebar, tab
				{
					m_Dfp++;
					if( bflag )	break;
					else		continue;
				}
				else
				{
					buf[j] = m_pData[i];
					j++;
					m_Dfp++;
					bflag = TRUE;
				}
			}
			buf[j] = 0;
		yCASE(MHFILE_NORMALMODE)
			rt = fscanf( fp, "%s", buf );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			rt = m_pFileStorage->FSScanf(m_pFilePointer,"%s",buf);

	yENDSWITCH

	_parsingKeywordString( buf, (char**)(&pBuf) );

	return rt;
}
*/
char* CMHFile::GetStringInQuotation()
{
	char buf[512]={0,};
	static char buf2[512];
	int c;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
//			m_Dfp--;
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
					__asm int 3;
					buf[n] = 0;
					break;
				}
				++n;
			}
			char* aa = &buf2[0];
			memcpy( buf2, buf, n+1 );	
		yCASE(MHFILE_NORMALMODE)
			while( (c=fgetc(fp)) != '"')	// 첫따옴표 나올때까지 스킵
			{
				if( c == 10 )//첫따옴표 나오기전에 엔터가 있으면 중지
				{
					Seek(-1);
					buf2[0] = 0;
					return buf2;
				}
			}
			int n=0;
			while(1)
			{
				ASSERT(!feof(fp));
				buf[n] = fgetc(fp);
				if(buf[n] == '"' )
				{
					buf[n] = 0;
					break;
				}
				if(buf[n] == 10) // 엔터
				{
					Seek(-1);
					buf[n] = 0;
					break;
				}
				++n;
			}
			char * aa = &buf2[0];
			//	_parsingKeywordString(buf, &aa);	//KES 030828 막았음. 특수기능 키가 짤림?
			memcpy( buf2, buf, n+1 );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH

	return buf2;
}

void CMHFile::GetStringInQuotation(char* pBuf)
{
	strcpy(pBuf,GetStringInQuotation());
}

void CMHFile::GetLine(char * pBuf, int len)
{
	char buf[2048] = {0,};
	DWORD j = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
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
		yCASE(MHFILE_NORMALMODE)
			fgets(buf, 512, fp);
			if( buf[strlen(buf)-1] == 10 )
				buf[strlen(buf)-1] = 0;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH

	_parsingKeywordString(buf, &pBuf);
}

void CMHFile::GetLineX(char * pBuf, int len)
{
	char buf[2048] = {0,};
	DWORD j = 0;

	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
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
		yCASE(MHFILE_NORMALMODE)
			fgets(buf, 512, fp);
			if( buf[strlen(buf)-1] == 10 )
				buf[strlen(buf)-1] = 0;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH

	_parsingKeywordString(buf, &pBuf);
}

int CMHFile::GetInt()
{
	return atoi(GetString());
}

LONG CMHFile::GetLong()
{
	return (LONG)atoi(GetString());
}

float CMHFile::GetFloat()
{
	return (float)atof(GetString());
}

DWORD CMHFile::GetDword()
{
	return (DWORD)atoi(GetString());
}

WORD CMHFile::GetWord()
{
	return (WORD)atoi(GetString());
}

BYTE CMHFile::GetByte()
{
	return (BYTE)atoi(GetString());
}

BOOL CMHFile::GetBool()
{
	return (BOOL)atoi(GetString());
}

void CMHFile::GetFloat(float* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetFloat();
	}
}

void CMHFile::GetWord(WORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetWord();
	}
}

void CMHFile::GetDword(DWORD* pArray,int count)
{
	for(int n=0;n<count;++n)
	{
		pArray[n] = GetDword();
	}
}

int CMHFile::GetWord(WORD* pOut)
{
	ASSERT(m_bReadMode == MHFILE_ENGINEPACKEDFILE);
	
	return m_pFileStorage->FSScanf(m_pFilePointer,"%d",pOut);
}

DWORD CMHFile::GetExpPoint()
{
	return (DWORD)atoi(GetString());
}

// magi82 - Exp Variable Type Change(070523)
EXPTYPE CMHFile::GetExpPoint64()
{
	return (EXPTYPE)_atoi64(GetString());
}

LEVELTYPE CMHFile::GetLevel()
{
	return (LEVELTYPE)atoi(GetString());
}

BOOL CMHFile::IsEOF()
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			if( m_Header.FileSize == (DWORD)m_Dfp )
				return TRUE;
			else
				return FALSE;
		yCASE(MHFILE_NORMALMODE)
			return feof(fp) ? TRUE : FALSE;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH
		
	return TRUE;
}

BOOL CMHFile::IsInited()
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			return m_bInit ? TRUE : FALSE;
		yCASE(MHFILE_NORMALMODE)
			return fp ? TRUE : FALSE;
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH
		
	return TRUE;
}
BOOL CMHFile::Seek(int n)
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			m_Dfp += n;
			return TRUE;
		yCASE(MHFILE_NORMALMODE)
			return fseek( fp, n, SEEK_CUR );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함

	yENDSWITCH
		
	return TRUE;
}
BOOL CMHFile::GetHex(DWORD* pOut)
{
	ySWITCH(m_bReadMode)
		yCASE(MHFILE_PACKEDFILE)
			ASSERT(0);
			// 삑!~ 지원안함
		yCASE(MHFILE_NORMALMODE)
			return fscanf( fp, "%x", pOut );
		yCASE(MHFILE_ENGINEPACKEDFILE)
			return m_pFileStorage->FSScanf(m_pFilePointer,"%x",pOut);

	yENDSWITCH

	return TRUE;
}

BOOL CMHFile::CheckLineEnd( int lineNum )
{
	char buf[512] = {0,};
	int rt = TRUE;
	if( m_Dfp == (int)m_Header.FileSize ) 
		return rt;

	char* pData = m_pData;	//어디서 참조하며 쓰일지 모르므로..
	if( m_bReadMode == MHFILE_PACKEDFILE )
	{
		for( DWORD i = m_Dfp; i < m_Header.FileSize; ++i )
		{
			if( 0x0d == pData[i] && 0x0a == pData[i+1] )	//CR
			{
				rt = TRUE;
				break;
			}
			else if( 0x09 == pData[i] || 0x20 == pData[i] )
			{
				continue;
			}
			else
			{
				sprintf( buf, "Check! LastValidNum [ %d ]  Gets End '%s'\nWanna Stop?", lineNum, GetString() );
				if( MessageBox( NULL, buf, m_sFileName, MB_YESNO) == IDYES )
				{
					HWND pHwnd = GetActiveWindow();
					PostMessage( pHwnd, WM_CLOSE, NULL, NULL );
				}
				rt = FALSE;
				
				break;
			}
		}
	}
	else
	{
		MessageBox( NULL, "ReadMode is not PACKEDFILE", m_sFileName, MB_OK );
		rt = FALSE;
	}

	return rt;
}

void _parsingKeywordString(char * inString, char ** outString)
{
	char * sp = inString;
	char * sp2 = *outString;
	while(*sp != 0)
	{
		//if(*sp & 0x80)	//국외에서 문제다!
		if( IsDBCSLeadByte(*sp) )
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
					case TEXT_DELIMITER:	//KES 추가
						{
							*sp2 = '^';
							++sp2;
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

/*
void _parsingKeywordStringEx(char * inString, line_node ** Top)
{
	line_node::release_line_node(Top);

	if(*inString == 0 || Top == NULL) return;
	
	char * sp = inString;
	line_node * curLineNode = (*Top) = new line_node;
	curLineNode->nextLine = NULL;
	char * cur_line = curLineNode->line;
	while(*sp != 0)
	{
		if(*sp & 0x80)
		{
			*cur_line++ = *sp++;
			*cur_line++ = *sp++;
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
					case TEXT_NEWLINECHAR:	// new line
						{
							*cur_line = 0;
							curLineNode->len = strlen(curLineNode->line);
							curLineNode = curLineNode->nextLine = new line_node;
							curLineNode->nextLine = NULL;
							cur_line = curLineNode->line;
						}
						break;
					case TEXT_TABCHAR:
						{
						}
						break;
					case TEXT_SPACECHAR:
						{	
							*cur_line = ' ';
							++cur_line;
						}
						break;
					}// - switch()
				}
				break;
			default:
				{
					*cur_line = *sp;
					++cur_line;
				}
			}//- switch()
			++sp;
		}
	}
	curLineNode->len = strlen(curLineNode->line);
}
*/
