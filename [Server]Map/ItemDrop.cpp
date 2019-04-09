// ItemDrop.cpp: implementation of the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemDrop.h"
#include "Player.h"
#include "time.h"
#include "ItemManager.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "PartyManager.h"
#include "Distribute_Random.h"
#include "Party.h"
#include "ChannelSystem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemDrop::CItemDrop()
{
	m_MonsterItemDropTable.Initialize(500);
}

CItemDrop::~CItemDrop()
{
	ClearMonsterDropItemList();
}

void CItemDrop::Init()
{
	srand((unsigned)time(NULL));
	LoadMonsterDropItemList();
}

/*//KES 이게 모지..? 주석처리닷!
void CItemDrop::ReloadDropItem(MONSTEREX_LIST * pMonsterExList)
{
	for(int n=0;n<eDROPITEMKIND_MAX;++n)
	{
		ASSERT(pMonsterExList->CurDropRate[n] == 0);
		pMonsterExList->CurDropRate[n] = pMonsterExList->DropRate[n];
	}
}
*/

int CItemDrop::DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer)
{
	DWORD CurTotalRate = 0;
	DWORD DropRate[eDROPITEMKIND_MAX];

	for(int i=0; i<eDROPITEMKIND_MAX; i++)
	{
		DropRate[i] = pMonsterExList->CurDropRate[i];

		if(i == eMONEY)
		{
			//DropRate[eMONEY] *= gMoneyRate;
			DropRate[eMONEY] = (DWORD)(DropRate[eMONEY]*gEventRate[eEvent_MoneyRate]);
		}
		else if(i >= eITEM1 && i <= eITEM3)
		{
			// DropRate[i] *= gItemRate;
			DropRate[i] = (DWORD)(DropRate[i]*gEventRate[eEvent_ItemRate]);

			// 
			if( pPlayer->GetPartyIdx() )
			{
				CParty* pParty = PARTYMGR->GetParty( pPlayer->GetPartyIdx() );
				if( pParty )
				{
					int count = pParty->GetMemberCountofMap( pPlayer->GetID() );
					if( count && gPartyEvent[ePartyEvent_ItemRate].Rate[count-1] )
						DropRate[i] = (DWORD)(DropRate[i]*gPartyEvent[ePartyEvent_ItemRate].Rate[count-1]);
				}
			}
		}

		CurTotalRate += DropRate[i];	
	}

	// RaMa - 06.02.08 ->사냥꾼의부적
	if( pPlayer->GetShopItemStats()->AddItemDrop )
	{
		for(i=eITEM1; i<=eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			//2007. 11. 20. CBH - 사냥꾼부적 관련 공식 수정
			//(1+(pPlayer->GetShopItemStats()->AddItemDrop*0.01f)) 이 값이 1.999999가 나오는데 DWORD로
			//변환하면서 뒤 소수점은 버린다. 해결책으로 0.001을 더해줘서 2값이 나오게 해서 2배가 되게 한다.
			DropRate[i] *= (DWORD)(1+(pPlayer->GetShopItemStats()->AddItemDrop*0.01f) + 0.001f);
			CurTotalRate += DropRate[i];
		}
	}

	//SW080515 채널별 세팅
	float fDropRate = CHANNELSYSTEM->GetDropRate( pPlayer->GetChannelID());
	if( fDropRate != 1.0f )
	{
		for(i=eITEM1; i<= eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] = DWORD(fDropRate * DropRate[i]);
			CurTotalRate += DropRate[i];
		}
	}

	// RaMa -04.11.24  ->드랍율수정
	if( CurTotalRate > MAX_DROP_ITEM_PERCENT )
	{
		int dwTemp = DropRate[eMONEY]+DropRate[eITEM1]+DropRate[eITEM2]+DropRate[eITEM3];

		if( dwTemp < MAX_DROP_ITEM_PERCENT )
		{
			DropRate[eNOITEM] = MAX_DROP_ITEM_PERCENT - dwTemp;
			CurTotalRate = MAX_DROP_ITEM_PERCENT;
		}
		else
		{
			DropRate[eNOITEM] = 0;
			CurTotalRate = dwTemp;
		}
	}

	//CurTotalRate == 0이면 안된다.
	if( CurTotalRate == 0 )
	{
//		char buf[128];
//		wsprintf( buf, "MonsterIdx:%d", pMonsterExList->MonsterKind );
//		ASSERTMSG(0, buf);
		ASSERT(0);
		return -1;
	}

	DWORD RandRate = random(1, CurTotalRate);
	DWORD CurMinRate=0,CurMaxRate=0;

	for( i=0; i<eDROPITEMKIND_MAX; i++)
	{
		CurMinRate = CurMaxRate;
		CurMaxRate += DropRate[i];

		if(RandRate > CurMinRate && RandRate <= CurMaxRate)
		{
			return i;
		}
	}
	ASSERT(0);

	char buf[128];
	wsprintf( buf, "MonsterIdx:%d, CurTotalRate:%d, RandRate:%d, CurMinRate:%d, CurMaxRate:%d,",
		pMonsterExList->MonsterKind, CurTotalRate, RandRate, CurMinRate, CurMaxRate );
	ASSERTMSG(0, buf);

	return -1;

}
MONEYTYPE CItemDrop::MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind)
{

//	ASSERT(pMob->GetObjectKind() & eObjectKind_Monster );
	
	// RaMa - 04.11.24   ->ShopItemOption의 ItemDrop비율 추가

	MONEYTYPE TotalMoney = 0; 

	for( int i = 0 ; i < pMonInfo->MaxDropItemNum ; ++i )
	{
		int eDropItemKind = DropItemKind(pMonInfo, pPlayer);

		switch(eDropItemKind)
		{
		case eNOITEM:
			{

			}
			break;
		case eMONEY:
			{
				//if( g_pServerSystem->GetMapNum() > TITANMAP_START_INDEX )
				if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
				{
					MONSTERDROP_TIMAP* pMonsterDropInfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( pMonInfo->Level );
					if( pMonsterDropInfo )
						TotalMoney = random( pMonsterDropInfo->stDropInfo[0].wMoneyMin, pMonsterDropInfo->stDropInfo[0].wMoneyMax );
				}
				else
					TotalMoney += MonsterDropMoney(pMonInfo);
			}
			break;
		case eITEM1:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum1, pPlayer);
			}
			break;
		case eITEM2:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum2, pPlayer);
			}
			break;
		case eITEM3:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum3, pPlayer);
			}
			break;
		default:
			{
				ASSERT(0);
			}
		}
	}

