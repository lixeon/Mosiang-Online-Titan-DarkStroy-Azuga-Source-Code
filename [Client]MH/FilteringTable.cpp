// FilteringTable.cpp: implementation of the CFilteringTable class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FilteringTable.h"
#include "MHFile.h"

#ifdef _MHCLIENT_
#include "MainGame.h"
extern HWND _g_hWnd;
GLOBALTON(CFilteringTable)
#endif

extern int g_nServerSetNum;

CFilteringTable::CFilteringTable()
{

}

CFilteringTable::~CFilteringTable()
{
	Release();
}

void CFilteringTable::Init()
{
#ifdef _FILTER_TEST_
	m_nMemory = 0;
#endif
	
#ifdef _FILE_BIN_
	LoadFilterWordBinary( FILEPATH_FILTERWORD );
#else
	LoadFilterWord( FILEPATH_FILTERWORD );
#endif

#ifdef _JAPAN_LOCAL_
	RANGE_ARRAY RangeArray[] = 
	{
			// 2 BYTE Characters.
	{ 0x815b, 0x815b }, // Katakana-Hiragana Prolonged Sound Mark
	{ 0x8199, 0x81a5 }, // White Star ~ Black Down-pointing Ttriangle
	{ 0x81f2, 0x81f5 }, // Music Sharp Sign ~ Dagger
	{ 0x824F, 0x8258 }, // Fullwidth Digit Zero ~ Fullwidth Digit Nine
	{ 0x8260, 0x8279 }, // Fullwidth Latin Capital Letter A ~ Fullwidth Latin Capital Letter Z
	{ 0x8281, 0x829a }, // Fullwidth Latin Small Letter A ~ Fullwidth Latin Small Letter Z
	{ 0x829f, 0x82f1 }, // Hiragana Letter Small A ~ Hiragana Letter N
	{ 0x8340, 0x837e },	// Katakana Letter Small A ~ Katakana Letter Small Me 
	{ 0x8380, 0x8396 }, // Katakana Letter Small Mu ~ Katakana Letter Small Ke
	{ 0x8794, 0x8794 }, // N-Ary Summation
	{ 0x8740, 0x875d }, // Circled Digit One ~ Roman Numeral Ten
	{ 0xfa40, 0xfa49 }, // Small Roman Numeral One ~ Small Roman Numeral Ten

	// Kanji.. !!!!! TT
	{ 0x889f, 0x88fc },	{ 0x8940, 0x897e },	{ 0x8980, 0x89fc },	{ 0x8a40, 0x8a7e },	{ 0x8a80, 0x8afc },	{ 0x8b40, 0x8b7e },	
	{ 0x8b80, 0x8bfc },	{ 0x8c40, 0x8c7e },	{ 0x8c80, 0x8cfc },	{ 0x8d40, 0x8d7e },	{ 0x8d80, 0x8dfc },	{ 0x8e40, 0x8e7e },
	{ 0x8e80, 0x8efc },	{ 0x8f40, 0x8f7e },	{ 0x8f80, 0x8ffc },	{ 0x9040, 0x907e },	{ 0x9080, 0x90fc },	{ 0x9140, 0x917e },
	{ 0x9180, 0x91fc },	{ 0x9240, 0x927e },	{ 0x9280, 0x92fc },	{ 0x9340, 0x937e },	{ 0x9380, 0x93fc },	{ 0x9440, 0x947e },
	{ 0x9480, 0x94fc },	{ 0x9540, 0x957e },	{ 0x9580, 0x95fc },	{ 0x9640, 0x967e },	{ 0x9680, 0x96fc },	{ 0x9740, 0x977e }, 
	{ 0x9780, 0x97fc },	{ 0x9840, 0x9872 },	{ 0x989f, 0x98fc },	{ 0x9940, 0x997e },	{ 0x9980, 0x99fc },	{ 0x9a40, 0x9a7e },
	{ 0x9a80, 0x9afc },	{ 0x9b40, 0x9b7e },	{ 0x9b80, 0x9bfc },	{ 0x9c40, 0x9c7e },	{ 0x9c80, 0x9cfc },	{ 0x9d40, 0x9d7e },	
	{ 0x9d80, 0x9dfc },	{ 0x9e40, 0x9e7e },	{ 0x9e80, 0x9efc },	{ 0x9f40, 0x9f7e },	{ 0x9f80, 0x9ffc },	{ 0xe040, 0xe07e },
	{ 0xe080, 0xe0fc },	{ 0xe140, 0xe17e },	{ 0xe180, 0xe1fc },	{ 0xe240, 0xe27e },	{ 0xe280, 0xe2fc },	{ 0xe340, 0xe37e },
	{ 0xe380, 0xe3fc },	{ 0xe440, 0xe47e },	{ 0xe480, 0xe4fc },	{ 0xe540, 0xe57e },	{ 0xe580, 0xe5fc },	{ 0xe640, 0xe67e },
	{ 0xe680, 0xe6fc },	{ 0xe740, 0xe77e },	{ 0xe780, 0xe7fc },	{ 0xe840, 0xe87e },	{ 0xe880, 0xe8fc },	{ 0xe940, 0xe97e },	
	{ 0xe980, 0xe9fc },	{ 0xea40, 0xea7e },	{ 0xea80, 0xeaa4 },	{ 0xed40, 0xed7e },	{ 0xed80, 0xedfc },	{ 0xee40, 0xee7e },
	{ 0xee80, 0xeeec },	{ 0xfa5c, 0xfa7e },	{ 0xfa80, 0xfafc },	{ 0xfb40, 0xfb7e },	{ 0xfb80, 0xfbfc },	{ 0xfc40, 0xfc4b }
	};

	m_dwRangeArrayCount = sizeof(RangeArray)/sizeof(RANGE_ARRAY);
	m_RangeArray = new RANGE_ARRAY[m_dwRangeArrayCount];

	for( DWORD i = 0 ; i < m_dwRangeArrayCount ; ++i )
	{
		m_RangeArray[i] = RangeArray[i];
	}

#endif

	m_pCurNode = NULL;
}

