#ifndef _PARTY_MATCHING_DLG_H
#define _PARTY_MATCHING_DLG_H

#include "interface/cDialog.h"
#include "interface/cEditBox.h"
#include "interface/cListDialog.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class CPartyMatchingDlg : public cDialog
{
private:
	enum
	{
		eREFLESH_DELAYTIME = 5000,	//방파 리스트 목록 갱신 딜레이 타임 5초
	};

private:	
	cEditBox* m_pLimitLevelEdit;	//레벨 조건
	cListDialog* m_pPartyList;	//방파 목록 리스트

	DWORD m_dwSelectIdx;
	DWORD m_dwReflashDelayTime;	//리스트 목록 갱신 딜레이 시간
	CYHHashTable<PARTY_MATCHINGLIST_INFO> m_PartyInfoTable;
	
public:
	CPartyMatchingDlg();
	virtual ~CPartyMatchingDlg();

	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse* mouseInfo);	
	virtual BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	
	void RemovePartyInfoTable();
	void SetPartyMatchingList(MSG_PARTYMATCHING_INFO* pPartyListInfo);	//2008. 5. 28. CBH - 파티정보 요청한 결과값 셋팅
	void PartyListSort(PARTY_MATCHINGLIST_INFO* pPartyListInfo, WORD wMaxNum);	//파티 데이터 정렬
	void PartyRandomSelect();		//파티 자동입장
	WORD GetPartyMemberNum(PARTY_MATCHINGLIST_INFO* pPartyInfo);		//파티 인원수 반환
	BOOL CheckPartyRequest(DWORD dwPartyIDX);		//파티신청 체크 사항
	void MasterToPartyRequestSyn(DWORD dwPartyIDX, DWORD dwMasterID);	//파티신청	
	void SetPartyInfo(PARTY_MATCHINGLIST_INFO* pPartyInfo, PARTY_MATCHINGLIST_INFO Info);	//파티 정보를 셋팅
	void StartReflashDelayTime();		//목록 갱신 딜레이 시간 초기화	
	PARTY_MATCHINGLIST_INFO* GetPartyInfo(DWORD dwPartyIDX);
};

#endif _PARTY_MATCHING_DLG_H