//	return 0;
	return TotalMoney;
}

MONEYTYPE CItemDrop::MonsterDropMoney(MONSTEREX_LIST * pMonsterExList)
{
	MONEYTYPE MinMoney = pMonsterExList->MinDropMoney;
	MONEYTYPE MaxMoney = pMonsterExList->MaxDropMoney;
//	ASSERT(MaxMoney >= MinMoney);

	MONEYTYPE ResultMoney = 0;
/*
	if(MinMoney == MaxMoney)
	{
		ResultMoney = MinMoney;
	}
	else
	{
		ResultMoney = (rand() % (MaxMoney - MinMoney)) + MinMoney;
	}
*/	
	if( MaxMoney > MinMoney )
	{
		ResultMoney = random( MinMoney, MaxMoney );
	}
	else
	{
		ResultMoney = MinMoney;
	}

	// RaMa -04.11.24   ->돈 얻는 액수
	// ResultMoney *= gGetMoney;
	ResultMoney = (MONEYTYPE)(ResultMoney*gEventRate[eEvent_GetMoney]);

	return ResultMoney;
}

void CItemDrop::MonsterDropItem(WORD MonsterKind, WORD wDropItemIdx, CPlayer* pPlayer)
{
	MONSTER_DROP_ITEM * pDropItem = GetMonsterItemDrop(wDropItemIdx);
	ASSERT(pDropItem);
	if(pDropItem == NULL)
		return;
	
	if( pDropItem->dwCurTotalRate == 0 )
	{
		// yh 아이템을 드랍하지 않는 경우(TotalRate 가 0)에 FALSE 리턴
		if(ReloadMonsterDropItem(pDropItem) == FALSE)
			return;
	}

	DWORD RandRate = random( 1, pDropItem->dwCurTotalRate );
	
	DWORD FromPercent	= 0;
	DWORD ToPercent		= 0;

	for(WORD i = 0 ; i < pDropItem->wTotalNum ; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pDropItem->dwCurMonsterDropItemPercent[i];
//		if(FromPercent <= RandRate && RandRate < ToPercent)
		if(FromPercent < RandRate && RandRate <= ToPercent)
		{
//			ASSERT(pDropItem->CurMonsterDropItemPercent[i]!=0);
//			ASSERT(pDropItem->wCurTotalRate!=0);
			--pDropItem->dwCurMonsterDropItemPercent[i];
			--pDropItem->dwCurTotalRate;


			if(pDropItem->MonsterDropItemPercent[i].wItemIdx)
			{
				WORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;

					DISTRIBUTERAND->ItemChangeAtLv(DropItemId);

				
				ITEMMGR->MonsterObtainItem(pPlayer, DropItemId, MonsterKind);
			}
			break;
		}
	}

}


