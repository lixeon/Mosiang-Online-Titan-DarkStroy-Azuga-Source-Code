// RarenessManager.cpp: implementation of the RarenessManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RarenessManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(RarenessManager)

RarenessManager::RarenessManager()
{
	Init();
}

RarenessManager::~RarenessManager()
{
	Release();
}

void RarenessManager::Init()
{
	m_RareItemInfoTable.Initialize(64);
}

void RarenessManager::Release()
{
	ReleaseRareItemInfo();
}

bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
//	sprintf(FileName, "Resource/Item_RareOptionInfo.bin");
	sprintf(FileName, "Resource/Server/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;
#else
	sprintf(FileName, "Reaource/Item_RareOptionInfo.txt");
	if( !fp.Init( FileName, "rt" ) )
		return FALSE;
#endif

	DWORD dwItemKindIdx = 0;
	while( !fp.IsEOF() )
	{
		if(dwItemKindIdx == RareItemKindMAX)
			break;

		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line, 512);
			continue;
		}
		
		if( szBuf[0] == '#' )
		{
			m_RereItemInfo[dwItemKindIdx].GenGol_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].GenGol = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].MinChub = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].CheRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].SimMek = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Life_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Life = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].NaeRyuk = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].Shield_Min = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].Shield = fp.GetDword();
			m_RereItemInfo[dwItemKindIdx].PhyAttack_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyAttack = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense_Min = fp.GetWord();
			m_RereItemInfo[dwItemKindIdx].PhyDefense = fp.GetWord();
			for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
			{
			m_RereItemInfo[dwItemKindIdx].AttrAttack_Min.SetElement_Val( i, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrAttack.SetElement_Val( i, fp.GetFloat() );
			}
			for(int j = ATTR_FIRE; j <= ATTR_MAX; ++j )
			{
			m_RereItemInfo[dwItemKindIdx].AttrRegist_Min.SetElement_Val( j, fp.GetFloat() );
			m_RereItemInfo[dwItemKindIdx].AttrRegist.SetElement_Val( j, fp.GetFloat() );
			}

			dwItemKindIdx++;
		}

	}

	return TRUE;
}
/*
bool RarenessManager::LoadRareItemOptionInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	sprintf(FileName,"Resource/Item_RareOptionInfo.bin");
	if( !fp.Init( FileName, "rb" ) )
		return FALSE;
#else
	sprintf(FileName,"Resource/Item_RareOptionInfo.txt");
	if( !fp.Init( FileName, "rt" ) )
		return FALSE;
#endif		

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}
		strupr(szBuf);

		if( strcmp(szBuf, "$WEAPONS") == 0 )
		{
			//fp.GetLineX(line,512);
			for( int i = 0; i < WeaponKindMAX; ++i )
			{
				fp.GetString(szBuf);	//Name
				m_Weapon[i].BaseMin = fp.GetDword();
				m_Weapon[i].BaseMax = fp.GetDword();
				m_Weapon[i].AddMin = fp.GetDword();
				m_Weapon[i].AddMax = fp.GetDword();
			}
		}
		else if( strcmp(szBuf, "$PROTECTORS") == 0 )
		{
			for( int i = 0; i < ProtectorkindMAX; ++i )
			{
				fp.GetString(szBuf);
				m_Protector[i].BaseMin = fp.GetDword();
				m_Protector[i].BaseMax = fp.GetDword();
				m_Protector[i].AttribMin = fp.GetDword();
				m_Protector[i].AttribMax = fp.GetDword();
				m_Protector[i].PlusValue[LIFE] = fp.GetDword();
				m_Protector[i].PlusValue[NAERYUK] = fp.GetDword();
				m_Protector[i].PlusValue[SHIELD] = fp.GetDword();
			}
		}
		else if( strcmp(szBuf, "$ACCESSARIES") == 0 )
		{
			for( int i = 0; i < AccessaryKindMAX; ++i )
			{
				fp.GetString(szBuf);
				m_Accessary[i].BaseMin = fp.GetDword();
				m_Accessary[i].BaseMax = fp.GetDword();
			}
		}
	}

	return TRUE;
}*/

bool RarenessManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

#ifdef _FILE_BIN_
	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareItemInfo.txt");
	if( !fp.Init(FileName,"rt") ) return FALSE;
#endif

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}

//		strupr(szBuf);

		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}

	return TRUE;
}

