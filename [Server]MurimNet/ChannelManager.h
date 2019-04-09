// ChannelManager.h: interface for the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELMANAGER_H__28137EA9_F71E_4F27_B55A_B1EF47A8E644__INCLUDED_)
#define AFX_CHANNELMANAGER_H__28137EA9_F71E_4F27_B55A_B1EF47A8E644__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//max channels in a M.Net?

#include <HashTable.h>
#include "../4DyuchiGXGFunc/global.h"
#include "Channel.h"

class CFront;
class CMNPlayer;

class CChannelManager  
{
protected:

//	CFront*	m_pFront;
	CMemoryPool<CChannel>		m_mpChannel;
	CYHHashTable<CChannel>		m_htChannel;
	INDEXCR_HANDLE				m_hIndexCreator;

	CChannel*	m_pDefaultChannel;

	DWORD		m_dwTotalChannel;
	DWORD		m_dwMaxChannel;


public:
	
	CChannelManager();
	virtual ~CChannelManager();

	void Init( DWORD dwMaxChannel );
	void Release();

	CChannel* CreateChannel( CHANNEL_CREATE_INFO* pcci );
	void DeleteChannel( DWORD dwChannelIndex );
	void DeleteChannel( CChannel* pChannel );
	void DeleteAllChannel();
	CChannel* GetChannel( DWORD dwChannelIndex );
	
//	void SelectChannelMode( CMNPlayer* pMNPlayer, int nChannelMode );
	void SendMsg_ChannelList( CMNPlayer* pMNPlayer );

//for Default-Channel(DC)
	CChannel* GetDefaultChannel()	{ return m_pDefaultChannel; }

//inline service func.
	DWORD GetTotalChannelNum()		{ return m_dwTotalChannel; }


};

#endif // !defined(AFX_CHANNELMANAGER_H__28137EA9_F71E_4F27_B55A_B1EF47A8E644__INCLUDED_)
