// PackedData.h: interface for the CPackedData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKEDDATA_H__AB7ED393_2927_44DB_AE3C_D44693E1B3D4__INCLUDED_)
#define AFX_PACKEDDATA_H__AB7ED393_2927_44DB_AE3C_D44693E1B3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject;
class CDataBlock;
class CDataBlockManager;

#define PACKEDDATA_OBJ CPackedData::GetInstance()

/*
// taiyo gamestruct.h·Î ¿Å±è
//#define MAX_PACKEDDATA_SIZE 4096
struct SEND_PACKED_DATA : public MSGBASE
{
	WORD wRealDataSize;
	WORD wReceiverNum;
	char Data[MAX_PACKEDDATA_SIZE];
};
*/

class CPackedData  
{
	SEND_PACKED_DATA m_SendData;
	BOOL m_bInited;

	BOOL m_bUsing;

	DWORD m_MaxConnectionIndexPlusOne;		// ¹è¿­ÀÌ 0ºÎÅÍ ½ÃÀÛÇÏ±â ¶§¹®¿¡ +1À» ÇØÁØ´Ù.
	WORD* m_PackedIDNum;
	CDataBlock** m_PackedIDs;
	
	CDataBlockManager* m_pDataBlockManager;

	void Init(int MaxConnectionIndex,void* pData,WORD MsgLen);
public:
	GETINSTANCE(CPackedData)

	CPackedData();
	virtual ~CPackedData();
	
	void AddObject(CObject* pObject);
	void Send();


	void QuickSend(CObject* pObject,MSGBASE* pMsg,int MsgLen);
	void QuickSendExceptObjectSelf(CObject* pObject,MSGBASE* pMsg,int MsgLen);


	void SendToMapServer(DWORD AgentNum,MAPTYPE ToMapNum,MSGBASE* pMsg,WORD MsgLen);
	void SendToBroadCastMapServer(MSGBASE* pMsg, WORD MsgLen);
	
	MSGBASE* GetMsg()	{	return (MSGBASE*)m_SendData.Data;	}
	int GetMsgLen()		{	return m_SendData.wRealDataSize;	}

	friend class CGridSystem;
	friend class CGeneralGridTable;

	
	DWORD GetLastSendIDNum();
};

#endif // !defined(AFX_PACKEDDATA_H__AB7ED393_2927_44DB_AE3C_D44693E1B3D4__INCLUDED_)