bool RarenessManager::LoadRareValueProbList()
{
	CMHFile fp;

	char FileName[256];
	char szBuf[256] = {0,};

#ifdef _FILE_BIN_
//	sprintf(FileName, "Resource/Item_RareStatSetRate.bin");
	sprintf(FileName, "Resource/Server/Item_RareStatSetRate.bin");
	if(!fp.Init(FileName,"rb")) return FALSE;
#else
	sprintf(FileName, "Resource/Item_RareStatSetRate.txt");
	if(!fp.Init(FileName,"rt")) return FALSE;
#endif

	fp.GetLine(szBuf,256); //@

	DWORD Index = 1;
	while( !fp.IsEOF() )
	{
		if(Index == RARE_VALUE_PROB_LIST_MAX) break;
		
		Index	= fp.GetDword();	//1~100
		m_dwRareValueProb[Index - 1] = fp.GetDword();
	}

	return TRUE;
}

void RarenessManager::ReleaseRareItemInfo()
{
	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();
}

bool RarenessManager::GetRare( WORD ObtainItemIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo, CPlayer* pPlayer, BOOL bRare )
{
	if(ObtainItemIdx > BOUNDARY_INDEX_FOR_SHOPITEM) return FALSE;	//WeaponType 를 샾 아이템과 같이쓰고 있다.

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ObtainItemIdx );
	if( !pInfo ) return FALSE; //등록된 아이템이 아니면 리턴

	eITEM_KINDBIT eItemKind = GetItemKind( ObtainItemIdx );	//!인자 확인
	if( !(eItemKind & eEQUIP_ITEM) ) return FALSE;	//장착류 아이템이 아니면 리턴

	DWORD ItemIdx = ObtainItemIdx;
	//해당 아이템의 레어 생성 확률을 가져와 비교.
	if( eItemKind == eEQUIP_ITEM_WEAPON 
		|| eItemKind == eEQUIP_ITEM_DRESS
		|| eItemKind == eEQUIP_ITEM_HAT
		|| eItemKind == eEQUIP_ITEM_SHOES )
	{
		ItemIdx /= 10;
		ItemIdx *= 10;
	}	//악세사리를 제외한 +조합 아이템들의 리스트 인덱스는 하나로 통일. 정보 구함.

	sRareItemInfo* RareInfo = m_RareItemInfoTable.GetData(ItemIdx);

	if( !RareInfo ) return FALSE;
