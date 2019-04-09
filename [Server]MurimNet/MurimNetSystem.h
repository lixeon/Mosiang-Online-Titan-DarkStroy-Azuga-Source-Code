// MurimNetSystem.h: interface for the CMurimNetSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MURIMNETSYSTEM_H__88E92222_0B1E_4B52_AB9D_22CD91C72154__INCLUDED_)
#define AFX_MURIMNETSYSTEM_H__88E92222_0B1E_4B52_AB9D_22CD91C72154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMNServerManager;
class CMNPlayerManager;
class CChannelManager;
class CPlayRoomManager;
class CFront;
class CMNPlayer;

class CMurimNetSystem  
{
protected:

//	CMNServerManager	//---MNServer managing
//	CMNPlayerManager	//---player managing
	
//	CFront				//---front
//	CChannelManager		//---channel managing
//	CPlayRoomManager	//---playroom managing


	CMNServerManager*	m_pMNServerMgr;
	CMNPlayerManager*	m_pMNPlayerMgr;

	//CFront*				m_pFront;	
	CChannelManager*	m_pChannelMgr;
	CPlayRoomManager*	m_pPlayRoomMgr;


public:
	CMurimNetSystem();
	virtual ~CMurimNetSystem();

	void Init();
	void Release();

	void SelectChannelMode( CMNPlayer* pMNPlayer, int nChannelMode );
	
	CMNPlayerManager*	GetMNPlayerManager()	{ return m_pMNPlayerMgr; }
	CChannelManager*	GetChannelManager()		{ return m_pChannelMgr; }
	CPlayRoomManager*	GetPlayRoomManager()	{ return m_pPlayRoomMgr; }
	
};

extern CMurimNetSystem g_MNSystem;

#endif // !defined(AFX_MURIMNETSYSTEM_H__88E92222_0B1E_4B52_AB9D_22CD91C72154__INCLUDED_)
