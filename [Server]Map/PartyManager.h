#pragma once 
#define PARTYMGR CPartyManager::GetInstance()


class CPlayer;
class CParty;
struct PARTYMEMBER;


class CPartyManager {
	CYHHashTable<CParty> m_partyRoomHashTable;	

	CPartyManager();

public:
	GETINSTANCE(CPartyManager);

	virtual ~CPartyManager();

	CParty* GetParty(DWORD partyIDx); 
	
	CParty* RegistParty(DWORD PartyIDX);
	// 2008. 5. 21. CBH - 파티 생성 추가된 옵션 관련 수정 ////
	//void CreatePartyQuery(CPlayer * pMaster, BYTE bOption);
	//void CreatePartyResult(CPlayer * pMaster, DWORD PartyIDX, BYTE Option);
	void CreatePartyQuery(CPlayer * pMaster, PARTY_ADDOPTION* pAddOption);
	void CreatePartyResult(CPlayer * pMaster, PARTY_ADDOPTION* pAddOption);
	//////////////////////////////////////////////////////////

	void AddMember(DWORD PartyID, DWORD TargetPlayerID);
	void AddMemberResult(CParty* pParty, CPlayer* pNewMember);
	void AddMemberNack(DWORD PartyID, DWORD TargetPlayerID);
	void DelMember(DWORD PlayerID,DWORD PartyID);
	void DelMemberSyn(DWORD PlayerID, DWORD PartyID);
	void RemoveParty(DWORD DeletedPlayerID, DWORD PartyID);
	void BanParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID);
	void ChangeMasterParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID);
	void DoChangeMasterParty(CParty* pParty, DWORD TargetID);
	void BreakupParty(DWORD PartyID,DWORD PlayerID);

	void UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin);
	void UserLogOut(CPlayer* pPlayer);

	void AddPartyInvite(DWORD MasterID, DWORD TargetID);

	// 유저가 로그인 했을때 파티가 없으면 DB에서 파티정보를 얻어왔을때 호출하는 함수
	//2008. 5. 22. CBH - 방파 추가된 옵션 관련 수정
	//void UserLogInAfterDBQuery(DWORD CharacterID,DWORD PartyIdx,PARTYMEMBER* PartyInfo, BYTE Option);
	void UserLogInAfterDBQuery(DWORD CharacterID,DWORD PartyIdx,PARTYMEMBER* PartyInfo, PARTY_ADDOPTION AddOption);

	void NotifyChangesOtherMapServer(DWORD TargetPlayerID,CParty* pParty, BYTE Protocol, DWORD PartyIDX, LEVELTYPE Level=0);

	//void NotifyCreateParty(DWORD PartyIDX);
	void NotifyCreateParty(PARTY_INFO* pPartyInfo);

	void NotifyAddParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyBanParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyChangeMasterParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyBreakupParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyDelParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyDelSyn(MSG_DWORD2* pmsg);
	void NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyUserLogOut(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg);
	void NotifyMemberLoginMsg(DWORD PartyID, DWORD PlayerID);
	
	void SendErrMsg(DWORD PlayerID, int ErrKind, BYTE Protocol);
	BOOL SendPartyMsg(DWORD PartyID, MSGBASE* pmsg, int msglen);
	void SetTacticObjectID(DWORD PartyID,DWORD TacticObjectID);
	
	void MemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl);
	void DoMemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl);
	
	void SendObtainItemMsg(CPlayer* pPlayer, WORD ObtainIdx);

	void MasterToPartyRequest(CPlayer* pPlayer, MSG_NAME_DWORD2* pMsg);	// 2008. 5. 23. CBH - 방파장에게 방파 신청 ////
	void MasterToPartyRequestErr(DWORD dwPlayerID, DWORD dwErrIndex);	//2008. 5. 23. CBH - 방파 신청 실패	
	void NotifyPartyInfoSyn(DWORD dwPartyID);		//2008. 5. 27. CBH - 해당 방파의 정보를 모든 맵서버에 보내는 함수
	void NotifyPartyInfo(PARTY_INFO* pPartyInfo);		//2008. 5. 27. CBH - 파티 정보를 셋팅한다.
	void PartyMatchingInfo(CPlayer* pPlayer, MSG_WORD* pMsg);		//2008. 5. 28. CBH - 방파매칭 리스트 정보
	void Process();		//2008. 6. 4. CBH - 방파신청 카운트 처리
};

