// ChannelManager.cpp: implementation of the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelManager.h"
//#include "Front.h"
#include "Channel.h"
#include "MNPlayer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChannelManager::CChannelManager()
{
	m_pDefaultChannel = NULL;
}

CChannelManager::~CChannelManager()
{

}

void CChannelManager::Init( DWORD dwMaxChannel )
{
	m_mpChannel.Init( dwMaxChannel );
	m_htChannel.Initialize( dwMaxChannel );

	m_hIndexCreator = ICCreate();
	ICInitialize( m_hIndexCreator, dwMaxChannel );

	m_dwTotalChannel	= 0;
	m_dwMaxChannel		= dwMaxChannel;
	
//---Create Default-Channel(DC)
	CHANNEL_CREATE_INFO		cci;
	cci.cbChannelKind		= eCHNK_PUBLIC;
	cci.wMaxPlayer			= MAXPLAYER_IN_CHANNEL;
	strcpy( cci.strChannelTitle, DEFAULT_CHANNEL_NAME );
	m_pDefaultChannel		= CreateChannel( &cci );
//-----------------------------
	
//---Create Test Channels
	for( int i = 0 ; i < 100 ; ++i )
	{
		cci.cbChannelKind	= eCHNK_PUBLIC;
		cci.wMaxPlayer		= MAXPLAYER_IN_CHANNEL;
		wsprintf( cci.strChannelTitle, "테스트채널-%d", i );
		CreateChannel( &cci );
	}
//-----------------------
}

void CChannelManager::Release()
{
	DeleteAllChannel();
	ICRelease( m_hIndexCreator );
}

CChannel* CChannelManager::CreateChannel( CHANNEL_CREATE_INFO* pcci )
{
	if( m_dwTotalChannel >= m_dwMaxChannel )	return NULL;

	CChannel* pChannel		= m_mpChannel.Alloc();
	pcci->dwChannelIndex	= ICAllocIndex( m_hIndexCreator );
	pChannel->Create( pcci );
	
	m_htChannel.Add( pChannel, pcci->dwChannelIndex );
	++m_dwTotalChannel;

	return pChannel;
}

void CChannelManager::DeleteChannel( DWORD dwChannelIndex )
{
	CChannel* pChannel = m_htChannel.GetData( dwChannelIndex );
	m_mpChannel.Free( pChannel );
	ICFreeIndex( m_hIndexCreator, dwChannelIndex );

	m_htChannel.Remove( dwChannelIndex );
	--m_dwTotalChannel;
}

void CChannelManager::DeleteChannel( CChannel* pChannel )
{
	DWORD dwChannelIndex = pChannel->GetChannelIndex();
	pChannel->Release();
	m_mpChannel.Free( pChannel );
	ICFreeIndex( m_hIndexCreator, dwChannelIndex );

	m_htChannel.Remove( dwChannelIndex );
	--m_dwTotalChannel;
}

void CChannelManager::DeleteAllChannel()
{
	CChannel* pChannel;

	m_htChannel.SetPositionHead();
	while( pChannel = m_htChannel.GetData() )
	{
		DeleteChannel( pChannel );
	}
}

CChannel* CChannelManager::GetChannel( DWORD dwChannelIndex )
{
	return m_htChannel.GetData( dwChannelIndex );
}

void CChannelManager::SendMsg_ChannelList( CMNPlayer* pMNPlayer )
{
	MSG_CHANNEL_BASEINFOLIST	msgChannelList;

	msgChannelList.Category				= MP_MURIMNET;
	msgChannelList.Protocol				= MP_MURIMNET_CHNL_CHANNELINFOLIST;
	msgChannelList.dwTotalChannelNum	= m_dwTotalChannel;

	CChannel* pChannel;
	m_htChannel.SetPositionHead();
	for( int i = 0 ; pChannel = m_htChannel.GetData() ; ++i )
	{
		msgChannelList.ChannelInfo[i] = *pChannel->GetBaseInfo();
	}

	pMNPlayer->SendMsg( &msgChannelList, msgChannelList.GetMsgLength() );
}