BOOL CItemDrop::ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem)
{
	WORD HaveItem = pDropItem->wTotalNum;

	pDropItem->dwCurTotalRate = 0;
	for(WORD i = 0; i < HaveItem; ++i)
	{
		pDropItem->dwCurMonsterDropItemPercent[i] = pDropItem->MonsterDropItemPercent[i].dwDropPercent;
		pDropItem->dwCurTotalRate += pDropItem->dwCurMonsterDropItemPercent[i];
	}
	if(pDropItem->dwCurTotalRate == 0)
		return FALSE;

	return TRUE;
}


void CItemDrop::ClearMonsterDropItemList()
{
	// reset data
	m_MonsterItemDropTable.SetPositionHead();
	MONSTER_DROP_ITEM* pDropItem;
	while(pDropItem = m_MonsterItemDropTable.GetData())
	{
		delete pDropItem;
	}
	m_MonsterItemDropTable.RemoveAll();
}

BOOL CItemDrop::LoadMonsterDropItemList()
{
	ClearMonsterDropItemList();

	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
//	sprintf(filename,"./Resource/MonsterDropItemList.bin");	
	sprintf(filename,"./Resource/Server/MonsterDropItemList.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;
#else
	sprintf(filename,"./Resource/MonsterDropItemList.txt");	
	if(!file.Init(filename, "rt"))
		return FALSE;
#endif

	while(1)
	{
		if(file.IsEOF())
			break;
		
		MONSTER_DROP_ITEM* pDropItem = new MONSTER_DROP_ITEM;
		memset(pDropItem, 0, sizeof(MONSTER_DROP_ITEM));
		pDropItem->wItemDropIndex = file.GetWord();// 읽기만 한다.

		WORD totalnum = 0;
		for(int idx=0; idx<MAX_DROPITEM_NUM; ++idx)
		{
#ifdef _DEBUG
			//file.GetString(pDropItem->MonsterDropItemPercent[idx].Name);
			SafeStrCpy( pDropItem->MonsterDropItemPercent[idx].Name, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
			
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
//			if(pDropItem->MonsterDropItemPercent[idx].wItemIdx)
				++totalnum;
#else
			char szTempName[256];
			file.GetString(szTempName);
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
//			if(pDropItem->MonsterDropItemPercent[idx].wItemIdx)
				++totalnum;
#endif
		}
		
		pDropItem->wTotalNum = totalnum;
		pDropItem->dwCurTotalRate = 0;

		ASSERT(!m_MonsterItemDropTable.GetData(pDropItem->wItemDropIndex));
		m_MonsterItemDropTable.Add(pDropItem,pDropItem->wItemDropIndex);

		ReloadMonsterDropItem(pDropItem);
	}	
	
	file.Release();
	
	return TRUE;
}

MONSTER_DROP_ITEM* CItemDrop::GetMonsterItemDrop(WORD wItemDropIdx)
{
	return m_MonsterItemDropTable.GetData(wItemDropIdx);
}

//////////////////////////////////////////////////////////////////////////
// test
// old code 
/*


void CItemDrop::TestMonsterDropItem(WORD wDropItemListIdx)
{
	int Kind = wDropItemListIdx;
	
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);
	int HaveItem = pDropItem->m_MonsterDropTotalNum;	
	int MaxDropNum = 0;
	int DropNum = 0;
	int DropPercent = 0;
	int CurPercent = 0;
	int TempPercent = 0;
	
	int VarMaxPercent = pDropItem->m_MonsterMaxVarPercent; // 분모
	if(!VarMaxPercent)
		return;

	DropNum = 1;
	for(int i = 0; i < DropNum; i++)
	{
		CurPercent = 0;
		TempPercent = 0;
		DropPercent = rand() % VarMaxPercent; // 떨어뜨릴 확률
		if(!DpPercent)
			DropPercent = 1;
		for(int j=0; j<HaveItem; j++)
		{
			CurPercent = pDropItem->m_MonsterDropItemVarPercent[j].DropPercent;
			if(!CurPercent)	// 퍼센트가 0이면 Zero체크후 Reload
			{
				BOOL flag = ZeroCheckPercent(Kind);
				if(flag)
				{
					ReloadDropItemPercent(Kind); // 모두 0이면 재충전
					
					FILE* fp = fopen("itemdroptest.txt","a+");
					fprintf(fp,"ReloadDropItemPercent\n",pDropItem->m_MonsterDropItemVarPercent[j].Name,
						pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx);
					
					
					fclose(fp);
				}
			}
			else if(TempPercent<DropPercent && TempPercent+CurPercent>=DropPercent)
			{
				// 확률에 들어왔으므로 드롭
				MONSTER_ITEM tmp;
				WORD HuntItemNum = pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx;
				tmp = pDropItem->m_MonsterDropItemVarPercent[j];
				if(HuntItemNum == 0) 
					return;
				ASSERT(HuntItemNum != 52685);
				FILE* fp = fopen("itemdroptest.txt","a+");
				fprintf(fp,"item : %s (%d)\n",pDropItem->m_MonsterDropItemVarPercent[j].Name,
											pDropItem->m_MonsterDropItemVarPercent[j].ItemIdx);
				

				fclose(fp);
				
				CalculItemPercent(Kind, HaveItem, j);
			}
			TempPercent = TempPercent+CurPercent;
		}
		
	}
}
*/
//////////////////////////////////////////////////////////////////////////


/*
void CItemDrop::CalculItemPercent(WORD Kind, int HaveItem, int idx)
{
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);

	int VarPercent = 0;
	int tmpPercent = pDropItem->m_MonsterDropItemVarPercent[idx].DropPercent;
	int VarMaxPercent = pDropItem->m_MonsterMaxVarPercent; // 분모
	if(tmpPercent)
	{
		for(int i = 0; i < HaveItem; i++)
		{
			VarPercent = pDropItem->m_MonsterDropItemVarPercent[i].DropPercent;
			if(!VarPercent)
			{
				pDropItem->m_MonsterDropItemVarPercent[i].DropPercent = VarPercent;
				continue;
			}
			if(!VarMaxPercent)
				ReloadDropItemPercent(Kind); // 0이면 재충전
			if(i == idx)
			{
				pDropItem->m_MonsterDropItemVarPercent[i].DropPercent = VarPercent-1;
				pDropItem->m_MonsterMaxVarPercent = VarMaxPercent-1;
			}
		}
		// 모든 퍼센트가 0인지 검사
		BOOL flag = ZeroCheckPercent(Kind);
		if(flag)
			ReloadDropItemPercent(Kind); // 모두 0이면 재충전
	}
	else
	{
		// 여기에 걸리면 에러라네
		BOOL flag = ZeroCheckPercent(Kind);
		if(flag)
			ReloadDropItemPercent(Kind); // 모두 0이면 재충전
	}
}

BOOL CItemDrop::ZeroCheckPercent(WORD Kind)
{
	MONSTER_ITEM_DROP* pDropItem = GetMonsterItemDrop(Kind);

	int tmpPercent[MAX_DROPITEM_NUM];
	int AddPercent = 0;
	for(int i = 0; i < MAX_DROPITEM_NUM; i++)
	{
		tmpPercent[i] = pDropItem->m_MonsterDropItemVarPercent[i].DropPercent;
		AddPercent = AddPercent + tmpPercent[i];
	}

	if(AddPercent) // 퍼센트 누적의 합이 0이면 전부 0임
		return FALSE;  // Reload 하라고 지시
	else
		return TRUE;
}
*/
