// MoveMng.h: interface for the CMoveMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEMNG_H__1D7F1DF5_69FA_4EE6_8630_9513259ABE15__INCLUDED_)
#define AFX_MOVEMNG_H__1D7F1DF5_69FA_4EE6_8630_9513259ABE15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MOVEMNG USINGTON(CMoveMng)

class CMoveMng  
{
protected:
	map< int, SEND_MONSTER_TOTALINFO >				m_mapMonster;
	map< int, SEND_MONSTER_TOTALINFO >::iterator	mi;
	
public:
	CMoveMng();
	virtual ~CMoveMng();

	void	Init();
	void	Exit();
	void	Reset();
	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

public:
	void	AddMonster( SEND_MONSTER_TOTALINFO info );
	void	RemoveMonster( int key );
	void	RemoveAllMonster();
	SEND_MONSTER_TOTALINFO	FindNearMonster( VECTOR3 pos );
	SEND_MONSTER_TOTALINFO	FindMonster( DWORD dwID );
	int		GetMonsterNum()	{ return m_mapMonster.size(); }
	VECTOR3	GetMonsterPosition( DWORD dwID );

	void	StartMove( DWORD dwID, VECTOR3 targetpos );
	void	SetPosition( DWORD dwID, VECTOR3 pos );

	BOOL	IsMonster( DWORD dwID, VECTOR3 heropos );

};

EXTERNGLOBALTON(CMoveMng)

#endif // !defined(AFX_MOVEMNG_H__1D7F1DF5_69FA_4EE6_8630_9513259ABE15__INCLUDED_)