void CFilteringTable::Release()
{
	for( int i = 0 ; i < eFilter_Count ; ++i )
	{
		if( m_RootNode[i].pChild )
			DeleteNode( m_RootNode[i].pChild );
	}

#ifdef _JAPAN_LOCAL_
	if( m_RangeArray )
		delete[] m_RangeArray;
#endif
}

void CFilteringTable::DeleteNode( FILTER_NODE* pNode )
{
	if( pNode->pChild )
	{
		DeleteNode( pNode->pChild );
		pNode->pChild = NULL;
	}

	if( pNode->pSibling )
	{
		DeleteNode( pNode->pSibling );
		pNode->pSibling = NULL;
	}
	
	delete pNode;
}

void CFilteringTable::CleanNode( FILTER_NODE* pNode )
{
	if( pNode->pChild )
	{
		CleanNode( pNode->pChild );
	}

	if( pNode->pSibling )
	{
		CleanNode( pNode->pSibling );
	}
	
//	pNode->bSpread = FALSE;
}

void CFilteringTable::AddWord( char *pWord, int nKind )
{
	AddNode( &m_RootNode[nKind], pWord );
}

void CFilteringTable::AddNode( FILTER_NODE* pNode, char* pWord )
{
	if( *pWord == NULL )
	{
		pNode->bEndFlag = TRUE;
		return;
	}

	if( pNode->pChild )
	{
		pNode = pNode->pChild;

		while( pNode )
		{
			if( pNode->cChar == *pWord )
			{
				if( IsDBCSLeadByte( pNode->cChar ) )
				{
					if( pNode->cExChar == *(pWord+1) )
					{
						pWord += 2;
						AddNode( pNode, pWord );
						return;
					}
				}
				else
				{
					++pWord;
					AddNode( pNode, pWord );
					return;
				}
			}
			
			if( pNode->pSibling == NULL )
			{
				FILTER_NODE* pSibling	= new FILTER_NODE;
#ifdef _FILTER_TEST_
				m_nMemory += sizeof( FILTER_NODE );
#endif
				pSibling->cChar			= *pWord++;
				if( IsDBCSLeadByte( pSibling->cChar ) )
				{
					pSibling->cExChar	= *pWord++;
				}
				
				pNode->pSibling			= pSibling;
//				pSibling->pParent		= pNode->pParent;
				
				AddNode( pSibling, pWord );
				return;
			}

			pNode = pNode->pSibling;
		}
	}
	else
	{
		FILTER_NODE* pChild = new FILTER_NODE;
#ifdef _FILTER_TEST_
		m_nMemory += sizeof( FILTER_NODE );
#endif
		pChild->cChar		= *pWord++;
		if( IsDBCSLeadByte( pChild->cChar ) )
		{
			pChild->cExChar	= *pWord++;
		}

		pNode->pChild		= pChild;
//		pChild->pParent		= pNode;

		AddNode( pChild, pWord );
	}
}

