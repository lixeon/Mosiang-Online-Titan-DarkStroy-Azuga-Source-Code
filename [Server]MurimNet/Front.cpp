// Front.cpp: implementation of the CFront class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Front.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFront::CFront()
{

}

CFront::~CFront()
{

}

void CFront::Init( DWORD dwMaxPlayer )
{
	m_htPlayer.Initialize( dwMaxPlayer );

	m_nTotalPlayer	= 0;
	m_dwMaxPlayer	= dwMaxPlayer;
}

void CFront::AddPlayer( CMNPlayer* pPlayer, DWORD dwKey )
{
	m_htPlayer.Add( pPlayer, dwKey );
}

void CFront::DeletePlayer( DWORD dwKey )
{
	m_htPlayer.Remove( dwKey );
}

void CFront::DeleteAllPlayers()
{
	m_htPlayer.RemoveAll();
}