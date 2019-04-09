#include "StdAfx.h"
#include "PartyMatchingDlg.h"
#include "WindowIDEnum.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "PartyManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "BattleSystem_Client.h"
#include "GameIn.h"
#include "cIMEex.h"

CPartyMatchingDlg::CPartyMatchingDlg()
{
	m_pLimitLevelEdit = NULL;
	m_pPartyList = NULL;
	m_dwSelectIdx = -1;
	m_dwReflashDelayTime = 0;
	m_PartyInfoTable.Initialize(MAX_PARTYMATCHING_INFO_NUM);	
}

CPartyMatchingDlg::~CPartyMatchingDlg()
{
	m_pPartyList->RemoveAll();
	this->RemovePartyInfoTable();
}

void CPartyMatchingDlg::Linking()
{
	m_pPartyList = (cListDialog*)GetWindowForID(PARTYMATCHING_LIST);	
	m_pLimitLevelEdit = (cEditBox*)GetWindowForID(PARTYMATCHING_LEVEL_EDIT);	

	m_pPartyList->SetShowSelect(TRUE);	
	m_pLimitLevelEdit->SetEditText("0");
	m_pLimitLevelEdit->SetValidCheck( VCM_NUMBER );
	m_pLimitLevelEdit->SetAlign( TXT_RIGHT );
}

void CPartyMatchingDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if(val == FALSE)
	{
		m_pPartyList->RemoveAll();
		this->RemovePartyInfoTable();
		m_dwSelectIdx = -1;
		m_pPartyList->SetCurSelectedRowIdx(m_dwSelectIdx);
		m_pLimitLevelEdit->SetEditText("0");
	}
}

BOOL CPartyMatchingDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case PARTYMATCHING_RANDOMIN_BTN:
		{
			PartyRandomSelect();
		}
		break;
	case PARTYMATCHING_REFLESH_BTN:
		{
			if( (m_dwReflashDelayTime != 0) && (m_dwReflashDelayTime > gCurTime) )
			{
				DWORD dwTime = ((m_dwReflashDelayTime - gCurTime) / 1000) + 1;
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1043),  dwTime);
				return TRUE;
			}

			PARTYMGR->RefleshPartyMatchingList(atoi(m_pLimitLevelEdit->GetEditText()));
			StartReflashDelayTime();
		}
		break;
	case PARTYMATCHING_JOIN_BTN:
		{
			if(m_dwSelectIdx == -1)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1738));
				return TRUE;
			}

			PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData(m_dwSelectIdx);
			if(pPartyInfo == NULL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
				return TRUE;
			}

			DWORD dwPartyIndex = pPartyInfo->PartyAddOption.dwPartyIndex;
			DWORD dwPartyMasterID = pPartyInfo->dwMemberID[0];
			MasterToPartyRequestSyn(dwPartyIndex, dwPartyMasterID);
		}
		break;
	}

	return TRUE;
}

DWORD CPartyMatchingDlg::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pPartyList->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{		
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pPartyList->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx;
			}
		}
	}	

	return we;
}

void CPartyMatchingDlg::RemovePartyInfoTable()
{
	m_PartyInfoTable.SetPositionHead();
	while (PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData())
	{
		SAFE_DELETE(pPartyInfo);
	}
	m_PartyInfoTable.RemoveAll();
}