BOOL CFilteringTable::FilterWordInString( char* pStr, int nKind, int nMethod )
{
	//초기화 넣기(spread를 모두 풀기)
	for( int i = 0 ; i < eFilter_Count ; ++i )
	{
		if( m_RootNode[i].pChild )
			CleanNode( m_RootNode[i].pChild );
	}

	m_bSearched = FALSE;

	switch( nMethod )
	{
	case eFM_WHOLE_MATCH:
		return FM_WholeMatch( pStr, nKind );

	case eFM_INCLUDE:
		return FM_Include( pStr, nKind );

	case eFM_ALLOWSPACE:
//		return FM_AllowSpace( pStr, nKind, 8 );
		return FM_Include( pStr, nKind );
	}

	return FALSE;
}

BOOL CFilteringTable::FM_WholeMatch( char* pStr, int nKind )
{
//	m_nStrPos = 0;
	
	while( *pStr )
	{
		m_bSpread = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			SearchNode( m_RootNode[nKind].pChild, pStr, 0, TRUE );
			pStr += 2;
		}
		else
		{
			SearchNode( m_RootNode[nKind].pChild, pStr, 0, FALSE );
			++pStr;
		}

//		++m_nStrPos;

		if( m_bSearched )
		{
			if( *pStr ) m_bSearched = FALSE;
			else		return TRUE;
		}
		if( m_bSpread == FALSE ) return FALSE;
	}

	return FALSE;
}

