// MoveMng.cpp: implementation of the CMoveMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMoveMng)

CMoveMng::CMoveMng()
{

}

CMoveMng::~CMoveMng()
{

}

void CMoveMng::Init()
{
}

void CMoveMng::Exit()
{
///	Reset();
}

void CMoveMng::Reset()
{
	RemoveAllMonster();
}

void CMoveMng::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_MOVE_TARGET:
		{
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;

			if( pmsg->dwMoverID >= 100000 )
			{
				MOVE_INFO info;
				pmsg->GetTargetInfo(&info);
												
				SetPosition( pmsg->dwMoverID, *info.GetTargetPosition(info.GetMaxTargetPosIdx()-1) );
			}
		}
		break;

	case MP_MOVE_STOP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;

//			if( IsMonster( pmsg->dwMoverID ) )
			{
				VECTOR3 pos;
				pmsg->cpos.Decompress(&pos);

				SetPosition( pmsg->dwMoverID, pos );
			}
		}
		break;
	case MP_MOVE_MONSTERMOVE_NOTIFY:
		{
		}
		break;
	}
}

void CMoveMng::AddMonster( SEND_MONSTER_TOTALINFO info )
{
	SEND_MONSTER_TOTALINFO temp = info;

	m_mapMonster.insert( make_pair( temp.BaseObjectInfo.dwObjectID, info ) );
}

void CMoveMng::RemoveMonster( int key )
{
	mi = m_mapMonster.find( key );
	if( mi != m_mapMonster.end() )
		m_mapMonster.erase( mi );
}

void CMoveMng::RemoveAllMonster()
{
//	mi = m_mapMonster.begin();
//	while( mi != m_mapMonster.end() )
//	{
//		m_mapMonster.erase( mi );
//		++mi;
//	}
	m_mapMonster.clear();
}

SEND_MONSTER_TOTALINFO CMoveMng::FindNearMonster( VECTOR3 pos )
{
	SEND_MONSTER_TOTALINFO info;
	memset( &info, 0, sizeof(SEND_MONSTER_TOTALINFO) );
/*	VECTOR3 monpos, minpos;
	FLOAT distance = 100000.0f;
	FLOAT dis = 0.0f;	

	mi = m_mapMonster.begin();
	while( mi != m_mapMonster.end() )
	{
		monpos = (*mi).second.BaseMoveInfo.CurPosition;
		dis = CalcDistanceXZ( &pos, &monpos );
		if( dis <= distance )
		{
			distance = dis;
			info = (*mi).second;
		}
		++mi;
	}
*/
	return info;
}

SEND_MONSTER_TOTALINFO CMoveMng::FindMonster( DWORD dwID )
{
	SEND_MONSTER_TOTALINFO info;
	memset( &info, 0, sizeof(SEND_MONSTER_TOTALINFO) );

	mi = m_mapMonster.find( dwID );
	if( mi != m_mapMonster.end() )
	{
		info = (*mi).second;
		return info;
	}

	return info;
}

void CMoveMng::SetPosition( DWORD dwID, VECTOR3 pos )
{
/*	mi = m_mapMonster.find( dwID );
	if( mi != m_mapMonster.end() )
		(*mi).second.BaseMoveInfo.CurPosition = pos;	
		*/
}

BOOL CMoveMng::IsMonster( DWORD dwID, VECTOR3 heropos )
{
/*	mi = m_mapMonster.find( dwID );
	if( mi != m_mapMonster.end() )
	{
		SEND_MONSTER_TOTALINFO info = FindMonster( dwID );
		VECTOR3 dist;
		dist.x = sqrt( (info.BaseMoveInfo.CurPosition.x - heropos.x)*(info.BaseMoveInfo.CurPosition.x - heropos.x) );
		dist.z = sqrt( (info.BaseMoveInfo.CurPosition.z - heropos.z)*(info.BaseMoveInfo.CurPosition.z - heropos.z) );

		if( sqrt( dist.x*dist.x + dist.z*dist.z) <= 500 )
			return TRUE;
		else
			return FALSE;
	}
*/
	return FALSE;
}

VECTOR3 CMoveMng::GetMonsterPosition( DWORD dwID )
{
	VECTOR3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;

/*	SEND_MONSTER_TOTALINFO info;
	memset( &info, 0, sizeof(SEND_MONSTER_TOTALINFO) );

	mi = m_mapMonster.find( dwID );
	if( mi != m_mapMonster.end() )
	{
		info = (*mi).second;
		pos = info.BaseMoveInfo.CurPosition;
	}
*/
	return pos;	
}