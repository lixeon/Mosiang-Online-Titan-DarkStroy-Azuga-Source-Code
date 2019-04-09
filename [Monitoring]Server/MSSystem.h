// MSSystem.h: interface for the CMSSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSSYSTEM_H__88388F7A_0177_4524_AAE2_9C7E947CDC2C__INCLUDED_)
#define AFX_MSSYSTEM_H__88388F7A_0177_4524_AAE2_9C7E947CDC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMSSystem  
{
public:
	WORD m_wServerType;
	BOOL m_bInit;

public:
	CMSSystem();
	virtual ~CMSSystem();


	static void OnAcceptServer(DWORD dwConnectionIndex);
	
	static void OnDisconnectServer(DWORD dwConnectionIndex);

	static void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

	static void ReceivedMsgFromMS(DWORD dwConnectionIndex,char* pmsg,DWORD dwLength);
	static void ReceivedMsgFrom2Server(DWORD dwConnectionIndex,char* pmsg,DWORD dwLength);

	static void OnConnectMASSuccess(DWORD dwConnectionIndex, void* pVoid);
	static void OnConnectMASFail(void* pVoid);

	void Start(WORD wType);
	void End();

	static void Process();


	//mas only
	static void OnAcceptUser(DWORD dwConnectionIndex);
	static void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void OnDisconnectUser(DWORD dwConnectionIndex);

	// º¹±¸ TOOL only
	static void	ReceivedMsgFromRMTool(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void ConnentMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void UserMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void MunpaMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void GameLogMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void OperLogMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void StatisticsMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void AdminMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void CharacterMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
	static void ItemMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);


	// autopatch
	static void AutoPatchProcess();



};
extern CMSSystem * g_pMSSystem;


#endif // !defined(AFX_MSSYSTEM_H__88388F7A_0177_4524_AAE2_9C7E947CDC2C__INCLUDED_)