BOOL CFilteringTable::FM_Include( char* pStr, int nKind )
{
//	m_nStrPos = 0;
//	int nOffset = 1;

	m_pCurNode = m_RootNode[nKind].pChild; //+

	while( *pStr )
	{
		m_bSpread = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
//			SearchNode( m_RootNode[nKind].pChild, pStr, 0, TRUE );	//+
			SearchNode( m_pCurNode, pStr, 0, TRUE );	//+
//			nOffset = 2;
			pStr += 2;
		}
		else
		{
//			SearchNode( m_RootNode[nKind].pChild, pStr, 0, FALSE );	//+
			SearchNode( m_pCurNode, pStr, 0, FALSE );	//+
//			nOffset = 1;
//			if( m_pCurNode == m_RootNode[nKind].pChild || m_bSpread == TRUE )	//+
			++pStr;
		}

		if( m_bSpread == TRUE )
		{
			if( m_pCurStrPos == NULL )
				m_pCurStrPos = pStr;
		}
		else
		{
			m_pCurNode = m_RootNode[nKind].pChild;
			if( m_pCurStrPos )
			{
				pStr = m_pCurStrPos;
				m_pCurStrPos = NULL;
			}
		}


//		++m_nStrPos;

		if( m_bSearched ) return TRUE;
//		if( m_bSpread == FALSE )
//		{
//			if( m_nStrPos > 0 ) nOffset = 0;	//만약 서치 중간에 실패하면 다시 처음부터 시작.
//			m_nStrPos = 0;
//		}
//		else
//		{
//			++m_nStrPos;
//		}

//		pStr += nOffset;
	}

	return FALSE;
}
/*
BOOL CFilteringTable::FM_AllowSpace( char* pStr, int nKind, int nAllowSpace )
{
	while( *pStr )
	{
		if( IsDBCSLeadByte( *pStr ) )
		{
			SearchNode_AllowSpace( m_RootNode[nKind].pChild, pStr, TRUE );
			pStr += 2;
		}
		else
		{
			SearchNode_AllowSpace( m_RootNode[nKind].pChild, pStr, FALSE );
			++pStr;
		}

		if( m_bSearched ) return TRUE;
	}

	return FALSE;
}
*/
void CFilteringTable::SearchNode( FILTER_NODE* pNode, char* pStr, int nDepth, BOOL bDBC )
{
	if( pNode == NULL )	return;

//	if( pNode->bSpread )
//	{
//		SearchNode( pNode->pChild, pStr, nDepth + 1, bDBC );
//---
//		if( m_bSpread == FALSE )
//			pNode->bSpread = FALSE;
//---
//	}
//	else
	{
		if( bDBC )		//confirm
		{
			if( pNode->cChar == *pStr && pNode->cExChar == *(pStr+1)/* && m_nStrPos == nDepth*/ )
			{
//				pNode->bSpread	= TRUE;
				m_bSpread		= TRUE;

				m_pCurNode = pNode->pChild;	

				if( pNode->bEndFlag == TRUE )
					m_bSearched = TRUE;

				return;
			}
		}
		else
		{
			if( pNode->cChar == *pStr/* && m_nStrPos == nDepth*/ )
			{
//				pNode->bSpread	= TRUE;
				m_bSpread		= TRUE;	

				m_pCurNode = pNode->pChild;	//++

				if( pNode->bEndFlag == TRUE )
					m_bSearched = TRUE;

				return;
			}
		}
	}

	SearchNode( pNode->pSibling, pStr, nDepth, bDBC );
}
/*
void CFilteringTable::SearchNode_AllowSpace( FILTER_NODE* pNode, char* pStr, BOOL bDBC )
{
	if( pNode == NULL )	return;

	if( pNode->bSpread )
	{
		SearchNode_AllowSpace( pNode->pChild, pStr, bDBC );
	}
	else
	{
		if( bDBC )		//confirm
		{
			if( pNode->cChar == *pStr && pNode->cExChar == *(pStr+1) )
			{
				if( pNode->bEndFlag == TRUE )
					m_bSearched = TRUE;

				pNode->bSpread	= TRUE;

				return;
			}
		}
		else
		{
			if( pNode->cChar == *pStr )
			{
				if( pNode->bEndFlag == TRUE )
					m_bSearched = TRUE;

				pNode->bSpread	= TRUE;

				return;
			}
		}
	}

	SearchNode_AllowSpace( pNode->pSibling, pStr, bDBC );
}
*/
BOOL CFilteringTable::FilterChat( char* pChat )
{
	strcpy( m_strBuf, pChat );
	strupr( m_strBuf );
	
	if( FilterWordInString( m_strBuf, eFilter_Slang, eFM_INCLUDE ) ) //eFM_ALLOWSPACE
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CFilteringTable::IsUsableName( char* pName, BOOL bUseByte )
{
	strcpy( m_strBuf, pName );
	strupr( m_strBuf );

	if( FilterWordInString( m_strBuf, eFilter_System, eFM_WHOLE_MATCH ) )
		return FALSE;

	if( FilterWordInString( m_strBuf, eFilter_GM, eFM_INCLUDE ) )
		return FALSE;

	if( FilterWordInString( m_strBuf, eFilter_Slang, eFM_INCLUDE ) )	//eFM_ALLOWSPACE
		return FALSE;

	if( bUseByte )
	if( FilterWordInString( m_strBuf, eFilter_Byte, eFM_INCLUDE ) )
		return FALSE;

	return TRUE;
}

BOOL CFilteringTable::IsInvalidCharInclude( unsigned char* pStr, BOOL AllowAllDBC /*= FALSE*/ )
{
	BOOL bOk;

	while( *pStr )
	{
		bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{

			if( AllowAllDBC )
			{
				bOk = TRUE;
			}
			else
			{

#ifdef _JAPAN_LOCAL_
				if( IsInDBCRange( MAKEWORD(pStr[1],pStr[0]) ) )
					bOk = TRUE;

#elif defined TAIWAN_LOCAL
				///////////
				bOk = TRUE;
				///////////
#elif defined _ML_LOCAL_

#ifdef _MHCLIENT_
				if( g_nServerSetNum != 1 || MAINGAME->GetUserLevel() <= 4 )	// !english server
#else
				if( g_nServerSetNum != 1 || g_pServerSystem->IsTestServer() )
#endif
				{
					//big5 only chinese character (http://www.microsoft.com/globaldev/reference/dbcs/950.mspx)
					if( ( pStr[0] >= 0xa4 && pStr[0] <= 0xc5 ) || ( pStr[0] >= 0xc9 && pStr[0] <= 0xf8 ) )
					{
						if( ( pStr[1] >= 0x40 && pStr[1] < 0x7f ) || ( pStr[1] > 0xa0 && pStr[1] < 0xff ) )
							bOk = TRUE;
					}
					else if( pStr[0] == 0xc6 )
					{
						if( pStr[1] >= 0x40 && pStr[1] < 0x7f )
							bOk = TRUE;
					}
					else if( pStr[0] == 0xf9 )
					{
						if( ( pStr[1] >= 0x40 && pStr[1] < 0x7f ) || ( pStr[1] > 0xa0 && pStr[1] <= 0xdc ) )
							bOk = TRUE;
					}
					/*
					if( pStr[0] >= 0xa1 && pStr[0] <=0xf9 &&
					( pStr[1] >= 0x40 && pStr[1] <= 0x7e ||
					pStr[1] >= 0xa1 && pStr[1] <= 0xfe ) )
					{
					bOk = TRUE;
					}
					*/
				}
#elif defined _HK_LOCAL_
				bOk = TRUE;

				/*			if( pStr[0] >= 0xa4 && pStr[0] <=0xc6 && pStr[1] >= 0x40 && pStr[1] <= 0x7e )//0xA440-0xC67E
				{
				bOk = TRUE;
				}
				else if( pStr[0] >= 0xc9 && pStr[0] <= 0xf9 && pStr[1] >= 0x40 && pStr[1] <= 0xd5 )//0xC940-0xF9D5
				{
				bOk = TRUE;
				}
				*/
				///////////
#elif defined _KOR_LOCAL_
				bOk = TRUE;

#elif defined _TL_LOCAL_

				bOk = FALSE;
#else
				if( pStr[0] >= 0xb0 && pStr[0] <=0xc8 && pStr[1] >= 0xa1 && pStr[1] <= 0xfe )//0xB0A1~0xC8FE
				{
					bOk = TRUE;
				}
				else if( pStr[0] >= 0x81 && pStr[0] <= 0xc6 && pStr[1] >= 0x41 && pStr[1] <= 0xfe )
				{
					bOk = TRUE;
				}

				if( pStr[0] >= 0xa1 && pStr[0] <= 0xac && pStr[1] >= 0x80 && pStr[1] <= 0xfe )
				{
					bOk = FALSE;
				}
#endif
			}

			++pStr;
		}
		else
		{
			//영문
			//if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
			//	bOk = TRUE;
			//숫자
			//else if( *pStr >= '0' && *pStr <= '9' )
			//if( *str == '[' || *str == ']')
			if( (*pStr>= 32 && *pStr < 127) || IsDBCSLeadByte(*pStr) )
				bOk = TRUE;
#ifdef _JAPAN_LOCAL_
			else if( *pStr >= 0xa6 && *pStr <= 0xdf )	//일본 반각문자 //// Halfwidth Katakana Letter Wo ~ Halfwidth Katakana Semi-Voiced Sound Mark
				bOk = TRUE;
#endif
#ifdef _TL_LOCAL_
			else if( *pStr >= 0xa1 && *pStr <= 0xce )
				bOk = TRUE;
			else if( *pStr >= 0xcf && *pStr <= 0xd9 )
				bOk = TRUE;
			else if( *pStr >= 0xe0 && *pStr <= 0xec )
				bOk = TRUE;
#endif


//#ifdef TAIWAN_LOCAL			
//			//기호들
//			else if( *pStr >= 32 && *pStr < 127 )
//				bOk = TRUE;
//#endif
			//鯤소폅츰櫓쇌꼇옵鹿관벵왕목
			if (*pStr == 0x20 )// 0x20 // '\40' 
			{
				bOk = FALSE;
			}
		}

		++pStr;

		if( bOk == FALSE )
			return TRUE;
	}

	return FALSE;
}


//문자 포함여부 검사
//2바이트 문자는 검사 불가!
BOOL CFilteringTable::IsCharInString( char* pStr, char* pNoChar )
{
	while( *pStr )
	{
		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;			
		}
		else
		{
			char* p = pNoChar;
			while( *p )
			{
				if( *pStr == *p ) return TRUE;
				++p;
			}
		}
		++pStr;
	}

	return FALSE;
}

