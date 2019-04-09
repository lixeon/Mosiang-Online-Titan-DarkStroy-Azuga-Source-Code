// MNStgPlayRoom.h: interface for the CMNStgPlayRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNSTGPLAYROOM_H__5F6725CF_E284_4E97_B056_7658A50952D0__INCLUDED_)
#define AFX_MNSTGPLAYROOM_H__5F6725CF_E284_4E97_B056_7658A50952D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MNStage.h"


class CMNPlayRoomDialog;

/////////////////////////////
// PlayRoom
//	- prepare to play a game

class CMNStgPlayRoom : public CMNStage  
{
protected:
	
	static CMNPlayRoomDialog*		m_pInterface;

	DWORD		m_dwPlayRoomIndex;

//countdown
	DWORD		m_dwStartTime;
	DWORD		m_dwCountDown;
	BOOL		m_bGameStart;
	DWORD		m_dwMapNum;
	
public:

	CMNStgPlayRoom();
	virtual ~CMNStgPlayRoom();

	virtual void Init();
	virtual void Release();

	virtual void Open();
	virtual void Close();

	virtual void Process();
	virtual void Render();

	virtual void NetworkMsgParse( BYTE Category, BYTE Protocol, void* pMsg );

//Inline
	static CMNPlayRoomDialog* GetInterface() { return m_pInterface; }

};

#endif // !defined(AFX_MNSTGPLAYROOM_H__5F6725CF_E284_4E97_B056_7658A50952D0__INCLUDED_)
