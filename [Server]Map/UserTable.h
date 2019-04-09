// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "ServerSystem.h"
//#include <HashTable.h>



class CSkillArea;

#ifdef __MAPSERVER__

class CUserTable : public CYHHashTable<CObject>
{
	DWORD m_dwUserCount;
	DWORD m_dwMonsterCount;
	DWORD m_dwNpcCount;
	DWORD m_dwExtraCount;
public:
	CUserTable();
	virtual ~CUserTable();

	void Init(DWORD dwBucket);
	CObject * FindUser(DWORD dwKey);
	CObject * FindUserForName( char* strCharacterName );
	BOOL AddUser(CObject* pObject,DWORD dwKey);
	CObject * RemoveUser(DWORD dwKey);
	void RemoveAllUser();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 기존 GetTargetInRange 함수에 마지막 인자로 안전거리 추가
	void GetTargetInRange(VECTOR3* pPivotPos,float Radius,CTargetList* pTList,float SafeRange = 0);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	void GetTargetInArea(CSkillArea* pSkillArea,CTargetList* pTList);
	void GetTargetInDonutRangeInChannel(DWORD MonGridID, VECTOR3* pPivotPos,float MaxRadius, float MinRadius, CObject* rtpObject);

	
	DWORD GetUserCount(){ return m_dwUserCount; }
	DWORD GetMonsterCount(){ return m_dwMonsterCount; }
	DWORD GetNpcCount(){ return m_dwNpcCount; }
	DWORD GetExtraCount(){ return m_dwExtraCount; }

	void SetPositionUserHead() {  CYHHashTable<CObject>::SetPositionHead(); }
	CObject * GetUserData() { return CYHHashTable<CObject>::GetData(); }

};
#else

BOOL DisconnectUser(DWORD dwConnectionIndex);
BOOL DisconnectUser( DWORD dwConnectionIndex, int flag );

#include "Crypt.h"

struct aGAMEOPTION
{
	BYTE bNoFriend;
	BYTE bNoWhisper;

	aGAMEOPTION():bNoWhisper(0), bNoFriend(0) {}
};

typedef struct tagUSERINFO
{
	DWORD dwConnectionIndex;
	DWORD dwCharacterID;
	DWORD dwUserID;
	BYTE UserLevel;
	DWORD dwMapServerConnectionIndex;
	WORD  wUserMapNum;
	DWORD dwCharacterIDS[4];	// 유저가 가진 캐릭번호를 비교하기위함
	BYTE bLoginMapNum[4];

#ifdef _AGENTSERVER
	DWORD DistAuthKey;			// Distribute 서버에서 보내온 AuthKey 보관
#else
	DWORD State;
	BOOL bRecvDistOut;
#endif
	
#ifdef _NETWORKTEST
	DWORD dwStartTrafficTime;
	DWORD dwTrafficForUserSend[MP_MAX];
	DWORD dwTrafficForServerSend[MP_MAX];
	DWORD dwPacketForUserSend[MP_MAX];
	DWORD dwPacketForServerSend[MP_MAX];
	void AddPacketForUser(DWORD c, DWORD size)
	{
		dwTrafficForUserSend[c] += size;
		++dwPacketForUserSend[c];
	}
	void AddPacketForServer(DWORD c, DWORD size)
	{
		dwTrafficForServerSend[c] += size;
		++dwPacketForServerSend[c];
	}
#endif

	DWORD dwUniqueConnectIdx;	// 서버에서의 고유 연결 번호
								// dwConnectionIdx와는 다름. 계속 증가함

	CCrypt crypto;
	BYTE CheckSum;

	aGAMEOPTION GameOption;

	BOOL CheckCorrectCheckSum(BYTE CS)
	{
		if(CS != CheckSum)
			return FALSE;
		++CheckSum;
		return TRUE;
	}

	DWORD dwLastConnectionCheckTime;
	BOOL m_ConnectionCheckFailed;
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
#endif
  


extern CUserTable * g_pUserTable;							// key : connectionidx
#ifdef __AGENTSERVER__
extern CUserTable * g_pUserTableForUserID;					// key : UserID
extern CUserTable * g_pUserTableForObjectID;				// key : dwObjectID
#endif
#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
