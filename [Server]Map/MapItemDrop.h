//----------------------------------------------------------------------------------------------------
//  MapItemDrop   version:  1.0   ·  date: 05/22/2008
//
//  Copyright (C) 2008 - All Rights Reserved
//----------------------------------------------------------------------------------------------------
///	Index	Stiner(MapDropItem)
///	@file	MapItemDrop.h
///	@author	이성민
///	@brief	맵 아이템 드랍 클래스
//----------------------------------------------------------------------------------------------------
#pragma once

#define	MAX_DROP_MAPITEM_PERCENT	1000000

#define MAPITEMDROP_OBJ	CMapItemDrop::GetInstance()

struct MAP_ITEM
{
#ifdef _DEBUG
	char	Name[MAX_ITEMNAME_LENGTH+1];	// 아이템 이름
#endif
	WORD	wItemIdx;						// 아이템 인덱스
	DWORD	dwDropPercent;					// 드랍 확률
	WORD	wDropCount;						// 현재까지의 드랍 횟수
	WORD	wMaxDropCount;					// 드랍 제한 횟수
};

struct MAP_DROP_ITEM
{
	DWORD		wChannel;							// 채널번호
	WORD		wMaxDropNum;						// 최대 드랍 개수
	WORD		wNumDropItem;						// 드랍되는 아이템 종류 수
	DWORD		dwTotalSumDropPercent;				// 드랍률 총 합
	MAP_ITEM	arrMapItems[MAX_DROPITEM_NUM];		// 드랍되는 아이템 종류
};

class CMapItemDrop
{
	BOOL	m_bLoadSucceed;	// 스크립트 로딩 성공 여부
	BOOL	m_bInited;		// 주간 초기화 여부

	// 초기 날짜
	WORD	m_wInitDay;		// Sun = 0, Mon, Tue, Wed, Thu, Fri, Sat
	WORD	m_wInitHour;

	CYHHashTable<MAP_DROP_ITEM>	m_MapItemDropTable;

	// 지난번 초기화 했던 날짜
	SYSTEMTIME	m_PrevInitedTime;

protected:
	BOOL	LoadMapDropItemList();
	void	ClearMapDropItemList();
	BOOL	LoadInitedTimeFile();

	MAP_DROP_ITEM*	GetMapDropItem(DWORD wChannel);

	MAP_ITEM*	GetMapItem(WORD wChannel, WORD wItemIdx);

	void	DropItem(CPlayer* pPlayer, MAP_ITEM* pMapItem, WORD MonsterKind);


	void	SetPrevInitedTime(SYSTEMTIME* sTime);
	SYSTEMTIME*	GetPrevInitedTime();

	void	InitItemDropInfo();

	void	InitItemDropInfoToDB();
	void	UpdateItemDropInfoToDB( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount );
	void	LogMapDropItem(WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount);
	void	GetDropItemInfoFromDB();

public:
	GETINSTANCE(CMapItemDrop);

	CMapItemDrop(void);
	~CMapItemDrop(void);

	void	Init();
	void	Process();

	void	CalculateDropRate(CPlayer* pPlayer, WORD MonsterKind);

	void	SetItemDropInfoByDB(WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount);
};
