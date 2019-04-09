// cJackpotDialog.h: interface for the cJackpotDialog class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_)
//#define AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#include "./Interface/cDialog.h"
//
//#define NUMIMAGE_W 8
//#define NUMIMAGE_H 14
//
//#define BASIC_ANI_TIMELENGTH	2000
//#define BETWEEN_ANI_TIMELENGTH	500		//interval between Numbers
//#define NUM_CHANGE_TIMELENGTH	100		//interval
//
//class cButton;
//
//struct stNumImage
//{
//	cImage* pImage;
//	DWORD dwW,dwH;
//	stNumImage():pImage(NULL),dwW(0),dwH(0){}
//};
//
//struct stCipherNum
//{
//	stCipherNum():dwNumber(0),dwRealCipherNum(0),bIsAni(FALSE){}
//	DWORD dwNumber;// 자리수 숫자 (출력용)
//	DWORD dwRealCipherNum;// 자리수 숫자 (실제값)
//	bool bIsAni;// 숫자 애니 진행 중인지..(애니용)
//};
//
//class cJackpotDialog : public cDialog
//{
//	enum{DEFAULT_IMAGE = 99, NUM_0 = 0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_COUNT, CIPHER_NUM = 9};
//
//	stNumImage	m_stNumImage[NUM_COUNT];	// 숫자 이미지
//	VECTOR2		m_vPos[CIPHER_NUM];		// 숫자창 자리수 위치
//
//	cButton*	m_pBtnClose;			// 닫기 버튼
//
//	DWORD		m_dwTotalMoney;			// 총금액
//	DWORD		m_dwOldTotalMoney;			// 저장 금액(변화비교)
//	DWORD		m_dwTempMoney;			// @사족
//
//	stCipherNum m_stCipherNum[CIPHER_NUM];	// 각 자리별 출력할 숫자
//	DWORD		m_dwAniStartTime;		// 숫자 애니 시작 시간
//	DWORD		m_dwNumChangeTime;		// 자리 애니 변환 간격	// 숫자 갱신 간격
//	DWORD		m_dwIntervalAniTime;	// 자리간 애니 변환 간격 // 먼저 자리 애니가 끝나면 ..지나 다음 자리 애니 종료 하는 식~
//	DWORD		m_dwMaxCipher;			// 금액 총 자리수 (애니용)
//	DWORD		m_dwCipherCount;		// 자리수 (애니용)
//	bool		m_bIsAnimationing;			// 전체 애니 진행 중인지..(애니용)
//	bool		m_bDoSequenceAni;			// ForSequenceAni
//
//public:
//	cJackpotDialog();
//	virtual ~cJackpotDialog();
//
//	void InitNumImage();				// 숫자 이미지 등록
//	void ReleaseNumImage();				// 숫자 이미지 해제
//	void Linking();						// 버튼 연결
//	void SetNumImagePos();				// 숫자 이미지 출력 좌표 지정	
//	void ConvertCipherNum();			// 단위별 숫자
//	bool IsNumChanged();				// 숫자 변화 체크	// Check Num charnges
////	void DoNumChangeAni();				// 숫자 변환 애니	// Ticktime control
//	void DoAni();
//	void DoSequenceAni();
//	void Render();						// 출력
//	void Process();
//
//	void InitForAni();
//	void InitForSequenceAni();
//	void Init();
//	void Release();
//
//};
//
//#endif // !defined(AFX_CJACKPOTDIALOG_H__8AEBC3BE_AB04_48D0_BB0E_AA0800D15728__INCLUDED_)
//
///*
// *	AGENT 로부터 MSG를 받아 서버의 잭팟 정보를 출력한다
// *	
// */