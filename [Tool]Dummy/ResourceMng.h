// ResourceMng.h: interface for the CResourceMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCEMNG_H__65F124F6_95ED_46C2_A5E6_266E45EAC9BF__INCLUDED_)
#define AFX_RESOURCEMNG_H__65F124F6_95ED_46C2_A5E6_266E45EAC9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------
#pragma pack(push,1)

struct IPPORT
{
	char	Ip[16];
	WORD	Port;
};

struct MAPCHANGE
{
	int		nToMapNum;				// 17, 18, 19...
	int		nChangeNum;				// 1001, 1002, 1003...
	VECTOR3	vMapChange;
};

struct MAPDESC
{
	int			nMapNum;			// 17, 18, 19...
	VECTOR3		vLoginPoint;
	int			nMaxMapChange;
	MAPCHANGE	MapChangePoint[4];
	int			nMaxMovePoint;
	VECTOR3		vMovePoint[8];
};

#pragma pack(pop)
//--------------------------------------

#define RESMNG USINGTON(CResourceMng)

class CResourceMng  
{
protected:
	IPPORT		m_DisIPPort;
	IPPORT		m_AgentIPPort;

	int			m_nMaxMapDesc;
	MAPDESC*	m_pMapDesc;
		
public:
	CResourceMng();
	virtual ~CResourceMng();

	void	Init();
	void	Exit();

public:
	void	LoadGameDesc( char* filename, char* mode );
	void	LoadMapDesc( char* filename, char* mode );

	IPPORT*	GetDistribute()				{ return &m_DisIPPort; }
	IPPORT* GetAgent()					{ return &m_AgentIPPort; }
	void	SetAgentIP( char* ip )		{ strcpy( m_AgentIPPort.Ip, ip ); }
	void	SetAgentPort( WORD port )	{ m_AgentIPPort.Port = port; }

	MAPDESC	GetMapDesc( int mapnum );

};
	
EXTERNGLOBALTON(CResourceMng)

#endif // !defined(AFX_RESOURCEMNG_H__65F124F6_95ED_46C2_A5E6_266E45EAC9BF__INCLUDED_)