void CPartyMatchingDlg::SetPartyMatchingList(MSG_PARTYMATCHING_INFO* pPartyListInfo)
{
	m_pPartyList->RemoveAll();
	this->RemovePartyInfoTable();

	WORD wCount = pPartyListInfo->wCount;	
	
	//최소레벨 값의 낮은 순으로 정렬
	PartyListSort(pPartyListInfo->PartyMatchingInfo, wCount);

	// 공개/비공개 정렬
	PARTY_MATCHINGLIST_INFO pTemp[MAX_PARTYMATCHING_INFO_NUM];
	int nTemp = 0;
	int nKey = 0;	

	for(int i = 0 ; i < wCount ; i++)
	{
		BOOL bPublic = pPartyListInfo->PartyMatchingInfo[i].PartyAddOption.bPublic;
		if(bPublic == TRUE)
		{
			PARTY_MATCHINGLIST_INFO* pPartyInfo = new PARTY_MATCHINGLIST_INFO;
			SetPartyInfo(pPartyInfo, pPartyListInfo->PartyMatchingInfo[i]);
			m_PartyInfoTable.Add(pPartyInfo, nKey);
			nKey++;
		}
		else
		{
			pTemp[nTemp] = pPartyListInfo->PartyMatchingInfo[i];
			nTemp++;
		}
	}
	for(int n = 0 ; n < nTemp ; n++)
	{
		PARTY_MATCHINGLIST_INFO* pPartyInfo = new PARTY_MATCHINGLIST_INFO;
		SetPartyInfo(pPartyInfo, pTemp[n]);
		m_PartyInfoTable.Add(pPartyInfo, nKey);		
		nKey++;
	}
	///////

	// 정렬된 데이터 문자 여백 정렬 및 ListDlg에 추가
	char szListStr[128];
	m_PartyInfoTable.SetPositionHead();
	for(DWORD i = 0 ; i < m_PartyInfoTable.GetDataNum() ; i++)	
	{
		PARTY_MATCHINGLIST_INFO* pPartyInfo = m_PartyInfoTable.GetData(i);		
		if(pPartyInfo == NULL)
			continue;

		int nLen = 0;

		//인덱스 번호 정렬
		char szIndexNum[7+1];
		char szTempStr[7+1];
		memset(szIndexNum, 0, sizeof(szIndexNum));
		memset(szTempStr, 0, sizeof(szTempStr));
		sprintf(szTempStr, "%d", i+1);
		nLen = strlen(szTempStr);
		int nCount = 0;
		for( int k = 0 ; k < 7 ; k++ )
		{
			if(k > (3-nLen) )	//자릿수마다 시작하는 칸이 틀리다.(최대 3자리)
			{
				if( (nLen > 0) && (nCount < nLen) )
				{
					szIndexNum[k] = szTempStr[nCount];
					nCount++;
				}
				else
				{
					szIndexNum[k] = 0x20;
				}
			}
			else
			{
				szIndexNum[k] = 0x20;				
			}

			szIndexNum[k+1] = NULL;
		}
		
		//방파 제목 정렬
		//목록 문자 정렬을 위해 기존 방파이름 크기보다 8정도 크게 줬다.
		char szPartyName[MAX_PARTY_NAME+8];
		memset(szPartyName, 0, sizeof(szPartyName));
		SafeStrCpy(szPartyName, pPartyInfo->PartyAddOption.szTheme, MAX_PARTY_NAME+8);
		nLen = strlen(szPartyName);
		for( int k = nLen ; k < MAX_PARTY_NAME+8-1 ; k++ )
		{
			szPartyName[k] = 0x20;
			szPartyName[k+1] = NULL;
		}

		//방파장 이름 정렬
		//목록 문자 정렬을 위해 기존 방파장 이름 크기보다 8정도 크게 줬다.
		char szPartyMasterName[MAX_NAME_LENGTH+8];
		memset(szPartyMasterName, 0, sizeof(szPartyMasterName));
		SafeStrCpy(szPartyMasterName, pPartyInfo->szMasterName, MAX_NAME_LENGTH+8);
		nLen = strlen(szPartyMasterName);
		for( int k = nLen ; k < MAX_NAME_LENGTH+8-1 ; k++ )
		{
			szPartyMasterName[k] = 0x20;
			szPartyMasterName[k+1] = NULL;
		}

		//레벨제한 정렬
		char szLimitLevel[10+1];		//레벨 제한
		memset(szLimitLevel, 0, sizeof(szLimitLevel));
		WORD wMinLevel = pPartyInfo->PartyAddOption.wMinLevel;		//최소레벨
		WORD wMaxLevel = pPartyInfo->PartyAddOption.wMaxLevel;		//최대레벨		
		sprintf(szLimitLevel, "%d ~ %d", wMinLevel, wMaxLevel);		
		nLen = strlen(szLimitLevel);
		for( int k = nLen ; k < 9 ; k++ )
		{
			szLimitLevel[k] = 0x20;
			szLimitLevel[k+1] = NULL;
		}

		//인원 정렬
		char szMemberNum[12+1];
		memset(szMemberNum, 0, sizeof(szMemberNum));
		WORD wMaxMemberNum = pPartyInfo->PartyAddOption.wLimitCount;	//최대인원
		WORD wCurMemberNum = 0;																//가입된 인원
		//가입된 방파원 수를 구한다
		wCurMemberNum = GetPartyMemberNum(pPartyInfo);		
		sprintf(szMemberNum, "%d / %d", wCurMemberNum, wMaxMemberNum);		

		sprintf(szListStr, "%s %s %s %s %s", szIndexNum, szPartyName, szPartyMasterName, szLimitLevel, szMemberNum);		
		
		DWORD dwColor = 0xffffffff;
		if(pPartyInfo->PartyAddOption.bPublic == FALSE)	//비공개는 색 다르게~
			dwColor = RGBA_MAKE(172,182,199,255);

		//본인의 가입된 파티는 녹색
		if( (pPartyInfo->PartyAddOption.dwPartyIndex == HERO->GetPartyIdx()) && (HERO->GetPartyIdx() != 0) )
			dwColor = RGBA_MAKE(0,255,0,255);
		
		m_pPartyList->AddItem(szListStr, dwColor);
	}	
}