//!!!+
//	if( (rand()%10000) * 10000 + (rand()%10000 + 1) < RareInfo->RareProb )

	//BOOL baa = ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) <= (RareInfo->RareProb) );

	//SW061120 신규펫

	BOOL baa = FALSE;

	if( FALSE == bRare )
	{
		float addPob = 1.0f;	// 50%추가 -> 0.5f
		pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_RareProbUp, &addPob);
		baa = ( ( (DWORD)((rand()%10000) * 10000 + (rand()%10000 + 1)) )*addPob <= (RareInfo->RareProb) );
		//DWORD rnd = ((rand()%10000) * 10000 + (rand()%10000 + 1));
		//baa = ( DWORD(rnd*addPob) <= (RareInfo->RareProb) );
	}

	if( baa || bRare )	//레어다!!!
	{
		DWORD RareItemKind = 0;	//멤버 배열의 인자로 쓰인다.
		if( eItemKind == eEQUIP_ITEM_WEAPON )
		{
			RareItemKind = pInfo->WeaponType - 1;
		}
		else if( eItemKind == eEQUIP_ITEM_DRESS )			RareItemKind = eRI_DRESS;
		else if( eItemKind == eEQUIP_ITEM_HAT )				RareItemKind = eRI_HAT;
		else if( eItemKind == eEQUIP_ITEM_SHOES )			RareItemKind = eRI_SHOES;
		else if( eItemKind == eEQUIP_ITEM_RING )			RareItemKind = eRI_RING;
		else if( eItemKind == eEQUIP_ITEM_CAPE )			RareItemKind = eRI_CAPE;
		else if( eItemKind == eEQUIP_ITEM_NECKLACE )		RareItemKind = eRI_NECKLACE;
		else if( eItemKind == eEQUIP_ITEM_ARMLET )			RareItemKind = eRI_ARMLET;
		else if( eItemKind == eEQUIP_ITEM_BELT )			RareItemKind = eRI_BELT;

		if( m_RereItemInfo[RareItemKind].GenGol || m_RereItemInfo[RareItemKind].MinChub || m_RereItemInfo[RareItemKind].CheRyuk || m_RereItemInfo[RareItemKind].SimMek )	//현재 장신구 적용
		{
			switch( rand()%StatesKindMAX )
			{
			case eRI_GENGOL:
				pRareOptionInfo->GenGol = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol );
				break;
			case eRI_MINCHUB:
				pRareOptionInfo->MinChub = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub );
				break;
			case eRI_CHERYUK:
				pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk );
				break;
			case eRI_SIMMEK:
				pRareOptionInfo->SimMek = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek );
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{	//!!!SW050928 왜 이렇게 했지?
//			ASSERTMSG(0, "RareItem : Not All RareOption_StatesSet on Script");

			pRareOptionInfo->GenGol = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].GenGol_Min, (DWORD)m_RereItemInfo[RareItemKind].GenGol );
			pRareOptionInfo->MinChub = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].MinChub_Min, (DWORD)m_RereItemInfo[RareItemKind].MinChub );
			pRareOptionInfo->CheRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].CheRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].CheRyuk );
			pRareOptionInfo->SimMek = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].SimMek_Min, (DWORD)m_RereItemInfo[RareItemKind].SimMek );
		}

		if( eItemKind == eEQUIP_ITEM_DRESS || eItemKind == eEQUIP_ITEM_HAT || eItemKind == eEQUIP_ITEM_SHOES )	//현재 방어구 적용
		{
			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk );
			pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield );
		}
		else if( m_RereItemInfo[RareItemKind].Life && m_RereItemInfo[RareItemKind].NaeRyuk && m_RereItemInfo[RareItemKind].Shield )	//현재 무기 적용
		{
			switch( rand()%ValueKindMAX )
			{
			case eRI_LIFE:
				pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
				break;
			case eRI_NAERYUK:
				pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk );
				break;
			case eRI_SHIELD:
				pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield );
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
//			ASSERTMSG(0, "RareItem : Not All RareOption_AddValue on Script");

			pRareOptionInfo->Life = GetRareItemValue( m_RereItemInfo[RareItemKind].Life_Min, m_RereItemInfo[RareItemKind].Life );
			pRareOptionInfo->NaeRyuk = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk_Min, (DWORD)m_RereItemInfo[RareItemKind].NaeRyuk );
			pRareOptionInfo->Shield = GetRareItemValue( m_RereItemInfo[RareItemKind].Shield_Min, m_RereItemInfo[RareItemKind].Shield );
		}
		
		pRareOptionInfo->PhyAttack = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyAttack_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyAttack );
		pRareOptionInfo->PhyDefense = (WORD)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].PhyDefense_Min, (DWORD)m_RereItemInfo[RareItemKind].PhyDefense );

		WORD i = rand()%ATTR_MAX + 1;
		
		if(rand()%2)	//공격 or 방어
		{
			pRareOptionInfo->AttrAttack.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrAttack_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrAttack.GetElement_Val(i))) / 100.f );
		}
		else
		{
			pRareOptionInfo->AttrRegist.SetElement_Val( i, (float)GetRareItemValue( (DWORD)m_RereItemInfo[RareItemKind].AttrRegist_Min.GetElement_Val(i)
				, (DWORD)(m_RereItemInfo[RareItemKind].AttrRegist.GetElement_Val(i))) / 100.f );
		}

		return TRUE;
	}
	else
		return FALSE;

