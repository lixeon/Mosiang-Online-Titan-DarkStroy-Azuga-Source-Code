// PackedData.cpp: implementation of the CPackedData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PackedData.h"
#include "DataBlockManager.h"
#include "DataBlock.h"
#include "Network.h"
#include "ServerTable.h"
#include "Object.h"
#include "GridSystem.h"

#include "MemoryChecker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackedData::CPackedData()
{
	m_bInited = FALSE;
	m_bUsing = FALSE;
	m_MaxConnectionIndexPlusOne = 0;
	m_PackedIDNum = NULL;
	m_PackedIDs = NULL;
	m_pDataBlockManager = NULL;
}

CPackedData::~CPackedData()
{
	m_bUsing = FALSE;
	m_MaxConnectionIndexPlusOne = 0;
	SAFE_DELETE_ARRAY(m_PackedIDs);
	SAFE_DELETE_ARRAY(m_PackedIDNum);
	SAFE_DELETE(m_pDataBlockManager);
}

void CPackedData::Init(int MaxConnectionIndex,void* pData,WORD MsgLen)
{
	// ¼­¹ö³¢¸®ÀÇ ¿¬°áÀÌ agent 3 + monitoring server 1 + (0Àº ¾È¾¸)
	MaxConnectionIndex = 4;

	ASSERT(!m_bUsing);
	m_bUsing = TRUE;
	
	if(m_bInited == TRUE)
	{
		ASSERT(0);
		g_Console.Log(eLogDisplay,1,"PackedData Already Inited");
		FILE* fp = fopen("__PackedDebug.txt","a+");
		if(fp)
		{
			fprintf(fp,"PackedData Already Inited\n");
			fclose(fp);
		}
	}
	m_bInited = TRUE;

	m_SendData.Category = MP_PACKEDDATA;
	m_SendData.Protocol = MP_PACKEDDATA_NORMAL;
	m_SendData.wRealDataSize = MsgLen;
	memcpy(m_SendData.Data,pData,MsgLen);

	if(m_MaxConnectionIndexPlusOne < (DWORD)MaxConnectionIndex + 1)
	{
/*		
		// Memory Check
		stMemoryInfo* pMemoryInfo = MEMORYCHK->GetMemoryInfo( ePackedData_Init );
		if( pMemoryInfo && m_PackedIDs && m_PackedIDNum && m_pDataBlockManager )
		{
			pMemoryInfo->Decrease( _msize(m_PackedIDs)+_msize(m_PackedIDNum)+_msize(m_pDataBlockManager) );
		}
*/
		int lastmax = m_MaxConnectionIndexPlusOne;
		m_MaxConnectionIndexPlusOne = MaxConnectionIndex +1;
		
		SAFE_DELETE_ARRAY(m_PackedIDs);
		SAFE_DELETE_ARRAY(m_PackedIDNum);
		SAFE_DELETE(m_pDataBlockManager);
	
		m_pDataBlockManager = new CDataBlockManager;
		m_pDataBlockManager->Init(m_MaxConnectionIndexPlusOne,MAX_PACKEDDATA_SIZE);
		m_PackedIDs = new CDataBlock*[m_MaxConnectionIndexPlusOne];
		for(DWORD n=0;n<m_MaxConnectionIndexPlusOne;++n)
		{
			m_PackedIDs[n] = m_pDataBlockManager->GetEmptyDataBlock();
		}
		m_PackedIDNum = new WORD[m_MaxConnectionIndexPlusOne];
/*
		if( pMemoryInfo && m_PackedIDs && m_PackedIDNum && m_pDataBlockManager )
			pMemoryInfo->Increase( _msize( m_PackedIDs )+_msize( m_PackedIDNum )+_msize( m_pDataBlockManager ) );
*/
	}
	memset(m_PackedIDNum,0,sizeof(WORD)*m_MaxConnectionIndexPlusOne);
	for(DWORD n=0;n<m_MaxConnectionIndexPlusOne;++n)
	{
		m_PackedIDs[n]->Clear(TRUE);
	}
	m_bUsing = FALSE;
}