void CPartyMatchingDlg::PartyListSort(PARTY_MATCHINGLIST_INFO* pPartyListInfo, WORD wMaxNum)
{	
	PARTY_MATCHINGLIST_INFO temp;

	for(int i = 0 ; i < wMaxNum ; i++)
	{
		for(int j = i+1 ; j < wMaxNum ; j++)
		{
			WORD wCurValue = pPartyListInfo[i].PartyAddOption.wMinLevel;
			WORD wNextValue = pPartyListInfo[j].PartyAddOption.wMinLevel;
			
			if(wCurValue > wNextValue)
			{
				temp = pPartyListInfo[i];
				pPartyListInfo[i] = pPartyListInfo[j];
				pPartyListInfo[j] = temp;
			}
		}
	}
}

WORD CPartyMatchingDlg::GetPartyMemberNum(PARTY_MATCHINGLIST_INFO* pPartyInfo)
{
	WORD wCurMemberNum = 0;
	//가입된 방파원 수를 구한다
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		if(pPartyInfo->dwMemberID[i] != 0)
			wCurMemberNum++;
	}

	return wCurMemberNum;
}

void CPartyMatchingDlg::PartyRandomSelect()
{	
	DWORD dwRandomIndexArray[MAX_PARTYMATCHING_INFO_NUM];	//랜덤 중복 검사용
	memset(&dwRandomIndexArray, -1, sizeof(dwRandomIndexArray));

	DWORD dwPartyNum = m_PartyInfoTable.GetDataNum();
	BOOL bCheck = FALSE;

	//랜덤수 목록을 미리 만들어 논다
	for(DWORD i = 0 ; i < dwPartyNum ; i++)
	{
		while(1)
		{
			bCheck = FALSE;

			//랜덤 수 뽑기
			DWORD dwRandomIndex = rand()%dwPartyNum;

			//중복 검사
			for(DWORD j = 0 ; j < i ; j++)
			{
				if(dwRandomIndexArray[j] == dwRandomIndex)
				{
					bCheck = TRUE;
					break;
				}
			}
			//중복되는 데이터가 없으면 목록에 
			if(bCheck == FALSE)
			{
				dwRandomIndexArray[i] = dwRandomIndex;
				break;
			}
		}
	}

	//목록중에 차례대로 조건에 맞은 파티가 있는지 검사 후 입장
	PARTY_MATCHINGLIST_INFO* pPartyInfo = NULL;
	bCheck = FALSE;

	for(DWORD i = 0 ; i < dwPartyNum ; i++)
	{
		pPartyInfo = m_PartyInfoTable.GetData(dwRandomIndexArray[i]);
        if(pPartyInfo == NULL)
			continue;

		if(pPartyInfo->PartyAddOption.bPublic == FALSE) //공개, 비공개 체크
			continue;
		if(pPartyInfo->PartyAddOption.wLimitCount <= GetPartyMemberNum(pPartyInfo))	//인원 체크
			continue;
		
		LEVELTYPE wLevel = HERO->GetLevel();
		if(pPartyInfo->PartyAddOption.wMinLevel > wLevel)	//최소레벨 체크
			continue;
		if(pPartyInfo->PartyAddOption.wMaxLevel < wLevel)	//최대레벨 체크
			continue;

        bCheck = TRUE;
		break;
	}

	if(bCheck == TRUE)	//신청할 수 있는 파티가 있으면 파티신청
	{
		DWORD dwPartyIndex = pPartyInfo->PartyAddOption.dwPartyIndex;
		DWORD dwPartyMasterID = pPartyInfo->dwMemberID[0];
				
		if(CheckPartyRequest(dwPartyIndex) == FALSE)
			return;

		this->MasterToPartyRequestSyn(dwPartyIndex, dwPartyMasterID);		
	}
	else	//신청할 수 있는 파티가 없으면 오류 메세지
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1734));

}

