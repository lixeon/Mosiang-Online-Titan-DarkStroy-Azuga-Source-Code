//----------------------------------------------------------------------------------------------------
//  StallFindDlg   version:  1.0   ·  date: 03/31/2008
//
//  Copyright (C) 2008 - All Rights Reserved
//----------------------------------------------------------------------------------------------------
///	Index	Stiner(8)
///	@file	StallFindDlg.h
///	@author	이성민
///	@brief	노점 검색을 위한 다이얼로그
//----------------------------------------------------------------------------------------------------
#pragma once

#include "./Interface/cDialog.h"
#include "GameResourceStruct.h"

#define SEARCH_DELAY		3000	///< 검색 딜레이 시간
#define	ITEMVIEW_DELAY		1000	///< 아이템 뷰 딜레이
#define MAX_RESULT_PAGE		5		///< 최대 페이지
#define MAX_LINE_PER_PAGE	6		///< 최대 줄 수

class cListDialog;
class cEditBox;
class cButton;
class cPushupButton;
class cComboBox;
class cStatic;

class CMouse;
class CExchangeItem;

/// @class	노점 검색 다이얼로그 클래스
class CStallFindDlg : public cDialog
{
	/// @brief 아이템 대/소분류 정보 구조체
	struct TItemInfo
	{
		int		Type;		///< 아이템 대분류
		int		DetailType;	///< 아이템 소분류
		DWORD	ItemIdx;	///< 아이템 인덱스
	};

	/// @brief 아이템 대분류 종류
	enum ITEM_TYPE
	{
		WEAPON,				///< 무기
		CLOTHES,			///< 의복
		ACCESSORY,			///< 장신구
		POTION,				///< 회복약
		MATERIAL,			///< 재료
		ETC,				///< 기타
		ITEM_MALL,			///< 아이템 몰
		TITAN_ITEM,			///< 타이탄 아이템
		ITEM_TYPE_COUNT,	///< 타입 최대 개수
	};

	BOOL		m_bSearchedAll;			///< 전체검색이였는지 체크
	DWORD		m_dwSearchType;			///< 검색 타입

	cPtrList	m_ptrItemInfo;			///< 아이템 정보

	int						m_nStallCount;						///< 검색된 노점상 수
	STREETSTALL_PRICE_INFO	m_arrStallInfo[MAX_STALLITEM_NUM];	///< 노점상 정보

	cComboBox*		m_pItemTypeCombo;							///< 아이템 대분류 타입 콤보
	cComboBox*		m_arrItemDetailTypeCombo[ITEM_TYPE_COUNT];	///< 아이템 소분류 타입 콤보

	cListDialog*	m_pItemList;		///< 아이템 리스트
	cListDialog*	m_pClassList;		///< 등급리스트

	cPushupButton*	m_pSellModeRadioBtn;	///< 판매 검색 라디오버튼
	cPushupButton*	m_pBuyModeRadioBtn;		///< 구매 검색 라디오버튼

	cStatic*		m_pNameStatic;		///< 노점상 리스트 케릭터명 제목
	cStatic*		m_pPriceStatic;		///< 노점상 리스트 케릭터명 제목

	cListDialog*	m_pStallList;					///< 노점상 리스트
	cPushupButton*	m_parrPageBtn[MAX_RESULT_PAGE];	///< 페이지 버튼
	cButton*		m_parrPageUpDownBtn[2];			///< 페이지 업/다운 버튼(0:업/1:다운)

	int	m_nBasePage;				///< 현재의 기본 페이지
	int	m_nMaxPage;					///< 전체 페이지
	int	m_nCurrentPage;				///< 현재 노점상 목록 페이지

	int m_nItemType;				///< 선택된 대분류 아이템 타입
	int m_nItemDetailType;			///< 선택된 소분류 아이템 타입

	int	m_nSelectedItemListIdx;		///< 선택된 아이템
	int	m_nSelectedClassListIdx;	///< 선택된 클래스
	int	m_nSelectedStallListIdx;	///< 선택된 노점상

	DWORD	m_dwSelectedObjectIndex;	///< 노점 선택한 주인의 인덱스 값
	DWORD	m_dwPrevSelectedType;		///< 이전 선택한 검색 타입

	/// @brief	아이템 리스트 로딩
	/// @return	void
	void	LoadItemList();

	/// @brief	아이템 리스트 갱신
	/// @return	void
	void	UpdateItemList();

	/// @brief	상점 리스트 갱신
	/// @return	void
	void	UpdateStallList();

	/// @brief	상점 정렬
	/// @return	void
	/// @param	flag	- TRUE:오름차순, FALSE:내림차순
	void	SortStallList(BOOL flag);