/*스크립 의존적으로 변경
		DWORD BaseMin = 0;
		DWORD BaseMax = 0;
		DWORD AddOpt = 0;
		int ProtectItemKind = 0;
		int AccessaryItemKind = 0;
		//해당 아이템의 종류에 맞는 레어 옵션 생성.
		switch(eItemKind)
		{
	//WEAPON
		case eEQUIP_ITEM_WEAPON:
			{
				ASSERT(pInfo->WeaponType != 0);	//(Script) 검1,권2,도3,창4,궁5,암기6
				BaseMin = m_Weapon[pInfo->WeaponType - 1].BaseMin;
				BaseMax = m_Weapon[pInfo->WeaponType - 1].BaseMax;
				//RareInfo 에서 Min, Max 값을 가져와 공식또는 확률 테이블에서 해당 값을 구해온다.
				RareInfo.PhyAttack = (WORD)GetRareItemValue(BaseMin, BaseMax);
				//생명력,내력,호신강기중 추가 옵션
				AddOpt = GetRareItemValue(m_Weapon[pInfo->WeaponType - 1].AddMin,
												m_Weapon[pInfo->WeaponType - 1].AddMax);
				switch(rand()%ValueKindMAX)
				{
				case LIFE:		{RareInfo.Life = AddOpt;}		break;
				case NAERYUK:	{RareInfo.NaeRyuk = (WORD)AddOpt;}	break;
				case SHIELD:	{RareInfo.Shield = AddOpt;}		break;
				default:
					ASSERT(0);
					break;
				}
			}
			break;
//			머리띠	: 0
//			무기류	: 1
//			옷		: 2
//			신발	: 3
//			반지	: 4
//			망토	: 6
//			목걸이	: 7
//			팔찌	: 8
//			허리띠	: 9
	//PROTECTOR
		case eEQUIP_ITEM_DRESS:
			{
				ASSERT(pInfo->EquipKind==2);	//(Script)
				ProtectItemKind = DRESS;
			}
		case eEQUIP_ITEM_HAT:
			{
				ASSERT(pInfo->EquipKind==0);
				ProtectItemKind = HAT;
			}
		case eEQUIP_ITEM_SHOES:
			{
				ASSERT(pInfo->EquipKind==3);
				ProtectItemKind = SHOES;
			}
			//물리방어 추가
			RareInfo.PhyDefense = (WORD)GetRareItemValue(m_Protector[ProtectItemKind].BaseMin,m_Protector[ProtectItemKind].BaseMax);
			//속성방어 추가
			AddOpt = GetRareItemValue(m_Protector[ProtectItemKind].AttribMin,m_Protector[ProtectItemKind].AttribMax);
			if( rand()%2 )
				RareInfo.AttrAttack.SetElement_Val(rand()%ATTR_MAX+1, (float)AddOpt);
			else
				RareInfo.AttrRegist.SetElement_Val(rand()%ATTR_MAX+1, (float)AddOpt);
			//생명력,내력,호신강기 추가
			RareInfo.Life = m_Protector[ProtectItemKind].PlusValue[LIFE];
			RareInfo.NaeRyuk = (WORD)m_Protector[ProtectItemKind].PlusValue[NAERYUK];
			RareInfo.Shield = m_Protector[ProtectItemKind].PlusValue[SHIELD];
			break;

	//ACCESSARY
		case eEQUIP_ITEM_RING:
			{
				ASSERT(pInfo->EquipKind==4);
				AccessaryItemKind = RING;
			}
		case eEQUIP_ITEM_CAPE:
			{
				ASSERT(pInfo->EquipKind==6);
				AccessaryItemKind = CAPE;
			}
		case eEQUIP_ITEM_NECKLACE:
			{
				ASSERT(pInfo->EquipKind==7);
				AccessaryItemKind = NECKLACE;
			}
		case eEQUIP_ITEM_ARMLET:
			{
				ASSERT(pInfo->EquipKind==8);
				AccessaryItemKind = ARMLET;
			}
		case eEQUIP_ITEM_BELT:
			{
				ASSERT(pInfo->EquipKind==9);
				AccessaryItemKind = BELT;
			}

			AddOpt = GetRareItemValue(m_Accessary[AccessaryItemKind].BaseMin,
										m_Accessary[AccessaryItemKind].BaseMax);
			switch(rand()%StatesKindMAX)
			{
			case GENGOL:	{RareInfo.GenGol = (WORD)AddOpt;}		break;
			case MINCHUB:	{RareInfo.MinChub = (WORD)AddOpt;}	break;
			case CHERYUK:	{RareInfo.CheRyuk = (WORD)AddOpt;}	break;
			case SIMMEK:	{RareInfo.SimMek = (WORD)AddOpt;}		break;
			default:
				ASSERT(0);
				break;
			}
			break;	//ACCESSARY

		default:
			ASSERT(0);
			break;	//eItemKind
		}*/


	//!DB에 저장
	
	
}

DWORD RarenessManager::GetRareItemValue(DWORD RndMin, DWORD RndMax)
{
	if( !RndMin || !RndMax ) return 0; //최대 최소 값중 하나라도 0 이면 리턴

	if( RndMin == RndMax ) return RndMax;	//고정 수치로 붙는 옵션은 스크립트 상에서 최소/최대 값 같게 세팅.

	ASSERTMSG(RndMax>RndMin, "Check RareOptionInfo File!");

	DWORD RndValue = ((rand()%10000) * 10000 + (rand()%10000 + 1));

	DWORD Index = 0;
	while(RndValue < m_dwRareValueProb[Index])
	{
		Index++;
	}

	//!임시
	DWORD XRndValue = (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
	return XRndValue;
	//return (DWORD)((RndMax - RndMin) * Index / RARE_VALUE_PROB_LIST_MAX ) + RndMin;
}
