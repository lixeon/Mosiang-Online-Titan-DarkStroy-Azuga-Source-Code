// Party.h: interface for the CParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_)
#define AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Player.h"
class CDistributeWay;
class MONSTEREX_LIST;
template <class T> void swap(T& x, T& y) 
{ 
	T temp = x;
	x = y;
	y = temp;
}
struct PARTYMEMBER
{    
	PARTYMEMBER()	
	{
		MemberID = 0;
		Name[0] = 0;
		bLogged = FALSE;
		LifePercent = 0;
		ShieldPercent = 0;
		NaeRyukPercent = 0;
		Level = 0;
	}
	void SetMember(DWORD AddMemberID,char* strName,CPlayer* pPlayer, LEVELTYPE lvl)
	{
		MemberID = AddMemberID;
		//strcpy(this->Name,strName);
		SafeStrCpy(this->Name,strName, MAX_NAME_LENGTH+1);
		
		Level = lvl;
		if(pPlayer)
		{
			bLogged = TRUE;
			NaeRyukPercent = pPlayer->GetNaeRyukPercent();
			ShieldPercent = pPlayer->GetShieldPercent();
			LifePercent = pPlayer->GetLifePercent();
		}
		else
		{
			bLogged = FALSE;
			NaeRyukPercent = 0;
			ShieldPercent = 0;
			LifePercent = 0;
		}
	}
	void SetLevel(LEVELTYPE lvl)
	{
		Level = lvl;
	}

	char Name[MAX_NAME_LENGTH+1];
	DWORD MemberID;
	BOOL bLogged;
	BYTE LifePercent;
	BYTE ShieldPercent;
	BYTE NaeRyukPercent;
	LEVELTYPE Level;
};

class CParty
{
private:
	//2008. 6. 3. CBH - 파티 신청 결정 카운트 시간(10초)
	enum
	{
		eDICISION_TIME = 10000,
	};
	
	DWORD m_PartyIDx;
	PARTYMEMBER m_Member[MAX_PARTY_LISTNUM];
	DWORD m_TacticObjectID;
	BYTE m_Option;
	DWORD m_OldSendtime;	

	//2008. 5. 21. CBH - 파티생성 추가 옵션 관련 수정
	PARTY_ADDOPTION m_AddPotion;
	//2008. 6. 4. CBH - 파티 신청시 10초의 선택시간 처리
    DWORD m_dwRequestPlayerID; //파티 신청자 ID
	DWORD m_dwRequestProcessTime;	//파티 신청 결정 카운트 시간

	CDistributeWay* m_pDistributeWay;

	//2008. 5. 21. CBH - 파티생성 추가 옵션 관련 수정
	//CParty(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,BYTE Option);
	CParty(DWORD id,DWORD MasterID,char* strMasterName,CPlayer* pMaster,PARTY_ADDOPTION* pAddOption);
	~CParty();

	void SendPartyInfo(CPlayer* pToPlayer);
public:
	BOOL AddPartyMember(DWORD AddMemberID,char* Name,CPlayer* pPlayer,LEVELTYPE lvl);
	BOOL RemovePartyMember(DWORD MemberID);
	BOOL ChangeMaster(DWORD FromID,DWORD ToID);
	BOOL IsPartyMember(DWORD PlayerID);
	BOOL IsMemberLogIn(int n);

	void SetMaster(CPlayer* pMaster);
	void SetOption(BYTE Option);
	BYTE GetOption();
	

	void UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin);
	void UserLogOut(DWORD PlayerID);
	void SendMsgUserLogOut(DWORD PlayerID);
	
	void SendMsgLoginMemberInfo(SEND_PARTY_MEMBER_INFO* msg, DWORD GridID);

	void SendMsgToAll(MSGBASE* msg, int size);
	void SendMsgExceptOne(MSGBASE* msg, int size, DWORD PlayerID);

	DWORD GetPartyIdx()	{	return m_PartyIDx;	}
	DWORD GetMemberID(int n)	{	return m_Member[n].MemberID; }
	DWORD GetMasterID()			{ return m_Member[0].MemberID;	}
	char* GetMemberName(DWORD MemberID);

	DWORD GetFirstOnlineMemberID();

	void SetTacticObjectID(DWORD TacticObjID) { m_TacticObjectID = TacticObjID; 	}
	DWORD GetTacticObjectID() { return m_TacticObjectID;	}
	
	void BreakUp();

	void GetMemberInfo(int n,PARTY_MEMBER* pRtInfo);
	PARTYMEMBER* GetPartyMember()	{ return m_Member; }
	void NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void SendMemberLogInMsg(DWORD PlayerID);
	void NotifyMemberLogin(DWORD PlayerID);
	void SendPlayerInfoToOtherMembers(int n, DWORD GridID);
	int SetMemberInfo(DWORD MemberID, char* strName, CPlayer* pPlayer, LEVELTYPE lvl, BOOL bLog);
	
	void SetMasterChanging(BOOL val);
	BOOL IsMasterChanging(CPlayer* pPlayer, BYTE Protocol);
	void SendMsgExceptOneinChannel(MSGBASE* msg, int size, DWORD PlayerID, DWORD GridID);

	friend class CPartyManager;
	
	//level
	void SetMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	void SendMemberPos(DWORD MoverID, BASEMOVE_INFO* pMoveInfo);

	void SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pMemberInfo, LEVELTYPE MaxLevel );
	void SendItem(PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel);

	int	GetMemberCountofMap( DWORD dwPlayerID );
	
#ifdef _JAPAN_LOCAL_
	BOOL IsHelpPartyMember( DWORD dwPlayerID );
#endif

	DWORD GetMemberNum();

	//2008. 5. 21. CBH - 파티생성 추가 옵션 관련 수정
	void SetAddOption(PARTY_ADDOPTION* pAddOption);
	PARTY_ADDOPTION* GetAddOption();

	void SetRequestPlayerID(DWORD dwID);
	DWORD GetRequestPlayerID();

	void StartRequestProcessTime();
	void InitRequestTime();

	void Process();

private:
	BOOL m_MasterChanging;
};

#endif // !defined(AFX_PARTY_H__87E3B123_4418_45C7_B287_81730C428C79__INCLUDED_)