	/// @brief	상점 리스트 페이지 설정
	/// @return	void
	/// @param	index	- 0 ~ 4의 값
	void	SetPage(int index);

	/// @brief	기본 페이지 설정
	/// @return	void
	/// @param	bNext	- TRUE : PageUp / FALSE : PageDown
	void	SetBasePage(BOOL bNext);

	/// @brief	시간 딜레이 첵크
	/// @return	BOOL	- TRUE : dwDelayTime 이후 / FALSE : dwDelayTime 이전
	/// @param	dwDelayTime	- 시간 간격(밀리세컨드 단위)
	/// @param	nID	- 딜레이 체크 ID, ID별로 따로 체크가 이루어짐 0~4까지 가능
	BOOL	CheckDelay(DWORD dwDelayTime, int nID);


public:
	CStallFindDlg(void);
	~CStallFindDlg(void);

	/// @brief	컴포넌트 등록
	/// @return	void
	void	Linking();

	/// @brief	검색 타입 설정
	/// @return	void
	/// @param	val		- eSK_SELL:구입노점 / eSK_BUY:판매노점
	void	SetSearchType(DWORD val);

	/// @brief	노점 검색 결과 정보 세팅
	/// @return	void
	/// @param	pStallInfo	- 노점상 정보 구조체의 포인터
	void	SetStallPriceInfo(SEND_STREETSTALL_INFO * pStallInfo);

	/// @brief	아이템 보기 패킷 송신
	/// @return	void
	void	SendItemViewMsg();

	/// @brief	선택된 노점상의 주인 인덱스 리턴
	/// @return	DWORD	- 노점상 주인 인덱스(ID값)
	inline DWORD	GetSelectedObjectIndex()	{ return m_dwSelectedObjectIndex; };

	/// @brief	선택된 검색 타입 리턴
	/// @return	DWORD	- eSK_BUY / eSK_SELL 값으로 리턴됨
	inline DWORD	GetPrevSelectedType()		{ return m_dwPrevSelectedType; };

	/// @brief	선택 되었던 노점상의 주인 인덱스 세팅
	/// @return	void
	/// @param	dwIndex	- 설정할 인덱스 값
	inline void		SetSelectedObjectIndex(DWORD dwIndex)	{ m_dwSelectedObjectIndex = dwIndex; };

	/// @brief	선택 되었던 검색 타입 세팅
	/// @return	void
	/// @param	dwType	- eSK_BUY / eSK_SELL
	inline void		SetPrevSelectedType(DWORD dwType)		{ m_dwPrevSelectedType = dwType; };

	/// @brief	다이얼로그의 이벤트 핸들러 \n 이 메서드에서 각각의 이벤트 처리 메서드(On메서드)를 호출
	/// @return	void
	/// @param	lId	- 이벤트 발생한 ID값
	/// @param	p	- 이벤트 발생한 객체의 부모 객체 포인터
	/// @param	we	- 이벤트 값
	void	OnActionEvent(LONG lId, void * p, DWORD we);

	/// @brief	다이얼로그 활성/비활성
	/// @return	void
	/// @param	val	- TRUE:활성 / FALSE:비활성
	virtual void	SetActive(BOOL val);

	/// @brief	이벤트 수신 / 처리
	/// @return	DWORD	- 이벤트 값
	/// @param	mouseInfo	- 마우스 컨르롤에 대한 정보
	virtual DWORD	ActionEvent(CMouse * mouseInfo);

protected:
	//--- 이벤트 처리 ---//
	void	OnClickSearchBtn();										///< 검색 버튼
	void	OnClickSearchAllBtn();									///< 전체검색 버튼
	void	OnClickFindTypeBtn(LONG lId, void * p, DWORD we);		///< 검색 타입 라디오 버튼	void	OnClickPageBtn(LONG lId, void * p, DWORD we);			///< 페이지 버튼
	void	OnEventTypeCombo(LONG lId, void * p, DWORD we);			///< 대분류 콤보박스
	void	OnEventDetailTypeCombo(LONG lId, void * p, DWORD we);	///< 소분류 콤보박스
	void	OnEventItemList(LONG lId, void * p, DWORD we);			///< 아이템 리스트
	void	OnEventClassList(LONG lId, void * p, DWORD we);			///< 등급 리스트
	void	OnEventStallList(LONG lId, void * p, DWORD we);			///< 검색결과 리스트
	void	OnClickPageBtn(LONG lId);								///< 페이지 버튼
	void	OnClickPageUpDonwBtn(LONG lId);							///< 페이지 업/다운 버튼
	void	OnClose();												///< 닫기버튼
};
