#include "stdafx.h"
#include "cDialogueList.h"

#ifdef _TL_LOCAL_
#include "./Interface/cFont.h"
#endif


cDialogueList::cDialogueList()
{
	m_dwDefaultColor = NORMAL_COLOR;
	m_dwStressColor = STRESS_COLOR;
}

cDialogueList::~cDialogueList()
{
	for(int i=0;i<MAX_DIALOGUE_COUNT;++i)
	{
		PTRLISTSEARCHSTART(m_Dialogue[i],DIALOGUE*,Msg)
			delete Msg;
		PTRLISTSEARCHEND
			
		m_Dialogue[i].RemoveAll();
	}
}


void cDialogueList::LoadDialogueListFile(char* filePath, char* mode)
{
	CMHFile fp;
	if(!fp.Init(filePath, mode))
		return;
	char buff[128]={0,};

	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 128);
			continue;
		}
		CMD_ST(buff)
		CMD_CS("#Msg")
			DWORD dwMsgId = fp.GetDword(); // ¸Þ¼¼Áö ¹øÈ£ ÀÎ½Ä..
			if((fp.GetString())[0] == '{')
			{
				LoadDialogueList(dwMsgId, &fp);
			}
			else
				__asm int 3;
		CMD_CS("#TEXTCOLOR")
			m_dwDefaultColor = RGB( fp.GetDword(), fp.GetDword(), fp.GetDword() );
			m_dwStressColor = RGB( fp.GetDword(), fp.GetDword(), fp.GetDword() );
		CMD_EN
	}
}

void cDialogueList::LoadDialogueList(DWORD dwId, CMHFile* fp)
{
	char buff[1024];

	while(1)
	{
		fp->GetLine( buff, 1024 );
		if( *buff == '}' || fp->IsEOF() )
			break;

		ParsingLine( dwId, buff );
	}
}

#define NPCDIALOG_TEXTLEN	36

#ifdef _TL_LOCAL_
#define NPCDIALOG_TEXTWIDTH	220
#endif

void cDialogueList::ParsingLine( DWORD dwId, char* buf )
{
	DWORD dwColor	= m_dwDefaultColor;
	WORD wLine		= m_Dialogue[dwId].GetCount();	//????????

	char wBuff[512] = {0,};
	int	nCut = 0;
	int nStrLen = 0;

#ifdef _TL_LOCAL_
	BYTE brk[512];
	int nBrkNum = 0;
	int nTextExtent = 0;
#endif

	while( *buf )
	{
		if( *buf == '$' )
		{
			if( nStrLen != 0 )
			{
				wBuff[nStrLen] = 0;
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
				memset( wBuff, 0, 512 );
				nStrLen = 0;
			}

			buf += 2;
			if( *buf == 's' || *buf == 'S' )
				dwColor = m_dwStressColor;
			else
				dwColor = m_dwDefaultColor;

			++buf;

			continue;
		}

#ifdef _TL_LOCAL_	//for WB
		//---
		wBuff[nStrLen] = *buf;
		if( IsDBCSLeadByte( *buf ) )
		{
			++buf;
			++nStrLen;
			++nCut;

			wBuff[nStrLen] = *buf;
		}

		++buf;
		++nStrLen;
		++nCut;

		nTextExtent = CFONT_OBJ->GetTextExtentEx( 0, wBuff, nCut );

		if( nTextExtent > NPCDIALOG_TEXTWIDTH )
		{
			nBrkNum = g_TBreak.FindThaiWordBreak( wBuff, nStrLen, brk, 512, FTWB_SEPARATESYMBOL );

			//TEST
//			int n = 0;
//			for( int i = 0 ; i < nBrkNum ; ++i )
//			{
//				n+=brk[i];
//			}
//			if( nStrLen != n )
//				int assert = 0;
			//===

			if( nBrkNum > 1 )
			{
				buf -= brk[nBrkNum-1];
				wBuff[nStrLen-brk[nBrkNum-1]] = 0;
			}
			else
			{
				wBuff[nStrLen] = 0;
			}

			AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
			memset( wBuff, 0, 512 );

			nStrLen = 0;
			nCut = 0;
			++wLine;
			if( *buf == ' ' )
			{
				++buf;
			}
		}

		//---
#else
		wBuff[nStrLen] = *buf;
		if( IsDBCSLeadByte( *buf ) )
		{
			++buf;
			++nStrLen;
			++nCut;
			wBuff[nStrLen] = *buf;
		}

		++nCut;
		++buf;
		++nStrLen;

		if( nCut >= NPCDIALOG_TEXTLEN )		//ÁÙ¹Ù²Þ
		{
			if( nStrLen != 0 )
			{
				wBuff[nStrLen] = 0;
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );			
				nStrLen = 0;				
			}
			nCut = 0;
			++wLine;
			if( *buf == ' ' ) ++buf;
		}
/*
		if( buf != CharNext( CharPrev( buf, buf ) ) )	//2¹ÙÀÌÆ® ¹®ÀÚ¸é
		{
			wBuff[++nStrLen] = *(++buf);
			++nCut;
		}
*/
#endif

	}

	if( nStrLen != 0 )
	{
		wBuff[nStrLen] = 0;
		AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
	}
}
/*
void cDialogueList::ParsingLine( DWORD dwId, char* buf )
{
	DWORD dwColor = m_dwDefaultColor;
	WORD wLine = m_Dialogue[dwId].GetCount();
	unsigned int nLen = 0;

	char wBuff[256]={0,};

	int i = 0;

	// °ËÃâÇØ¼­ ¸¸µç´Ù.
	while(1)
	{
		if( (*buf) == '\0' )
		{
			// ÀúÀå ¸ñ·ÏÀÌ ÀÖÀ¸¸é ÀúÀå
			if( nLen = strlen(wBuff) )
			{
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
//				memset(wBuff, 0, 256);
//				i = 0;
			}
			break;
		}

		if((*buf) == '$')
		{
			// ÀúÀå ¸ñ·ÏÀÌ ÀÖÀ¸¸é ÀúÀå
			if( nLen = strlen(wBuff) )
			{
				AddLine( dwId, wBuff, dwColor, wLine, emLink_Null );
				memset(wBuff, 0, 256);
				i = 0;
			}

			char flag[4]={0,};
			memcpy(flag, buf, 3);
			buf += 3;
			
			if( strcmp(flag, "$cs") == 0 )
			{
//				dwColor = STRESS_COLOR;
				dwColor = m_dwStressColor;
			}
			if( strcmp(flag, "$ce") == 0 )
			{
//				dwColor = NORMAL_COLOR;
				dwColor = m_dwDefaultColor;
			}
		}
		wBuff[i++] = *buf;

		++buf;
	}
}
*/

DIALOGUE* cDialogueList::GetDialogue( DWORD dwMsgId, WORD wLine )
{
	if( m_Dialogue[dwMsgId].IsEmpty() ) return NULL;

	PTRLISTPOS p = m_Dialogue[dwMsgId].FindIndex(wLine);
	if( p == NULL ) return NULL;

	return (DIALOGUE*)m_Dialogue[dwMsgId].GetAt(p);
}


void cDialogueList::AddLine( DWORD dwId, char* str, DWORD color, WORD Line, WORD type)
{

	DIALOGUE* p = new DIALOGUE;
	p->Init();

	p->dwColor = color;
	strcpy( p->str, str );
	p->wLine = Line;
	p->wType = type;

	m_Dialogue[dwId].AddTail( p );
}