void CPackedData::AddObject(CObject* pObject)
{
	ASSERT(!m_bUsing);
	m_bUsing = TRUE;
	
	ASSERT(pObject->GetAgentNum() < m_MaxConnectionIndexPlusOne);
	DWORD id = pObject->GetID();
	m_PackedIDs[pObject->GetAgentNum()]->AddData(&id,sizeof(DWORD));
	++m_PackedIDNum[pObject->GetAgentNum()];
	
	m_bUsing = FALSE;
}

void CPackedData::Send()
{
	ASSERT(!m_bUsing);
	m_bUsing = TRUE;

	char* pIDData = &m_SendData.Data[m_SendData.wRealDataSize];
	int TotalDataLen;
	for(DWORD n=0;n<m_MaxConnectionIndexPlusOne;++n)
	{
		if(m_PackedIDNum[n] == 0)
			continue;

		m_SendData.wReceiverNum = m_PackedIDNum[n];
		memcpy(pIDData,m_PackedIDs[n]->GetData(),m_PackedIDs[n]->GetDataLen());
		TotalDataLen = m_SendData.wRealDataSize + m_PackedIDs[n]->GetDataLen() + sizeof(MSGBASE)+4;

		g_Network.Send2Server(n,(char*)&m_SendData,TotalDataLen);
	}
	
	m_bInited = FALSE;
	m_bUsing = FALSE;
}

void CPackedData::QuickSendExceptObjectSelf(CObject* pObject,MSGBASE* pMsg,int MsgLen)
{
	if(pObject->GetInited() == FALSE)
		return;

	Init(g_pServerTable->GetMaxServerConnectionIndex(),pMsg,MsgLen);
		
	g_pServerSystem->GetGridSystem()->AddGridToPackedDataWithoutOne(
		pObject->GetGridPosition(),	pObject->GetID(),PACKEDDATA_OBJ, pObject);
	Send();
}

void CPackedData::QuickSend(CObject* pObject,MSGBASE* pMsg,int MsgLen)
{
	if(pObject->GetInited() == FALSE)
		return;

	GridPosition* pGridPosition = pObject->GetGridPosition();
	if(pGridPosition->x == 65535 || pGridPosition->z == 65535)
	{
//		g_Console.Log(eLogDisplay,4,"Not Inited Object Send %s",pObject->GetObjectName());
		return;
	}

	Init(g_pServerTable->GetMaxServerConnectionIndex(),pMsg,MsgLen);

	g_pServerSystem->GetGridSystem()->AddGridToPackedDataWithoutOne(
		pGridPosition, 0,PACKEDDATA_OBJ, pObject);	
	Send();
}

void CPackedData::SendToMapServer(DWORD AgentNum,WORD ToMapNum,MSGBASE* pMsg,WORD MsgLen)
{
	static SEND_PACKED_TOMAPSERVER_DATA msg;
	msg.Category = MP_PACKEDDATA;
	msg.Protocol = MP_PACKEDDATA_TOMAPSERVER;
	msg.ToMapNum = ToMapNum;
	msg.wRealDataSize = MsgLen;
	memcpy(msg.Data,pMsg,MsgLen);
	g_Network.Send2Server(AgentNum,(char*)&msg,msg.GetMsgLength());
}

void CPackedData::SendToBroadCastMapServer(MSGBASE* pMsg, WORD MsgLen)
{
	DWORD AgentNum;
	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		AgentNum = pInfo->dwConnectionIndex;
		break;
	}
	static SEND_PACKED_TOMAPSERVER_DATA msg;
	msg.Category = MP_PACKEDDATA;
	msg.Protocol = MP_PACKEDDATA_TOBROADMAPSERVER;
	msg.ToMapNum = 0;
	msg.wRealDataSize = MsgLen;
	memcpy(msg.Data,pMsg,MsgLen);
	g_Network.Send2Server(AgentNum,(char*)&msg,msg.GetMsgLength());
}

DWORD CPackedData::GetLastSendIDNum()
{
	DWORD IDNum = 0;
	for(DWORD n=0;n<m_MaxConnectionIndexPlusOne;++n)
	{
		if(m_PackedIDNum[n] == 0)
			continue;

		IDNum += m_PackedIDNum[n];
	}
	return IDNum;
}