BOOL CPartyMatchingDlg::CheckPartyRequest(DWORD dwPartyIDX)
{
	if(PARTYMGR->IsProcessing() == TRUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
		return FALSE;
	}

	DWORD dwPartyNum = m_PartyInfoTable.GetDataNum();
	if(dwPartyNum == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
		return FALSE;
	}

	//방파가 있는지 검사
	if(HERO->GetPartyIdx() != 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1732) );
		return FALSE;
	}

	//신청자가 문토, 장파전쟁, 수련장, 비무, 공성전 상태일때 신청되지 않게 한다
	//단, 문파전쟁은 허락한다.
	if( (BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE) &&
		(BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_MUNPAFIELD) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG,  CHATMGR->GetChatMsg(1737));
		return FALSE;		
	}

	PARTY_MATCHINGLIST_INFO* pPartyInfo = this->GetPartyInfo(dwPartyIDX);
	if(pPartyInfo == NULL)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1725));
		return FALSE;
	}

	LEVELTYPE wHeroLevel = HERO->GetLevel();
    LEVELTYPE wLimitMinLevel = pPartyInfo->PartyAddOption.wMinLevel;
	LEVELTYPE wLimitMaxLevel = pPartyInfo->PartyAddOption.wMaxLevel;
	WORD wLimitCount = pPartyInfo->PartyAddOption.wLimitCount;
	BOOL bPublic = pPartyInfo->PartyAddOption.bPublic;

	//레벨체크
	if( (wLimitMinLevel > wHeroLevel) || (wLimitMaxLevel < wHeroLevel) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1731) );
		return FALSE;
	}

	WORD wCurMemberNum = GetPartyMemberNum(pPartyInfo);	
	if(wLimitCount < wCurMemberNum)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1733) );
		return FALSE;
	}

	if(bPublic == FALSE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1729) );
		return FALSE;
	}

	return TRUE;
}

void CPartyMatchingDlg::MasterToPartyRequestSyn(DWORD dwPartyIDX, DWORD dwMasterID)
{
	if( (dwPartyIDX == 0) || (dwMasterID == 0) )
		return;

	if(CheckPartyRequest(dwPartyIDX) == FALSE)
		return;

	MSG_NAME_DWORD2 msg;
	msg.Category	= MP_PARTY;
	msg.Protocol	= MP_PARTY_MASTERTOREQUEST_SYN;
	msg.dwObjectID	= HEROID;	
	msg.dwData1		= dwPartyIDX;
	msg.dwData2		= dwMasterID;
	SafeStrCpy(msg.Name, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	NETWORK->Send(&msg,sizeof(msg));

	PARTYMGR->SetIsProcessing(TRUE);	
}

void CPartyMatchingDlg::SetPartyInfo(PARTY_MATCHINGLIST_INFO* pPartyInfo, PARTY_MATCHINGLIST_INFO Info)
{
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; i++)
	{
		pPartyInfo->dwMemberID[i] = Info.dwMemberID[i];
	}

	SafeStrCpy(pPartyInfo->szMasterName, Info.szMasterName, MAX_NAME_LENGTH+1);

	pPartyInfo->PartyAddOption.bOption = Info.PartyAddOption.bOption;
	pPartyInfo->PartyAddOption.bPublic = Info.PartyAddOption.bPublic;
	pPartyInfo->PartyAddOption.dwPartyIndex = Info.PartyAddOption.dwPartyIndex;
	SafeStrCpy(pPartyInfo->PartyAddOption.szTheme, Info.PartyAddOption.szTheme, MAX_PARTY_NAME+1);
	pPartyInfo->PartyAddOption.wLimitCount = Info.PartyAddOption.wLimitCount;
	pPartyInfo->PartyAddOption.wMaxLevel = Info.PartyAddOption.wMaxLevel;
	pPartyInfo->PartyAddOption.wMinLevel = Info.PartyAddOption.wMinLevel;

	
}

void CPartyMatchingDlg::StartReflashDelayTime()
{
	m_dwReflashDelayTime = eREFLESH_DELAYTIME + gCurTime;
}

PARTY_MATCHINGLIST_INFO* CPartyMatchingDlg::GetPartyInfo(DWORD dwPartyIDX)
{
	PARTY_MATCHINGLIST_INFO* pPartyInfo = NULL;

	m_PartyInfoTable.SetPositionHead();
	while (pPartyInfo = m_PartyInfoTable.GetData())
	{
		if(dwPartyIDX == pPartyInfo->PartyAddOption.dwPartyIndex)
			return pPartyInfo;
	}

	return NULL;
}