BOOL CFilteringTable::LoadFilterWord( char* strFile )
{
	HANDLE	hFile;

	hFile = CreateFile( strFile, GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
#ifdef _MHCLIENT_
		MessageBox( _g_hWnd, "File Not Found!", "Error", MB_OK );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
#endif
		return FALSE;
	}

	DWORD dwFileSize = GetFileSize( hFile, NULL );
	if( dwFileSize == -1 ) return FALSE;

	char* buf = new char[dwFileSize + 1];

	DWORD dwRead;
	if( !ReadFile( hFile, buf, dwFileSize, &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		delete[] buf;
		return FALSE;
	}

	buf[dwFileSize] = NULL;
	SettingFilterWord( buf, dwFileSize );
	
	CloseHandle( hFile );
	delete[] buf;
	return TRUE;	
}

BOOL CFilteringTable::LoadFilterWordBinary( char* strFile )
{
	CMHFile file;
	if( file.OpenBin( strFile ) )
	{
		DWORD dwFileSize = file.GetDataSize();
		char* buf = new char[dwFileSize + 1];
		memcpy( buf, file.GetData(), dwFileSize+1);
		buf[dwFileSize] = NULL;
		SettingFilterWord( buf, dwFileSize );
		delete[] buf;

		file.Release();

		return TRUE;
	}

#ifdef _MHCLIENT_
	MessageBox( _g_hWnd, "File Not Found!", "Error", MB_OK );
	PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
#endif
	return FALSE;
}

void CFilteringTable::SettingFilterWord( char* pStr, DWORD bufLen )
{
//	m_strFile = pStr;
	int nKind = 0;

	char seps[] = ",\t\n\r";
	char* token;
	char OutBuf[256];
	
	token = strtok( pStr, seps );
	RemoveSpace( token, OutBuf );
	strupr( OutBuf );
	
	while( token != NULL )
	{
		if( token[0] != '@' )	//@걸러내고..
		{
			if( token[0] == '#' )
			{
				if( strcmp( token, "#SLANG" ) == 0 )
				{
					nKind = eFilter_Slang;
				}
				else if( strcmp( token, "#GM" ) == 0 )
				{
					nKind = eFilter_GM;
				}
				else if( strcmp( token, "#SYSTEM" ) == 0 )
				{
					nKind = eFilter_System;
				}
				else if( strcmp( token, "#BYTE" ) == 0 )
				{
					nKind = eFilter_Byte;
				}
			}
			else
			{
				AddWord( OutBuf, nKind );
			}
		}

		token = strtok( NULL, seps );	//get next token
		RemoveSpace( token, OutBuf );
		strupr( OutBuf );
	}
}

int CFilteringTable::RemoveSpace( char* pStrIn, char* pStrOut )
{
	if( pStrIn == NULL ) return 0;
	
	int nLen = 0;

	while( *pStrIn )
	{
		if( *pStrIn != ' ' )
		{
			*pStrOut = *pStrIn;
			++pStrOut;
			++nLen;
		}

		++pStrIn;
	}

	*pStrOut = 0;

	return nLen;
}
