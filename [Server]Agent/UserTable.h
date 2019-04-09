// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL DisconnectUser(DWORD dwConnectionIndex);

#include <AntiCpSvrFunc.h>
#include "Crypt.h"

#ifdef _NPROTECT_
class CCSAuth2;
#endif

struct aGAMEOPTION
{
	BYTE bNoFriend;
	BYTE bNoWhisper;

	aGAMEOPTION():bNoWhisper(0), bNoFriend(0) {}
};

struct CHARSELECTINFO
{
	DWORD dwCharacterID;
	LEVELTYPE Level;
	MAPTYPE MapNum;
	BYTE Gender;
	char CharacterName[MAX_NAME_LENGTH+1];
};

typedef struct tagUSERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwCharacterID;
	DWORD	dwUserID;
	BYTE	UserLevel;
	DWORD	dwMapServerConnectionIndex;
	WORD	wUserMapNum;
	CHARSELECTINFO SelectInfoArray[MAX_CHARACTER_NUM];

	DWORD	DistAuthKey;			// Distribute 서버에서 보내온 AuthKey 보관
//KES 연속 채팅 금지
	DWORD	dwLastChatTime;	

#ifdef _CHINA_LOCAL
//KES China Billing
	int		nRemainTime;
	BYTE	bBillType;
	DWORD	dwLastCheckRemainTime;
//
#endif

	WORD	wChannel;
	DWORD	dwUniqueConnectIdx;	// 서버에서의 고유 연결 번호
								// dwConnectionIdx와는 다름. 계속 증가함

	CCrypt crypto;
	aGAMEOPTION GameOption;

	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;
	
#ifdef _JAPAN_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _JAPAN_LOCAL_
#ifdef _HK_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _HK_LOCAL_
#ifdef _TL_LOCAL_
	DWORD	dwExtraUserSlot;
#endif	// _TL_LOCAL_
#ifdef _HACK_SHIELD_
	BYTE m_GUIDInfo[SIZEOF_GUIDREQINFO];
	BYTE m_Info[SIZEOF_REQINFO];
	unsigned long* m_pCrcInfo;
	BOOL m_bHSCheck;
#endif
#ifdef _NPROTECT_
	CCSAuth2 *m_pCSA;
	BOOL m_bCSA;
	int m_nCSAInit;
	DWORD	dwLastNProtectCheck;
	DWORD	m_dwHUC;
	BOOL bHackToolUser;
#endif
}USERINFO;

class CUserTable : public CYHHashTable<USERINFO>
{

	DWORD m_MaxUserCount;

	DWORD m_dwUserCount;


	DWORD m_addCount;
	DWORD m_removeCount;

public:
	CUserTable();
	virtual ~CUserTable();
	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }
	void Init(DWORD dwBucket);
	USERINFO * FindUser(DWORD dwKey);
	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount(){ return m_dwUserCount; }

	DWORD GetUserMaxCount(){return m_MaxUserCount;}
	void SetCalcMaxCount(DWORD CurCount);
	
	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
	DWORD GetRemoveCount() { return m_removeCount; }
};
  


extern CUserTable * g_pUserTable;							// key : connectionidx
extern CUserTable * g_pUserTableForUserID;					// key : UserID
extern CUserTable * g_pUserTableForObjectID;				// key : dwObjectID

#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
